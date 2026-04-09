/**
 * @file main.c
 * @brief QL25 AMP 从核应用（Core 0 数据面）
 *
 * 任务架构：
 *   task_ipc_service  (prio 4) — IPC 命令接收/响应 + 事件上报
 *   task_heartbeat    (prio 3) — 心跳 tick + 快照更新
 *   task_data_plane   (prio 2) — 数据面转发（占位）
 *
 * 约束：
 *   - 全静态分配（configSUPPORT_DYNAMIC_ALLOCATION = 0）
 *   - 整个 bin 加载到 ILM，<= 48KB
 *   - 不引用 printf/sprintf 等重量级函数（节省 Flash）
 *
 * 构建：make SOC=ns_core0 BOARD=fpga_eval DOWNLOAD=ilm all
 */

#include "FreeRTOS.h"
#include "task.h"

#include "nuclei_sdk_soc.h"
#include "utb_ipc.h"
#include "utb_ipc_msg.h"

/* ========================================================================
 * 配置常量
 * ======================================================================== */

/* 任务栈大小（单位：word = 4 字节） */
#define IPC_TASK_STACK_WORDS    192     /* 768B：ring pop 需要 ~130B 局部 */
#define HB_TASK_STACK_WORDS     128     /* 512B */
#define DP_TASK_STACK_WORDS     128     /* 512B */

/* 任务优先级 */
#define IPC_TASK_PRIO           4
#define HB_TASK_PRIO            3
#define DP_TASK_PRIO            2

/* 心跳周期 */
#define HB_PERIOD_MS            500

/* IPC 服务轮询超时（无门铃时的兜底唤醒） */
#define IPC_POLL_TIMEOUT_MS     50

/* ========================================================================
 * 静态分配：TCB + 栈
 * ======================================================================== */

static StaticTask_t s_ipc_tcb;
static StackType_t  s_ipc_stack[IPC_TASK_STACK_WORDS];

static StaticTask_t s_hb_tcb;
static StackType_t  s_hb_stack[HB_TASK_STACK_WORDS];

static StaticTask_t s_dp_tcb;
static StackType_t  s_dp_stack[DP_TASK_STACK_WORDS];

/* Idle task 静态内存（FreeRTOS 静态分配模式必须提供） */
static StaticTask_t s_idle_tcb;
static StackType_t  s_idle_stack[configMINIMAL_STACK_SIZE];

/* IPC 任务句柄（门铃回调需要） */
static TaskHandle_t s_ipc_task_handle;

/* ========================================================================
 * 从核本地状态（用于快照上报和命令响应）
 * ======================================================================== */

static volatile uint32_t s_uptime_sec;

/* 数据面统计（简化版，后续接入真实转发路径） */
static utb_ipc_status_payload_t s_dp_status;

/* 固件版本 */
static const utb_ipc_version_payload_t s_version = {
    .major      = 0,
    .minor      = 1,
    .patch      = 0,
    .build_id   = 0,
    .build_date = __DATE__,     /* "Apr  9 2026" 12 chars + '\0' < 16 */
};

/* ========================================================================
 * IDU 门铃回调（ISR 上下文）
 *
 * 通过 FreeRTOS 任务通知唤醒 IPC 服务 task。
 * 这是最轻量的 ISR→task 唤醒机制。
 * ======================================================================== */

static void ipc_doorbell_cb(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(s_ipc_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/* ========================================================================
 * 命令分派（从 IPC 服务 task 调用）
 *
 * 从核收到主核的控制请求，根据 cmd_id 处理并响应。
 * ======================================================================== */

static void slave_handle_command(const utb_ipc_msg_hdr_t *hdr,
                                 const void *payload, uint16_t len)
{
    int32_t result = UTB_IPC_OK;

    switch (hdr->cmd_id) {

    case UTB_CMD_NOP:
        /* 连通性测试，直接回 OK */
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK, (void *)0, 0);
        break;

    case UTB_CMD_HEARTBEAT: {
        utb_ipc_heartbeat_payload_t rsp;
        rsp.sender_tick = xTaskGetTickCount();
        rsp.uptime_sec  = s_uptime_sec;
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK,
                             &rsp, (uint16_t)sizeof(rsp));
        break;
    }

    case UTB_CMD_GET_STATUS:
        s_dp_status.uptime_sec = s_uptime_sec;
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK,
                             &s_dp_status, (uint16_t)sizeof(s_dp_status));
        break;

    case UTB_CMD_GET_VERSION:
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK,
                             &s_version, (uint16_t)sizeof(s_version));
        break;

    case UTB_CMD_RESET_DP:
        /*
         * 数据面复位：清零统计、重置状态。
         * 真实实现需停转发任务→清状态→重启转发任务。
         * 当前仅清零统计作为占位。
         */
        s_dp_status.rx_packets = 0;
        s_dp_status.tx_packets = 0;
        s_dp_status.rx_bytes   = 0;
        s_dp_status.tx_bytes   = 0;
        s_dp_status.rx_errors  = 0;
        s_dp_status.tx_errors  = 0;
        s_dp_status.rx_drops   = 0;
        s_dp_status.tx_drops   = 0;
        s_dp_status.dp_state   = UTB_DP_STATE_READY;
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK, (void *)0, 0);
        break;

    case UTB_CMD_SET_CONFIG:
    case UTB_CMD_GET_CONFIG:
    case UTB_CMD_DEL_CONFIG:
        /* 配置命令：暂不支持，返回 ERR_PARAM */
        result = UTB_IPC_ERR_PARAM;
        utb_ipc_ctrl_respond(hdr->txn_id, result, (void *)0, 0);
        break;

    default:
        /* 未知命令 */
        result = UTB_IPC_ERR_INVALID;
        utb_ipc_ctrl_respond(hdr->txn_id, result, (void *)0, 0);
        break;
    }
}

/* ========================================================================
 * IPC 服务 task
 *
 * 阻塞等待门铃通知（兜底 50ms 超时），drain ctrl_req_ring。
 * ======================================================================== */

static void task_ipc_service(void *arg)
{
    (void)arg;

    uint32_t ctrl_req_addr = UTB_IPC_SHM_BASE + UTB_IPC_CTRL_REQ_OFS;

    for (;;) {
        /* 等待门铃或超时 */
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(IPC_POLL_TIMEOUT_MS));

        /* Drain ctrl_req_ring：处理所有待处理的命令 */
        for (;;) {
            utb_ipc_msg_hdr_t hdr;
            uint8_t payload[UTB_IPC_CTRL_PAYLOAD_MAX];

            int ret = utb_ipc_recv(ctrl_req_addr, &hdr,
                                   payload, (uint16_t)sizeof(payload));
            if (ret == UTB_IPC_ERR_EMPTY) {
                break;  /* ring 空，退出内层循环 */
            }
            if (ret != UTB_IPC_OK) {
                continue;  /* 非法消息，跳过 */
            }

            /* 分派命令 */
            slave_handle_command(&hdr, payload, hdr.payload_len);
        }
    }
}

/* ========================================================================
 * 心跳 task
 *
 * 周期性更新心跳 tick 和状态/统计快照。
 * ======================================================================== */

static void task_heartbeat(void *arg)
{
    (void)arg;

    TickType_t last_wake = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(HB_PERIOD_MS));

        uint32_t tick = xTaskGetTickCount();

        /* 更新运行时间 */
        s_uptime_sec = tick / configTICK_RATE_HZ;

        /* 更新 IPC 心跳 */
        utb_ipc_heartbeat_tick(tick);

        /* 更新状态快照（主核可随时 seqlock 读取） */
        s_dp_status.uptime_sec = s_uptime_sec;
        utb_ipc_snapshot_write(UTB_IPC_STATE_SNAP_ADDR,
                               &s_dp_status,
                               (uint32_t)sizeof(s_dp_status),
                               tick);

        /* 更新统计快照（IPC 诊断计数器） */
        const utb_ipc_diag_t *diag = utb_ipc_diag_get();
        utb_ipc_snapshot_write(UTB_IPC_STATS_SNAP_ADDR,
                               diag,
                               (uint32_t)sizeof(utb_ipc_diag_t),
                               tick);
    }
}

/* ========================================================================
 * 数据面 task（占位）
 *
 * 未来：SPI / Ethernet RX 处理、转发、分片/重组。
 * 当前：空循环 + 状态标记。
 * ======================================================================== */

static void task_data_plane(void *arg)
{
    (void)arg;

    s_dp_status.dp_state = UTB_DP_STATE_RUNNING;

    for (;;) {
        /*
         * 占位：实际数据面任务在此处理：
         *   - SPI/ETH 收包
         *   - 转发决策
         *   - 发包
         *   - 统计更新（s_dp_status.rx_packets++ 等）
         *   - 异常时发送事件：utb_ipc_event_send(UTB_IPC_EVT_FAULT, ...)
         */
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/* ========================================================================
 * FreeRTOS 静态分配回调
 * ======================================================================== */

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer   = &s_idle_tcb;
    *ppxIdleTaskStackBuffer = s_idle_stack;
    *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}

/* ========================================================================
 * FreeRTOS Hook 函数
 * ======================================================================== */

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    /* 栈溢出：进入死循环，便于 JTAG 抓现场 */
    taskDISABLE_INTERRUPTS();
    for (;;);
}

/* ========================================================================
 * main — 从核入口
 * ======================================================================== */

int main(void)
{
    /* ---- 1. IPC 初始化（从核角色） ---- */
    int ret = utb_ipc_init(0);  /* is_primary = 0 */
    if (ret != UTB_IPC_OK) {
        /* IPC 初始化失败：死循环等待 JTAG */
        for (;;);
    }

    /* ---- 2. 注册真正的门铃回调（替换 init 中的空操作） ---- */
    utb_ipc_notify_init(ipc_doorbell_cb);

    /* ---- 3. 初始化数据面状态 ---- */
    s_dp_status.dp_state   = UTB_DP_STATE_INIT;
    s_dp_status.link_state = 0;

    /* ---- 4. 创建任务（静态分配） ---- */
    s_ipc_task_handle = xTaskCreateStatic(
        task_ipc_service, "ipc",
        IPC_TASK_STACK_WORDS, (void *)0, IPC_TASK_PRIO,
        s_ipc_stack, &s_ipc_tcb);

    xTaskCreateStatic(
        task_heartbeat, "hb",
        HB_TASK_STACK_WORDS, (void *)0, HB_TASK_PRIO,
        s_hb_stack, &s_hb_tcb);

    xTaskCreateStatic(
        task_data_plane, "dp",
        DP_TASK_STACK_WORDS, (void *)0, DP_TASK_PRIO,
        s_dp_stack, &s_dp_tcb);

    /* ---- 5. 启动调度器 ---- */
    vTaskStartScheduler();

    /* 不应到达 */
    for (;;);
}

/**
 * @file main.c
 * @brief QL25 AMP 主核应用（Core 1 管理面）
 *
 * 任务架构：
 *   task_ipc_service  (prio 5) — 事件接收、ctrl 响应分派
 *   task_heartbeat    (prio 4) — 心跳 + 存活检测 + 重启恢复
 *   task_management   (prio 3) — 状态查询、配置下发、快照读取
 *
 * 构建：
 *   make SOC=ns_core1 BOARD=fpga_eval DOWNLOAD=ilm all         # FPGA bringup
 *   make SOC=ns_core1 BOARD=fpga_eval DOWNLOAD=flashxip0 all   # 目标模式
 */

#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>

#include "nuclei_sdk_soc.h"
#include "utb_ipc.h"
#include "utb_ipc_msg.h"

/* ========================================================================
 * 配置常量
 * ======================================================================== */

/* 任务栈大小（word） */
#define IPC_TASK_STACK_WORDS    512     /* 2KB：event drain + payload 缓冲 */
#define HB_TASK_STACK_WORDS     256     /* 1KB */
#define MGMT_TASK_STACK_WORDS   512     /* 2KB：sync request 栈开销 ~140B */

/* 任务优先级 */
#define IPC_TASK_PRIO           5
#define HB_TASK_PRIO            4
#define MGMT_TASK_PRIO          3

/* 心跳周期 */
#define HB_PERIOD_MS            500

/* 心跳超时判定（2 秒） */
#define HB_MAX_AGE_MS           2000

/* 管理任务状态查询周期 */
#define MGMT_POLL_PERIOD_MS     2000

/* 同步请求参数 */
#define SYNC_TIMEOUT_CYCLES     500000  /* 轮询计数，约数百毫秒 */
#define SYNC_MAX_RETRY          2

/* IPC 服务轮询超时 */
#define IPC_POLL_TIMEOUT_MS     50

/* ========================================================================
 * 任务句柄
 * ======================================================================== */

static TaskHandle_t s_ipc_task_handle;

/* ========================================================================
 * IDU 门铃回调（ISR 上下文）
 * ======================================================================== */

static void ipc_doorbell_cb(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(s_ipc_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/* ========================================================================
 * 事件处理（从 IPC 服务 task 调用）
 * ======================================================================== */

static void master_handle_event(const utb_ipc_msg_hdr_t *hdr,
                                const void *payload, uint16_t len)
{
    switch (hdr->cmd_id) {

    case UTB_IPC_EVT_ALARM: {
        if (len >= sizeof(utb_ipc_alarm_payload_t)) {
            const utb_ipc_alarm_payload_t *alarm =
                (const utb_ipc_alarm_payload_t *)payload;
            printf("[EVT] ALARM id=%u sev=%u mod=%u\r\n",
                   alarm->alarm_id, alarm->severity, alarm->module_id);
        }
        break;
    }

    case UTB_IPC_EVT_FAULT: {
        if (len >= sizeof(utb_ipc_fault_payload_t)) {
            const utb_ipc_fault_payload_t *fault =
                (const utb_ipc_fault_payload_t *)payload;
            printf("[EVT] FAULT id=%u fatal=%u addr=0x%08lx\r\n",
                   fault->fault_id, fault->fatal,
                   (unsigned long)fault->fault_addr);
        }
        break;
    }

    case UTB_IPC_EVT_LINK_UP:
        printf("[EVT] LINK UP\r\n");
        break;

    case UTB_IPC_EVT_LINK_DOWN:
        printf("[EVT] LINK DOWN\r\n");
        break;

    default:
        printf("[EVT] unknown cmd_id=0x%04x\r\n", hdr->cmd_id);
        break;
    }
}

/* ========================================================================
 * IPC 服务 task
 *
 * 等待门铃通知，drain 两个 ring：
 *   - event_ring：从核事件
 *   - ctrl_rsp_ring：控制响应（sync 模式下由 sync API 自行消费，
 *     这里只处理遗留/异步响应）
 * ======================================================================== */

static void task_ipc_service(void *arg)
{
    (void)arg;

    for (;;) {
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(IPC_POLL_TIMEOUT_MS));

        /* Drain event_ring */
        for (;;) {
            utb_ipc_msg_hdr_t hdr;
            uint8_t payload[UTB_IPC_EVENT_PAYLOAD_MAX];

            int ret = utb_ipc_event_recv(&hdr, payload,
                                         (uint16_t)sizeof(payload));
            if (ret != UTB_IPC_OK) {
                break;
            }
            master_handle_event(&hdr, payload, hdr.payload_len);
        }

        /*
         * ctrl_rsp_ring 通常由 utb_ipc_ctrl_request_sync() 自行轮询消费。
         * 此处 drain 遗留响应（超时后残留的过期事务），避免 ring 淤积。
         */
        for (;;) {
            utb_ipc_msg_hdr_t hdr;
            uint8_t payload[UTB_IPC_CTRL_PAYLOAD_MAX];
            uint32_t rsp_addr = UTB_IPC_SHM_BASE + UTB_IPC_CTRL_RSP_OFS;

            int ret = utb_ipc_recv(rsp_addr, &hdr,
                                   payload, (uint16_t)sizeof(payload));
            if (ret != UTB_IPC_OK) {
                break;
            }
            /* 遗留响应：仅记日志，不做业务处理 */
        }
    }
}

/* ========================================================================
 * 心跳 task
 *
 * 500ms 周期：
 *   1. 更新本核心跳 tick
 *   2. 检查从核存活
 *   3. 检测从核重启 → 执行恢复
 * ======================================================================== */

static void task_heartbeat(void *arg)
{
    (void)arg;

    TickType_t last_wake = xTaskGetTickCount();

    /* 等待从核就绪后再开始心跳检测 */
    while (!utb_ipc_is_peer_ready()) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    printf("[HB] peer (slave) ready\r\n");

    for (;;) {
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(HB_PERIOD_MS));

        uint32_t tick = xTaskGetTickCount();

        /* 更新本核心跳 */
        utb_ipc_heartbeat_tick(tick);

        /* 检测从核存活 */
        if (!utb_ipc_is_peer_alive(tick, HB_MAX_AGE_MS)) {
            printf("[HB] WARNING: slave heartbeat stale\r\n");
        }

        /* 检测从核重启 */
        if (utb_ipc_check_peer_restart()) {
            printf("[HB] slave restart detected, recovering...\r\n");
            int ret = utb_ipc_recover();
            if (ret == UTB_IPC_OK) {
                printf("[HB] IPC recovery done, waiting peer ready\r\n");
                /* 等待从核重新 ready */
                while (!utb_ipc_is_peer_ready()) {
                    vTaskDelay(pdMS_TO_TICKS(100));
                }
                printf("[HB] peer ready again\r\n");
            } else {
                printf("[HB] recovery failed: %d\r\n", ret);
            }
        }
    }
}

/* ========================================================================
 * 管理 task
 *
 * 周期性查询从核状态、读取快照。
 * 启动时查询版本信息。
 * ======================================================================== */

static void task_management(void *arg)
{
    (void)arg;

    /* 等待从核就绪 */
    while (!utb_ipc_is_peer_ready()) {
        vTaskDelay(pdMS_TO_TICKS(200));
    }

    /* ---- 启动：查询从核版本 ---- */
    {
        utb_ipc_version_payload_t ver;
        int32_t result = 0;
        int ret = utb_ipc_ctrl_request_sync(
            UTB_CMD_GET_VERSION,
            (void *)0, 0,
            &ver, (uint16_t)sizeof(ver),
            &result,
            SYNC_TIMEOUT_CYCLES, SYNC_MAX_RETRY);

        if (ret == UTB_IPC_OK && result == UTB_IPC_OK) {
            printf("[MGMT] slave version: %u.%u.%u build=%lu date=%.16s\r\n",
                   ver.major, ver.minor, ver.patch,
                   (unsigned long)ver.build_id, ver.build_date);
        } else {
            printf("[MGMT] GET_VERSION failed: ret=%d result=%ld\r\n",
                   ret, (long)result);
        }
    }

    /* ---- 周期性状态查询 ---- */
    TickType_t last_wake = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(MGMT_POLL_PERIOD_MS));

        /* 1. 同步查询从核状态 */
        utb_ipc_status_payload_t status;
        int32_t result = 0;
        int ret = utb_ipc_ctrl_request_sync(
            UTB_CMD_GET_STATUS,
            (void *)0, 0,
            &status, (uint16_t)sizeof(status),
            &result,
            SYNC_TIMEOUT_CYCLES, SYNC_MAX_RETRY);

        if (ret == UTB_IPC_OK && result == UTB_IPC_OK) {
            printf("[MGMT] dp_state=%u link=%u uptime=%lus "
                   "rx=%lu tx=%lu\r\n",
                   status.dp_state, status.link_state,
                   (unsigned long)status.uptime_sec,
                   (unsigned long)status.rx_packets,
                   (unsigned long)status.tx_packets);
        }

        /* 2. 读取状态快照（seqlock） */
        {
            utb_ipc_status_payload_t snap_status;
            uint32_t gen = 0, ts = 0;
            ret = utb_ipc_snapshot_read(
                UTB_IPC_STATE_SNAP_ADDR,
                &snap_status, (uint32_t)sizeof(snap_status),
                &gen, &ts);
            if (ret == UTB_IPC_OK) {
                printf("[MGMT] snapshot gen=%lu ts=%lu dp=%u\r\n",
                       (unsigned long)gen, (unsigned long)ts,
                       snap_status.dp_state);
            }
        }

        /* 3. 读取 IPC 诊断快照 */
        {
            utb_ipc_diag_t snap_diag;
            uint32_t gen = 0, ts = 0;
            ret = utb_ipc_snapshot_read(
                UTB_IPC_STATS_SNAP_ADDR,
                &snap_diag, (uint32_t)sizeof(snap_diag),
                &gen, &ts);
            if (ret == UTB_IPC_OK) {
                printf("[MGMT] slave diag: ctrl_tx=%lu evt_tx=%lu drop=%lu\r\n",
                       (unsigned long)snap_diag.ctrl_tx,
                       (unsigned long)snap_diag.evt_tx,
                       (unsigned long)snap_diag.evt_tx_drop);
            }
        }

        /* 4. 本核 IPC 诊断 */
        {
            const utb_ipc_diag_t *diag = utb_ipc_diag_get();
            printf("[MGMT] local diag: ctrl_tx=%lu ctrl_rx=%lu "
                   "evt_rx=%lu timeout=%lu\r\n",
                   (unsigned long)diag->ctrl_tx,
                   (unsigned long)diag->ctrl_rx,
                   (unsigned long)diag->evt_rx,
                   (unsigned long)diag->sync_timeout);
        }
    }
}

/* ========================================================================
 * FreeRTOS Hook 函数
 * ======================================================================== */

void vApplicationMallocFailedHook(void)
{
    printf("[FATAL] malloc failed\r\n");
    taskDISABLE_INTERRUPTS();
    for (;;);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    printf("[FATAL] stack overflow: %s\r\n", pcTaskName ? pcTaskName : "?");
    taskDISABLE_INTERRUPTS();
    for (;;);
}

void vApplicationIdleHook(void)
{
    /* 空闲：可用于低功耗或 watchdog 喂狗 */
}

/* ========================================================================
 * main — 主核入口
 * ======================================================================== */

int main(void)
{
    printf("QL25 AMP Master starting...\r\n");

    /* ---- 1. IPC 初始化（主核角色） ---- */
    int ret = utb_ipc_init(1);  /* is_primary = 1 */
    if (ret != UTB_IPC_OK) {
        printf("[FATAL] IPC init failed: %d\r\n", ret);
        for (;;);
    }
    printf("[INIT] IPC initialized\r\n");

    /* ---- 2. 注册真正的门铃回调 ---- */
    utb_ipc_notify_init(ipc_doorbell_cb);

    /* ---- 3. 创建任务（动态分配） ---- */
    xTaskCreate(task_ipc_service, "ipc",
                IPC_TASK_STACK_WORDS, (void *)0, IPC_TASK_PRIO,
                &s_ipc_task_handle);

    xTaskCreate(task_heartbeat, "hb",
                HB_TASK_STACK_WORDS, (void *)0, HB_TASK_PRIO,
                (void *)0);

    xTaskCreate(task_management, "mgmt",
                MGMT_TASK_STACK_WORDS, (void *)0, MGMT_TASK_PRIO,
                (void *)0);

    /* ---- 4. 启动调度器 ---- */
    printf("[INIT] starting scheduler\r\n");
    vTaskStartScheduler();

    /* 不应到达 */
    printf("[FATAL] scheduler returned\r\n");
    for (;;);
}

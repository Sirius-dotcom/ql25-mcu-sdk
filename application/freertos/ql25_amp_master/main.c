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
#define IPC_TASK_STACK_WORDS    512
#define HB_TASK_STACK_WORDS     256
#define MGMT_TASK_STACK_WORDS   512

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
#define SYNC_TIMEOUT_CYCLES     500000
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
 * ======================================================================== */

static void task_ipc_service(void *arg)
{
    (void)arg;

    for (;;) {
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(IPC_POLL_TIMEOUT_MS));

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

        for (;;) {
            utb_ipc_msg_hdr_t hdr;
            uint8_t payload[UTB_IPC_CTRL_PAYLOAD_MAX];
            uint32_t rsp_addr = UTB_IPC_SHM_BASE + UTB_IPC_CTRL_RSP_OFS;

            int ret = utb_ipc_recv(rsp_addr, &hdr,
                                   payload, (uint16_t)sizeof(payload));
            if (ret != UTB_IPC_OK) {
                break;
            }
        }
    }
}

/* ========================================================================
 * 心跳 task
 * ======================================================================== */

static void task_heartbeat(void *arg)
{
    (void)arg;

    TickType_t last_wake = xTaskGetTickCount();

    while (!utb_ipc_is_peer_ready()) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    printf("[HB] peer (slave) ready\r\n");

    for (;;) {
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(HB_PERIOD_MS));

        {
            uint32_t tick = xTaskGetTickCount();

            utb_ipc_heartbeat_tick(tick);

            if (!utb_ipc_is_peer_alive(tick, HB_MAX_AGE_MS)) {
                printf("[HB] WARNING: slave heartbeat stale\r\n");
            }

            if (utb_ipc_check_peer_restart()) {
                printf("[HB] slave restart detected, recovering...\r\n");
                {
                    int ret = utb_ipc_recover();
                    if (ret == UTB_IPC_OK) {
                        printf("[HB] IPC recovery done, waiting peer ready\r\n");
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
    }
}

/* ========================================================================
 * 管理 task
 * ======================================================================== */

static void task_management(void *arg)
{
    (void)arg;

    while (!utb_ipc_is_peer_ready()) {
        vTaskDelay(pdMS_TO_TICKS(200));
    }

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

    {
        TickType_t last_wake = xTaskGetTickCount();

        for (;;) {
            vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(MGMT_POLL_PERIOD_MS));

            {
                utb_ipc_status_payload_t status;
                int32_t result = 0;
                int ret = utb_ipc_ctrl_request_sync(
                    UTB_CMD_GET_STATUS,
                    (void *)0, 0,
                    &status, (uint16_t)sizeof(status),
                    &result,
                    SYNC_TIMEOUT_CYCLES, SYNC_MAX_RETRY);

                if (ret == UTB_IPC_OK && result == UTB_IPC_OK) {
                    printf("[MGMT] dp_state=%u link=%u uptime=%lus rx=%lu tx=%lu\r\n",
                           status.dp_state, status.link_state,
                           (unsigned long)status.uptime_sec,
                           (unsigned long)status.rx_packets,
                           (unsigned long)status.tx_packets);
                }
            }

            {
                utb_ipc_status_payload_t snap_status;
                uint32_t gen = 0;
                uint32_t ts = 0;
                int ret = utb_ipc_snapshot_read(
                    UTB_IPC_STATE_SNAP_ADDR,
                    &snap_status, (uint32_t)sizeof(snap_status),
                    &gen, &ts);
                if (ret == UTB_IPC_OK) {
                    printf("[MGMT] snapshot gen=%lu ts=%lu dp=%u\r\n",
                           (unsigned long)gen, (unsigned long)ts,
                           snap_status.dp_state);
                }
            }

            {
                utb_ipc_diag_t snap_diag;
                uint32_t gen = 0;
                uint32_t ts = 0;
                int ret = utb_ipc_snapshot_read(
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

            {
                const utb_ipc_diag_t *diag = utb_ipc_diag_get();
                printf("[MGMT] local diag: ctrl_tx=%lu ctrl_rx=%lu evt_rx=%lu timeout=%lu\r\n",
                       (unsigned long)diag->ctrl_tx,
                       (unsigned long)diag->ctrl_rx,
                       (unsigned long)diag->evt_rx,
                       (unsigned long)diag->sync_timeout);
            }
        }
    }
}

void vApplicationMallocFailedHook(void)
{
    printf("[FATAL] malloc failed\r\n");
    taskDISABLE_INTERRUPTS();
    for (;;)
        ;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    printf("[FATAL] stack overflow: %s\r\n", pcTaskName ? pcTaskName : "?");
    taskDISABLE_INTERRUPTS();
    for (;;)
        ;
}

void vApplicationIdleHook(void)
{
}

int main(void)
{
    printf("QL25 AMP Master starting...\r\n");

    {
        int ret = utb_ipc_init(1);
        if (ret != UTB_IPC_OK) {
            printf("[FATAL] IPC init failed: %d\r\n", ret);
            for (;;)
                ;
        }
    }
    printf("[INIT] IPC initialized\r\n");

    utb_ipc_notify_init(ipc_doorbell_cb);

    xTaskCreate(task_ipc_service, "ipc",
                IPC_TASK_STACK_WORDS, (void *)0, IPC_TASK_PRIO,
                &s_ipc_task_handle);

    xTaskCreate(task_heartbeat, "hb",
                HB_TASK_STACK_WORDS, (void *)0, HB_TASK_PRIO,
                (void *)0);

    xTaskCreate(task_management, "mgmt",
                MGMT_TASK_STACK_WORDS, (void *)0, MGMT_TASK_PRIO,
                (void *)0);

    printf("[INIT] starting scheduler\r\n");
    vTaskStartScheduler();

    printf("[FATAL] scheduler returned\r\n");
    for (;;)
        ;
}

#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "nuclei_sdk_soc.h"
#include "utb_hal.h"
#include "utb_ipc.h"
#include "utb_ipc_msg.h"

#define IPC_TASK_STACK_WORDS        192u
#define HB_TASK_STACK_WORDS         160u
#define DP_TASK_STACK_WORDS         256u

#define IPC_TASK_PRIO               4u
#define HB_TASK_PRIO                3u
#define DP_TASK_PRIO                2u

#define HB_PERIOD_MS                500u
#define IPC_POLL_TIMEOUT_MS         50u

#define DP_FIXED_LEN                472u
#define DP_MAC_TIMER                0x8000u
#define DP_AHB_TIMER                0xC350u
#define DP_SPI_CLK_DIV              8u
#define DP_RX_WAIT_MS               100u
#define DP_IDLE_SLEEP_MS            1u
#define DP_EVENT_TIMEOUT_THRESHOLD  32u
#define DP_FAULT_INIT               1u
#define DP_FAULT_SPI                2u

typedef struct {
    volatile uint8_t dp_state;
    volatile uint8_t link_state;
    volatile uint32_t rx_packets;
    volatile uint32_t tx_packets;
    volatile uint32_t rx_bytes;
    volatile uint32_t tx_bytes;
    volatile uint32_t rx_errors;
    volatile uint32_t tx_errors;
    volatile uint32_t rx_drops;
    volatile uint32_t tx_drops;
    volatile uint32_t rx_timeouts;
    volatile uint32_t rx_ready_int_hits;
    volatile uint32_t rx_ready_fifo_hits;
    volatile uint32_t fault_events;
} ql25_dp_runtime_t;

static StaticTask_t s_ipc_tcb;
static StackType_t  s_ipc_stack[IPC_TASK_STACK_WORDS];

static StaticTask_t s_hb_tcb;
static StackType_t  s_hb_stack[HB_TASK_STACK_WORDS];

static StaticTask_t s_dp_tcb;
static StackType_t  s_dp_stack[DP_TASK_STACK_WORDS];

static StaticTask_t s_idle_tcb;
static StackType_t  s_idle_stack[configMINIMAL_STACK_SIZE];

static TaskHandle_t s_ipc_task_handle;

static volatile uint32_t s_uptime_sec;
static volatile uint32_t s_dp_reset_req;
static ql25_dp_runtime_t s_dp_runtime;
static utb_ipc_status_payload_t s_dp_status;
static uint8_t s_dp_rx_buf[DP_FIXED_LEN];
static utb_hal_spi_handle_t s_dp_spi_handle;
static utb_hal_spi_static_storage_t s_dp_spi_storage;

static const utb_ipc_version_payload_t s_version = {
    .major      = 0,
    .minor      = 1,
    .patch      = 0,
    .build_id   = 0,
    .build_date = __DATE__,
};

static void ipc_doorbell_cb(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(s_ipc_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void ql25_dp_iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_SCK_IOF_IVAL, QSPI1_SCK_PAD_SEL, QSPI1_SCK_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_CS_0_IOF_IVAL, QSPI1_CS_0_PAD_SEL, QSPI1_CS_0_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_IVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_OVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_IVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_OVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
}

static int ql25_dp_mac_init(void)
{
    utb_hal_mac_cfg_t mac_cfg;

    memset(&mac_cfg, 0, sizeof(mac_cfg));
    mac_cfg.fixed_length = DP_FIXED_LEN;
    mac_cfg.mac_timer = DP_MAC_TIMER;
    mac_cfg.ahb_symb_timer = DP_AHB_TIMER;
    mac_cfg.mac_en = 1u;

    return utb_hal_mac_init(&mac_cfg);
}

static int ql25_dp_spi_init(void)
{
    utb_hal_spi_cfg_t spi_cfg;

    memset(&spi_cfg, 0, sizeof(spi_cfg));
    memset(&s_dp_spi_handle, 0, sizeof(s_dp_spi_handle));
    memset(&s_dp_spi_storage, 0, sizeof(s_dp_spi_storage));

    spi_cfg.bus = UTB_HAL_SPI_BUS_QSPI1;
    spi_cfg.master = true;
    spi_cfg.mode = 1u;
    spi_cfg.bits_per_word = 8u;
    spi_cfg.lsb_first = false;
    spi_cfg.clock_div = DP_SPI_CLK_DIV;

    return utb_hal_spi_open_static(&s_dp_spi_handle, &s_dp_spi_storage, &spi_cfg);
}

static void ql25_dp_reset_runtime(void)
{
    memset(&s_dp_runtime, 0, sizeof(s_dp_runtime));
    s_dp_runtime.dp_state = UTB_DP_STATE_READY;
}

static void ql25_dp_report_fault(uint16_t fault_id, uint32_t info0)
{
    utb_ipc_fault_payload_t fault;

    memset(&fault, 0, sizeof(fault));
    fault.fault_id = fault_id;
    fault.fatal = 0u;
    fault.module_id = 1u;
    fault.info0 = info0;
    fault.timestamp_ms = xTaskGetTickCount();
    if (utb_ipc_event_send(UTB_IPC_EVT_FAULT, &fault, (uint16_t)sizeof(fault)) == UTB_IPC_OK) {
        s_dp_runtime.fault_events++;
    }
}

static int ql25_dp_wait_rx_ready(uint32_t timeout_ms)
{
    uint32_t waited_ms;

    for (waited_ms = 0; waited_ms < timeout_ms; waited_ms++) {
        utb_hal_mac_fifo_slice_state_t fifo_state;
        uint32_t int_status;

        int_status = utb_hal_mac_get_int_status();
        utb_hal_mac_get_rx_fifo_slice_state(&fifo_state);

        if ((int_status & (1u << UTB_HAL_MAC_INT_RX_SLICE_READABLE)) != 0u) {
            s_dp_runtime.rx_ready_int_hits++;
            return UTB_OK;
        }

        if (fifo_state.committed_slices != 0u) {
            s_dp_runtime.rx_ready_fifo_hits++;
            return UTB_OK;
        }

        vTaskDelay(pdMS_TO_TICKS(DP_IDLE_SLEEP_MS));
    }

    return UTB_ERR_TIMEOUT;
}

static void ql25_dp_read_slice(uint8_t *buf, uint32_t len)
{
    uint32_t i;

    for (i = 0; i < len; i++) {
        buf[i] = utb_hal_mac_read_symb8();
    }
}

static void ql25_dp_refresh_status(void)
{
    s_dp_status.dp_state = s_dp_runtime.dp_state;
    s_dp_status.link_state = s_dp_runtime.link_state;
    s_dp_status.uptime_sec = s_uptime_sec;
    s_dp_status.rx_packets = s_dp_runtime.rx_packets;
    s_dp_status.tx_packets = s_dp_runtime.tx_packets;
    s_dp_status.rx_bytes = s_dp_runtime.rx_bytes;
    s_dp_status.tx_bytes = s_dp_runtime.tx_bytes;
    s_dp_status.rx_errors = s_dp_runtime.rx_errors;
    s_dp_status.tx_errors = s_dp_runtime.tx_errors;
    s_dp_status.rx_drops = s_dp_runtime.rx_drops;
    s_dp_status.tx_drops = s_dp_runtime.tx_drops;
}

static void slave_handle_command(const utb_ipc_msg_hdr_t *hdr,
                                 const void *payload, uint16_t len)
{
    int32_t result;

    (void)payload;
    (void)len;
    result = UTB_IPC_OK;

    switch (hdr->cmd_id) {
    case UTB_CMD_NOP:
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK, (void *)0, 0);
        break;

    case UTB_CMD_HEARTBEAT: {
        utb_ipc_heartbeat_payload_t rsp;

        rsp.sender_tick = xTaskGetTickCount();
        rsp.uptime_sec = s_uptime_sec;
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK, &rsp, (uint16_t)sizeof(rsp));
        break;
    }

    case UTB_CMD_GET_STATUS:
        ql25_dp_refresh_status();
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK,
                             &s_dp_status, (uint16_t)sizeof(s_dp_status));
        break;

    case UTB_CMD_GET_VERSION:
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK,
                             &s_version, (uint16_t)sizeof(s_version));
        break;

    case UTB_CMD_RESET_DP:
        s_dp_reset_req = 1u;
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK, (void *)0, 0);
        break;

    case UTB_CMD_SET_CONFIG:
    case UTB_CMD_GET_CONFIG:
    case UTB_CMD_DEL_CONFIG:
        result = UTB_IPC_ERR_PARAM;
        utb_ipc_ctrl_respond(hdr->txn_id, result, (void *)0, 0);
        break;

    default:
        result = UTB_IPC_ERR_INVALID;
        utb_ipc_ctrl_respond(hdr->txn_id, result, (void *)0, 0);
        break;
    }
}

static void task_ipc_service(void *arg)
{
    uint32_t ctrl_req_addr;

    (void)arg;
    ctrl_req_addr = UTB_IPC_SHM_BASE + UTB_IPC_CTRL_REQ_OFS;

    for (;;) {
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(IPC_POLL_TIMEOUT_MS));

        for (;;) {
            utb_ipc_msg_hdr_t hdr;
            uint8_t payload[UTB_IPC_CTRL_PAYLOAD_MAX];
            int ret;

            ret = utb_ipc_recv(ctrl_req_addr, &hdr, payload, (uint16_t)sizeof(payload));
            if (ret == UTB_IPC_ERR_EMPTY) {
                break;
            }
            if (ret != UTB_IPC_OK) {
                continue;
            }

            slave_handle_command(&hdr, payload, hdr.payload_len);
        }
    }
}

static void task_heartbeat(void *arg)
{
    TickType_t last_wake;

    (void)arg;
    last_wake = xTaskGetTickCount();

    for (;;) {
        uint32_t tick;
        const utb_ipc_diag_t *diag;

        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(HB_PERIOD_MS));

        tick = xTaskGetTickCount();
        s_uptime_sec = tick / configTICK_RATE_HZ;

        utb_ipc_heartbeat_tick(tick);

        ql25_dp_refresh_status();
        utb_ipc_snapshot_write(UTB_IPC_STATE_SNAP_ADDR,
                               &s_dp_status,
                               (uint32_t)sizeof(s_dp_status),
                               tick);

        diag = utb_ipc_diag_get();
        utb_ipc_snapshot_write(UTB_IPC_STATS_SNAP_ADDR,
                               diag,
                               (uint32_t)sizeof(utb_ipc_diag_t),
                               tick);
    }
}

static void task_data_plane(void *arg)
{
    uint32_t timeout_accum;
    int ret;

    (void)arg;
    timeout_accum = 0u;
    ql25_dp_reset_runtime();

    ql25_dp_iomux_config();

    ret = ql25_dp_mac_init();
    if (ret != UTB_OK) {
        s_dp_runtime.dp_state = UTB_DP_STATE_ERROR;
        s_dp_runtime.rx_errors++;
        ql25_dp_report_fault(DP_FAULT_INIT, (uint32_t)(uint16_t)(-ret));
        for (;;) {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    ret = ql25_dp_spi_init();
    if (ret != UTB_OK) {
        s_dp_runtime.dp_state = UTB_DP_STATE_ERROR;
        s_dp_runtime.tx_errors++;
        ql25_dp_report_fault(DP_FAULT_INIT, (uint32_t)(uint16_t)(-ret));
        for (;;) {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    utb_hal_mac_clear_ints(0xFFFFu);
    utb_hal_mac_clear_stats();
    s_dp_runtime.dp_state = UTB_DP_STATE_RUNNING;

    for (;;) {
        if (s_dp_reset_req != 0u) {
            s_dp_reset_req = 0u;
            ql25_dp_reset_runtime();
            utb_hal_mac_clear_ints(0xFFFFu);
            utb_hal_mac_clear_stats();
            s_dp_runtime.dp_state = UTB_DP_STATE_RUNNING;
            timeout_accum = 0u;
        }

        ret = ql25_dp_wait_rx_ready(DP_RX_WAIT_MS);
        if (ret != UTB_OK) {
            timeout_accum++;
            s_dp_runtime.rx_timeouts++;
            if ((timeout_accum % DP_EVENT_TIMEOUT_THRESHOLD) == 0u) {
                utb_hal_mac_stats_t mac_stats;

                memset(&mac_stats, 0, sizeof(mac_stats));
                utb_hal_mac_get_stats(&mac_stats);
                if ((mac_stats.rx_timeout_wrong_num != 0u) ||
                    (mac_stats.phy_len_wrong_num != 0u) ||
                    (mac_stats.rx_fifo_overflow_symb_num != 0u) ||
                    (mac_stats.ahb_rx_timeout_num != 0u)) {
                    s_dp_runtime.rx_errors++;
                    ql25_dp_report_fault(DP_FAULT_SPI, mac_stats.rx_timeout_wrong_num);
                }
            }
            continue;
        }

        timeout_accum = 0u;
        ql25_dp_read_slice(s_dp_rx_buf, DP_FIXED_LEN);
        utb_hal_mac_clear_ints(1u << UTB_HAL_MAC_INT_RX_SLICE_READABLE);

        s_dp_runtime.link_state = 1u;
        s_dp_runtime.rx_packets++;
        s_dp_runtime.rx_bytes += DP_FIXED_LEN;

        ret = utb_hal_spi_transfer(&s_dp_spi_handle, s_dp_rx_buf, NULL, DP_FIXED_LEN);
        if (ret != UTB_OK) {
            s_dp_runtime.tx_errors++;
            s_dp_runtime.tx_drops++;
            ql25_dp_report_fault(DP_FAULT_SPI, (uint32_t)(uint16_t)(-ret));
            continue;
        }

        s_dp_runtime.tx_packets++;
        s_dp_runtime.tx_bytes += DP_FIXED_LEN;
    }
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &s_idle_tcb;
    *ppxIdleTaskStackBuffer = s_idle_stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    taskDISABLE_INTERRUPTS();
    for (;;) {
    }
}

int main(void)
{
    int ret;
    TaskHandle_t hb_task_handle;
    TaskHandle_t dp_task_handle;

    memset(&s_dp_status, 0, sizeof(s_dp_status));
    memset(&s_dp_runtime, 0, sizeof(s_dp_runtime));
    memset(s_dp_rx_buf, 0, sizeof(s_dp_rx_buf));

    ret = utb_ipc_init(0);
    if (ret != UTB_IPC_OK) {
        for (;;) {
        }
    }

    s_dp_runtime.dp_state = UTB_DP_STATE_INIT;

    s_ipc_task_handle = xTaskCreateStatic(
        task_ipc_service, "ipc",
        IPC_TASK_STACK_WORDS, (void *)0, IPC_TASK_PRIO,
        s_ipc_stack, &s_ipc_tcb);
    if (s_ipc_task_handle == NULL) {
        for (;;) {
        }
    }

    hb_task_handle = xTaskCreateStatic(
        task_heartbeat, "hb",
        HB_TASK_STACK_WORDS, (void *)0, HB_TASK_PRIO,
        s_hb_stack, &s_hb_tcb);
    if (hb_task_handle == NULL) {
        for (;;) {
        }
    }

    dp_task_handle = xTaskCreateStatic(
        task_data_plane, "dp",
        DP_TASK_STACK_WORDS, (void *)0, DP_TASK_PRIO,
        s_dp_stack, &s_dp_tcb);
    if (dp_task_handle == NULL) {
        for (;;) {
        }
    }

    utb_ipc_notify_init(ipc_doorbell_cb);

    vTaskStartScheduler();

    for (;;) {
    }
}

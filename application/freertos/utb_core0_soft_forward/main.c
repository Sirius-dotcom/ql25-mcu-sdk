#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "utb_api.h"

#define UTB_CORE0_FWD_FIXED_LEN              472u
#define UTB_CORE0_FWD_MAC_TIMER              0x8000u
#define UTB_CORE0_FWD_AHB_TIMER              0xC350u
#define UTB_CORE0_FWD_SPI_CLK_DIV            8u
#define UTB_CORE0_FWD_RX_TASK_STACK_WORDS    1024u
#define UTB_CORE0_FWD_RX_TASK_PRIO           3u
#define UTB_CORE0_FWD_MAIN_TASK_STACK_WORDS  1024u
#define UTB_CORE0_FWD_MAIN_TASK_PRIO         4u
#define UTB_CORE0_FWD_RX_WAIT_MS             100u
#define UTB_CORE0_FWD_IDLE_SLEEP_MS          1u
#define UTB_CORE0_FWD_LOG_INTERVAL           128u
#define UTB_CORE0_FWD_TIMEOUT_LOG_INTERVAL   16u

typedef struct {
    uint32_t rx_slices;
    uint32_t spi_tx_slices;
    uint32_t rx_timeouts;
    uint32_t spi_errors;
    uint32_t rx_ready_int_hits;
    uint32_t rx_ready_fifo_hits;
} utb_core0_fwd_stats_t;

static uint8_t g_utb_core0_fwd_rx_buf[UTB_CORE0_FWD_FIXED_LEN];
static utb_hal_spi_handle_t *g_utb_core0_fwd_spi;
static utb_core0_fwd_stats_t g_utb_core0_fwd_stats;

static void utb_core0_fwd_iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_SCK_IOF_IVAL, QSPI1_SCK_PAD_SEL, QSPI1_SCK_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_CS_0_IOF_IVAL, QSPI1_CS_0_PAD_SEL, QSPI1_CS_0_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_IVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_OVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_IVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_OVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
}

static void utb_core0_fwd_dump_mac_snapshot(const char *tag)
{
    utb_hal_mac_fifo_slice_state_t rx_fifo_state;
    utb_hal_mac_fifo_byte_state_t rx_fifo_bytes;
    utb_hal_mac_path_state_t rx_path_state;
    uint32_t int_status;

    int_status = utb_hal_mac_get_int_status();
    utb_hal_mac_get_rx_fifo_slice_state(&rx_fifo_state);
    utb_hal_mac_get_rx_fifo_byte_state(&rx_fifo_bytes);
    utb_hal_mac_get_rx_path_state(&rx_path_state);

    UTB_PRINT("[CORE0_FWD] %s int=0x%08lx rx_avail=%u rx_commit=%u rx_unwritten=%u rx_written=%u rx_state=%u ahb_bytes=%u mac_bytes=%u\r\n",
              tag,
              (unsigned long)int_status,
              (unsigned)rx_fifo_state.available_slices,
              (unsigned)rx_fifo_state.committed_slices,
              (unsigned)rx_fifo_bytes.unwritten_bytes,
              (unsigned)rx_fifo_bytes.written_bytes,
              (unsigned)rx_path_state.state,
              (unsigned)rx_path_state.ahb_byte_cnt,
              (unsigned)rx_path_state.mac_byte_cnt);
}

static void utb_core0_fwd_dump_mac_stats(const char *tag)
{
    utb_hal_mac_stats_t mac_stats;

    memset(&mac_stats, 0, sizeof(mac_stats));
    utb_hal_mac_get_stats(&mac_stats);

    UTB_PRINT("[CORE0_FWD] %s mcu_len_right=%lu total_2phy=%lu valid_2phy=%lu rx_timeout=%lu phy_len_wrong=%lu phy_req_len_wrong=%lu rx_overflow=%lu ahb_rx_timeout=%lu\r\n",
              tag,
              (unsigned long)mac_stats.mcu_len_right_num,
              (unsigned long)mac_stats.total_slice_num_2phy,
              (unsigned long)mac_stats.valid_slice_num_2phy,
              (unsigned long)mac_stats.rx_timeout_wrong_num,
              (unsigned long)mac_stats.phy_len_wrong_num,
              (unsigned long)mac_stats.phy_req_len_wrong_num,
              (unsigned long)mac_stats.rx_fifo_overflow_symb_num,
              (unsigned long)mac_stats.ahb_rx_timeout_num);
}

static utb_hal_spi_handle_t *utb_core0_fwd_open_spi(void)
{
    utb_hal_spi_cfg_t spi_cfg;

    memset(&spi_cfg, 0, sizeof(spi_cfg));
    spi_cfg.bus = UTB_HAL_SPI_BUS_QSPI1;
    spi_cfg.master = true;
    spi_cfg.mode = 1u;
    spi_cfg.bits_per_word = 8u;
    spi_cfg.lsb_first = false;
    spi_cfg.clock_div = UTB_CORE0_FWD_SPI_CLK_DIV;

    return utb_hal_spi_open(&spi_cfg);
}

static int utb_core0_fwd_mac_init(void)
{
    utb_hal_mac_cfg_t mac_cfg;

    memset(&mac_cfg, 0, sizeof(mac_cfg));
    mac_cfg.fixed_length = UTB_CORE0_FWD_FIXED_LEN;
    mac_cfg.mac_timer = UTB_CORE0_FWD_MAC_TIMER;
    mac_cfg.ahb_symb_timer = UTB_CORE0_FWD_AHB_TIMER;
    mac_cfg.mac_en = 1u;

    return utb_hal_mac_init(&mac_cfg);
}

static int utb_core0_fwd_wait_rx_ready(uint32_t timeout_ms)
{
    uint32_t waited_ms;

    for (waited_ms = 0; waited_ms < timeout_ms; waited_ms++) {
        utb_hal_mac_fifo_slice_state_t fifo_state;
        uint32_t int_status;

        int_status = utb_hal_mac_get_int_status();
        utb_hal_mac_get_rx_fifo_slice_state(&fifo_state);

        if ((int_status & (1u << UTB_HAL_MAC_INT_RX_SLICE_READABLE)) != 0u) {
            g_utb_core0_fwd_stats.rx_ready_int_hits++;
            return UTB_OK;
        }

        if (fifo_state.committed_slices != 0u) {
            g_utb_core0_fwd_stats.rx_ready_fifo_hits++;
            return UTB_OK;
        }

        utb_os_thread_sleep_ms(UTB_CORE0_FWD_IDLE_SLEEP_MS);
    }

    return UTB_ERR_TIMEOUT;
}

static void utb_core0_fwd_read_slice(uint8_t *buf, uint32_t len)
{
    uint32_t i;

    for (i = 0; i < len; i++) {
        buf[i] = utb_hal_mac_read_symb8();
    }
}

static void utb_core0_fwd_log_slice_edges(const uint8_t *buf, uint32_t len)
{
    uint32_t i;
    uint32_t head_len;
    uint32_t tail_start;

    if ((buf == NULL) || (len == 0u)) {
        return;
    }

    head_len = (len < 8u) ? len : 8u;
    tail_start = (len > 8u) ? (len - 8u) : 0u;

    UTB_PRINT("[CORE0_FWD] head:");
    for (i = 0; i < head_len; i++) {
        UTB_PRINT(" %02x", buf[i]);
    }
    UTB_PRINT("\r\n");

    UTB_PRINT("[CORE0_FWD] tail:");
    for (i = tail_start; i < len; i++) {
        UTB_PRINT(" %02x", buf[i]);
    }
    UTB_PRINT("\r\n");
}

static void utb_core0_fwd_rx_task(void *arg)
{
    uint32_t next_log_slice;

    (void)arg;
    next_log_slice = 1u;

    UTB_PRINT("[CORE0_FWD] rx task start fixed_len=%u\r\n", (unsigned)UTB_CORE0_FWD_FIXED_LEN);
    utb_core0_fwd_dump_mac_snapshot("rx-task-start");

    for (;;) {
        int ret;

        ret = utb_core0_fwd_wait_rx_ready(UTB_CORE0_FWD_RX_WAIT_MS);
        if (ret != UTB_OK) {
            g_utb_core0_fwd_stats.rx_timeouts++;
            if ((g_utb_core0_fwd_stats.rx_timeouts % UTB_CORE0_FWD_TIMEOUT_LOG_INTERVAL) == 0u) {
                UTB_PRINT("[CORE0_FWD] rx timeout cnt=%lu\r\n",
                          (unsigned long)g_utb_core0_fwd_stats.rx_timeouts);
                utb_core0_fwd_dump_mac_snapshot("rx-timeout");
                utb_core0_fwd_dump_mac_stats("rx-timeout");
            }
            continue;
        }

        utb_core0_fwd_read_slice(g_utb_core0_fwd_rx_buf, UTB_CORE0_FWD_FIXED_LEN);
        utb_hal_mac_clear_ints(1u << UTB_HAL_MAC_INT_RX_SLICE_READABLE);
        g_utb_core0_fwd_stats.rx_slices++;

        ret = utb_hal_spi_transfer(g_utb_core0_fwd_spi,
                                   g_utb_core0_fwd_rx_buf,
                                   NULL,
                                   UTB_CORE0_FWD_FIXED_LEN);
        if (ret != UTB_OK) {
            g_utb_core0_fwd_stats.spi_errors++;
            UTB_PRINT("[CORE0_FWD] spi transfer failed ret=%d err_cnt=%lu\r\n",
                      ret,
                      (unsigned long)g_utb_core0_fwd_stats.spi_errors);
            utb_core0_fwd_dump_mac_snapshot("spi-error");
            utb_core0_fwd_dump_mac_stats("spi-error");
            utb_os_thread_sleep_ms(10);
            continue;
        }

        g_utb_core0_fwd_stats.spi_tx_slices++;
        if (g_utb_core0_fwd_stats.rx_slices == 1u) {
            utb_core0_fwd_log_slice_edges(g_utb_core0_fwd_rx_buf, UTB_CORE0_FWD_FIXED_LEN);
            utb_core0_fwd_dump_mac_snapshot("first-slice");
            utb_core0_fwd_dump_mac_stats("first-slice");
            next_log_slice = UTB_CORE0_FWD_LOG_INTERVAL;
        } else if (g_utb_core0_fwd_stats.rx_slices >= next_log_slice) {
            UTB_PRINT("[CORE0_FWD] rx=%lu spi=%lu rx_timeout=%lu spi_err=%lu ready_int=%lu ready_fifo=%lu stack_wm=%lu\r\n",
                      (unsigned long)g_utb_core0_fwd_stats.rx_slices,
                      (unsigned long)g_utb_core0_fwd_stats.spi_tx_slices,
                      (unsigned long)g_utb_core0_fwd_stats.rx_timeouts,
                      (unsigned long)g_utb_core0_fwd_stats.spi_errors,
                      (unsigned long)g_utb_core0_fwd_stats.rx_ready_int_hits,
                      (unsigned long)g_utb_core0_fwd_stats.rx_ready_fifo_hits,
                      (unsigned long)utb_os_thread_stack_watermark(NULL));
            utb_core0_fwd_dump_mac_stats("periodic");
            next_log_slice += UTB_CORE0_FWD_LOG_INTERVAL;
        }
    }
}

static void utb_core0_fwd_main(void *arg)
{
    utb_os_thread_init_t rx_task_cfg;
    int ret;

    (void)arg;
    memset(&g_utb_core0_fwd_stats, 0, sizeof(g_utb_core0_fwd_stats));
    memset(g_utb_core0_fwd_rx_buf, 0, sizeof(g_utb_core0_fwd_rx_buf));

    UTB_PRINT("\r\n==============================\r\n");
    UTB_PRINT("Core0 Soft Forward Start\r\n");
    UTB_PRINT("==============================\r\n");

    utb_core0_fwd_iomux_config();

    ret = utb_core0_fwd_mac_init();
    if (ret != UTB_OK) {
        UTB_PRINT("[CORE0_FWD][FATAL] mac init failed: %d\r\n", ret);
        for (;;) {
            utb_os_thread_sleep_ms(1000);
        }
    }

    g_utb_core0_fwd_spi = utb_core0_fwd_open_spi();
    if (g_utb_core0_fwd_spi == NULL) {
        UTB_PRINT("[CORE0_FWD][FATAL] spi open failed\r\n");
        for (;;) {
            utb_os_thread_sleep_ms(1000);
        }
    }

    utb_hal_mac_clear_ints(0xFFFFu);
    utb_hal_mac_clear_stats();
    utb_core0_fwd_dump_mac_snapshot("init-done");

    memset(&rx_task_cfg, 0, sizeof(rx_task_cfg));
    strcpy(rx_task_cfg.thread_name, "utb_rx_fwd");
    rx_task_cfg.stack_words = UTB_CORE0_FWD_RX_TASK_STACK_WORDS;
    rx_task_cfg.sched_priority = UTB_CORE0_FWD_RX_TASK_PRIO;
    rx_task_cfg.func_thread_call_back = utb_core0_fwd_rx_task;
    rx_task_cfg.arg = NULL;

    if (utb_os_thread_create(&rx_task_cfg) == NULL) {
        UTB_PRINT("[CORE0_FWD][FATAL] rx task create failed\r\n");
        for (;;) {
            utb_os_thread_sleep_ms(1000);
        }
    }

    for (;;) {
        utb_os_thread_sleep_ms(5000);
        UTB_PRINT("[CORE0_FWD] heartbeat rx=%lu spi=%lu rx_timeout=%lu spi_err=%lu\r\n",
                  (unsigned long)g_utb_core0_fwd_stats.rx_slices,
                  (unsigned long)g_utb_core0_fwd_stats.spi_tx_slices,
                  (unsigned long)g_utb_core0_fwd_stats.rx_timeouts,
                  (unsigned long)g_utb_core0_fwd_stats.spi_errors);
    }
}

void vApplicationMallocFailedHook(void)
{
    UTB_PRINT("[CORE0_FWD][FATAL] malloc failed\r\n");
    for (;;) {}
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    UTB_PRINT("[CORE0_FWD][FATAL] stack overflow: %s\r\n", pcTaskName);
    for (;;) {}
}

void vApplicationIdleHook(void)
{
}

int main(void)
{
    utb_os_thread_init_t main_cfg;

    memset(&main_cfg, 0, sizeof(main_cfg));
    strcpy(main_cfg.thread_name, "utb_core0_fwd");
    main_cfg.stack_words = UTB_CORE0_FWD_MAIN_TASK_STACK_WORDS;
    main_cfg.sched_priority = UTB_CORE0_FWD_MAIN_TASK_PRIO;
    main_cfg.func_thread_call_back = utb_core0_fwd_main;
    main_cfg.arg = NULL;

    UTB_PRINT("Starting Core0 soft forward...\r\n");
    if (utb_os_thread_create(&main_cfg) == NULL) {
        UTB_PRINT("[CORE0_FWD][FATAL] main task create failed\r\n");
        for (;;) {}
    }

    osal_scheduler_start();

    UTB_PRINT("[CORE0_FWD][FATAL] scheduler exited\r\n");
    for (;;) {}
}

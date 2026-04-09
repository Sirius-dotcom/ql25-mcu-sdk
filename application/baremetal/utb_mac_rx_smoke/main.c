#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ns_sdk_hal.h"
#include "utb_hal.h"

#define UTB_RX_SMOKE_FIXED_LEN          472u
#define UTB_RX_SMOKE_MAC_TIMER          0x8000u
#define UTB_RX_SMOKE_AHB_TIMER          0xC350u
#define UTB_RX_SMOKE_SPI_CLK_DIV        8u

#ifndef CFG_SIMULATION
#define UTB_RX_SMOKE_WAIT_LOOPS         5000000u
#define UTB_RX_SMOKE_PROGRESS_STEP      500000u
#else
#define UTB_RX_SMOKE_WAIT_LOOPS         200000u
#define UTB_RX_SMOKE_PROGRESS_STEP      20000u
#endif

static uint8_t g_rx_slice_buf[UTB_RX_SMOKE_FIXED_LEN];

static void utb_rx_smoke_dump_mac_snapshot(const char *tag)
{
    utb_hal_mac_fifo_slice_state_t rx_fifo_state;
    utb_hal_mac_fifo_byte_state_t rx_fifo_bytes;
    utb_hal_mac_path_state_t rx_path_state;
    uint32_t int_status;

    int_status = utb_hal_mac_get_int_status();
    utb_hal_mac_get_rx_fifo_slice_state(&rx_fifo_state);
    utb_hal_mac_get_rx_fifo_byte_state(&rx_fifo_bytes);
    utb_hal_mac_get_rx_path_state(&rx_path_state);

    printf("[UTB_MAC_RX_SMOKE] %s int=0x%08lx rx_avail=%u rx_commit=%u rx_unwritten=%u rx_written=%u rx_state=%u ahb_bytes=%u mac_bytes=%u\r\n",
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

static void utb_rx_smoke_dump_mac_stats(const char *tag)
{
    utb_hal_mac_stats_t stats;

    memset(&stats, 0, sizeof(stats));
    utb_hal_mac_get_stats(&stats);

    printf("[UTB_MAC_RX_SMOKE] %s phy_len_wrong=%lu tx_full_wrong=%lu rx_full_wrong=%lu rx_timeout=%lu mcu_len_right=%lu total_2phy=%lu valid_2phy=%lu tx_timeout=%lu phy_req_len_wrong=%lu rx_overflow=%lu ahb_rx_timeout=%lu\r\n",
           tag,
           (unsigned long)stats.phy_len_wrong_num,
           (unsigned long)stats.tx_full_write_wrong_num,
           (unsigned long)stats.rx_full_write_wrong_num,
           (unsigned long)stats.rx_timeout_wrong_num,
           (unsigned long)stats.mcu_len_right_num,
           (unsigned long)stats.total_slice_num_2phy,
           (unsigned long)stats.valid_slice_num_2phy,
           (unsigned long)stats.tx_timeout_wrong_num,
           (unsigned long)stats.phy_req_len_wrong_num,
           (unsigned long)stats.rx_fifo_overflow_symb_num,
           (unsigned long)stats.ahb_rx_timeout_num);
}

static void utb_rx_smoke_fail(const char *reason)
{
    printf("[UTB_MAC_RX_SMOKE] fail: %s\r\n", reason);
    utb_rx_smoke_dump_mac_snapshot("fail-snapshot");
    utb_rx_smoke_dump_mac_stats("fail-stats");
    simulation_fail();
    while (1) {
    }
}

static void utb_rx_smoke_iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_SCK_IOF_IVAL, QSPI1_SCK_PAD_SEL, QSPI1_SCK_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_CS_0_IOF_IVAL, QSPI1_CS_0_PAD_SEL, QSPI1_CS_0_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_IVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_OVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_IVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_OVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
}

static utb_hal_spi_handle_t *utb_rx_smoke_open_spi(void)
{
    utb_hal_spi_cfg_t spi_cfg;

    memset(&spi_cfg, 0, sizeof(spi_cfg));
    spi_cfg.bus = UTB_HAL_SPI_BUS_QSPI1;
    spi_cfg.master = true;
    spi_cfg.mode = 1u;
    spi_cfg.bits_per_word = 8u;
    spi_cfg.lsb_first = false;
    spi_cfg.clock_div = UTB_RX_SMOKE_SPI_CLK_DIV;

    return utb_hal_spi_open(&spi_cfg);
}

static int utb_rx_smoke_wait_slice_ready(uint32_t *int_status_out,
                                         utb_hal_mac_fifo_slice_state_t *fifo_state_out)
{
    uint32_t loops;

    for (loops = 0; loops < UTB_RX_SMOKE_WAIT_LOOPS; loops++) {
        uint32_t int_status;
        utb_hal_mac_fifo_slice_state_t fifo_state;

        int_status = utb_hal_mac_get_int_status();
        utb_hal_mac_get_rx_fifo_slice_state(&fifo_state);

        if (((int_status & (1u << UTB_HAL_MAC_INT_RX_SLICE_READABLE)) != 0u) ||
            (fifo_state.committed_slices != 0u)) {
            if (int_status_out != NULL) {
                *int_status_out = int_status;
            }
            if (fifo_state_out != NULL) {
                *fifo_state_out = fifo_state;
            }
            return UTB_OK;
        }

        if ((loops != 0u) && ((loops % UTB_RX_SMOKE_PROGRESS_STEP) == 0u)) {
            printf("[UTB_MAC_RX_SMOKE] wait loops=%lu int=0x%08lx rx_commit=%u\r\n",
                   (unsigned long)loops,
                   (unsigned long)int_status,
                   (unsigned)fifo_state.committed_slices);
        }
    }

    return UTB_ERR_TIMEOUT;
}

static void utb_rx_smoke_read_one_slice(uint8_t *buf, uint32_t len)
{
    uint32_t i;

    for (i = 0; i < len; i++) {
        buf[i] = utb_hal_mac_read_symb8();
    }
}

static void utb_rx_smoke_dump_slice_edges(const uint8_t *buf, uint32_t len)
{
    uint32_t i;
    uint32_t start_count;
    uint32_t tail_start;

    if ((buf == NULL) || (len == 0u)) {
        return;
    }

    start_count = (len < 8u) ? len : 8u;
    tail_start = (len > 8u) ? (len - 8u) : 0u;

    printf("[UTB_MAC_RX_SMOKE] head:");
    for (i = 0; i < start_count; i++) {
        printf(" %02x", buf[i]);
    }
    printf("\r\n");

    printf("[UTB_MAC_RX_SMOKE] tail:");
    for (i = tail_start; i < len; i++) {
        printf(" %02x", buf[i]);
    }
    printf("\r\n");
}

int main(void)
{
    utb_hal_mac_cfg_t mac_cfg;
    utb_hal_spi_handle_t *spi_handle;
    utb_hal_mac_fifo_slice_state_t rx_fifo_state;
    uint32_t int_status;
    int ret;

    memset(g_rx_slice_buf, 0, sizeof(g_rx_slice_buf));
    memset(&mac_cfg, 0, sizeof(mac_cfg));
    mac_cfg.fixed_length = UTB_RX_SMOKE_FIXED_LEN;
    mac_cfg.mac_timer = UTB_RX_SMOKE_MAC_TIMER;
    mac_cfg.ahb_symb_timer = UTB_RX_SMOKE_AHB_TIMER;
    mac_cfg.mac_en = 1u;

    utb_rx_smoke_iomux_config();

    ret = utb_hal_mac_init(&mac_cfg);
    if (ret != UTB_OK) {
        printf("[UTB_MAC_RX_SMOKE] mac init failed: %d\r\n", ret);
        utb_rx_smoke_fail("mac init");
    }

    spi_handle = utb_rx_smoke_open_spi();
    if (spi_handle == NULL) {
        utb_rx_smoke_fail("spi open");
    }

    utb_hal_mac_clear_ints(0xFFFFu);
    utb_hal_mac_clear_stats();
    utb_rx_smoke_dump_mac_snapshot("start");

    printf("[UTB_MAC_RX_SMOKE] wait rx slice len=%u\r\n", (unsigned)UTB_RX_SMOKE_FIXED_LEN);
    ret = utb_rx_smoke_wait_slice_ready(&int_status, &rx_fifo_state);
    if (ret != UTB_OK) {
        printf("[UTB_MAC_RX_SMOKE] rx wait timeout\r\n");
        utb_rx_smoke_fail("wait rx slice");
    }

    printf("[UTB_MAC_RX_SMOKE] rx ready int=0x%08lx rx_avail=%u rx_commit=%u\r\n",
           (unsigned long)int_status,
           (unsigned)rx_fifo_state.available_slices,
           (unsigned)rx_fifo_state.committed_slices);
    utb_rx_smoke_dump_mac_snapshot("before-read");

    utb_rx_smoke_read_one_slice(g_rx_slice_buf, UTB_RX_SMOKE_FIXED_LEN);
    utb_rx_smoke_dump_slice_edges(g_rx_slice_buf, UTB_RX_SMOKE_FIXED_LEN);
    utb_hal_mac_clear_ints(1u << UTB_HAL_MAC_INT_RX_SLICE_READABLE);
    utb_rx_smoke_dump_mac_snapshot("after-read");
    utb_rx_smoke_dump_mac_stats("after-read");

    ret = utb_hal_spi_transfer(spi_handle, g_rx_slice_buf, NULL, UTB_RX_SMOKE_FIXED_LEN);
    if (ret != UTB_OK) {
        printf("[UTB_MAC_RX_SMOKE] spi transfer failed: %d\r\n", ret);
        utb_rx_smoke_fail("spi transfer");
    }

    utb_rx_smoke_dump_mac_snapshot("after-spi");
    utb_rx_smoke_dump_mac_stats("after-spi");
    printf("[UTB_MAC_RX_SMOKE] pass\r\n");
    simulation_pass();
    while (1) {
    }
}

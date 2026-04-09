#include <string.h>

#include "nuclei_sdk_soc.h"
#include "fpga_eval.h"
#include "utb_hal.h"

#include "dp_spi.h"

#define DP_SPI_CLK_DIV              8u

static utb_hal_spi_handle_t s_dp_spi_handle;
static utb_hal_spi_static_storage_t s_dp_spi_storage;

void dp_spi_iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_SCK_IOF_IVAL, QSPI1_SCK_PAD_SEL, QSPI1_SCK_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_CS_0_IOF_IVAL, QSPI1_CS_0_PAD_SEL, QSPI1_CS_0_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_IVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_OVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_IVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_OVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
}

int dp_spi_init(void)
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

int dp_spi_transfer(const uint8_t *tx_buf, uint32_t len)
{
    return utb_hal_spi_transfer(&s_dp_spi_handle, tx_buf, NULL, len);
}

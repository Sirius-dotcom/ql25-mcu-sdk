/**
  * Copyright (c) 2019 Nuclei Limited. All rights reserved.
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  * Licensed under the Apache License, Version 2.0 (the License); you may
  * not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  * www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an AS IS BASIS, WITHOUT
  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */

#include <stdio.h>
#include <string.h>
#include "ns_sdk_hal.h"

#define PKT_LEN                 38U
#define PKT_LEN_LO              ((uint8_t)(PKT_LEN & 0xFFU))
#define PKT_LEN_HI              ((uint8_t)((PKT_LEN >> 8) & 0xFFU))
#define DMA_TIMEOUT             3000000000UL
#define SRAM_BUF_BASE           (SRAM0_MEM_BASE + 0x1000U)
#define EXP_BUF_ADDR            (SRAM_BUF_BASE)
#define RX_BUF_ADDR             (SRAM_BUF_BASE + 0x0400U)
#define RDBACK_BUF_ADDR         (SRAM_BUF_BASE + 0x0800U)

static uint8_t *const expected_buf = (uint8_t *)EXP_BUF_ADDR;
static uint8_t *const rx_buf = (uint8_t *)RX_BUF_ADDR;
static uint8_t *const rdback_buf = (uint8_t *)RDBACK_BUF_ADDR;

static void build_expected_payload(void)
{
    uint32_t i;

    expected_buf[0] = 0x55;
    expected_buf[1] = 0xAA;
    expected_buf[2] = PKT_LEN_LO;
    expected_buf[3] = PKT_LEN_HI;

    for (i = 4; i < PKT_LEN; i++) {
        expected_buf[i] = (uint8_t)((i ^ 0x5A) & 0xFFU);
    }
}

static int compare_buf(const uint8_t *lhs, const uint8_t *rhs, uint32_t len, uint32_t *mismatch_idx)
{
    uint32_t i;

    for (i = 0; i < len; i++) {
        if (lhs[i] != rhs[i]) {
            if (mismatch_idx != NULL) {
                *mismatch_idx = i;
            }
            return 0;
        }
    }
    return 1;
}

static void dump_head_tail(const char *tag, const uint8_t *buf)
{
    int i;

    printf("%s head:", tag);
    for (i = 0; i < 16; i++) {
        printf(" %02x", buf[i]);
    }
    printf("\r\n");

    printf("%s tail:", tag);
    for (i = (int)PKT_LEN - 16; i < (int)PKT_LEN; i++) {
        printf(" %02x", buf[i]);
    }
    printf("\r\n");
}

static void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_SCK_IOF_IVAL, QSPI1_SCK_PAD_SEL, QSPI1_SCK_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_CS_0_IOF_IVAL, QSPI1_CS_0_PAD_SEL, QSPI1_CS_0_HS_SEL, 0, 0);

    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_IVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_OVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);

    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_IVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_OVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
}

static void spi_master_init(void)
{
    QSPI_InitTypeDef spi_init_struct = {0};

    QSPI_StructInit(&spi_init_struct);
    spi_init_struct.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    spi_init_struct.DataSize = QSPI_FMT_LEN_8B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_LOW;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE2;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    spi_init_struct.DevMode = (QSPI_CR_MODE_MASTER | QSPI_CR_DMA_ENABLE);

    QSPI_Init(QSPI1, &spi_init_struct);
    QSPI1->RSIZE = PKT_LEN;
    QSPI_DirectionConfig(QSPI1, QSPI_FMT_DIR_RX);
    QSPI_ClearFifo(QSPI1, QSPI_RX_FIFO);
    QSPI_ClearFifo(QSPI1, QSPI_TX_FIFO);
}

static void udma_rx_config(void)
{
    UDMA_PAM2MTypeDef udma_rx = {0};

    UDMA_PA2M_ClearITStatus(QSPI1_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_CLEAR_STAT);

    udma_rx.UDMA_TransEn = PA2M_TRANS_ENABLE;
    udma_rx.UDMA_DstBaseAddr = CAL_ADDR_LO(rx_buf);
    udma_rx.UDMA_SrcBaseAddr = CAL_ADDR_LO(&(QSPI1->RXDATA));
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    udma_rx.UDMA_DstBaseAddr_H = CAL_ADDR_HI(rx_buf);
    udma_rx.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(&(QSPI1->RXDATA));
#endif
    udma_rx.UDMA_BufferSize = PKT_LEN;
    udma_rx.UDMA_DstInc = PA2M_MDNA_ENABLE;
    udma_rx.UDMA_SrcInc = PA2M_MSNA_DISABLE;
    udma_rx.UDMA_Width = PA2M_MDWIDTH_8BIT;
    udma_rx.UDMA_Mode = PA2M_MODE_NORMAL;
    udma_rx.UDMA_PER_SEL = UDMA_SEL_QSPI1_RX_DMA;
    UDMA_PAM2M_Init(QSPI1_RX_DMA_DMA_CH, &udma_rx);
}

static int wait_udma_rx_done(void)
{
    unsigned long timeout = DMA_TIMEOUT;

    while (!UDMA_PA2M_GetITStatus(QSPI1_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_STAT)) {
        if (timeout-- == 0UL) {
            return 0;
        }
    }

    UDMA_PA2M_ClearITStatus(QSPI1_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_CLEAR_STAT);
    return 1;
}

static int spi_send_payload(void)
{
    uint32_t i;
    unsigned long timeout;

    for (i = 0; i < PKT_LEN; i++) {
        timeout = DMA_TIMEOUT;
        while (SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_FULL)) {
            if (timeout-- == 0UL) {
                return 0;
            }
        }
        QSPI_SendData(QSPI1, expected_buf[i]);
    }

    return 1;
}

static void copy_to_ahb_slv0(void)
{
    volatile uint8_t *const ahb_slv0 = (volatile uint8_t *)AHB_SLV0_RATIO_MEM_BASE;
    uint32_t i;

    for (i = 0; i < PKT_LEN; i++) {
        ahb_slv0[i] = rx_buf[i];
    }
}

static void readback_from_ahb_slv0(void)
{
    volatile uint8_t *const ahb_slv0 = (volatile uint8_t *)AHB_SLV0_RATIO_MEM_BASE;
    uint32_t i;

    for (i = 0; i < PKT_LEN; i++) {
        rdback_buf[i] = ahb_slv0[i];
    }
}

void main(void)
{
    uint32_t mismatch_idx = 0;

#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    MFlushInvalDCache();
    DisableDCache();
#endif

#ifdef MISC_HAS_QSPI1_HAS_CLK
    qspi1_clk_en(ENABLE);
#endif
#ifdef MISC_HAS_QSPI1_RST
    qspi1_set_rst(DISABLE);
    qspi1_set_rst(ENABLE);
#endif
#ifdef MISC_HAS_QSPI1_CLK_DIV
    qspi1_clk_div(0);
#endif

#ifdef MISC_HAS_UDMA0_HAS_CLK
    udma0_clk_en(ENABLE);
#endif
#ifdef MISC_HAS_UDMA0_RST
    udma0_set_rst(DISABLE);
    udma0_set_rst(ENABLE);
#endif
#ifdef MISC_HAS_UDMA0_CLK_DIV
    udma0_clk_div(0);
#endif

    memset(expected_buf, 0x00, PKT_LEN);
    memset(rx_buf, 0x00, PKT_LEN);
    memset(rdback_buf, 0x00, PKT_LEN);
    build_expected_payload();

    iomux_config();
    UDMA_PA2M_ITConfig(QSPI1_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);

    printf("[SPI_RX_DMA_AHB] start\r\n");
    printf("[SPI_RX_DMA_AHB] self loopback tx/rx len=%u\r\n", PKT_LEN);

    spi_master_init();
    udma_rx_config();
    QSPI_RxDmaStart(QSPI1, ENABLE);

    if (!spi_send_payload()) {
        printf("[SPI_RX_DMA_AHB] tx timeout\r\n");
        QSPI_RxDmaStart(QSPI1, DISABLE);
        simulation_fail();
        while (1) {}
    }

    if (!wait_udma_rx_done()) {
        printf("[SPI_RX_DMA_AHB] rx dma timeout\r\n");
        QSPI_RxDmaStart(QSPI1, DISABLE);
        simulation_fail();
        while (1) {}
    }

    QSPI_RxDmaStart(QSPI1, DISABLE);
    printf("[SPI_RX_DMA_AHB] tx done\r\n");
    printf("[SPI_RX_DMA_AHB] rx dma done\r\n");

    if (!compare_buf(expected_buf, rx_buf, PKT_LEN, &mismatch_idx)) {
        printf("[SPI_RX_DMA_AHB] sram compare fail idx=%u exp=%02x got=%02x\r\n",
               mismatch_idx, expected_buf[mismatch_idx], rx_buf[mismatch_idx]);
        dump_head_tail("expected", expected_buf);
        dump_head_tail("rx_buf", rx_buf);
        simulation_fail();
        while (1) {}
    }

    printf("[SPI_RX_DMA_AHB] sram compare pass\r\n");

    copy_to_ahb_slv0();
    readback_from_ahb_slv0();

    if (!compare_buf(expected_buf, rdback_buf, PKT_LEN, &mismatch_idx)) {
        printf("[SPI_RX_DMA_AHB] ahb compare fail idx=%u exp=%02x got=%02x\r\n",
               mismatch_idx, expected_buf[mismatch_idx], rdback_buf[mismatch_idx]);
        dump_head_tail("expected", expected_buf);
        dump_head_tail("ahb_rdback", rdback_buf);
        simulation_fail();
        while (1) {}
    }

    printf("[SPI_RX_DMA_AHB] ahb compare pass\r\n");
    printf("[SPI_RX_DMA_AHB] PASS\r\n");
    simulation_pass();

    while (1) {}
}

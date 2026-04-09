/**
  * Copyright (c) 2019 Nuclei Limited. All rights reserved.
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include <stdio.h>
#include <string.h>
#include "ns_sdk_hal.h"

#define PKT_LEN                     472U
#define DMA_TIMEOUT                 3000000000UL
#define UTB_LOOPBACK_WAIT           2000000UL

#define SRAM_BUF_BASE               (SRAM0_MEM_BASE + 0x1000U)
#define EXP_BUF_ADDR                (SRAM_BUF_BASE + 0x0000U)
#define SPI_CPU_RX_BUF_ADDR         (SRAM_BUF_BASE + 0x0400U)
#define UTB_RX_BUF_ADDR             (SRAM_BUF_BASE + 0x0800U)
#define SPI_DMA_RX_BUF_ADDR         (SRAM_BUF_BASE + 0x0C00U)

#define UTB_MAC_BASE                AHB_SLV0_RATIO_MEM_BASE
#define UTB_PHY_BASE                AHB_SLV1_RATIO_MEM_BASE

#define UTB_MAC_TX_SYMB             (UTB_MAC_BASE + 0x0100U)
#define UTB_MAC_RX_SYMB             (UTB_MAC_BASE + 0x0104U)
#define UTB_MAC_CTRL                (UTB_MAC_BASE + 0x0000U)
#define UTB_MAC_FIXED_LENGTH        (UTB_MAC_BASE + 0x000CU)

#define UTB_PHY_CONFIGURATION       (UTB_PHY_BASE + 0x0000U)
#define UTB_PHY_MODE_CONFIGURATION  (UTB_PHY_BASE + 0x0004U)
#define UTB_PHY_ENC_MCS             (UTB_PHY_BASE + 0x0008U)
#define UTB_PHY_ENC_CRC24           (UTB_PHY_BASE + 0x000CU)
#define UTB_PHY_SBB_CONFIGURATION   (UTB_PHY_BASE + 0x0010U)
#define UTB_PHY_MSN_CONFIGURATION   (UTB_PHY_BASE + 0x0014U)
#define UTB_PHY_CC_CONFIGURATION    (UTB_PHY_BASE + 0x0018U)
#define UTB_PHY_OFDMC_CONFIGURATION1 (UTB_PHY_BASE + 0x001CU)
#define UTB_PHY_OFDMC_CONFIGURATION2 (UTB_PHY_BASE + 0x0020U)
#define UTB_PHY_OFDMD_CONFIGURATION1 (UTB_PHY_BASE + 0x0024U)
#define UTB_PHY_OFDMD_CONFIGURATION2 (UTB_PHY_BASE + 0x0028U)
#define UTB_PHY_RX_CONFIGURATION    (UTB_PHY_BASE + 0x002CU)
#define UTB_PHY_RX_NND              (UTB_PHY_BASE + 0x0030U)
#define UTB_PHY_RX_NODE_CTRL        (UTB_PHY_BASE + 0x0034U)
#define UTB_PHY_RX_ABN_MAX          (UTB_PHY_BASE + 0x0038U)
#define UTB_PHY_RX_DP11             (UTB_PHY_BASE + 0x003CU)
#define UTB_PHY_RX_FRM_CTRL1        (UTB_PHY_BASE + 0x0040U)
#define UTB_PHY_RX_FRM_CTRL2        (UTB_PHY_BASE + 0x0044U)
#define UTB_PHY_RX_FRM_CTRL3        (UTB_PHY_BASE + 0x0048U)
#define UTB_PHY_RX_SFRM_CTRL1       (UTB_PHY_BASE + 0x004CU)
#define UTB_PHY_RX_SFRM_CTRL2       (UTB_PHY_BASE + 0x0050U)
#define UTB_PHY_RX_SFRM_CTRL3       (UTB_PHY_BASE + 0x0054U)
#define UTB_PHY_RX_SFRM_CTRL4       (UTB_PHY_BASE + 0x0058U)
#define UTB_PHY_RX_SFRM_CTRL5       (UTB_PHY_BASE + 0x005CU)
#define UTB_PHY_TEST_CONFIGURATION  (UTB_PHY_BASE + 0x0168U)
#define UTB_PHY_TX_MEM_BASE         (UTB_PHY_BASE + 0x0800U)

static uint8_t *const expected_buf = (uint8_t *)EXP_BUF_ADDR;
static uint8_t *const spi_cpu_rx_buf = (uint8_t *)SPI_CPU_RX_BUF_ADDR;
static uint8_t *const utb_rx_buf = (uint8_t *)UTB_RX_BUF_ADDR;
static uint8_t *const spi_dma_rx_buf = (uint8_t *)SPI_DMA_RX_BUF_ADDR;

static inline void write_reg32(uint32_t addr, uint32_t value)
{
    REG32(addr) = value;
}

static void build_expected_payload(void)
{
    uint32_t i;

    expected_buf[0] = 0x55U;
    expected_buf[1] = 0xAAU;
    expected_buf[2] = (uint8_t)(PKT_LEN & 0xFFU);
    expected_buf[3] = (uint8_t)((PKT_LEN >> 8) & 0xFFU);

    for (i = 4; i < PKT_LEN; i++) {
        expected_buf[i] = (uint8_t)((i ^ 0x5AU) & 0xFFU);
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

static void fail_with_compare(const char *stage, const uint8_t *exp, const uint8_t *got, uint32_t mismatch_idx)
{
    printf("[%s] compare fail idx=%u exp=%02x got=%02x\r\n",
           stage, mismatch_idx, exp[mismatch_idx], got[mismatch_idx]);
    dump_head_tail("expected", exp);
    dump_head_tail("actual", got);
    simulation_fail();
    while (1) {}
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

static void spi_rx_dma_config(uint8_t *dst_buf)
{
    UDMA_PAM2MTypeDef udma_rx = {0};

    UDMA_PA2M_ClearITStatus(QSPI1_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_CLEAR_STAT);

    udma_rx.UDMA_TransEn = PA2M_TRANS_ENABLE;
    udma_rx.UDMA_DstBaseAddr = CAL_ADDR_LO(dst_buf);
    udma_rx.UDMA_SrcBaseAddr = CAL_ADDR_LO(&(QSPI1->RXDATA));
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    udma_rx.UDMA_DstBaseAddr_H = CAL_ADDR_HI(dst_buf);
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

static void spi_tx_dma_config(const uint8_t *src_buf)
{
    UDMA_PAM2MTypeDef udma_tx = {0};

    UDMA_PA2M_ClearITStatus(QSPI1_TX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_CLEAR_STAT);

    udma_tx.UDMA_TransEn = PA2M_TRANS_ENABLE;
    udma_tx.UDMA_DstBaseAddr = CAL_ADDR_LO(&(QSPI1->TXDATA));
    udma_tx.UDMA_SrcBaseAddr = CAL_ADDR_LO(src_buf);
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    udma_tx.UDMA_DstBaseAddr_H = CAL_ADDR_HI(&(QSPI1->TXDATA));
    udma_tx.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(src_buf);
#endif
    udma_tx.UDMA_BufferSize = PKT_LEN;
    udma_tx.UDMA_DstInc = PA2M_MDNA_DISABLE;
    udma_tx.UDMA_SrcInc = PA2M_MSNA_ENABLE;
    udma_tx.UDMA_Width = PA2M_MDWIDTH_8BIT;
    udma_tx.UDMA_Mode = PA2M_MODE_NORMAL;
    udma_tx.UDMA_PER_SEL = UDMA_SEL_QSPI1_TX_DMA;
    UDMA_PAM2M_Init(QSPI1_TX_DMA_DMA_CH, &udma_tx);
}

static int wait_pa2m_done(UDMA_PA_CHx_IRQ_TypeDef *irq)
{
    unsigned long timeout = DMA_TIMEOUT;

    while (!UDMA_PA2M_GetITStatus(irq, PA2M_FTRANS_IRQ_STAT)) {
        if (timeout-- == 0UL) {
            return 0;
        }
    }

    UDMA_PA2M_ClearITStatus(irq, PA2M_FTRANS_IRQ_CLEAR_STAT);
    return 1;
}

static int spi_send_payload_cpu(const uint8_t *src_buf)
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
        QSPI_SendData(QSPI1, src_buf[i]);
    }

    return 1;
}

static int spi_loopback_cpu_tx(const uint8_t *src_buf, uint8_t *dst_buf)
{
    spi_master_init();
    spi_rx_dma_config(dst_buf);
    QSPI_RxDmaStart(QSPI1, ENABLE);

    if (!spi_send_payload_cpu(src_buf)) {
        QSPI_RxDmaStart(QSPI1, DISABLE);
        return 0;
    }

    if (!wait_pa2m_done(QSPI1_RX_DMA_DMA_IRQ)) {
        QSPI_RxDmaStart(QSPI1, DISABLE);
        return 0;
    }

    QSPI_RxDmaStart(QSPI1, DISABLE);
    return 1;
}

static int spi_loopback_dma_tx(const uint8_t *src_buf, uint8_t *dst_buf)
{
    spi_master_init();
    spi_rx_dma_config(dst_buf);
    spi_tx_dma_config(src_buf);

    QSPI_RxDmaStart(QSPI1, ENABLE);
    QSPI_TxDmaStart(QSPI1, ENABLE);

    if (!wait_pa2m_done(QSPI1_RX_DMA_DMA_IRQ)) {
        QSPI_TxDmaStart(QSPI1, DISABLE);
        QSPI_RxDmaStart(QSPI1, DISABLE);
        return 0;
    }

    if (!wait_pa2m_done(QSPI1_TX_DMA_DMA_IRQ)) {
        QSPI_TxDmaStart(QSPI1, DISABLE);
        QSPI_RxDmaStart(QSPI1, DISABLE);
        return 0;
    }

    QSPI_TxDmaStart(QSPI1, DISABLE);
    QSPI_RxDmaStart(QSPI1, DISABLE);
    return 1;
}

static int udma_m2m_transfer(const void *src, void *dst, uint32_t len,
                             uint32_t src_inc, uint32_t dst_inc,
                             uint32_t src_width, uint32_t dst_width)
{
    UDMA_InitTypeDef udma_init = {0};
    unsigned long timeout = DMA_TIMEOUT;

    UDMA_ClearITStatus(UDMA0_CH0_M2M_IRQ, UDMA_FTRANS_IRQ);

    UDMA_StructInit(&udma_init);
    udma_init.UDMA_DstBaseAddr = (uint32_t)CAL_ADDR((uintptr_t)dst);
    udma_init.UDMA_SrcBaseAddr = (uint32_t)CAL_ADDR((uintptr_t)src);
    udma_init.UDMA_DstBaseAddr_h = 0x0U;
    udma_init.UDMA_SrcBaseAddr_h = 0x0U;
    udma_init.UDMA_BufferSize = len;
    udma_init.UDMA_DstInc = dst_inc;
    udma_init.UDMA_SrcInc = src_inc;
    udma_init.UDMA_SrcWidth = src_width;
    udma_init.UDMA_DstWidth = dst_width;
    udma_init.UDMA_Mode = UDMA_MODE_NORMAL;
    udma_init.UDMA_Priority = UDMA_PRIORITY_LOW;

    UDMA_Init(UDMA0_M2M_CH0, &udma_init);
    M2M_DMA_Cmd(UDMA0_M2M_CH0, ENABLE);

    while (UDMA_GetITStatus(UDMA0_CH0_M2M_IRQ, UDMA_FTRANS_IRQ) == RESET) {
        if (timeout-- == 0UL) {
            M2M_DMA_Cmd(UDMA0_M2M_CH0, DISABLE);
            return 0;
        }
    }

    UDMA_ClearITStatus(UDMA0_CH0_M2M_IRQ, UDMA_FTRANS_IRQ_CLR);
    M2M_DMA_Cmd(UDMA0_M2M_CH0, DISABLE);
    return 1;
}

static void utb_phy_tx_mem_config(void)
{
    uint32_t frame;
    uint32_t sym;
    uint32_t value;

    for (frame = 0; frame < 8U; frame++) {
        for (sym = 0; sym < 64U; sym++) {
            if (sym == 0U) {
                value = 1U;
            } else if (sym == 1U) {
                value = 2U;
            } else if (sym <= 7U) {
                value = 0U;
            } else if (sym == 8U) {
                value = 1U;
            } else if (sym <= 61U) {
                value = 3U;
            } else {
                value = 0U;
            }

            write_reg32(UTB_PHY_TX_MEM_BASE + ((frame * 64U + sym) * 4U), value);
        }
    }
}

static void utb_mac_phy_config(void)
{
    write_reg32(UTB_MAC_CTRL, 0x0001C350U);
    write_reg32(UTB_MAC_FIXED_LENGTH, 0x013AFFFFU);

    write_reg32(UTB_PHY_CONFIGURATION, 0x00111100U);
    write_reg32(UTB_PHY_MODE_CONFIGURATION, 0x00000300U);
    write_reg32(UTB_PHY_ENC_MCS, 0x006DB6DBU);
    write_reg32(UTB_PHY_ENC_CRC24, 0x0001BB6DU);
    write_reg32(UTB_PHY_SBB_CONFIGURATION, 0x00000080U);
    write_reg32(UTB_PHY_MSN_CONFIGURATION, 0x00000010U);
    write_reg32(UTB_PHY_CC_CONFIGURATION, 0x00000000U);
    write_reg32(UTB_PHY_OFDMC_CONFIGURATION1, 0x00010900U);
    write_reg32(UTB_PHY_OFDMC_CONFIGURATION2, 0x00009BC0U);
    write_reg32(UTB_PHY_OFDMD_CONFIGURATION1, 0x00021200U);
    write_reg32(UTB_PHY_OFDMD_CONFIGURATION2, 0x00013AC0U);
    write_reg32(UTB_PHY_RX_CONFIGURATION, 0x00100000U);
    write_reg32(UTB_PHY_RX_NND, 0x00000000U);
    write_reg32(UTB_PHY_RX_NODE_CTRL, 0x00000000U);
    write_reg32(UTB_PHY_RX_ABN_MAX, 0x00000000U);
    write_reg32(UTB_PHY_RX_DP11, 0x0D1B7175U);
    write_reg32(UTB_PHY_RX_FRM_CTRL1, 0x04000800U);
    write_reg32(UTB_PHY_RX_FRM_CTRL2, 0x02000200U);
    write_reg32(UTB_PHY_RX_FRM_CTRL3, 0x00000080U);
    write_reg32(UTB_PHY_RX_SFRM_CTRL1, 0xAAA90000U);
    write_reg32(UTB_PHY_RX_SFRM_CTRL2, 0xAAAAAAAAU);
    write_reg32(UTB_PHY_RX_SFRM_CTRL3, 0xAAAAAAAAU);
    write_reg32(UTB_PHY_RX_SFRM_CTRL4, 0xAAAAAAAU);
    write_reg32(UTB_PHY_RX_SFRM_CTRL5, 0x00000000U);
    write_reg32(UTB_PHY_TEST_CONFIGURATION, 0x00000800U);

    utb_phy_tx_mem_config();
}

static int utb_loopback_transfer(const uint8_t *src_buf, uint8_t *dst_buf)
{
    volatile uint32_t *const mac_tx = (volatile uint32_t *)UTB_MAC_TX_SYMB;
    volatile uint32_t *const mac_rx = (volatile uint32_t *)UTB_MAC_RX_SYMB;
    unsigned long timeout = UTB_LOOPBACK_WAIT;

    if ((PKT_LEN & 0x3U) != 0U) {
        return 0;
    }

    if (!udma_m2m_transfer(src_buf, (void *)mac_tx, PKT_LEN / 4U,
                           UDMA_MSNA_ENABLE, UDMA_MDNA_DISABLE,
                           UDMA_MSWIDTH_32BIT, UDMA_MDWIDTH_32BIT)) {
        return 0;
    }

    while (timeout-- != 0UL) {
        __asm volatile("nop");
    }

    if (!udma_m2m_transfer((const void *)mac_rx, dst_buf, PKT_LEN / 4U,
                           UDMA_MSNA_DISABLE, UDMA_MDNA_ENABLE,
                           UDMA_MSWIDTH_32BIT, UDMA_MDWIDTH_32BIT)) {
        return 0;
    }

    return 1;
}

void main(void)
{
    uint32_t mismatch_idx = 0U;

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
    memset(spi_cpu_rx_buf, 0x00, PKT_LEN);
    memset(utb_rx_buf, 0x00, PKT_LEN);
    memset(spi_dma_rx_buf, 0x00, PKT_LEN);
    build_expected_payload();
    utb_mac_phy_config();
    iomux_config();
    UDMA_PA2M_ITConfig(QSPI1_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);
    UDMA_PA2M_ITConfig(QSPI1_TX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);
    UDMA_ITConfig(UDMA0_CH0_M2M_IRQ, UDMA_FTRANS_IRQ, ENABLE);

    printf("[UTB_MAC_PHY_LOOPBACK] start len=%u\r\n", PKT_LEN);

    if (!spi_loopback_cpu_tx(expected_buf, spi_cpu_rx_buf)) {
        printf("[UTB_MAC_PHY_LOOPBACK] spi cpu loopback timeout\r\n");
        simulation_fail();
        while (1) {}
    }

    if (!compare_buf(expected_buf, spi_cpu_rx_buf, PKT_LEN, &mismatch_idx)) {
        fail_with_compare("SPI_CPU_LOOPBACK", expected_buf, spi_cpu_rx_buf, mismatch_idx);
    }
    printf("[UTB_MAC_PHY_LOOPBACK] spi cpu loopback pass\r\n");

    if (!utb_loopback_transfer(spi_cpu_rx_buf, utb_rx_buf)) {
        printf("[UTB_MAC_PHY_LOOPBACK] utb loopback timeout\r\n");
        simulation_fail();
        while (1) {}
    }

    if (!compare_buf(expected_buf, utb_rx_buf, PKT_LEN, &mismatch_idx)) {
        fail_with_compare("UTB_LOOPBACK", expected_buf, utb_rx_buf, mismatch_idx);
    }
    printf("[UTB_MAC_PHY_LOOPBACK] utb mac/phy loopback pass\r\n");

    if (!spi_loopback_dma_tx(utb_rx_buf, spi_dma_rx_buf)) {
        printf("[UTB_MAC_PHY_LOOPBACK] spi dma loopback timeout\r\n");
        simulation_fail();
        while (1) {}
    }

    if (!compare_buf(expected_buf, spi_dma_rx_buf, PKT_LEN, &mismatch_idx)) {
        fail_with_compare("SPI_DMA_LOOPBACK", expected_buf, spi_dma_rx_buf, mismatch_idx);
    }
    printf("[UTB_MAC_PHY_LOOPBACK] spi dma loopback pass\r\n");

    printf("[UTB_MAC_PHY_LOOPBACK] PASS\r\n");
    simulation_pass();

    while (1) {}
}

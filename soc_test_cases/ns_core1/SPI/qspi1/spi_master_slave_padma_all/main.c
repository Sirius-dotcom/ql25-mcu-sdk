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

#define ARRAYSIZE    (10)
uint32_t *dst_base_addr1, *src_base_addr1, *dst_base_addr2, *src_base_addr2;
/**
  * \brief memory compare function
  *
  * \param src: source data pointer
  * \param dst: destination data pointer
  * \param length: the compare data length
  *
  * \retval ErrStatus: ERROR or SUCCESS
  */
ErrStatus MemoryCompare(uint8_t* src, uint8_t* dst, uint16_t length)
{
    while (length--) {
        if (*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

void iomux_config(void)
{
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_SCK_IOF_OVAL, QSPI1_SCK_PAD_SEL, QSPI1_SCK_HS_SEL, 0, 0);
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_CS_0_IOF_OVAL, QSPI1_CS_0_PAD_SEL, QSPI1_CS_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_IVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_OVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_IVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_OVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_2_IOF_IVAL, QSPI1_DQ_2_PAD_SEL, QSPI1_DQ_2_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_2_IOF_OVAL, QSPI1_DQ_2_PAD_SEL, QSPI1_DQ_2_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_3_IOF_IVAL, QSPI1_DQ_3_PAD_SEL, QSPI1_DQ_3_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_3_IOF_OVAL, QSPI1_DQ_3_PAD_SEL, QSPI1_DQ_3_HS_SEL, 0, 0);

    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_SCK_IOF_IVAL, QSPI2_SCK_PAD_SEL, QSPI2_SCK_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_CS_0_IOF_IVAL, QSPI2_CS_0_PAD_SEL, QSPI2_CS_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_0_IOF_IVAL, QSPI2_DQ_0_PAD_SEL, QSPI2_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_0_IOF_OVAL, QSPI2_DQ_0_PAD_SEL, QSPI2_DQ_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_1_IOF_IVAL, QSPI2_DQ_1_PAD_SEL, QSPI2_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_1_IOF_OVAL, QSPI2_DQ_1_PAD_SEL, QSPI2_DQ_1_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_2_IOF_IVAL, QSPI2_DQ_2_PAD_SEL, QSPI2_DQ_2_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_2_IOF_OVAL, QSPI2_DQ_2_PAD_SEL, QSPI2_DQ_2_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_3_IOF_IVAL, QSPI2_DQ_3_PAD_SEL, QSPI2_DQ_3_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_3_IOF_OVAL, QSPI2_DQ_3_PAD_SEL, QSPI2_DQ_3_HS_SEL, 0, 0);
}

/**
  * \brief configure the SPI peripheral
  */
void SPI_Config(void)
{
    QSPI_InitTypeDef spi_init_struct;
    QSPI_StructInit(&spi_init_struct);
    spi_init_struct.DevMode = (QSPI_CR_MODE_MASTER | QSPI_CR_DMA_ENABLE);
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    
    spi_init_struct.DataSize = QSPI_FMT_LEN_32B;
    
    QSPI_Init(QSPI1, &spi_init_struct);
    spi_init_struct.DevMode = (QSPI_CR_MODE_SLAVE | QSPI_CR_DMA_ENABLE);
    QSPI_Init(QSPI2, &spi_init_struct);
}

void main(void)
{
    int state0 = 0;
    int state1 = 0;

    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif

    dst_base_addr1 = (uint32_t *)(0x9000000 + 32768 - 0x400);
    src_base_addr1 = (uint32_t *)(0x9000000 + 32768 - 0x300);
    dst_base_addr2 = (uint32_t *)(0x9000000 + 32768 - 0x200);
    src_base_addr2 = (uint32_t *)(0x9000000 + 32768 - 0x100);

    uint32_t * src_base_addr1_tmp = src_base_addr1;
    uint32_t * src_base_addr2_tmp = src_base_addr2;

    for (int i = 0; i < ARRAYSIZE; i++) {
        *src_base_addr1_tmp++ = 0x12345678 + i;
        *src_base_addr2_tmp++ = 0x87654321 - i;
    }

    #ifdef MISC_HAS_QSPI1_HAS_CLK
    qspi1_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_QSPI1_RST
    qspi1_set_rst(DISABLE);
    qspi1_set_rst(ENABLE);
    #endif

    #ifdef MISC_HAS_UDMA0_HAS_CLK
    udma0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_UDMA0_RST
    udma0_set_rst(DISABLE);
    udma0_set_rst(ENABLE);
    #endif
    
    #ifdef MISC_HAS_QSPI2_HAS_CLK
    qspi2_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_QSPI2_RST
    qspi2_set_rst(DISABLE);
    qspi2_set_rst(ENABLE);
    #endif

    #ifdef MISC_HAS_UDMA0_HAS_CLK
    udma0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_UDMA0_RST
    udma0_set_rst(DISABLE);
    udma0_set_rst(ENABLE);
    #endif

    iomux_config();

    UDMA_PA2M_ITConfig(QSPI1_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);
    UDMA_PA2M_ITConfig(QSPI2_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);

    SPI_Config();

    UDMA_PAM2MTypeDef UDMA_PAM2MStruct = {0};
    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(dst_base_addr1);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(&(QSPI1->RXDATA));
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(dst_base_addr1);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(&(QSPI1->RXDATA));
#endif
    UDMA_PAM2MStruct.UDMA_BufferSize = ARRAYSIZE*4;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_32BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_QSPI1_RX_DMA;  
    UDMA_PAM2M_Init(QSPI1_RX_DMA_DMA_CH, &UDMA_PAM2MStruct);

    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(dst_base_addr2);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(&(QSPI2->RXDATA));
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(dst_base_addr2);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(&(QSPI2->RXDATA));
#endif
    UDMA_PAM2MStruct.UDMA_BufferSize = ARRAYSIZE*4;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_32BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_QSPI2_RX_DMA;
    UDMA_PAM2M_Init(QSPI2_RX_DMA_DMA_CH, &UDMA_PAM2MStruct);

    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(&(QSPI2->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(src_base_addr2);
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(&(QSPI2->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(src_base_addr2);
#endif
    UDMA_PAM2MStruct.UDMA_BufferSize = ARRAYSIZE*4;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_32BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_QSPI2_TX_DMA;
    UDMA_PAM2M_Init(QSPI2_TX_DMA_DMA_CH, &UDMA_PAM2MStruct);

    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(&(QSPI1->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(src_base_addr1);
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(&(QSPI1->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(src_base_addr1);
#endif
    UDMA_PAM2MStruct.UDMA_BufferSize = ARRAYSIZE*4;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_32BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_QSPI1_TX_DMA;  
    UDMA_PAM2M_Init(QSPI1_TX_DMA_DMA_CH, &UDMA_PAM2MStruct);

    QSPI_TxDmaStart(QSPI2, ENABLE);
    QSPI_RxDmaStart(QSPI2, ENABLE);
    QSPI_TxDmaStart(QSPI1, ENABLE);
    QSPI_RxDmaStart(QSPI1, ENABLE);

    while (!UDMA_PA2M_GetITStatus(QSPI1_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_STAT)) {}
    while (!UDMA_PA2M_GetITStatus(QSPI2_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_STAT)) {}

    state0 = MemoryCompare((uint8_t *)dst_base_addr2, (uint8_t *)src_base_addr1, ARRAYSIZE*4);
    state1 = MemoryCompare((uint8_t *)src_base_addr2, (uint8_t *)dst_base_addr1, ARRAYSIZE*4);

    if (state0 & state1) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

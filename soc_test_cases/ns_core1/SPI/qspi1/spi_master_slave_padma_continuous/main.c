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

volatile uint32_t send_n = 0, receive_n = 0;

volatile uint8_t spi_tx = 0;
volatile uint8_t spi_rx = 0;

volatile uint8_t dma_tx = 0;
volatile uint8_t dma_rx = 0;

UDMA_PAM2MTypeDef UDMA_PAM2MRxStruct = {0};
UDMA_PAM2MTypeDef UDMA_PAM2MTxStruct = {0};
uint32_t *dst_base_addr, *src_base_addr;
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
    
    spi_init_struct.TxDmaContinousMode = QSPI_CR_DMA_TCONTINOUS_MODE_ENABLE;
    spi_init_struct.DataSize = QSPI_FMT_LEN_32B;
    QSPI_Init(QSPI1, &spi_init_struct);
    
    QSPI1->TSIZE = QSPI_TSIZE_NUM(ARRAYSIZE);
    spi_init_struct.RxDmaContinousMode = QSPI_CR_DMA_RCONTINOUS_MODE_ENABLE;
    spi_init_struct.DevMode = (QSPI_CR_MODE_SLAVE | QSPI_CR_DMA_ENABLE);
    QSPI_Init(QSPI2, &spi_init_struct);
    QSPI2->RSIZE = ARRAYSIZE;
}

/**
 * \brief UDMA IRQ Handler
 */
void UDMA0_IRQHandler(void)
{
    if (UDMA_PA2M_GetITStatus(QSPI2_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_STAT)) {
        UDMA_PA2M_ClearITStatus(QSPI2_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_CLEAR_STAT);
        dma_rx ++;
        if(dma_rx == 3){
            UDMA_PAM2MRxStruct.UDMA_TransEn = PA2M_TRANS_DISABLE;
            UDMA_PAM2MRxStruct.UDMA_PER_SEL = UDMA_SEL_QSPI2_RX_DMA;
            UDMA_PAM2M_Init(QSPI2_RX_DMA_DMA_CH, &UDMA_PAM2MRxStruct);
        }
    }
}

void QSPI2_IRQHandler(void)
{
    if ((SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_DONE)) & 
        (SET == QSPI_GetITConfig(QSPI2, QSPI_IE_RX_DONE))) {
        QSPI_ClearFlag(QSPI2,QSPI_STATUS_RX_DONE);
        spi_rx ++;
        if(spi_rx == 3) {
            QSPI_ITConfig(QSPI2, QSPI_IE_RX_DONE, DISABLE);
        }
    }
}

void main(void)
{
    int state0 = 0;

    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif

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

    dst_base_addr = (uint32_t *)(0x9000000 + 32768 - 0x200);
    src_base_addr = (uint32_t *)(0x9000000 + 32768 - 0x100);

    uint32_t *src_base_addr_tmp = src_base_addr;

    for (int i = 0; i < ARRAYSIZE; i++) {
        *src_base_addr_tmp++ = 0x12345678 + i;
    }

    iomux_config();

    SPI_Config();

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt UDMA_IRQn */
    ECLIC_Register_IRQ(UDMA0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    UDMA0_IRQHandler);

    /*register interrupt QSPI2_IRQn */
    ECLIC_Register_IRQ(QSPI2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    QSPI2_IRQHandler);

    UDMA_PA2M_ITConfig(QSPI2_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);

    QSPI_ITConfig(QSPI2, QSPI_IE_RX_DONE,ENABLE);

    UDMA_PAM2MRxStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MRxStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(dst_base_addr);
    UDMA_PAM2MRxStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(&(QSPI2->RXDATA));
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MRxStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(dst_base_addr);
    UDMA_PAM2MRxStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(&(QSPI2->RXDATA));
#endif
    UDMA_PAM2MRxStruct.UDMA_BufferSize = ARRAYSIZE*4;
    UDMA_PAM2MRxStruct.UDMA_DstInc = PA2M_MDNA_ENABLE;
    UDMA_PAM2MRxStruct.UDMA_SrcInc = PA2M_MSNA_DISABLE;
    UDMA_PAM2MRxStruct.UDMA_Width = PA2M_MDWIDTH_32BIT;
    UDMA_PAM2MRxStruct.UDMA_Mode = PA2M_MODE_CONTINUOUS;
    UDMA_PAM2MRxStruct.UDMA_PER_SEL = UDMA_SEL_QSPI2_RX_DMA;
    UDMA_PAM2M_Init(QSPI2_RX_DMA_DMA_CH, &UDMA_PAM2MRxStruct);

    UDMA_PAM2MTxStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MTxStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(&(QSPI1->TXDATA));
    UDMA_PAM2MTxStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(src_base_addr);
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MTxStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(&(QSPI1->TXDATA));
    UDMA_PAM2MTxStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(src_base_addr);
#endif
    UDMA_PAM2MTxStruct.UDMA_BufferSize = ARRAYSIZE*4;
    UDMA_PAM2MTxStruct.UDMA_DstInc = PA2M_MDNA_DISABLE;
    UDMA_PAM2MTxStruct.UDMA_SrcInc = PA2M_MSNA_ENABLE;
    UDMA_PAM2MTxStruct.UDMA_Width = PA2M_MDWIDTH_32BIT;
    UDMA_PAM2MTxStruct.UDMA_Mode = PA2M_MODE_CONTINUOUS;
    UDMA_PAM2MTxStruct.UDMA_PER_SEL = UDMA_SEL_QSPI1_TX_DMA;
    UDMA_PAM2M_Init(QSPI1_TX_DMA_DMA_CH, &UDMA_PAM2MTxStruct);

    QSPI_TxDmaStart(QSPI1, ENABLE);
    QSPI_RxDmaStart(QSPI2, ENABLE);

    while (spi_rx != 3 && dma_rx != 3) {}

    /* disable dma en */
    QSPI_TxDmaStart(QSPI1, DISABLE);
    QSPI_RxDmaStart(QSPI2, DISABLE);
    /* disable QSPI1_TX_DMA_DMA_CH QSPI2_RX_DMA_DMA_CH */ /* for using multi udma */
    UDMA_PAM2MTxStruct.UDMA_TransEn = PA2M_TRANS_DISABLE;
    UDMA_PAM2MTxStruct.UDMA_PER_SEL = UDMA_SEL_QSPI1_TX_DMA;
    UDMA_PAM2M_Init(QSPI1_TX_DMA_DMA_CH, &UDMA_PAM2MTxStruct);
    UDMA_PAM2MRxStruct.UDMA_TransEn = PA2M_TRANS_DISABLE;
    UDMA_PAM2MRxStruct.UDMA_PER_SEL = UDMA_SEL_QSPI2_RX_DMA;
    UDMA_PAM2M_Init(QSPI2_RX_DMA_DMA_CH, &UDMA_PAM2MRxStruct);

    state0 = MemoryCompare((uint8_t *)src_base_addr, (uint8_t *)dst_base_addr, ARRAYSIZE*4);

    if (state0) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

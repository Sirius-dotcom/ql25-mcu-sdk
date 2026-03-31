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
volatile int  send_flag = 0;
volatile  int receive_flag = 0;

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
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_SCK_IOF_OVAL, QSPI3_SCK_PAD_SEL, QSPI3_SCK_HS_SEL, 0, 0);
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_CS_0_IOF_OVAL, QSPI3_CS_0_PAD_SEL, QSPI3_CS_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_0_IOF_IVAL, QSPI3_DQ_0_PAD_SEL, QSPI3_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_0_IOF_OVAL, QSPI3_DQ_0_PAD_SEL, QSPI3_DQ_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_1_IOF_IVAL, QSPI3_DQ_1_PAD_SEL, QSPI3_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_1_IOF_OVAL, QSPI3_DQ_1_PAD_SEL, QSPI3_DQ_1_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_2_IOF_IVAL, QSPI3_DQ_2_PAD_SEL, QSPI3_DQ_2_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_2_IOF_OVAL, QSPI3_DQ_2_PAD_SEL, QSPI3_DQ_2_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_3_IOF_IVAL, QSPI3_DQ_3_PAD_SEL, QSPI3_DQ_3_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_3_IOF_OVAL, QSPI3_DQ_3_PAD_SEL, QSPI3_DQ_3_HS_SEL, 0, 0);

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
    QSPI_ITConfig(QSPI3,QSPI_IE_DONE,ENABLE);
    QSPI_InitTypeDef spi_init_struct;
    QSPI_StructInit(&spi_init_struct);
    spi_init_struct.DevMode = (QSPI_CR_MODE_MASTER | QSPI_CR_DMA_ENABLE);
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    
    QSPI_Init(QSPI3, &spi_init_struct);
    
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
        receive_flag=1;
    } 
}

/**
 * \brief this function handles QSPI3 Handler
 */
void QSPI3_IRQHandler(void)
{
    if (RESET != QSPI_GetFlag(QSPI3, QSPI_STATUS_DONE)) {
        QSPI_ClearFlag(QSPI3,QSPI_STATUS_DONE);
        send_flag=1;
    } 
}

void main(void)
{
    int state0 = 0;

    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif
    #ifdef MISC_HAS_QSPI3_HAS_CLK
    qspi3_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_QSPI3_RST
    qspi3_set_rst(DISABLE);
    qspi3_set_rst(ENABLE);
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

    dst_base_addr = (uint32_t *)(0x8000000 + 32768 - 0x200);
    src_base_addr = (uint32_t *)(0x8000000 + 32768 - 0x100);

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
    /*register interrupt QSPI3_IRQn */
    ECLIC_Register_IRQ(QSPI3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    QSPI3_IRQHandler);

    UDMA_PA2M_ITConfig(QSPI2_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);
    QSPI_ITConfig(QSPI3, QSPI_IE_TX_DONE,ENABLE);

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
    UDMA_PAM2MRxStruct.UDMA_Width = PA2M_MDWIDTH_8BIT;
    UDMA_PAM2MRxStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MRxStruct.UDMA_PER_SEL = UDMA_SEL_QSPI2_RX_DMA;
    UDMA_PAM2M_Init(QSPI2_RX_DMA_DMA_CH, &UDMA_PAM2MRxStruct);

    UDMA_PAM2MTxStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MTxStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(&(QSPI3->TXDATA));
    UDMA_PAM2MTxStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(src_base_addr);
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MTxStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(&(QSPI3->TXDATA));
    UDMA_PAM2MTxStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(src_base_addr);
#endif
    UDMA_PAM2MTxStruct.UDMA_BufferSize = ARRAYSIZE*4;
    UDMA_PAM2MTxStruct.UDMA_DstInc = PA2M_MDNA_DISABLE;
    UDMA_PAM2MTxStruct.UDMA_SrcInc = PA2M_MSNA_ENABLE;
    UDMA_PAM2MTxStruct.UDMA_Width = PA2M_MDWIDTH_8BIT;
    UDMA_PAM2MTxStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MTxStruct.UDMA_PER_SEL = UDMA_SEL_QSPI3_TX_DMA;  
    UDMA_PAM2M_Init(QSPI3_TX_DMA_DMA_CH, &UDMA_PAM2MTxStruct);

    QSPI_RxDmaStart(QSPI2, ENABLE);
    QSPI_TxDmaStart(QSPI3, ENABLE);

    while (send_flag == RESET) {}
    while (receive_flag == RESET) {}

    state0 = MemoryCompare((uint8_t *)src_base_addr, (uint8_t *)dst_base_addr, ARRAYSIZE*4);

    if (state0) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

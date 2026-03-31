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

#define SOC_CLK                (SystemCoreClock)

#define I2C1_SPEED             (100000)
#define I3_SLAVE_ADDRESS1 (0x62)
#define I3_SLAVE_ADDRESS2 (0x72)
#define ARRAYNUM(arr_nanme)    (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define BUFFER_SIZE            (2)
uint32_t *dst_base_addr = NULL;
uint32_t *src_base_addr = NULL;
volatile ErrStatus state = ERROR;
volatile uint8_t interrupt_time_tx = 0;
volatile uint8_t interrupt_time_rx = 0;
UDMA_PAM2MTypeDef UDMA_PAM2MStruct = {0};

/**
  * \brief memory compare function
  *
  * \param src: source data pointer
  * \param dst: destination data pointer
  * \param length: the compare data length
  *
  * \retval ErrStatus: ERROR or SUCCESS
  */
ErrStatus MemoryCompare(uint32_t* src, uint32_t* dst, uint16_t length)
{
    while (length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,I2C1_SCL_IOF_IVAL, I2C1_SCL_PAD_SEL, I2C1_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I2C1_SCL_IOF_OVAL, I2C1_SCL_PAD_SEL,  I2C1_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,I2C1_SDA_IOF_IVAL, I2C1_SDA_PAD_SEL, I2C1_SDA_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I2C1_SDA_IOF_OVAL, I2C1_SDA_PAD_SEL,  I2C1_SDA_HS_SEL, 0, 0);

    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I2C1_SCL_PAD_SEL, I2C1_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I2C1_SDA_PAD_SEL, I2C1_SDA_HS_SEL, 0, 0);

    iomux_ls_iof_ival_cfg(IOMUX_BASE,I3_SCL_IOF_IVAL, I3_SCL_PAD_SEL,I3_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I3_SCL_IOF_OVAL, I3_SCL_PAD_SEL,I3_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,I3_SDA_IOF_IVAL, I3_SDA_PAD_SEL,I3_SDA_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I3_SDA_IOF_OVAL, I3_SDA_PAD_SEL,I3_SDA_HS_SEL, 0, 0);

    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I3_SCL_PAD_SEL, I3_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I3_SDA_PAD_SEL, I3_SDA_HS_SEL, 0, 0);
}

/**
  * \brief cofigure the I2C1 and I3 interfaces
  */
void I2C_Config(void)
{
    I2C_InitTypeDef init = {0};
    init.speed = I2C_SPEED_100K;
    init.time_cfg = DISABLE;
    init.auto_end = DISABLE;
    init.mode = I2C_MASTER_MODE;
    init.scl_pull = ENABLE;
    init.sda_pull = ENABLE;
    
    I2C_Init(I2C1, &init);

    init.mode = I2C_SLAVE_MODE;
    
    init.slave_info1.slave_addr = I3_SLAVE_ADDRESS1;
    init.slave_info1.slave_addr_mask = 0x3FF;
    init.slave_info1.slave_addr_en = ENABLE;
    I2C_Init(I3, &init);
}

/**
 * \brief UDMA IRQ Handler
 */
void UDMA0_IRQHandler(void)
{
    if (UDMA_PA2M_GetITStatus(I2C1_I2C_DMA_TX_DMA_IRQ, PA2M_FTRANS_IRQ_STAT)) {
        if(interrupt_time_tx > 3){
            UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_DISABLE;
            UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_I2C1_I2C_DMA_TX;
            UDMA_PAM2M_Init(I2C1_I2C_DMA_TX_DMA_CH, &UDMA_PAM2MStruct);
        }
        UDMA_PA2M_ClearITStatus(I2C1_I2C_DMA_TX_DMA_IRQ, PA2M_FTRANS_IRQ_CLEAR_STAT);
        interrupt_time_tx ++;
    }
    if (UDMA_PA2M_GetITStatus(I3_I2C_DMA_RX_DMA_IRQ, PA2M_FTRANS_IRQ_STAT)) {
        if(interrupt_time_rx > 3){
            UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_DISABLE;
            UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_I3_I2C_DMA_RX;
            UDMA_PAM2M_Init(I3_I2C_DMA_RX_DMA_CH, &UDMA_PAM2MStruct);
        }
        UDMA_PA2M_ClearITStatus(I3_I2C_DMA_RX_DMA_IRQ, PA2M_FTRANS_IRQ_CLEAR_STAT);
        interrupt_time_rx ++;
    }
}

void main(void)
{
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif

    #ifdef MISC_HAS_I2C1_HAS_CLK
    i2c1_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_I2C1_RST
    i2c1_set_rst(DISABLE);
    i2c1_set_rst(ENABLE);
    #endif
    
    #ifdef MISC_HAS_UDMA0_HAS_CLK
    udma0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_UDMA0_RST
    udma0_set_rst(DISABLE);
    udma0_set_rst(ENABLE);
    #endif

    #ifdef MISC_HAS_UDMA_HAS_CLK
    udma_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_UDMA_RST
    udma_set_rst(DISABLE);
    udma_set_rst(ENABLE);
    #endif

    dst_base_addr = (uint32_t *)(0x8000000 + 32768 - 0x200);
    src_base_addr = (uint32_t *)(0x8000000 + 32768 - 0x100);
    uint32_t *ptmp_data = src_base_addr;
    for(int i = 0; i < BUFFER_SIZE; i++) {
        *ptmp_data++ = ((uint32_t)(i * 4 + 0) << 24 | (uint32_t)(i * 4 + 1) << 16 | (uint32_t)(i * 4 + 2) << 8 | (uint32_t)(i * 4 + 3));
    }

    iomux_config();

    I2C_Config();

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt UDMA_IRQn */
    ECLIC_Register_IRQ(UDMA0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    UDMA0_IRQHandler);

    UDMA_PA2M_ITConfig(I2C1_I2C_DMA_TX_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);
    UDMA_PA2M_ITConfig(I3_I2C_DMA_RX_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);

    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(I2C1, I3_SLAVE_ADDRESS1, I2C_DIR_WRITE, 0xFFFFFFFF);
    while (!I2C_Get_Status(I3, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I3, I2C_STATUS_ADDR_MATCH);
    while (!I2C_Get_Status(I2C1, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I2C1, I2C_STATUS_ADDR_MATCH);

    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(&(I2C1->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(src_base_addr);
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(&(I2C1->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(src_base_addr);
#endif
    UDMA_PAM2MStruct.UDMA_BufferSize = BUFFER_SIZE*4;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_8BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_CONTINUOUS;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_I2C1_I2C_DMA_TX;  
    UDMA_PAM2M_Init(I2C1_I2C_DMA_TX_DMA_CH, &UDMA_PAM2MStruct);

    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(dst_base_addr);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(&(I3->RXDATA));
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(dst_base_addr);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(&(I3->RXDATA));
#endif
    UDMA_PAM2MStruct.UDMA_BufferSize = BUFFER_SIZE*4;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_8BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_CONTINUOUS;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_I3_I2C_DMA_RX;
    UDMA_PAM2M_Init(I3_I2C_DMA_RX_DMA_CH, &UDMA_PAM2MStruct);

    /* config i2c dma mode and size */
    I2C_Set_TxSize(I2C1, BUFFER_SIZE*4);
    I2C_Set_RxSize(I3, BUFFER_SIZE*4);

    I2C_TxDma_ContinuousEn(I2C1, ENABLE);
    I2C_RxDma_ContinuousEn(I3, ENABLE);

    I2C_RxDma_Start(I3, ENABLE);
    I2C_TxDma_Start(I2C1, ENABLE);

    I2C_Dma_Cfg(I3, ENABLE);
    I2C_Dma_Cfg(I2C1, ENABLE);

    while (interrupt_time_tx <= 3 || interrupt_time_rx <= 3) {}
    I2C_Stop(I2C1);
    I2C_WaitStopOver(I2C1);

    I2C_Dma_Cfg(I3, DISABLE);
    I2C_Dma_Cfg(I2C1, DISABLE);
    I2C_RxDma_Start(I3, DISABLE);
    I2C_TxDma_Start(I2C1, DISABLE);
    I2C_TxDma_ContinuousEn(I2C1, DISABLE);
    I2C_RxDma_ContinuousEn(I3, DISABLE);

    /* clear channel */
    I2C_TxChannel_Clear(I2C1);
    I2C_RxChannel_Clear(I3);

    /* wait 1ms for completing trans*/
    
    state = MemoryCompare((uint32_t *)src_base_addr, (uint32_t *)dst_base_addr, BUFFER_SIZE);
    if (SUCCESS == state) {
        simulation_pass();
    } else {
        simulation_fail();
    }
    while (1) {}
}

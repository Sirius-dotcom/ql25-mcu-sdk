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
#include "eeprom.h"

#if CFG_SIMULATION
#define SOC_CLK                (100000000)
#else
#define SOC_CLK                (SystemCoreClock)
#endif
#define EEP_FIRST_PAGE         (0x00)
#define I2C_OK                 (0)
#define I2C_FAIL               (1)
#define EEPROM_BLOCK0_ADDRESS  (0x50)
#define BUFFER_SIZE            (5)
#define I2C0_SPEED             (100000)
#define I2C0_SLAVE_ADDRESS7    (0x50)
#define I2C_PAGE_SIZE          (16)
uint16_t transmit_address = 0;
uint8_t *dst_base_addr = NULL;
uint8_t *src_base_addr = NULL;
extern volatile uint8_t tx_dma_done_flag;
extern volatile uint8_t rx_dma_done_flag;
volatile ErrStatus state = ERROR;

/**
  * \brief memory compare function
  *
  * \param src: source data pointer
  * \param dst: destination data pointer
  * \param length: the compare data length
  *
  * \retval ErrStatus: ERROR or SUCCESS
  */
static ErrStatus MemoryCompare(uint8_t* src, uint8_t* dst, uint16_t length)
{
    while (length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

/**
  * \brief cofigure the I2C0 interfaces
  */
void I2C_Config(void)
{
    I2C_InitTypeDef init = {0};
    init.speed = I2C_SPEED_100K;
    init.time_cfg = DISABLE;
    init.mode = I2C_MASTER_MODE;
    init.scl_pull = ENABLE;
    init.sda_pull = ENABLE;
    I2C_Init(I2C0, &init);
}

/**
 * \brief UDMA IRQ Handler
 */
void UDMA0_IRQHandler(void)
{
    if (UDMA_PA2M_GetITStatus(I2C0_I2C_DMA_TX_DMA_IRQ, PA2M_FTRANS_IRQ_STAT)) {
        UDMA_PA2M_ClearITStatus(I2C0_I2C_DMA_TX_DMA_IRQ, PA2M_FTRANS_IRQ_CLEAR_STAT);
        tx_dma_done_flag = 1;
    }
    if (UDMA_PA2M_GetITStatus(I2C0_I2C_DMA_RX_DMA_IRQ, PA2M_FTRANS_IRQ_STAT)) {
        UDMA_PA2M_ClearITStatus(I2C0_I2C_DMA_RX_DMA_IRQ, PA2M_FTRANS_IRQ_CLEAR_STAT);
        rx_dma_done_flag = 1;
    }
}

void UDMA_Config(void)
{
      /* enable trasmit over int */
    UDMA_PA2M_ITConfig(I2C0_I2C_DMA_TX_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);
    UDMA_PA2M_ITConfig(I2C0_I2C_DMA_RX_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);

    UDMA_PAM2MTypeDef UDMA_PAM2MStruct = {0};
    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(&(I2C0->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(src_base_addr);
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(&(I2C0->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(src_base_addr);
#endif
    UDMA_PAM2MStruct.UDMA_BufferSize = BUFFER_SIZE;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_8BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_I2C0_I2C_DMA_TX;
    UDMA_PAM2M_Init(I2C0_I2C_DMA_TX_DMA_CH, &UDMA_PAM2MStruct);

    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(dst_base_addr);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(&(I2C0->RXDATA));
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(dst_base_addr);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(&(I2C0->RXDATA));
#endif
    UDMA_PAM2MStruct.UDMA_BufferSize = BUFFER_SIZE;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_8BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_I2C0_I2C_DMA_RX;
    UDMA_PAM2M_Init(I2C0_I2C_DMA_RX_DMA_CH, &UDMA_PAM2MStruct);
}

ErrStatus i2c_eeprom_test()
{
    Eeprom_ByteWriteDma(I2C0, transmit_address, BUFFER_SIZE);

    Eeprom_WaitStandbyState(I2C0);

    Eeprom_ByteReadDma(I2C0, transmit_address, BUFFER_SIZE);

    state = MemoryCompare((uint8_t *)dst_base_addr, (uint8_t *)src_base_addr, BUFFER_SIZE);

    return state;
}
/**
  * \brief main
  *
  * \return int
  */
int main(void)
{
    
    tx_dma_done_flag = 0;
    rx_dma_done_flag = 0;

    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif

    #ifdef MISC_HAS_I2C0_HAS_CLK
    i2c0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_I2C0_RST
    i2c0_set_rst(DISABLE);
    i2c0_set_rst(ENABLE);
    #endif
    #ifdef CFG_SIMULATION
    #ifdef MISC_HAS_I2C0_CLK_DIV
    
    #endif
    #endif

    #ifdef MISC_HAS_UDMA0_HAS_CLK
    udma0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_UDMA0_RST
    udma0_set_rst(DISABLE);
    udma0_set_rst(ENABLE);
    #endif

    dst_base_addr = (uint8_t *)(0x9000000 + 32768 - 0x200);
    src_base_addr = (uint8_t *)(0x9000000 + 32768 - 0x100);
    uint8_t *ptmp_data = src_base_addr;
    for(int i = 0; i < BUFFER_SIZE; i++) {
        *ptmp_data++ = (uint8_t)(2*i+1) << 4 | (uint8_t)(2*i+1);
    }

    /* configure I2C */
    I2C_Config();

    /* Global interrupt enable*/
    __enable_irq();

    /*register interrupt UDMA_IRQn */
    ECLIC_Register_IRQ(UDMA0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    UDMA0_IRQHandler);

    UDMA_Config();

    if (SUCCESS == i2c_eeprom_test()) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

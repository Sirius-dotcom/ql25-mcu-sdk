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
#include <stdlib.h>
#include <string.h>

#include "ns_sdk_hal.h"

#define SRC_DATA_ACC_ADDR_BASE           (0x8000000 + 32768 - 0x200)
#define DES_DATA_ACC_ADDR_BASE           (0x8000000 + 32768 - 0x100)

#define ARR_SIZE            (2)
uint32_t buffer[ARR_SIZE] = {400, 800};
uint8_t state = 1; 
/**
  * \brief This function handles TIMER0 interrupt request.
  */
void BASIC_TIMER1_IRQHandler(void)
{
    static uint8_t num = 0;
    if(SET == Basic_Timer_InterruptFlagGet(BASIC_TIMER1, BASIC_TIMER_DIER_UIE)) {
        /* clear channel 0 interrupt bit */
        Basic_Timer_InterruptFlagClear(BASIC_TIMER1, BASIC_TIMER_DIER_UIE);
        num++;
        if (3 <= num)
        {
            state = 0;
            Basic_Timer_Disable(BASIC_TIMER1);
        }
    }
}

/**
  * \brief main
  *
  * \return int
  */
int main(void)
{
    #ifdef MISC_HAS_BASIC_TIMER1_HAS_CLK
    basic_timer1_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_BASIC_TIMER1_RST
    basic_timer1_set_rst(DISABLE);
    basic_timer1_set_rst(ENABLE);
    #endif
    #ifdef MISC_HAS_UDMA0_HAS_CLK
    udma0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_UDMA0_RST
    udma0_set_rst(DISABLE);
    udma0_set_rst(ENABLE);
    #endif

    uint8_t *dst_base_addr = (uint8_t *)(DES_DATA_ACC_ADDR_BASE);
    uint8_t *src_base_addr = (uint8_t *)(SRC_DATA_ACC_ADDR_BASE);
    memset((uint8_t *)dst_base_addr, 0, ARR_SIZE*4 );
    memcpy((uint8_t *)src_base_addr, (uint8_t *)buffer, ARR_SIZE*4 );

    /* Global interrupt enable */
    __enable_irq();
    /* register interrupt BASIC_TIMER0_IRQn */
    ECLIC_Register_IRQ(BASIC_TIMER1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    BASIC_TIMER1_IRQHandler);
    UDMA_PAM2MTypeDef UDMA_PAM2MStruct = {0};
    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(BASIC_TIMER1 + 0x2C);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO((src_base_addr));

    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = 0x00;
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = 0x00;

    UDMA_PAM2MStruct.UDMA_BufferSize = (ARR_SIZE*4);
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_32BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_CONTINUOUS;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_BASIC_TIMER1_I;
    UDMA_PAM2M_Init(BASIC_TIMER1_I_DMA_CH, &UDMA_PAM2MStruct);

    Basic_TIMER_Init_TypeDef timer_initpara;
    /* initialize TIMER init parameter struct */
    Basic_Timer_StructParaInit(&timer_initpara);
    /* BASIC_TIMER1 configuration */
    #ifdef CFG_SIMULATION
    timer_initpara.prescaler = 0;
    #else
    timer_initpara.prescaler = 4;
    #endif
    timer_initpara.period = 200;
    Basic_Timer_Init(BASIC_TIMER1, &timer_initpara);
    Basic_Timer_DmaEnable(BASIC_TIMER1, BASIC_TIMER_DIER_UDE);
    Basic_Timer_InterruptEnable(BASIC_TIMER1, BASIC_TIMER_DIER_UIE);
    Basic_Timer_Enable(BASIC_TIMER1);

    while (state) {};
    simulation_pass();
    while (1) {}
}

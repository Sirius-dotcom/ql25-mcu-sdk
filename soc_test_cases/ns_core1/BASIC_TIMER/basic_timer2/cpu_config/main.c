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
#include "ns_sdk_hal.h"
uint8_t flag = 0;
/**
  * \brief This function handles TIMER0 interrupt request.
  */
void BASIC_TIMER2_IRQHandler(void)
{
    if( SET == Basic_Timer_InterruptFlagGet(BASIC_TIMER2, BASIC_TIMER_DIER_UIE)) {
        /* clear channel 0 interrupt bit */
        Basic_Timer_InterruptFlagClear(BASIC_TIMER2, BASIC_TIMER_DIER_UIE);
        Basic_Timer_InterruptDisable(BASIC_TIMER2,BASIC_TIMER_DIER_UIE);
        Basic_Timer_Disable(BASIC_TIMER2);
        flag = 1;
    }
}

/**
  * \brief configure the TIMER peripheral
  */
void TIMER_Config(void)
{

    Basic_TIMER_Init_TypeDef timer_initpara;

    /* initialize TIMER init parameter struct */
    Basic_Timer_StructParaInit(&timer_initpara);
    /* BASIC_TIMER2 configuration */

    #ifdef CFG_SIMULATION
    timer_initpara.prescaler = 0;
    #else
    timer_initpara.prescaler = 4;
    #endif
    timer_initpara.period = 200;
    Basic_Timer_Init(BASIC_TIMER2, &timer_initpara);

    Basic_Timer_InterruptEnable(BASIC_TIMER2, BASIC_TIMER_DIER_UIE);
    Basic_Timer_Enable(BASIC_TIMER2);
}

/**
  * \brief main
  *
  * \return int
  */
int main(void)
{
    #ifdef MISC_HAS_BASIC_TIMER2_HAS_CLK
    basic_timer2_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_BASIC_TIMER2_RST
    basic_timer2_set_rst(DISABLE);
    basic_timer2_set_rst(ENABLE);
    #endif

    /* Global interrupt enable */
    __enable_irq();
    /* register interrupt BASIC_TIMER0_IRQn */
    ECLIC_Register_IRQ(BASIC_TIMER2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    BASIC_TIMER2_IRQHandler);
    TIMER_Config();
    delay_1ms(5);
    if(flag){
        simulation_pass();
    }else {
        simulation_fail();
    }
    while (1) {}
}

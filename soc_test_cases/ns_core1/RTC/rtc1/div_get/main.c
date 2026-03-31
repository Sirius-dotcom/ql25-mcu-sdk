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

uint32_t state = 1;

/**
  * \brief this function handles RTC interrupt
  */
void RTC1_IRQHandler(void)
{
    if (SET == RTC_InterruptFlagGet(RTC1, RTC_CRL_SECF)) { 
        RTC_InterruptFlagClear(RTC1, RTC_CRL_SECF);
        
        state = 0;
    }
    if (SET == RTC_InterruptFlagGet(RTC1, RTC_CRL_ALRF)) {
        RTC_InterruptFlagClear(RTC1, RTC_CRL_ALRF);
        
    }
    if (SET == RTC_InterruptFlagGet(RTC1, RTC_CRL_OWF)) {
        RTC_InterruptFlagClear(RTC1, RTC_CRL_OWF);
        
    }

    #ifdef MISC_HAS_RTC1_RST
    rtc1_set_rst(DISABLE);
    rtc1_set_rst(ENABLE);
    #endif

}

/**
  * \brief configure the RTC peripheral
  */
void RTC_Config(void)
{
    /* set RTC prescaler: set RTC period to 1s */
    RTC_PrescalerSet(RTC1, 0xFF);
    /* enable the RTC second interrupt */
    RTC_InterruptEnable(RTC1, RTC_CRH_SECIE);
}

/**
  * \brief main
  *
  * \return int
  */
void main(void)
{    
    #ifdef MISC_HAS_IDU_HAS_CLK
    idu_clk_en(ENABLE);
    #endif  
    #ifdef MISC_HAS_RTC1_HAS_CLK
    rtc1_clk_en(ENABLE);
    #endif  
    #ifdef MISC_HAS_RTC1_RST
    rtc1_set_rst(DISABLE);
    rtc1_set_rst(ENABLE);
    #endif
    uint32_t div_val = 0;
    printf("RTC Start Config\n");
    /* Global interrupt enable */
    __enable_irq();
    /*register interrupt */
    ECLIC_Register_IRQ(RTC1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_POSTIVE_EDGE_TRIGGER, 1, 1,
                                    RTC1_IRQHandler);
    /* RTC configuration */
    RTC_Config();
    
    while (state) {
        div_val = RTC_DividerGet(RTC1);
        printf("div value = %lu\n", div_val);    
    }
    RTC_InterruptDisable(RTC1, RTC_CRH_SECIE);
    RTC_InterruptFlagClear(RTC1, RTC_CRL_SECF);
    simulation_pass();
    while (1) {}
}

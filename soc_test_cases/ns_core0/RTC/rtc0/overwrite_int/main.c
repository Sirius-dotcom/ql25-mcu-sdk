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
void RTC0_IRQHandler(void)
{
    if (SET == RTC_InterruptFlagGet(RTC0, RTC_CRL_SECF)) {
        RTC_InterruptFlagClear(RTC0, RTC_CRL_SECF);
        
    }
    if (SET == RTC_InterruptFlagGet(RTC0, RTC_CRL_ALRF)) {
        RTC_InterruptFlagClear(RTC0, RTC_CRL_ALRF);
        
    }
    if (SET == RTC_InterruptFlagGet(RTC0, RTC_CRL_OWF)) {
        RTC_InterruptFlagClear(RTC0, RTC_CRL_OWF);
        
        state = 0; 
    }
    #ifdef MISC_HAS_RTC0_RST
    rtc0_set_rst(DISABLE);
    rtc0_set_rst(ENABLE);
    #endif

}

/**
  * \brief configure the RTC peripheral
  */
void RTC_Config(void)
{
    /* set RTC prescaler: set RTC period to 1s */
    RTC_PrescalerSet(RTC0, 0xF);
    RTC_CounerSet(RTC0, 0xFFFFFFFFFFFFFFF0);
    /* enable the RTC overwrite interrupt */
    RTC_InterruptEnable(RTC0, RTC_CRH_OWIE);
}

/**
  * \brief main
  *
  * \return int
  */
int main(void)
{
    #ifdef MISC_HAS_IDU_HAS_CLK
    idu_clk_en(ENABLE);
    #endif  
    #ifdef MISC_HAS_RTC0_HAS_CLK
    rtc0_clk_en(ENABLE);
    #endif  
    #ifdef MISC_HAS_RTC0_RST
    rtc0_set_rst(DISABLE);
    rtc0_set_rst(ENABLE);
    #endif
    printf("RTC Start Config\n");
    /* Global interrupt enable*/
    __enable_irq();
    /* register interrupt */
    ECLIC_Register_IRQ(RTC0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_POSTIVE_EDGE_TRIGGER, 1, 1,
                                    RTC0_IRQHandler);
    /* RTC configuration */
    RTC_Config();

    while (state) {}
    simulation_pass();
}

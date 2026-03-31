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

#define HART_NUM 2
#define RTC_AS_EVENT ((RTC2_IRQn - 20) % 32)
#define IRQ_AS_EVENT_OFS 0x180
#define EVENT_SEL_OFS 0x100
#define EVENT_ENA_ADDR (IDU_BASE + IRQ_AS_EVENT_OFS + ((RTC2_IRQn - 20) / 32) * 4)

/**
  * \brief this function handles RTC interrupt
  */
void RTC2_IRQHandler(void)
{
      printf("Why I am here? I am an event test, how come I became a IRQ here?\n");
      /* Should not come to here */
      simulation_fail();
}

/**
  * \brief configure the RTC peripheral
  */
void RTC_Config(void)
{
    RTC_CounerSet(RTC2, 0);
    RTC_AlarmSet(RTC2, 0x3);
    /* set RTC prescaler: set RTC period to 1s */
    RTC_PrescalerSet(RTC2, 0xF);

    /* enable the RTC alarm interrupt */
    RTC_InterruptEnable(RTC2, RTC_CRH_ALRIE);
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
    #ifdef MISC_HAS_RTC2_HAS_CLK
    rtc2_clk_en(ENABLE);
    #endif  
    #ifdef MISC_HAS_RTC2_RST
    rtc2_set_rst(DISABLE);
    rtc2_set_rst(ENABLE);
    #endif

    RTC_InterruptFlagClear(RTC2, RTC_CRL_ALRF);
    /* TODO:Check HardID, to choose set the corret event_sel */
    uint32_t hartid = __RV_CSR_READ(CSR_MHARTID);

    /* IDU irq as event cfg */
    REG32(EVENT_ENA_ADDR) |= BIT(RTC_AS_EVENT);
    
    /* Event cfg, Set event_sel bit0 ,set bit 0 to enable the irq as event */
    REG32(IDU_BASE + EVENT_SEL_OFS + hartid * 4) |= BIT(0); 
    
    /* Global interrupt enable */
    __enable_irq();
    /* register interrupt */
    ECLIC_Register_IRQ(RTC2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_POSTIVE_EDGE_TRIGGER, 1, 1,
                                    RTC2_IRQHandler);
    
    printf("Before WFE: RTC event to wfe\n");
    /* RTC configuration */
    RTC_Config();

    /* implent wfe */
     
    __WFE();
 
    /* wait for RTC irq as event,if event trigger core,pass */
    #ifdef MISC_HAS_RTC2_RST
    rtc2_set_rst(DISABLE);
    rtc2_set_rst(ENABLE);
    #endif

    simulation_pass();
    
}

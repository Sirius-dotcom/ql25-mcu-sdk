/**
  * Copyright (c) 2021 Nuclei Limited. All rights reserved.
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

uint8_t volatile count = 0;

#ifdef RTC0
void RTC0_IRQHandler(void)
{
    if(SET == IDU_IsFirstClaim(IDU_BASE, (RTC0_IRQn - 20), hart_id.b.processor_id)) {

        IDU_SetIndicator(IDU_BASE, (USART0_IRQn - 20), BIT(hart_id.b.processor_id));

        if(BIT(hart_id.b.processor_id) != IDU_GetIndicator(IDU_BASE, (USART0_IRQn - 20))) {
            simulation_fail();
            while(1);
        } else {
            for(int j = 0; j < 100; j++) {
                __NOP();
            }
            /* reset RTC0 interrupt indicator mask value */
            IDU_SetClaim(IDU_BASE, (RTC0_IRQn - 20), 0xFFFFFFFF);
            count++;
        }
    }

    if(count == 2) {
        uint32_t rtc_indicator = IDU_GetIndicator(IDU_BASE, (RTC0_IRQn - 20));
        /* interrupt will not send to current cluster */
        IDU_SetIndicator(IDU_BASE, (RTC0_IRQn - 20), (rtc_indicator & (~ (BIT(hart_id.b.processor_id)))));
        simulation_pass();
        while(1);
    }
}

/**
  * \brief configure the RTC peripheral
  */
void RTC_Config(void)
{
    /* set RTC prescaler */
    RTC_PrescalerSet(RTC0,0xF);
    RTC_AlarmSet(RTC0,0x3);
    /* enable the RTC ALARM interrupt*/
    RTC_InterruptEnable(RTC0,RTC_CRH_ALRIE);
}
#endif

void main(void)
{
    #ifdef MISC_HAS_IDU_HAS_CLK
    idu_clk_en(ENABLE);
    #endif

    #ifdef MISC_HAS_RTC0_HAS_CLK
    rtc0_clk_en(ENABLE);
    #endif

    hart_id.d = __RV_CSR_READ(CSR_MHARTID);
    #ifdef RTC0
    if(hart_id.b.core_id == 0){

        /* Global interrupt enable*/
        __enable_irq();
        /*register interrupt */
        ECLIC_Register_IRQ(RTC0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                        ECLIC_LEVEL_TRIGGER, 1, 1,
                                        RTC0_IRQHandler);
        /* RTC configuration */
        RTC_Config();

        while(1);
    } else {
        simulation_pass();
        while(1);
    }
    #else
    simulation_pass(); 
    while(1);
    #endif
}

#if defined(SMP_MODE) && (SMP_MODE == 1)
/* Reimplementation of smp_main for multi-harts */
void smp_main(void)
{
    main();
}
#endif

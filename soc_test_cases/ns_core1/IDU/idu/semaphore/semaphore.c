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

volatile uint8_t count = 0, count1 = 0;

void main(void)
{
    #ifdef MISC_HAS_IDU_HAS_CLK 
    idu_clk_en(ENABLE);
    #endif

    if (hart_id.b.core_id == 0){
        #ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
        soc_clk_cluster1_core0_stop_on_reset(DISABLE);
        #else 
        #ifdef SOC_CLK_CORE1_STOP_ON_RESET
        soc_clk_core1_stop_on_reset(DISABLE);
        #else
        core1_stop_on_reset(DISABLE);
        #endif
        #endif
    } else {
        simulation_pass();
        while(1);
    }

    uint32_t semaphore_num = IDU_GetSemaphoreNum(IDU_BASE);

    while(1) {
        IDU_SetSemaphoreStatus(IDU_BASE, 0, hart_id.d);
        for(int j = 0; j < 100; j++) {
            __NOP();
        }
        if(hart_id.d == IDU_GetSemaphoreStatus(IDU_BASE, 0)) {
 #ifndef __SMP_DMA_ADDR_OFFSET

            IDU_SetIndicator(IDU_BASE, (USART0_IRQn - 20), BIT(hart_id.b.processor_id));

            for(int j = 0; j < 100; j++) {
                __NOP();
            }

            if(BIT(hart_id.b.processor_id) != IDU_GetIndicator(IDU_BASE, (USART0_IRQn - 20))) {
                simulation_fail();
                while(1) {}
            } else {
                IDU_SetSemaphoreStatus(IDU_BASE, 0, 0xFFFFFFFF);
                count++;
            }
#else
            IDU_SetSemaphoreStatus(IDU_BASE, 0, 0xFFFFFFFF);
            count++;
#endif
        }

        if(count == 2) break;
    }
#ifdef USART1 
    while(1) {
        if(IDU_CheckSemaphoreStatus(IDU_BASE, (semaphore_num - 1), hart_id.d)) {
 #ifndef __SMP_DMA_ADDR_OFFSET

            IDU_SetIndicator(IDU_BASE,(USART1_IRQn - 20), BIT(hart_id.b.processor_id));

            for(int j = 0; j < 100; j++) {
                __NOP();
            }

            if(BIT(hart_id.b.processor_id) != IDU_GetIndicator(IDU_BASE,(USART1_IRQn - 20))) {
                simulation_fail(); 
                while(1) {}
            } else {
                IDU_SetSemaphoreStatus(IDU_BASE,(semaphore_num - 1),0xFFFFFFFF);
                count1 ++;
            }
#else
        IDU_SetSemaphoreStatus(IDU_BASE,(semaphore_num - 1),0xFFFFFFFF);
        count1++;
#endif
        }
        if(count1 == 2) break;
    }
#endif
    simulation_pass();
    while(1);
}

#if defined(SMP_MODE) && (SMP_MODE == 1)
/* Reimplementation of smp_main for multi-harts */
void smp_main(void)
{
    main();   
}
#endif

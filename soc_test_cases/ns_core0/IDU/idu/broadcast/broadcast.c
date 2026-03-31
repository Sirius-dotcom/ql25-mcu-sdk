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

#ifdef LGPIO0
void LGPIO0_IRQHandler(void)
{
    if(LGPIO_GetITStatus(LGPIO0,LGPIO_PinSource0,LGPIO_IT_HIGH) == LGPIO_PinSource0){
        simulation_pass();
        while(1);
    }
}

void gpio_init()
{
    LGPIO_ITConfig(LGPIO0,LGPIO_PinSource0,LGPIO_IT_HIGH,ENABLE);
    LGPIO_SetSoftIT(LGPIO0,LGPIO_PinSource0,LGPIO_IT_HIGH);
}
#endif

void main(void)
{
    #ifdef MISC_HAS_IDU_HAS_CLK 
    idu_clk_en(ENABLE);
    #endif

    #ifdef MISC_HAS_LGPIO0_HAS_CLK
    lgpio0_clk_en(ENABLE);
    #endif

    uint32_t hart_num = IDU_GetHartNum(IDU_BASE); 
#ifdef LGPIO0
    
    IDU_SetClaim(IDU_BASE,(LGPIO0_IRQn - 20),0XFFFFFFFF);
    /* Global interrupt enable*/
    __enable_irq();
	ECLIC_Register_IRQ(LGPIO0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    LGPIO0_IRQHandler);
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
#ifdef LGPIO0
        gpio_init(); 
#else
        simulation_pass();
#endif
    }
    else{
        simulation_pass();
    }
    
    while(1);
}

#if defined(SMP_MODE) && (SMP_MODE == 1)
/* Reimplementation of smp_main for multi-harts */
void smp_main(void)
{
    main();   
}
#endif

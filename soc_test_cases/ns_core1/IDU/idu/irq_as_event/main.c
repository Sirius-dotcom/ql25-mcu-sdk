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

uint32_t volatile count = 0;
uint8_t volatile core_id ;

uint32_t volatile hart_num = 0;
uint32_t volatile cluster_num = 0;

volatile uint32_t sem_val = 0;

#ifdef LGPIO0
void gpio_init()
{
    LGPIO_ITConfig(LGPIO0,LGPIO_PinSource0,LGPIO_IT_HIGH,ENABLE);
    LGPIO_SetSoftIT(LGPIO0,LGPIO_PinSource0,LGPIO_IT_HIGH);
    
}
#endif
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void main(void)
{
    #ifdef MISC_HAS_IDU_HAS_CLK 
    idu_clk_en(ENABLE);
    #endif

    volatile HartID_Info_Typedef hart_id;
    hart_id.d = __RV_CSR_READ(CSR_MHARTID);

    if(( hart_id.b.core_id == 0)&& ( hart_id.b.processor_id == 0)) {
        #ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
        soc_clk_cluster1_core0_stop_on_reset(DISABLE);
        #else 
        #ifdef SOC_CLK_CORE1_STOP_ON_RESET
        soc_clk_core1_stop_on_reset(DISABLE);
        #else
        core1_stop_on_reset(DISABLE);
        #endif
        #endif

        hart_num = IDU_GetHartNum(IDU_BASE); 
        cluster_num = IDU_GetClusterNum(IDU_BASE);
        uint32_t id_sent = hart_id.b.processor_id << 16; 
        
        uint32_t id_receive = 0;
       
        IDU_SetIrqAsEvent(IDU_BASE, (LGPIO0_IRQn-20));    
        for(uint32_t j = 0; j < hart_num; j++) {
            
            IDU_SetEventSel(IDU_BASE,j,0xffffffff);
        }
#ifdef LGPIO0
        
        if(cluster_num >= 2) {
            for (uint32_t k = 1; k < cluster_num; k++) {
                do {
                        sem_val = IDU_GetSemaphoreStatus(IDU_BASE , k );
                } while(sem_val != 1);
            }
        }
        gpio_init();
#endif
        simulation_pass();
        while(1);
    } else {
        if (hart_id.b.processor_id == 0 || hart_id.b.core_id != 0) {
            simulation_pass();
        }
#ifdef LGPIO0
        
        IDU_SetSemaphoreStatus(IDU_BASE, hart_id.b.processor_id, 1);
        __WFE();
#endif
        simulation_pass();
        while(1);
    }
}

#if defined(SMP_MODE) && (SMP_MODE == 1)
/* Reimplementation of smp_main for multi-harts */
void smp_main(void)
{
    main();
}
#endif

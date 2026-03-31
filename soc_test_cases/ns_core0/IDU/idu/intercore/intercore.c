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

#if defined(SMP_MODE) && (SMP_MODE == 1)
#if !defined(__riscv_atomic)
#error "RVA(atomic) extension is required for SMP"
#endif

#if !defined(SMP_CPU_CNT)
#error "SMP_CPU_CNT macro is not defined, please set SMP_CPU_CNT to integer value > 1"
#endif

#endif

uint32_t volatile count = 0;
uint8_t volatile core_id ;
uint32_t volatile hart_num = 0; 
uint32_t volatile cluster_num = 0;
uint32_t volatile wait_core0_ici = 0;

void INTER_CORE_IRQHandler(void)
{
    uint32_t status_value = IDU_GetProcessxICI_Status(IDU_BASE,(hart_id.b.processor_id % 32)); 
    
    uint32_t m = 1;
    wait_core0_ici = 1;
    for(uint8_t i = 0; i < 32; i++) {
        if((status_value & 1) == 0) {
            status_value >>= 1;
                m <<= 1;            
        }
        else {
            
            IDU_ProcessxClearICI(IDU_BASE, (hart_id.b.processor_id % 32), m);
                break;
        }
    }
    count += 1;
    if(cluster_num == 1) { /* smp + amp(core=1) */
        if(count == cluster_num) {
            simulation_pass();
        }
    } else if(hart_num == cluster_num) { /* amp: 1-cluster 1-core */
        if(count == hart_num) {
            simulation_pass();
        }
    } else if(hart_num > cluster_num){ /* amp: 1-cluster multi-core */
        if(count == cluster_num) {
            simulation_pass();
        }
    } else {
        if(count == cluster_num) {
            simulation_pass();
        }
    }
}

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
    hart_num = IDU_GetHartNum(IDU_BASE);
    cluster_num = IDU_GetClusterNum(IDU_BASE);
    
    hart_id.d = __RV_CSR_READ(CSR_MHARTID);
    
    if(hart_id.b.core_id ==0 ){
        /* Global interrupt enable*/
        __enable_irq();
        /*register interrupt INTER_CORE_IRQn */
        ECLIC_Register_IRQ(INTER_CORE_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                        ECLIC_LEVEL_TRIGGER, 3, 1,
                                    INTER_CORE_IRQHandler);
        #ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
        soc_clk_cluster1_core0_stop_on_reset(DISABLE);
        #else 
        #ifdef SOC_CLK_CORE1_STOP_ON_RESET
        soc_clk_core1_stop_on_reset(DISABLE);
        #else
        core1_stop_on_reset(DISABLE);
        #endif
        #endif

        uint32_t id_sent = hart_id.b.processor_id;
        
        uint32_t id_receive = 0;
        
        if(hart_id.b.processor_id != 0) {
            while(!wait_core0_ici) {}
        }

        for (int i = 0; i < (100 * id_sent); i++){
            __NOP();
        }

        for(uint32_t j = 0; j < cluster_num; j++) {
            
            uint32_t id_receive = j;
            IDU_SendICI(IDU_BASE,id_sent,j);
            
        }
    }
    else{
        simulation_pass();
        while(1);
    }

    while (1); 

}

#if defined(SMP_MODE) && (SMP_MODE == 1)
/* Reimplementation of smp_main for multi-harts */
void smp_main(void)
{
    main();
}
#endif

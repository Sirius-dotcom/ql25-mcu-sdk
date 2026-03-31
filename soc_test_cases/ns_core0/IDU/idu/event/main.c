/*
    Copyright (c) 2020, Nucleisys Technology  Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
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

uint32_t volatile hart_num = 0;
uint32_t volatile cluster_num = 0;
volatile uint32_t ready=0;
uint32_t volatile count = 0;

void INTER_CORE_IRQHandler(void)
{
    uint32_t status_value = IDU_GetProcessxICI_Status(IDU_BASE,(hart_id.b.processor_id % 32)); 
    
    uint32_t m = 1;
    for(uint8_t i = 0; i < 32; i++) {
        if((status_value & 1) == 0) {
            status_value >>= 1;
                m <<= 1;            
        } else {
            
            IDU_ProcessxClearICI(IDU_BASE, (hart_id.b.processor_id % 32), m);
            break;
        }
    }
    count += 1;
    
    if(count == cluster_num-1) {
        ready=1;
    }    
}

void main(void)
{
    volatile HartID_Info_Typedef hart_id;
    cluster_num = IDU_GetClusterNum(IDU_BASE);
    hart_num = IDU_GetHartNum(IDU_BASE);
    hart_id.d = __RV_CSR_READ(CSR_MHARTID);
    if (cluster_num > 1)
    {
        if((hart_id.b.core_id == 0) && (hart_id.b.processor_id == 0))
        {
            volatile unsigned long waitcnt = 0;
            uint32_t id_sent = 0 << 16; 
            uint32_t id_receive = 0;

            #ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
            soc_clk_cluster1_core0_stop_on_reset(DISABLE);
            #else 
            #ifdef SOC_CLK_CORE1_STOP_ON_RESET
            soc_clk_core1_stop_on_reset(DISABLE);
            #else
            core1_stop_on_reset(DISABLE);
            #endif
            #endif

            /* Global interrupt enable*/
            __enable_irq();
            /*register interrupt INTER_CORE_IRQn */
            ECLIC_Register_IRQ(INTER_CORE_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                            ECLIC_LEVEL_TRIGGER, 3, 1,
                                        INTER_CORE_IRQHandler);
            while(ready==0);
            printf("all ready\r\n");
            for(uint32_t j = 1; j < hart_num; j++) {
                IDU_SetEventSel(IDU_BASE, j ,0xffffffff) ;
            }
            __RV_CSR_WRITE(0x812,1);
            simulation_pass();
        } else {
            uint32_t id_sent = hart_id.b.processor_id;
            uint32_t id_receive = 0;
            IDU_SendICI(IDU_BASE,id_sent,id_receive);
            __WFE();
            simulation_pass();
        }
    } else {
        if (hart_num >1) {
            if((hart_id.b.core_id == 0) && (hart_id.b.processor_id == 0)) {
                for(uint32_t j = 1; j < hart_num; j++) {
                    
                    IDU_SetEventSel(IDU_BASE, j ,0xffffffff) ;
                }
                __RV_CSR_WRITE(0x812,1);
                simulation_pass();
            } else {
                    __WFE();
        
                    simulation_pass();
            }
        } else{
            simulation_pass();
        }

    }

}

#if defined(SMP_MODE) && (SMP_MODE == 1)
/* Reimplementation of smp_main for multi-harts */
void smp_main(void)
{
    main();   
}
#endif

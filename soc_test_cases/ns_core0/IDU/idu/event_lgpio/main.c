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

uint32_t volatile hart_num = 0;
uint32_t volatile cluster_num = 0;

void main(void)
{
    volatile HartID_Info_Typedef hart_id;
    hart_id.d = __RV_CSR_READ(CSR_MHARTID);
    if((hart_id.b.core_id == 0) && (hart_id.b.processor_id == 0)) {
        #ifdef LGPIO0
        LGPIO_EventRiseConfig(LGPIO0, LGPIO_PinSource0, ENABLE);
        iomux_ls_iof_ival_cfg(IOMUX_BASE,LGPIO0_GPIO0_IOF_IVAL,13,LGPIO0_GPIO0_HS_SEL,0,0);
        iomux_ls_iof_ival_cfg(IOMUX_BASE,LGPIO0_GPIO1_IOF_IVAL,14,LGPIO0_GPIO1_HS_SEL,0,0);
        #endif
        #ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
        soc_clk_cluster1_core0_stop_on_reset(DISABLE);
        #else 
        #ifdef SOC_CLK_CORE1_STOP_ON_RESET
        soc_clk_core1_stop_on_reset(DISABLE);
        #else
        core1_stop_on_reset(DISABLE);
        #endif
        #endif

        volatile uint32_t sem_val = 0;
        uint32_t hart_num = IDU_GetHartNum(IDU_BASE);
        uint32_t cluster_num = IDU_GetClusterNum(IDU_BASE);
        uint32_t id_sent = 0 << 16; 
    
        uint32_t id_receive = 0;

        for(uint32_t j = 1; j < hart_num; j++) {
            IDU_SetEventSel(IDU_BASE, j, 0xffffffff);
        }

        if(cluster_num >= 2) {
            for (uint32_t k = 1; k < cluster_num; k++) {
                do {
                    sem_val = IDU_GetSemaphoreStatus(IDU_BASE, k);
                } while(sem_val != 1);
            }
        }

        #ifdef RTC0
        REG32(RTC0_BASE+0x4)|=BIT(4);
        REG32(RTC0_BASE+0x18)=0x1;
        #endif
        simulation_pass();
        while (1);
    } else {
        if (hart_id.b.processor_id == 0 || hart_id.b.core_id != 0) {
            simulation_pass();
        }
        #ifdef LGPIO0
        #ifdef RTC0
        IDU_SetSemaphoreStatus(IDU_BASE, hart_id.b.processor_id, 1);
        __WFE();
        #endif
        #endif
        simulation_pass();
        while (1);
    }
}

#if defined(SMP_MODE) && (SMP_MODE == 1)
/* Reimplementation of smp_main for multi-harts */
void smp_main(void)
{
    main();
}
#endif
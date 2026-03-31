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

uint8_t count = 0;

uint8_t volatile  core_id ;

void main(void)
{
    #ifdef MISC_HAS_IDU_HAS_CLK 
    idu_clk_en(ENABLE);
    #endif
    hart_id.d = __RV_CSR_READ(CSR_MHARTID);
    if((hart_id.b.core_id == 0) && (hart_id.b.processor_id == 0)) {
        
        IDU_SetNMISel(IDU_BASE,core_id,0);

        uint32_t back_nmi = IDU_GetLockNMI(IDU_BASE,core_id );

        if(back_nmi == 0)
            count++;

        IDU_LockNMI(IDU_BASE);

        IDU_SetNMISel(IDU_BASE,core_id,1);
        
        back_nmi = IDU_GetLockNMI(IDU_BASE,core_id);
        if(back_nmi != 1) 
            count++;
        while (1) {
            if(count == 2)
                break;
        }
        #ifdef MISC_HAS_IDU_RST
        idu_set_rst(DISABLE);
        idu_set_rst(ENABLE);
        #endif
        simulation_pass();
        while(1) {}
    } else {
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

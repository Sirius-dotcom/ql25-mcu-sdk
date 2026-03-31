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
#ifdef MISC_HAS_BROM_CPU_RST_N_O_RST
#define POR_FLAG ~(RESET_CTRL_WWDG1_WDOGRES | RESET_CTRL_BROM_CPU_RST_N_O)
#else
#define POR_FLAG ~por_flag_i
#endif

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint32_t por_flag_i = Get_status_wwdg1_WDOGRES();
    uint32_t id = __RV_CSR_READ(CSR_MHARTID);

    /* enable WDOGCLKEN */
    #ifdef MISC_HAS_WWDG1_HAS_CLK
    wwdg1_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_WWDG1_RST
    wwdg1_set_rst(DISABLE);
    wwdg1_set_rst(ENABLE);
    #endif
    #if defined RESET_CTRL0_BROM_CPU_RST_N_O || defined RESET_CTRL_BROM_CPU_RST_N_O
        #if defined RESET_CTRL_OFS
        if(REG32(MISC_CTL_BASE + RESET_CTRL_OFS) != (Get_status_brom_cpu_rst_n_o() | por_flag_i)) {
            clear_reset_status();;
            delay_1ms(1);
        }
        #else
        if(REG32(MISC_CTL_BASE + RESET_CTRL0_OFS) != (Get_status_brom_cpu_rst_n_o() | por_flag_i)) {
        clear_reset_status();;
        delay_1ms(1);
        }
        #endif
    #else
        #if defined RESET_CTRL_OFS
        if((REG32(MISC_CTL_BASE + RESET_CTRL0_OFS) & POR_FLAG)!=0) {
            clear_reset_status();;
            delay_1ms(1);
        }
        #else
        if((REG32(MISC_CTL_BASE + RESET_CTRL0_OFS) & POR_FLAG)!=0) {
            clear_reset_status();;
            delay_1ms(1);
        }
        #endif
    #endif

    if(id == 0) {
        if(Get_status_wwdg1_WDOGRES()) {
            clear_reset_status();;
            delay_1ms(1);
            if((Get_status_wwdg1_WDOGRES() == 0)) {
                printf("this is WWDG1 reset,test pass\r\n");

                simulation_pass();

                #ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
                    soc_clk_cluster1_core0_stop_on_reset(DISABLE);
                #else 
                    #ifdef SOC_CLK_CORE1_STOP_ON_RESET
                        soc_clk_core1_stop_on_reset(DISABLE);
                    #else
                        #ifdef CORE1_STOP_ON_RESET
                        core1_stop_on_reset(DISABLE);
                        #endif
                    #endif
                #endif

                while(1);
            } else {
                simulation_fail();
                while(1);
            }
        }
    } else {
        if((Get_status_wwdg1_WDOGRES() == 0)) {
            simulation_pass();
            while(1);
        } else {
            simulation_fail();
            while(1);
        }
    }

    /*
     *  set WWDG clock = (CLK1/4096)/1 = 13184Hz (~4854.4 us)
     *  set counter value to 0x3fffff
     *  set window value to 80
     *  refresh window is: WWDG clock * (0x3fffff-80)= 5.655676703111111h < refresh window < WWDG clock * (0x3fffff-63) = 5.655699626666667h
     */
    #ifdef CFG_SIMULATION
    wwdg_config(WWDG1,0x3fffff, 80, WWDG_CFR_WDGTB_DIV_1);
    delay_1ms(1);
    wwdg_counter_update(WWDG1,0x3fffff);
    #else
    wwdg_config(WWDG1,0x3fffff, 80, WWDG_CFR_WDGTB_DIV_1);
    #endif

    while (1);
}
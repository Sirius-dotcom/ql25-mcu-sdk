/*
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

#include "ns_wwdg.h"

/*!
    \brief      configure the window watchdog timer counter value
    \param      counter_value 0x00 - 0x7F
    \param      wwdg
    \retval     none
*/
void wwdg_counter_update(WWDG_TypeDef * wwdg,uint32_t counter_value)
{
    uint32_t reg = 0U;
    
    reg = (wwdg->CR & (~WWDG_CR_T_MASK));
    reg |= WWDG_CR_T(counter_value);
    
    wwdg->CR = reg;
}

/*!
    \brief      configure counter value, window value, and prescaler divider value  
    \param      counter 0x00 - 0x3FFFFF   
    \param      window 0x00 - 0x3FFFFF  
    \param      prescaler wwdg prescaler value
                only one parameter can be selected which is shown as below:
      \arg        wwdg->CFR_PSC_DIV1: the time base of window watchdog counter = (PCLK1/4096)/1
      \arg        wwdg->CFR_PSC_DIV2: the time base of window watchdog counter = (PCLK1/4096)/2
      \arg        wwdg->CFR_PSC_DIV4: the time base of window watchdog counter = (PCLK1/4096)/4
      \arg        wwdg->CFR_PSC_DIV8: the time base of window watchdog counter = (PCLK1/4096)/8
      \arg        wwdg->CFR_PSC_DIV16: the time base of window watchdog counter = (PCLK1/4096)/16
      \arg        wwdg->CFR_PSC_DIV32: the time base of window watchdog counter = (PCLK1/4096)/32
      \arg        wwdg->CFR_PSC_DIV64: the time base of window watchdog counter = (PCLK1/4096)/64
      \arg        wwdg->CFR_PSC_DIV128: the time base of window watchdog counter = (PCLK1/4096)/128
    \param      wwdg
    \retval     none
*/
void wwdg_config(WWDG_TypeDef * wwdg,uint32_t counter, uint32_t window, uint32_t prescaler)
{
    uint32_t reg_cfg = 0U, reg_ctl = 0U;

    /* clear WIN and PSC bits, clear CNT bit */
    reg_cfg = (wwdg->CFR &(~(WWDG_CFR_W_MASK|WWDG_CFR_WDGTB_MASK)));
    reg_ctl = (wwdg->CR &(~WWDG_CR_T_MASK));
  
    /* configure WIN and PSC bits, configure CNT bit */
    reg_cfg |= WWDG_CFR_W(window);
    reg_cfg |= prescaler;
    reg_ctl |= WWDG_CR_T(counter);
    
    wwdg->CR = reg_ctl | WWDG_CR_WDGA;
    wwdg->CFR = reg_cfg;
}

/*!
    \brief      enable early wakeup interrupt of WWDG
    \param      wwdg
    \retval     none
*/
void wwdg_interrupt_enable(WWDG_TypeDef * wwdg)
{
    wwdg->CFR |= WWDG_CFR_EWI;
}

/*!
    \brief      check early wakeup interrupt state of WWDG
    \param      wwdg
    \retval     FlagStatus SET or RESET
*/
FlagStatus wwdg_flag_get(WWDG_TypeDef * wwdg)
{
    if(wwdg->SR & WWDG_SR_EWIF){
        return SET;
    }

    return RESET;
}

/*!
    \brief      check early wakeup interrupt state of WWDG
    \param      wwdg
    \retval     FlagStatus SET or RESET
*/
FlagStatus wwdg_wait_hardware_flag_get(WWDG_TypeDef * wwdg)
{
    if(wwdg->SR & WWDG_SR_T_UPD){
        return SET;
    }

    return RESET;
}

/*!
    \brief      clear early wakeup interrupt state of WWDG
    \param      wwdg
    \retval     none
*/
void wwdg_flag_clear(WWDG_TypeDef * wwdg)
{
    wwdg->SR &= (~WWDG_SR_EWIF);
}

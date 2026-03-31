/**
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

#include <stdio.h>
#include "ns_sdk_hal.h"

/**
  * \brief configure the TIMER peripheral
  */
void TIMER_Config(void)
{
    TIMER_Output_TypeDef timer_ocinitpara={0};
    TIMER_Init_TypeDef timer_initpara={0};

    Timer_StructParaInit(&timer_initpara);

    #ifdef CFG_SIMULATION
    timer_initpara.prescaler = 0;
    #else
    timer_initpara.prescaler = 8 - 1;
    #endif
    timer_initpara.alignedmode = ADV_TIMER_TIM_CR1_CMS_EDGE_ALIGNED;
    timer_initpara.counterdirection = ADV_TIMER_TIM_CR1_DIR_DISABLE;
    timer_initpara.period = 100;
    timer_initpara.clockdivision = ADV_TIMER_TIM_CR1_CKD_CKD1;
    timer_initpara.repetitioncounter = 0;
    Timer_Init(ADVANCED_TIMER1, &timer_initpara);

    Timer_ChannelOutputStructParaInit(&timer_ocinitpara);

    timer_ocinitpara.outputstate = ADV_TIMER_TIM_CCER_CC1E_ENABLE;
    timer_ocinitpara.outputnstate = ADV_TIMER_TIM_CCER_CC1NE_DISABLE;
    timer_ocinitpara.ocpolarity = ADV_TIMER_TIM_CCER_CC1P_DISABLE;
    timer_ocinitpara.ocnpolarity = ADV_TIMER_TIM_CCER_CC1NP_HIGH;
    timer_ocinitpara.ocidlestate = ADV_TIMER_TIM_CR2_OIS1_DISABLE;
    timer_ocinitpara.ocnidlestate = ADV_TIMER_TIM_CR2_OIS1N_DISABLE;
    Timer_ChannelOutputConfig(ADVANCED_TIMER1, TIMER_CH_1, &timer_ocinitpara);

    timer_ocinitpara.outputstate = ADV_TIMER_TIM_CCER_CC2E_ENABLE;
    timer_ocinitpara.outputnstate = ADV_TIMER_TIM_CCER_CC2NE_DISABLE;
    timer_ocinitpara.ocpolarity = ADV_TIMER_TIM_CCER_CC2P_DISABLE;
    timer_ocinitpara.ocnpolarity = ADV_TIMER_TIM_CCER_CC2NP_HIGH;
    timer_ocinitpara.ocidlestate = ADV_TIMER_TIM_CR2_OIS2_DISABLE;
    timer_ocinitpara.ocnidlestate = ADV_TIMER_TIM_CR2_OIS2N_DISABLE;
    Timer_ChannelOutputConfig(ADVANCED_TIMER1, TIMER_CH_2, &timer_ocinitpara);

    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER1, TIMER_CH_1, 40);
    Timer_ChannelOutputModeConfig(ADVANCED_TIMER1, TIMER_CH_1,  ADV_TIMER_TIM_CCMR1_OC1M_PWM1);
    Timer_ChannelOutputShadowConfig(ADVANCED_TIMER1, TIMER_CH_1, ADV_TIMER_TIM_CCMR1_OC1PE_DISABLE);

    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER1, TIMER_CH_2, 80);
    Timer_ChannelOutputModeConfig(ADVANCED_TIMER1, TIMER_CH_2,  ADV_TIMER_TIM_CCMR1_OC2M_PWM1);
    Timer_ChannelOutputShadowConfig(ADVANCED_TIMER1, TIMER_CH_2, ADV_TIMER_TIM_CCMR1_OC2PE_DISABLE);

    Timer_InputTriggerSourceSelect(ADVANCED_TIMER1, ADV_TIMER_TIM_SMCR_TS_ETRF);

    Timer_PrimaryOutputConfig(ADVANCED_TIMER1, ENABLE);

    Timer_AutoReloadShadowEnable(ADVANCED_TIMER1);

    Timer_Enable(ADVANCED_TIMER1);
}

void iomux_config(void)
{
  iomux_ls_iof_ival_cfg(IOMUX_BASE,ADVANCED_TIMER1_TIM_ETR_0_IOF_IVAL,ADVANCED_TIMER1_TIM_ETR_0_PAD_SEL,ADVANCED_TIMER1_TIM_ETR_0_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER1_TIM_CH1_IOF_OVAL,ADVANCED_TIMER1_TIM_CH1_PAD_SEL,ADVANCED_TIMER1_TIM_CH1_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER1_TIM_CH2_IOF_OVAL,ADVANCED_TIMER1_TIM_CH2_PAD_SEL,ADVANCED_TIMER1_TIM_CH2_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER1_TIM_CH3_IOF_OVAL,ADVANCED_TIMER1_TIM_CH3_PAD_SEL,ADVANCED_TIMER1_TIM_CH3_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER1_TIM_CH4_IOF_OVAL,ADVANCED_TIMER1_TIM_CH4_PAD_SEL,ADVANCED_TIMER1_TIM_CH4_HS_SEL,0,0);
}
/**
  * \brief main
  *
  * \return int
  */
void main(void)
{
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif
    #ifdef MISC_HAS_ADVANCED_TIMER1_HAS_CLK
    advanced_timer1_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_ADVANCED_TIMER1_RST
    advanced_timer1_set_rst(DISABLE);
    advanced_timer1_set_rst(ENABLE);
    #endif

    iomux_config();

    TIMER_Config();
    delay_1ms(2);
    Timer_ChannelOutputClearConfig(ADVANCED_TIMER1,TIMER_CH_1,ADV_TIMER_TIM_CCMR1_OC1CE_ENABLE);
    delay_1ms(2);
    simulation_check();

    while(1) {}
}

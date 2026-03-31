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
    TIMER_Input_TypeDef timer_icinitpara={0};

    Timer_StructParaInit(&timer_initpara);

    #ifdef CFG_SIMULATION
    timer_initpara.prescaler = 0;
    #else
    timer_initpara.prescaler = 8 - 1;
    #endif
    timer_initpara.alignedmode = ADV_TIMER_TIM_CR1_CMS_EDGE_ALIGNED;
    timer_initpara.counterdirection = ADV_TIMER_TIM_CR1_DIR_DISABLE;
    timer_initpara.period = 1500;
    timer_initpara.clockdivision = ADV_TIMER_TIM_CR1_CKD_CKD1;
    timer_initpara.repetitioncounter = 0;
    Timer_Init(ADVANCED_TIMER2, &timer_initpara);

    Timer_AutoReloadShadowDisable(ADVANCED_TIMER2);

    Timer_ChannelOutputStructParaInit(&timer_ocinitpara);

    timer_ocinitpara.ocpolarity = ADV_TIMER_TIM_CCER_CC2P_DISABLE;
    timer_ocinitpara.outputstate = ADV_TIMER_TIM_CCER_CC2E_ENABLE;
    timer_ocinitpara.ocnpolarity = ADV_TIMER_TIM_CCER_CC2NP_HIGH;
    timer_ocinitpara.outputnstate = ADV_TIMER_TIM_CCER_CC2NE_DISABLE;
    timer_ocinitpara.ocidlestate = ADV_TIMER_TIM_CR2_OIS2_DISABLE;
    timer_ocinitpara.ocnidlestate = ADV_TIMER_TIM_CR2_OIS2N_DISABLE;
    Timer_ChannelOutputConfig(ADVANCED_TIMER2, TIMER_CH_2, &timer_ocinitpara);

    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER2, TIMER_CH_2, 500);
    Timer_ChannelOutputModeConfig(ADVANCED_TIMER2, TIMER_CH_2, ADV_TIMER_TIM_CCMR1_OC2M_PWM2);
    Timer_ChannelOutputShadowConfig(ADVANCED_TIMER2, TIMER_CH_2, ADV_TIMER_TIM_CCMR1_OC2PE_DISABLE);

    Timer_ChannelInputStructParaInit(&timer_icinitpara);

    timer_icinitpara.icpolarity  = ADV_TIMER_TIM_CCER_CC1P_ENABLE;
    timer_icinitpara.icselection = ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT;
    timer_icinitpara.icprescaler = ADV_TIMER_TIM_CCMR1_IC1PSC_NO_PRESCALER;
    timer_icinitpara.icfilter    = ADV_TIMER_TIM_CCMR1_IC1F_NO_FILTER;
    Timer_InputCaptureConfig(ADVANCED_TIMER2, TIMER_CH_1, &timer_icinitpara);

    Timer_SinglePulseModeConfig(ADVANCED_TIMER2, ADV_TIMER_TIM_CR1_OPM_ENABLE);

    Timer_PrimaryOutputConfig(ADVANCED_TIMER2, ENABLE);

    Timer_InputTriggerSourceSelect(ADVANCED_TIMER2, ADV_TIMER_TIM_SMCR_TS_TI1FP1);
    Timer_SlaveModeSelect(ADVANCED_TIMER2, ADV_TIMER_TIM_SMCR_SMS_TRIGGER);

}

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,ADVANCED_TIMER2_TIM_CH1_IOF_IVAL,ADVANCED_TIMER2_TIM_CH1_PAD_SEL,ADVANCED_TIMER2_TIM_CH1_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER2_TIM_CH2_IOF_OVAL,ADVANCED_TIMER2_TIM_CH2_PAD_SEL,ADVANCED_TIMER2_TIM_CH2_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER2_TIM_CH3_IOF_OVAL,ADVANCED_TIMER2_TIM_CH3_PAD_SEL,ADVANCED_TIMER2_TIM_CH3_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER2_TIM_CH4_IOF_OVAL,ADVANCED_TIMER2_TIM_CH4_PAD_SEL,ADVANCED_TIMER2_TIM_CH4_HS_SEL,0,0);
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
    #ifdef MISC_HAS_ADVANCED_TIMER2_HAS_CLK
    advanced_timer2_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_ADVANCED_TIMER2_RST
    advanced_timer2_set_rst(DISABLE);
    advanced_timer2_set_rst(ENABLE);
    #endif

    iomux_config();

    uint32_t id = __RV_CSR_READ(CSR_MHARTID);
    if(id==0){

    }else{
        simulation_pass();
        while(1) {}
    }
    TIMER_Config();
    delay_1ms(15);

    while(1) {}
}

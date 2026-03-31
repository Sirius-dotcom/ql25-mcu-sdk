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

#define MAX_COUNT (1500)
#define CH1_COUNT (500)

/**
  * \brief configure the TIMER peripheral
  */
void TIMER_Config(void)
{
    TIMER_Output_TypeDef timer_ocinitpara={0};
    TIMER_Init_TypeDef timer_initpara={0};
    TIMER_Input_TypeDef timer_icinitpara={0};

    /* initialize TIMER init parameter struct */
    Timer_StructParaInit(&timer_initpara);
    /* ADVANCED_TIMER2 configuration */
    #ifdef CFG_SIMULATION
    timer_initpara.prescaler = 0;
    #else
    timer_initpara.prescaler = 8 - 1;
    #endif
    timer_initpara.alignedmode = ADV_TIMER_TIM_CR1_CMS_EDGE_ALIGNED;
    timer_initpara.counterdirection = ADV_TIMER_TIM_CR1_DIR_DISABLE;
    timer_initpara.period = MAX_COUNT;
    timer_initpara.clockdivision = ADV_TIMER_TIM_CR1_CKD_CKD1;
    timer_initpara.repetitioncounter = ADV_TIMER_TIM_RCR_REP_OFS;
    Timer_Init(ADVANCED_TIMER2, &timer_initpara);

    /* auto-reload preload disable */
    Timer_AutoReloadShadowDisable(ADVANCED_TIMER2);

    /* initialize TIMER channel output parameter struct */
    Timer_ChannelOutputStructParaInit(&timer_ocinitpara);
    /* CH1 configuration in OC PWM mode 1 */
    timer_ocinitpara.ocpolarity = ADV_TIMER_TIM_CCER_CC1P_DISABLE;
    timer_ocinitpara.outputstate = ADV_TIMER_TIM_CCER_CC1E_ENABLE;
    timer_ocinitpara.ocnpolarity = ADV_TIMER_TIM_CCER_CC1NP_HIGH;
    timer_ocinitpara.outputnstate = ADV_TIMER_TIM_CCER_CC1NE_ENABLE;
    timer_ocinitpara.ocidlestate = ADV_TIMER_TIM_CR2_OIS1_DISABLE;
    timer_ocinitpara.ocnidlestate = ADV_TIMER_TIM_CR2_OIS1N_DISABLE;
    Timer_ChannelOutputConfig(ADVANCED_TIMER2, TIMER_CH_1, &timer_ocinitpara);

    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER2, TIMER_CH_1, CH1_COUNT);
    Timer_ChannelOutputModeConfig(ADVANCED_TIMER2, TIMER_CH_1, ADV_TIMER_TIM_CCMR1_OC1M_PWM2);
    Timer_ChannelOutputShadowConfig(ADVANCED_TIMER2, TIMER_CH_1, ADV_TIMER_TIM_CCMR1_OC1PE_DISABLE);

    /* initialize TIMER channel input parameter struct */
    Timer_ChannelInputStructParaInit(&timer_icinitpara);

    /* ADVANCED_TIMER2 CH0 input capture configuration */
    timer_icinitpara.icpolarity  = ADV_TIMER_TIM_CCER_CC2P_DISABLE|ADV_TIMER_TIM_CCER_CC2NP_HIGH;
    timer_icinitpara.icselection = ADV_TIMER_TIM_CCMR1_CC2S_TI2;
    timer_icinitpara.icprescaler = ADV_TIMER_TIM_CCMR1_IC2PSC_NO_PRESCALER;
    timer_icinitpara.icfilter    = ADV_TIMER_TIM_CCMR1_IC2F_NO_FILTER;
    Timer_InputCaptureConfig(ADVANCED_TIMER2, TIMER_CH_2, &timer_icinitpara);
    timer_icinitpara.icpolarity  = ADV_TIMER_TIM_CCER_CC4P_LOW;
    timer_icinitpara.icselection = ADV_TIMER_TIM_CCMR2_CC4S_TI4;
    timer_icinitpara.icprescaler = ADV_TIMER_TIM_CCMR2_IC4PSC_NO_PRESCALER;
    timer_icinitpara.icfilter    = ADV_TIMER_TIM_CCMR2_IC4F_NO_FILTER;
    Timer_InputCaptureConfig(ADVANCED_TIMER2, TIMER_CH_4, &timer_icinitpara);

    /* ADVANCED_TIMER2 primary output function enable */
    Timer_PrimaryOutputConfig(ADVANCED_TIMER2, ENABLE);
    /* auto-reload preload enable */
    Timer_AutoReloadShadowEnable(ADVANCED_TIMER2);
    /* auto-reload preload enable */
    Timer_Enable(ADVANCED_TIMER2);
}

void iomux_config(void)
{
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER2_TIM_CH1N_IOF_OVAL,ADVANCED_TIMER2_TIM_CH1N_PAD_SEL,ADVANCED_TIMER2_TIM_CH1N_HS_SEL,0,0);

    iomux_ls_iof_ival_cfg(IOMUX_BASE,ADVANCED_TIMER2_TIM_CH2_IOF_IVAL,ADVANCED_TIMER2_TIM_CH2_PAD_SEL,ADVANCED_TIMER2_TIM_CH2_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,ADVANCED_TIMER2_TIM_CH4_IOF_IVAL,ADVANCED_TIMER2_TIM_CH4_PAD_SEL,ADVANCED_TIMER2_TIM_CH4_HS_SEL,0,0);
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
    uint32_t times = 20;
    uint32_t count2 = 0;
    uint32_t count4 = 0;
    uint32_t status = 0;
    
    #ifdef MISC_HAS_ADVANCED_TIMER2_HAS_CLK
    advanced_timer2_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_ADVANCED_TIMER2_RST
    advanced_timer2_set_rst(DISABLE);
    advanced_timer2_set_rst(ENABLE);
    #endif

    iomux_config();

    TIMER_Config();
    printf("config finish\r\n");
    while (times--) {
        count2 = Timer_ChannelCaptureValueRegisterRead(ADVANCED_TIMER2, TIMER_CH_2);
        count4 = Timer_ChannelCaptureValueRegisterRead(ADVANCED_TIMER2, TIMER_CH_4);
        if (CH1_COUNT == (count4 - count2)) {
            status = 1;
            break;
        }
    }
    if (status) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

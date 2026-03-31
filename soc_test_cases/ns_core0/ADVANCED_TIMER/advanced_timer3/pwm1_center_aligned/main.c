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

uint8_t state0 = 0;
uint8_t state1 = 0;
uint8_t state2 = 0;
uint8_t state3 = 0;

/**
  * \brief This function handles ADVANCED_TIMER3 interrupt request.
  */
void ADVANCED_TIMER3_IRQHandler(void)
{
    if (SET == Timer_InterruptFlagGet(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC1IE_ENABLE)) {

        Timer_InterruptDisable(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC1IE_ENABLE );
        Timer_InterruptFlagClear(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC1IE_ENABLE);
        state0=1;
    } else if (SET == Timer_InterruptFlagGet(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC2IE_ENABLE)) {

        Timer_InterruptDisable(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC1IE_ENABLE );
        Timer_InterruptFlagClear(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC2IE_ENABLE);
        state1=1;

    } else if (SET == Timer_InterruptFlagGet(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC3IE_ENABLE)) {

        Timer_InterruptDisable(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC3IE_ENABLE );
        Timer_InterruptFlagClear(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC3IE_ENABLE);
        state2=1;
    } else if (SET == Timer_InterruptFlagGet(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC4IE_ENABLE)) {

        Timer_InterruptDisable(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC4IE_ENABLE );
        Timer_InterruptFlagClear(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC4IE_ENABLE);
        state3=1;

    } 
}
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
    timer_initpara.prescaler = 800 - 1;
    #endif
    timer_initpara.alignedmode = ADV_TIMER_TIM_CR1_CMS_EDGE_ALIGNED;
    timer_initpara.counterdirection = ADV_TIMER_TIM_CR1_DIR_DISABLE;
    timer_initpara.period = 1000;
    timer_initpara.clockdivision = ADV_TIMER_TIM_CR1_CKD_CKD1;
    timer_initpara.repetitioncounter = 0;
    Timer_Init(ADVANCED_TIMER3, &timer_initpara);

    Timer_ChannelOutputStructParaInit(&timer_ocinitpara);

    timer_ocinitpara.outputstate = ADV_TIMER_TIM_CCER_CC1E_ENABLE;
    timer_ocinitpara.outputnstate = ADV_TIMER_TIM_CCER_CC1NE_DISABLE;
    timer_ocinitpara.ocpolarity = ADV_TIMER_TIM_CCER_CC1P_DISABLE;
    timer_ocinitpara.ocnpolarity = ADV_TIMER_TIM_CCER_CC1NP_HIGH;
    timer_ocinitpara.ocidlestate = ADV_TIMER_TIM_CR2_OIS1_DISABLE;
    timer_ocinitpara.ocnidlestate = ADV_TIMER_TIM_CR2_OIS1N_DISABLE;

    Timer_ChannelOutputConfig(ADVANCED_TIMER3, TIMER_CH_1, &timer_ocinitpara);
    timer_ocinitpara.outputstate = ADV_TIMER_TIM_CCER_CC2E_ENABLE;
    timer_ocinitpara.outputnstate = ADV_TIMER_TIM_CCER_CC2NE_DISABLE;
    timer_ocinitpara.ocpolarity = ADV_TIMER_TIM_CCER_CC2P_DISABLE;
    timer_ocinitpara.ocnpolarity = ADV_TIMER_TIM_CCER_CC2NP_HIGH;
    timer_ocinitpara.ocidlestate = ADV_TIMER_TIM_CR2_OIS2_DISABLE;
    timer_ocinitpara.ocnidlestate = ADV_TIMER_TIM_CR2_OIS2N_DISABLE;
    Timer_ChannelOutputConfig(ADVANCED_TIMER3, TIMER_CH_2, &timer_ocinitpara);
    timer_ocinitpara.outputstate = ADV_TIMER_TIM_CCER_CC3E_ENABLE;
    timer_ocinitpara.outputnstate = ADV_TIMER_TIM_CCER_CC3NE_DISABLE;
    timer_ocinitpara.ocpolarity = ADV_TIMER_TIM_CCER_CC3P_HIGH;
    timer_ocinitpara.ocnpolarity = ADV_TIMER_TIM_CCER_CC3NP_HIGH;
    timer_ocinitpara.ocidlestate = ADV_TIMER_TIM_CR2_OIS3_DISABLE;
    timer_ocinitpara.ocnidlestate = ADV_TIMER_TIM_CR2_OIS3N_DISABLE;
    Timer_ChannelOutputConfig(ADVANCED_TIMER3, TIMER_CH_4, &timer_ocinitpara);
    timer_ocinitpara.outputstate = ADV_TIMER_TIM_CCER_CC4E_ENABLE;

    timer_ocinitpara.ocpolarity = ADV_TIMER_TIM_CCER_CC4P_HIGH;

    timer_ocinitpara.ocidlestate = ADV_TIMER_TIM_CR2_OIS4_DISABLE;

    Timer_ChannelOutputConfig(ADVANCED_TIMER3, TIMER_CH_4, &timer_ocinitpara);

    Timer_CounterAlignment(ADVANCED_TIMER3, ADV_TIMER_TIM_CR1_CMS_CENTER_ALIGNED3);

    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER3,TIMER_CH_1,200);
    Timer_ChannelOutputModeConfig(ADVANCED_TIMER3, TIMER_CH_1, ADV_TIMER_TIM_CCMR1_OC1M_PWM1);
    Timer_ChannelOutputShadowConfig(ADVANCED_TIMER3, TIMER_CH_1, ADV_TIMER_TIM_CCMR1_OC1PE_DISABLE);

    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER3,TIMER_CH_2,400);
    Timer_ChannelOutputModeConfig(ADVANCED_TIMER3, TIMER_CH_2, ADV_TIMER_TIM_CCMR1_OC2M_PWM1);
    Timer_ChannelOutputShadowConfig(ADVANCED_TIMER3, TIMER_CH_2, ADV_TIMER_TIM_CCMR1_OC2PE_DISABLE);

    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER3,TIMER_CH_3,600);
    Timer_ChannelOutputModeConfig(ADVANCED_TIMER3, TIMER_CH_3, ADV_TIMER_TIM_CCMR2_OC3M_PWM1);
    Timer_ChannelOutputShadowConfig(ADVANCED_TIMER3, TIMER_CH_3, ADV_TIMER_TIM_CCMR2_OC3PE_DISABLE);

    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER3,TIMER_CH_4,800);
    Timer_ChannelOutputModeConfig(ADVANCED_TIMER3, TIMER_CH_4, ADV_TIMER_TIM_CCMR2_OC4M_PWM1);
    Timer_ChannelOutputShadowConfig(ADVANCED_TIMER3, TIMER_CH_4, ADV_TIMER_TIM_CCMR2_OC4PE_DISABLE);

    Timer_InterruptEnable(ADVANCED_TIMER3, (ADV_TIMER_TIM_DIER_CC1IE | ADV_TIMER_TIM_DIER_CC2IE | ADV_TIMER_TIM_DIER_CC3IE | ADV_TIMER_TIM_DIER_CC4IE ));
    Timer_PrimaryOutputConfig(ADVANCED_TIMER3, ENABLE);
    Timer_Enable(ADVANCED_TIMER3);
}

void iomux_config(void)
{
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER3_TIM_CH1_IOF_OVAL,ADVANCED_TIMER3_TIM_CH1_PAD_SEL,ADVANCED_TIMER3_TIM_CH1_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER3_TIM_CH2_IOF_OVAL,ADVANCED_TIMER3_TIM_CH2_PAD_SEL,ADVANCED_TIMER3_TIM_CH2_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER3_TIM_CH3_IOF_OVAL,ADVANCED_TIMER3_TIM_CH3_PAD_SEL,ADVANCED_TIMER3_TIM_CH3_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER3_TIM_CH4_IOF_OVAL,ADVANCED_TIMER3_TIM_CH4_PAD_SEL,ADVANCED_TIMER3_TIM_CH4_HS_SEL,0,0);
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
    #ifdef MISC_HAS_ADVANCED_TIMER3_HAS_CLK
    advanced_timer3_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_ADVANCED_TIMER3_RST
    advanced_timer3_set_rst(DISABLE);
    advanced_timer3_set_rst(ENABLE);
    #endif

    iomux_config();

    __enable_irq();
    ECLIC_Register_IRQ(ADVANCED_TIMER3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    ADVANCED_TIMER3_IRQHandler);
    TIMER_Config();
    
    while( (state0==0) |(state1==0) |(state2==0) |(state3==0)){};

    if((state0==1)&(state1==1)&(state2==1)&(state3==1)){
        simulation_pass();
    }else{
        simulation_fail();
    }
    
    while(1) {}
}

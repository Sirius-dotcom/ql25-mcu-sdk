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

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,ADVANCED_TIMER3_TIM_CH1_IOF_IVAL,ADVANCED_TIMER3_TIM_CH1_PAD_SEL,ADVANCED_TIMER3_TIM_CH1_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,ADVANCED_TIMER3_TIM_CH2_IOF_IVAL,ADVANCED_TIMER3_TIM_CH2_PAD_SEL,ADVANCED_TIMER3_TIM_CH2_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER3_TIM_CH3_IOF_OVAL,ADVANCED_TIMER3_TIM_CH3_PAD_SEL,ADVANCED_TIMER3_TIM_CH3_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER3_TIM_CH4_IOF_OVAL,ADVANCED_TIMER3_TIM_CH4_PAD_SEL,ADVANCED_TIMER3_TIM_CH4_HS_SEL,0,0);
}

uint32_t cnt_buf=0;
uint32_t cnt_buf1=0;

uint32_t cnt_buf2=0;
uint32_t cnt_buf3=0;
uint32_t capture_buf[50]={0};

void ADVANCED_TIMER3_IRQHandler(void)
{

    if(Timer_FlagGet(ADVANCED_TIMER3, ADV_TIMER_TIM_SR_CC1IF) == SET)
    {
     
        if(cnt_buf==1){
            if(Timer_FlagGet(ADVANCED_TIMER3, ADV_TIMER_TIM_SR_CC1OF) == SET)
            {
                cnt_buf2 = 1;
                Timer_InterruptDisable(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC1IE); 
                Timer_InterruptFlagClear(ADVANCED_TIMER3, ADV_TIMER_TIM_SR_CC1IF);
                Timer_InterruptFlagClear(ADVANCED_TIMER3, ADV_TIMER_TIM_SR_CC1OF);
            }
        }else{
            Timer_InterruptFlagClear(ADVANCED_TIMER3, ADV_TIMER_TIM_SR_CC1IF);
            cnt_buf = 1;
        }
     
    }
    else if(Timer_FlagGet(ADVANCED_TIMER3, ADV_TIMER_TIM_SR_CC2IF) == SET)
    { 
        if(cnt_buf1==1){
            if(Timer_FlagGet(ADVANCED_TIMER3, ADV_TIMER_TIM_SR_CC2OF) == SET)
            {
                cnt_buf3 = 1;
                Timer_InterruptDisable(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC2IE); 
                Timer_InterruptFlagClear(ADVANCED_TIMER3, ADV_TIMER_TIM_SR_CC2IF);
                Timer_InterruptFlagClear(ADVANCED_TIMER3, ADV_TIMER_TIM_SR_CC2OF);
            }
        }else{
            Timer_InterruptFlagClear(ADVANCED_TIMER3, ADV_TIMER_TIM_SR_CC2IF);
            cnt_buf1 = 1;
        }
        
    }

}

void TIMER_Config(void)
{
    TIMER_Input_TypeDef timer_icinitpara={0};
    Timer_InputTriggerSourceSelect(ADVANCED_TIMER3,ADV_TIMER_TIM_SMCR_TS_TI1F_ED);
    /*****************CH1 Capture Configure***************/

    timer_icinitpara.icselection = ADV_TIMER_TIM_CCMR1_CC1S_TRC;
    timer_icinitpara.icfilter    = ADV_TIMER_TIM_CCMR1_IC1F_FL1_N8; 
    timer_icinitpara.icpolarity  &= (~ADV_TIMER_TIM_CCER_CC1P_ENABLE); 
    timer_icinitpara.icprescaler = ADV_TIMER_TIM_CCMR1_IC1PSC_NO_PRESCALER;
    Timer_InputCaptureConfig(ADVANCED_TIMER3, TIMER_CH_1, &timer_icinitpara);
    Timer_InterruptEnable(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC1IE);  
    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER3, TIMER_CH_1, 12000);
    /*****************************************************/

    /*****************CH2 Capture Configure***************/

    timer_icinitpara.icselection = ADV_TIMER_TIM_CCMR1_CC2S_TRC;
    timer_icinitpara.icfilter = ADV_TIMER_TIM_CCMR1_IC2F_FL1_N8;
    timer_icinitpara.icpolarity  = ADV_TIMER_TIM_CCER_CC2P_ENABLE;
    timer_icinitpara.icprescaler = ADV_TIMER_TIM_CCMR1_IC2PSC_NO_PRESCALER;
    Timer_InputCaptureConfig(ADVANCED_TIMER3, TIMER_CH_2, &timer_icinitpara);
    Timer_InterruptEnable(ADVANCED_TIMER3, ADV_TIMER_TIM_DIER_CC2IE); 
    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER3, TIMER_CH_2, 12000); 
    /*****************************************************/

    ECLIC_Register_IRQ(ADVANCED_TIMER3_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 0, ADVANCED_TIMER3_IRQHandler);

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
    timer_initpara.period = 16000;
    timer_initpara.clockdivision = ADV_TIMER_TIM_CR1_CKD_CKD1;
    timer_initpara.repetitioncounter = 0;
    Timer_Init(ADVANCED_TIMER3, &timer_initpara);

    Timer_ChannelOutputStructParaInit(&timer_ocinitpara);

    timer_ocinitpara.outputstate = ADV_TIMER_TIM_CCER_CC3E_ENABLE;
    timer_ocinitpara.outputnstate = ADV_TIMER_TIM_CCER_CC3NE_DISABLE;
    timer_ocinitpara.ocpolarity = ADV_TIMER_TIM_CCER_CC3P_HIGH;
    timer_ocinitpara.ocnpolarity = ADV_TIMER_TIM_CCER_CC3NP_HIGH;
    timer_ocinitpara.ocidlestate = ADV_TIMER_TIM_CR2_OIS3_DISABLE;
    timer_ocinitpara.ocnidlestate = ADV_TIMER_TIM_CR2_OIS3N_DISABLE;

    Timer_ChannelOutputConfig(ADVANCED_TIMER3, TIMER_CH_3, &timer_ocinitpara);
    timer_ocinitpara.outputstate = ADV_TIMER_TIM_CCER_CC4E_ENABLE;

    timer_ocinitpara.ocpolarity = ADV_TIMER_TIM_CCER_CC4P_HIGH;

    timer_ocinitpara.ocidlestate = ADV_TIMER_TIM_CR2_OIS4_DISABLE;

    Timer_ChannelOutputConfig(ADVANCED_TIMER3, TIMER_CH_4, &timer_ocinitpara);

    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER3, TIMER_CH_3, 12000);
    Timer_ChannelOutputModeConfig(ADVANCED_TIMER3, TIMER_CH_3,ADV_TIMER_TIM_CCMR2_OC3M_PWM1);
    Timer_ChannelOutputShadowConfig(ADVANCED_TIMER3, TIMER_CH_3,ADV_TIMER_TIM_CCMR2_OC3PE_DISABLE);

    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER3, TIMER_CH_4, 12000);
    Timer_ChannelOutputModeConfig(ADVANCED_TIMER3, TIMER_CH_4,ADV_TIMER_TIM_CCMR2_OC4M_PWM1);
    Timer_ChannelOutputShadowConfig(ADVANCED_TIMER3, TIMER_CH_4,ADV_TIMER_TIM_CCMR2_OC4PE_DISABLE);

    Timer_PrimaryOutputConfig(ADVANCED_TIMER3, ENABLE);

    Timer_AutoReloadShadowEnable(ADVANCED_TIMER3);

    Timer_Enable(ADVANCED_TIMER3);

}

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
    __enable_irq();

    iomux_config();

    TIMER_Config();
    while((cnt_buf==0)|(cnt_buf1==0)|(cnt_buf2==0)|(cnt_buf3==0)){}
    if((cnt_buf==1)&(cnt_buf1==1)&(cnt_buf2==1)&(cnt_buf3==1)){
        simulation_pass();
    }else{
        simulation_fail();
    }

    while(1) {}
}

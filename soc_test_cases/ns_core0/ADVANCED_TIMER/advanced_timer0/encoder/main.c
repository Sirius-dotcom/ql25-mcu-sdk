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
    iomux_ls_iof_ival_cfg(IOMUX_BASE,ADVANCED_TIMER0_TIM_CH1_IOF_IVAL,ADVANCED_TIMER0_TIM_CH1_PAD_SEL,ADVANCED_TIMER0_TIM_CH1_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,ADVANCED_TIMER0_TIM_CH2_IOF_IVAL,ADVANCED_TIMER0_TIM_CH2_PAD_SEL,ADVANCED_TIMER0_TIM_CH2_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER0_TIM_CH3_IOF_OVAL,ADVANCED_TIMER0_TIM_CH3_PAD_SEL,ADVANCED_TIMER0_TIM_CH3_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER0_TIM_CH4_IOF_OVAL,ADVANCED_TIMER0_TIM_CH4_PAD_SEL,ADVANCED_TIMER0_TIM_CH4_HS_SEL,0,0);
}
uint32_t cnt_z=0;
uint32_t cnt_f=0;

uint32_t capture_buf[50]={0};

void ADVANCED_TIMER0_IRQHandler(void)
{

    if(Timer_FlagGet(ADVANCED_TIMER0, ADV_TIMER_TIM_DIER_UIE) == SET)
    {
        if((ADVANCED_TIMER0->TIM_CR1 & ADV_TIMER_TIM_CR1_DIR)== ADV_TIMER_TIM_CR1_DIR)
        {
            cnt_z++;
        }else{
            cnt_f++;
        }
    
        Timer_InterruptFlagClear(ADVANCED_TIMER0, ADV_TIMER_TIM_SR_UIF);
            
    }

}

void TIMER_Config(void)
{

    TIMER_Init_TypeDef timer_initpara ={0};

    /* initialize TIMER init parameter struct */
    Timer_StructParaInit(&timer_initpara);
    /* ADVANCED_TIMER0 configuration */
    timer_initpara.prescaler = 0;
    timer_initpara.alignedmode = ADV_TIMER_TIM_CR1_CMS_EDGE_ALIGNED;
    timer_initpara.counterdirection = ADV_TIMER_TIM_CR1_DIR_DISABLE;
    timer_initpara.period = 32;
    timer_initpara.clockdivision = ADV_TIMER_TIM_CR1_CKD_CKD1;
    timer_initpara.repetitioncounter = ADV_TIMER_TIM_RCR_REP_OFS;
    timer_initpara.cnt = 0;
    Timer_Init(ADVANCED_TIMER0, &timer_initpara);

    TIMER_Input_TypeDef timer_icinitpara={0};
    Timer_InputTriggerSourceSelect(ADVANCED_TIMER0,ADV_TIMER_TIM_SMCR_TS_TI1F_ED);
    Timer_SlaveModeSelect(ADVANCED_TIMER0,  ADV_TIMER_TIM_SMCR_SMS_ENCODER3);
    /*****************CH1 Capture Configure***************/

    timer_icinitpara.icselection = ADV_TIMER_TIM_CCMR1_CC1S_TI1;
    timer_icinitpara.icfilter    = ADV_TIMER_TIM_CCMR1_IC1F_FL1_N8; 
    timer_icinitpara.icpolarity  &= (~ADV_TIMER_TIM_CCER_CC1P_ENABLE); 
    timer_icinitpara.icprescaler = ADV_TIMER_TIM_CCMR1_IC1PSC_NO_PRESCALER;
    Timer_InputCaptureConfig(ADVANCED_TIMER0, TIMER_CH_1, &timer_icinitpara);
   
    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER0, TIMER_CH_1, 12000);
    /*****************************************************/

    /*****************CH2 Capture Configure***************/

    timer_icinitpara.icselection = ADV_TIMER_TIM_CCMR1_CC2S_TI2;
    timer_icinitpara.icfilter = ADV_TIMER_TIM_CCMR1_IC2F_FL1_N8;
    timer_icinitpara.icpolarity  &= (~ADV_TIMER_TIM_CCER_CC2P_ENABLE); 
    timer_icinitpara.icprescaler = ADV_TIMER_TIM_CCMR1_IC2PSC_NO_PRESCALER;
    Timer_InputCaptureConfig(ADVANCED_TIMER0, TIMER_CH_2, &timer_icinitpara);
    Timer_InterruptEnable(ADVANCED_TIMER0, ADV_TIMER_TIM_DIER_UIE); 
    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER0, TIMER_CH_2, 12000); 

    /*****************************************************/

    ECLIC_Register_IRQ(ADVANCED_TIMER0_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 0, ADVANCED_TIMER0_IRQHandler);

    Timer_Enable(ADVANCED_TIMER0);

}

void main(void)
{
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif
    #ifdef MISC_HAS_ADVANCED_TIMER0_HAS_CLK
    advanced_timer0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_ADVANCED_TIMER0_RST
    advanced_timer0_set_rst(DISABLE);
    advanced_timer0_set_rst(ENABLE);
    #endif
    __enable_irq();
    uint32_t id = __RV_CSR_READ(CSR_MHARTID);

    if(id==0){

    }else{
        simulation_pass();
        while(1) {}
    }
    iomux_config();
    TIMER_Config();

    while((cnt_f<13)|(cnt_z<13)){}
    if((cnt_f==13)&(cnt_z==13)){
        simulation_pass();
    }else{
        simulation_fail();
    }

    while(1) {
       
    }
}

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
#include <stdlib.h>
#include <string.h>
#include "ns_sdk_hal.h"

#define ADVANCED_TIMER1_CH0CV_ADDR       (ADVANCED_TIMER1_BASE + 0x4c)
#define ARR_SIZE            (3)
uint32_t buffer[ARR_SIZE] = {250, 500, 750};

#define SRC_DATA_ACC_ADDR_BASE           (0x9000000 + 32768 - 0x200)
#define DES_DATA_ACC_ADDR_BASE           (0x9000000 + 32768 - 0x100)

uint8_t state0 = 1;
uint8_t state1 = 1;

void ADVANCED_TIMER1_IRQHandler(void)
{

    if(Timer_FlagGet(ADVANCED_TIMER1, ADV_TIMER_TIM_SR_TIF) == SET)
    {
        state0 = 0;
        Timer_InterruptFlagClear(ADVANCED_TIMER1, ADV_TIMER_TIM_SR_TIF);
    }

}

void UDMA0_IRQHandler(void)
{
    if (UDMA_PA2M_GetITStatus(ADVANCED_TIMER1_TRG_DMA_IRQ, PA2M_FTRANS_IRQ_STAT) == SET) {
        Timer_DmaEnable(ADVANCED_TIMER1, ADV_TIMER_TIM_DIER_TDE_DISABLE);
        state1 = 0;
        UDMA_PA2M_ClearITStatus(ADVANCED_TIMER1_TRG_DMA_IRQ, PA2M_FTRANS_IRQ_CLEAR_STAT);
    }
}

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
    timer_initpara.period = 1000;
    timer_initpara.clockdivision = ADV_TIMER_TIM_CR1_CKD_CKD1;
    timer_initpara.repetitioncounter = 0;
    Timer_Init(ADVANCED_TIMER1, &timer_initpara);

    Timer_ChannelOutputStructParaInit(&timer_ocinitpara);

    timer_ocinitpara.ocpolarity = ADV_TIMER_TIM_CCER_CC3P_HIGH;
    timer_ocinitpara.ocnpolarity = ADV_TIMER_TIM_CCER_CC3NP_HIGH;
    timer_ocinitpara.outputstate = ADV_TIMER_TIM_CCER_CC3E_ENABLE;
    timer_ocinitpara.outputnstate = ADV_TIMER_TIM_CCER_CC3NE_DISABLE;
    timer_ocinitpara.ocidlestate = ADV_TIMER_TIM_CR2_OIS3_DISABLE;
    timer_ocinitpara.ocnidlestate = ADV_TIMER_TIM_CR2_OIS3N_DISABLE;
    Timer_ChannelOutputConfig(ADVANCED_TIMER1, TIMER_CH_3, &timer_ocinitpara);
    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER1, TIMER_CH_3, 500);
    Timer_ChannelOutputModeConfig(ADVANCED_TIMER1, TIMER_CH_3,  ADV_TIMER_TIM_CCMR2_OC3M_PWM1);
    Timer_ChannelOutputShadowConfig(ADVANCED_TIMER1, TIMER_CH_3, ADV_TIMER_TIM_CCMR2_OC3PE_DISABLE);

    Timer_ChannelInputStructParaInit(&timer_icinitpara);

    timer_icinitpara.icpolarity = ADV_TIMER_TIM_CCER_CC1P_DISABLE;
    timer_icinitpara.icselection = ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT;
    timer_icinitpara.icprescaler = ADV_TIMER_TIM_CCMR1_IC1PSC_NO_PRESCALER;
    timer_icinitpara.icfilter = ADV_TIMER_TIM_CCMR1_IC1F_FL1_N2;
    Timer_InputCaptureConfig(ADVANCED_TIMER1, TIMER_CH_1, &timer_icinitpara);

    Timer_PrimaryOutputConfig(ADVANCED_TIMER1, ENABLE);

    Timer_InputTriggerSourceSelect(ADVANCED_TIMER1, ADV_TIMER_TIM_SMCR_TS_TI1FP1);
    Timer_SlaveModeSelect(ADVANCED_TIMER1, ADV_TIMER_TIM_SMCR_SMS_TRIGGER);

    Timer_AutoReloadShadowEnable(ADVANCED_TIMER1);

    Timer_InterruptEnable(ADVANCED_TIMER1, ADV_TIMER_TIM_DIER_TIE | ADV_TIMER_TIM_DIER_TDE);  
    ECLIC_Register_IRQ(ADVANCED_TIMER1_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 0, ADVANCED_TIMER1_IRQHandler);

    Timer_DmaEnable(ADVANCED_TIMER1, ADV_TIMER_TIM_DIER_TDE);
    Timer_DmaTransferConfig(ADVANCED_TIMER1,0x0D,0);
    Timer_Enable(ADVANCED_TIMER1);
   
}

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,ADVANCED_TIMER1_TIM_CH1_IOF_IVAL,ADVANCED_TIMER1_TIM_CH1_PAD_SEL,ADVANCED_TIMER1_TIM_CH1_HS_SEL,0,0);
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

    #ifdef MISC_HAS_UDMA0_HAS_CLK
    udma0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_UDMA0_RST
    udma0_set_rst(DISABLE);
    udma0_set_rst(ENABLE);
    #endif
    
    #ifdef MISC_HAS_ADVANCED_TIMER1_RST
    advanced_timer1_set_rst(DISABLE);
    advanced_timer1_set_rst(ENABLE);
    #endif
    __enable_irq();
    uint32_t id = __RV_CSR_READ(CSR_MHARTID);

    if(id==0){

    }else{
        simulation_pass();
        while(1) {}
    }
    uint8_t *dst_base_addr = (uint8_t *)(DES_DATA_ACC_ADDR_BASE);
    uint8_t *src_base_addr = (uint8_t *)(SRC_DATA_ACC_ADDR_BASE);
    memset((uint8_t *)dst_base_addr, 0, ARR_SIZE*4 );
    memcpy((uint8_t *)src_base_addr, (uint8_t *)buffer, ARR_SIZE*4 );

    UDMA_PAM2MTypeDef UDMA_PAM2MStruct={0};
    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(ADVANCED_TIMER1_CH0CV_ADDR);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO((src_base_addr));
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(ADVANCED_TIMER1_CH0CV_ADDR);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(src_base_addr);
#endif
    UDMA_PAM2MStruct.UDMA_BufferSize = 4*1;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_32BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_ADVANCED_TIMER1_TRG;
    UDMA_PAM2M_Init(ADVANCED_TIMER1_TRG_DMA_CH, &UDMA_PAM2MStruct);

    ECLIC_Register_IRQ(UDMA0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                    ECLIC_LEVEL_TRIGGER, 1, 0,
                    UDMA0_IRQHandler);

    UDMA_PA2M_ITConfig(ADVANCED_TIMER1_TRG_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);

    iomux_config();

    TIMER_Config();
    while((state0==1)|(state1==1)){};
    if((state0==0)&(state1==0)&(ADVANCED_TIMER1->TIM_CCR1==250)){
        simulation_pass();
    }else{
        simulation_fail();
    }

    while(1) {}
}

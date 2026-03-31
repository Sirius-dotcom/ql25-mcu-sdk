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
uint8_t state=1;

#define SRC_DATA_ACC_ADDR_BASE           (0x9000000 + 32768 - 0x200)
#define DES_DATA_ACC_ADDR_BASE           (0x9000000 + 32768 - 0x100)

/**
  * \brief configure the DMA peripheral
  */

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
    timer_initpara.period = 1000;
    timer_initpara.clockdivision = ADV_TIMER_TIM_CR1_CKD_CKD1;
    timer_initpara.repetitioncounter = 1;
    Timer_DmaEnable(ADVANCED_TIMER1, ADV_TIMER_TIM_DIER_CC1DE_ENABLE);

    Timer_ChannelOutputStructParaInit(&timer_ocinitpara);

    timer_ocinitpara.outputstate = ADV_TIMER_TIM_CCER_CC1E_ENABLE;
    timer_ocinitpara.outputnstate = ADV_TIMER_TIM_CCER_CC1NE_DISABLE;
    timer_ocinitpara.ocpolarity = ADV_TIMER_TIM_CCER_CC1P_DISABLE;
    timer_ocinitpara.ocnpolarity = ADV_TIMER_TIM_CCER_CC1NP_HIGH;
    timer_ocinitpara.ocidlestate = ADV_TIMER_TIM_CR2_OIS1_DISABLE;
    timer_ocinitpara.ocnidlestate = ADV_TIMER_TIM_CR2_OIS1N_DISABLE;
    Timer_ChannelOutputConfig(ADVANCED_TIMER1, TIMER_CH_1, &timer_ocinitpara);

    Timer_DmaTransferConfig(ADVANCED_TIMER1,0x0D,0);
    Timer_Init(ADVANCED_TIMER1, &timer_initpara);
    Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER1, TIMER_CH_1, 200);
    Timer_ChannelOutputModeConfig(ADVANCED_TIMER1, TIMER_CH_1,  ADV_TIMER_TIM_CCMR1_OC1M_PWM1);
    Timer_ChannelOutputShadowConfig(ADVANCED_TIMER1, TIMER_CH_1, ADV_TIMER_TIM_CCMR1_OC1PE_DISABLE);
    Timer_PrimaryOutputConfig(ADVANCED_TIMER1, ENABLE);
    Timer_AutoReloadShadowEnable(ADVANCED_TIMER1);

    Timer_Enable(ADVANCED_TIMER1);
}

void iomux_config(void)
{
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER1_TIM_CH1_IOF_OVAL,ADVANCED_TIMER1_TIM_CH1_PAD_SEL,ADVANCED_TIMER1_TIM_CH1_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER1_TIM_CH2_IOF_OVAL,ADVANCED_TIMER1_TIM_CH2_PAD_SEL,ADVANCED_TIMER1_TIM_CH2_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER1_TIM_CH3_IOF_OVAL,ADVANCED_TIMER1_TIM_CH3_PAD_SEL,ADVANCED_TIMER1_TIM_CH3_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,ADVANCED_TIMER1_TIM_CH4_IOF_OVAL,ADVANCED_TIMER1_TIM_CH4_PAD_SEL,ADVANCED_TIMER1_TIM_CH4_HS_SEL,0,0);
}

void UDMA0_IRQHandler(void)
{
     if (UDMA_PA2M_GetITStatus(ADVANCED_TIMER1_CC1_DMA_IRQ, PA2M_FTRANS_IRQ_STAT) == SET) {
        Timer_DmaEnable(ADVANCED_TIMER1, ADV_TIMER_TIM_DIER_CC1DE_DISABLE);
        state=0;
        UDMA_PA2M_ClearITStatus(ADVANCED_TIMER1_CC1_DMA_IRQ, PA2M_FTRANS_IRQ_CLEAR_STAT);
     }
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
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_ADVANCED_TIMER1_CC1;
    UDMA_PAM2M_Init(ADVANCED_TIMER1_CC1_DMA_CH, &UDMA_PAM2MStruct);

    ECLIC_Register_IRQ(UDMA0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                    ECLIC_LEVEL_TRIGGER, 1, 0,
                    UDMA0_IRQHandler);

    UDMA_PA2M_ITConfig(ADVANCED_TIMER1_CC1_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);

    iomux_config();

    TIMER_Config();
    while (state){};

    if(ADVANCED_TIMER1->TIM_CCR1==250){
        simulation_pass();
    }else{
        simulation_fail();
    }
    while(1) {}
}

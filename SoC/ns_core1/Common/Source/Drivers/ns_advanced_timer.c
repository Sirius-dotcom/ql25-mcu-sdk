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

/**
  * \file ns_adv_timer.c
  * \brief TIMER feature API source file for NS_ SDK
  */
#include "ns_advanced_timer.h"

/* TIMER init parameter mask */
#define TIMER_ALIGNEDMODE_MASK            ((uint32_t)0x00000060U)   /**< TIMER init parameter aligne dmode mask */
#define TIMER_COUNTERDIRECTION_MASK       ((uint32_t)0x00000010U)   /**< TIMER init parameter counter direction mask */
#define TIMER_CLOCKDIVISION_MASK          ((uint32_t)0x00000300U)   /**< TIMER init parameter clock division value mask */

/******************Base&General&Advanced********************/
/**
  * \brief initialize TIMER init parameter struct with a default value
  *
  * \param initpara: init parameter struct
  */
void Timer_StructParaInit(TIMER_Init_TypeDef* initpara)
{
    /* initialize the init parameter struct member with the default value */
    initpara->prescaler         = 0U;
    initpara->alignedmode       = ADV_TIMER_TIM_CR1_CMS_EDGE_ALIGNED;
    initpara->counterdirection  = ADV_TIMER_TIM_CR1_DIR_ENABLE;
    initpara->period            = 65535U;
    initpara->clockdivision     = ADV_TIMER_TIM_CR1_CKD_CKD1;
    initpara->repetitioncounter = 0U;
}

/******************Base&General&Advanced********************/
/**
  * \brief initialize TIMER counter
  *
  * \param timer: TIMERx(x=0..4)
  * \param initpara: init parameter struct
  *     \arg initpara->prescaler: prescaler value of the counter clock, 0~65535
  *     \arg initpara->alignedmode: ADV_TIMER_TIM_CR1_CMS_EDGE_ALIGNED, TIMER_COUNTER_CENTER_DOWN, TIMER_COUNTER_CENTER_UP,TIMER_COUNTER_CENTER_BOTH
  *     \arg initpara->counterdirection: ADV_TIMER_TIM_CR1_DIR_DISABLE, TIMER_COUNTER_DOWN
  *     \arg initpara->period: counter auto reload value, 0~65535
  *     \arg initpara->clockdivision: ADV_TIMER_TIM_CR1_CKD_CKD1, ADV_TIMER_TIM_CR1_CKD_CKD2, ADV_TIMER_TIM_CR1_CKD_CKD4
  *     \arg initpara->repetitioncounter: counter repetition value, 0~255
  */
void Timer_Init(ADVANCED_TIMER_TypeDef* timer, TIMER_Init_TypeDef* initpara)
{
    /* configure the counter prescaler value */
    timer->TIM_PSC = (uint32_t)initpara->prescaler;
    /* configure the counter direction and aligned mode */
    timer->TIM_CR1 &= (~(uint32_t)(ADV_TIMER_TIM_CR1_DIR | ADV_TIMER_TIM_CR1_CMS_MASK));
    timer->TIM_CR1 |= (uint32_t)(initpara->alignedmode & TIMER_ALIGNEDMODE_MASK);
    timer->TIM_CR1 |= (uint32_t)(initpara->counterdirection & TIMER_COUNTERDIRECTION_MASK);
    /* configure the autoreload value */
    timer->TIM_CNT = (uint32_t)initpara->cnt;
    timer->TIM_ARR = (uint32_t)initpara->period;
    timer->TIM_RCR = (uint32_t)initpara->repetitioncounter;
    
}

/******************Base&General&Advanced********************/
/**
  * \brief enable a timer
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_Enable(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_CR1 |= (uint32_t)ADV_TIMER_TIM_CR1_CEN;
}

/******************Base&General&Advanced********************/
/**
  * \brief disable a timer
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_Disable(ADVANCED_TIMER_TypeDef* timer)
{
    do { 
        if ((timer->TIM_CCER & (ADV_TIMER_TIM_CCER_CC1E | ADV_TIMER_TIM_CCER_CC2E | ADV_TIMER_TIM_CCER_CC3E | ADV_TIMER_TIM_CCER_CC4E)) == 0U) 
        { 
            if((timer->TIM_CCER & (ADV_TIMER_TIM_CCER_CC1NE | ADV_TIMER_TIM_CCER_CC2NE | ADV_TIMER_TIM_CCER_CC3NE )) == 0U) 
            { 
                timer->TIM_CR1 &= ~(ADV_TIMER_TIM_CR1_CEN); 
            } 
        } 
    } while(0);
}

/******************Base&General&Advanced********************/
/**
  * \brief enable the auto reload shadow function
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_AutoReloadShadowEnable(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_CR1 |= (uint32_t)ADV_TIMER_TIM_CR1_ARPE;
}

/******************Base&General&Advanced********************/
/**
  * \brief disable the auto reload shadow function
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_AutoReloadShadowDisable(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_CR1 &= ~(uint32_t)ADV_TIMER_TIM_CR1_ARPE;
}

/******************Base&General&Advanced********************/
/**
  * \brief  enable the update event
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_UpdateEventEnable(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_CR1 &= ~(uint32_t)ADV_TIMER_TIM_CR1_UDIS;
}

/******************Base&General&Advanced********************/
/**
  * \brief disable the update event
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_UpdateEventDisable(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_CR1 |= (uint32_t) ADV_TIMER_TIM_CR1_UDIS;
}

/******************Base&General&Advanced********************/
/**
  * \brief set TIMER counter alignment mode
  *
  * \param timer: TIMERx(x=0..4)
  * \param aligned: only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CR1_CMS_EDGE_ALIGNED: edge-aligned mode
  *     \arg TIMER_COUNTER_CENTER_DOWN: center-aligned and counting down assert mode
  *     \arg TIMER_COUNTER_CENTER_UP: center-aligned and counting up assert mode
  *     \arg TIMER_COUNTER_CENTER_BOTH: center-aligned and counting up/down assert mode
  */
void Timer_CounterAlignment(ADVANCED_TIMER_TypeDef* timer, uint32_t aligned)
{
    timer->TIM_CR1 &= (uint32_t)(~ADV_TIMER_TIM_CR1_CMS_MASK);
    timer->TIM_CR1 |= (uint32_t)aligned;
}

/******************Base&General&Advanced********************/
/**
  * \brief set TIMER counter up direction
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_CounterUpDirection(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_CR1 &= ~(uint32_t)ADV_TIMER_TIM_CR1_DIR;
}

/******************Base&General&Advanced********************/
/**
  * \brief set TIMER counter down direction
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_CounterDownDirection(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_CR1 |= (uint32_t)ADV_TIMER_TIM_CR1_DIR;
}

/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER prescaler
  *
  * \param timer: TIMERx(x=0..4)
  * \param prescaler: prescaler value
  * \param pscreload: prescaler reload mode only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_EGR_UG: the prescaler is loaded right now
  *     \arg TIMER_PSC_RELOAD_UPDATE: the prescaler is loaded at the next update event
  */
void Timer_PrescalerConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t pscreload)
{
    if (ADV_TIMER_TIM_EGR_UG == pscreload) {
        timer->TIM_EGR |= (uint32_t)ADV_TIMER_TIM_EGR_UG;
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER repetition register value
  *
  * \param timer: TIMERx(x=0..4)
  * \param repetition: the counter repetition value, 0~255
  */
void Timer_RepetitionValueConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t repetition)
{
    timer->TIM_RCR |= (uint32_t)repetition;
}

/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER autoreload register value
  *
  * \param timer: TIMERx(x=0..4)
  * \param autoreload: the counter auto-reload value
  */
void Timer_AutoReloadValueConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t autoreload)
{
    timer->TIM_ARR = (uint32_t)autoreload;
}

/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER counter register value
  *
  * \param timer: TIMERx(x=0..4)
  * \param counter: the counter value
  */
void Timer_CounterValueConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t counter)
{
    timer->TIM_CNT = (uint32_t)counter;
}

/******************Base&General&Advanced********************/
/**
  * \brief read TIMER counter value
  *
  * \param timer: TIMERx(x=0..4)
  *
  * \return counter value
  */
uint32_t Timer_CounterRead(ADVANCED_TIMER_TypeDef* timer)
{
    uint32_t count_value = 0U;
    count_value = timer->TIM_CNT;
    return (count_value);
}

/******************Base&General&Advanced********************/
/**
  * \brief read TIMER prescaler value
  *
  * \param timer: TIMERx(x=0..4)
  *
  * \return prescaler register value
  */
uint32_t Timer_PrescalerRead(ADVANCED_TIMER_TypeDef* timer)
{
    uint32_t prescaler_value = 0U;
    prescaler_value = (uint32_t) (timer->TIM_PSC);
    return (prescaler_value);
}

/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER single pulse mode
  *
  * \param timer: TIMERx(x=0..4)
  * \param spmode: only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CR1_OPM_ENABLE: single pulse mode
  *     \arg ADV_TIMER_TIM_CR1_OPM_DISABLE: repetitive pulse mode
  */
void Timer_SinglePulseModeConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t spmode)
{
    if (ADV_TIMER_TIM_CR1_OPM_ENABLE == spmode) {
        timer->TIM_CR1 |= (uint32_t)ADV_TIMER_TIM_CR1_OPM;
    } else if (ADV_TIMER_TIM_CR1_OPM_DISABLE == spmode) {
        timer->TIM_CR1 &= ~((uint32_t)ADV_TIMER_TIM_CR1_OPM);
    } else {
        /* illegal parameters */
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER update source
  *
  * \param timer: TIMERx(x=0..4)
  * \param update: only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CR1_URS_DISABLE: update generate by setting of UPG bit or the counter overflow/underflow,
  *         or the slave mode controller trigger
  *     \arg ADV_TIMER_TIM_CR1_URS_ENABLE: update generate only by counter overflow/underflow
  */
void Timer_UpdateSourceConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t update)
{
    if (ADV_TIMER_TIM_CR1_URS_ENABLE == update) {
        timer->TIM_CR1 |= (uint32_t)ADV_TIMER_TIM_CR1_URS;
    } else if (ADV_TIMER_TIM_CR1_URS_DISABLE == update) {
        timer->TIM_CR1 &= ~(uint32_t)ADV_TIMER_TIM_CR1_URS;
    } else {
        /* illegal parameters */
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief enable the TIMER DMA
  *
  * \param timer: TIMERx(x=0..4)
  * \param dma: specify which DMA to enable one or more parameters can be selected which are shown as below:
  *     \arg ADV_TIMER_TIM_DIER_UDE_ENABLE:  update DMA enable, TIMERx(x=0..4)
  *     \arg TIMER_DMA_CH1D: channel 0 DMA enable, TIMERx(x=0..4)
  *     \arg TIMER_DMA_CH2D: channel 1 DMA enable, TIMERx(x=0..4)
  *     \arg TIMER_DMA_CH3D: channel 2 DMA enable, TIMERx(x=0..4)
  *     \arg TIMER_DMA_CH4D: channel 3 DMA enable, TIMERx(x=0..4)
  *     \arg TIMER_DMA_CMTD: channel commutation DMA request enable, TIMERx(x=0..4)
  *     \arg TIMER_DMA_TRGD: trigger DMA enable, TIMERx(x=0..4)
  */
void Timer_DmaEnable(ADVANCED_TIMER_TypeDef* timer, uint32_t dma)
{
    timer->TIM_DIER |= (uint32_t) dma;
}

/******************Base&General&Advanced********************/
/**
  * \brief disable the TIMER DMA
  *
  * \param timer: TIMERx(x=0..4)
  * \param dma: specify which DMA to disbale one or more parameters can be selected which are shown as below:
  *     \arg ADV_TIMER_TIM_DIER_UDE_ENABLE:  update DMA enable, TIMERx(x=0..4)
  *     \arg TIMER_DMA_CH1D: channel 0 DMA enable, TIMERx(x=0..4)
  *     \arg TIMER_DMA_CH2D: channel 1 DMA enable, TIMERx(x=0..4)
  *     \arg TIMER_DMA_CH3D: channel 2 DMA enable, TIMERx(x=0..4)
  *     \arg TIMER_DMA_CH4D: channel 3 DMA enable, TIMERx(x=0..4)
  *     \arg TIMER_DMA_CMTD: channel commutation DMA request enable, TIMERx(x=0..4)
  *     \arg TIMER_DMA_TRGD: trigger DMA enable, TIMERx(x=0..4,7)
  */
void Timer_DmaDisable(ADVANCED_TIMER_TypeDef* timer, uint32_t dma)
{
    timer->TIM_DIER &= (~(uint32_t)(dma));
}

/******************Base&General&Advanced********************/
/**
  * \brief channel DMA request source selection
  *
  * \param timer: TIMERx(x=0..4)
  * \param dma_request: channel DMA request source selection only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CR2_CCDS_DISABLE: DMA request of channel n is sent when channel n event occurs
  *     \arg ADV_TIMER_TIM_CR2_CCDS_ENABLE: DMA request of channel n is sent when update event occurs
  */
void Timer_ChannelDmaRequestSourceSelect(ADVANCED_TIMER_TypeDef* timer, uint32_t dma_request)
{
    if (ADV_TIMER_TIM_CR2_CCDS_ENABLE == dma_request) {
        timer->TIM_CR2 |= (uint32_t)ADV_TIMER_TIM_CR2_CCDS;
    } else if (ADV_TIMER_TIM_CR2_CCDS_DISABLE == dma_request) {
        timer->TIM_CR2 &= ~(uint32_t)ADV_TIMER_TIM_CR2_CCDS;
    } else {
        /* illegal parameters */
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief configure the TIMER DMA transfer
  *
  * \param timer: TIMERx(x=0..4)
  * \param dma_baseaddr: only one parameter can be selected which is shown as below:
  *     \arg TIMER_DCR_DMATA_CTL0: DMA transfer address is TIMER_CR1, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_CTL1: DMA transfer address is TIMER_CR2, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_SMCFG: DMA transfer address is TIMER_SMCR, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_DMAINTEN: DMA transfer address is TIMER_DIER, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_INTF: DMA transfer address is TIMER_SR, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_SWEVG: DMA transfer address is TIMER_EGR, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_CHCTL0: DMA transfer address is TIMER_CCMR1, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_CHCTL1: DMA transfer address is TIMER_CCMR2, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_CHCTL2: DMA transfer address is TIMER_CCER, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_CNT: DMA transfer address is TIMER_CNT, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_PSC: DMA transfer address is TIMER_PSC, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_CAR: DMA transfer address is TIMER_AAR, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_CREP: DMA transfer address is TIMER_RCR, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_CH0CV: DMA transfer address is TIMER_CCR1, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_CH1CV: DMA transfer address is TIMER_CCR2, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_CH2CV: DMA transfer address is TIMER_CCR3, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_CH3CV: DMA transfer address is TIMER_CCR4, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_CCHP: DMA transfer address is TIMER_BDTR, TIMERx(x=0..4)
  *     \arg TIMER_DCR_DMATA_DMACFG: DMA transfer address is TIMER_DCR, TIMERx(x=0..4)
  * \param dma_lenth: only one parameter can be selected which is shown as below:
  *     \arg TIMER_DCR_DMATC_xTRANSFER(x=1..6): DMA transfer x time
  */
void Timer_DmaTransferConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t dma_baseaddr, uint32_t dma_lenth)
{
    timer->TIM_DCR &= (~(uint32_t)(ADV_TIMER_TIM_DCR_DBA_MASK | ADV_TIMER_TIM_DCR_DBL_MASK));
    timer->TIM_DCR |= (uint32_t)(dma_baseaddr | (dma_lenth<<8));
}

/******************Base&General&Advanced********************/
/**
  * \brief software generate events
  *
  * \param timer: TIMERx(x=0..4)
  * \param event: the timer software event generation sources one or more parameters can be selected which are shown as below:
  *     \arg TIMER_EVENT_SRC_UPG: update event generation, TIMERx(x=0..4)
  *     \arg TIMER_EVENT_SRC_CH1G: channel 0 capture or compare event generation, TIMERx(x=0..4)
  *     \arg TIMER_EVENT_SRC_CH2G: channel 1 capture or compare event generation, TIMERx(x=0..4)
  *     \arg TIMER_EVENT_SRC_CH3G: channel 2 capture or compare event generation, TIMERx(x=0..4)
  *     \arg TIMER_EVENT_SRC_CH4G: channel 3 capture or compare event generation, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_EGR_TG: channel commutation event generation, TIMERx(x=0..4)
  *     \arg TIMER_EVENT_SRC_TRGG: trigger event generation, TIMERx(x=0..4)
  *     \arg TIMER_EVENT_SRC_BRKG:  break event generation, TIMERx(x=0..4)
  */
void Timer_EventSoftwareGenerate(ADVANCED_TIMER_TypeDef* timer, uint32_t event)
{
    timer->TIM_EGR |= (uint32_t)event;
}

/******************Advanced********************/
/**
  * \brief initialize TIMER break parameter struct with a default value
  *
  * \param breakpara: TIMER break parameter struct
  */
void Timer_BreakStructParaInit(TIMER_Break_TypeDef* breakpara)
{
    /* initialize the break parameter struct member with the default value */
    breakpara->runoffstate     = ADV_TIMER_TIM_BDTR_OSSR_ENABLE;
    breakpara->ideloffstate    = ADV_TIMER_TIM_BDTR_OSSI_ENABLE;
    breakpara->deadtime        = 0U;
    breakpara->breakpolarity   = ADV_TIMER_TIM_BDTR_BKP_HIGH | ADV_TIMER_TIM_BDTR_BK2P_ENABLE;
    breakpara->outputautostate = ADV_TIMER_TIM_BDTR_AOE_SOFTWARE;
    breakpara->protectmode     = ADV_TIMER_TIM_BDTR_LOCK_LOCK_OFF;
    breakpara->breakstate      = ADV_TIMER_TIM_BDTR_BKE_DISABLE;
}

/******************Advanced********************/
/**
  * \brief configure TIMER break function
  *
  * \param timer: TIMERx(x=0..4)
  * \param breakpara: TIMER break parameter struct
  *     \arg breakpara->runoffstate: ADV_TIMER_TIM_BDTR_OSSR_ENABLE, ADV_TIMER_TIM_BDTR_OSSR_DISABLE
  *     \arg breakpara->ideloffstate: ADV_TIMER_TIM_BDTR_OSSI_ENABLE, ADV_TIMER_TIM_BDTR_OSSI_DISABLE
  *     \arg breakpara->deadtime: 0~255
  *     \arg breakpara->breakpolarity: ADV_TIMER_TIM_BDTR_BKP_LOW, TIMER_BREAK_POLARITY_HIGH
  *     \arg breakpara->outputautostate: ADV_TIMER_TIM_BDTR_AOE_SOFTWARE_AUTOMATICALLY, ADV_TIMER_TIM_BDTR_AOE_SOFTWARE
  *     \arg breakpara->protectmode: ADV_TIMER_TIM_BDTR_LOCK_LOCK_OFF, TIMER_BDTR_PROT_0, TIMER_BDTR_PROT_1, TIMER_BDTR_PROT_2
  *     \arg breakpara->breakstate: TIMER_BREAK_ENABLE, ADV_TIMER_TIM_BDTR_BKE_DISABLE
  */
void Timer_BreakConfig(ADVANCED_TIMER_TypeDef* timer, TIMER_Break_TypeDef* breakpara)
{
    timer->TIM_BDTR = (uint32_t)(((uint32_t)(breakpara->runoffstate)) |
                    ((uint32_t)(breakpara->ideloffstate)) |
                    ((uint32_t)(breakpara->deadtime)) |
                    ((uint32_t)(breakpara->breakpolarity)) |
                    ((uint32_t)(breakpara->outputautostate)) |
                    ((uint32_t)(breakpara->protectmode)) |
                    ((uint32_t)(breakpara->breakstate)));
}

/******************Advanced********************/
/**
  * \brief enable TIMER break function
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_BreakEnable(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_BDTR |= (uint32_t)ADV_TIMER_TIM_BDTR_BKE;
}

/******************Advanced********************/
/**
  * \brief disable TIMER break function
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_BreakDisable(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_BDTR &= ~(uint32_t)ADV_TIMER_TIM_BDTR_BKE;
}

/**
  * \brief enable TIMER break function
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_Break2Enable(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_BDTR |= (uint32_t)ADV_TIMER_TIM_BDTR_BK2E;
}

/******************Advanced********************/
/**
  * \brief disable TIMER break function
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_Break2Disable(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_BDTR &= ~(uint32_t)ADV_TIMER_TIM_BDTR_BK2E;
}
/******************Base&General&Advanced********************/
/**
  * \brief enable TIMER output automatic function
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_AutoMaticOutputEnable(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_BDTR |= (uint32_t)ADV_TIMER_TIM_BDTR_AOE;
}

/******************Base&General&Advanced********************/
/**
  * \brief disable TIMER output automatic function
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_AutoMaticOutputDisable(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_BDTR &= ~(uint32_t)ADV_TIMER_TIM_BDTR_AOE;
}

/******************General&Advanced********************/
/**
  * \brief enable or disable TIMER primary output function
  *
  * \param timer: TIMERx(x=0..4)
  * \param newvalue: ENABLE or DISABLE
  */
void Timer_PrimaryOutputConfig(ADVANCED_TIMER_TypeDef* timer, ControlStatus newvalue)
{
    if (ENABLE == newvalue) {
        timer->TIM_BDTR |= (uint32_t)ADV_TIMER_TIM_BDTR_MOE;
    } else {
        timer->TIM_BDTR &= (~(uint32_t)ADV_TIMER_TIM_BDTR_MOE);
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief enable or disable channel capture/compare control shadow register
  *
  * \param timer: TIMERx(x=0..4)
  * \param newvalue: ENABLE or DISABLE
  */
void Timer_ChannelControlShadowConfig(ADVANCED_TIMER_TypeDef* timer, ControlStatus newvalue)
{
    if (ENABLE == newvalue) {
        timer->TIM_CR2 |= (uint32_t)ADV_TIMER_TIM_CR2_CCPC;
    } else {
        timer->TIM_CR2 &= (~(uint32_t)ADV_TIMER_TIM_CR2_CCPC);
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER channel control shadow register update control
  *
  * \param timer: TIMERx(x=0..4)
  * \param ccuctl: channel control shadow register update control only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CR2_CCUS_DISABLE: the shadow registers update by when CMTG bit is set
  *     \arg ADV_TIMER_TIM_CR2_CCUS_ENABLE: the shadow registers update by when CMTG bit is set or an rising edge of TRGI occurs
  */
void Timer_ChannelControlShadowUpdateConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t ccuctl)
{
    if (ADV_TIMER_TIM_CR2_CCUS_DISABLE == ccuctl) {
        timer->TIM_CR2 &= (~(uint32_t)ADV_TIMER_TIM_CR2_CCUS);
    } else if (ADV_TIMER_TIM_CR2_CCUS_ENABLE == ccuctl) {
        timer->TIM_CR2 |= (uint32_t)ADV_TIMER_TIM_CR2_CCUS;
    } else {
        /* illegal parameters */
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief initialize TIMER channel output parameter struct with a default value
  *
  * \param ocpara: TIMER channel n output parameter struct
  */
void Timer_ChannelOutputStructParaInit(TIMER_Output_TypeDef* ocpara)
{
    /* initialize the channel output parameter struct member with the default value */
    ocpara->outputstate  = ADV_TIMER_TIM_CCER_CC1E_DISABLE;
    ocpara->outputnstate = ADV_TIMER_TIM_CCER_CC1NE_DISABLE;
    ocpara->ocpolarity   = ADV_TIMER_TIM_CCER_CC1P_DISABLE;
    ocpara->ocnpolarity  = ADV_TIMER_TIM_CCER_CC1NP_HIGH;
    ocpara->ocidlestate  = ADV_TIMER_TIM_CR2_OIS1_DISABLE;
    ocpara->ocnidlestate = ADV_TIMER_TIM_CR2_OIS1N_DISABLE;
}

/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER channel output function
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  *     \arg TIMER_CH_4: TIMER channel 4(TIMERx(x=0..4))
  * \param ocpara: TIMER channeln output parameter struct
  *     \arg ocpara->outputstate: ADV_TIMER_TIM_CCER_CC1E_ENABLE, ADV_TIMER_TIM_CCER_CC1E_DISABLE
  *     \arg ocpara->outputnstate: ADV_TIMER_TIM_CCER_CC1NE_ENABLE, ADV_TIMER_TIM_CCER_CC1NE_DISABLE
  *     \arg ocpara->ocpolarity: ADV_TIMER_TIM_CCER_CC1P_DISABLE, TIMER_OC_POLARITY_LOW
  *     \arg ocpara->ocnpolarity: ADV_TIMER_TIM_CCER_CC1NP_HIGH, TIMER_OCN_POLARITY_LOW
  *     \arg ocpara->ocidlestate: ADV_TIMER_TIM_CR2_OIS1_DISABLE, ADV_TIMER_TIM_CR2_OIS1_ENABLE
  *     \arg ocpara->ocnidlestate: ADV_TIMER_TIM_CR2_OIS1N_DISABLE, ADV_TIMER_TIM_CR2_OIS1N_ENABLE
  */
void Timer_ChannelOutputConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, TIMER_Output_TypeDef* ocpara)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            /* reset the CH0EN bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC1E);
            /* set the CH0EN bit */
            timer->TIM_CCER |= (uint32_t)ocpara->outputstate;
            /* reset the CH0P bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC1P);
            /* set the CH0P bit */
            timer->TIM_CCER |= (uint32_t)ocpara->ocpolarity;
            /* reset the CH0NEN bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC1NE);
            /* set the CH0NEN bit */
            timer->TIM_CCER |= (uint32_t)ocpara->outputnstate;
            /* reset the CH0NP bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC1NP);
            /* set the CH0NP bit */
            timer->TIM_CCER |= (uint32_t)ocpara->ocnpolarity;
            /* reset the ISO0 bit */
            timer->TIM_CR2 &= (~(uint32_t)ADV_TIMER_TIM_CR2_OIS1);
            /* set the ISO0 bit */
            timer->TIM_CR2 |= (uint32_t)ocpara->ocidlestate;
            /* reset the ISO0N bit */
            timer->TIM_CR2 &= (~(uint32_t)ADV_TIMER_TIM_CR2_OIS1N);
            /* set the ISO0N bit */
            timer->TIM_CR2 |= (uint32_t)ocpara->ocnidlestate;
            timer->TIM_CCMR1 &= ~(uint32_t)ADV_TIMER_TIM_CCMR1_CC1S_MASK;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            /* reset the CH1EN bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC2E);
            /* set the CH1EN bit */
            timer->TIM_CCER |= (uint32_t)((uint32_t)(ocpara->outputstate));
            /* reset the CH1P bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC2P);
            /* set the CH1P bit */
            timer->TIM_CCER |= (uint32_t)((uint32_t)(ocpara->ocpolarity));
            /* reset the CH1NEN bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC2NE);
            /* set the CH1NEN bit */
            timer->TIM_CCER |= (uint32_t)((uint32_t)(ocpara->outputnstate));
            /* reset the CH1NP bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC2NP);
            /* set the CH1NP bit */
            timer->TIM_CCER |= (uint32_t)((uint32_t)(ocpara->ocnpolarity));
            /* reset the ISO1 bit */
            timer->TIM_CR2 &= (~(uint32_t)ADV_TIMER_TIM_CR2_OIS2);
            /* set the ISO1 bit */
            timer->TIM_CR2 |= (uint32_t)((uint32_t)(ocpara->ocidlestate));
            /* reset the ISO1N bit */
            timer->TIM_CR2 &= (~(uint32_t)ADV_TIMER_TIM_CR2_OIS2N);
            /* set the ISO1N bit */
            timer->TIM_CR2 |= (uint32_t)((uint32_t)(ocpara->ocnidlestate));
            timer->TIM_CCMR1 &= ~(uint32_t)ADV_TIMER_TIM_CCMR1_CC2S_MASK;
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            /* reset the CH2EN bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC3E);
            /* set the CH2EN bit */
            timer->TIM_CCER |= (uint32_t)((uint32_t)(ocpara->outputstate) );
            /* reset the CH2P bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC3P);
            /* set the CH2P bit */
            timer->TIM_CCER |= (uint32_t)((uint32_t)(ocpara->ocpolarity) );
            /* reset the CH2NEN bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC3NE);
            /* set the CH2NEN bit */
            timer->TIM_CCER |= (uint32_t)((uint32_t)(ocpara->outputnstate) );
            /* reset the CH2NP bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC3NP);
            /* set the CH2NP bit */
            timer->TIM_CCER |= (uint32_t)((uint32_t)(ocpara->ocnpolarity));
            /* reset the ISO2 bit */
            timer->TIM_CR2 &= (~(uint32_t)ADV_TIMER_TIM_CR2_OIS3);
            /* set the ISO2 bit */
            timer->TIM_CR2 |= (uint32_t)((uint32_t)(ocpara->ocidlestate) );
            /* reset the ISO2N bit */
            timer->TIM_CR2 &= (~(uint32_t)ADV_TIMER_TIM_CR2_OIS3N);
            /* set the ISO2N bit */
            timer->TIM_CR2 |= (uint32_t)((uint32_t)(ocpara->ocnidlestate));
            timer->TIM_CCMR2 &= ~(uint32_t)ADV_TIMER_TIM_CCMR2_CC3S_MASK;
            break;
        /* configure TIMER_CH_4 */
        case TIMER_CH_4:
            /* reset the CH3EN bit */
            timer->TIM_CCER &=(~(uint32_t)ADV_TIMER_TIM_CCER_CC4E);
            /* set the CH3EN bit */
            timer->TIM_CCER |= (uint32_t)((uint32_t)(ocpara->outputstate) );
            /* reset the CH3P bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC4P);
            /* set the CH3P bit */
            timer->TIM_CCER |= (uint32_t)((uint32_t)(ocpara->ocpolarity) );
            /* reset the ISO3 bit */
            timer->TIM_CR2 &= (~(uint32_t)ADV_TIMER_TIM_CR2_OIS4);
            /* set the ISO3 bit */
            timer->TIM_CR2 |= (uint32_t)((uint32_t)(ocpara->ocidlestate));
            timer->TIM_CCMR2 &= ~(uint32_t)ADV_TIMER_TIM_CCMR2_CC4S_MASK;
            break;
        default:
            break;
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER channel output compare mode
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  *     \arg TIMER_CH_4: TIMER channel 4(TIMERx(x=0..4))
  * \param ocmode: channel output compare mode only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CCMR1_OC1M_FROZEN: timing mode
  *     \arg TIMER_OC_MODE_ACTIVE: active mode
  *     \arg TIMER_OC_MODE_INACTIVE: inactive mode
  *     \arg TIMER_OC_MODE_TOGGLE: toggle mode
  *     \arg TIMER_OC_MODE_LOW: force low mode
  *     \arg TIMER_OC_MODE_HIGH: force high mode
  *     \arg TIMER_OC_MODE_PWM0: PWM mode 0
  *     \arg ADV_TIMER_TIM_CCMR1_OC1M_PWM2: PWM mode 1
  */
void Timer_ChannelOutputModeConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ocmode)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_OC1M_MASK);
            timer->TIM_CCMR1 |= (uint32_t)ocmode;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_OC2M_MASK);
            timer->TIM_CCMR1 |= (uint32_t)ocmode;
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_OC3M_MASK);
            timer->TIM_CCMR2 |= (uint32_t)ocmode;
            break;
        /* configure TIMER_CH_4 */
        case TIMER_CH_4:
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_OC4M_MASK);
            timer->TIM_CCMR2 |= (uint32_t)ocmode;
            break;
        default:
            break;
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER channel output compare mode
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  *     \arg TIMER_CH_4: TIMER channel 4(TIMERx(x=0..4))
  * \param ocfe: Output Compare x fast enable
  */
void Timer_ChannelFastOutput(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ocxfe)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_OC1FE);
            timer->TIM_CCMR1 |= (uint32_t)ocxfe;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_OC2FE);
            timer->TIM_CCMR1 |= (uint32_t)ocxfe;
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_OC3FE);
            timer->TIM_CCMR2 |= (uint32_t)ocxfe;
            break;
        /* configure TIMER_CH_4 */
        case TIMER_CH_4:
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_OC4FE);
            timer->TIM_CCMR2 |= (uint32_t)ocxfe;
            break;
        default:
            break;
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER channel output compare mode
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  *     \arg TIMER_CH_4: TIMER channel 4(TIMERx(x=0..4))
  * \param ocfe: Output Compare x fast enable
  */
void Timer_ChannelFastOutputDelay(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ofdelay)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            timer->TIM_OUTPUT_DELAY &= (~(uint32_t)ADV_TIMER_TIM_OUTPUT_DELAY_OC1_DELAY_MASK);
            timer->TIM_OUTPUT_DELAY |= (uint32_t)ofdelay;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            timer->TIM_OUTPUT_DELAY &= (~(uint32_t)ADV_TIMER_TIM_OUTPUT_DELAY_OC2_DELAY_MASK);
            timer->TIM_OUTPUT_DELAY |= (uint32_t)ofdelay;
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            timer->TIM_OUTPUT_DELAY &= (~(uint32_t)ADV_TIMER_TIM_OUTPUT_DELAY_OC3_DELAY_MASK);
            timer->TIM_OUTPUT_DELAY |= (uint32_t)ofdelay;
            break;
        /* configure TIMER_CH_4 */
        case TIMER_CH_4:
            timer->TIM_OUTPUT_DELAY &= (~(uint32_t)ADV_TIMER_TIM_OUTPUT_DELAY_OC4_DELAY_MASK);
            timer->TIM_OUTPUT_DELAY |= (uint32_t)ofdelay;
            break;
        default:
            break;
    }
}
/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER channel output pulse value
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  *     \arg TIMER_CH_4: TIMER channel 4(TIMERx(x=0..4))
  * \param pulse: channel output pulse value
  */
void Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t pulse)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            timer->TIM_CCR1 = (uint32_t)pulse;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            timer->TIM_CCR2 = (uint32_t)pulse;
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            timer->TIM_CCR3 = (uint32_t)pulse;
            break;
        /* configure TIMER_CH_4 */
        case TIMER_CH_4:
            timer->TIM_CCR4 = (uint32_t)pulse;
            break;
        default:
            break;
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief configure TIMER channel output shadow function
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  *     \arg TIMER_CH_4: TIMER channel 4(TIMERx(x=0..4))
  * \param ocshadow: channel output shadow state only one parameter can be selected which is shown as below:
  *     \arg TIMER_OC_SHADOW_ENABLE: channel output shadow state enable
  *     \arg ADV_TIMER_TIM_CCMR1_OC1PE_DISABLE: channel output shadow state disable
  */
void Timer_ChannelOutputShadowConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ocshadow)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_OC1PE);
            timer->TIM_CCMR1 |= (uint32_t)ocshadow;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_OC2PE);
            timer->TIM_CCMR1 |= (uint32_t)ocshadow;
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_OC3PE);
            timer->TIM_CCMR2 |= (uint32_t)ocshadow;
            break;
        /* configure TIMER_CH_4 */
        case TIMER_CH_4:
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_OC4PE);
            timer->TIM_CCMR2 |= (uint32_t)ocshadow;
            break;
        default:
            break;
    }
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER channel output clear function
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..41))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  *     \arg TIMER_CH_4: TIMER channel 4(TIMERx(x=0..4))
  * \param occlear: channel output clear function only one parameter can be selected which is shown as below:
  *     \arg TIMER_OC_CLEAR_ENABLE: channel output clear function enable
  *     \arg TIMER_OC_CLEAR_DISABLE: channel output clear function disable
  */
void Timer_ChannelOutputClearConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t occlear)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_OC1CE);
            timer->TIM_CCMR1 |= (uint32_t)occlear;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_OC2CE);
            timer->TIM_CCMR1 |= (uint32_t)((uint32_t)occlear);
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_OC3CE);
            timer->TIM_CCMR2 |= (uint32_t)occlear;
            break;
        /* configure TIMER_CH_4 */
        case TIMER_CH_4:
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_OC4CE);
            timer->TIM_CCMR2 |= (uint32_t)((uint32_t)occlear );
            break;
        default:
            break;
    }
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER channel output polarity
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  *     \arg TIMER_CH_4: TIMER channel 4(TIMERx(x=0..4))
  * \param ocpolarity: channel output polarity only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CCER_CC1P_DISABLE: channel output polarity is high
  *     \arg TIMER_OC_POLARITY_LOW: channel output polarity is low
  */
void Timer_ChannelOutputPolarityConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ocpolarity)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC1P);
            timer->TIM_CCER |= (uint32_t)ocpolarity;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC2P);
            timer->TIM_CCER |= (uint32_t)((uint32_t)ocpolarity );
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC3P);
            timer->TIM_CCER |= (uint32_t)((uint32_t)ocpolarity );
            break;
        /* configure TIMER_CH_4 */
        case TIMER_CH_4:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC4P);
            timer->TIM_CCER |= (uint32_t)((uint32_t)ocpolarity );
            break;
        default:
            break;
    }
}

/******************Advanced********************/
/**
  * \brief configure TIMER channel complementary output polarity
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  * \param ocnpolarity: channel complementary output polarity only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CCER_CC1NP_HIGH: channel complementary output polarity is high
  *     \arg TIMER_OCN_POLARITY_LOW: channel complementary output polarity is low
  */
void Timer_ChannelComplementaryOutputPolarityConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ocnpolarity)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC1NP);
            timer->TIM_CCER |= (uint32_t)ocnpolarity;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC2NP);
            timer->TIM_CCER |= (uint32_t)((uint32_t)ocnpolarity );
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC3NP);
            timer->TIM_CCER |= (uint32_t)((uint32_t)ocnpolarity );
            break;
        default:
            break;
    }
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER channel enable state
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  *     \arg TIMER_CH_4: TIMER channel 4(TIMERx(x=0..4))
  * \param state: TIMER channel enable state only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CCER_CC1E_ENABLE: channel enable
  *     \arg ADV_TIMER_TIM_CCER_CC1E_DISABLE: channel disable
  */
void Timer_ChannelOutputStateConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t state)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC1E);
            timer->TIM_CCER |= (uint32_t)state;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC2E);
            timer->TIM_CCER |= (uint32_t)((uint32_t)state );
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC3E);
            timer->TIM_CCER |= (uint32_t)((uint32_t)state );
            break;
        /* configure TIMER_CH_4 */
        case TIMER_CH_4:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC4E);
            timer->TIM_CCER |= (uint32_t)((uint32_t)state);
            break;
        default:
            break;
    }
}

/******************Advanced********************/
/**
  * \brief configure TIMER channel complementary output enable state
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1
  *     \arg TIMER_CH_2: TIMER channel 2
  *     \arg TIMER_CH_3: TIMER channel 3
  * \param ocnstate: TIMER channel complementary output enable state only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CCER_CC1NE_ENABLE: channel complementary enable
  *     \arg ADV_TIMER_TIM_CCER_CC1NE_DISABLE: channel complementary disable
  */
void Timer_ChannelComplementaryOutputStateConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ocnstate)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC1NE);
            timer->TIM_CCER |= (uint32_t)ocnstate;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC2NE);
            timer->TIM_CCER |= (uint32_t)((uint32_t)ocnstate);
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC3NE);
            timer->TIM_CCER |= (uint32_t)((uint32_t)ocnstate );
            break;
        default:
            break;
    }
}

/******************General&Advanced********************/
/**
  * \brief initialize TIMER channel input parameter struct with a default value
  *
  * \param icpara: TIMER channel intput parameter struct
 */
void Timer_ChannelInputStructParaInit(TIMER_Input_TypeDef* icpara)
{
    /* initialize the channel input parameter struct member with the default value */
    icpara->icpolarity  = ADV_TIMER_TIM_CCER_CC2P_DISABLE|ADV_TIMER_TIM_CCER_CC2NP_HIGH;
    icpara->icselection = ADV_TIMER_TIM_CCMR1_CC2S_TI1;
    icpara->icprescaler = ADV_TIMER_TIM_CCMR1_IC2PSC_NO_PRESCALER;
    icpara->icfilter    = ADV_TIMER_TIM_CCMR1_IC2F_NO_FILTER;
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER input capture parameter
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  *     \arg TIMER_CH_4: TIMER channel 4(TIMERx(x=0..4))
  * \param icpara: TIMER channel intput parameter struct
  *     \arg icpara->icpolarity: ADV_TIMER_TIM_CCER_CC1P_DISABLE, ADV_TIMER_TIM_CCER_CC1P_ENABLE,
  *         TIMER_IC_POLARITY_BOTH_EDGE(only for TIMER1~TIMER8)
  *     \arg icpara->icselection: ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT, ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT,
  *         TIMER_IC_SELECTION_ITS
  *     \arg icpara->icprescaler: TIMER_IC_PSC_DIV1, TIMER_IC_PSC_DIV2, TIMER_IC_PSC_DIV4,
  *         TIMER_IC_PSC_DIV8
  *     \arg icpara->icfilter: 0~15
  */
void Timer_InputCaptureConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, TIMER_Input_TypeDef* icpara)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            /* reset the CH0EN bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC1E);
            /* reset the CH0P and CH0NP bits */
            timer->TIM_CCER &= (~(uint32_t)(ADV_TIMER_TIM_CCER_CC1P | ADV_TIMER_TIM_CCER_CC1NP));
            timer->TIM_CCER |= (uint32_t)(icpara->icpolarity);
            /* reset the CH0MS bit */
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_CC1S_MASK);
            timer->TIM_CCMR1 |= (uint32_t)(icpara->icselection);
            /* reset the CH0CAPFLT bit */
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_IC1F_MASK);
            timer->TIM_CCMR1 |= (uint32_t)(icpara->icfilter);
            /* set the CH0EN bit */
            timer->TIM_CCER |= (uint32_t)ADV_TIMER_TIM_CCER_CC1E;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            /* reset the CH1EN bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC2E);
            /* reset the CH1P and CH1NP bits */
            timer->TIM_CCER &= (~(uint32_t)(ADV_TIMER_TIM_CCER_CC2P | ADV_TIMER_TIM_CCER_CC2NP));
            timer->TIM_CCER |= (uint32_t)(icpara->icpolarity);
            /* reset the CH1MS bit */
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_CC2S_MASK);
            timer->TIM_CCMR1 |= (uint32_t)(icpara->icselection);
            /* reset the CH1CAPFLT bit */
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_IC2F_MASK);
            timer->TIM_CCMR1 |= (uint32_t)(icpara->icfilter);
            /* set the CH1EN bit */
            timer->TIM_CCER |= (uint32_t)ADV_TIMER_TIM_CCER_CC2E;
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            /* reset the CH2EN bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC3E);
            /* reset the CH2P and CH2NP bits */
            timer->TIM_CCER &= (~(uint32_t)(ADV_TIMER_TIM_CCER_CC3P | ADV_TIMER_TIM_CCER_CC3NP));
            timer->TIM_CCER |= (uint32_t)(icpara->icpolarity);
            /* reset the CH2MS bit */
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_CC3S_MASK);
            timer->TIM_CCMR2 |= (uint32_t)((uint32_t)(icpara->icselection));
            /* reset the CH2CAPFLT bit */
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_IC3F_MASK);
            timer->TIM_CCMR2 |= (uint32_t)(icpara->icfilter);
            /* set the CH2EN bit */
            timer->TIM_CCER |= (uint32_t)ADV_TIMER_TIM_CCER_CC3E;
            break;
        /* configure TIMER_CH_4 */
        case TIMER_CH_4:
            /* reset the CH3EN bit */
            timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC4E);
            /* reset the CH3P bits */
            timer->TIM_CCER &= (~(uint32_t)(ADV_TIMER_TIM_CCER_CC4P));
            timer->TIM_CCER |= (uint32_t)(icpara->icpolarity);
            /* reset the CH3MS bit */
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_CC4S_MASK);
            timer->TIM_CCMR2 |= (uint32_t)(icpara->icselection);
            /* reset the CH3CAPFLT bit */
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_IC4F_MASK);
            timer->TIM_CCMR2 |= (uint32_t)(icpara->icfilter) ;
            /* set the CH3EN bit */
            timer->TIM_CCER |= (uint32_t)ADV_TIMER_TIM_CCER_CC4E;
            break;
        default:
            break;
    }
    /* configure TIMER channel input capture prescaler value */
    Timer_ChannelInputCapturePrescalerConfig(timer, channel, (uint32_t)(icpara->icprescaler));
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER channel input capture prescaler value
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  *     \arg TIMER_CH_4: TIMER channel 4(TIMERx(x=0..4))
  * \param prescaler: channel input capture prescaler value
  *     only one parameter can be selected which is shown as below:
  *     \arg TIMER_IC_PSC_DIV1: no prescaler
  *     \arg TIMER_IC_PSC_DIV2: divided by 2
  *     \arg TIMER_IC_PSC_DIV4: divided by 4
  *     \arg TIMER_IC_PSC_DIV8: divided by 8
  */
void Timer_ChannelInputCapturePrescalerConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t prescaler)
{
    switch (channel) {
        /* configure TIMER_CH_1 */
        case TIMER_CH_1:
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_IC1PSC_MASK);
            timer->TIM_CCMR1 |= (uint32_t)prescaler;
            break;
        /* configure TIMER_CH_2 */
        case TIMER_CH_2:
            timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_IC2PSC_MASK);
            timer->TIM_CCMR1 |= ((uint32_t)prescaler);
            break;
        /* configure TIMER_CH_3 */
        case TIMER_CH_3:
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_IC3PSC_MASK);
            timer->TIM_CCMR2 |= (uint32_t)prescaler;
            break;
        /* configure TIMER_CH_4 */
        case TIMER_CH_4:
            timer->TIM_CCMR2 &= (~(uint32_t)ADV_TIMER_TIM_CCMR2_IC4PSC_MASK);
            timer->TIM_CCMR2 |= ((uint32_t)prescaler);
            break;
        default:
            break;
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief read TIMER channel capture compare register value
  *
  * \param timer: please refer to the following parameters
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
  *     \arg TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
  *     \arg TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
  *     \arg TIMER_CH_4: TIMER channel 4(TIMERx(x=0..4))
  *
  * \retval channel capture compare register value
  */
uint32_t Timer_ChannelCaptureValueRegisterRead(ADVANCED_TIMER_TypeDef* timer, uint32_t channel)
{
    uint32_t count_value = 0U;

    switch (channel) {
        case TIMER_CH_1:
            /* read TIMER channel 1 capture compare register value */
            count_value = timer->TIM_CCR1;
            break;
        case TIMER_CH_2:
            /* read TIMER channel 2 capture compare register value */
            count_value = timer->TIM_CCR2;
            break;
        case TIMER_CH_3:
            /* read TIMER channel 3 capture compare register value */
            count_value = timer->TIM_CCR3;
            break;
        case TIMER_CH_4:
            /* read TIMER channel 4 capture compare register value */
            count_value = timer->TIM_CCR4;
            break;
        default:
            break;
        }
    return (count_value);
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER input pwm capture function
  *
  * \param timer: TIMERx(x=0..4)
  * \param channel: only one parameter can be selected which is shown as below:
  *     \arg TIMER_CH_1: TIMER channel 1
  *     \arg TIMER_CH_2: TIMER channel 2
  * \param icpwm: TIMER channel intput pwm parameter struct
  *     \arg icpwm->icpolarity: ADV_TIMER_TIM_CCER_CC1P_DISABLE, ADV_TIMER_TIM_CCER_CC1P_ENABLE
  *     \arg icpwm->icselection: ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT, ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT
  *     \arg icpwm->icprescaler: TIMER_IC_PSC_DIV1, TIMER_IC_PSC_DIV2, TIMER_IC_PSC_DIV4, TIMER_IC_PSC_DIV8
  *     \arg icpwm->icfilter: 0~15
  */
void Timer_InputPwmCaptureConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, TIMER_Input_TypeDef* icpwm)
{
    uint32_t icpolarity  = 0x0U;
    uint32_t icselection = 0x0U;

    /* Set channel input polarity */
    if (ADV_TIMER_TIM_CCER_CC1P_DISABLE == icpwm->icpolarity) {
        icpolarity = ADV_TIMER_TIM_CCER_CC1P_ENABLE;
    } else {
        icpolarity = ADV_TIMER_TIM_CCER_CC1P_DISABLE;
    }
    /* Set channel input mode selection */
    if (ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT == icpwm->icselection) {
        icselection = ADV_TIMER_TIM_CCMR1_CC1S_TI1;
    } else {
        icselection = ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT;
    }
    if (TIMER_CH_1 == channel) {
        /* reset the CH0EN bit */
        timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC1E);
        /* reset the CH0P and CH0NP bits */
        timer->TIM_CCER &= (~(uint32_t)(ADV_TIMER_TIM_CCER_CC1P | ADV_TIMER_TIM_CCER_CC1NP));
        /* set the CH0P and CH0NP bits */
        timer->TIM_CCER |= (uint32_t)(icpwm->icpolarity);
        /* reset the CH0MS bit */
        timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_CC1S_MASK);
        /* set the CH0MS bit */
        timer->TIM_CCMR1 |= (uint32_t)(icpwm->icselection);
        /* reset the CH0CAPFLT bit */
        timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_IC1F_MASK);
        /* set the CH0CAPFLT bit */
        timer->TIM_CCMR1 |= ((uint32_t)(icpwm->icfilter));
        /* set the CH0EN bit */
        timer->TIM_CCER |= (uint32_t)ADV_TIMER_TIM_CCER_CC1E;
        /* configure TIMER channel input capture prescaler value */
        Timer_ChannelInputCapturePrescalerConfig(timer, TIMER_CH_1, (uint32_t)(icpwm->icprescaler));
        /* reset the CH1EN bit */
        timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC2E);
        /* reset the CH1P and CH1NP bits */
        timer->TIM_CCER &= (~(uint32_t)(ADV_TIMER_TIM_CCER_CC2P | ADV_TIMER_TIM_CCER_CC2NP));
        /* set the CH1P and CH1NP bits */
        timer->TIM_CCER |= (uint32_t)((uint32_t)icpolarity);
        /* reset the CH1MS bit */
        timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_CC2S_MASK);
        /* set the CH1MS bit */
        timer->TIM_CCMR1 |= (uint32_t)((uint32_t)icselection);
        /* reset the CH1CAPFLT bit */
        timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_IC2F_MASK);
        /* set the CH1CAPFLT bit */
        timer->TIM_CCMR1 |= (uint32_t)((uint32_t)(icpwm->icfilter) );
        /* set the CH1EN bit */
        timer->TIM_CCER |= (uint32_t)ADV_TIMER_TIM_CCER_CC2E;
        /* configure TIMER channel input capture prescaler value */
        Timer_ChannelInputCapturePrescalerConfig(timer, TIMER_CH_2, (uint32_t)(icpwm->icprescaler));
    } else {
        /* reset the CH1EN bit */
        timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC2E);
        /* reset the CH1P and CH1NP bits */
        timer->TIM_CCER &= (~(uint32_t)(ADV_TIMER_TIM_CCER_CC2P | ADV_TIMER_TIM_CCER_CC2NP));
        /* set the CH1P and CH1NP bits */
        timer->TIM_CCER |= (uint32_t)((uint32_t)(icpwm->icpolarity));
        /* reset the CH1MS bit */
        timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_CC2S_MASK);
        /* set the CH1MS bit */
        timer->TIM_CCMR1 |= (uint32_t)((uint32_t)(icpwm->icselection) );
        /* reset the CH1CAPFLT bit */
        timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_IC2F_MASK);
        /* set the CH1CAPFLT bit */
        timer->TIM_CCMR1 |= (uint32_t)((uint32_t)(icpwm->icfilter) );
        /* set the CH1EN bit */
        timer->TIM_CCER |= (uint32_t)ADV_TIMER_TIM_CCER_CC2E;
        /* configure TIMER channel input capture prescaler value */
        Timer_ChannelInputCapturePrescalerConfig(timer, TIMER_CH_2, (uint32_t)(icpwm->icprescaler));
        /* reset the CH0EN bit */
        timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC1E);
        /* reset the CH0P and CH0NP bits */
        timer->TIM_CCER &= (~(uint32_t)(ADV_TIMER_TIM_CCER_CC1P | ADV_TIMER_TIM_CCER_CC1NP));
        /* set the CH0P and CH0NP bits */
        timer->TIM_CCER |= (uint32_t)icpolarity;
        /* reset the CH0MS bit */
        timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_CC1S_MASK);
        /* set the CH0MS bit */
        timer->TIM_CCMR1 |= (uint32_t)icselection;
        /* reset the CH0CAPFLT bit */
        timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_IC1F_MASK);
        /* set the CH0CAPFLT bit */
        timer->TIM_CCMR1 |= ((uint32_t)(icpwm->icfilter));
        /* set the CH0EN bit */
        timer->TIM_CCER |= (uint32_t)ADV_TIMER_TIM_CCER_CC1E;
        /* configure TIMER channel input capture prescaler value */
        Timer_ChannelInputCapturePrescalerConfig(timer, TIMER_CH_1, (uint32_t)(icpwm->icprescaler));
    }
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER hall sensor mode
  *
  * \param timer: TIMERx(x=0..4)
  * \param hallmode: only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CR2_TI1S: TIMER hall sensor mode enable
  *     \arg ADV_TIMER_TIM_CR2_TI1S_CH1_TI1: TIMER hall sensor mode disable
  */
void Timer_HallModeConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t hallmode)
{
    if (ADV_TIMER_TIM_CR2_TI1S == hallmode) {
        timer->TIM_CR2 |= (uint32_t)ADV_TIMER_TIM_CR2_TI1S_XOR;
    } else if (ADV_TIMER_TIM_CR2_TI1S_CH1_TI1 == hallmode) {
        timer->TIM_CR2 &= ~(uint32_t)ADV_TIMER_TIM_CR2_TI1S_XOR;
    } else {
        /* illegal parameters */
    }
}

/******************General&Advanced********************/
/**
  * \brief select TIMER input trigger source
  *
  * \param timer: TIMERx(x=0..4)
  * \param intrigger: only one parameter can be selected which is shown as below:
  *     \arg TIMER_SMCR_TRGSEL_ITI1: internal trigger 0(TIMERx(x=0..4))
  *     \arg TIMER_SMCR_TRGSEL_ITI2: internal trigger 1(TIMERx(x=0..4))
  *     \arg TIMER_SMCR_TRGSEL_ITI3: internal trigger 2(TIMERx(x=0..4))
  *     \arg TIMER_SMCR_TRGSEL_ITI4: internal trigger 3(TIMERx(x=0..4))
  *     \arg TIMER_SMCR_TRGSEL_CI0F_ED: TI0 edge detector(TIMERx(x=0..4))
  *     \arg ADV_TIMER_TIM_SMCR_TS_TI1FP1: filtered TIMER input 0(TIMERx(x=0..4))
  *     \arg ADV_TIMER_TIM_SMCR_SMS_TRIGGER: filtered TIMER input 1(TIMERx(x=0..4))
  *     \arg ADV_TIMER_TIM_SMCR_TS_ETRF: filtered external trigger input(TIMERx(x=0..4))
  */
void Timer_InputTriggerSourceSelect(ADVANCED_TIMER_TypeDef* timer, uint32_t intrigger)
{
    timer->TIM_SMCR &= (~(uint32_t)ADV_TIMER_TIM_SMCR_TS_MASK);
    timer->TIM_SMCR |= (uint32_t)intrigger;
}

/******************General&Advanced********************/
/**
  * \brief select TIMER master mode output trigger source
  *
  * \param timer: TIMERx(x=0..4)
  * \param outrigger: only one parameter can be selected which is shown as below:
  *     \arg TIMER_TRI_OUT_SRC_RESET: the UPG bit as trigger output(TIMERx(x=0..4))
  *     \arg TIMER_TRI_OUT_SRC_ENABLE: the counter enable signal ADV_TIMER_TIM_CR1_CEN as trigger output(TIMERx(x=0..4))
  *     \arg TIMER_TRI_OUT_SRC_UPDATE: update event as trigger output(TIMERx(x=0..4))
  *     \arg TIMER_TRI_OUT_SRC_CH0: a capture or a compare match occurred in channel 0 as trigger output TRGO(TIMERx(x=0..4))
  *     \arg TIMER_TRI_OUT_SRC_O1CPRE: O0CPRE as trigger output(TIMERx(x=0..4))
  *     \arg TIMER_TRI_OUT_SRC_O2CPRE: O1CPRE as trigger output(TIMERx(x=0..4))
  *     \arg TIMER_TRI_OUT_SRC_O3CPRE: O2CPRE as trigger output(TIMERx(x=0..4))
  *     \arg TIMER_TRI_OUT_SRC_O4CPRE: O3CPRE as trigger output(TIMERx(x=0..4))
  */
void Timer_MasterOutputTriggerSourceSelect(ADVANCED_TIMER_TypeDef* timer, uint32_t outrigger)
{
    timer->TIM_CR2 &= (~(uint32_t)ADV_TIMER_TIM_CR2_MMS_MASK);
    timer->TIM_CR2 |= (uint32_t)outrigger;
}

/******************General&Advanced********************/
/**
  * \brief select TIMER slave mode
  *
  * \param timer: TIMERx(x=0..4)
  * \param slavemode: only one parameter can be selected which is shown as below:
  *     \arg TIMER_SLAVE_MODE_DISABLE: slave mode disable
  *     \arg TIMER_ENCODER_MODE0: encoder mode 0
  *     \arg TIMER_ENCODER_MODE1: encoder mode 1
  *     \arg TIMER_ENCODER_MODE2: encoder mode 2
  *     \arg TIMER_SLAVE_MODE_RESTART: restart mode
  *     \arg TIMER_SLAVE_MODE_PAUSE: pause mode
  *     \arg ADV_TIMER_TIM_SMCR_SMS_TRIGGER: event mode
  *     \arg ADV_TIMER_TIM_SMCR_SMS_EXTERNAL_CLOCK: external clock mode 0
  */
void Timer_SlaveModeSelect(ADVANCED_TIMER_TypeDef* timer, uint32_t slavemode)
{
    timer->TIM_SMCR &= (~(uint32_t)ADV_TIMER_TIM_SMCR_SMS_MASK);
    timer->TIM_SMCR |= (uint32_t)slavemode;
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER master slave mode
  *
  * \param timer: TIMERx(x=0..4)
  * \param masterslave: only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_SMCR_MSM_ENABLE: master slave mode enable
  *     \arg ADV_TIMER_TIM_SMCR_MSM_DISABLE: master slave mode disable
  */
void Timer_MasterSlaveModeConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t masterslave)
{
    if (ADV_TIMER_TIM_SMCR_MSM_ENABLE == masterslave) {
        timer->TIM_SMCR |= (uint32_t)ADV_TIMER_TIM_SMCR_MSM;
    } else if (ADV_TIMER_TIM_SMCR_MSM_DISABLE == masterslave) {
        timer->TIM_SMCR &= ~(uint32_t)ADV_TIMER_TIM_SMCR_MSM;
    } else {
        /* illegal parameters */
    }
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER external trigger input
  *
  * \param timer: TIMERx(x=0..4)
  * \param extprescaler: only one parameter can be selected which is shown as below:
  *     \arg TIMER_EXT_TRI_PSC_OFF: no divided
  *     \arg TIMER_EXT_TRI_PSC_DIV2: divided by 2
  *     \arg TIMER_EXT_TRI_PSC_DIV4: divided by 4
  *     \arg TIMER_EXT_TRI_PSC_DIV8: divided by 8
  * \param extpolarity: only one parameter can be selected which is shown as below:
  *     \arg TIMER_ETP_FALLING: active low or falling edge active
  *     \arg TIMER_ETP_RISING: active high or rising edge active
  * \param extfilter: a value between 0 and 15
  */
void Timer_ExternalTriggerConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t extprescaler, uint32_t extpolarity, uint32_t extfilter)
{
    timer->TIM_SMCR &= (~(uint32_t)(ADV_TIMER_TIM_SMCR_ETP | ADV_TIMER_TIM_SMCR_ETPS_MASK | ADV_TIMER_TIM_SMCR_ETF_MASK));
    timer->TIM_SMCR |= (uint32_t)(extprescaler | extpolarity);
    timer->TIM_SMCR |= (uint32_t)(extfilter );
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER quadrature decoder mode
  *
  * \param timer: TIMERx(x=0..4)
  * \param decomode: only one parameter can be selected which is shown as below:
  *     \arg TIMER_ENCODER_MODE0: counter counts on CI0FE0 edge depending on CI1FE1 level
  *     \arg TIMER_ENCODER_MODE1: counter counts on CI1FE1 edge depending on CI0FE0 level
  *     \arg TIMER_ENCODER_MODE2: counter counts on both CI0FE0 and CI1FE1 edges depending on the level of the other input
  * \param ic0polarity: only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CCER_CC1P_DISABLE: capture rising edge
  *     \arg ADV_TIMER_TIM_CCER_CC1P_ENABLE: capture falling edge
  * \param ic1polarity: only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CCER_CC1P_DISABLE: capture rising edge
  *     \arg ADV_TIMER_TIM_CCER_CC1P_ENABLE: capture falling edge
  */
void Timer_QuadratureDecoderModeConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t decomode, uint32_t ic0polarity, uint32_t ic1polarity)
{
    /* configure the quadrature decoder mode */
    timer->TIM_SMCR &= (~(uint32_t)ADV_TIMER_TIM_SMCR_SMS_MASK);
    timer->TIM_SMCR |= (uint32_t)decomode;
    /* configure input capture selection */
    timer->TIM_CCMR1 &= (uint32_t)(((~(uint32_t)ADV_TIMER_TIM_CCMR1_CC1S_MASK)) & ((~(uint32_t)ADV_TIMER_TIM_CCMR1_CC2S_MASK)));
    timer->TIM_CCMR1 |= (uint32_t)(ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT | ((uint32_t)ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT ));
    /* configure channel input capture polarity */
    timer->TIM_CCER &= (~(uint32_t)(ADV_TIMER_TIM_CCER_CC1P | ADV_TIMER_TIM_CCER_CC1NP));
    timer->TIM_CCER &= (~(uint32_t)(ADV_TIMER_TIM_CCER_CC2P | ADV_TIMER_TIM_CCER_CC2NP));
    timer->TIM_CCER |= ((uint32_t)ic0polarity | ((uint32_t)ic1polarity ));
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER internal clock mode
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_InternalClockConfig(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_SMCR &= ~(uint32_t)ADV_TIMER_TIM_SMCR_SMS_MASK;
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER the internal trigger as external clock input
  *
  * \param timer: TIMERx(x=0..4)
  * \param intrigger: only one parameter can be selected which is shown as below:
  *     \arg TIMER_SMCR_TRGSEL_ITI1: internal trigger 0
  *     \arg TIMER_SMCR_TRGSEL_ITI2: internal trigger 1
  *     \arg TIMER_SMCR_TRGSEL_ITI3: internal trigger 2
  *     \arg TIMER_SMCR_TRGSEL_ITI4: internal trigger 3
  */
void Timer_InternalTriggerAsExternalClockConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t intrigger)
{
    Timer_InputTriggerSourceSelect(timer, intrigger);
    timer->TIM_SMCR &= ~(uint32_t)ADV_TIMER_TIM_SMCR_SMS_MASK;
    timer->TIM_SMCR |= (uint32_t)ADV_TIMER_TIM_SMCR_SMS_EXTERNAL_CLOCK;
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER the external trigger as external clock input
  *
  * \param timer: TIMERx(x=0..4)
  * \param extrigger: only one parameter can be selected which is shown as below:
  *     \arg TIMER_SMCR_TRGSEL_CI0F_ED: TI0 edge detector
  *     \arg ADV_TIMER_TIM_SMCR_TS_TI1FP1: filtered TIMER input 0
  *     \arg ADV_TIMER_TIM_SMCR_SMS_TRIGGER: filtered TIMER input 1
  * \param extpolarity: only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CCER_CC1P_DISABLE: active low or falling edge active
  *     \arg ADV_TIMER_TIM_CCER_CC1P_ENABLE: active high or rising edge active
  * \param extfilter: a value between 0 and 15
  */
void Timer_ExternalTriggerAsExternalClockConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t extrigger, uint32_t extpolarity, uint32_t extfilter)
{
    if (ADV_TIMER_TIM_SMCR_SMS_TRIGGER == extrigger) {
        /* reset the CH1EN bit */
        timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC2E);
        /* reset the CH1NP bit */
        timer->TIM_CCER &= (~(uint32_t)(ADV_TIMER_TIM_CCER_CC2P | ADV_TIMER_TIM_CCER_CC2NP));
        /* set the CH1NP bit */
        timer->TIM_CCER |= (uint32_t)((uint32_t)extpolarity);
        /* reset the CH1MS bit */
        timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_CC2S_MASK);
        /* set the CH1MS bit */
        timer->TIM_CCMR1 |= (uint32_t)((uint32_t)ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT);
        /* reset the CH1CAPFLT bit */
        timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_IC2F_MASK);
        /* set the CH1CAPFLT bit */
        timer->TIM_CCMR1 |= (uint32_t)(extfilter );
        /* set the CH1EN bit */
        timer->TIM_CCER |= (uint32_t)ADV_TIMER_TIM_CCER_CC2E;
    } else {
        /* reset the CH0EN bit */
        timer->TIM_CCER &= (~(uint32_t)ADV_TIMER_TIM_CCER_CC1E);
        /* reset the CH0P and CH0NP bits */
        timer->TIM_CCER &= (~(uint32_t)(ADV_TIMER_TIM_CCER_CC1P | ADV_TIMER_TIM_CCER_CC1NP));
        /* set the CH0P and CH0NP bits */
        timer->TIM_CCER |= (uint32_t)extpolarity;
        /* reset the CH0MS bit */
        timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_CC1S_MASK);
        /* set the CH0MS bit */
        timer->TIM_CCMR1 |= (uint32_t)ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT;
        /* reset the CH0CAPFLT bit */
        timer->TIM_CCMR1 &= (~(uint32_t)ADV_TIMER_TIM_CCMR1_IC1F_MASK);
        /* reset the CH0CAPFLT bit */
        timer->TIM_CCMR1 |= (uint32_t)(extfilter);
        /* set the CH0EN bit */
        timer->TIM_CCER |= (uint32_t)ADV_TIMER_TIM_CCER_CC1E;
    }
    /* select TIMER input trigger source */
    Timer_InputTriggerSourceSelect(timer, extrigger);
    /* reset the SMC bit */
    timer->TIM_SMCR &= (~(uint32_t)ADV_TIMER_TIM_SMCR_SMS_MASK);
    /* set the SMC bit */
    timer->TIM_SMCR |= (uint32_t)ADV_TIMER_TIM_SMCR_SMS_EXTERNAL_CLOCK;
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER the external clock mode0
  *
  * \param timer: TIMERx(x=0..4)
  * \param extprescaler: only one parameter can be selected which is shown as below:
  *     \arg TIMER_EXT_TRI_PSC_OFF: no divided
  *     \arg TIMER_EXT_TRI_PSC_DIV2: divided by 2
  *     \arg TIMER_EXT_TRI_PSC_DIV4: divided by 4
  *     \arg TIMER_EXT_TRI_PSC_DIV8: divided by 8
  * \param extpolarity: only one parameter can be selected which is shown as below:
  *     \arg TIMER_ETP_FALLING: active low or falling edge active
  *     \arg TIMER_ETP_RISING: active high or rising edge active
  * \param extfilter: a value between 0 and 15
  */
void Timer_ExternalClockMode0Config(ADVANCED_TIMER_TypeDef* timer, uint32_t extprescaler, uint32_t extpolarity, uint32_t extfilter)
{
    /* configure TIMER external trigger input */
    Timer_ExternalTriggerConfig(timer, extprescaler, extpolarity, extfilter);
    /* reset the SMC bit,TRGS bit */
    timer->TIM_SMCR &= (~(uint32_t)(ADV_TIMER_TIM_SMCR_SMS_MASK | ADV_TIMER_TIM_SMCR_TS_MASK));
    /* set the SMC bit,TRGS bit */
    timer->TIM_SMCR |= (uint32_t)(ADV_TIMER_TIM_SMCR_SMS_EXTERNAL_CLOCK | ADV_TIMER_TIM_SMCR_TS_ETRF);
}

/******************General&Advanced********************/
/**
  * \brief configure TIMER the external clock mode1
  *
  * \param timer: TIMERx(x=0..4)
  * \param extprescaler: only one parameter can be selected which is shown as below:
  *     \arg TIMER_EXT_TRI_PSC_OFF: no divided
  *     \arg TIMER_EXT_TRI_PSC_DIV2: divided by 2
  *     \arg TIMER_EXT_TRI_PSC_DIV4: divided by 4
  *     \arg TIMER_EXT_TRI_PSC_DIV8: divided by 8
  * \param extpolarity: only one parameter can be selected which is shown as below:
  *     \arg TIMER_ETP_FALLING: active low or falling edge active
  *     \arg TIMER_ETP_RISING: active high or rising edge active
  * \param extfilter: a value between 0 and 15
  */
void Timer_ExternalClockMode1Config(ADVANCED_TIMER_TypeDef* timer, uint32_t extprescaler, uint32_t extpolarity, uint32_t extfilter)
{
    /* configure TIMER external trigger input */
    Timer_ExternalTriggerConfig(timer, extprescaler, extpolarity, extfilter);
    timer->TIM_SMCR |= (uint32_t)ADV_TIMER_TIM_SMCR_ECE;
}

/******************General&Advanced********************/
/**
  * \brief disable TIMER the external clock mode1
  *
  * \param timer: TIMERx(x=0..4)
  */
void Timer_ExternalClockMode1Disable(ADVANCED_TIMER_TypeDef* timer)
{
    timer->TIM_SMCR &= ~(uint32_t)ADV_TIMER_TIM_SMCR_ECE;
}

/******************Base&General&Advanced********************/
/**
  * \brief enable the TIMER interrupt
  *
  * \param timer: please refer to the following parameters
  * \param interrupt: specify which interrupt to enable
  *     one or more parameters can be selected which are shown as below:
  *     \arg TIMER_INT_UP: update interrupt enable, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC1IE: channel 0 interrupt enable, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC2IE: channel 1 interrupt enable, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC3IE: channel 2 interrupt enable, TIMERx(x=0..4)
  *     \arg TIMER_INT_CH4: channel 3 interrupt enable, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_COMIE: commutation interrupt enable, TIMERx(x=0..4)
  *     \arg TIMER_INT_TRG: trigger interrupt enable, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC3IE_ENABLE: break interrupt enable, TIMERx(x=0..4)
  */
void Timer_InterruptEnable(ADVANCED_TIMER_TypeDef* timer, uint32_t interrupt)
{
    timer->TIM_DIER |= (uint32_t) interrupt;
}

/******************Base&General&Advanced********************/
/**
  * \brief disable the TIMER interrupt
  *
  * \param timer: TIMERx(x=0..4)
  * \param interrupt: specify which interrupt to disbale
  *     one or more parameters can be selected which are shown as below:
  *     \arg TIMER_INT_UP: update interrupt enable, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC1IE: channel 0 interrupt enable, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC2IE: channel 1 interrupt enable, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC3IE: channel 2 interrupt enable, TIMERx(x=0..4)
  *     \arg TIMER_INT_CH4: channel 3 interrupt enable , TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_COMIE: commutation interrupt enable, TIMERx(x=0..4)
  *     \arg TIMER_INT_TRG: trigger interrupt enable, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC3IE_ENABLE: break interrupt enable, TIMERx(x=0..4)
  */
void Timer_InterruptDisable(ADVANCED_TIMER_TypeDef* timer, uint32_t interrupt)
{
    timer->TIM_DIER &= (~(uint32_t)interrupt);
}

/******************Base&General&Advanced********************/
/**
  * \brief get timer interrupt flag
  *
  * \param timer: TIMERx(x=0..4)
  * \param interrupt: the timer interrupt bits
  *     only one parameter can be selected which is shown as below:
  *     \arg TIMER_INT_FLAG_UP: update interrupt flag, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC1IE_ENABLE: channel 0 interrupt flag, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC2IE_ENABLE: channel 1 interrupt flag, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC3IE_ENABLE: channel 2 interrupt flag, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC4IE_ENABLE: channel 3 interrupt flag, TIMERx(x=0..4)
  *     \arg TIMER_INT_FLAG_CMT: channel commutation interrupt flag, TIMERx(x=0..4)
  *     \arg TIMER_INT_FLAG_TRG: trigger interrupt flag, TIMERx(x=0..4)
  *     \arg TIMER_INT_FLAG_BRK: break interrupt flag, TIMERx(x=0..4)
  *
  * \retval FlagStatus: SET or RESET
  */
FlagStatus Timer_InterruptFlagGet(ADVANCED_TIMER_TypeDef* timer, uint32_t interrupt)
{
    uint32_t val;
    val = (timer->TIM_DIER & interrupt);
    if ((RESET != (timer->TIM_SR & interrupt)) && (RESET != val)) {
        return SET;
    } else {
        return RESET;
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief clear TIMER interrupt flag
  *
  * \param timer: TIMERx(x=0..4)
  * \param interrupt: the timer interrupt bits
  *     one or more parameters can be selected which are shown as below:
  *     \arg TIMER_INT_FLAG_UP: update interrupt flag, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC1IE_ENABLE: channel 0 interrupt flag, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC2IE_ENABLE: channel 1 interrupt flag, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC3IE_ENABLE: channel 2 interrupt flag, TIMERx(x=0..4)
  *     \arg ADV_TIMER_TIM_DIER_CC4IE_ENABLE: channel 3 interrupt flag, TIMERx(x=0..4)
  *     \arg TIMER_INT_FLAG_CMT: channel commutation interrupt flag, TIMERx(x=0..4)
  *     \arg TIMER_INT_FLAG_TRG: trigger interrupt flag, TIMERx(x=0..4)
  *     \arg TIMER_INT_FLAG_BRK: break interrupt flag, TIMERx(x=0..4)
  */
void Timer_InterruptFlagClear(ADVANCED_TIMER_TypeDef* timer, uint32_t interrupt)
{
    timer->TIM_SR &= (~(uint32_t)interrupt);
}

/******************Base&General&Advanced********************/
/**
  * \brief get TIMER flags
  *
  * \param timer: TIMERx(x=0..4)
  * \param flag: the timer interrupt flags
  *     only one parameter can be selected which is shown as below:
  *     \arg TIMER_FLAG_UP: update flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH1: channel 0 flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH2: channel 1 flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH3: channel 2 flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH4: channel 3 flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CMT: channel commutation flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_TRG: trigger flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_BRK: break flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH1O: channel 0 overcapture flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH2O: channel 1 overcapture flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH3O: channel 2 overcapture flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH4O: channel 3 overcapture flag, TIMERx(x=0..4)
  *
  * \retval     FlagStatus: SET or RESET
  */
FlagStatus Timer_FlagGet(ADVANCED_TIMER_TypeDef* timer, uint32_t flag)
{
    if (RESET != (timer->TIM_SR & flag)) {
        return SET;
    } else {
        return RESET;
    }
}

/******************Base&General&Advanced********************/
/**
  * \brief clear TIMER flags
  *
  * \param timer: TIMERx(x=0..4)
  * \param flag: the timer interrupt flags
  *     one or more parameters can be selected which are shown as below:
  *     \arg TIMER_FLAG_UP: update flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH1: channel 0 flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH2: channel 1 flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH3: channel 2 flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH4: channel 3 flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CMT: channel commutation flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_TRG: trigger flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_BRK: break flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH1O: channel 0 overcapture flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH2O: channel 1 overcapture flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH3O: channel 2 overcapture flag, TIMERx(x=0..4)
  *     \arg TIMER_FLAG_CH4O: channel 3 overcapture flag, TIMERx(x=0..4)
  */
void Timer_FlagClear(ADVANCED_TIMER_TypeDef* timer, uint32_t flag)
{
    timer->TIM_SR = (~(uint32_t)flag);
}

/**
  * \brief set division ratio between TIMER clock and dead-time and sampling clock
  *
  * \param timer: TIMERx(x=0..4)
  * \param div: only one parameter can be selected which is shown as below:
  *     \arg ADV_TIMER_TIM_CR1_CKD_CKD1: no divided
  *     \arg ADV_TIMER_TIM_CR1_CKD_CKD2: divided by 2
  *     \arg ADV_TIMER_TIM_CR1_CKD_CKD4: divided by 4
  */
void Timer_CKDIV(ADVANCED_TIMER_TypeDef* timer, uint32_t div)
{
    timer->TIM_CR1 &= ADV_TIMER_TIM_CR1_CKD_MASK;
    timer->TIM_CR1 |= div;
}

void Timer_BKAF1(ADVANCED_TIMER_TypeDef* timer,ControlStatus state)
{
 if (ENABLE == state) {
        timer->TIM_AF1 |= (uint32_t)ADV_TIMER_TIM_AF1_BKINE;
    } else {
        timer->TIM_AF1 &= (~(uint32_t)ADV_TIMER_TIM_AF1_BKINE);
    }
}

void Timer_BK2AF2(ADVANCED_TIMER_TypeDef* timer,ControlStatus state)
{
 if (ENABLE == state) {
        timer->TIM_AF2 |= (uint32_t)ADV_TIMER_TIM_AF2_BK2INE;
    } else {
        timer->TIM_AF2 &= (~(uint32_t)ADV_TIMER_TIM_AF2_BK2INE);
    }
}
void TIM_PWM_Start(ADVANCED_TIMER_TypeDef *timer, uint32_t Channel,uint32_t state)
{
    Timer_ChannelOutputStateConfig(timer, Channel , state); 

    Timer_PrimaryOutputConfig(timer, ENABLE); 

    /* Enable the Peripheral */
    Timer_Enable(timer);
  
} 

void TIMEx_PWMN_Start(ADVANCED_TIMER_TypeDef *timer, uint32_t Channel,uint32_t state)
{
  
    /* Enable the complementary PWM output  */
    Timer_ChannelComplementaryOutputStateConfig(timer, Channel , state);

    /* Enable the Main Output */
    Timer_PrimaryOutputConfig(timer, ENABLE); 

    /* Enable the Peripheral */
    Timer_Enable(timer);
} 

void TIM_PWM_Stop(ADVANCED_TIMER_TypeDef* timer, uint32_t Channel,uint32_t state)
{ 

    /* Disable the Capture compare channel */
    Timer_ChannelOutputStateConfig(timer, Channel , state);

    Timer_PrimaryOutputConfig(timer, DISABLE); 

    /* Disable the Peripheral */
    Timer_Disable(timer);
} 

void TIMEx_PWMN_Stop(ADVANCED_TIMER_TypeDef* timer, uint32_t Channel ,uint32_t state)
{ 
    /* Disable the Capture compare channel */
    Timer_ChannelComplementaryOutputStateConfig(timer, Channel , state);

    /* Disable the Main Output */
    Timer_PrimaryOutputConfig(timer, DISABLE); 

    /* Disable the Peripheral */
    Timer_Disable(timer);
} 

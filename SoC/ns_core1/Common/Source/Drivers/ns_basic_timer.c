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
  * \file ns_basic_timer.c
  * \brief TIMER feature API source file for Nuclei SDK
  */

#include "ns_basic_timer.h"

/**
  * \brief initialize TIMER init parameter struct with a default value
  *
  * \param initpara: init parameter struct
  */
void Basic_Timer_StructParaInit(Basic_TIMER_Init_TypeDef* initpara)
{
    /* initialize the init parameter struct member with the default value */
    initpara->prescaler         = 0U;
    initpara->period            = 65535U;
}

/**
  * \brief initialize TIMER counter
  *
  * \param timer: TIMERx(x=0..4)
  * \param initpara: init parameter struct
  *     \arg initpara->prescaler: prescaler value of the counter clock, 0~65535
  *     \arg initpara->period: counter auto reload value, 0~65535
  */
void Basic_Timer_Init(BASIC_TIMER_TypeDef* timer, Basic_TIMER_Init_TypeDef* initpara)
{
    /* configure the counter prescaler value */
    timer->PSC = (uint32_t)initpara->prescaler;
    /* configure the autoreload value */
    timer->ARR = (uint32_t)initpara->period;
    /* generate an update event */
    timer->EGR |= (uint32_t)BASIC_TIMER_EGR_UG;
}

/**
  * \brief enable a timer
  *
  * \param timer: TIMERx(x=0..4)
  */
void Basic_Timer_Enable(BASIC_TIMER_TypeDef* timer)
{
    timer->CR1 |= (uint32_t)BASIC_TIMER_CR1_CEN;
}

/**
  * \brief disable a timer
  *
  * \param timer: TIMERx(x=0..4)
  */
void Basic_Timer_Disable(BASIC_TIMER_TypeDef* timer)
{
    timer->CR1 &= ~(uint32_t)BASIC_TIMER_CR1_CEN;
}

/**
  * \brief enable the auto reload shadow function
  *
  * \param timer: TIMERx(x=0..4)
  */
void Basic_Timer_AutoReloadShadowEnable(BASIC_TIMER_TypeDef* timer)
{
    timer->CR1 |= (uint32_t)BASIC_TIMER_CR1_AUTO_BUFFER_EN;
}

/**
  * \brief disable the auto reload shadow function
  *
  * \param timer: TIMERx(x=0..4)
  */
void Basic_Timer_AutoReloadShadowDisable(BASIC_TIMER_TypeDef* timer)
{
    timer->CR1 &= ~(uint32_t)BASIC_TIMER_CR1_AUTO_BUFFER_EN;
}

/**
  * \brief  enable the update event
  *
  * \param timer: TIMERx(x=0..4)
  */
void Basic_Timer_UpdateEventEnable(BASIC_TIMER_TypeDef* timer)
{
    timer->CR1 &= ~(uint32_t)BASIC_TIMER_CR1_UPD_EVT_DIS;
}

/**
  * \brief disable the update event
  *
  * \param timer: TIMERx(x=0..4)
  */
void Basic_Timer_UpdateEventDisable(BASIC_TIMER_TypeDef* timer)
{
    timer->CR1 |= (uint32_t) BASIC_TIMER_CR1_UPD_EVT_DIS;
}

/**
  * \brief configure TIMER prescaler
  *
  * \param timer: TIMERx(x=0..4)
  * \param prescaler: prescaler value
  * \param pscreload: prescaler reload mode only one parameter can be selected which is shown as below:
  *     \arg BASIC_TIMER_EGR_UG: the prescaler is loaded right now
  *     \arg TIMER_PSC_RELOAD_UPDATE: the prescaler is loaded at the next update event
  */
void Basic_Timer_PrescalerConfig(BASIC_TIMER_TypeDef* timer, uint32_t prescaler, uint32_t pscreload)
{
    timer->PSC = (uint32_t)prescaler;
    if (BASIC_TIMER_EGR_UG == pscreload) {
        timer->EGR |= (uint32_t)BASIC_TIMER_EGR_UG;
    }
}

/**
  * \brief configure TIMER autoreload register value
  *
  * \param timer: TIMERx(x=0..4)
  * \param autoreload: the counter auto-reload value
  */
void Basic_Timer_AutoReloadValueConfig(BASIC_TIMER_TypeDef* timer, uint32_t autoreload)
{
    timer->ARR = (uint32_t)autoreload;
}

/**
  * \brief configure TIMER counter register value
  *
  * \param timer: TIMERx(x=0..4)
  * \param counter: the counter value
  */
void Basic_Timer_CounterValueConfig(BASIC_TIMER_TypeDef* timer, uint32_t counter)
{
    timer->CNT = (uint32_t)counter;
}

/**
  * \brief read TIMER counter value
  *
  * \param timer: TIMERx(x=0..4)
  *
  * \return counter value
  */
uint32_t Basic_Timer_CounterRead(BASIC_TIMER_TypeDef* timer)
{
    uint32_t count_value = 0U;
    count_value = timer->CNT;
    return (count_value);
}

/**
  * \brief read TIMER prescaler value
  *
  * \param timer: TIMERx(x=0..4)
  *
  * \return prescaler register value
  */
uint32_t Basic_Timer_PrescalerRead(BASIC_TIMER_TypeDef* timer)
{
    uint32_t prescaler_value = 0U;
    prescaler_value = (uint32_t) (timer->PSC);
    return (prescaler_value);
}

/**
  * \brief configure TIMER single pulse mode
  *
  * \param timer: TIMERx(x=0..4)
  * \param spmode: only one parameter can be selected which is shown as below:
  *     \arg BASIC_TIMER_CR1_OPM: single pulse mode
  *     \arg BASIC_TIMER_CR1_OPM_DISABLED: repetitive pulse mode
  */
void Basic_Timer_SinglePulseModeConfig(BASIC_TIMER_TypeDef* timer, uint32_t spmode)
{
    if (BASIC_TIMER_CR1_OPM == spmode) {
        timer->CR1 |= (uint32_t)BASIC_TIMER_CR1_OPM;
    } else if (BASIC_TIMER_CR1_OPM_DISABLED == spmode) {
        timer->CR1 &= ~((uint32_t)BASIC_TIMER_CR1_OPM);
    } else {
        /* illegal parameters */
    }
}

/**
  * \brief configure TIMER update source
  *
  * \param timer: TIMERx(x=0..4)
  * \param update: only one parameter can be selected which is shown as below:
  *     \arg BASIC_TIMER_CR1_UPD_EVT_SEL_ANY_ENABLED: update generate by setting of UPG bit or the counter overflow/underflow,
  *         or the slave mode controller trigger
  *     \arg BASIC_TIMER_CR1_UPD_EVT_SEL: update generate only by counter overflow/underflow
  */
void Basic_Timer_UpdateSourceConfig(BASIC_TIMER_TypeDef* timer, uint32_t update)
{
    if (BASIC_TIMER_CR1_UPD_EVT_SEL == update) {
        timer->CR1 |= (uint32_t)BASIC_TIMER_CR1_UPD_EVT_SEL;
    } else if (BASIC_TIMER_CR1_UPD_EVT_SEL_ANY_ENABLED == update) {
        timer->CR1 &= ~(uint32_t)BASIC_TIMER_CR1_UPD_EVT_SEL;
    } else {
        /* illegal parameters */
    }
}

/**
  * \brief enable the TIMER DMA
  *
  * \param timer: TIMERx(x=0..4)
  * \param dma: specify which DMA to enable one or more parameters can be selected which are shown as below:
  *     \arg BASIC_TIMER_DIER_UDE:  update DMA enable, TIMERx(x=0..4)
  */
void Basic_Timer_DmaEnable(BASIC_TIMER_TypeDef* timer, uint32_t dma)
{
    timer->DIER |= (uint32_t) dma;
}

/**
  * \brief disable the TIMER DMA
  *
  * \param timer: TIMERx(x=0..4)
  * \param dma: specify which DMA to disbale one or more parameters can be selected which are shown as below:
  *     \arg BASIC_TIMER_DIER_UDE:  update DMA enable, TIMERx(x=0..4)
  */
void Basic_Timer_DmaDisable(BASIC_TIMER_TypeDef* timer, uint32_t dma)
{
    timer->DIER &= (~(uint32_t)(dma));
}

/**
  * \brief software generate events
  *
  * \param timer: TIMERx(x=0..4)
  * \param event: the timer software event generation sources one or more parameters can be selected which are shown as below:
  *     \arg TIMER_EVENT_SRC_UPG: update event generation, TIMERx(x=0..4)
  */
void Basic_Timer_EventSoftwareGenerate(BASIC_TIMER_TypeDef* timer, uint32_t event)
{
    timer->EGR |= (uint32_t)event;
}

/**
  * \brief select TIMER master mode output trigger source
  *
  * \param timer: TIMERx(x=0..4)
  * \param outrigger: only one parameter can be selected which is shown as below:
  *     \arg TIMER_TRI_OUT_SRC_RESET: the UPG bit as trigger output(TIMERx(x=0..4))
  *     \arg TIMER_TRI_OUT_SRC_ENABLE: the counter enable signal BASIC_TIMER_CR1_CEN as trigger output(TIMERx(x=0..4))
  *     \arg TIMER_TRI_OUT_SRC_UPDATE: update event as trigger output(TIMERx(x=0..4))
  */
void Basic_Timer_MasterOutputTriggerSourceSelect(BASIC_TIMER_TypeDef* timer, uint32_t outrigger)
{
    timer->CR2 &= (~(uint32_t)BASIC_TIMER_CR2_MMS_MASK);
    timer->CR2 |= (uint32_t)outrigger;
}

/**
  * \brief enable the TIMER interrupt
  *
  * \param timer: please refer to the following parameters
  * \param interrupt: specify which interrupt to enable
  *     one or more parameters can be selected which are shown as below:
  *     \arg BASIC_TIMER_DIER_UIE: update interrupt enable, TIMERx(x=0..4)
  */
void Basic_Timer_InterruptEnable(BASIC_TIMER_TypeDef* timer, uint32_t interrupt)
{
    timer->DIER |= (uint32_t) interrupt;
}

/**
  * \brief disable the TIMER interrupt
  *
  * \param timer: TIMERx(x=0..4)
  * \param interrupt: specify which interrupt to disbale
  *     one or more parameters can be selected which are shown as below:
  *     \arg BASIC_TIMER_DIER_UIE: update interrupt enable, TIMERx(x=0..4)
  */
void Basic_Timer_InterruptDisable(BASIC_TIMER_TypeDef* timer, uint32_t interrupt)
{
    timer->DIER &= (~(uint32_t)interrupt);
}

/**
  * \brief get timer interrupt flag
  *
  * \param timer: TIMERx(x=0..4)
  * \param interrupt: the timer interrupt bits
  *     only one parameter can be selected which is shown as below:
  *     \arg TIMER_INT_FLAG_UP: update interrupt flag, TIMERx(x=0..4)
  *
  * \retval FlagStatus: SET or RESET
  */
FlagStatus Basic_Timer_InterruptFlagGet(BASIC_TIMER_TypeDef* timer, uint32_t interrupt)
{
    uint32_t val;
    val = (timer->DIER & interrupt);
    if ((RESET != (timer->SR & interrupt)) && (RESET != val)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief clear TIMER interrupt flag
  *
  * \param timer: TIMERx(x=0..4)
  * \param interrupt: the timer interrupt bits
  *     one or more parameters can be selected which are shown as below:
  *     \arg TIMER_INT_FLAG_UP: update interrupt flag, TIMERx(x=0..4)
  */
void Basic_Timer_InterruptFlagClear(BASIC_TIMER_TypeDef* timer, uint32_t interrupt)
{
    timer->SR = (~(uint32_t)interrupt);
}

/**
  * \brief get TIMER flags
  *
  * \param timer: TIMERx(x=0..4)
  * \param flag: the timer interrupt flags
  *     only one parameter can be selected which is shown as below:
  *     \arg TIMER_FLAG_UP: update flag, TIMERx(x=0..4)
  *
  * \retval     FlagStatus: SET or RESET
  */
FlagStatus Basic_Timer_FlagGet(BASIC_TIMER_TypeDef* timer, uint32_t flag)
{
    if (RESET != (timer->SR & flag)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief clear TIMER flags
  *
  * \param timer: TIMERx(x=0..4)
  * \param flag: the timer interrupt flags
  *     one or more parameters can be selected which are shown as below:
  *     \arg TIMER_FLAG_UP: update flag, TIMERx(x=0..4)
  */
void Basic_Timer_FlagClear(BASIC_TIMER_TypeDef* timer, uint32_t flag)
{
    timer->SR = (~(uint32_t)flag);
}

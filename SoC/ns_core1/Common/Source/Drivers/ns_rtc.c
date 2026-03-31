
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
  * \file ns_rtc.c
  * \brief source file for the RTC
  */

#include "ns_rtc.h"

/* RTC register high / low bits mask */
#define RTC_HIGH_BITS_MASK         ((uint32_t)0xFFFFFFFFU)  /* RTC high bits mask */
#define RTC_LOW_BITS_MASK          ((uint32_t)0xFFFFFFFFU)  /* RTC low bits mask */

/* RTC register high bits offset */
#define RTC_HIGH_BITS_OFFSET       ((uint32_t)32U)

/**
  * \brief wait RTC last write operation finished
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  */
void RTC_LwOffWait(RTC_TypeDef *RTCx)
{
    /* loop until LWOFF flag is set */
    while (RESET == (RTCx->CRL & RTC_CRL_RTOFF)) {}
}

/**
  * \brief enter RTC configuration mode
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  */
void RTC_ConfigModeEnter(RTC_TypeDef *RTCx)
{
    RTCx->CRL |= RTC_CRL_CNF;
    RTC_LwOffWait(RTCx);
}

/**
  * \brief exit RTC configuration mode
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  */
void RTC_ConfigModeExit(RTC_TypeDef *RTCx)
{
    RTCx->CRL &= ~RTC_CRL_CNF;
    RTC_LwOffWait(RTCx);
}

/**
  * \brief set RTC counter value
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  * \param cnt set the value of counter register,range from
  *         0 to 2^64-1
  */
void RTC_CounerSet(RTC_TypeDef *RTCx, uint64_t cnt)
{
    RTC_ConfigModeEnter(RTCx);
    /* set the RTC counter high bits */
    RTCx->CNTH = cnt >> 32;
    RTC_LwOffWait(RTCx);
    /* set the RTC counter low bits */
    RTCx->CNTL = cnt;
    RTC_LwOffWait(RTCx);
    RTC_ConfigModeExit(RTCx);
}

/**
  * \brief set RTC prescaler value
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  * \param prl set the value of prescaler register,range from
  *         0 to 2^20-1
  */
void RTC_PrescalerSet(RTC_TypeDef *RTCx, uint32_t prl)
{
    RTC_ConfigModeEnter(RTCx);
    /* set the RTC prescaler */
    RTCx->PRL = (prl & RTC_PRL_CLK_MASK);
    RTC_LwOffWait(RTCx);
    RTC_ConfigModeExit(RTCx);
}

/**
  * \brief set RTC alarm value
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  * \param alarm set the value of alarm register,range from
  *         0 to 2^64-1
  */
void RTC_AlarmSet(RTC_TypeDef *RTCx, uint64_t alarm)
{
    RTC_ConfigModeEnter(RTCx);
    /* set the alarm high bits */
    RTCx->ALRH = alarm >> 32;
    RTC_LwOffWait(RTCx);
    /* set the alarm low bits */
    RTCx->ALRL = alarm;
    RTC_LwOffWait(RTCx);
    RTC_ConfigModeExit(RTCx);
}

/**
  * \brief get RTC counter value
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  *
  * \retval RTC counter value
  */
uint64_t RTC_CounterGet(RTC_TypeDef *RTCx)
{
    uint64_t counter = RTCx->CNTH;
    counter <<= 32;
    counter |= RTCx->CNTL;
    return counter;
}

/**
  * \brief get RTC prescaler counter value
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  *
  * \retval RTC prescaler counter value
  */
uint32_t RTC_DividerGet(RTC_TypeDef *RTCx)
{
    uint32_t temp = 0x00U;
    temp = RTCx->DIV;
    return temp;
}

/**
  * \brief get RTC flag status
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  * \param flag specify which flag status to get
  *     only one parameter can be selected which is shown as below:
  *     \arg RTC_FLAG_SECOND: second interrupt flag
  *     \arg RTC_FLAG_ALARM: alarm interrupt flag
  *     \arg RTC_FLAG_OVERFLOW: overflow interrupt flag
  *     \arg RTC_FLAG_CNF: configuration flag
  *     \arg RTC_FLAG_RTOFF: RTC operation OFF（icb_clk -> rtc_clk done!） flag
  *
  * \retval SET or RESET
  */
FlagStatus RTC_FlagGet(RTC_TypeDef *RTCx, uint32_t flag)
{
    if (RESET != (RTCx->CRL & flag)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief clear RTC flag status
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  * \param flag specify which flag status to get
  *     only one parameter can be selected which is shown as below:
  *     \arg RTC_FLAG_SECOND: second interrupt flag
  *     \arg RTC_FLAG_ALARM: alarm interrupt flag
  *     \arg RTC_FLAG_OVERFLOW: overflow interrupt flag
  *     \arg RTC_FLAG_CNF: configuration flag
  *     \arg RTC_FLAG_RTOFF: RTC operation OFF（icb_clk -> rtc_clk done!） flag
  */
void RTC_FlagClear(RTC_TypeDef *RTCx, uint32_t flag)
{
    /* clear RTC flag */
    RTCx->CRL &= ~flag;
}

/**
  * \brief get RTC interrupt flag status
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  * \param flag specify which flag status to get
  *     only one parameter can be selected which is shown as below:
  *     \arg RTC_CRL_SECF: second interrupt flag
  *     \arg RTC_CRL_ALRF: alarm interrupt flag
  *     \arg RTC_CRL_OWF: overflow interrupt flag
  *
  * \retval SET or RESET
  */
FlagStatus RTC_InterruptFlagGet(RTC_TypeDef *RTCx, uint32_t flag)
{
    if (RESET != (RTCx->CRL & flag)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief clear RTC interrupt flag status
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  * \param flag specify which flag status to get
  *     only one parameter can be selected which is shown as below:
  *     \arg RTC_CRL_SECF: second interrupt flag
  *     \arg RTC_CRL_ALRF: alarm interrupt flag
  *     \arg RTC_CRL_OWF: overflow interrupt flag
  */
void RTC_InterruptFlagClear(RTC_TypeDef *RTCx, uint32_t flag)
{
    /* clear RTC flag */
    RTCx->CRL &= ~flag;
}

/**
  * \brief enable RTC interrupt
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  * \param interrupt which interrupt want to enable
  *     \arg RTC_CRH_SECIE: second interrupt Enable
  *     \arg RTC_CRH_ALRIE: alarm interrupt Enable
  *     \arg RTC_CRH_OWIE: overflow interrupt Enable
  */
void RTC_InterruptEnable(RTC_TypeDef *RTCx, uint32_t interrupt)
{
    RTCx->CRH |= interrupt;
}

/**
  * \brief Disable RTC Interrupt
  *
  * \param RTCx Where x can be 0 to select the RTC peripheral.
  * \param interrupt which interrupt want to disable
  *     \arg RTC_CRH_SECIE: second interrupt Enable
  *     \arg RTC_CRH_ALRIE: alarm interrupt Enable
  *     \arg RTC_CRH_OWIE: overflow interrupt Enable
  */
void RTC_InterruptDisable(RTC_TypeDef *RTCx, uint32_t interrupt)
{
    RTCx->CRH &= ~interrupt;
}

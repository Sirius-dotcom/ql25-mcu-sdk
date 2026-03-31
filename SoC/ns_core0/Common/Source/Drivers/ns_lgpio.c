/*
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier Apache-2.0
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

/* Includes ------------------------------------------------------------------*/
#include "ns.h"
#include "ns_lgpio.h"

/*!
 * \file     ns__gpio.c
 * \brief    This file contains all the functions prototypes for the LGPIO firmware
  */

/**
  * \brief  Configurates the specified LGPIO event rise function.
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param  Status Status of the specified LGPIO rise event.
  *                This parameter can be ENABLE or DISABLE.
  */
void LGPIO_EventRiseConfig(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status)
{
    if (Status) {
        LGPIOx->PAD_GRP0_EVENT_RISE |= LGPIO_Pin;
    } else {
        LGPIOx->PAD_GRP0_EVENT_RISE &= ~LGPIO_Pin;
    }
}

/**
  * \brief  Configurates the specified LGPIO event fall function.
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param  Status Status of the specified LGPIO fall event.
  *                This parameter can be ENABLE or DISABLE.
  */
void LGPIO_EventFallConfig(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status)
{
    if (Status) {
        LGPIOx->PAD_GRP0_EVENT_FALL |= LGPIO_Pin;
    } else {
        LGPIOx->PAD_GRP0_EVENT_FALL &= ~LGPIO_Pin;
    }
}

/**
  * \brief  Sets or clears the selected data port bit.
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param  BitVal specifies the value to be written to the selected bit.
  *     \arg 0 to clear the port pin
  *     \arg 1 to set the port pin
  */
void LGPIO_WriteBit(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, uint32_t BitVal)
{
    /* If BitVal != 0, mean set gpio pin high, otherwise set pin low */
    if (BitVal) {
        LGPIOx->PAD_GRP0_OVAL |= LGPIO_Pin;
    } else {
        LGPIOx->PAD_GRP0_OVAL &= ~LGPIO_Pin;
    }
}

/**
  * \brief      configure GPIO iof mode
  * \param[in]  LGPIOx LGPIO peripheral.
  * \param[in]  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param[in]  mode IOF mode 
  *     \arg    HIGHZ High impedance state
  *     \arg    PP push-pull mode
  *     \arg    OD open drain mode
  */
void LGPIO_Mode(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin,uint32_t mode)
{
    switch(mode)
    {
        case HIGHZ:
            LGPIOx->GPIO_GRP0_MODE0 &= ~LGPIO_Pin;
            LGPIOx->GPIO_GRP0_MODE1 &= ~LGPIO_Pin;
            break;
        case PP:
            LGPIOx->GPIO_GRP0_MODE0 |= LGPIO_Pin;
            LGPIOx->GPIO_GRP0_MODE1 &= ~LGPIO_Pin;
            break;
        case OD:
            LGPIOx->GPIO_GRP0_MODE0 &= ~LGPIO_Pin;
            LGPIOx->GPIO_GRP0_MODE1 |= LGPIO_Pin;
            break;
        default:
            break;
    }
}
/**
  * \brief  configure GPIO PULL mode
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param[in]  pullmode PULLmode PULL_UP, PULL_DUWN, SPU.
  */
void LGPIO_PULLMode(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin,uint32_t pullmode)
{
    switch(pullmode)
    {
        case HIGH_Z:
            LGPIOx->PAD_GRP0_PULL_MODE1 &= ~LGPIO_Pin;
            LGPIOx->PAD_GRP0_PULL_MODE0 &= ~LGPIO_Pin;
            break;
        case WPUE:
            LGPIOx->PAD_GRP0_PULL_MODE1 &= ~LGPIO_Pin;
            LGPIOx->PAD_GRP0_PULL_MODE0 |= LGPIO_Pin;
            break;
        case SPUE:
            LGPIOx->PAD_GRP0_PULL_MODE1 |= LGPIO_Pin;
            LGPIOx->PAD_GRP0_PULL_MODE0 &= ~LGPIO_Pin;
            break;
        case PDE:
            LGPIOx->PAD_GRP0_PULL_MODE1 |= LGPIO_Pin;
            LGPIOx->PAD_GRP0_PULL_MODE0 |= LGPIO_Pin;
            break;
        default:
            break;
    }
}

/**
  * \brief  Toggles the specified the selected data port bit.
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  */
void LGPIO_Toggle(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin)
{
    LGPIOx->PAD_GRP0_TOGGLE = LGPIO_Pin;
}

/**
  * \brief  Set the specified the selected data port bit.
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  */
void PAD_GRP0_SET(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin)
{
    LGPIOx->PAD_GRP0_SET = LGPIO_Pin;
}

/**
  * \brief  Clear the specified the selected data port bit.
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  */
void PAD_GRP0_CLR(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin)
{
    LGPIOx->PAD_GRP0_CLR = LGPIO_Pin;
}

/**
  * \brief  Turn on/off soft-ware bus keep .
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param  Status Status of the specified LGPIO rise event.
  *                This parameter can be ENABLE or DISABLE.
  */
void GPIO_SW_KEEP(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin,  ControlStatus Status)
{
    if(Status)
    {
        LGPIOx->PAD_GRP0_SW |= LGPIO_Pin;
    }
    else
    {
        LGPIOx->PAD_GRP0_SW &= ~LGPIO_Pin;
    } 
}

/**
  * \brief  Reads the specified LGPIO input pin.
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \retval the LGPIO input pin value.
  */
uint32_t LGPIO_ReadInputDataBit(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin)
{
    uint32_t lgpio_val = LGPIOx->PAD_GRP0_IVAL & LGPIO_Pin;
    return lgpio_val;
}

/**
  * \brief  Enables or disables the specified LGPIO interrupts.
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param  LGPIO_IE specifies the LGPIO interrupt sources to be enabled or disabled.
  *     \arg LGPIO_IT_RISE
  *     \arg LGPIO_IT_FALL
  *     \arg LGPIO_IT_HIGH
  *     \arg LGPIO_IT_LOW
  * \param  Status Status of the specified LGPIO interrupts.
  *                 This parameter can be ENABLE or DISABLE.
  */
void LGPIO_ITConfig(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, uint32_t LGPIO_IE, ControlStatus Status)
{
    switch (LGPIO_IE) {
        case LGPIO_IT_RISE:
            if (Status){
                LGPIOx->PAD_GRP0_RISE_IE  |= LGPIO_Pin;
            } else {
                LGPIOx->PAD_GRP0_RISE_IE  &= ~LGPIO_Pin;
            }
            break;
        case LGPIO_IT_FALL:
            if (Status) {
                LGPIOx->PAD_GRP0_FALL_IE  |= LGPIO_Pin;
            } else {
                LGPIOx->PAD_GRP0_FALL_IE  &= ~LGPIO_Pin;
            }
            break;
        case LGPIO_IT_HIGH:
            if (Status) {
                LGPIOx->PAD_GRP0_HIGH_IE  |= LGPIO_Pin;
            } else {
                LGPIOx->PAD_GRP0_HIGH_IE  &= ~LGPIO_Pin;
            }
            break;
        case LGPIO_IT_LOW:
                if (Status) {
                    LGPIOx->PAD_GRP0_LOW_IE   |= LGPIO_Pin;
                } else {
                    LGPIOx->PAD_GRP0_LOW_IE   &= ~LGPIO_Pin;
                }
            break;
        default:
            break;
    }
}

/**
  * \brief  Clears the specified LGPIO interrupt source.
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param  LGPIO_IT specifies the LGPIO interrupt source to clear.
  *     \arg    LGPIO_IT_RISE
  *     \arg    LGPIO_IT_FALL
  *     \arg    LGPIO_IT_HIGH
  *     \arg    LGPIO_IT_LOW
  */
void LGPIO_ITClear(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, uint32_t LGPIO_IT)
{
    switch (LGPIO_IT) {
        case LGPIO_IT_RISE:
            LGPIOx->PAD_GRP0_RISE_IP &= ~LGPIO_Pin;
            break;
        case LGPIO_IT_FALL:
            LGPIOx->PAD_GRP0_FALL_IP &= ~LGPIO_Pin;
            break;
        case LGPIO_IT_HIGH:
            LGPIOx->PAD_GRP0_HIGH_IP &= ~LGPIO_Pin;
            break;
        case LGPIO_IT_LOW:
            LGPIOx->PAD_GRP0_LOW_IP  &= ~LGPIO_Pin;
            break;
        default:
            break;
    }
}

/**
  * \brief  Checks whether the specified LGPIO interrupt has occurred or not.
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param  LGPIO_IT specifies the LGPIO interrupt source to check.
        \arg    LGPIO_IT_RISE
        \arg    LGPIO_IT_FALL
        \arg    LGPIO_IT_HIGH
        \arg    LGPIO_IT_LOW
  * \retval Set status of gpio pin source
  */
uint32_t LGPIO_GetITStatus(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, uint32_t LGPIO_IT)
{
    uint32_t value_tmp = 0;
    switch (LGPIO_IT) {
        case LGPIO_IT_RISE:
            value_tmp = LGPIOx->PAD_GRP0_RISE_IP & LGPIO_Pin;
            break;
        case LGPIO_IT_FALL:
            value_tmp = LGPIOx->PAD_GRP0_FALL_IP & LGPIO_Pin;
            break;
        case LGPIO_IT_HIGH:
            value_tmp = LGPIOx->PAD_GRP0_HIGH_IP & LGPIO_Pin;
            break;
        case LGPIO_IT_LOW:
            value_tmp = LGPIOx->PAD_GRP0_LOW_IP  & LGPIO_Pin;
            break;
        default:
            break;
    }
    return value_tmp;
}

/**
  * \brief  Check which lgpio is interrupted.
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
        \arg    LGPIO_IRQ_STATUS
  * \retval Set status of gpio pin source.
  */
uint32_t LGPIO_GetIRQStatus(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin)
{
    uint32_t value_tmp = 0;
    value_tmp = LGPIOx->GPIO_GRP0 & LGPIO_Pin;
    return value_tmp;
}

/**
  * \brief  Sets the specified LGPIO soft interrupt.
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param  LGPIO_IT specifies the LGPIO soft interrupt.
  *     \arg    LGPIO_IT_RISE
  *     \arg    LGPIO_IT_FALL
  *     \arg    LGPIO_IT_HIGH
  *     \arg    LGPIO_IT_LOW
  */
void LGPIO_SetSoftIT(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, uint32_t LGPIO_IT)
{
    switch (LGPIO_IT) {
        case LGPIO_IT_RISE:
            LGPIOx->PAD_GRP0_RISE_IP |= LGPIO_Pin;
            break;
        case LGPIO_IT_FALL:
            LGPIOx->PAD_GRP0_FALL_IP |= LGPIO_Pin;
            break;
        case LGPIO_IT_HIGH:
            LGPIOx->PAD_GRP0_HIGH_IP |= LGPIO_Pin;
            break;
        case LGPIO_IT_LOW:
            LGPIOx->PAD_GRP0_LOW_IP  |= LGPIO_Pin;
            break;
        default:
            break;
    }
}

/**
  * \brief   LGPIO pad high event enable
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param  Status Status of the specified LGPIO rise event
  *                This parameter can be ENABLE or DISABLE.
  */
void LGPIO_EVENT_HIGH(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status)
{
    if(Status)
    {
        LGPIOx->PAD_GRP0_EVENT_HIGH |= LGPIO_Pin;
    }
    else
    {
        LGPIOx->PAD_GRP0_EVENT_HIGH &= ~LGPIO_Pin;
    } 
}

/**
  * \brief   LGPIO pad low event enable
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param  Status Status of the specified LGPIO rise event
  *                This parameter can be ENABLE or DISABLE.
  */
void LGPIO_EVENT_LOW(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status)
{
    if(Status)
    {
        LGPIOx->PAD_GRP0_EVENT_LOW |= LGPIO_Pin;
    }
    else
    {
        LGPIOx->PAD_GRP0_EVENT_LOW &= ~LGPIO_Pin;
    } 
}

/**
  * \brief   LGPIO pad digital noise filter enable
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param  Status Status of the specified LGPIO rise event
  *                This parameter can be ENABLE or DISABLE.
  */
void LGPIO_Digital_Noise_Filter(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status)
{
    if(Status)
    {
        LGPIOx->PAD_GRP0_DIGITAL_NOISE_FILTER |= LGPIO_Pin;
    }
    else
    {
        LGPIOx->PAD_GRP0_DIGITAL_NOISE_FILTER &= ~LGPIO_Pin;
    } 
}

/**
  * \brief   LGPIO pad digital noise filter num
  * \param  LGPIOx the LGPIO peripheral.
  * \param  LGPIO_Pin specifies the LGPIO pin from 0 to 31.
  * \param  Status Status of the specified LGPIO rise event
  *                This parameter can be ENABLE or DISABLE.
  */
void LGPIO_DN_Filter_Num(LGPIO_TypeDef *LGPIOx, uint32_t num)
{
    LGPIOx->DIGITAL_NOISE_FILTER &= (~LGPIO_DIGITAL_NOISE_FILTER_NUM_MASK);
    
    LGPIOx->DIGITAL_NOISE_FILTER = num;
}

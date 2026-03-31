/*!
    \file    ns_eval.c
    \brief   firmware functions to manage leds, keys, COM ports

    \version V1.0.0,
*/

#include "ns_sdk_hal.h"
#include "ns_usart.h"

#include "ns_lgpio.h"

#define LEDn 3
static uint32_t LED_CLORK[] = {SOC_LED_RED_GPIO_MASK, SOC_LED_GREEN_GPIO_MASK,SOC_LED_BLUE_GPIO_MASK};

/*!
    \brief      configure led LGPIO
    \param[in]  lednum: specify the led to be configured
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void Nuclei_EvalLedInit(led_typedef_enum lednum)
{

}

/*!
    \brief      turn on selected led
    \param[in]  lednum: specify the led to be turned on
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void Nuclei_EvalLedOn(led_typedef_enum lednum)
{
    
}

/*!
    \brief      turn off selected led
    \param[in]  lednum: specify the led to be turned off
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void Nuclei_EvalLedOff(led_typedef_enum lednum)
{
    
}

/*!
    \brief      toggle selected led
    \param[in]  lednum: specify the led to be toggled
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void Nuclei_EvalLedToggle(led_typedef_enum lednum)
{
    
}

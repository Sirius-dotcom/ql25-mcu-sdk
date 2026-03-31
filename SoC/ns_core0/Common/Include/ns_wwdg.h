
/*!
    \file  ns_.h
    \brief definitions for the WWDG
*/

/*
    Copyright (c) 2020, Nucleisys Technology  Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/
#ifndef _NS_WWDG_H_
#define _NS_WWDG_H_
#include "ns.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define WWDG_CR_OFFSET                                                 0x0 /*!< Control register. */
#define WWDG_CFR_OFFSET                                                0x4 /*!< Configuration register. */
#define WWDG_SR_OFFSET                                                 0x8 /*!< Status register. */
#define WWDG_VER_OFFSET                                                0xc /*!< IP_VERSION. */

 /* ===== WWDG CR Register definition ===== */
#define WWDG_CR_T_MASK                       BITS(0,21)                                   /*!< WWDG CR: T Bit Mask */  
#define WWDG_CR_T_OFS                        0U                                          /*!< WWDG CR: T Bit Offset */
#define WWDG_CR_T(regval)                    (BITS(0,21) & ((uint32_t)(regval) << 0))        /*!< WWDG CR: T Bit Value */  
#define WWDG_CR_WDGA                         BIT(31)                                      /*!< Activation bit This bit is set by software and only cleared by hardware after a reset. When WDGA = 1, the watchdog can generate a reset. -1'b0: watchdog_disable : Watchdog disabled -1'b1: watchdog_enable :  Watchdog enabled */
#define WWDG_CR_WDGA_OFS                     31U                                          /*!< WWDG CR: WDGA Bit Offset */
#define WWDG_CR_WDGA_VAL(regval)                 (BIT(31) & ((uint32_t)(regval) << 31))        /*!< WWDG CR: WDGA Bit Value */  
#define WWDG_CR_WDGA_WATCHDOG_DISABLE             0x0UL                                                  /*!< WATCHDOG_DISABLE */
#define WWDG_CR_WDGA_WATCHDOG_ENABLE              BIT(31)                                                  /*!< WATCHDOG_ENABLE */

/**
  * \brief Check the WWDG cr wdga bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid wdga bit.
  * \retval 1 This is a valid wdga bit.
  */
#define IS_WWDG_CR_WDGA(regval)                   (\
                                             ((regval) == WWDG_CR_WDGA_WATCHDOG_DISABLE    ) || \
                                             ((regval) == WWDG_CR_WDGA_WATCHDOG_ENABLE     )  \
                                                 )

 /* ===== WWDG CFR Register definition ===== */
#define WWDG_CFR_W_MASK                       BITS(0,21)                                   /*!< WWDG CFR: W Bit Mask */  
#define WWDG_CFR_W_OFS                        0U                                          /*!< WWDG CFR: W Bit Offset */
#define WWDG_CFR_W(regval)                    (BITS(0,21) & ((uint32_t)(regval) << 0))        /*!< WWDG CFR: W Bit Value */  
#define WWDG_CFR_WDGTB_MASK                   BITS(28,30)                                   /*!< WWDG CFR: WDGTB Bit Mask */  
#define WWDG_CFR_WDGTB_OFS                    28U                                          /*!< WWDG CFR: WDGTB Bit Offset */
#define WWDG_CFR_WDGTB(regval)                (BITS(28,30) & ((uint32_t)(regval) << 28))        /*!< WWDG CFR: WDGTB Bit Value */  
#define WWDG_CFR_WDGTB_DIV_1                       WWDG_CFR_WDGTB(0)                                                   /*!< DIV_1 */
#define WWDG_CFR_WDGTB_DIV_2                       WWDG_CFR_WDGTB(1)                                                   /*!< DIV_2 */
#define WWDG_CFR_WDGTB_DIV_4                       WWDG_CFR_WDGTB(2)                                                   /*!< DIV_4 */
#define WWDG_CFR_WDGTB_DIV_8                       WWDG_CFR_WDGTB(3)                                                   /*!< DIV_8 */
#define WWDG_CFR_WDGTB_DIV_16                      WWDG_CFR_WDGTB(4)                                                   /*!< DIV_16 */
#define WWDG_CFR_WDGTB_DIV_32                      WWDG_CFR_WDGTB(5)                                                   /*!< DIV_32 */
#define WWDG_CFR_WDGTB_DIV_64                      WWDG_CFR_WDGTB(6)                                                   /*!< DIV_64 */
#define WWDG_CFR_WDGTB_DIV_128                     WWDG_CFR_WDGTB(7)                                                   /*!< DIV_128 */

/**
  * \brief Check the WWDG cfr wdgtb bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid wdgtb bits.
  * \retval 1 This is a valid wdgtb bits.
  */
#define IS_WWDG_CFR_WDGTB(regval)                  (\
                                            ((regval) == WWDG_CFR_WDGTB_DIV_1               ) || \
                                            ((regval) == WWDG_CFR_WDGTB_DIV_2               ) || \
                                            ((regval) == WWDG_CFR_WDGTB_DIV_4               ) || \
                                            ((regval) == WWDG_CFR_WDGTB_DIV_8               ) || \
                                            ((regval) == WWDG_CFR_WDGTB_DIV_16              ) || \
                                            ((regval) == WWDG_CFR_WDGTB_DIV_32              ) || \
                                            ((regval) == WWDG_CFR_WDGTB_DIV_64              ) || \
                                            ((regval) == WWDG_CFR_WDGTB_DIV_128             )  \
                                                 )

#define WWDG_CFR_EWI                          BIT(31)                                      /*!< WWDG reset interrupt enable. An interrupt occurs whenever the counter reaches the value 0x40, when the EWI field is set. -1'b0: disable : Disable  -1'b1: enable : Enable */
#define WWDG_CFR_EWI_OFS                      31U                                          /*!< WWDG CFR: EWI Bit Offset */
#define WWDG_CFR_EWI_VAL(regval)                  (BIT(31) & ((uint32_t)(regval) << 31))        /*!< WWDG CFR: EWI Bit Value */  
#define WWDG_CFR_EWI_DISABLE                       0x0UL                                                 /*!< DISABLE */
#define WWDG_CFR_EWI_ENABLE                        BIT(31)                                                  /*!< ENABLE */

/**
  * \brief Check the WWDG cfr ewi bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ewi bit.
  * \retval 1 This is a valid ewi bit.
  */
#define IS_WWDG_CFR_EWI(regval)                    (\
                                              ((regval) == WWDG_CFR_EWI_DISABLE             ) || \
                                              ((regval) == WWDG_CFR_EWI_ENABLE              )  \
                                                 )

 /* ===== WWDG SR Register definition ===== */
#define WWDG_SR_EWIF                         BIT(0)                                      /*!< Early wakeup interrupt flag This bit is set by hardware when the counter has reached the value 0x40. It must be cleared by software by writing 0. A write of 1 has no effect. This bit is also set if the interrupt is not Enabled. */
#define WWDG_SR_EWIF_OFS                     0U                                          /*!< WWDG SR: EWIF Bit Offset */
#define WWDG_SR_EWIF_VAL(regval)                 (BIT(0) & ((uint32_t)(regval) << 0))        /*!< WWDG SR: EWIF Bit Value */  
#define WWDG_SR_T_UPD                        BIT(1)                                      /*!< Watchdog counter value update flag This bit is set by hardware to indicate that an update of the watchdog count is ongoing. If the update process is finish, This bit will cleared by hardware. -1'b0: watchdog_disable :The update process is done. -1'b1: watchdog_enable :The update process is ongoing. */
#define WWDG_SR_T_UPD_WATCHDOG_DISABLE             ((uint32_t)(0) << 1)                                                    /*!< WATCHDOG_DISABLE */
#define WWDG_SR_T_UPD_WATCHDOG_ENABLE             ((uint32_t)(1) << 1)                                                    /*!< WATCHDOG_ENABLE */

/**
  * \brief Check the WWDG sr t_upd bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid t_upd bit.
  * \retval 1 This is a valid t_upd bit.
  */
#define IS_WWDG_SR_T_UPD(regval)                  (\
                                            ((regval) == WWDG_SR_T_UPD_WATCHDOG_DISABLE    ) || \
                                            ((regval) == WWDG_SR_T_UPD_WATCHDOG_ENABLE     )  \
                                                 )

 /* ===== WWDG VER Register definition ===== */
#define WWDG_VER_VERSION_NUM                  BITS(0,31)                

/* configure the window watchdog timer counter value */
void wwdg_counter_update(WWDG_TypeDef * wwdg,uint32_t counter_value);
/* configure counter value, window value, and prescaler divider value */
void wwdg_config(WWDG_TypeDef * wwdg,uint32_t counter, uint32_t window, uint32_t prescaler);

/* enable early wakeup interrupt of WWDG */
void wwdg_interrupt_enable(WWDG_TypeDef * wwdg);
/* check early wakeup interrupt state of WWDG */
FlagStatus wwdg_flag_get(WWDG_TypeDef * wwdg);
/* wait hardware flag get WWDG */
FlagStatus wwdg_wait_hardware_flag_get(WWDG_TypeDef * wwdg);
/* clear early wakeup interrupt state of WWDG */
void wwdg_flag_clear(WWDG_TypeDef * wwdg);
#ifdef __cplusplus
}
#endif
#endif /* _NS__WWDG_H */

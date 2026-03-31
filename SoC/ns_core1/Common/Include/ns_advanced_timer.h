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
#ifndef __NS__TIMER_H__
#define __NS__TIMER_H__

/**
  * \file     nuclei_timer.h
  * \brief    TIMER feature API header file for Nuclei SDK
  */

#ifdef __cplusplus
extern "C" {
#endif

#include "ns.h"

/* constants definitions */
/* TIMER init parameter struct definitions */
typedef struct {
    uint32_t prescaler;                                                       /*!< prescaler value */
    uint32_t alignedmode;                                                     /*!< aligned mode */
    uint32_t counterdirection;                                                /*!< counter direction */
    uint32_t period;                                                          /*!< period value */
    uint32_t clockdivision;                                                   /*!< clock division value */
    uint8_t  repetitioncounter;                                               /*!< the counter repetition value */
    uint32_t cnt;                                                             /*!< cnt */
} TIMER_Init_TypeDef;

/* break parameter struct definitions */
typedef struct {
    uint32_t runoffstate;                                                     /*!< run mode off-state */
    uint32_t ideloffstate;                                                    /*!< idle mode off-state */
    uint32_t deadtime;                                                        /*!< dead time */
    uint32_t breakpolarity;                                                   /*!< break polarity */
    uint32_t outputautostate;                                                 /*!< output automatic enable */
    uint32_t protectmode;                                                     /*!< complementary register protect control */
    uint32_t breakstate;                                                      /*!< break enable */
} TIMER_Break_TypeDef;

/* channel output parameter struct definitions */
typedef struct {
    uint32_t outputstate;                                                     /*!< channel output state */
    uint32_t outputnstate;                                                    /*!< channel complementary output state */
    uint32_t ocpolarity;                                                      /*!< channel output polarity */
    uint32_t ocnpolarity;                                                     /*!< channel complementary output polarity */
    uint32_t ocidlestate;                                                     /*!< idle state of channel output */
    uint32_t ocnidlestate;                                                    /*!< idle state of channel complementary output */
} TIMER_Output_TypeDef;

/* channel input parameter struct definitions */
typedef struct {
    uint32_t icpolarity;                                                      /*!< channel input polarity */
    uint32_t icselection;                                                     /*!< channel input mode selection */
    uint32_t icprescaler;                                                     /*!< channel input capture prescaler */
    uint32_t icfilter;                                                        /*!< channel input capture filter control */
} TIMER_Input_TypeDef;

/* TIMER channel n(n=1,2,3,4) */
#define TIMER_CH_1                          ((uint32_t)0x0000U)               /*!< TIMER channel 1(TIMERx(x=0..4)) */
#define TIMER_CH_2                          ((uint32_t)0x0001U)               /*!< TIMER channel 2(TIMERx(x=0..4)) */
#define TIMER_CH_3                          ((uint32_t)0x0002U)               /*!< TIMER channel 3(TIMERx(x=0..4)) */
#define TIMER_CH_4                          ((uint32_t)0x0003U)               /*!< TIMER channel 4(TIMERx(x=0..4)) */

#define ADV_TIMER_TIM_CR1_OFFSET                                            0x0 /*!< control register 1 */
#define ADV_TIMER_TIM_CR2_OFFSET                                            0x4 /*!< control register 2 */
#define ADV_TIMER_TIM_SMCR_OFFSET                                           0x8 /*!< slave mode contorl register */
#define ADV_TIMER_TIM_DIER_OFFSET                                           0xc /*!< DMA/interrupt enable register */
#define ADV_TIMER_TIM_SR_OFFSET                                             0x10 /*!< status register */
#define ADV_TIMER_TIM_EGR_OFFSET                                            0x14 /*!< event generation register */
#define ADV_TIMER_TIM_CCMR1_OFFSET                                          0x18 /*!< capture/compare mode register 1 */
#define ADV_TIMER_TIM_CCMR2_OFFSET                                          0x1c /*!< capture/compare mode register 2 */
#define ADV_TIMER_TIM_CCER_OFFSET                                           0x20 /*!< capture/compare enable register */
#define ADV_TIMER_TIM_CNT_OFFSET                                            0x24 /*!< counter */
#define ADV_TIMER_TIM_PSC_OFFSET                                            0x28 /*!< prescaler */
#define ADV_TIMER_TIM_ARR_OFFSET                                            0x2c /*!< auto-reload register */
#define ADV_TIMER_TIM_RCR_OFFSET                                            0x30 /*!< repetition counter register */
#define ADV_TIMER_TIM_CCR1_OFFSET                                           0x34 /*!< capture/compare register 1 */
#define ADV_TIMER_TIM_CCR2_OFFSET                                           0x38 /*!< capture/compare register 2 */
#define ADV_TIMER_TIM_CCR3_OFFSET                                           0x3c /*!< capture/compare register 3 */
#define ADV_TIMER_TIM_CCR4_OFFSET                                           0x40 /*!< capture/compare register 4 */
#define ADV_TIMER_TIM_BDTR_OFFSET                                           0x44 /*!< break and dead-time register */
#define ADV_TIMER_TIM_DCR_OFFSET                                            0x48 /*!< DMA control register */
#define ADV_TIMER_TIM_DMAR_OFFSET                                           0x4c /*!< DMA address for full transfer */
#define ADV_TIMER_TIM_VERSION_OFFSET                                        0x50 /*!< adv_timer version. */
#define ADV_TIMER_TIM_IC_PSC_REP_OFFSET                                     0x54 /*!< input capture prescalar for larger prescale. */
#define ADV_TIMER_TIM_AF1_OFFSET                                            0x58 /*!< alternate function option register 1. */
#define ADV_TIMER_TIM_AF2_OFFSET                                            0x5c /*!< alternate function option register 2. */
#define ADV_TIMER_TIM_TISEL_OFFSET                                          0x60 /*!< timer input selection register. */
#define ADV_TIMER_TIM_OUTPUT_DELAY_OFFSET                                   0x64 /*!< timer output delay register. */

 /* ===== ADV TIMER TIM_CR1 Register definition ===== */
#define ADV_TIMER_TIM_CR1_CEN                          BIT(0)                                      /*!< Counter enable - 1'b0: disabled_Counter :Counter disabled - 1'b1: enabled_Counter :Counter enabled */
#define ADV_TIMER_TIM_CR1_CEN_OFS                      0U                                          /*!< ADV TIMER TIM CR1: CEN Bit Offset */
#define ADV_TIMER_TIM_CR1_CEN_VAL(regval)                  (BIT(0) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM CR1: CEN Bit Value */  
#define ADV_TIMER_TIM_CR1_CEN_DISABLED_COUNTER              0x0UL                                             /*!< DISABLED_COUNTER */
#define ADV_TIMER_TIM_CR1_CEN_ENABLED_COUNTER               BIT(0)                                                   /*!< ENABLED_COUNTER */

/**
  * \brief Check the ADV_TIMER tim_cr1 cen bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cen bit.
  * \retval 1 This is a valid cen bit.
  */
#define IS_ADV_TIMER_TIM_CR1_CEN(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_CR1_CEN_DISABLED_COUNTER    ) || \
                                              ((regval) == ADV_TIMER_TIM_CR1_CEN_ENABLED_COUNTER     )  \
                                                 )

#define ADV_TIMER_TIM_CR1_UDIS                         BIT(1)                                      /*!< Update disable This bit is set and cleared by software to enable/disable UEV event generation. - 1'b0:  disabled_UEV  :UEV enabled. The Update (UEV) event is generated by one of the following events:   a. Counter overflow/underflow   b. Setting the UG bit   c. Update generation through the slave mode controller Buffered registers are then loaded with their preload values. - 1'b1:  enabled_UEV  :UEV disabled. The Update event is not generated, shadow registers keep their value (ARR, PSC, CCRx).  */
#define ADV_TIMER_TIM_CR1_UDIS_OFS                     1U                                          /*!< ADV TIMER TIM CR1: UDIS Bit Offset */
#define ADV_TIMER_TIM_CR1_UDIS_VAL(regval)                 (BIT(1) & ((uint32_t)(regval) << 1))        /*!< ADV TIMER TIM CR1: UDIS Bit Value */  
#define ADV_TIMER_TIM_CR1_UDIS_DISABLED_UEV                 0x0UL                                             /*!< DISABLED_UEV */
#define ADV_TIMER_TIM_CR1_UDIS_ENABLED_UEV                  BIT(1)                                                   /*!< ENABLED_UEV */

/**
  * \brief Check the ADV_TIMER tim_cr1 udis bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid udis bit.
  * \retval 1 This is a valid udis bit.
  */
#define IS_ADV_TIMER_TIM_CR1_UDIS(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CR1_UDIS_DISABLED_UEV        ) || \
                                             ((regval) == ADV_TIMER_TIM_CR1_UDIS_ENABLED_UEV         )  \
                                                 )

#define ADV_TIMER_TIM_CR1_URS                          BIT(2)                                      /*!< Update request source This bit is set and cleared by software to select the UEV event sources. - 1'b0:  disable:Any of the following events generate an update interrupt or DMA request if enabled. These events can be:         a. Counter overflow/underflow        b. Setting the UG bit        c. Update generation through the slave mode controller - 1'b1:  enable:Only counter overflow/underflow generates an update interrupt or DMA request if  enabled. */
#define ADV_TIMER_TIM_CR1_URS_OFS                      2U                                          /*!< ADV TIMER TIM CR1: URS Bit Offset */
#define ADV_TIMER_TIM_CR1_URS_VAL(regval)                  (BIT(2) & ((uint32_t)(regval) << 2))        /*!< ADV TIMER TIM CR1: URS Bit Value */  
#define ADV_TIMER_TIM_CR1_URS_DISABLE                       0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR1_URS_ENABLE                        BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr1 urs bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid urs bit.
  * \retval 1 This is a valid urs bit.
  */
#define IS_ADV_TIMER_TIM_CR1_URS(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_CR1_URS_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_CR1_URS_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR1_OPM                          BIT(3)                                      /*!< One pulse mode - 1'b0:  disable: Counter is not stopped at update event - 1'b1:  enable: Counter stops counting at the next update event (clearing the bit CEN) */
#define ADV_TIMER_TIM_CR1_OPM_OFS                      3U                                          /*!< ADV TIMER TIM CR1: OPM Bit Offset */
#define ADV_TIMER_TIM_CR1_OPM_VAL(regval)                  (BIT(3) & ((uint32_t)(regval) << 3))        /*!< ADV TIMER TIM CR1: OPM Bit Value */  
#define ADV_TIMER_TIM_CR1_OPM_DISABLE                       0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR1_OPM_ENABLE                        BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr1 opm bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid opm bit.
  * \retval 1 This is a valid opm bit.
  */
#define IS_ADV_TIMER_TIM_CR1_OPM(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_CR1_OPM_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_CR1_OPM_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR1_DIR                          BIT(4)                                      /*!< Direction - 1'b0:  disable: Counter used as upcounter - 1'b1:  enable: Counter used as downcounter note: when CMS != 0, DIR is update by hardware,so you can't write this bit.but if you real write this bit ,this bit is overwriten.  */
#define ADV_TIMER_TIM_CR1_DIR_OFS                      4U                                          /*!< ADV TIMER TIM CR1: DIR Bit Offset */
#define ADV_TIMER_TIM_CR1_DIR_VAL(regval)                  (BIT(4) & ((uint32_t)(regval) << 4))        /*!< ADV TIMER TIM CR1: DIR Bit Value */  
#define ADV_TIMER_TIM_CR1_DIR_DISABLE                       0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR1_DIR_ENABLE                        BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr1 dir bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dir bit.
  * \retval 1 This is a valid dir bit.
  */
#define IS_ADV_TIMER_TIM_CR1_DIR(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_CR1_DIR_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_CR1_DIR_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR1_CMS_MASK                     BITS(5,6)                                   /*!< ADV TIMER TIM CR1: CMS Bit Mask */  
#define ADV_TIMER_TIM_CR1_CMS_OFS                      5U                                          /*!< ADV TIMER TIM CR1: CMS Bit Offset */
#define ADV_TIMER_TIM_CR1_CMS(regval)                  (BITS(5,6) & ((uint32_t)(regval) << 5))        /*!< ADV TIMER TIM CR1: CMS Bit Value */  
#define ADV_TIMER_TIM_CR1_CMS_EDGE_ALIGNED                  ADV_TIMER_TIM_CR1_CMS(0)                                               /*!< EDGE_ALIGNED */
#define ADV_TIMER_TIM_CR1_CMS_CENTER_ALIGNED1               ADV_TIMER_TIM_CR1_CMS(1)                                               /*!< CENTER_ALIGNED1 */
#define ADV_TIMER_TIM_CR1_CMS_CENTER_ALIGNED2               ADV_TIMER_TIM_CR1_CMS(2)                                               /*!< CENTER_ALIGNED2 */
#define ADV_TIMER_TIM_CR1_CMS_CENTER_ALIGNED3               ADV_TIMER_TIM_CR1_CMS(3)                                               /*!< CENTER_ALIGNED3 */

/**
  * \brief Check the ADV_TIMER tim_cr1 cms bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid cms bits.
  * \retval 1 This is a valid cms bits.
  */
#define IS_ADV_TIMER_TIM_CR1_CMS(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_CR1_CMS_EDGE_ALIGNED        ) || \
                                              ((regval) == ADV_TIMER_TIM_CR1_CMS_CENTER_ALIGNED1     ) || \
                                              ((regval) == ADV_TIMER_TIM_CR1_CMS_CENTER_ALIGNED2     ) || \
                                              ((regval) == ADV_TIMER_TIM_CR1_CMS_CENTER_ALIGNED3     )  \
                                                 )

#define ADV_TIMER_TIM_CR1_ARPE                         BIT(7)                                      /*!< Auto-reload preload enable - 1'b0:  disable:TIMx_ARR register is not buffered - 1'b1:  enable:TIMx_ARR register is buffered */
#define ADV_TIMER_TIM_CR1_ARPE_OFS                     7U                                          /*!< ADV TIMER TIM CR1: ARPE Bit Offset */
#define ADV_TIMER_TIM_CR1_ARPE_VAL(regval)                 (BIT(7) & ((uint32_t)(regval) << 7))        /*!< ADV TIMER TIM CR1: ARPE Bit Value */  
#define ADV_TIMER_TIM_CR1_ARPE_DISABLE                      0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR1_ARPE_ENABLE                       BIT(7)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr1 arpe bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid arpe bit.
  * \retval 1 This is a valid arpe bit.
  */
#define IS_ADV_TIMER_TIM_CR1_ARPE(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CR1_ARPE_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CR1_ARPE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR1_CKD_MASK                     BITS(8,9)                                   /*!< ADV TIMER TIM CR1: CKD Bit Mask */  
#define ADV_TIMER_TIM_CR1_CKD_OFS                      8U                                          /*!< ADV TIMER TIM CR1: CKD Bit Offset */
#define ADV_TIMER_TIM_CR1_CKD(regval)                  (BITS(8,9) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM CR1: CKD Bit Value */  
#define ADV_TIMER_TIM_CR1_CKD_CKD1                          ADV_TIMER_TIM_CR1_CKD(0)                                               /*!< CKD1 */
#define ADV_TIMER_TIM_CR1_CKD_CKD2                          ADV_TIMER_TIM_CR1_CKD(1)                                               /*!< CKD2 */
#define ADV_TIMER_TIM_CR1_CKD_CKD4                          ADV_TIMER_TIM_CR1_CKD(2)                                               /*!< CKD4 */
#define ADV_TIMER_TIM_CR1_CKD_CKD01                         ADV_TIMER_TIM_CR1_CKD(3)                                               /*!< CKD01 */

/**
  * \brief Check the ADV_TIMER tim_cr1 ckd bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid ckd bits.
  * \retval 1 This is a valid ckd bits.
  */
#define IS_ADV_TIMER_TIM_CR1_CKD(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_CR1_CKD_CKD1                ) || \
                                              ((regval) == ADV_TIMER_TIM_CR1_CKD_CKD2                ) || \
                                              ((regval) == ADV_TIMER_TIM_CR1_CKD_CKD4                ) || \
                                              ((regval) == ADV_TIMER_TIM_CR1_CKD_CKD01               )  \
                                                 )

 /* ===== ADV TIMER TIM_CR2 Register definition ===== */
#define ADV_TIMER_TIM_CR2_CCPC                         BIT(0)                                      /*!< Capture/compare preloaded control - 1'b0:  disable: CCxP,CCxNP, CCxE, CCxNE and OCxM bits are not preloaded - 1'b1:  enable: CCxP,CCxNP, CCxE, CCxNE and OCxM bits are preloaded, after having been written, they are updated only when a commutation event (COM) occurs (COMG bit set or rising edge detected on TRGI, depending on the CCUS bit). */
#define ADV_TIMER_TIM_CR2_CCPC_OFS                     0U                                          /*!< ADV TIMER TIM CR2: CCPC Bit Offset */
#define ADV_TIMER_TIM_CR2_CCPC_VAL(regval)                 (BIT(0) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM CR2: CCPC Bit Value */  
#define ADV_TIMER_TIM_CR2_CCPC_DISABLE                      0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR2_CCPC_ENABLE                       BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr2 ccpc bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ccpc bit.
  * \retval 1 This is a valid ccpc bit.
  */
#define IS_ADV_TIMER_TIM_CR2_CCPC(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CR2_CCPC_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CR2_CCPC_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR2_CCUS                         BIT(2)                                      /*!< Capture/compare control update selection - 1'b0:   disable: When capture/compare control bits are preloaded (CCPC=1), they are updated by setting the COMG bit only - 1'b1:   enable: When capture/compare control bits are preloaded (CCPC=1), they are updated by setting the COMG bit or when an rising edge occurs on TRGI */
#define ADV_TIMER_TIM_CR2_CCUS_OFS                     2U                                          /*!< ADV TIMER TIM CR2: CCUS Bit Offset */
#define ADV_TIMER_TIM_CR2_CCUS_VAL(regval)                 (BIT(2) & ((uint32_t)(regval) << 2))        /*!< ADV TIMER TIM CR2: CCUS Bit Value */  
#define ADV_TIMER_TIM_CR2_CCUS_DISABLE                      0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR2_CCUS_ENABLE                       BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr2 ccus bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ccus bit.
  * \retval 1 This is a valid ccus bit.
  */
#define IS_ADV_TIMER_TIM_CR2_CCUS(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CR2_CCUS_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CR2_CCUS_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR2_CCDS                         BIT(3)                                      /*!< Capture/compare DMA selection - 1'b0:  disable: CCx DMA request sent when CCx event occurs - 1'b1:  enable:  CCx DMA requests sent when update event occurs */
#define ADV_TIMER_TIM_CR2_CCDS_OFS                     3U                                          /*!< ADV TIMER TIM CR2: CCDS Bit Offset */
#define ADV_TIMER_TIM_CR2_CCDS_VAL(regval)                 (BIT(3) & ((uint32_t)(regval) << 3))        /*!< ADV TIMER TIM CR2: CCDS Bit Value */  
#define ADV_TIMER_TIM_CR2_CCDS_DISABLE                      0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR2_CCDS_ENABLE                       BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr2 ccds bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ccds bit.
  * \retval 1 This is a valid ccds bit.
  */
#define IS_ADV_TIMER_TIM_CR2_CCDS(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CR2_CCDS_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CR2_CCDS_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR2_MMS_MASK                     BITS(4,6)                                   /*!< ADV TIMER TIM CR2: MMS Bit Mask */  
#define ADV_TIMER_TIM_CR2_MMS_OFS                      4U                                          /*!< ADV TIMER TIM CR2: MMS Bit Offset */
#define ADV_TIMER_TIM_CR2_MMS(regval)                  (BITS(4,6) & ((uint32_t)(regval) << 4))        /*!< ADV TIMER TIM CR2: MMS Bit Value */  
#define ADV_TIMER_TIM_CR2_MMS_RESET_UG                      ADV_TIMER_TIM_CR2_MMS(0)                                               /*!< RESET_UG */
#define ADV_TIMER_TIM_CR2_MMS_EN_CNT                        ADV_TIMER_TIM_CR2_MMS(1)                                               /*!< EN_CNT */
#define ADV_TIMER_TIM_CR2_MMS_TRIGGER_OUTPUT                ADV_TIMER_TIM_CR2_MMS(2)                                               /*!< TRIGGER_OUTPUT */
#define ADV_TIMER_TIM_CR2_MMS_CC1IF                         ADV_TIMER_TIM_CR2_MMS(3)                                               /*!< CC1IF */
#define ADV_TIMER_TIM_CR2_MMS_OC1REF                        ADV_TIMER_TIM_CR2_MMS(4)                                               /*!< OC1REF */
#define ADV_TIMER_TIM_CR2_MMS_OC2REF                        ADV_TIMER_TIM_CR2_MMS(5)                                               /*!< OC2REF */
#define ADV_TIMER_TIM_CR2_MMS_OC3REF                        ADV_TIMER_TIM_CR2_MMS(6)                                               /*!< OC3REF */
#define ADV_TIMER_TIM_CR2_MMS_OC4REF                        ADV_TIMER_TIM_CR2_MMS(7)                                               /*!< OC4REF */

/**
  * \brief Check the ADV_TIMER tim_cr2 mms bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid mms bits.
  * \retval 1 This is a valid mms bits.
  */
#define IS_ADV_TIMER_TIM_CR2_MMS(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_CR2_MMS_RESET_UG            ) || \
                                              ((regval) == ADV_TIMER_TIM_CR2_MMS_EN_CNT              ) || \
                                              ((regval) == ADV_TIMER_TIM_CR2_MMS_TRIGGER_OUTPUT      ) || \
                                              ((regval) == ADV_TIMER_TIM_CR2_MMS_CC1IF               ) || \
                                              ((regval) == ADV_TIMER_TIM_CR2_MMS_OC1REF              ) || \
                                              ((regval) == ADV_TIMER_TIM_CR2_MMS_OC2REF              ) || \
                                              ((regval) == ADV_TIMER_TIM_CR2_MMS_OC3REF              ) || \
                                              ((regval) == ADV_TIMER_TIM_CR2_MMS_OC4REF              )  \
                                                 )

#define ADV_TIMER_TIM_CR2_TI1S                         BIT(7)                                      /*!< TI1 selection - 1'b0:  CH1_TI1: The CH1 pin is connected to TI1 input - 1'b1:  XOR:The CH1,CH2 and CH3 pins are connected to the TI1 input(XOR combination) */
#define ADV_TIMER_TIM_CR2_TI1S_OFS                     7U                                          /*!< ADV TIMER TIM CR2: TI1S Bit Offset */
#define ADV_TIMER_TIM_CR2_TI1S_VAL(regval)                 (BIT(7) & ((uint32_t)(regval) << 7))        /*!< ADV TIMER TIM CR2: TI1S Bit Value */  
#define ADV_TIMER_TIM_CR2_TI1S_CH1_TI1                      0x0UL                                             /*!< CH1_TI1 */
#define ADV_TIMER_TIM_CR2_TI1S_XOR                          BIT(7)                                                   /*!< XOR */

/**
  * \brief Check the ADV_TIMER tim_cr2 ti1s bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ti1s bit.
  * \retval 1 This is a valid ti1s bit.
  */
#define IS_ADV_TIMER_TIM_CR2_TI1S(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CR2_TI1S_CH1_TI1             ) || \
                                             ((regval) == ADV_TIMER_TIM_CR2_TI1S_XOR                 )  \
                                                 )

#define ADV_TIMER_TIM_CR2_OIS1                         BIT(8)                                      /*!< Output Idle state 1 (OC1 output) - 1'b0:  disable:OC1=0 (after a dead-time if OC1N is implemented) when MOE=0 - 1'b1:  enable:OC1=1 (after a dead-time if OC1N is implemented) when MOE=0 */
#define ADV_TIMER_TIM_CR2_OIS1_OFS                     8U                                          /*!< ADV TIMER TIM CR2: OIS1 Bit Offset */
#define ADV_TIMER_TIM_CR2_OIS1_VAL(regval)                 (BIT(8) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM CR2: OIS1 Bit Value */  
#define ADV_TIMER_TIM_CR2_OIS1_DISABLE                      0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR2_OIS1_ENABLE                       BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr2 ois1 bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ois1 bit.
  * \retval 1 This is a valid ois1 bit.
  */
#define IS_ADV_TIMER_TIM_CR2_OIS1(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CR2_OIS1_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CR2_OIS1_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR2_OIS1N                        BIT(9)                                      /*!< Output Idle state 1 (OC1N output) - 1'b0:  disable: OC1N=0 after a dead-time when MOE=0 - 1'b1:  enable: OC1N=1 after a dead-time when MOE=0 */
#define ADV_TIMER_TIM_CR2_OIS1N_OFS                    9U                                          /*!< ADV TIMER TIM CR2: OIS1N Bit Offset */
#define ADV_TIMER_TIM_CR2_OIS1N_VAL(regval)                (BIT(9) & ((uint32_t)(regval) << 9))        /*!< ADV TIMER TIM CR2: OIS1N Bit Value */  
#define ADV_TIMER_TIM_CR2_OIS1N_DISABLE                     0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR2_OIS1N_ENABLE                      BIT(9)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr2 ois1n bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ois1n bit.
  * \retval 1 This is a valid ois1n bit.
  */
#define IS_ADV_TIMER_TIM_CR2_OIS1N(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CR2_OIS1N_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CR2_OIS1N_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR2_OIS2                         BIT(10)                                      /*!< Output Idle state 2 (OC2 output) - 1'b0:  disable:OC1=0 (after a dead-time if OC1N is implemented) when MOE=0 - 1'b1:  enable:OC1=1 (after a dead-time if OC1N is implemented) when MOE=0 */
#define ADV_TIMER_TIM_CR2_OIS2_OFS                     10U                                          /*!< ADV TIMER TIM CR2: OIS2 Bit Offset */
#define ADV_TIMER_TIM_CR2_OIS2_VAL(regval)                 (BIT(10) & ((uint32_t)(regval) << 10))        /*!< ADV TIMER TIM CR2: OIS2 Bit Value */  
#define ADV_TIMER_TIM_CR2_OIS2_DISABLE                      0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR2_OIS2_ENABLE                       BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr2 ois2 bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ois2 bit.
  * \retval 1 This is a valid ois2 bit.
  */
#define IS_ADV_TIMER_TIM_CR2_OIS2(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CR2_OIS2_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CR2_OIS2_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR2_OIS2N                        BIT(11)                                      /*!< Output Idle state 2 (OC2N output) - 1'b0:  disable: OC1N=0 after a dead-time when MOE=0 - 1'b1:  enable: OC1N=1 after a dead-time when MOE=0 */
#define ADV_TIMER_TIM_CR2_OIS2N_OFS                    11U                                          /*!< ADV TIMER TIM CR2: OIS2N Bit Offset */
#define ADV_TIMER_TIM_CR2_OIS2N_VAL(regval)                (BIT(11) & ((uint32_t)(regval) << 11))        /*!< ADV TIMER TIM CR2: OIS2N Bit Value */  
#define ADV_TIMER_TIM_CR2_OIS2N_DISABLE                     0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR2_OIS2N_ENABLE                      BIT(11)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr2 ois2n bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ois2n bit.
  * \retval 1 This is a valid ois2n bit.
  */
#define IS_ADV_TIMER_TIM_CR2_OIS2N(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CR2_OIS2N_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CR2_OIS2N_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR2_OIS3                         BIT(12)                                      /*!< Output Idle state 3 (OC3 output) - 1'b0:  disable:OC1=0 (after a dead-time if OC1N is implemented) when MOE=0 - 1'b1:  enable:OC1=1 (after a dead-time if OC1N is implemented) when MOE=0 */
#define ADV_TIMER_TIM_CR2_OIS3_OFS                     12U                                          /*!< ADV TIMER TIM CR2: OIS3 Bit Offset */
#define ADV_TIMER_TIM_CR2_OIS3_VAL(regval)                 (BIT(12) & ((uint32_t)(regval) << 12))        /*!< ADV TIMER TIM CR2: OIS3 Bit Value */  
#define ADV_TIMER_TIM_CR2_OIS3_DISABLE                      0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR2_OIS3_ENABLE                       BIT(12)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr2 ois3 bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ois3 bit.
  * \retval 1 This is a valid ois3 bit.
  */
#define IS_ADV_TIMER_TIM_CR2_OIS3(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CR2_OIS3_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CR2_OIS3_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR2_OIS3N                        BIT(13)                                      /*!< Output Idle state 3 (OC3N output) - 1'b0:  disable: OC1N=0 after a dead-time when MOE=0 - 1'b1:  enable: OC1N=1 after a dead-time when MOE=0 */
#define ADV_TIMER_TIM_CR2_OIS3N_OFS                    13U                                          /*!< ADV TIMER TIM CR2: OIS3N Bit Offset */
#define ADV_TIMER_TIM_CR2_OIS3N_VAL(regval)                (BIT(13) & ((uint32_t)(regval) << 13))        /*!< ADV TIMER TIM CR2: OIS3N Bit Value */  
#define ADV_TIMER_TIM_CR2_OIS3N_DISABLE                     0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR2_OIS3N_ENABLE                      BIT(13)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr2 ois3n bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ois3n bit.
  * \retval 1 This is a valid ois3n bit.
  */
#define IS_ADV_TIMER_TIM_CR2_OIS3N(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CR2_OIS3N_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CR2_OIS3N_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CR2_OIS4                         BIT(14)                                      /*!< Output Idle state 4 (OC4 output) - 1'b0:  disable:OC1=0 (after a dead-time if OC1N is implemented) when MOE=0 - 1'b1:  enable:OC1=1 (after a dead-time if OC1N is implemented) when MOE=0 */
#define ADV_TIMER_TIM_CR2_OIS4_OFS                     14U                                          /*!< ADV TIMER TIM CR2: OIS4 Bit Offset */
#define ADV_TIMER_TIM_CR2_OIS4_VAL(regval)                 (BIT(14) & ((uint32_t)(regval) << 14))        /*!< ADV TIMER TIM CR2: OIS4 Bit Value */  
#define ADV_TIMER_TIM_CR2_OIS4_DISABLE                      0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_CR2_OIS4_ENABLE                       BIT(14)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_cr2 ois4 bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ois4 bit.
  * \retval 1 This is a valid ois4 bit.
  */
#define IS_ADV_TIMER_TIM_CR2_OIS4(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CR2_OIS4_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CR2_OIS4_ENABLE              )  \
                                                 )

 /* ===== ADV TIMER TIM_SMCR Register definition ===== */
#define ADV_TIMER_TIM_SMCR_SMS_MASK                     BITS(0,3)                                   /*!< ADV TIMER TIM SMCR: SMS Bit Mask */  
#define ADV_TIMER_TIM_SMCR_SMS_OFS                      0U                                          /*!< ADV TIMER TIM SMCR: SMS Bit Offset */
#define ADV_TIMER_TIM_SMCR_SMS(regval)                  (BITS(0,3) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM SMCR: SMS Bit Value */  
#define ADV_TIMER_TIM_SMCR_SMS_INTERNAL_CLOCK                ADV_TIMER_TIM_SMCR_SMS(0)                                              /*!< INTERNAL_CLOCK */
#define ADV_TIMER_TIM_SMCR_SMS_ENCODER1                      ADV_TIMER_TIM_SMCR_SMS(1)                                              /*!< ENCODER1 */
#define ADV_TIMER_TIM_SMCR_SMS_ENCODER2                      ADV_TIMER_TIM_SMCR_SMS(2)                                              /*!< ENCODER2 */
#define ADV_TIMER_TIM_SMCR_SMS_ENCODER3                      ADV_TIMER_TIM_SMCR_SMS(3)                                              /*!< ENCODER3 */
#define ADV_TIMER_TIM_SMCR_SMS_RESET                         ADV_TIMER_TIM_SMCR_SMS(4)                                              /*!< RESET */
#define ADV_TIMER_TIM_SMCR_SMS_GATED                         ADV_TIMER_TIM_SMCR_SMS(5)                                              /*!< GATED */
#define ADV_TIMER_TIM_SMCR_SMS_TRIGGER                       ADV_TIMER_TIM_SMCR_SMS(6)                                              /*!< TRIGGER */
#define ADV_TIMER_TIM_SMCR_SMS_EXTERNAL_CLOCK                ADV_TIMER_TIM_SMCR_SMS(7)                                              /*!< EXTERNAL_CLOCK */

/**
  * \brief Check the ADV_TIMER tim_smcr sms bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid sms bits.
  * \retval 1 This is a valid sms bits.
  */
#define IS_ADV_TIMER_TIM_SMCR_SMS(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_SMCR_SMS_INTERNAL_CLOCK      ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_SMS_ENCODER1            ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_SMS_ENCODER2            ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_SMS_ENCODER3            ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_SMS_RESET               ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_SMS_GATED               ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_SMS_TRIGGER             ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_SMS_EXTERNAL_CLOCK      )  \
                                                 )

#define ADV_TIMER_TIM_SMCR_TS_MASK                      BITS(4,8)                                   /*!< ADV TIMER TIM SMCR: TS Bit Mask */  
#define ADV_TIMER_TIM_SMCR_TS_OFS                       4U                                          /*!< ADV TIMER TIM SMCR: TS Bit Offset */
#define ADV_TIMER_TIM_SMCR_TS(regval)                   (BITS(4,8) & ((uint32_t)(regval) << 4))        /*!< ADV TIMER TIM SMCR: TS Bit Value */  
#define ADV_TIMER_TIM_SMCR_TS_ITR0                           ADV_TIMER_TIM_SMCR_TS(0)                                              /*!< ITR0 */
#define ADV_TIMER_TIM_SMCR_TS_ITR1                           ADV_TIMER_TIM_SMCR_TS(1)                                              /*!< ITR1 */
#define ADV_TIMER_TIM_SMCR_TS_ITR2                           ADV_TIMER_TIM_SMCR_TS(2)                                              /*!< ITR2 */
#define ADV_TIMER_TIM_SMCR_TS_ITR3                           ADV_TIMER_TIM_SMCR_TS(3)                                              /*!< ITR3 */
#define ADV_TIMER_TIM_SMCR_TS_TI1F_ED                        ADV_TIMER_TIM_SMCR_TS(4)                                              /*!< TI1F_ED */
#define ADV_TIMER_TIM_SMCR_TS_TI1FP1                         ADV_TIMER_TIM_SMCR_TS(5)                                              /*!< TI1FP1 */
#define ADV_TIMER_TIM_SMCR_TS_TI2FP2                         ADV_TIMER_TIM_SMCR_TS(6)                                              /*!< TI2FP2 */
#define ADV_TIMER_TIM_SMCR_TS_ETRF                           ADV_TIMER_TIM_SMCR_TS(7)                                              /*!< ETRF */
#define ADV_TIMER_TIM_SMCR_TS_ITR4                           ADV_TIMER_TIM_SMCR_TS(12)                                              /*!< ITR4 */
#define ADV_TIMER_TIM_SMCR_TS_ITR5                           ADV_TIMER_TIM_SMCR_TS(13)                                              /*!< ITR5 */
#define ADV_TIMER_TIM_SMCR_TS_ITR6                           ADV_TIMER_TIM_SMCR_TS(14)                                              /*!< ITR6 */
#define ADV_TIMER_TIM_SMCR_TS_ITR7                           ADV_TIMER_TIM_SMCR_TS(15)                                              /*!< ITR7 */
#define ADV_TIMER_TIM_SMCR_TS_ITR8                           ADV_TIMER_TIM_SMCR_TS(20)                                              /*!< ITR8 */
#define ADV_TIMER_TIM_SMCR_TS_ITR9                           ADV_TIMER_TIM_SMCR_TS(21)                                              /*!< ITR9 */
#define ADV_TIMER_TIM_SMCR_TS_ITR10                          ADV_TIMER_TIM_SMCR_TS(22)                                              /*!< ITR10 */
#define ADV_TIMER_TIM_SMCR_TS_ITR11                          ADV_TIMER_TIM_SMCR_TS(23)                                              /*!< ITR11 */
#define ADV_TIMER_TIM_SMCR_TS_ITR12                          ADV_TIMER_TIM_SMCR_TS(28)                                              /*!< ITR12 */
#define ADV_TIMER_TIM_SMCR_TS_ITR13                          ADV_TIMER_TIM_SMCR_TS(29)                                              /*!< ITR13 */
#define ADV_TIMER_TIM_SMCR_TS_ITR14                          ADV_TIMER_TIM_SMCR_TS(30)                                              /*!< ITR14 */
#define ADV_TIMER_TIM_SMCR_TS_ITR15                          ADV_TIMER_TIM_SMCR_TS(31)                                              /*!< ITR15 */

/**
  * \brief Check the ADV_TIMER tim_smcr ts bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid ts bits.
  * \retval 1 This is a valid ts bits.
  */
#define IS_ADV_TIMER_TIM_SMCR_TS(regval)                     (\
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR0                ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR1                ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR2                ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR3                ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_TI1F_ED             ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_TI1FP1              ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_TI2FP2              ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ETRF                ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR4                ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR5                ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR6                ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR7                ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR8                ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR9                ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR10               ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR11               ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR12               ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR13               ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR14               ) || \
                                               ((regval) == ADV_TIMER_TIM_SMCR_TS_ITR15               )  \
                                                 )

#define ADV_TIMER_TIM_SMCR_MSM                          BIT(9)                                      /*!< Master/slave mode - 1'b0:  disable: No action - 1'b1:  enable: The effect of an event on the trigger input (TRGI) is delayed to allow a perfect synchronization between the current timer and its slaves (through TRGO). It is useful if we want to synchronize several timers on a single external event. */
#define ADV_TIMER_TIM_SMCR_MSM_OFS                      9U                                          /*!< ADV TIMER TIM SMCR: MSM Bit Offset */
#define ADV_TIMER_TIM_SMCR_MSM_VAL(regval)                  (BIT(9) & ((uint32_t)(regval) << 9))        /*!< ADV TIMER TIM SMCR: MSM Bit Value */  
#define ADV_TIMER_TIM_SMCR_MSM_DISABLE                       0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_SMCR_MSM_ENABLE                        BIT(9)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_smcr msm bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid msm bit.
  * \retval 1 This is a valid msm bit.
  */
#define IS_ADV_TIMER_TIM_SMCR_MSM(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_SMCR_MSM_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_MSM_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_SMCR_ETF_MASK                     BITS(10,13)                                   /*!< ADV TIMER TIM SMCR: ETF Bit Mask */  
#define ADV_TIMER_TIM_SMCR_ETF_OFS                      10U                                          /*!< ADV TIMER TIM SMCR: ETF Bit Offset */
#define ADV_TIMER_TIM_SMCR_ETF(regval)                  (BITS(10,13) & ((uint32_t)(regval) << 10))        /*!< ADV TIMER TIM SMCR: ETF Bit Value */  
#define ADV_TIMER_TIM_SMCR_ETF_NO_FILTER                     ADV_TIMER_TIM_SMCR_ETF(0)                                              /*!< NO_FILTER */
#define ADV_TIMER_TIM_SMCR_ETF_FL1_N2                        ADV_TIMER_TIM_SMCR_ETF(1)                                              /*!< FL1_N2 */
#define ADV_TIMER_TIM_SMCR_ETF_FL1_N4                        ADV_TIMER_TIM_SMCR_ETF(2)                                              /*!< FL1_N4 */
#define ADV_TIMER_TIM_SMCR_ETF_FL1_N8                        ADV_TIMER_TIM_SMCR_ETF(3)                                              /*!< FL1_N8 */
#define ADV_TIMER_TIM_SMCR_ETF_FL2_N6                        ADV_TIMER_TIM_SMCR_ETF(4)                                              /*!< FL2_N6 */
#define ADV_TIMER_TIM_SMCR_ETF_FL2_N8                        ADV_TIMER_TIM_SMCR_ETF(5)                                              /*!< FL2_N8 */
#define ADV_TIMER_TIM_SMCR_ETF_FL4_N6                        ADV_TIMER_TIM_SMCR_ETF(6)                                              /*!< FL4_N6 */
#define ADV_TIMER_TIM_SMCR_ETF_FL4_N8                        ADV_TIMER_TIM_SMCR_ETF(7)                                              /*!< FL4_N8 */
#define ADV_TIMER_TIM_SMCR_ETF_FL8_N6                        ADV_TIMER_TIM_SMCR_ETF(8)                                              /*!< FL8_N6 */
#define ADV_TIMER_TIM_SMCR_ETF_FL8_N8                        ADV_TIMER_TIM_SMCR_ETF(9)                                              /*!< FL8_N8 */
#define ADV_TIMER_TIM_SMCR_ETF_FL16_N5                       ADV_TIMER_TIM_SMCR_ETF(10)                                              /*!< FL16_N5 */
#define ADV_TIMER_TIM_SMCR_ETF_FL16_N6                       ADV_TIMER_TIM_SMCR_ETF(11)                                              /*!< FL16_N6 */
#define ADV_TIMER_TIM_SMCR_ETF_FL16_N8                       ADV_TIMER_TIM_SMCR_ETF(12)                                              /*!< FL16_N8 */
#define ADV_TIMER_TIM_SMCR_ETF_FL32_N5                       ADV_TIMER_TIM_SMCR_ETF(13)                                              /*!< FL32_N5 */
#define ADV_TIMER_TIM_SMCR_ETF_FL32_N6                       ADV_TIMER_TIM_SMCR_ETF(14)                                              /*!< FL32_N6 */
#define ADV_TIMER_TIM_SMCR_ETF_FL32_N8                       ADV_TIMER_TIM_SMCR_ETF(15)                                              /*!< FL32_N8 */

/**
  * \brief Check the ADV_TIMER tim_smcr etf bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid etf bits.
  * \retval 1 This is a valid etf bits.
  */
#define IS_ADV_TIMER_TIM_SMCR_ETF(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_NO_FILTER           ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL1_N2              ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL1_N4              ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL1_N8              ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL2_N6              ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL2_N8              ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL4_N6              ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL4_N8              ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL8_N6              ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL8_N8              ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL16_N5             ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL16_N6             ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL16_N8             ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL32_N5             ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL32_N6             ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETF_FL32_N8             )  \
                                                 )

#define ADV_TIMER_TIM_SMCR_ETPS_MASK                    BITS(14,15)                                   /*!< ADV TIMER TIM SMCR: ETPS Bit Mask */  
#define ADV_TIMER_TIM_SMCR_ETPS_OFS                     14U                                          /*!< ADV TIMER TIM SMCR: ETPS Bit Offset */
#define ADV_TIMER_TIM_SMCR_ETPS(regval)                 (BITS(14,15) & ((uint32_t)(regval) << 14))        /*!< ADV TIMER TIM SMCR: ETPS Bit Value */  
#define ADV_TIMER_TIM_SMCR_ETPS_PRESCALER                    ADV_TIMER_TIM_SMCR_ETPS(0)                                              /*!< PRESCALER */
#define ADV_TIMER_TIM_SMCR_ETPS_ETRP2                        ADV_TIMER_TIM_SMCR_ETPS(1)                                              /*!< ETRP2 */
#define ADV_TIMER_TIM_SMCR_ETPS_ETRP4                        ADV_TIMER_TIM_SMCR_ETPS(2)                                              /*!< ETRP4 */
#define ADV_TIMER_TIM_SMCR_ETPS_ETRP8                        ADV_TIMER_TIM_SMCR_ETPS(3)                                              /*!< ETRP8 */

/**
  * \brief Check the ADV_TIMER tim_smcr etps bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid etps bits.
  * \retval 1 This is a valid etps bits.
  */
#define IS_ADV_TIMER_TIM_SMCR_ETPS(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_SMCR_ETPS_PRESCALER           ) || \
                                             ((regval) == ADV_TIMER_TIM_SMCR_ETPS_ETRP2               ) || \
                                             ((regval) == ADV_TIMER_TIM_SMCR_ETPS_ETRP4               ) || \
                                             ((regval) == ADV_TIMER_TIM_SMCR_ETPS_ETRP8               )  \
                                                 )

#define ADV_TIMER_TIM_SMCR_ECE                          BIT(16)                                      /*!< External clock enable This bit enables External clock mode 2.  - 1'b0:  disable: External clock mode 2 disabled - 1'b1:  enable: External clock mode 2 enabled. The counter is clocked by any active edge on the ETRF signal. Note: 1: Setting the ECE bit has the same effect as selecting external clock mode 1 with  TRGI connected to ETRF (SMS=111 and TS=00111). 2: It is possible to simultaneously use external clock mode 2 with the following slave modes: reset mode, gated mode and trigger mode. Nevertheless, TRGI must not be connected to ETRF in this case (TS bits must not be 00111). 3: If external clock mode 1 and external clock mode 2 are enabled at the same time, the external clock input is ETRF */
#define ADV_TIMER_TIM_SMCR_ECE_OFS                      16U                                          /*!< ADV TIMER TIM SMCR: ECE Bit Offset */
#define ADV_TIMER_TIM_SMCR_ECE_VAL(regval)                  (BIT(16) & ((uint32_t)(regval) << 16))        /*!< ADV TIMER TIM SMCR: ECE Bit Value */  
#define ADV_TIMER_TIM_SMCR_ECE_DISABLE                       0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_SMCR_ECE_ENABLE                        BIT(16)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_smcr ece bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ece bit.
  * \retval 1 This is a valid ece bit.
  */
#define IS_ADV_TIMER_TIM_SMCR_ECE(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_SMCR_ECE_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ECE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_SMCR_ETP                          BIT(17)                                      /*!< External trigger polarity This bit selects whether ETR or ETR is used for trigger operations  - 1'b0:  disable: ETR is non-inverted, active at high level or rising edge. - 1'b1:  enable: ETR is inverted, active at low level or falling edge. */
#define ADV_TIMER_TIM_SMCR_ETP_OFS                      17U                                          /*!< ADV TIMER TIM SMCR: ETP Bit Offset */
#define ADV_TIMER_TIM_SMCR_ETP_VAL(regval)                  (BIT(17) & ((uint32_t)(regval) << 17))        /*!< ADV TIMER TIM SMCR: ETP Bit Value */  
#define ADV_TIMER_TIM_SMCR_ETP_DISABLE                       0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_SMCR_ETP_ENABLE                        BIT(17)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_smcr etp bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid etp bit.
  * \retval 1 This is a valid etp bit.
  */
#define IS_ADV_TIMER_TIM_SMCR_ETP(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETP_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_SMCR_ETP_ENABLE              )  \
                                                 )

 /* ===== ADV TIMER TIM_DIER Register definition ===== */
#define ADV_TIMER_TIM_DIER_UIE                          BIT(0)                                      /*!< Update interrupt enable - 1'b0:  disable: Update interrupt disabled - 1'b1:  enable: Update interrupt enabled */
#define ADV_TIMER_TIM_DIER_UIE_OFS                      0U                                          /*!< ADV TIMER TIM DIER: UIE Bit Offset */
#define ADV_TIMER_TIM_DIER_UIE_VAL(regval)                  (BIT(0) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM DIER: UIE Bit Value */  
#define ADV_TIMER_TIM_DIER_UIE_DISABLE                       0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_UIE_ENABLE                        BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier uie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid uie bit.
  * \retval 1 This is a valid uie bit.
  */
#define IS_ADV_TIMER_TIM_DIER_UIE(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_DIER_UIE_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_DIER_UIE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_CC1IE                        BIT(1)                                      /*!< Capture/Compare 1 interrupt enable - 1'b0:  disable:CC1 interrupt disabled - 1'b1:  enable: CC1 interrupt enabled */
#define ADV_TIMER_TIM_DIER_CC1IE_OFS                    1U                                          /*!< ADV TIMER TIM DIER: CC1IE Bit Offset */
#define ADV_TIMER_TIM_DIER_CC1IE_VAL(regval)                (BIT(1) & ((uint32_t)(regval) << 1))        /*!< ADV TIMER TIM DIER: CC1IE Bit Value */  
#define ADV_TIMER_TIM_DIER_CC1IE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_CC1IE_ENABLE                      BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier cc1ie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc1ie bit.
  * \retval 1 This is a valid cc1ie bit.
  */
#define IS_ADV_TIMER_TIM_DIER_CC1IE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_DIER_CC1IE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_DIER_CC1IE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_CC2IE                        BIT(2)                                      /*!< Capture/Compare 2 interrupt enable - 1'b0:  disable:CC2 interrupt disabled - 1'b1:  enable: CC2 interrupt enabled */
#define ADV_TIMER_TIM_DIER_CC2IE_OFS                    2U                                          /*!< ADV TIMER TIM DIER: CC2IE Bit Offset */
#define ADV_TIMER_TIM_DIER_CC2IE_VAL(regval)                (BIT(2) & ((uint32_t)(regval) << 2))        /*!< ADV TIMER TIM DIER: CC2IE Bit Value */  
#define ADV_TIMER_TIM_DIER_CC2IE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_CC2IE_ENABLE                      BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier cc2ie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc2ie bit.
  * \retval 1 This is a valid cc2ie bit.
  */
#define IS_ADV_TIMER_TIM_DIER_CC2IE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_DIER_CC2IE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_DIER_CC2IE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_CC3IE                        BIT(3)                                      /*!< Capture/Compare 3 interrupt enable - 1'b0:  disable:CC3 interrupt disabled - 1'b1:  enable: CC3 interrupt enabled */
#define ADV_TIMER_TIM_DIER_CC3IE_OFS                    3U                                          /*!< ADV TIMER TIM DIER: CC3IE Bit Offset */
#define ADV_TIMER_TIM_DIER_CC3IE_VAL(regval)                (BIT(3) & ((uint32_t)(regval) << 3))        /*!< ADV TIMER TIM DIER: CC3IE Bit Value */  
#define ADV_TIMER_TIM_DIER_CC3IE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_CC3IE_ENABLE                      BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier cc3ie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc3ie bit.
  * \retval 1 This is a valid cc3ie bit.
  */
#define IS_ADV_TIMER_TIM_DIER_CC3IE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_DIER_CC3IE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_DIER_CC3IE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_CC4IE                        BIT(4)                                      /*!< Capture/Compare 4 interrupt enable - 1'b0:  disable:CC4 interrupt disabled - 1'b1:  enable: CC4 interrupt enabled */
#define ADV_TIMER_TIM_DIER_CC4IE_OFS                    4U                                          /*!< ADV TIMER TIM DIER: CC4IE Bit Offset */
#define ADV_TIMER_TIM_DIER_CC4IE_VAL(regval)                (BIT(4) & ((uint32_t)(regval) << 4))        /*!< ADV TIMER TIM DIER: CC4IE Bit Value */  
#define ADV_TIMER_TIM_DIER_CC4IE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_CC4IE_ENABLE                      BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier cc4ie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc4ie bit.
  * \retval 1 This is a valid cc4ie bit.
  */
#define IS_ADV_TIMER_TIM_DIER_CC4IE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_DIER_CC4IE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_DIER_CC4IE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_COMIE                        BIT(5)                                      /*!< COM interrupt enable - 1'b0:  disable: COM interrupt disabled - 1'b1:  enable: COM interrupt enabled */
#define ADV_TIMER_TIM_DIER_COMIE_OFS                    5U                                          /*!< ADV TIMER TIM DIER: COMIE Bit Offset */
#define ADV_TIMER_TIM_DIER_COMIE_VAL(regval)                (BIT(5) & ((uint32_t)(regval) << 5))        /*!< ADV TIMER TIM DIER: COMIE Bit Value */  
#define ADV_TIMER_TIM_DIER_COMIE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_COMIE_ENABLE                      BIT(5)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier comie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid comie bit.
  * \retval 1 This is a valid comie bit.
  */
#define IS_ADV_TIMER_TIM_DIER_COMIE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_DIER_COMIE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_DIER_COMIE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_TIE                          BIT(6)                                      /*!< Trigger interrupt enable - 1'b0:  disable:  Trigger interrupt disabled - 1'b1:  enable:  Trigger interrupt enabled */
#define ADV_TIMER_TIM_DIER_TIE_OFS                      6U                                          /*!< ADV TIMER TIM DIER: TIE Bit Offset */
#define ADV_TIMER_TIM_DIER_TIE_VAL(regval)                  (BIT(6) & ((uint32_t)(regval) << 6))        /*!< ADV TIMER TIM DIER: TIE Bit Value */  
#define ADV_TIMER_TIM_DIER_TIE_DISABLE                       0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_TIE_ENABLE                        BIT(6)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier tie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tie bit.
  * \retval 1 This is a valid tie bit.
  */
#define IS_ADV_TIMER_TIM_DIER_TIE(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_DIER_TIE_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_DIER_TIE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_BIE                          BIT(7)                                      /*!< Break interrupt enable - 1'b0:  disable:  Break interrupt disabled - 1'b1:  enable:  Break interrupt enabled */
#define ADV_TIMER_TIM_DIER_BIE_OFS                      7U                                          /*!< ADV TIMER TIM DIER: BIE Bit Offset */
#define ADV_TIMER_TIM_DIER_BIE_VAL(regval)                  (BIT(7) & ((uint32_t)(regval) << 7))        /*!< ADV TIMER TIM DIER: BIE Bit Value */  
#define ADV_TIMER_TIM_DIER_BIE_DISABLE                       0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_BIE_ENABLE                        BIT(7)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier bie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bie bit.
  * \retval 1 This is a valid bie bit.
  */
#define IS_ADV_TIMER_TIM_DIER_BIE(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_DIER_BIE_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_DIER_BIE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_UDE                          BIT(8)                                      /*!< Update DMA request enable - 1'b0:  disable:  Update DMA request disabled - 1'b1:  enable:  Update DMA request enabled */
#define ADV_TIMER_TIM_DIER_UDE_OFS                      8U                                          /*!< ADV TIMER TIM DIER: UDE Bit Offset */
#define ADV_TIMER_TIM_DIER_UDE_VAL(regval)                  (BIT(8) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM DIER: UDE Bit Value */  
#define ADV_TIMER_TIM_DIER_UDE_DISABLE                       0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_UDE_ENABLE                        BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier ude bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ude bit.
  * \retval 1 This is a valid ude bit.
  */
#define IS_ADV_TIMER_TIM_DIER_UDE(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_DIER_UDE_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_DIER_UDE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_CC1DE                        BIT(9)                                      /*!< Capture/Compare 1 DMA request enable - 1'b0:  disable:  CC1 DMA request disabled - 1'b1:  enable: CC1 DMA request enabled */
#define ADV_TIMER_TIM_DIER_CC1DE_OFS                    9U                                          /*!< ADV TIMER TIM DIER: CC1DE Bit Offset */
#define ADV_TIMER_TIM_DIER_CC1DE_VAL(regval)                (BIT(9) & ((uint32_t)(regval) << 9))        /*!< ADV TIMER TIM DIER: CC1DE Bit Value */  
#define ADV_TIMER_TIM_DIER_CC1DE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_CC1DE_ENABLE                      BIT(9)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier cc1de bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc1de bit.
  * \retval 1 This is a valid cc1de bit.
  */
#define IS_ADV_TIMER_TIM_DIER_CC1DE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_DIER_CC1DE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_DIER_CC1DE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_CC2DE                        BIT(10)                                      /*!< Capture/Compare 2 DMA request enable - 1'b0:  disable:  CC2 DMA request disabled - 1'b1:  enable:  CC2 DMA request enabled */
#define ADV_TIMER_TIM_DIER_CC2DE_OFS                    10U                                          /*!< ADV TIMER TIM DIER: CC2DE Bit Offset */
#define ADV_TIMER_TIM_DIER_CC2DE_VAL(regval)                (BIT(10) & ((uint32_t)(regval) << 10))        /*!< ADV TIMER TIM DIER: CC2DE Bit Value */  
#define ADV_TIMER_TIM_DIER_CC2DE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_CC2DE_ENABLE                      BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier cc2de bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc2de bit.
  * \retval 1 This is a valid cc2de bit.
  */
#define IS_ADV_TIMER_TIM_DIER_CC2DE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_DIER_CC2DE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_DIER_CC2DE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_CC3DE                        BIT(11)                                      /*!< Capture/Compare 3 DMA request enable - 1'b0:  disable:  CC3 DMA request disabled - 1'b1:  enable:  CC3 DMA request enabled */
#define ADV_TIMER_TIM_DIER_CC3DE_OFS                    11U                                          /*!< ADV TIMER TIM DIER: CC3DE Bit Offset */
#define ADV_TIMER_TIM_DIER_CC3DE_VAL(regval)                (BIT(11) & ((uint32_t)(regval) << 11))        /*!< ADV TIMER TIM DIER: CC3DE Bit Value */  
#define ADV_TIMER_TIM_DIER_CC3DE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_CC3DE_ENABLE                      BIT(11)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier cc3de bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc3de bit.
  * \retval 1 This is a valid cc3de bit.
  */
#define IS_ADV_TIMER_TIM_DIER_CC3DE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_DIER_CC3DE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_DIER_CC3DE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_CC4DE                        BIT(12)                                      /*!< Capture/Compare 4 DMA request enable - 1'b0:  disable:CC4 DMA request disabled - 1'b1:  enable:CC4 DMA request enabled */
#define ADV_TIMER_TIM_DIER_CC4DE_OFS                    12U                                          /*!< ADV TIMER TIM DIER: CC4DE Bit Offset */
#define ADV_TIMER_TIM_DIER_CC4DE_VAL(regval)                (BIT(12) & ((uint32_t)(regval) << 12))        /*!< ADV TIMER TIM DIER: CC4DE Bit Value */  
#define ADV_TIMER_TIM_DIER_CC4DE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_CC4DE_ENABLE                      BIT(12)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier cc4de bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc4de bit.
  * \retval 1 This is a valid cc4de bit.
  */
#define IS_ADV_TIMER_TIM_DIER_CC4DE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_DIER_CC4DE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_DIER_CC4DE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_COMDE                        BIT(13)                                      /*!< COM DMA request enable - 1'b0:  disable: COM DMA request disabled - 1'b1:  enable: COM DMA request enabled */
#define ADV_TIMER_TIM_DIER_COMDE_OFS                    13U                                          /*!< ADV TIMER TIM DIER: COMDE Bit Offset */
#define ADV_TIMER_TIM_DIER_COMDE_VAL(regval)                (BIT(13) & ((uint32_t)(regval) << 13))        /*!< ADV TIMER TIM DIER: COMDE Bit Value */  
#define ADV_TIMER_TIM_DIER_COMDE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_COMDE_ENABLE                      BIT(13)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier comde bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid comde bit.
  * \retval 1 This is a valid comde bit.
  */
#define IS_ADV_TIMER_TIM_DIER_COMDE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_DIER_COMDE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_DIER_COMDE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_DIER_TDE                          BIT(14)                                      /*!< Trigger DMA request enable - 1'b0:  disable:Trigger DMA request disabled - 1'b1:  enable: Trigger DMA request enabled */
#define ADV_TIMER_TIM_DIER_TDE_OFS                      14U                                          /*!< ADV TIMER TIM DIER: TDE Bit Offset */
#define ADV_TIMER_TIM_DIER_TDE_VAL(regval)                  (BIT(14) & ((uint32_t)(regval) << 14))        /*!< ADV TIMER TIM DIER: TDE Bit Value */  
#define ADV_TIMER_TIM_DIER_TDE_DISABLE                       0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_DIER_TDE_ENABLE                        BIT(14)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_dier tde bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tde bit.
  * \retval 1 This is a valid tde bit.
  */
#define IS_ADV_TIMER_TIM_DIER_TDE(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_DIER_TDE_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_DIER_TDE_ENABLE              )  \
                                                 )

 /* ===== ADV TIMER TIM_SR Register definition ===== */
#define ADV_TIMER_TIM_SR_UIF                          BIT(0)                                      /*!< Update interrupt flag This bit is set by hardware on an update event. It is cleared by software.  - 0: No update occurred. - 1: Update interrupt pending. This bit is set by hardware when the registers are updated: - At overflow or underflow regarding the repetition counter value (update if repetition counter = 0) and if the UDIS=0 in the TIMx_CR1 register. - When CNT is reinitialized by software using the UG bit in TIMx_EGR register, if URS=0 and UDIS=0 in the TIMx_CR1 register. - When CNT is reinitialized by a trigger event,if URS=0 and UDIS=0 in the TIMx_CR1 register. note: this bit can read clear or write 0 to clear(RC / W0C) */
#define ADV_TIMER_TIM_SR_UIF_OFS                      0U                                          /*!< ADV TIMER TIM SR: UIF Bit Offset */
#define ADV_TIMER_TIM_SR_UIF_VAL(regval)                  (BIT(0) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM SR: UIF Bit Value */  
#define ADV_TIMER_TIM_SR_CC1IF                        BIT(1)                                      /*!< Capture/Compare 1 interrupt flag If channel CC1 is configured as output: This flag is set by hardware when the counter matches the compare value, with some exception in center-aligned mode (refer to the CMS bits in the TIMx_CR1 register description). It is cleared by software.  - 0: No match. - 1: The content of the counter TIMx_CNT matches the content of the TIMx_CCR1 register.  When the contents of TIMx_CCR1 are greater than the contents of TIMx_ARR, the CC1IF bit goes high on the counter overflow (in upcounting and up/down-counting modes) or underflow (in downcounting mode) If channel CC1 is configured as input: This bit is set by hardware on a capture. It is cleared by software or by reading the TIMx_CCR1 register. - 0: No input capture occurred - 1: The counter value has been captured in TIMx_CCR1 register (An edge has been detected on IC1 which matches the selected polarity) */
#define ADV_TIMER_TIM_SR_CC1IF_OFS                    1U                                          /*!< ADV TIMER TIM SR: CC1IF Bit Offset */
#define ADV_TIMER_TIM_SR_CC1IF_VAL(regval)                (BIT(1) & ((uint32_t)(regval) << 1))        /*!< ADV TIMER TIM SR: CC1IF Bit Value */  
#define ADV_TIMER_TIM_SR_CC2IF                        BIT(2)                                      /*!< Capture/Compare 2 interrupt flag Refer to CC1IF description */
#define ADV_TIMER_TIM_SR_CC2IF_OFS                    2U                                          /*!< ADV TIMER TIM SR: CC2IF Bit Offset */
#define ADV_TIMER_TIM_SR_CC2IF_VAL(regval)                (BIT(2) & ((uint32_t)(regval) << 2))        /*!< ADV TIMER TIM SR: CC2IF Bit Value */  
#define ADV_TIMER_TIM_SR_CC3IF                        BIT(3)                                      /*!< Capture/Compare 3 interrupt flag Refer to CC1IF description */
#define ADV_TIMER_TIM_SR_CC3IF_OFS                    3U                                          /*!< ADV TIMER TIM SR: CC3IF Bit Offset */
#define ADV_TIMER_TIM_SR_CC3IF_VAL(regval)                (BIT(3) & ((uint32_t)(regval) << 3))        /*!< ADV TIMER TIM SR: CC3IF Bit Value */  
#define ADV_TIMER_TIM_SR_CC4IF                        BIT(4)                                      /*!< Capture/Compare 4 interrupt flag Refer to CC1IF description */
#define ADV_TIMER_TIM_SR_CC4IF_OFS                    4U                                          /*!< ADV TIMER TIM SR: CC4IF Bit Offset */
#define ADV_TIMER_TIM_SR_CC4IF_VAL(regval)                (BIT(4) & ((uint32_t)(regval) << 4))        /*!< ADV TIMER TIM SR: CC4IF Bit Value */  
#define ADV_TIMER_TIM_SR_COMIF                        BIT(5)                                      /*!< COM interrupt flag This flag is set by hardware on COM event (when Capture/compare Control bits - CCxE, CCxNE, OCxM - have been updated). It is cleared by software. - 0: No COM event occurred. - 1: COM interrupt pending. */
#define ADV_TIMER_TIM_SR_COMIF_OFS                    5U                                          /*!< ADV TIMER TIM SR: COMIF Bit Offset */
#define ADV_TIMER_TIM_SR_COMIF_VAL(regval)                (BIT(5) & ((uint32_t)(regval) << 5))        /*!< ADV TIMER TIM SR: COMIF Bit Value */  
#define ADV_TIMER_TIM_SR_TIF                          BIT(6)                                      /*!< Trigger interrupt flag This flag is set by hardware on trigger event (active edge detected on TRGI input when the slave mode controller is enabled in all modes but gated mode. It is set when the counter starts or stops when gated mode is selected. It is cleared by software. - 0: No trigger event occurred. - 1: Trigger interrupt pending. */
#define ADV_TIMER_TIM_SR_TIF_OFS                      6U                                          /*!< ADV TIMER TIM SR: TIF Bit Offset */
#define ADV_TIMER_TIM_SR_TIF_VAL(regval)                  (BIT(6) & ((uint32_t)(regval) << 6))        /*!< ADV TIMER TIM SR: TIF Bit Value */  
#define ADV_TIMER_TIM_SR_BIF                          BIT(7)                                      /*!< Break interrupt flag This flag is set by hardware as soon as the break input goes active. It can be cleared by software if the break input is not active. - 0: No break event occurred. - 1: An active level has been detected on the break input. An interrupt is generated if BIE=1 in the TIM_DIER register. */
#define ADV_TIMER_TIM_SR_BIF_OFS                      7U                                          /*!< ADV TIMER TIM SR: BIF Bit Offset */
#define ADV_TIMER_TIM_SR_BIF_VAL(regval)                  (BIT(7) & ((uint32_t)(regval) << 7))        /*!< ADV TIMER TIM SR: BIF Bit Value */  
#define ADV_TIMER_TIM_SR_B2IF                         BIT(8)                                      /*!< Break 2 interrupt flag  This flag is set by hardware as soon as the break 2 input goes active. It can be cleared by software if the break 2 input is not active. - 0: No break event occurred. - 1: An active level has been detected on the break 2 input. An interrupt is generated if BIE=1 in the TIM_DIER register. */
#define ADV_TIMER_TIM_SR_B2IF_OFS                     8U                                          /*!< ADV TIMER TIM SR: B2IF Bit Offset */
#define ADV_TIMER_TIM_SR_B2IF_VAL(regval)                 (BIT(8) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM SR: B2IF Bit Value */  
#define ADV_TIMER_TIM_SR_CC1OF                        BIT(9)                                      /*!< Capture/Compare 1 overcapture flag This flag is set by hardware only when the corresponding channel is configured in input capture mode. It is cleared by software by writing it to '0'.  - 0: No overcapture has been detected. - 1: The counter value has been captured in TIMx_CCR1 register while CC1IF flag was already set */
#define ADV_TIMER_TIM_SR_CC1OF_OFS                    9U                                          /*!< ADV TIMER TIM SR: CC1OF Bit Offset */
#define ADV_TIMER_TIM_SR_CC1OF_VAL(regval)                (BIT(9) & ((uint32_t)(regval) << 9))        /*!< ADV TIMER TIM SR: CC1OF Bit Value */  
#define ADV_TIMER_TIM_SR_CC2OF                        BIT(10)                                      /*!< Capture/Compare 2 overcapture flag Refer to CC1OF description */
#define ADV_TIMER_TIM_SR_CC2OF_OFS                    10U                                          /*!< ADV TIMER TIM SR: CC2OF Bit Offset */
#define ADV_TIMER_TIM_SR_CC2OF_VAL(regval)                (BIT(10) & ((uint32_t)(regval) << 10))        /*!< ADV TIMER TIM SR: CC2OF Bit Value */  
#define ADV_TIMER_TIM_SR_CC3OF                        BIT(11)                                      /*!< Capture/Compare 3 overcapture flag Refer to CC1OF description */
#define ADV_TIMER_TIM_SR_CC3OF_OFS                    11U                                          /*!< ADV TIMER TIM SR: CC3OF Bit Offset */
#define ADV_TIMER_TIM_SR_CC3OF_VAL(regval)                (BIT(11) & ((uint32_t)(regval) << 11))        /*!< ADV TIMER TIM SR: CC3OF Bit Value */  
#define ADV_TIMER_TIM_SR_CC4OF                        BIT(12)                                      /*!< Capture/Compare 4 overcapture flag Refer to CC1OF description */
#define ADV_TIMER_TIM_SR_CC4OF_OFS                    12U                                          /*!< ADV TIMER TIM SR: CC4OF Bit Offset */
#define ADV_TIMER_TIM_SR_CC4OF_VAL(regval)                (BIT(12) & ((uint32_t)(regval) << 12))        /*!< ADV TIMER TIM SR: CC4OF Bit Value */  
#define ADV_TIMER_TIM_SR_SBIF                         BIT(13)                                      /*!< System Break interrupt flag This flag is set by hardware as soon as the system break input goes active. It can be cleared by software if the system break input is not active. This flag must be reset to re-start PWM operation. - 0: NO break event occurred. - 1: An active level has been detected on the system break input. An interrupt is generated if BIE=1 in the TIM_DIER register. */
#define ADV_TIMER_TIM_SR_SBIF_OFS                     13U                                          /*!< ADV TIMER TIM SR: SBIF Bit Offset */
#define ADV_TIMER_TIM_SR_SBIF_VAL(regval)                 (BIT(13) & ((uint32_t)(regval) << 13))        /*!< ADV TIMER TIM SR: SBIF Bit Value */  
 
 /* ===== ADV TIMER TIM_EGR Register definition ===== */
#define ADV_TIMER_TIM_EGR_UG                           BIT(0)                                      /*!< Update generation This bit can be set by software, it is automatically cleared by hardware. - 0: No action - 1: Reinitialize the counter and generates an update of the registers. Note that the prescaler counter is cleared too (anyway the prescaler ratio is not affected). The counter is cleared if the center-aligned mode is selected or if DIR=0 (upcounting), else it takes the auto-reload value (TIMx_ARR) if DIR=1 (downcounting). */
#define ADV_TIMER_TIM_EGR_UG_OFS                       0U                                          /*!< ADV TIMER TIM EGR: UG Bit Offset */
#define ADV_TIMER_TIM_EGR_UG_VAL(regval)                   (BIT(0) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM EGR: UG Bit Value */  
#define ADV_TIMER_TIM_EGR_CC1G                         BIT(1)                                      /*!< Capture/Compare 1 generation This bit is set by software in order to generate an event, it is automatically cleared by hardware.  0: No action 1: A capture/compare event is generated on channel 1: If channel CC1 is configured as output: CC1IF flag is set, Corresponding interrupt or DMA request is sent if enabled. If channel CC1 is configured as input: The current value of the counter is captured in TIMx_CCR1 register. The CC1IF flag is set, the corresponding interrupt or DMA request is sent if enabled. The CC1OF flag is set if the CC1IF flag was already high. */
#define ADV_TIMER_TIM_EGR_CC1G_OFS                     1U                                          /*!< ADV TIMER TIM EGR: CC1G Bit Offset */
#define ADV_TIMER_TIM_EGR_CC1G_VAL(regval)                 (BIT(1) & ((uint32_t)(regval) << 1))        /*!< ADV TIMER TIM EGR: CC1G Bit Value */  
#define ADV_TIMER_TIM_EGR_CC2G                         BIT(2)                                      /*!< Capture/Compare 2 generation Refer to CC1G description */
#define ADV_TIMER_TIM_EGR_CC2G_OFS                     2U                                          /*!< ADV TIMER TIM EGR: CC2G Bit Offset */
#define ADV_TIMER_TIM_EGR_CC2G_VAL(regval)                 (BIT(2) & ((uint32_t)(regval) << 2))        /*!< ADV TIMER TIM EGR: CC2G Bit Value */  
#define ADV_TIMER_TIM_EGR_CC3G                         BIT(3)                                      /*!< Capture/Compare 3 generation Refer to CC1G description */
#define ADV_TIMER_TIM_EGR_CC3G_OFS                     3U                                          /*!< ADV TIMER TIM EGR: CC3G Bit Offset */
#define ADV_TIMER_TIM_EGR_CC3G_VAL(regval)                 (BIT(3) & ((uint32_t)(regval) << 3))        /*!< ADV TIMER TIM EGR: CC3G Bit Value */  
#define ADV_TIMER_TIM_EGR_CC4G                         BIT(4)                                      /*!< Capture/Compare 4 generation Refer to CC1G description */
#define ADV_TIMER_TIM_EGR_CC4G_OFS                     4U                                          /*!< ADV TIMER TIM EGR: CC4G Bit Offset */
#define ADV_TIMER_TIM_EGR_CC4G_VAL(regval)                 (BIT(4) & ((uint32_t)(regval) << 4))        /*!< ADV TIMER TIM EGR: CC4G Bit Value */  
#define ADV_TIMER_TIM_EGR_COMG                         BIT(5)                                      /*!< Capture/Compare control update generation This bit can be set by software, it is automatically cleared by hardware - 0: No action - 1: When CCPC bit is set, it allows to update CCxE, CCxNE and OCxM bits */
#define ADV_TIMER_TIM_EGR_COMG_OFS                     5U                                          /*!< ADV TIMER TIM EGR: COMG Bit Offset */
#define ADV_TIMER_TIM_EGR_COMG_VAL(regval)                 (BIT(5) & ((uint32_t)(regval) << 5))        /*!< ADV TIMER TIM EGR: COMG Bit Value */  
#define ADV_TIMER_TIM_EGR_TG                           BIT(6)                                      /*!< Trigger generation This bit is set by software in order to generate an event, it is automatically cleared by hardware. - 0: No action - 1: The TIF flag is set in TIMx_SR register. Related interrupt or DMA transfer can occur if enabled. */
#define ADV_TIMER_TIM_EGR_TG_OFS                       6U                                          /*!< ADV TIMER TIM EGR: TG Bit Offset */
#define ADV_TIMER_TIM_EGR_TG_VAL(regval)                   (BIT(6) & ((uint32_t)(regval) << 6))        /*!< ADV TIMER TIM EGR: TG Bit Value */  
#define ADV_TIMER_TIM_EGR_BG                           BIT(7)                                      /*!< Break generation This bit is set by software in order to generate an event, it is automatically cleared by hardware. - 0: No action - 1: A break event is generated. MOE bit is cleared and BIF flag is set. Related interrupt can occur if enabled. */
#define ADV_TIMER_TIM_EGR_BG_OFS                       7U                                          /*!< ADV TIMER TIM EGR: BG Bit Offset */
#define ADV_TIMER_TIM_EGR_BG_VAL(regval)                   (BIT(7) & ((uint32_t)(regval) << 7))        /*!< ADV TIMER TIM EGR: BG Bit Value */  
#define ADV_TIMER_TIM_EGR_B2G                          BIT(8)                                      /*!< Break generation This bit is set by software in order to generate an event, it is automatically cleared by hardware. - 0: No action - 1: A break 2 event is generated. MOE bit is cleared and B2IF flag is set. Related interrupt can occur if enabled. */
#define ADV_TIMER_TIM_EGR_B2G_OFS                      8U                                          /*!< ADV TIMER TIM EGR: B2G Bit Offset */
#define ADV_TIMER_TIM_EGR_B2G_VAL(regval)                  (BIT(8) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM EGR: B2G Bit Value */  
 
 /* ===== ADV TIMER TIM_CCMR1 Register definition ===== */
#define ADV_TIMER_TIM_CCMR1_CC1S_MASK                    BITS(0,1)                                   /*!< ADV TIMER TIM CCMR1: CC1S Bit Mask */  
#define ADV_TIMER_TIM_CCMR1_CC1S_OFS                     0U                                          /*!< ADV TIMER TIM CCMR1: CC1S Bit Offset */
#define ADV_TIMER_TIM_CCMR1_CC1S(regval)                 (BITS(0,1) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM CCMR1: CC1S Bit Value */  
#define ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT                       ADV_TIMER_TIM_CCMR1_CC1S(0)                                             /*!< OUTPUT */
#define ADV_TIMER_TIM_CCMR1_CC1S_TI1                          ADV_TIMER_TIM_CCMR1_CC1S(1)                                             /*!< TI1 */
#define ADV_TIMER_TIM_CCMR1_CC1S_TI2                          ADV_TIMER_TIM_CCMR1_CC1S(2)                                             /*!< TI2 */
#define ADV_TIMER_TIM_CCMR1_CC1S_TRC                          ADV_TIMER_TIM_CCMR1_CC1S(3)                                             /*!< TRC */

/**
  * \brief Check the ADV_TIMER tim_ccmr1 cc1s bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid cc1s bits.
  * \retval 1 This is a valid cc1s bits.
  */
#define IS_ADV_TIMER_TIM_CCMR1_CC1S(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCMR1_CC1S_OUTPUT              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_CC1S_TI1                 ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_CC1S_TI2                 ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_CC1S_TRC                 )  \
                                                 )

#define ADV_TIMER_TIM_CCMR1_OC1PE                        BIT(2)                                      /*!< Output Compare 1 preload enable - 1'b0: disable: Preload register on TIM_CCR1 disabled. TIM_CCR1 can be written at anytime, the new value is taken in account immediately. - 1'b1: enable:Preload register on TIM_CCR1 enabled. Read/Write operations access the preload register. TIM_CCR1 preload value is loaded in the active register at each update event. */
#define ADV_TIMER_TIM_CCMR1_OC1PE_OFS                    2U                                          /*!< ADV TIMER TIM CCMR1: OC1PE Bit Offset */
#define ADV_TIMER_TIM_CCMR1_OC1PE_VAL(regval)                (BIT(2) & ((uint32_t)(regval) << 2))        /*!< ADV TIMER TIM CCMR1: OC1PE Bit Value */  
#define ADV_TIMER_TIM_CCMR1_OC1PE_DISABLE                     0x0UL                                           /*!< DISABLE */
#define ADV_TIMER_TIM_CCMR1_OC1PE_ENABLE                      BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccmr1 oc1pe bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid oc1pe bit.
  * \retval 1 This is a valid oc1pe bit.
  */
#define IS_ADV_TIMER_TIM_CCMR1_OC1PE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCMR1_OC1PE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CCMR1_OC1PE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCMR1_OC1M_MASK                    BITS(3,6)                                   /*!< ADV TIMER TIM CCMR1: OC1M Bit Mask */  
#define ADV_TIMER_TIM_CCMR1_OC1M_OFS                     3U                                          /*!< ADV TIMER TIM CCMR1: OC1M Bit Offset */
#define ADV_TIMER_TIM_CCMR1_OC1M(regval)                 (BITS(3,6) & ((uint32_t)(regval) << 3))        /*!< ADV TIMER TIM CCMR1: OC1M Bit Value */  
#define ADV_TIMER_TIM_CCMR1_OC1M_FROZEN                       ADV_TIMER_TIM_CCMR1_OC1M(0)                                             /*!< FROZEN */
#define ADV_TIMER_TIM_CCMR1_OC1M_SET_ACTIVE                   ADV_TIMER_TIM_CCMR1_OC1M(1)                                             /*!< SET_ACTIVE */
#define ADV_TIMER_TIM_CCMR1_OC1M_SET_INACTIVE                 ADV_TIMER_TIM_CCMR1_OC1M(2)                                             /*!< SET_INACTIVE */
#define ADV_TIMER_TIM_CCMR1_OC1M_TOGGLE                       ADV_TIMER_TIM_CCMR1_OC1M(3)                                             /*!< TOGGLE */
#define ADV_TIMER_TIM_CCMR1_OC1M_FORCE_INACTIVE               ADV_TIMER_TIM_CCMR1_OC1M(4)                                             /*!< FORCE_INACTIVE */
#define ADV_TIMER_TIM_CCMR1_OC1M_FORCE_ACTIVE                 ADV_TIMER_TIM_CCMR1_OC1M(5)                                             /*!< FORCE_ACTIVE */
#define ADV_TIMER_TIM_CCMR1_OC1M_PWM1                         ADV_TIMER_TIM_CCMR1_OC1M(6)                                             /*!< PWM1 */
#define ADV_TIMER_TIM_CCMR1_OC1M_PWM2                         ADV_TIMER_TIM_CCMR1_OC1M(7)                                             /*!< PWM2 */
#define ADV_TIMER_TIM_CCMR1_OC1M_OPM1                         ADV_TIMER_TIM_CCMR1_OC1M(8)                                             /*!< OPM1 */
#define ADV_TIMER_TIM_CCMR1_OC1M_OPM2                         ADV_TIMER_TIM_CCMR1_OC1M(9)                                             /*!< OPM2 */
#define ADV_TIMER_TIM_CCMR1_OC1M_RESERVED                     ADV_TIMER_TIM_CCMR1_OC1M(10)                                             /*!< RESERVED */
#define ADV_TIMER_TIM_CCMR1_OC1M_RESERVED1                    ADV_TIMER_TIM_CCMR1_OC1M(11)                                             /*!< RESERVED1 */
#define ADV_TIMER_TIM_CCMR1_OC1M_COMBINED_PWM1                ADV_TIMER_TIM_CCMR1_OC1M(12)                                             /*!< COMBINED_PWM1 */
#define ADV_TIMER_TIM_CCMR1_OC1M_COMBINED_PWM2                ADV_TIMER_TIM_CCMR1_OC1M(13)                                             /*!< COMBINED_PWM2 */
#define ADV_TIMER_TIM_CCMR1_OC1M_ASYMMETRIC_PWM1              ADV_TIMER_TIM_CCMR1_OC1M(14)                                             /*!< ASYMMETRIC_PWM1 */
#define ADV_TIMER_TIM_CCMR1_OC1M_ASYMMETRIC_PWM2              ADV_TIMER_TIM_CCMR1_OC1M(15)                                             /*!< ASYMMETRIC_PWM2 */

/**
  * \brief Check the ADV_TIMER tim_ccmr1 oc1m bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid oc1m bits.
  * \retval 1 This is a valid oc1m bits.
  */
#define IS_ADV_TIMER_TIM_CCMR1_OC1M(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_FROZEN              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_SET_ACTIVE          ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_SET_INACTIVE        ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_TOGGLE              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_FORCE_INACTIVE      ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_FORCE_ACTIVE        ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_PWM1                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_PWM2                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_OPM1                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_OPM2                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_RESERVED            ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_RESERVED1           ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_COMBINED_PWM1       ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_COMBINED_PWM2       ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_ASYMMETRIC_PWM1     ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC1M_ASYMMETRIC_PWM2     )  \
                                                 )

#define ADV_TIMER_TIM_CCMR1_OC1CE                        BIT(7)                                      /*!< Output Compare 1 clear enable - 1'b0: disable:OC1Ref is not affected by the ETRF input - 1'b1: enable:OC1Ref is cleared as soon as a High level is detected on ETRF input */
#define ADV_TIMER_TIM_CCMR1_OC1CE_OFS                    7U                                          /*!< ADV TIMER TIM CCMR1: OC1CE Bit Offset */
#define ADV_TIMER_TIM_CCMR1_OC1CE_VAL(regval)                (BIT(7) & ((uint32_t)(regval) << 7))        /*!< ADV TIMER TIM CCMR1: OC1CE Bit Value */  
#define ADV_TIMER_TIM_CCMR1_OC1CE_DISABLE                     0x0UL                                           /*!< DISABLE */
#define ADV_TIMER_TIM_CCMR1_OC1CE_ENABLE                      BIT(7)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccmr1 oc1ce bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid oc1ce bit.
  * \retval 1 This is a valid oc1ce bit.
  */
#define IS_ADV_TIMER_TIM_CCMR1_OC1CE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCMR1_OC1CE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CCMR1_OC1CE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCMR1_CC2S_MASK                    BITS(8,9)                                   /*!< ADV TIMER TIM CCMR1: CC2S Bit Mask */  
#define ADV_TIMER_TIM_CCMR1_CC2S_OFS                     8U                                          /*!< ADV TIMER TIM CCMR1: CC2S Bit Offset */
#define ADV_TIMER_TIM_CCMR1_CC2S(regval)                 (BITS(8,9) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM CCMR1: CC2S Bit Value */  
#define ADV_TIMER_TIM_CCMR1_CC2S_OUTPUT                       ADV_TIMER_TIM_CCMR1_CC2S(0)                                             /*!< OUTPUT */
#define ADV_TIMER_TIM_CCMR1_CC2S_TI2                          ADV_TIMER_TIM_CCMR1_CC2S(1)                                             /*!< TI2 */
#define ADV_TIMER_TIM_CCMR1_CC2S_TI1                          ADV_TIMER_TIM_CCMR1_CC2S(2)                                             /*!< TI1 */
#define ADV_TIMER_TIM_CCMR1_CC2S_TRC                          ADV_TIMER_TIM_CCMR1_CC2S(3)                                             /*!< TRC */

/**
  * \brief Check the ADV_TIMER tim_ccmr1 cc2s bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid cc2s bits.
  * \retval 1 This is a valid cc2s bits.
  */
#define IS_ADV_TIMER_TIM_CCMR1_CC2S(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCMR1_CC2S_OUTPUT              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_CC2S_TI2                 ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_CC2S_TI1                 ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_CC2S_TRC                 )  \
                                                 )

#define ADV_TIMER_TIM_CCMR1_OC2PE                        BIT(10)                                      /*!< Output Compare 2 preload enable - 1'b0: disable: Preload register on TIM_CCR1 disabled. TIM_CCR1 can be written at anytime, the new value is taken in account immediately. - 1'b1: enable:Preload register on TIM_CCR1 enabled. Read/Write operations access the preload register. TIM_CCR1 preload value is loaded in the active register at each update event. */
#define ADV_TIMER_TIM_CCMR1_OC2PE_OFS                    10U                                          /*!< ADV TIMER TIM CCMR1: OC2PE Bit Offset */
#define ADV_TIMER_TIM_CCMR1_OC2PE_VAL(regval)                (BIT(10) & ((uint32_t)(regval) << 10))        /*!< ADV TIMER TIM CCMR1: OC2PE Bit Value */  
#define ADV_TIMER_TIM_CCMR1_OC2PE_DISABLE                     0x0UL                                           /*!< DISABLE */
#define ADV_TIMER_TIM_CCMR1_OC2PE_ENABLE                      BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccmr1 oc2pe bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid oc2pe bit.
  * \retval 1 This is a valid oc2pe bit.
  */
#define IS_ADV_TIMER_TIM_CCMR1_OC2PE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCMR1_OC2PE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CCMR1_OC2PE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCMR1_OC2M_MASK                    BITS(11,14)                                   /*!< ADV TIMER TIM CCMR1: OC2M Bit Mask */  
#define ADV_TIMER_TIM_CCMR1_OC2M_OFS                     11U                                          /*!< ADV TIMER TIM CCMR1: OC2M Bit Offset */
#define ADV_TIMER_TIM_CCMR1_OC2M(regval)                 (BITS(11,14) & ((uint32_t)(regval) << 11))        /*!< ADV TIMER TIM CCMR1: OC2M Bit Value */  
#define ADV_TIMER_TIM_CCMR1_OC2M_FROZEN                       ADV_TIMER_TIM_CCMR1_OC2M(0)                                             /*!< FROZEN */
#define ADV_TIMER_TIM_CCMR1_OC2M_SET_ACTIVE                   ADV_TIMER_TIM_CCMR1_OC2M(1)                                             /*!< SET_ACTIVE */
#define ADV_TIMER_TIM_CCMR1_OC2M_SET_INACTIVE                 ADV_TIMER_TIM_CCMR1_OC2M(2)                                             /*!< SET_INACTIVE */
#define ADV_TIMER_TIM_CCMR1_OC2M_TOGGLE                       ADV_TIMER_TIM_CCMR1_OC2M(3)                                             /*!< TOGGLE */
#define ADV_TIMER_TIM_CCMR1_OC2M_FORCE_INACTIVE               ADV_TIMER_TIM_CCMR1_OC2M(4)                                             /*!< FORCE_INACTIVE */
#define ADV_TIMER_TIM_CCMR1_OC2M_FORCE_ACTIVE                 ADV_TIMER_TIM_CCMR1_OC2M(5)                                             /*!< FORCE_ACTIVE */
#define ADV_TIMER_TIM_CCMR1_OC2M_PWM1                         ADV_TIMER_TIM_CCMR1_OC2M(6)                                             /*!< PWM1 */
#define ADV_TIMER_TIM_CCMR1_OC2M_PWM2                         ADV_TIMER_TIM_CCMR1_OC2M(7)                                             /*!< PWM2 */
#define ADV_TIMER_TIM_CCMR1_OC2M_OPM1                         ADV_TIMER_TIM_CCMR1_OC2M(8)                                             /*!< OPM1 */
#define ADV_TIMER_TIM_CCMR1_OC2M_OPM2                         ADV_TIMER_TIM_CCMR1_OC2M(9)                                             /*!< OPM2 */
#define ADV_TIMER_TIM_CCMR1_OC2M_RESERVED                     ADV_TIMER_TIM_CCMR1_OC2M(10)                                             /*!< RESERVED */
#define ADV_TIMER_TIM_CCMR1_OC2M_RESERVED1                    ADV_TIMER_TIM_CCMR1_OC2M(11)                                             /*!< RESERVED1 */
#define ADV_TIMER_TIM_CCMR1_OC2M_COMBINED_PWM1                ADV_TIMER_TIM_CCMR1_OC2M(12)                                             /*!< COMBINED_PWM1 */
#define ADV_TIMER_TIM_CCMR1_OC2M_COMBINED_PWM2                ADV_TIMER_TIM_CCMR1_OC2M(13)                                             /*!< COMBINED_PWM2 */
#define ADV_TIMER_TIM_CCMR1_OC2M_ASYMMETRIC_PWM1              ADV_TIMER_TIM_CCMR1_OC2M(14)                                             /*!< ASYMMETRIC_PWM1 */
#define ADV_TIMER_TIM_CCMR1_OC2M_ASYMMETRIC_PWM2              ADV_TIMER_TIM_CCMR1_OC2M(15)                                             /*!< ASYMMETRIC_PWM2 */

/**
  * \brief Check the ADV_TIMER tim_ccmr1 oc2m bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid oc2m bits.
  * \retval 1 This is a valid oc2m bits.
  */
#define IS_ADV_TIMER_TIM_CCMR1_OC2M(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_FROZEN              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_SET_ACTIVE          ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_SET_INACTIVE        ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_TOGGLE              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_FORCE_INACTIVE      ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_FORCE_ACTIVE        ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_PWM1                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_PWM2                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_OPM1                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_OPM2                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_RESERVED            ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_RESERVED1           ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_COMBINED_PWM1       ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_COMBINED_PWM2       ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_ASYMMETRIC_PWM1     ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_OC2M_ASYMMETRIC_PWM2     )  \
                                                 )

#define ADV_TIMER_TIM_CCMR1_OC2CE                        BIT(15)                                      /*!< Output Compare 2 clear enable  - 1'b0: disable:OC1Ref is not affected by the ETRF input  - 1'b1: enable:OC1Ref is cleared as soon as a High level is detected on ETRF input */
#define ADV_TIMER_TIM_CCMR1_OC2CE_OFS                    15U                                          /*!< ADV TIMER TIM CCMR1: OC2CE Bit Offset */
#define ADV_TIMER_TIM_CCMR1_OC2CE_VAL(regval)                (BIT(15) & ((uint32_t)(regval) << 15))        /*!< ADV TIMER TIM CCMR1: OC2CE Bit Value */  
#define ADV_TIMER_TIM_CCMR1_OC2CE_DISABLE                     0x0UL                                           /*!< DISABLE */
#define ADV_TIMER_TIM_CCMR1_OC2CE_ENABLE                      BIT(15)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccmr1 oc2ce bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid oc2ce bit.
  * \retval 1 This is a valid oc2ce bit.
  */
#define IS_ADV_TIMER_TIM_CCMR1_OC2CE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCMR1_OC2CE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CCMR1_OC2CE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCMR1_OC1FE                        BIT(16)                                      /*!< Output Compare 1 fast enable This bit is used to accelerate the effect of an event on the trigger in input on the CC output. - 0: CC1 behaves normally depending on counter and CCR1 values even when the trigger is ON. The minimum delay to activate CC1 output when an edge occurs on the trigger input is 5 clock cycles. - 1: An active edge on the trigger input acts like a compare match on CC1 output. Then, OC is set to the compare level independently from the result of the comparison. Delay to sample the trigger input and to activate CC1 output is reduced to 3 clock cycles. OCFE acts only if the channel is configured in PWM1 or PWM2 mode. */
#define ADV_TIMER_TIM_CCMR1_OC1FE_OFS                    16U                                          /*!< ADV TIMER TIM CCMR1: OC1FE Bit Offset */
#define ADV_TIMER_TIM_CCMR1_OC1FE_VAL(regval)                (BIT(16) & ((uint32_t)(regval) << 16))        /*!< ADV TIMER TIM CCMR1: OC1FE Bit Value */  
#define ADV_TIMER_TIM_CCMR1_IC1PSC_MASK                  BITS(18,19)                                   /*!< ADV TIMER TIM CCMR1: IC1PSC Bit Mask */  
#define ADV_TIMER_TIM_CCMR1_IC1PSC_OFS                   18U                                          /*!< ADV TIMER TIM CCMR1: IC1PSC Bit Offset */
#define ADV_TIMER_TIM_CCMR1_IC1PSC(regval)               (BITS(18,19) & ((uint32_t)(regval) << 18))        /*!< ADV TIMER TIM CCMR1: IC1PSC Bit Value */  
#define ADV_TIMER_TIM_CCMR1_IC1PSC_NO_PRESCALER               ADV_TIMER_TIM_CCMR1_IC1PSC(0)                                             /*!< NO_PRESCALER */
#define ADV_TIMER_TIM_CCMR1_IC1PSC_2_EVENTS                   ADV_TIMER_TIM_CCMR1_IC1PSC(1)                                             /*!< 2_EVENTS */
#define ADV_TIMER_TIM_CCMR1_IC1PSC_4_EVENTS                   ADV_TIMER_TIM_CCMR1_IC1PSC(2)                                             /*!< 4_EVENTS */
#define ADV_TIMER_TIM_CCMR1_IC1PSC_8_EVENTS                   ADV_TIMER_TIM_CCMR1_IC1PSC(3)                                             /*!< 8_EVENTS */

/**
  * \brief Check the ADV_TIMER tim_ccmr1 ic1psc bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid ic1psc bits.
  * \retval 1 This is a valid ic1psc bits.
  */
#define IS_ADV_TIMER_TIM_CCMR1_IC1PSC(regval)                 (\
                                           ((regval) == ADV_TIMER_TIM_CCMR1_IC1PSC_NO_PRESCALER        ) || \
                                           ((regval) == ADV_TIMER_TIM_CCMR1_IC1PSC_2_EVENTS            ) || \
                                           ((regval) == ADV_TIMER_TIM_CCMR1_IC1PSC_4_EVENTS            ) || \
                                           ((regval) == ADV_TIMER_TIM_CCMR1_IC1PSC_8_EVENTS            )  \
                                                 )

#define ADV_TIMER_TIM_CCMR1_IC1F_MASK                    BITS(20,23)                                   /*!< ADV TIMER TIM CCMR1: IC1F Bit Mask */  
#define ADV_TIMER_TIM_CCMR1_IC1F_OFS                     20U                                          /*!< ADV TIMER TIM CCMR1: IC1F Bit Offset */
#define ADV_TIMER_TIM_CCMR1_IC1F(regval)                 (BITS(20,23) & ((uint32_t)(regval) << 20))        /*!< ADV TIMER TIM CCMR1: IC1F Bit Value */  
#define ADV_TIMER_TIM_CCMR1_IC1F_NO_FILTER                    ADV_TIMER_TIM_CCMR1_IC1F(0)                                             /*!< NO_FILTER */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL1_N2                       ADV_TIMER_TIM_CCMR1_IC1F(1)                                             /*!< FL1_N2 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL1_N4                       ADV_TIMER_TIM_CCMR1_IC1F(2)                                             /*!< FL1_N4 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL1_N8                       ADV_TIMER_TIM_CCMR1_IC1F(3)                                             /*!< FL1_N8 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL2_N6                       ADV_TIMER_TIM_CCMR1_IC1F(4)                                             /*!< FL2_N6 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL2_N8                       ADV_TIMER_TIM_CCMR1_IC1F(5)                                             /*!< FL2_N8 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL4_N6                       ADV_TIMER_TIM_CCMR1_IC1F(6)                                             /*!< FL4_N6 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL4_N8                       ADV_TIMER_TIM_CCMR1_IC1F(7)                                             /*!< FL4_N8 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL8_N6                       ADV_TIMER_TIM_CCMR1_IC1F(8)                                             /*!< FL8_N6 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL8_N8                       ADV_TIMER_TIM_CCMR1_IC1F(9)                                             /*!< FL8_N8 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL16_N5                      ADV_TIMER_TIM_CCMR1_IC1F(10)                                             /*!< FL16_N5 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL16_N6                      ADV_TIMER_TIM_CCMR1_IC1F(11)                                             /*!< FL16_N6 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL16_N8                      ADV_TIMER_TIM_CCMR1_IC1F(12)                                             /*!< FL16_N8 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL32_N5                      ADV_TIMER_TIM_CCMR1_IC1F(13)                                             /*!< FL32_N5 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL32_N6                      ADV_TIMER_TIM_CCMR1_IC1F(14)                                             /*!< FL32_N6 */
#define ADV_TIMER_TIM_CCMR1_IC1F_FL32_N8                      ADV_TIMER_TIM_CCMR1_IC1F(15)                                             /*!< FL32_N8 */

/**
  * \brief Check the ADV_TIMER tim_ccmr1 ic1f bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid ic1f bits.
  * \retval 1 This is a valid ic1f bits.
  */
#define IS_ADV_TIMER_TIM_CCMR1_IC1F(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_NO_FILTER           ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL1_N2              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL1_N4              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL1_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL2_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL2_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL4_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL4_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL8_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL8_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL16_N5             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL16_N6             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL16_N8             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL32_N5             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL32_N6             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC1F_FL32_N8             )  \
                                                 )

#define ADV_TIMER_TIM_CCMR1_OC2FE                        BIT(24)                                      /*!< Output Compare 2 fast enable Refer to OC1FE description */
#define ADV_TIMER_TIM_CCMR1_OC2FE_OFS                    24U                                          /*!< ADV TIMER TIM CCMR1: OC2FE Bit Offset */
#define ADV_TIMER_TIM_CCMR1_OC2FE_VAL(regval)                (BIT(24) & ((uint32_t)(regval) << 24))        /*!< ADV TIMER TIM CCMR1: OC2FE Bit Value */  
#define ADV_TIMER_TIM_CCMR1_IC2PSC_MASK                  BITS(26,27)                                   /*!< ADV TIMER TIM CCMR1: IC2PSC Bit Mask */  
#define ADV_TIMER_TIM_CCMR1_IC2PSC_OFS                   26U                                          /*!< ADV TIMER TIM CCMR1: IC2PSC Bit Offset */
#define ADV_TIMER_TIM_CCMR1_IC2PSC(regval)               (BITS(26,27) & ((uint32_t)(regval) << 26))        /*!< ADV TIMER TIM CCMR1: IC2PSC Bit Value */  
#define ADV_TIMER_TIM_CCMR1_IC2PSC_NO_PRESCALER               ADV_TIMER_TIM_CCMR1_IC2PSC(0)                                             /*!< NO_PRESCALER */
#define ADV_TIMER_TIM_CCMR1_IC2PSC_2_EVENTS                   ADV_TIMER_TIM_CCMR1_IC2PSC(1)                                             /*!< 2_EVENTS */
#define ADV_TIMER_TIM_CCMR1_IC2PSC_4_EVENTS                   ADV_TIMER_TIM_CCMR1_IC2PSC(2)                                             /*!< 4_EVENTS */
#define ADV_TIMER_TIM_CCMR1_IC2PSC_8_EVENTS                   ADV_TIMER_TIM_CCMR1_IC2PSC(3)                                             /*!< 8_EVENTS */

/**
  * \brief Check the ADV_TIMER tim_ccmr1 ic2psc bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid ic2psc bits.
  * \retval 1 This is a valid ic2psc bits.
  */
#define IS_ADV_TIMER_TIM_CCMR1_IC2PSC(regval)                 (\
                                           ((regval) == ADV_TIMER_TIM_CCMR1_IC2PSC_NO_PRESCALER        ) || \
                                           ((regval) == ADV_TIMER_TIM_CCMR1_IC2PSC_2_EVENTS            ) || \
                                           ((regval) == ADV_TIMER_TIM_CCMR1_IC2PSC_4_EVENTS            ) || \
                                           ((regval) == ADV_TIMER_TIM_CCMR1_IC2PSC_8_EVENTS            )  \
                                                 )

#define ADV_TIMER_TIM_CCMR1_IC2F_MASK                    BITS(28,31)                                   /*!< ADV TIMER TIM CCMR1: IC2F Bit Mask */  
#define ADV_TIMER_TIM_CCMR1_IC2F_OFS                     28U                                          /*!< ADV TIMER TIM CCMR1: IC2F Bit Offset */
#define ADV_TIMER_TIM_CCMR1_IC2F(regval)                 (BITS(28,31) & ((uint32_t)(regval) << 28))        /*!< ADV TIMER TIM CCMR1: IC2F Bit Value */  
#define ADV_TIMER_TIM_CCMR1_IC2F_NO_FILTER                    ADV_TIMER_TIM_CCMR1_IC2F(0)                                             /*!< NO_FILTER */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL1_N2                       ADV_TIMER_TIM_CCMR1_IC2F(1)                                             /*!< FL1_N2 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL1_N4                       ADV_TIMER_TIM_CCMR1_IC2F(2)                                             /*!< FL1_N4 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL1_N8                       ADV_TIMER_TIM_CCMR1_IC2F(3)                                             /*!< FL1_N8 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL2_N6                       ADV_TIMER_TIM_CCMR1_IC2F(4)                                             /*!< FL2_N6 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL2_N8                       ADV_TIMER_TIM_CCMR1_IC2F(5)                                             /*!< FL2_N8 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL4_N6                       ADV_TIMER_TIM_CCMR1_IC2F(6)                                             /*!< FL4_N6 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL4_N8                       ADV_TIMER_TIM_CCMR1_IC2F(7)                                             /*!< FL4_N8 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL8_N6                       ADV_TIMER_TIM_CCMR1_IC2F(8)                                             /*!< FL8_N6 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL8_N8                       ADV_TIMER_TIM_CCMR1_IC2F(9)                                             /*!< FL8_N8 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL16_N5                      ADV_TIMER_TIM_CCMR1_IC2F(10)                                             /*!< FL16_N5 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL16_N6                      ADV_TIMER_TIM_CCMR1_IC2F(11)                                             /*!< FL16_N6 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL16_N8                      ADV_TIMER_TIM_CCMR1_IC2F(12)                                             /*!< FL16_N8 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL32_N5                      ADV_TIMER_TIM_CCMR1_IC2F(13)                                             /*!< FL32_N5 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL32_N6                      ADV_TIMER_TIM_CCMR1_IC2F(14)                                             /*!< FL32_N6 */
#define ADV_TIMER_TIM_CCMR1_IC2F_FL32_N8                      ADV_TIMER_TIM_CCMR1_IC2F(15)                                             /*!< FL32_N8 */

/**
  * \brief Check the ADV_TIMER tim_ccmr1 ic2f bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid ic2f bits.
  * \retval 1 This is a valid ic2f bits.
  */
#define IS_ADV_TIMER_TIM_CCMR1_IC2F(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_NO_FILTER           ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL1_N2              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL1_N4              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL1_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL2_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL2_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL4_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL4_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL8_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL8_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL16_N5             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL16_N6             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL16_N8             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL32_N5             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL32_N6             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR1_IC2F_FL32_N8             )  \
                                                 )

 /* ===== ADV TIMER TIM_CCMR2 Register definition ===== */
#define ADV_TIMER_TIM_CCMR2_CC3S_MASK                    BITS(0,1)                                   /*!< ADV TIMER TIM CCMR2: CC3S Bit Mask */  
#define ADV_TIMER_TIM_CCMR2_CC3S_OFS                     0U                                          /*!< ADV TIMER TIM CCMR2: CC3S Bit Offset */
#define ADV_TIMER_TIM_CCMR2_CC3S(regval)                 (BITS(0,1) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM CCMR2: CC3S Bit Value */  
#define ADV_TIMER_TIM_CCMR2_CC3S_OUTPUT                       ADV_TIMER_TIM_CCMR2_CC3S(0)                                             /*!< OUTPUT */
#define ADV_TIMER_TIM_CCMR2_CC3S_TI3                          ADV_TIMER_TIM_CCMR2_CC3S(1)                                             /*!< TI3 */
#define ADV_TIMER_TIM_CCMR2_CC3S_TI4                          ADV_TIMER_TIM_CCMR2_CC3S(2)                                             /*!< TI4 */
#define ADV_TIMER_TIM_CCMR2_CC3S_TRC                          ADV_TIMER_TIM_CCMR2_CC3S(3)                                             /*!< TRC */

/**
  * \brief Check the ADV_TIMER tim_ccmr2 cc3s bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid cc3s bits.
  * \retval 1 This is a valid cc3s bits.
  */
#define IS_ADV_TIMER_TIM_CCMR2_CC3S(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCMR2_CC3S_OUTPUT              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_CC3S_TI3                 ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_CC3S_TI4                 ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_CC3S_TRC                 )  \
                                                 )

#define ADV_TIMER_TIM_CCMR2_OC3PE                        BIT(2)                                      /*!< Output compare 3 preload enable - 1'b0: disable: Preload register on TIM_CCR1 disabled. TIM_CCR1 can be written at anytime, the new value is taken in account immediately. - 1'b1: enable:Preload register on TIM_CCR1 enabled. Read/Write operations access the preload register. TIM_CCR1 preload value is loaded in the active register at each update event. */
#define ADV_TIMER_TIM_CCMR2_OC3PE_OFS                    2U                                          /*!< ADV TIMER TIM CCMR2: OC3PE Bit Offset */
#define ADV_TIMER_TIM_CCMR2_OC3PE_VAL(regval)                (BIT(2) & ((uint32_t)(regval) << 2))        /*!< ADV TIMER TIM CCMR2: OC3PE Bit Value */  
#define ADV_TIMER_TIM_CCMR2_OC3PE_DISABLE                     0x0UL                                           /*!< DISABLE */
#define ADV_TIMER_TIM_CCMR2_OC3PE_ENABLE                      BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccmr2 oc3pe bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid oc3pe bit.
  * \retval 1 This is a valid oc3pe bit.
  */
#define IS_ADV_TIMER_TIM_CCMR2_OC3PE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCMR2_OC3PE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CCMR2_OC3PE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCMR2_OC3M_MASK                    BITS(3,6)                                   /*!< ADV TIMER TIM CCMR2: OC3M Bit Mask */  
#define ADV_TIMER_TIM_CCMR2_OC3M_OFS                     3U                                          /*!< ADV TIMER TIM CCMR2: OC3M Bit Offset */
#define ADV_TIMER_TIM_CCMR2_OC3M(regval)                 (BITS(3,6) & ((uint32_t)(regval) << 3))        /*!< ADV TIMER TIM CCMR2: OC3M Bit Value */  
#define ADV_TIMER_TIM_CCMR2_OC3M_FROZEN                       ADV_TIMER_TIM_CCMR2_OC3M(0)                                             /*!< FROZEN */
#define ADV_TIMER_TIM_CCMR2_OC3M_SET_ACTIVE                   ADV_TIMER_TIM_CCMR2_OC3M(1)                                             /*!< SET_ACTIVE */
#define ADV_TIMER_TIM_CCMR2_OC3M_SET_INACTIVE                 ADV_TIMER_TIM_CCMR2_OC3M(2)                                             /*!< SET_INACTIVE */
#define ADV_TIMER_TIM_CCMR2_OC3M_TOGGLE                       ADV_TIMER_TIM_CCMR2_OC3M(3)                                             /*!< TOGGLE */
#define ADV_TIMER_TIM_CCMR2_OC3M_FORCE_INACTIVE               ADV_TIMER_TIM_CCMR2_OC3M(4)                                             /*!< FORCE_INACTIVE */
#define ADV_TIMER_TIM_CCMR2_OC3M_FORCE_ACTIVE                 ADV_TIMER_TIM_CCMR2_OC3M(5)                                             /*!< FORCE_ACTIVE */
#define ADV_TIMER_TIM_CCMR2_OC3M_PWM1                         ADV_TIMER_TIM_CCMR2_OC3M(6)                                             /*!< PWM1 */
#define ADV_TIMER_TIM_CCMR2_OC3M_PWM2                         ADV_TIMER_TIM_CCMR2_OC3M(7)                                             /*!< PWM2 */
#define ADV_TIMER_TIM_CCMR2_OC3M_OPM1                         ADV_TIMER_TIM_CCMR2_OC3M(8)                                             /*!< OPM1 */
#define ADV_TIMER_TIM_CCMR2_OC3M_OPM2                         ADV_TIMER_TIM_CCMR2_OC3M(9)                                             /*!< OPM2 */
#define ADV_TIMER_TIM_CCMR2_OC3M_RESERVED                     ADV_TIMER_TIM_CCMR2_OC3M(10)                                             /*!< RESERVED */
#define ADV_TIMER_TIM_CCMR2_OC3M_RESERVED1                    ADV_TIMER_TIM_CCMR2_OC3M(11)                                             /*!< RESERVED1 */
#define ADV_TIMER_TIM_CCMR2_OC3M_COMBINED_PWM1                ADV_TIMER_TIM_CCMR2_OC3M(12)                                             /*!< COMBINED_PWM1 */
#define ADV_TIMER_TIM_CCMR2_OC3M_COMBINED_PWM2                ADV_TIMER_TIM_CCMR2_OC3M(13)                                             /*!< COMBINED_PWM2 */
#define ADV_TIMER_TIM_CCMR2_OC3M_ASYMMETRIC_PWM1              ADV_TIMER_TIM_CCMR2_OC3M(14)                                             /*!< ASYMMETRIC_PWM1 */
#define ADV_TIMER_TIM_CCMR2_OC3M_ASYMMETRIC_PWM2              ADV_TIMER_TIM_CCMR2_OC3M(15)                                             /*!< ASYMMETRIC_PWM2 */

/**
  * \brief Check the ADV_TIMER tim_ccmr2 oc3m bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid oc3m bits.
  * \retval 1 This is a valid oc3m bits.
  */
#define IS_ADV_TIMER_TIM_CCMR2_OC3M(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_FROZEN              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_SET_ACTIVE          ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_SET_INACTIVE        ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_TOGGLE              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_FORCE_INACTIVE      ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_FORCE_ACTIVE        ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_PWM1                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_PWM2                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_OPM1                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_OPM2                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_RESERVED            ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_RESERVED1           ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_COMBINED_PWM1       ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_COMBINED_PWM2       ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_ASYMMETRIC_PWM1     ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC3M_ASYMMETRIC_PWM2     )  \
                                                 )

#define ADV_TIMER_TIM_CCMR2_OC3CE                        BIT(7)                                      /*!< Output compare 3 clear enable - 1'b0: disable:OC1Ref is not affected by the ETRF input - 1'b1: enable:OC1Ref is cleared as soon as a High level is detected on ETRF input */
#define ADV_TIMER_TIM_CCMR2_OC3CE_OFS                    7U                                          /*!< ADV TIMER TIM CCMR2: OC3CE Bit Offset */
#define ADV_TIMER_TIM_CCMR2_OC3CE_VAL(regval)                (BIT(7) & ((uint32_t)(regval) << 7))        /*!< ADV TIMER TIM CCMR2: OC3CE Bit Value */  
#define ADV_TIMER_TIM_CCMR2_OC3CE_DISABLE                     0x0UL                                           /*!< DISABLE */
#define ADV_TIMER_TIM_CCMR2_OC3CE_ENABLE                      BIT(7)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccmr2 oc3ce bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid oc3ce bit.
  * \retval 1 This is a valid oc3ce bit.
  */
#define IS_ADV_TIMER_TIM_CCMR2_OC3CE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCMR2_OC3CE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CCMR2_OC3CE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCMR2_CC4S_MASK                    BITS(8,9)                                   /*!< ADV TIMER TIM CCMR2: CC4S Bit Mask */  
#define ADV_TIMER_TIM_CCMR2_CC4S_OFS                     8U                                          /*!< ADV TIMER TIM CCMR2: CC4S Bit Offset */
#define ADV_TIMER_TIM_CCMR2_CC4S(regval)                 (BITS(8,9) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM CCMR2: CC4S Bit Value */  
#define ADV_TIMER_TIM_CCMR2_CC4S_OUTPUT                       ADV_TIMER_TIM_CCMR2_CC4S(0)                                             /*!< OUTPUT */
#define ADV_TIMER_TIM_CCMR2_CC4S_TI4                          ADV_TIMER_TIM_CCMR2_CC4S(1)                                             /*!< TI4 */
#define ADV_TIMER_TIM_CCMR2_CC4S_TI3                          ADV_TIMER_TIM_CCMR2_CC4S(2)                                             /*!< TI3 */
#define ADV_TIMER_TIM_CCMR2_CC4S_TRC                          ADV_TIMER_TIM_CCMR2_CC4S(3)                                             /*!< TRC */

/**
  * \brief Check the ADV_TIMER tim_ccmr2 cc4s bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid cc4s bits.
  * \retval 1 This is a valid cc4s bits.
  */
#define IS_ADV_TIMER_TIM_CCMR2_CC4S(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCMR2_CC4S_OUTPUT              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_CC4S_TI4                 ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_CC4S_TI3                 ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_CC4S_TRC                 )  \
                                                 )

#define ADV_TIMER_TIM_CCMR2_OC4PE                        BIT(10)                                      /*!< Output compare 4 preload enable - 1'b0: disable: Preload register on TIM_CCR1 disabled. TIM_CCR1 can be written at anytime, the new value is taken in account immediately. - 1'b1: enable:Preload register on TIM_CCR1 enabled. Read/Write operations access the preload register. TIM_CCR1 preload value is loaded in the active register at each update event. */
#define ADV_TIMER_TIM_CCMR2_OC4PE_OFS                    10U                                          /*!< ADV TIMER TIM CCMR2: OC4PE Bit Offset */
#define ADV_TIMER_TIM_CCMR2_OC4PE_VAL(regval)                (BIT(10) & ((uint32_t)(regval) << 10))        /*!< ADV TIMER TIM CCMR2: OC4PE Bit Value */  
#define ADV_TIMER_TIM_CCMR2_OC4PE_DISABLE                     0x0UL                                           /*!< DISABLE */
#define ADV_TIMER_TIM_CCMR2_OC4PE_ENABLE                      BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccmr2 oc4pe bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid oc4pe bit.
  * \retval 1 This is a valid oc4pe bit.
  */
#define IS_ADV_TIMER_TIM_CCMR2_OC4PE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCMR2_OC4PE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CCMR2_OC4PE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCMR2_OC4M_MASK                    BITS(11,14)                                   /*!< ADV TIMER TIM CCMR2: OC4M Bit Mask */  
#define ADV_TIMER_TIM_CCMR2_OC4M_OFS                     11U                                          /*!< ADV TIMER TIM CCMR2: OC4M Bit Offset */
#define ADV_TIMER_TIM_CCMR2_OC4M(regval)                 (BITS(11,14) & ((uint32_t)(regval) << 11))        /*!< ADV TIMER TIM CCMR2: OC4M Bit Value */  
#define ADV_TIMER_TIM_CCMR2_OC4M_FROZEN                       ADV_TIMER_TIM_CCMR2_OC4M(0)                                             /*!< FROZEN */
#define ADV_TIMER_TIM_CCMR2_OC4M_SET_ACTIVE                   ADV_TIMER_TIM_CCMR2_OC4M(1)                                             /*!< SET_ACTIVE */
#define ADV_TIMER_TIM_CCMR2_OC4M_SET_INACTIVE                 ADV_TIMER_TIM_CCMR2_OC4M(2)                                             /*!< SET_INACTIVE */
#define ADV_TIMER_TIM_CCMR2_OC4M_TOGGLE                       ADV_TIMER_TIM_CCMR2_OC4M(3)                                             /*!< TOGGLE */
#define ADV_TIMER_TIM_CCMR2_OC4M_FORCE_INACTIVE               ADV_TIMER_TIM_CCMR2_OC4M(4)                                             /*!< FORCE_INACTIVE */
#define ADV_TIMER_TIM_CCMR2_OC4M_FORCE_ACTIVE                 ADV_TIMER_TIM_CCMR2_OC4M(5)                                             /*!< FORCE_ACTIVE */
#define ADV_TIMER_TIM_CCMR2_OC4M_PWM1                         ADV_TIMER_TIM_CCMR2_OC4M(6)                                             /*!< PWM1 */
#define ADV_TIMER_TIM_CCMR2_OC4M_PWM2                         ADV_TIMER_TIM_CCMR2_OC4M(7)                                             /*!< PWM2 */
#define ADV_TIMER_TIM_CCMR2_OC4M_OPM1                         ADV_TIMER_TIM_CCMR2_OC4M(8)                                             /*!< OPM1 */
#define ADV_TIMER_TIM_CCMR2_OC4M_OPM2                         ADV_TIMER_TIM_CCMR2_OC4M(9)                                             /*!< OPM2 */
#define ADV_TIMER_TIM_CCMR2_OC4M_RESERVED                     ADV_TIMER_TIM_CCMR2_OC4M(10)                                             /*!< RESERVED */
#define ADV_TIMER_TIM_CCMR2_OC4M_RESERVED1                    ADV_TIMER_TIM_CCMR2_OC4M(11)                                             /*!< RESERVED1 */
#define ADV_TIMER_TIM_CCMR2_OC4M_COMBINED_PWM1                ADV_TIMER_TIM_CCMR2_OC4M(12)                                             /*!< COMBINED_PWM1 */
#define ADV_TIMER_TIM_CCMR2_OC4M_COMBINED_PWM2                ADV_TIMER_TIM_CCMR2_OC4M(13)                                             /*!< COMBINED_PWM2 */
#define ADV_TIMER_TIM_CCMR2_OC4M_ASYMMETRIC_PWM1              ADV_TIMER_TIM_CCMR2_OC4M(14)                                             /*!< ASYMMETRIC_PWM1 */
#define ADV_TIMER_TIM_CCMR2_OC4M_ASYMMETRIC_PWM2              ADV_TIMER_TIM_CCMR2_OC4M(15)                                             /*!< ASYMMETRIC_PWM2 */

/**
  * \brief Check the ADV_TIMER tim_ccmr2 oc4m bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid oc4m bits.
  * \retval 1 This is a valid oc4m bits.
  */
#define IS_ADV_TIMER_TIM_CCMR2_OC4M(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_FROZEN              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_SET_ACTIVE          ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_SET_INACTIVE        ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_TOGGLE              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_FORCE_INACTIVE      ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_FORCE_ACTIVE        ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_PWM1                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_PWM2                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_OPM1                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_OPM2                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_RESERVED            ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_RESERVED1           ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_COMBINED_PWM1       ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_COMBINED_PWM2       ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_ASYMMETRIC_PWM1     ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_OC4M_ASYMMETRIC_PWM2     )  \
                                                 )

#define ADV_TIMER_TIM_CCMR2_OC4CE                        BIT(15)                                      /*!< Output compare 4 clear enable - 1'b0: disable:OC1Ref is not affected by the ETRF input - 1'b1: enable:OC1Ref is cleared as soon as a High level is detected on ETRF input */
#define ADV_TIMER_TIM_CCMR2_OC4CE_OFS                    15U                                          /*!< ADV TIMER TIM CCMR2: OC4CE Bit Offset */
#define ADV_TIMER_TIM_CCMR2_OC4CE_VAL(regval)                (BIT(15) & ((uint32_t)(regval) << 15))        /*!< ADV TIMER TIM CCMR2: OC4CE Bit Value */  
#define ADV_TIMER_TIM_CCMR2_OC4CE_DISABLE                     0x0UL                                           /*!< DISABLE */
#define ADV_TIMER_TIM_CCMR2_OC4CE_ENABLE                      BIT(15)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccmr2 oc4ce bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid oc4ce bit.
  * \retval 1 This is a valid oc4ce bit.
  */
#define IS_ADV_TIMER_TIM_CCMR2_OC4CE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCMR2_OC4CE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CCMR2_OC4CE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCMR2_OC3FE                        BIT(16)                                      /*!< Output Compare 3 fast enable Refer to OC1FE description */
#define ADV_TIMER_TIM_CCMR2_OC3FE_OFS                    16U                                          /*!< ADV TIMER TIM CCMR2: OC3FE Bit Offset */
#define ADV_TIMER_TIM_CCMR2_OC3FE_VAL(regval)                (BIT(16) & ((uint32_t)(regval) << 16))        /*!< ADV TIMER TIM CCMR2: OC3FE Bit Value */  
#define ADV_TIMER_TIM_CCMR2_IC3PSC_MASK                  BITS(18,19)                                   /*!< ADV TIMER TIM CCMR2: IC3PSC Bit Mask */  
#define ADV_TIMER_TIM_CCMR2_IC3PSC_OFS                   18U                                          /*!< ADV TIMER TIM CCMR2: IC3PSC Bit Offset */
#define ADV_TIMER_TIM_CCMR2_IC3PSC(regval)               (BITS(18,19) & ((uint32_t)(regval) << 18))        /*!< ADV TIMER TIM CCMR2: IC3PSC Bit Value */  
#define ADV_TIMER_TIM_CCMR2_IC3PSC_NO_PRESCALER               ADV_TIMER_TIM_CCMR2_IC3PSC(0)                                             /*!< NO_PRESCALER */
#define ADV_TIMER_TIM_CCMR2_IC3PSC_2_EVENTS                   ADV_TIMER_TIM_CCMR2_IC3PSC(1)                                             /*!< 2_EVENTS */
#define ADV_TIMER_TIM_CCMR2_IC3PSC_4_EVENTS                   ADV_TIMER_TIM_CCMR2_IC3PSC(2)                                             /*!< 4_EVENTS */
#define ADV_TIMER_TIM_CCMR2_IC3PSC_8_EVENTS                   ADV_TIMER_TIM_CCMR2_IC3PSC(3)                                             /*!< 8_EVENTS */

/**
  * \brief Check the ADV_TIMER tim_ccmr2 ic3psc bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid ic3psc bits.
  * \retval 1 This is a valid ic3psc bits.
  */
#define IS_ADV_TIMER_TIM_CCMR2_IC3PSC(regval)                 (\
                                           ((regval) == ADV_TIMER_TIM_CCMR2_IC3PSC_NO_PRESCALER        ) || \
                                           ((regval) == ADV_TIMER_TIM_CCMR2_IC3PSC_2_EVENTS            ) || \
                                           ((regval) == ADV_TIMER_TIM_CCMR2_IC3PSC_4_EVENTS            ) || \
                                           ((regval) == ADV_TIMER_TIM_CCMR2_IC3PSC_8_EVENTS            )  \
                                                 )

#define ADV_TIMER_TIM_CCMR2_IC3F_MASK                    BITS(20,23)                                   /*!< ADV TIMER TIM CCMR2: IC3F Bit Mask */  
#define ADV_TIMER_TIM_CCMR2_IC3F_OFS                     20U                                          /*!< ADV TIMER TIM CCMR2: IC3F Bit Offset */
#define ADV_TIMER_TIM_CCMR2_IC3F(regval)                 (BITS(20,23) & ((uint32_t)(regval) << 20))        /*!< ADV TIMER TIM CCMR2: IC3F Bit Value */  
#define ADV_TIMER_TIM_CCMR2_IC3F_NO_FILTER                    ADV_TIMER_TIM_CCMR2_IC3F(0)                                             /*!< NO_FILTER */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL1_N2                       ADV_TIMER_TIM_CCMR2_IC3F(1)                                             /*!< FL1_N2 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL1_N4                       ADV_TIMER_TIM_CCMR2_IC3F(2)                                             /*!< FL1_N4 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL1_N8                       ADV_TIMER_TIM_CCMR2_IC3F(3)                                             /*!< FL1_N8 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL2_N6                       ADV_TIMER_TIM_CCMR2_IC3F(4)                                             /*!< FL2_N6 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL2_N8                       ADV_TIMER_TIM_CCMR2_IC3F(5)                                             /*!< FL2_N8 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL4_N6                       ADV_TIMER_TIM_CCMR2_IC3F(6)                                             /*!< FL4_N6 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL4_N8                       ADV_TIMER_TIM_CCMR2_IC3F(7)                                             /*!< FL4_N8 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL8_N6                       ADV_TIMER_TIM_CCMR2_IC3F(8)                                             /*!< FL8_N6 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL8_N8                       ADV_TIMER_TIM_CCMR2_IC3F(9)                                             /*!< FL8_N8 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL16_N5                      ADV_TIMER_TIM_CCMR2_IC3F(10)                                             /*!< FL16_N5 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL16_N6                      ADV_TIMER_TIM_CCMR2_IC3F(11)                                             /*!< FL16_N6 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL16_N8                      ADV_TIMER_TIM_CCMR2_IC3F(12)                                             /*!< FL16_N8 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL32_N5                      ADV_TIMER_TIM_CCMR2_IC3F(13)                                             /*!< FL32_N5 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL32_N6                      ADV_TIMER_TIM_CCMR2_IC3F(14)                                             /*!< FL32_N6 */
#define ADV_TIMER_TIM_CCMR2_IC3F_FL32_N8                      ADV_TIMER_TIM_CCMR2_IC3F(15)                                             /*!< FL32_N8 */

/**
  * \brief Check the ADV_TIMER tim_ccmr2 ic3f bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid ic3f bits.
  * \retval 1 This is a valid ic3f bits.
  */
#define IS_ADV_TIMER_TIM_CCMR2_IC3F(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_NO_FILTER           ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL1_N2              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL1_N4              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL1_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL2_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL2_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL4_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL4_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL8_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL8_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL16_N5             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL16_N6             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL16_N8             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL32_N5             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL32_N6             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC3F_FL32_N8             )  \
                                                 )

#define ADV_TIMER_TIM_CCMR2_OC4FE                        BIT(24)                                      /*!< Output Compare 4 fast enable Refer to OC1FE description */
#define ADV_TIMER_TIM_CCMR2_OC4FE_OFS                    24U                                          /*!< ADV TIMER TIM CCMR2: OC4FE Bit Offset */
#define ADV_TIMER_TIM_CCMR2_OC4FE_VAL(regval)                (BIT(24) & ((uint32_t)(regval) << 24))        /*!< ADV TIMER TIM CCMR2: OC4FE Bit Value */  
#define ADV_TIMER_TIM_CCMR2_IC4PSC_MASK                  BITS(26,27)                                   /*!< ADV TIMER TIM CCMR2: IC4PSC Bit Mask */  
#define ADV_TIMER_TIM_CCMR2_IC4PSC_OFS                   26U                                          /*!< ADV TIMER TIM CCMR2: IC4PSC Bit Offset */
#define ADV_TIMER_TIM_CCMR2_IC4PSC(regval)               (BITS(26,27) & ((uint32_t)(regval) << 26))        /*!< ADV TIMER TIM CCMR2: IC4PSC Bit Value */  
#define ADV_TIMER_TIM_CCMR2_IC4PSC_NO_PRESCALER               ADV_TIMER_TIM_CCMR2_IC4PSC(0)                                             /*!< NO_PRESCALER */
#define ADV_TIMER_TIM_CCMR2_IC4PSC_2_EVENTS                   ADV_TIMER_TIM_CCMR2_IC4PSC(1)                                             /*!< 2_EVENTS */
#define ADV_TIMER_TIM_CCMR2_IC4PSC_4_EVENTS                   ADV_TIMER_TIM_CCMR2_IC4PSC(2)                                             /*!< 4_EVENTS */
#define ADV_TIMER_TIM_CCMR2_IC4PSC_8_EVENTS                   ADV_TIMER_TIM_CCMR2_IC4PSC(3)                                             /*!< 8_EVENTS */

/**
  * \brief Check the ADV_TIMER tim_ccmr2 ic4psc bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid ic4psc bits.
  * \retval 1 This is a valid ic4psc bits.
  */
#define IS_ADV_TIMER_TIM_CCMR2_IC4PSC(regval)                 (\
                                           ((regval) == ADV_TIMER_TIM_CCMR2_IC4PSC_NO_PRESCALER        ) || \
                                           ((regval) == ADV_TIMER_TIM_CCMR2_IC4PSC_2_EVENTS            ) || \
                                           ((regval) == ADV_TIMER_TIM_CCMR2_IC4PSC_4_EVENTS            ) || \
                                           ((regval) == ADV_TIMER_TIM_CCMR2_IC4PSC_8_EVENTS            )  \
                                                 )

#define ADV_TIMER_TIM_CCMR2_IC4F_MASK                    BITS(28,31)                                   /*!< ADV TIMER TIM CCMR2: IC4F Bit Mask */  
#define ADV_TIMER_TIM_CCMR2_IC4F_OFS                     28U                                          /*!< ADV TIMER TIM CCMR2: IC4F Bit Offset */
#define ADV_TIMER_TIM_CCMR2_IC4F(regval)                 (BITS(28,31) & ((uint32_t)(regval) << 28))        /*!< ADV TIMER TIM CCMR2: IC4F Bit Value */  
#define ADV_TIMER_TIM_CCMR2_IC4F_NO_FILTER                    ADV_TIMER_TIM_CCMR2_IC4F(0)                                             /*!< NO_FILTER */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL1_N2                       ADV_TIMER_TIM_CCMR2_IC4F(1)                                             /*!< FL1_N2 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL1_N4                       ADV_TIMER_TIM_CCMR2_IC4F(2)                                             /*!< FL1_N4 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL1_N8                       ADV_TIMER_TIM_CCMR2_IC4F(3)                                             /*!< FL1_N8 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL2_N6                       ADV_TIMER_TIM_CCMR2_IC4F(4)                                             /*!< FL2_N6 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL2_N8                       ADV_TIMER_TIM_CCMR2_IC4F(5)                                             /*!< FL2_N8 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL4_N6                       ADV_TIMER_TIM_CCMR2_IC4F(6)                                             /*!< FL4_N6 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL4_N8                       ADV_TIMER_TIM_CCMR2_IC4F(7)                                             /*!< FL4_N8 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL8_N6                       ADV_TIMER_TIM_CCMR2_IC4F(8)                                             /*!< FL8_N6 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL8_N8                       ADV_TIMER_TIM_CCMR2_IC4F(9)                                             /*!< FL8_N8 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL16_N5                      ADV_TIMER_TIM_CCMR2_IC4F(10)                                             /*!< FL16_N5 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL16_N6                      ADV_TIMER_TIM_CCMR2_IC4F(11)                                             /*!< FL16_N6 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL16_N8                      ADV_TIMER_TIM_CCMR2_IC4F(12)                                             /*!< FL16_N8 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL32_N5                      ADV_TIMER_TIM_CCMR2_IC4F(13)                                             /*!< FL32_N5 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL32_N6                      ADV_TIMER_TIM_CCMR2_IC4F(14)                                             /*!< FL32_N6 */
#define ADV_TIMER_TIM_CCMR2_IC4F_FL32_N8                      ADV_TIMER_TIM_CCMR2_IC4F(15)                                             /*!< FL32_N8 */

/**
  * \brief Check the ADV_TIMER tim_ccmr2 ic4f bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid ic4f bits.
  * \retval 1 This is a valid ic4f bits.
  */
#define IS_ADV_TIMER_TIM_CCMR2_IC4F(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_NO_FILTER           ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL1_N2              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL1_N4              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL1_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL2_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL2_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL4_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL4_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL8_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL8_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL16_N5             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL16_N6             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL16_N8             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL32_N5             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL32_N6             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCMR2_IC4F_FL32_N8             )  \
                                                 )

 /* ===== ADV TIMER TIM_CCER Register definition ===== */
#define ADV_TIMER_TIM_CCER_CC1E                         BIT(0)                                      /*!< Capture/Compare 1 output enable CC1 channel configured as output: - 1'b0:  disable:Off - OC1 is not active. OC1 level is then function of MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits. - 1'b1:  enable:On - OC1 signal is output on the corresponding output pin depending on MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits. CC1 channel configured as input: This bit determines if a capture of the counter value can actually be done into the input capture/compare register 1 (TIMx_CCR1) or not. 0: Capture disabled. 1: Capture enabled. On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1E active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC1E_OFS                     0U                                          /*!< ADV TIMER TIM CCER: CC1E Bit Offset */
#define ADV_TIMER_TIM_CCER_CC1E_VAL(regval)                 (BIT(0) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM CCER: CC1E Bit Value */  
#define ADV_TIMER_TIM_CCER_CC1E_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CC1E_ENABLE                       BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer cc1e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc1e bit.
  * \retval 1 This is a valid cc1e bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC1E(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCER_CC1E_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCER_CC1E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC1P                         BIT(1)                                      /*!< Capture/Compare 1 output polarity CC1 channel configured as output:  - 1'b0:  disable:OC1 active high - 1'b1:  enable:OC1 active low CC1 channel configured as input: CC1NP/CC1P bits select the active polarity of TI1FP1 and TI2FP1 for trigger or capture operations. 00: non-inverted/rising edge. The circuit is sensitive to TIxFP1 rising edge (capture or trigger operations in reset, external clock or trigger mode), TIxFP1 is not inverted (trigger operation in gated mode or encoder mode). 01: inverted/falling edge. The circuit is sensitive to TIxFP1 rising edge (capture or trigger operations in reset, external clock or trigger mode), TIxFP1 is inverted (trigger operation in gated mode or encoder mode). 10: reserved, do not use this configuration. 11: non-inverted/both edges/ The circuit is sensitive to both TIxFP1 rising and falling edges (capture or trigger operations in reset, external clock or trigger mode), TIxFP1 is not inverted (trigger operation in gated mode). This configuration must not be used in encoder mode. Note: This bit is not writable as soon as LOCK level 2 or 3 has been programmed (LOCK bits in TIMx_BDTR register). On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1P active bit takes the new value from the preloaded bit only when a Commutation event is generated. Note: This bit should not keep changing when CCX channel configured as input. for example, input is always inactive, but the CCXP or CCXNP is changed from 0 to 1, the implementation will mistakenly realize that there is a rising edge */
#define ADV_TIMER_TIM_CCER_CC1P_OFS                     1U                                          /*!< ADV TIMER TIM CCER: CC1P Bit Offset */
#define ADV_TIMER_TIM_CCER_CC1P_VAL(regval)                 (BIT(1) & ((uint32_t)(regval) << 1))        /*!< ADV TIMER TIM CCER: CC1P Bit Value */  
#define ADV_TIMER_TIM_CCER_CC1P_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CC1P_ENABLE                       BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer cc1p bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc1p bit.
  * \retval 1 This is a valid cc1p bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC1P(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCER_CC1P_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCER_CC1P_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC1NE                        BIT(2)                                      /*!< Capture/Compare 1 complementary output enable - 1'b0: disable: Off - OC1N is not active. OC1N level is then function of MOE, OSSI, OSSR, OIS1, OIS1N and CC1E bits. - 1'b1:  enable:On - OC1N signal is output on the corresponding output pin depending on MOE, OSSI, OSSR, OIS1, OIS1N and CC1E bits. On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1NE active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC1NE_OFS                    2U                                          /*!< ADV TIMER TIM CCER: CC1NE Bit Offset */
#define ADV_TIMER_TIM_CCER_CC1NE_VAL(regval)                (BIT(2) & ((uint32_t)(regval) << 2))        /*!< ADV TIMER TIM CCER: CC1NE Bit Value */  
#define ADV_TIMER_TIM_CCER_CC1NE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CC1NE_ENABLE                      BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer cc1ne bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc1ne bit.
  * \retval 1 This is a valid cc1ne bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC1NE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCER_CC1NE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CCER_CC1NE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC1NP                        BIT(3)                                      /*!< Capture/Compare 1 complementary output polarity CC1 channel configured as output: - 1'b0:  high: OC1N active high. - 1'b1:  low:OC1N active low. CC1 channel configured as input: This bit is used in conjunction with CC1P to define the polarity of TI1FP1 and TI2FP1. Refer to CC1P description. Note: This bit is not writable as soon as LOCK level 2 or 3 has been programmed (LOCK bits in TIMx_BDTR register) and CC1S='00' (channel configured as output). On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1NP active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC1NP_OFS                    3U                                          /*!< ADV TIMER TIM CCER: CC1NP Bit Offset */
#define ADV_TIMER_TIM_CCER_CC1NP_VAL(regval)                (BIT(3) & ((uint32_t)(regval) << 3))        /*!< ADV TIMER TIM CCER: CC1NP Bit Value */  
#define ADV_TIMER_TIM_CCER_CC1NP_HIGH                        0x0UL                                            /*!< HIGH */
#define ADV_TIMER_TIM_CCER_CC1NP_LOW                         BIT(3)                                                   /*!< LOW */

/**
  * \brief Check the ADV_TIMER tim_ccer cc1np bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc1np bit.
  * \retval 1 This is a valid cc1np bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC1NP(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCER_CC1NP_HIGH                ) || \
                                            ((regval) == ADV_TIMER_TIM_CCER_CC1NP_LOW                 )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC2E                         BIT(4)                                      /*!< Capture/Compare 2 output enable CC1 channel configured as output: - 1'b0:  disable:Off - OC1 is not active. OC1 level is then function of MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits. - 1'b1:  enable:On - OC1 signal is output on the corresponding output pin depending on MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits. CC1 channel configured as input: This bit determines if a capture of the counter value can actually be done into the input capture/compare register 1 (TIMx_CCR1) or not. 0: Capture disabled. 1: Capture enabled. On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1E active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC2E_OFS                     4U                                          /*!< ADV TIMER TIM CCER: CC2E Bit Offset */
#define ADV_TIMER_TIM_CCER_CC2E_VAL(regval)                 (BIT(4) & ((uint32_t)(regval) << 4))        /*!< ADV TIMER TIM CCER: CC2E Bit Value */  
#define ADV_TIMER_TIM_CCER_CC2E_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CC2E_ENABLE                       BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer cc2e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc2e bit.
  * \retval 1 This is a valid cc2e bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC2E(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCER_CC2E_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCER_CC2E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC2P                         BIT(5)                                      /*!< Capture/Compare 2 output polarity CC1 channel configured as output:  - 1'b0:  disable:OC1 active high - 1'b1:  enable:OC1 active low CC1 channel configured as input: CC1NP/CC1P bits select the active polarity of TI1FP1 and TI2FP1 for trigger or capture operations. 00: non-inverted/rising edge. The circuit is sensitive to TIxFP1 rising edge (capture or trigger operations in reset, external clock or trigger mode), TIxFP1 is not inverted (trigger operation in gated mode or encoder mode). 01: inverted/falling edge. The circuit is sensitive to TIxFP1 falling edge (capture or trigger operations in reset, external clock or trigger mode), TIxFP1 is inverted (trigger operation in gated mode or encoder mode). 10: reserved, do not use this configuration. 11: non-inverted/both edges/ The circuit is sensitive to both TIxFP1 rising and falling edges (capture or trigger operations in reset, external clock or trigger mode), TIxFP1 is not inverted (trigger operation in gated mode). This configuration must not be used in encoder mode. Note: This bit is not writable as soon as LOCK level 2 or 3 has been programmed (LOCK bits in TIMx_BDTR register). On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1P active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC2P_OFS                     5U                                          /*!< ADV TIMER TIM CCER: CC2P Bit Offset */
#define ADV_TIMER_TIM_CCER_CC2P_VAL(regval)                 (BIT(5) & ((uint32_t)(regval) << 5))        /*!< ADV TIMER TIM CCER: CC2P Bit Value */  
#define ADV_TIMER_TIM_CCER_CC2P_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CC2P_ENABLE                       BIT(5)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer cc2p bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc2p bit.
  * \retval 1 This is a valid cc2p bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC2P(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCER_CC2P_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCER_CC2P_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC2NE                        BIT(6)                                      /*!< Capture/Compare 2 complementary output enable - 1'b0: disable: Off - OC1N is not active. OC1N level is then function of MOE, OSSI, OSSR, OIS1, OIS1N and CC1E bits. - 1'b1:  enable:On - OC1N signal is output on the corresponding output pin depending on MOE, OSSI, OSSR, OIS1, OIS1N and CC1E bits. On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1NE active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC2NE_OFS                    6U                                          /*!< ADV TIMER TIM CCER: CC2NE Bit Offset */
#define ADV_TIMER_TIM_CCER_CC2NE_VAL(regval)                (BIT(6) & ((uint32_t)(regval) << 6))        /*!< ADV TIMER TIM CCER: CC2NE Bit Value */  
#define ADV_TIMER_TIM_CCER_CC2NE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CC2NE_ENABLE                      BIT(6)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer cc2ne bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc2ne bit.
  * \retval 1 This is a valid cc2ne bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC2NE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCER_CC2NE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CCER_CC2NE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC2NP                        BIT(7)                                      /*!< Capture/Compare 2 complementary output polarity CC1 channel configured as output: - 1'b0:  high: OC1N active high. - 1'b1:  low:OC1N active low. CC1 channel configured as input: This bit is used in conjunction with CC1P to define the polarity of TI1FP1 and TI2FP1. Refer to CC1P description. Note: This bit is not writable as soon as LOCK level 2 or 3 has been programmed (LOCK bits in TIMx_BDTR register) and CC1S='00' (channel configured as output). On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1NP active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC2NP_OFS                    7U                                          /*!< ADV TIMER TIM CCER: CC2NP Bit Offset */
#define ADV_TIMER_TIM_CCER_CC2NP_VAL(regval)                (BIT(7) & ((uint32_t)(regval) << 7))        /*!< ADV TIMER TIM CCER: CC2NP Bit Value */  
#define ADV_TIMER_TIM_CCER_CC2NP_HIGH                        0x0UL                                            /*!< HIGH */
#define ADV_TIMER_TIM_CCER_CC2NP_LOW                         BIT(7)                                                   /*!< LOW */

/**
  * \brief Check the ADV_TIMER tim_ccer cc2np bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc2np bit.
  * \retval 1 This is a valid cc2np bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC2NP(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCER_CC2NP_HIGH                ) || \
                                            ((regval) == ADV_TIMER_TIM_CCER_CC2NP_LOW                 )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC3E                         BIT(8)                                      /*!< Capture/Compare 3 output enable CC1 channel configured as output: - 1'b0:  disable:Off - OC1 is not active. OC1 level is then function of MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits. - 1'b1:  enable:On - OC1 signal is output on the corresponding output pin depending on MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits. CC1 channel configured as input: This bit determines if a capture of the counter value can actually be done into the input capture/compare register 1 (TIMx_CCR1) or not. 0: Capture disabled. 1: Capture enabled. On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1E active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC3E_OFS                     8U                                          /*!< ADV TIMER TIM CCER: CC3E Bit Offset */
#define ADV_TIMER_TIM_CCER_CC3E_VAL(regval)                 (BIT(8) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM CCER: CC3E Bit Value */  
#define ADV_TIMER_TIM_CCER_CC3E_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CC3E_ENABLE                       BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer cc3e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc3e bit.
  * \retval 1 This is a valid cc3e bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC3E(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCER_CC3E_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCER_CC3E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC3P                         BIT(9)                                      /*!< Capture/Compare 3 output polarity CC1 channel configured as output:  - 1'b0:  high:OC1 active high - 1'b1:  low:OC1 active low CC1 channel configured as input: CC1NP/CC1P bits select the active polarity of TI1FP1 and TI2FP1 for trigger or capture operations. 00: non-inverted/rising edge. The circuit is sensitive to TIxFP1 rising edge (capture or trigger operations in reset, external clock or trigger mode), TIxFP1 is not inverted (trigger operation in gated mode or encoder mode). 01: inverted/falling edge. The circuit is sensitive to TIxFP1 falling edge (capture or trigger operations in reset, external clock or trigger mode), TIxFP1 is inverted (trigger operation in gated mode or encoder mode). 10: reserved, do not use this configuration. 11: non-inverted/both edges/ The circuit is sensitive to both TIxFP1 rising and falling edges (capture or trigger operations in reset, external clock or trigger mode), TIxFP1 is not inverted (trigger operation in gated mode). This configuration must not be used in encoder mode. Note: This bit is not writable as soon as LOCK level 2 or 3 has been programmed (LOCK bits in TIMx_BDTR register). On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1P active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC3P_OFS                     9U                                          /*!< ADV TIMER TIM CCER: CC3P Bit Offset */
#define ADV_TIMER_TIM_CCER_CC3P_VAL(regval)                 (BIT(9) & ((uint32_t)(regval) << 9))        /*!< ADV TIMER TIM CCER: CC3P Bit Value */  
#define ADV_TIMER_TIM_CCER_CC3P_HIGH                         0x0UL                                            /*!< HIGH */
#define ADV_TIMER_TIM_CCER_CC3P_LOW                          BIT(9)                                                   /*!< LOW */

/**
  * \brief Check the ADV_TIMER tim_ccer cc3p bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc3p bit.
  * \retval 1 This is a valid cc3p bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC3P(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCER_CC3P_HIGH                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCER_CC3P_LOW                 )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC3NE                        BIT(10)                                      /*!< Capture/Compare 3 complementary output enable - 1'b0: disable: Off - OC1N is not active. OC1N level is then function of MOE, OSSI, OSSR, OIS1, OIS1N and CC1E bits. - 1'b1: enable:On - OC1N signal is output on the corresponding output pin depending on MOE, OSSI, OSSR, OIS1, OIS1N and CC1E bits. On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1NE active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC3NE_OFS                    10U                                          /*!< ADV TIMER TIM CCER: CC3NE Bit Offset */
#define ADV_TIMER_TIM_CCER_CC3NE_VAL(regval)                (BIT(10) & ((uint32_t)(regval) << 10))        /*!< ADV TIMER TIM CCER: CC3NE Bit Value */  
#define ADV_TIMER_TIM_CCER_CC3NE_DISABLE                     0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CC3NE_ENABLE                      BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer cc3ne bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc3ne bit.
  * \retval 1 This is a valid cc3ne bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC3NE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCER_CC3NE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_CCER_CC3NE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC3NP                        BIT(11)                                      /*!< Capture/Compare 3 complementary output polarity CC1 channel configured as output: - 1'b0:  high: OC1N active high. - 1'b1:  low:OC1N active low. CC1 channel configured as input: This bit is used in conjunction with CC1P to define the polarity of TI1FP1 and TI2FP1. Refer to CC1P description. Note: This bit is not writable as soon as LOCK level 2 or 3 has been programmed (LOCK bits in TIMx_BDTR register) and CC1S='00' (channel configured as output). On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1NP active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC3NP_OFS                    11U                                          /*!< ADV TIMER TIM CCER: CC3NP Bit Offset */
#define ADV_TIMER_TIM_CCER_CC3NP_VAL(regval)                (BIT(11) & ((uint32_t)(regval) << 11))        /*!< ADV TIMER TIM CCER: CC3NP Bit Value */  
#define ADV_TIMER_TIM_CCER_CC3NP_HIGH                        0x0UL                                            /*!< HIGH */
#define ADV_TIMER_TIM_CCER_CC3NP_LOW                         BIT(11)                                                  /*!< LOW */

/**
  * \brief Check the ADV_TIMER tim_ccer cc3np bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc3np bit.
  * \retval 1 This is a valid cc3np bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC3NP(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_CCER_CC3NP_HIGH                ) || \
                                            ((regval) == ADV_TIMER_TIM_CCER_CC3NP_LOW                 )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC4E                         BIT(12)                                      /*!< Capture/Compare 4 output enable CC1 channel configured as output: - 1'b0:  disable:Off - OC1 is not active. OC1 level is then function of MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits. - 1'b1:  enable:On - OC1 signal is output on the corresponding output pin depending on MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits. CC1 channel configured as input: This bit determines if a capture of the counter value can actually be done into the input capture/compare register 1 (TIMx_CCR1) or not. 0: Capture disabled. 1: Capture enabled. On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1E active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC4E_OFS                     12U                                          /*!< ADV TIMER TIM CCER: CC4E Bit Offset */
#define ADV_TIMER_TIM_CCER_CC4E_VAL(regval)                 (BIT(12) & ((uint32_t)(regval) << 12))        /*!< ADV TIMER TIM CCER: CC4E Bit Value */  
#define ADV_TIMER_TIM_CCER_CC4E_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CC4E_ENABLE                       BIT(12)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer cc4e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc4e bit.
  * \retval 1 This is a valid cc4e bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC4E(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCER_CC4E_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCER_CC4E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CC4P                         BIT(13)                                      /*!< Capture/Compare 4 output polarity CC1 channel configured as output:  - 1'b0:  high:OC1 active high - 1'b1:  low:OC1 active low CC1 channel configured as input: CC1NP/CC1P bits select the active polarity of TI1FP1 and TI2FP1 for trigger or capture operations. 00: non-inverted/rising edge. The circuit is sensitive to TIxFP1 rising edge (capture or trigger operations in reset, external clock or trigger mode), TIxFP1 is not inverted (trigger operation in gated mode or encoder mode). 01: inverted/falling edge. The circuit is sensitive to TIxFP1 falling edge (capture or trigger operations in reset, external clock or trigger mode), TIxFP1 is inverted (trigger operation in gated mode or encoder mode). 10: reserved, do not use this configuration. 11: non-inverted/both edges/ The circuit is sensitive to both TIxFP1 rising and falling edges (capture or trigger operations in reset, external clock or trigger mode), TIxFP1 is not inverted (trigger operation in gated mode). This configuration must not be used in encoder mode. Note: This bit is not writable as soon as LOCK level 2 or 3 has been programmed (LOCK bits in TIMx_BDTR register). On channels having a complementary output, this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1P active bit takes the new value from the preloaded bit only when a Commutation event is generated. */
#define ADV_TIMER_TIM_CCER_CC4P_OFS                     13U                                          /*!< ADV TIMER TIM CCER: CC4P Bit Offset */
#define ADV_TIMER_TIM_CCER_CC4P_VAL(regval)                 (BIT(13) & ((uint32_t)(regval) << 13))        /*!< ADV TIMER TIM CCER: CC4P Bit Value */  
#define ADV_TIMER_TIM_CCER_CC4P_HIGH                         0x0UL                                            /*!< HIGH */
#define ADV_TIMER_TIM_CCER_CC4P_LOW                          BIT(13)                                                  /*!< LOW */

/**
  * \brief Check the ADV_TIMER tim_ccer cc4p bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cc4p bit.
  * \retval 1 This is a valid cc4p bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CC4P(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCER_CC4P_HIGH                ) || \
                                             ((regval) == ADV_TIMER_TIM_CCER_CC4P_LOW                 )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CO1E                         BIT(14)                                      /*!< Channel 1 Complementary output enable - 1'b0:  disable:channel 1 Complementary output(OC1N) not implemented - 1'b1:  enable:channel 1 Complementary output(OC1N) implemented Note: this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1E active bit takes the new value from the preloaded bit only when a Commutation event is generated. Note: This bit is not writable as soon as LOCK level 2 or 3 has been programmed (LOCK bits in TIMx_BDTR register) */
#define ADV_TIMER_TIM_CCER_CO1E_OFS                     14U                                          /*!< ADV TIMER TIM CCER: CO1E Bit Offset */
#define ADV_TIMER_TIM_CCER_CO1E_VAL(regval)                 (BIT(14) & ((uint32_t)(regval) << 14))        /*!< ADV TIMER TIM CCER: CO1E Bit Value */  
#define ADV_TIMER_TIM_CCER_CO1E_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CO1E_ENABLE                       BIT(14)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer co1e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid co1e bit.
  * \retval 1 This is a valid co1e bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CO1E(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCER_CO1E_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCER_CO1E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CO2E                         BIT(15)                                      /*!< Channel 2 Complementary output enable - 1'b0:  disable:channel 2 Complementary output(OC2N) not implemented - 1'b1:  enable:channel 2 Complementary output(OC2N) implemented Note: this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1E active bit takes the new value from the preloaded bit only when a Commutation event is generated. Note: This bit is not writable as soon as LOCK level 2 or 3 has been programmed (LOCK bits in TIMx_BDTR register) */
#define ADV_TIMER_TIM_CCER_CO2E_OFS                     15U                                          /*!< ADV TIMER TIM CCER: CO2E Bit Offset */
#define ADV_TIMER_TIM_CCER_CO2E_VAL(regval)                 (BIT(15) & ((uint32_t)(regval) << 15))        /*!< ADV TIMER TIM CCER: CO2E Bit Value */  
#define ADV_TIMER_TIM_CCER_CO2E_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CO2E_ENABLE                       BIT(15)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer co2e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid co2e bit.
  * \retval 1 This is a valid co2e bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CO2E(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCER_CO2E_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCER_CO2E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CO3E                         BIT(16)                                      /*!< Channel 3 Complementary output enable - 1'b0: disable: channel 3 Complementary output(OC3N) not implemented - 1'b1:  enable:channel 3 Complementary output(OC3N) implemented Note: this bit is preloaded. If the CCPC bit is set in the TIMx_CR2 register then the CC1E active bit takes the new value from the preloaded bit only when a Commutation event is generated. Note: This bit is not writable as soon as LOCK level 2 or 3 has been programmed (LOCK bits in TIMx_BDTR register) */
#define ADV_TIMER_TIM_CCER_CO3E_OFS                     16U                                          /*!< ADV TIMER TIM CCER: CO3E Bit Offset */
#define ADV_TIMER_TIM_CCER_CO3E_VAL(regval)                 (BIT(16) & ((uint32_t)(regval) << 16))        /*!< ADV TIMER TIM CCER: CO3E Bit Value */  
#define ADV_TIMER_TIM_CCER_CO3E_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CO3E_ENABLE                       BIT(16)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer co3e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid co3e bit.
  * \retval 1 This is a valid co3e bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CO3E(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCER_CO3E_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCER_CO3E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_CCER_CO4E                         BIT(17)                                      /*!< Channel 4 Complementary output enable - 1'b0:  disable:channel 4 Complementary output(OC4N) not implemented - 1'b1:  enable:channel 4 Complementary output(OC4N) implemented Note: this bit is constant, CO4E equal to 0, because the channel 4 don't have Complementary output. Note: This bit is not writable as soon as LOCK level 2 or 3 has been programmed (LOCK bits in TIMx_BDTR register) */
#define ADV_TIMER_TIM_CCER_CO4E_DISABLE                      ((uint32_t)(0) << 17)                                              /*!< DISABLE */
#define ADV_TIMER_TIM_CCER_CO4E_ENABLE                       ((uint32_t)(1) << 17)                                              /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_ccer co4e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid co4e bit.
  * \retval 1 This is a valid co4e bit.
  */
#define IS_ADV_TIMER_TIM_CCER_CO4E(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_CCER_CO4E_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_CCER_CO4E_ENABLE              )  \
                                                 )

 /* ===== ADV TIMER TIM_CNT Register definition ===== */
#define ADV_TIMER_TIM_CNT_CNT_MASK                     BITS(0,31)                                   /*!< ADV TIMER TIM CNT: CNT Bit Mask */  
#define ADV_TIMER_TIM_CNT_CNT_OFS                      0U                                          /*!< ADV TIMER TIM CNT: CNT Bit Offset */
#define ADV_TIMER_TIM_CNT_CNT(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM CNT: CNT Bit Value */  
 
 /* ===== ADV TIMER TIM_PSC Register definition ===== */
#define ADV_TIMER_TIM_PSC_PSC_MASK                     BITS(0,31)                                   /*!< ADV TIMER TIM PSC: PSC Bit Mask */  
#define ADV_TIMER_TIM_PSC_PSC_OFS                      0U                                          /*!< ADV TIMER TIM PSC: PSC Bit Offset */
#define ADV_TIMER_TIM_PSC_PSC(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM PSC: PSC Bit Value */  
 
 /* ===== ADV TIMER TIM_ARR Register definition ===== */
#define ADV_TIMER_TIM_ARR_ARR_MASK                     BITS(0,31)                                   /*!< ADV TIMER TIM ARR: ARR Bit Mask */  
#define ADV_TIMER_TIM_ARR_ARR_OFS                      0U                                          /*!< ADV TIMER TIM ARR: ARR Bit Offset */
#define ADV_TIMER_TIM_ARR_ARR(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM ARR: ARR Bit Value */  
 
 /* ===== ADV TIMER TIM_RCR Register definition ===== */
#define ADV_TIMER_TIM_RCR_REP_MASK                     BITS(0,7)                                   /*!< ADV TIMER TIM RCR: REP Bit Mask */  
#define ADV_TIMER_TIM_RCR_REP_OFS                      0U                                          /*!< ADV TIMER TIM RCR: REP Bit Offset */
#define ADV_TIMER_TIM_RCR_REP(regval)                  (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM RCR: REP Bit Value */  
#define ADV_TIMER_TIM_RCR_GC4C_MASK                    BITS(8,10)                                   /*!< ADV TIMER TIM RCR: GC4C Bit Mask */  
#define ADV_TIMER_TIM_RCR_GC4C_OFS                     8U                                          /*!< ADV TIMER TIM RCR: GC4C Bit Offset */
#define ADV_TIMER_TIM_RCR_GC4C(regval)                 (BITS(8,10) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM RCR: GC4C Bit Value */  
#define ADV_TIMER_TIM_RCR_IC_PSC_REP_EN_MASK           BITS(11,14)                                   /*!< ADV TIMER TIM RCR: IC_PSC_REP_EN Bit Mask */  
#define ADV_TIMER_TIM_RCR_IC_PSC_REP_EN_OFS            11U                                          /*!< ADV TIMER TIM RCR: IC_PSC_REP_EN Bit Offset */
#define ADV_TIMER_TIM_RCR_IC_PSC_REP_EN(regval)        (BITS(11,14) & ((uint32_t)(regval) << 11))        /*!< ADV TIMER TIM RCR: IC_PSC_REP_EN Bit Value */  
 
 /* ===== ADV TIMER TIM_CCR1 Register definition ===== */
#define ADV_TIMER_TIM_CCR1_CCR1_MASK                    BITS(0,31)                                   /*!< ADV TIMER TIM CCR1: CCR1 Bit Mask */  
#define ADV_TIMER_TIM_CCR1_CCR1_OFS                     0U                                          /*!< ADV TIMER TIM CCR1: CCR1 Bit Offset */
#define ADV_TIMER_TIM_CCR1_CCR1(regval)                 (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM CCR1: CCR1 Bit Value */  
 
 /* ===== ADV TIMER TIM_CCR2 Register definition ===== */
#define ADV_TIMER_TIM_CCR2_CCR2_MASK                    BITS(0,31)                                   /*!< ADV TIMER TIM CCR2: CCR2 Bit Mask */  
#define ADV_TIMER_TIM_CCR2_CCR2_OFS                     0U                                          /*!< ADV TIMER TIM CCR2: CCR2 Bit Offset */
#define ADV_TIMER_TIM_CCR2_CCR2(regval)                 (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM CCR2: CCR2 Bit Value */  
 
 /* ===== ADV TIMER TIM_CCR3 Register definition ===== */
#define ADV_TIMER_TIM_CCR3_CCR3_MASK                    BITS(0,31)                                   /*!< ADV TIMER TIM CCR3: CCR3 Bit Mask */  
#define ADV_TIMER_TIM_CCR3_CCR3_OFS                     0U                                          /*!< ADV TIMER TIM CCR3: CCR3 Bit Offset */
#define ADV_TIMER_TIM_CCR3_CCR3(regval)                 (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM CCR3: CCR3 Bit Value */  
 
 /* ===== ADV TIMER TIM_CCR4 Register definition ===== */
#define ADV_TIMER_TIM_CCR4_CCR4_MASK                    BITS(0,31)                                   /*!< ADV TIMER TIM CCR4: CCR4 Bit Mask */  
#define ADV_TIMER_TIM_CCR4_CCR4_OFS                     0U                                          /*!< ADV TIMER TIM CCR4: CCR4 Bit Offset */
#define ADV_TIMER_TIM_CCR4_CCR4(regval)                 (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM CCR4: CCR4 Bit Value */  
 
 /* ===== ADV TIMER TIM_BDTR Register definition ===== */
#define ADV_TIMER_TIM_BDTR_DTG_MASK                     BITS(0,7)                                   /*!< ADV TIMER TIM BDTR: DTG Bit Mask */  
#define ADV_TIMER_TIM_BDTR_DTG_OFS                      0U                                          /*!< ADV TIMER TIM BDTR: DTG Bit Offset */
#define ADV_TIMER_TIM_BDTR_DTG(regval)                  (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM BDTR: DTG Bit Value */  
#define ADV_TIMER_TIM_BDTR_LOCK_MASK                    BITS(8,9)                                   /*!< ADV TIMER TIM BDTR: LOCK Bit Mask */  
#define ADV_TIMER_TIM_BDTR_LOCK_OFS                     8U                                          /*!< ADV TIMER TIM BDTR: LOCK Bit Offset */
#define ADV_TIMER_TIM_BDTR_LOCK(regval)                 (BITS(8,9) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM BDTR: LOCK Bit Value */  
#define ADV_TIMER_TIM_BDTR_LOCK_LOCK_OFF                     ADV_TIMER_TIM_BDTR_LOCK(0)                                              /*!< LOCK_OFF */
#define ADV_TIMER_TIM_BDTR_LOCK_LOCK_LEVEL1                  ADV_TIMER_TIM_BDTR_LOCK(1)                                              /*!< LOCK_LEVEL1 */
#define ADV_TIMER_TIM_BDTR_LOCK_LOCK_LEVEL2                  ADV_TIMER_TIM_BDTR_LOCK(2)                                              /*!< LOCK_LEVEL2 */
#define ADV_TIMER_TIM_BDTR_LOCK_LOCK_LEVEL3                  ADV_TIMER_TIM_BDTR_LOCK(3)                                              /*!< LOCK_LEVEL3 */

/**
  * \brief Check the ADV_TIMER tim_bdtr lock bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid lock bits.
  * \retval 1 This is a valid lock bits.
  */
#define IS_ADV_TIMER_TIM_BDTR_LOCK(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_BDTR_LOCK_LOCK_OFF            ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_LOCK_LOCK_LEVEL1         ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_LOCK_LOCK_LEVEL2         ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_LOCK_LOCK_LEVEL3         )  \
                                                 )

#define ADV_TIMER_TIM_BDTR_OSSI                         BIT(10)                                      /*!< Off-state selection for Idle mode This bit is used when MOE=0 due to a break event or by a software write, on channels configured as outputs. - 1'b0: disable: When inactive, OC/OCN outputs are disabled (the timer releases the output control which is taken over by the GPIO logic and which imposes a Hi-Z state).  - 1'b1: enable: When inactive, OC/OCN outputs are first forced with their inactive level then forced to their idle level after the deadtime. The timer maintains its control over the output. */
#define ADV_TIMER_TIM_BDTR_OSSI_OFS                     10U                                          /*!< ADV TIMER TIM BDTR: OSSI Bit Offset */
#define ADV_TIMER_TIM_BDTR_OSSI_VAL(regval)                 (BIT(10) & ((uint32_t)(regval) << 10))        /*!< ADV TIMER TIM BDTR: OSSI Bit Value */  
#define ADV_TIMER_TIM_BDTR_OSSI_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_BDTR_OSSI_ENABLE                       BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_bdtr ossi bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ossi bit.
  * \retval 1 This is a valid ossi bit.
  */
#define IS_ADV_TIMER_TIM_BDTR_OSSI(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_BDTR_OSSI_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_OSSI_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_BDTR_OSSR                         BIT(11)                                      /*!< Off-state selection for Run mode This bit is used when MOE=1 on channels. OSSR is not implemented if complementary output is implemented in the timer. - 1'b0:  disable: OC/OCN outputs are disabled as soon as CCxE=0 or CCxNE=0 (the timer releases the output control which is taken over by the GPIO logic, which forces a Hi-Z state). - 1'b1: enable: OC/OCN outputs are enabled with their inactive level as soon as CCxE=0 or CCxNE=0 (the output is still controlled by the timer). */
#define ADV_TIMER_TIM_BDTR_OSSR_OFS                     11U                                          /*!< ADV TIMER TIM BDTR: OSSR Bit Offset */
#define ADV_TIMER_TIM_BDTR_OSSR_VAL(regval)                 (BIT(11) & ((uint32_t)(regval) << 11))        /*!< ADV TIMER TIM BDTR: OSSR Bit Value */  
#define ADV_TIMER_TIM_BDTR_OSSR_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_BDTR_OSSR_ENABLE                       BIT(11)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_bdtr ossr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ossr bit.
  * \retval 1 This is a valid ossr bit.
  */
#define IS_ADV_TIMER_TIM_BDTR_OSSR(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_BDTR_OSSR_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_OSSR_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_BDTR_BKE                          BIT(12)                                      /*!< Break enable This bit enables the complete break protection (including all sources connected to bk_acth and BKIN sources). - 1'b0:  disable:Break function disabled - 1'b1:  enable:Break function enabled */
#define ADV_TIMER_TIM_BDTR_BKE_OFS                      12U                                          /*!< ADV TIMER TIM BDTR: BKE Bit Offset */
#define ADV_TIMER_TIM_BDTR_BKE_VAL(regval)                  (BIT(12) & ((uint32_t)(regval) << 12))        /*!< ADV TIMER TIM BDTR: BKE Bit Value */  
#define ADV_TIMER_TIM_BDTR_BKE_DISABLE                       0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_BDTR_BKE_ENABLE                        BIT(12)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_bdtr bke bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bke bit.
  * \retval 1 This is a valid bke bit.
  */
#define IS_ADV_TIMER_TIM_BDTR_BKE(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKE_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_BDTR_BKP                          BIT(13)                                      /*!< Break polarity - 1'b0:  low:Break input BRK is active low - 1'b1:  high:Break input BRK is active high */
#define ADV_TIMER_TIM_BDTR_BKP_OFS                      13U                                          /*!< ADV TIMER TIM BDTR: BKP Bit Offset */
#define ADV_TIMER_TIM_BDTR_BKP_VAL(regval)                  (BIT(13) & ((uint32_t)(regval) << 13))        /*!< ADV TIMER TIM BDTR: BKP Bit Value */  
#define ADV_TIMER_TIM_BDTR_BKP_LOW                           0x0UL                                            /*!< LOW */
#define ADV_TIMER_TIM_BDTR_BKP_HIGH                          BIT(13)                                                  /*!< HIGH */

/**
  * \brief Check the ADV_TIMER tim_bdtr bkp bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bkp bit.
  * \retval 1 This is a valid bkp bit.
  */
#define IS_ADV_TIMER_TIM_BDTR_BKP(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKP_LOW                 ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKP_HIGH                )  \
                                                 )

#define ADV_TIMER_TIM_BDTR_AOE                          BIT(14)                                      /*!< Automatic output enable - 1'b0:  software: MOE can be set only by software - 1'b1:  software_automatically: MOE can be set by software or automatically at the next update event (if none of the break inputs BRK and BRK2 is active) */
#define ADV_TIMER_TIM_BDTR_AOE_OFS                      14U                                          /*!< ADV TIMER TIM BDTR: AOE Bit Offset */
#define ADV_TIMER_TIM_BDTR_AOE_VAL(regval)                  (BIT(14) & ((uint32_t)(regval) << 14))        /*!< ADV TIMER TIM BDTR: AOE Bit Value */  
#define ADV_TIMER_TIM_BDTR_AOE_SOFTWARE                      0x0UL                                            /*!< SOFTWARE */
#define ADV_TIMER_TIM_BDTR_AOE_SOFTWARE_AUTOMATICALLY             BIT(14)                                                  /*!< SOFTWARE_AUTOMATICALLY */

/**
  * \brief Check the ADV_TIMER tim_bdtr aoe bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid aoe bit.
  * \retval 1 This is a valid aoe bit.
  */
#define IS_ADV_TIMER_TIM_BDTR_AOE(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_BDTR_AOE_SOFTWARE            ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_AOE_SOFTWARE_AUTOMATICALLY)  \
                                                 )

#define ADV_TIMER_TIM_BDTR_MOE                          BIT(15)                                      /*!< Main output enable This bit is cleared asynchronously by hardware as soon as one of the break inputs is active (BRK or BRK2). It is set by software or automatically depending on the AOE bit. It is acting only on the channels which are configured in output.  - 1'b0:  disable: In response to a break 2 event. OC and OCN outputs are disabled In response to a break event or if MOE is written to 0: OC and OCN outputs are disabled or forced to idle state depending on the OSSI bit. - 1'b1:  enable:OC and OCN outputs are enabled if their respective enable bits are set (CCxE, CCxNE in TIMx_CCER register). */
#define ADV_TIMER_TIM_BDTR_MOE_OFS                      15U                                          /*!< ADV TIMER TIM BDTR: MOE Bit Offset */
#define ADV_TIMER_TIM_BDTR_MOE_VAL(regval)                  (BIT(15) & ((uint32_t)(regval) << 15))        /*!< ADV TIMER TIM BDTR: MOE Bit Value */  
#define ADV_TIMER_TIM_BDTR_MOE_DISABLE                       0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_BDTR_MOE_ENABLE                        BIT(15)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_bdtr moe bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid moe bit.
  * \retval 1 This is a valid moe bit.
  */
#define IS_ADV_TIMER_TIM_BDTR_MOE(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_BDTR_MOE_DISABLE             ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_MOE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_BDTR_BKF_MASK                     BITS(16,19)                                   /*!< ADV TIMER TIM BDTR: BKF Bit Mask */  
#define ADV_TIMER_TIM_BDTR_BKF_OFS                      16U                                          /*!< ADV TIMER TIM BDTR: BKF Bit Offset */
#define ADV_TIMER_TIM_BDTR_BKF(regval)                  (BITS(16,19) & ((uint32_t)(regval) << 16))        /*!< ADV TIMER TIM BDTR: BKF Bit Value */  
#define ADV_TIMER_TIM_BDTR_BKF_NO_FILTER                     ADV_TIMER_TIM_BDTR_BKF(0)                                              /*!< NO_FILTER */
#define ADV_TIMER_TIM_BDTR_BKF_FL1_N2                        ADV_TIMER_TIM_BDTR_BKF(1)                                              /*!< FL1_N2 */
#define ADV_TIMER_TIM_BDTR_BKF_FL1_N4                        ADV_TIMER_TIM_BDTR_BKF(2)                                              /*!< FL1_N4 */
#define ADV_TIMER_TIM_BDTR_BKF_FL1_N8                        ADV_TIMER_TIM_BDTR_BKF(3)                                              /*!< FL1_N8 */
#define ADV_TIMER_TIM_BDTR_BKF_FL2_N6                        ADV_TIMER_TIM_BDTR_BKF(4)                                              /*!< FL2_N6 */
#define ADV_TIMER_TIM_BDTR_BKF_FL2_N8                        ADV_TIMER_TIM_BDTR_BKF(5)                                              /*!< FL2_N8 */
#define ADV_TIMER_TIM_BDTR_BKF_FL4_N6                        ADV_TIMER_TIM_BDTR_BKF(6)                                              /*!< FL4_N6 */
#define ADV_TIMER_TIM_BDTR_BKF_FL4_N8                        ADV_TIMER_TIM_BDTR_BKF(7)                                              /*!< FL4_N8 */
#define ADV_TIMER_TIM_BDTR_BKF_FL8_N6                        ADV_TIMER_TIM_BDTR_BKF(8)                                              /*!< FL8_N6 */
#define ADV_TIMER_TIM_BDTR_BKF_FL8_N8                        ADV_TIMER_TIM_BDTR_BKF(9)                                              /*!< FL8_N8 */
#define ADV_TIMER_TIM_BDTR_BKF_FL16_N5                       ADV_TIMER_TIM_BDTR_BKF(10)                                              /*!< FL16_N5 */
#define ADV_TIMER_TIM_BDTR_BKF_FL16_N6                       ADV_TIMER_TIM_BDTR_BKF(11)                                              /*!< FL16_N6 */
#define ADV_TIMER_TIM_BDTR_BKF_FL16_N8                       ADV_TIMER_TIM_BDTR_BKF(12)                                              /*!< FL16_N8 */
#define ADV_TIMER_TIM_BDTR_BKF_FL32_N5                       ADV_TIMER_TIM_BDTR_BKF(13)                                              /*!< FL32_N5 */
#define ADV_TIMER_TIM_BDTR_BKF_FL32_N6                       ADV_TIMER_TIM_BDTR_BKF(14)                                              /*!< FL32_N6 */
#define ADV_TIMER_TIM_BDTR_BKF_FL32_N8                       ADV_TIMER_TIM_BDTR_BKF(15)                                              /*!< FL32_N8 */

/**
  * \brief Check the ADV_TIMER tim_bdtr bkf bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid bkf bits.
  * \retval 1 This is a valid bkf bits.
  */
#define IS_ADV_TIMER_TIM_BDTR_BKF(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_NO_FILTER           ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL1_N2              ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL1_N4              ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL1_N8              ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL2_N6              ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL2_N8              ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL4_N6              ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL4_N8              ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL8_N6              ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL8_N8              ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL16_N5             ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL16_N6             ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL16_N8             ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL32_N5             ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL32_N6             ) || \
                                              ((regval) == ADV_TIMER_TIM_BDTR_BKF_FL32_N8             )  \
                                                 )

#define ADV_TIMER_TIM_BDTR_BK2F_MASK                    BITS(20,23)                                   /*!< ADV TIMER TIM BDTR: BK2F Bit Mask */  
#define ADV_TIMER_TIM_BDTR_BK2F_OFS                     20U                                          /*!< ADV TIMER TIM BDTR: BK2F Bit Offset */
#define ADV_TIMER_TIM_BDTR_BK2F(regval)                 (BITS(20,23) & ((uint32_t)(regval) << 20))        /*!< ADV TIMER TIM BDTR: BK2F Bit Value */  
#define ADV_TIMER_TIM_BDTR_BK2F_NO_FILTER                    ADV_TIMER_TIM_BDTR_BK2F(0)                                              /*!< NO_FILTER */
#define ADV_TIMER_TIM_BDTR_BK2F_FL1_N2                       ADV_TIMER_TIM_BDTR_BK2F(1)                                              /*!< FL1_N2 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL1_N4                       ADV_TIMER_TIM_BDTR_BK2F(2)                                              /*!< FL1_N4 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL1_N8                       ADV_TIMER_TIM_BDTR_BK2F(3)                                              /*!< FL1_N8 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL2_N6                       ADV_TIMER_TIM_BDTR_BK2F(4)                                              /*!< FL2_N6 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL2_N8                       ADV_TIMER_TIM_BDTR_BK2F(5)                                              /*!< FL2_N8 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL4_N6                       ADV_TIMER_TIM_BDTR_BK2F(6)                                              /*!< FL4_N6 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL4_N8                       ADV_TIMER_TIM_BDTR_BK2F(7)                                              /*!< FL4_N8 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL8_N6                       ADV_TIMER_TIM_BDTR_BK2F(8)                                              /*!< FL8_N6 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL8_N8                       ADV_TIMER_TIM_BDTR_BK2F(9)                                              /*!< FL8_N8 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL16_N5                      ADV_TIMER_TIM_BDTR_BK2F(10)                                              /*!< FL16_N5 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL16_N6                      ADV_TIMER_TIM_BDTR_BK2F(11)                                              /*!< FL16_N6 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL16_N8                      ADV_TIMER_TIM_BDTR_BK2F(12)                                              /*!< FL16_N8 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL32_N5                      ADV_TIMER_TIM_BDTR_BK2F(13)                                              /*!< FL32_N5 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL32_N6                      ADV_TIMER_TIM_BDTR_BK2F(14)                                              /*!< FL32_N6 */
#define ADV_TIMER_TIM_BDTR_BK2F_FL32_N8                      ADV_TIMER_TIM_BDTR_BK2F(15)                                              /*!< FL32_N8 */

/**
  * \brief Check the ADV_TIMER tim_bdtr bk2f bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid bk2f bits.
  * \retval 1 This is a valid bk2f bits.
  */
#define IS_ADV_TIMER_TIM_BDTR_BK2F(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_NO_FILTER           ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL1_N2              ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL1_N4              ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL1_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL2_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL2_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL4_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL4_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL8_N6              ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL8_N8              ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL16_N5             ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL16_N6             ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL16_N8             ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL32_N5             ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL32_N6             ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2F_FL32_N8             )  \
                                                 )

#define ADV_TIMER_TIM_BDTR_BK2E                         BIT(24)                                      /*!< Break 2 enable This bit enables the complete break 2 protection (including all sources connected to bk_acth and BKIN sources). - 1'b0:  disable:Break2 function disabled - 1'b1:  enable:Break2 function enabled Note: The BKIN2 must only be used with OSSR=OSSI=1. Note: This bit cannot be modified when LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register). */
#define ADV_TIMER_TIM_BDTR_BK2E_OFS                     24U                                          /*!< ADV TIMER TIM BDTR: BK2E Bit Offset */
#define ADV_TIMER_TIM_BDTR_BK2E_VAL(regval)                 (BIT(24) & ((uint32_t)(regval) << 24))        /*!< ADV TIMER TIM BDTR: BK2E Bit Value */  
#define ADV_TIMER_TIM_BDTR_BK2E_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_BDTR_BK2E_ENABLE                       BIT(24)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_bdtr bk2e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bk2e bit.
  * \retval 1 This is a valid bk2e bit.
  */
#define IS_ADV_TIMER_TIM_BDTR_BK2E(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2E_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_BDTR_BK2P                         BIT(25)                                      /*!< Break 2 polarity - 1'b0:  disable:Break input BRK2 is active low - 1'b1:  enable:Break input BRK2 is active high Note: This bit cannot be modified when LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register). */
#define ADV_TIMER_TIM_BDTR_BK2P_OFS                     25U                                          /*!< ADV TIMER TIM BDTR: BK2P Bit Offset */
#define ADV_TIMER_TIM_BDTR_BK2P_VAL(regval)                 (BIT(25) & ((uint32_t)(regval) << 25))        /*!< ADV TIMER TIM BDTR: BK2P Bit Value */  
#define ADV_TIMER_TIM_BDTR_BK2P_DISABLE                      0x0UL                                            /*!< DISABLE */
#define ADV_TIMER_TIM_BDTR_BK2P_ENABLE                       BIT(25)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_bdtr bk2p bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bk2p bit.
  * \retval 1 This is a valid bk2p bit.
  */
#define IS_ADV_TIMER_TIM_BDTR_BK2P(regval)                   (\
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2P_DISABLE             ) || \
                                             ((regval) == ADV_TIMER_TIM_BDTR_BK2P_ENABLE              )  \
                                                 )

 /* ===== ADV TIMER TIM_DCR Register definition ===== */
#define ADV_TIMER_TIM_DCR_DBA_MASK                     BITS(0,4)                                   /*!< ADV TIMER TIM DCR: DBA Bit Mask */  
#define ADV_TIMER_TIM_DCR_DBA_OFS                      0U                                          /*!< ADV TIMER TIM DCR: DBA Bit Offset */
#define ADV_TIMER_TIM_DCR_DBA(regval)                  (BITS(0,4) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM DCR: DBA Bit Value */  
#define ADV_TIMER_TIM_DCR_DBA_TIMX_CR1                      ADV_TIMER_TIM_DCR_DBA(0)                                               /*!< TIMX_CR1 */
#define ADV_TIMER_TIM_DCR_DBA_TIMX_CR2                      ADV_TIMER_TIM_DCR_DBA(1)                                               /*!< TIMX_CR2 */
#define ADV_TIMER_TIM_DCR_DBA_TIMX_SMCR                     ADV_TIMER_TIM_DCR_DBA(2)                                               /*!< TIMX_SMCR */

/**
  * \brief Check the ADV_TIMER tim_dcr dba bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid dba bits.
  * \retval 1 This is a valid dba bits.
  */
#define IS_ADV_TIMER_TIM_DCR_DBA(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_DCR_DBA_TIMX_CR1            ) || \
                                              ((regval) == ADV_TIMER_TIM_DCR_DBA_TIMX_CR2            ) || \
                                              ((regval) == ADV_TIMER_TIM_DCR_DBA_TIMX_SMCR           )  \
                                                 )

#define ADV_TIMER_TIM_DCR_DBL_MASK                     BITS(8,12)                                   /*!< ADV TIMER TIM DCR: DBL Bit Mask */  
#define ADV_TIMER_TIM_DCR_DBL_OFS                      8U                                          /*!< ADV TIMER TIM DCR: DBL Bit Offset */
#define ADV_TIMER_TIM_DCR_DBL(regval)                  (BITS(8,12) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM DCR: DBL Bit Value */  
#define ADV_TIMER_TIM_DCR_DBL_1_TRANSFER                    ADV_TIMER_TIM_DCR_DBL(0)                                               /*!< 1_TRANSFER */
#define ADV_TIMER_TIM_DCR_DBL_2_TRANSFER                    ADV_TIMER_TIM_DCR_DBL(1)                                               /*!< 2_TRANSFER */
#define ADV_TIMER_TIM_DCR_DBL_3_TRANSFER                    ADV_TIMER_TIM_DCR_DBL(2)                                               /*!< 3_TRANSFER */

/**
  * \brief Check the ADV_TIMER tim_dcr dbl bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid dbl bits.
  * \retval 1 This is a valid dbl bits.
  */
#define IS_ADV_TIMER_TIM_DCR_DBL(regval)                    (\
                                              ((regval) == ADV_TIMER_TIM_DCR_DBL_1_TRANSFER          ) || \
                                              ((regval) == ADV_TIMER_TIM_DCR_DBL_2_TRANSFER          ) || \
                                              ((regval) == ADV_TIMER_TIM_DCR_DBL_3_TRANSFER          )  \
                                                 )

#define ADV_TIMER_TIM_DCR_MAP_IDX                      BITS(16,20)                
#define ADV_TIMER_TIM_DCR_MAP_IDX_TIMX_CR1                  ((uint32_t)(0) << 16)                                               /*!< TIMX_CR1 */
#define ADV_TIMER_TIM_DCR_MAP_IDX_TIMX_CR2                  ((uint32_t)(1) << 16)                                               /*!< TIMX_CR2 */
#define ADV_TIMER_TIM_DCR_MAP_IDX_TIMX_SMCR                 ((uint32_t)(2) << 16)                                               /*!< TIMX_SMCR */

/**
  * \brief Check the ADV_TIMER tim_dcr map_idx bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid map_idx bits.
  * \retval 1 This is a valid map_idx bits.
  */
#define IS_ADV_TIMER_TIM_DCR_MAP_IDX(regval)                (\
                                          ((regval) == ADV_TIMER_TIM_DCR_MAP_IDX_TIMX_CR1            ) || \
                                          ((regval) == ADV_TIMER_TIM_DCR_MAP_IDX_TIMX_CR2            ) || \
                                          ((regval) == ADV_TIMER_TIM_DCR_MAP_IDX_TIMX_SMCR           )  \
                                                 )

 /* ===== ADV TIMER TIM_DMAR Register definition ===== */
#define ADV_TIMER_TIM_DMAR_DMAB_MASK                    BITS(0,31)                                   /*!< ADV TIMER TIM DMAR: DMAB Bit Mask */  
#define ADV_TIMER_TIM_DMAR_DMAB_OFS                     0U                                          /*!< ADV TIMER TIM DMAR: DMAB Bit Offset */
#define ADV_TIMER_TIM_DMAR_DMAB(regval)                 (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM DMAR: DMAB Bit Value */  
 
 /* ===== ADV TIMER TIM_VERSION Register definition ===== */
#define ADV_TIMER_TIM_VERSION_GIT_VER                      BITS(0,31)                
 
 /* ===== ADV TIMER TIM_IC_PSC_REP Register definition ===== */
#define ADV_TIMER_TIM_IC_PSC_REP_IC1PSC_REP_MASK              BITS(0,7)                                   /*!< ADV TIMER TIM IC PSC REP: IC1PSC_REP Bit Mask */  
#define ADV_TIMER_TIM_IC_PSC_REP_IC1PSC_REP_OFS               0U                                          /*!< ADV TIMER TIM IC PSC REP: IC1PSC_REP Bit Offset */
#define ADV_TIMER_TIM_IC_PSC_REP_IC1PSC_REP(regval)           (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM IC PSC REP: IC1PSC_REP Bit Value */  
#define ADV_TIMER_TIM_IC_PSC_REP_IC2PSC_REP_MASK              BITS(8,15)                                   /*!< ADV TIMER TIM IC PSC REP: IC2PSC_REP Bit Mask */  
#define ADV_TIMER_TIM_IC_PSC_REP_IC2PSC_REP_OFS               8U                                          /*!< ADV TIMER TIM IC PSC REP: IC2PSC_REP Bit Offset */
#define ADV_TIMER_TIM_IC_PSC_REP_IC2PSC_REP(regval)           (BITS(8,15) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM IC PSC REP: IC2PSC_REP Bit Value */  
#define ADV_TIMER_TIM_IC_PSC_REP_IC3PSC_REP_MASK              BITS(16,23)                                   /*!< ADV TIMER TIM IC PSC REP: IC3PSC_REP Bit Mask */  
#define ADV_TIMER_TIM_IC_PSC_REP_IC3PSC_REP_OFS               16U                                          /*!< ADV TIMER TIM IC PSC REP: IC3PSC_REP Bit Offset */
#define ADV_TIMER_TIM_IC_PSC_REP_IC3PSC_REP(regval)           (BITS(16,23) & ((uint32_t)(regval) << 16))        /*!< ADV TIMER TIM IC PSC REP: IC3PSC_REP Bit Value */  
#define ADV_TIMER_TIM_IC_PSC_REP_IC4PSC_REP_MASK              BITS(24,31)                                   /*!< ADV TIMER TIM IC PSC REP: IC4PSC_REP Bit Mask */  
#define ADV_TIMER_TIM_IC_PSC_REP_IC4PSC_REP_OFS               24U                                          /*!< ADV TIMER TIM IC PSC REP: IC4PSC_REP Bit Offset */
#define ADV_TIMER_TIM_IC_PSC_REP_IC4PSC_REP(regval)           (BITS(24,31) & ((uint32_t)(regval) << 24))        /*!< ADV TIMER TIM IC PSC REP: IC4PSC_REP Bit Value */  
 
 /* ===== ADV TIMER TIM_AF1 Register definition ===== */
#define ADV_TIMER_TIM_AF1_BKINE                        BIT(0)                                      /*!< BRK BKIN input enable This bit enables the BKIN alternate function input for the timer's BRK input.BKIN input is 'ORed' with the other BRK sources  - 1'b0:  disable:BKIN input disabled   - 1'b1:  enable:BKIN input enabled   Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF1_BKINE_OFS                    0U                                          /*!< ADV TIMER TIM AF1: BKINE Bit Offset */
#define ADV_TIMER_TIM_AF1_BKINE_VAL(regval)                (BIT(0) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM AF1: BKINE Bit Value */  
#define ADV_TIMER_TIM_AF1_BKINE_DISABLE                     0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF1_BKINE_ENABLE                      BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af1 bkine bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bkine bit.
  * \retval 1 This is a valid bkine bit.
  */
#define IS_ADV_TIMER_TIM_AF1_BKINE(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_AF1_BKINE_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_AF1_BKINE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_AF1_BKCMP1E                      BIT(1)                                      /*!< BRK COMP1 enable This bit enables the COMP1 for the timer's BRK input. COMP1 output is 'ORed' with the other BRK sources. - 1'b0:  disable:COMP1 input disabled - 1'b1:  ebabled:COMP1 input enabled Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF1_BKCMP1E_OFS                  1U                                          /*!< ADV TIMER TIM AF1: BKCMP1E Bit Offset */
#define ADV_TIMER_TIM_AF1_BKCMP1E_VAL(regval)              (BIT(1) & ((uint32_t)(regval) << 1))        /*!< ADV TIMER TIM AF1: BKCMP1E Bit Value */  
#define ADV_TIMER_TIM_AF1_BKCMP1E_DISABLE                   0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF1_BKCMP1E_EBABLED                   BIT(1)                                                   /*!< EBABLED */

/**
  * \brief Check the ADV_TIMER tim_af1 bkcmp1e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bkcmp1e bit.
  * \retval 1 This is a valid bkcmp1e bit.
  */
#define IS_ADV_TIMER_TIM_AF1_BKCMP1E(regval)                (\
                                          ((regval) == ADV_TIMER_TIM_AF1_BKCMP1E_DISABLE             ) || \
                                          ((regval) == ADV_TIMER_TIM_AF1_BKCMP1E_EBABLED             )  \
                                                 )

#define ADV_TIMER_TIM_AF1_BKCMP2E                      BIT(2)                                      /*!< BRK COMP2 enable This bit enables the COMP2 for the timer's BRK input. COMP2 output is 'ORed' with the other BRK sources. - 1'b0:  disable:COMP2 input disabled - 1'b1:  enable:COMP2 input enabled Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF1_BKCMP2E_OFS                  2U                                          /*!< ADV TIMER TIM AF1: BKCMP2E Bit Offset */
#define ADV_TIMER_TIM_AF1_BKCMP2E_VAL(regval)              (BIT(2) & ((uint32_t)(regval) << 2))        /*!< ADV TIMER TIM AF1: BKCMP2E Bit Value */  
#define ADV_TIMER_TIM_AF1_BKCMP2E_DISABLE                   0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF1_BKCMP2E_ENABLE                    BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af1 bkcmp2e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bkcmp2e bit.
  * \retval 1 This is a valid bkcmp2e bit.
  */
#define IS_ADV_TIMER_TIM_AF1_BKCMP2E(regval)                (\
                                          ((regval) == ADV_TIMER_TIM_AF1_BKCMP2E_DISABLE             ) || \
                                          ((regval) == ADV_TIMER_TIM_AF1_BKCMP2E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_AF1_BKDF1BK0E                    BIT(8)                                      /*!< BRK dfsdm1_break[0] enable This bit enables the dfsdm1_break[0] for the timer's BRK input. dfsdm1_break[0] output is 'ORed' with the other BRK sources. - 1'b0:  disable:dfsdm1_break[0] input disabled - 1'b1:  enable:dfsdm1_break[0] input enabled Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF1_BKDF1BK0E_OFS                8U                                          /*!< ADV TIMER TIM AF1: BKDF1BK0E Bit Offset */
#define ADV_TIMER_TIM_AF1_BKDF1BK0E_VAL(regval)            (BIT(8) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM AF1: BKDF1BK0E Bit Value */  
#define ADV_TIMER_TIM_AF1_BKDF1BK0E_DISABLE                 0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF1_BKDF1BK0E_ENABLE                  BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af1 bkdf1bk0e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bkdf1bk0e bit.
  * \retval 1 This is a valid bkdf1bk0e bit.
  */
#define IS_ADV_TIMER_TIM_AF1_BKDF1BK0E(regval)              (\
                                        ((regval) == ADV_TIMER_TIM_AF1_BKDF1BK0E_DISABLE             ) || \
                                        ((regval) == ADV_TIMER_TIM_AF1_BKDF1BK0E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_AF1_BKINP                        BIT(9)                                      /*!< BRK BKIN input polarity This bit selects the BKIN alternate function input sensitivity. It must be programmed together with the BKP polarity bit - 1'b0:  disable: BKIN input is active high - 1'b1:  enable:BKIN input is active low Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF1_BKINP_OFS                    9U                                          /*!< ADV TIMER TIM AF1: BKINP Bit Offset */
#define ADV_TIMER_TIM_AF1_BKINP_VAL(regval)                (BIT(9) & ((uint32_t)(regval) << 9))        /*!< ADV TIMER TIM AF1: BKINP Bit Value */  
#define ADV_TIMER_TIM_AF1_BKINP_DISABLE                     0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF1_BKINP_ENABLE                      BIT(9)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af1 bkinp bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bkinp bit.
  * \retval 1 This is a valid bkinp bit.
  */
#define IS_ADV_TIMER_TIM_AF1_BKINP(regval)                  (\
                                            ((regval) == ADV_TIMER_TIM_AF1_BKINP_DISABLE             ) || \
                                            ((regval) == ADV_TIMER_TIM_AF1_BKINP_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_AF1_BKCMP1P                      BIT(10)                                      /*!< BRK COMP1 input polarity This bit selects the COMP1 input sensitivity. It must be programmed together with the BKP polarity bit - 1'b0:  disable:COMP1 input is active high - 1'b1:  enable:COMP1 input is active low Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF1_BKCMP1P_OFS                  10U                                          /*!< ADV TIMER TIM AF1: BKCMP1P Bit Offset */
#define ADV_TIMER_TIM_AF1_BKCMP1P_VAL(regval)              (BIT(10) & ((uint32_t)(regval) << 10))        /*!< ADV TIMER TIM AF1: BKCMP1P Bit Value */  
#define ADV_TIMER_TIM_AF1_BKCMP1P_DISABLE                   0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF1_BKCMP1P_ENABLE                    BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af1 bkcmp1p bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bkcmp1p bit.
  * \retval 1 This is a valid bkcmp1p bit.
  */
#define IS_ADV_TIMER_TIM_AF1_BKCMP1P(regval)                (\
                                          ((regval) == ADV_TIMER_TIM_AF1_BKCMP1P_DISABLE             ) || \
                                          ((regval) == ADV_TIMER_TIM_AF1_BKCMP1P_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_AF1_BKCMP2P                      BIT(11)                                      /*!< BRK COMP2 input polarity This bit selects the COMP2 input sensitivity. It must be programmed together with the BKP polarity bit - 1'b0:  disable:COMP2 input is active high - 1'b1:  enable:COMP2 input is active low Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF1_BKCMP2P_OFS                  11U                                          /*!< ADV TIMER TIM AF1: BKCMP2P Bit Offset */
#define ADV_TIMER_TIM_AF1_BKCMP2P_VAL(regval)              (BIT(11) & ((uint32_t)(regval) << 11))        /*!< ADV TIMER TIM AF1: BKCMP2P Bit Value */  
#define ADV_TIMER_TIM_AF1_BKCMP2P_DISABLE                   0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF1_BKCMP2P_ENABLE                    BIT(11)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af1 bkcmp2p bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bkcmp2p bit.
  * \retval 1 This is a valid bkcmp2p bit.
  */
#define IS_ADV_TIMER_TIM_AF1_BKCMP2P(regval)                (\
                                          ((regval) == ADV_TIMER_TIM_AF1_BKCMP2P_DISABLE             ) || \
                                          ((regval) == ADV_TIMER_TIM_AF1_BKCMP2P_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_AF1_ETRSEL_MASK                  BITS(14,17)                                   /*!< ADV TIMER TIM AF1: ETRSEL Bit Mask */  
#define ADV_TIMER_TIM_AF1_ETRSEL_OFS                   14U                                          /*!< ADV TIMER TIM AF1: ETRSEL Bit Offset */
#define ADV_TIMER_TIM_AF1_ETRSEL(regval)               (BITS(14,17) & ((uint32_t)(regval) << 14))        /*!< ADV TIMER TIM AF1: ETRSEL Bit Value */  
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_0                      ADV_TIMER_TIM_AF1_ETRSEL(0)                                               /*!< ETR_0 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_1                      ADV_TIMER_TIM_AF1_ETRSEL(1)                                               /*!< ETR_1 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_2                      ADV_TIMER_TIM_AF1_ETRSEL(2)                                               /*!< ETR_2 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_3                      ADV_TIMER_TIM_AF1_ETRSEL(3)                                               /*!< ETR_3 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_4                      ADV_TIMER_TIM_AF1_ETRSEL(4)                                               /*!< ETR_4 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_5                      ADV_TIMER_TIM_AF1_ETRSEL(5)                                               /*!< ETR_5 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_6                      ADV_TIMER_TIM_AF1_ETRSEL(6)                                               /*!< ETR_6 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_7                      ADV_TIMER_TIM_AF1_ETRSEL(7)                                               /*!< ETR_7 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_8                      ADV_TIMER_TIM_AF1_ETRSEL(8)                                               /*!< ETR_8 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_9                      ADV_TIMER_TIM_AF1_ETRSEL(9)                                               /*!< ETR_9 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_10                     ADV_TIMER_TIM_AF1_ETRSEL(10)                                               /*!< ETR_10 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_11                     ADV_TIMER_TIM_AF1_ETRSEL(11)                                               /*!< ETR_11 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_12                     ADV_TIMER_TIM_AF1_ETRSEL(12)                                               /*!< ETR_12 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_13                     ADV_TIMER_TIM_AF1_ETRSEL(13)                                               /*!< ETR_13 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_14                     ADV_TIMER_TIM_AF1_ETRSEL(14)                                               /*!< ETR_14 */
#define ADV_TIMER_TIM_AF1_ETRSEL_ETR_15                     ADV_TIMER_TIM_AF1_ETRSEL(15)                                               /*!< ETR_15 */

/**
  * \brief Check the ADV_TIMER tim_af1 etrsel bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid etrsel bits.
  * \retval 1 This is a valid etrsel bits.
  */
#define IS_ADV_TIMER_TIM_AF1_ETRSEL(regval)                 (\
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_0               ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_1               ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_2               ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_3               ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_4               ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_5               ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_6               ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_7               ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_8               ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_9               ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_10              ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_11              ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_12              ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_13              ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_14              ) || \
                                           ((regval) == ADV_TIMER_TIM_AF1_ETRSEL_ETR_15              )  \
                                                 )

 /* ===== ADV TIMER TIM_AF2 Register definition ===== */
#define ADV_TIMER_TIM_AF2_BK2INE                       BIT(0)                                      /*!< BRK2 BKIN input enable  This bit enables the BKIN2 alternate function input for the timer's BRK2 input.BKIN2 input is 'ORed' with the other BRK2 sources  - 1'b0:  disable: BKIN2 input disabled   - 1'b1:  enable: BKIN2 input enabled   Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF2_BK2INE_OFS                   0U                                          /*!< ADV TIMER TIM AF2: BK2INE Bit Offset */
#define ADV_TIMER_TIM_AF2_BK2INE_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM AF2: BK2INE Bit Value */  
#define ADV_TIMER_TIM_AF2_BK2INE_DISABLE                    0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF2_BK2INE_ENABLE                     BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af2 bk2ine bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bk2ine bit.
  * \retval 1 This is a valid bk2ine bit.
  */
#define IS_ADV_TIMER_TIM_AF2_BK2INE(regval)                 (\
                                           ((regval) == ADV_TIMER_TIM_AF2_BK2INE_DISABLE             ) || \
                                           ((regval) == ADV_TIMER_TIM_AF2_BK2INE_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_AF2_BK2CMP1E                     BIT(1)                                      /*!< BRK2 COMP1 enable This bit enables the COMP1 for the timer's BRK2 input. COMP1 output is 'ORed' with the other BRK sources. - 1'b0:  disable: COMP1 input disabled - 1'b1:  enable: COMP1 input enabled Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF2_BK2CMP1E_OFS                 1U                                          /*!< ADV TIMER TIM AF2: BK2CMP1E Bit Offset */
#define ADV_TIMER_TIM_AF2_BK2CMP1E_VAL(regval)             (BIT(1) & ((uint32_t)(regval) << 1))        /*!< ADV TIMER TIM AF2: BK2CMP1E Bit Value */  
#define ADV_TIMER_TIM_AF2_BK2CMP1E_DISABLE                  0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF2_BK2CMP1E_ENABLE                   BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af2 bk2cmp1e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bk2cmp1e bit.
  * \retval 1 This is a valid bk2cmp1e bit.
  */
#define IS_ADV_TIMER_TIM_AF2_BK2CMP1E(regval)               (\
                                         ((regval) == ADV_TIMER_TIM_AF2_BK2CMP1E_DISABLE             ) || \
                                         ((regval) == ADV_TIMER_TIM_AF2_BK2CMP1E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_AF2_BK2CMP2E                     BIT(2)                                      /*!< BRK2 COMP2 enable This bit enables the COMP2 for the timer's BRK2 input. COMP2 output is 'ORed' with the other BRK2 sources. - 1'b0:  disable: COMP2 input disabled - 1'b1:  enable: COMP2 input enabled Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF2_BK2CMP2E_OFS                 2U                                          /*!< ADV TIMER TIM AF2: BK2CMP2E Bit Offset */
#define ADV_TIMER_TIM_AF2_BK2CMP2E_VAL(regval)             (BIT(2) & ((uint32_t)(regval) << 2))        /*!< ADV TIMER TIM AF2: BK2CMP2E Bit Value */  
#define ADV_TIMER_TIM_AF2_BK2CMP2E_DISABLE                  0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF2_BK2CMP2E_ENABLE                   BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af2 bk2cmp2e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bk2cmp2e bit.
  * \retval 1 This is a valid bk2cmp2e bit.
  */
#define IS_ADV_TIMER_TIM_AF2_BK2CMP2E(regval)               (\
                                         ((regval) == ADV_TIMER_TIM_AF2_BK2CMP2E_DISABLE             ) || \
                                         ((regval) == ADV_TIMER_TIM_AF2_BK2CMP2E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_AF2_BK2DF1BK1E                   BIT(8)                                      /*!< BRK2 dfsdm1_break[1] enable This bit enables the dfsdm1_break[1] for the timer's BRK2 input. dfsdm1_break[1] output is 'ORed' with the other BRK2 sources. - 1'b0:  disable: dfsdm1_break[1] input disabled - 1'b1:  enable: dfsdm1_break[1] input enabled Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF2_BK2DF1BK1E_OFS               8U                                          /*!< ADV TIMER TIM AF2: BK2DF1BK1E Bit Offset */
#define ADV_TIMER_TIM_AF2_BK2DF1BK1E_VAL(regval)           (BIT(8) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM AF2: BK2DF1BK1E Bit Value */  
#define ADV_TIMER_TIM_AF2_BK2DF1BK1E_DISABLE                0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF2_BK2DF1BK1E_ENABLE                 BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af2 bk2df1bk1e bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bk2df1bk1e bit.
  * \retval 1 This is a valid bk2df1bk1e bit.
  */
#define IS_ADV_TIMER_TIM_AF2_BK2DF1BK1E(regval)             (\
                                       ((regval) == ADV_TIMER_TIM_AF2_BK2DF1BK1E_DISABLE             ) || \
                                       ((regval) == ADV_TIMER_TIM_AF2_BK2DF1BK1E_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_AF2_BK2INP                       BIT(9)                                      /*!< BRK2 BKIN2 input polarity This bit selects the BKIN2 alternate function input sensitivity. It must be programmed together with the BK2P polarity bit - 1'b0:  disable: BKIN2 input is active high - 1'b1:  enable: BKIN2 input is active low Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF2_BK2INP_OFS                   9U                                          /*!< ADV TIMER TIM AF2: BK2INP Bit Offset */
#define ADV_TIMER_TIM_AF2_BK2INP_VAL(regval)               (BIT(9) & ((uint32_t)(regval) << 9))        /*!< ADV TIMER TIM AF2: BK2INP Bit Value */  
#define ADV_TIMER_TIM_AF2_BK2INP_DISABLE                    0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF2_BK2INP_ENABLE                     BIT(9)                                                   /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af2 bk2inp bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bk2inp bit.
  * \retval 1 This is a valid bk2inp bit.
  */
#define IS_ADV_TIMER_TIM_AF2_BK2INP(regval)                 (\
                                           ((regval) == ADV_TIMER_TIM_AF2_BK2INP_DISABLE             ) || \
                                           ((regval) == ADV_TIMER_TIM_AF2_BK2INP_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_AF2_BK2CMP1P                     BIT(10)                                      /*!< BRK COMP1 input polarity This bit selects the COMP1 input sensitivity. It must be programmed together with the BK2P polarity bit - 1'b0:  disable: COMP1 input is active high - 1'b1:  enable: COMP1 input is active low Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF2_BK2CMP1P_OFS                 10U                                          /*!< ADV TIMER TIM AF2: BK2CMP1P Bit Offset */
#define ADV_TIMER_TIM_AF2_BK2CMP1P_VAL(regval)             (BIT(10) & ((uint32_t)(regval) << 10))        /*!< ADV TIMER TIM AF2: BK2CMP1P Bit Value */  
#define ADV_TIMER_TIM_AF2_BK2CMP1P_DISABLE                  0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF2_BK2CMP1P_ENABLE                   BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af2 bk2cmp1p bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bk2cmp1p bit.
  * \retval 1 This is a valid bk2cmp1p bit.
  */
#define IS_ADV_TIMER_TIM_AF2_BK2CMP1P(regval)               (\
                                         ((regval) == ADV_TIMER_TIM_AF2_BK2CMP1P_DISABLE             ) || \
                                         ((regval) == ADV_TIMER_TIM_AF2_BK2CMP1P_ENABLE              )  \
                                                 )

#define ADV_TIMER_TIM_AF2_BK2CMP2P                     BIT(11)                                      /*!< BRK2 COMP2 input polarity This bit selects the COMP2 input sensitivity. It must be programmed together with the BK2P polarity bit - 1'b0:  disable: COMP2 input is active high - 1'b1:  enable: COMP2 input is active low Note: This bit can not be modified as long as LOCK level 1 has been programmed(LOCK bits in TIM_BDTR register) */
#define ADV_TIMER_TIM_AF2_BK2CMP2P_OFS                 11U                                          /*!< ADV TIMER TIM AF2: BK2CMP2P Bit Offset */
#define ADV_TIMER_TIM_AF2_BK2CMP2P_VAL(regval)             (BIT(11) & ((uint32_t)(regval) << 11))        /*!< ADV TIMER TIM AF2: BK2CMP2P Bit Value */  
#define ADV_TIMER_TIM_AF2_BK2CMP2P_DISABLE                  0x0UL                                             /*!< DISABLE */
#define ADV_TIMER_TIM_AF2_BK2CMP2P_ENABLE                   BIT(11)                                                  /*!< ENABLE */

/**
  * \brief Check the ADV_TIMER tim_af2 bk2cmp2p bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid bk2cmp2p bit.
  * \retval 1 This is a valid bk2cmp2p bit.
  */
#define IS_ADV_TIMER_TIM_AF2_BK2CMP2P(regval)               (\
                                         ((regval) == ADV_TIMER_TIM_AF2_BK2CMP2P_DISABLE             ) || \
                                         ((regval) == ADV_TIMER_TIM_AF2_BK2CMP2P_ENABLE              )  \
                                                 )

 /* ===== ADV TIMER TIM_TISEL Register definition ===== */
#define ADV_TIMER_TIM_TISEL_TI1SEL_MASK                  BITS(0,3)                                   /*!< ADV TIMER TIM TISEL: TI1SEL Bit Mask */  
#define ADV_TIMER_TIM_TISEL_TI1SEL_OFS                   0U                                          /*!< ADV TIMER TIM TISEL: TI1SEL Bit Offset */
#define ADV_TIMER_TIM_TISEL_TI1SEL(regval)               (BITS(0,3) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM TISEL: TI1SEL Bit Value */  
#define ADV_TIMER_TIM_TISEL_TI2SEL_MASK                  BITS(8,11)                                   /*!< ADV TIMER TIM TISEL: TI2SEL Bit Mask */  
#define ADV_TIMER_TIM_TISEL_TI2SEL_OFS                   8U                                          /*!< ADV TIMER TIM TISEL: TI2SEL Bit Offset */
#define ADV_TIMER_TIM_TISEL_TI2SEL(regval)               (BITS(8,11) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM TISEL: TI2SEL Bit Value */  
#define ADV_TIMER_TIM_TISEL_TI3SEL_MASK                  BITS(16,19)                                   /*!< ADV TIMER TIM TISEL: TI3SEL Bit Mask */  
#define ADV_TIMER_TIM_TISEL_TI3SEL_OFS                   16U                                          /*!< ADV TIMER TIM TISEL: TI3SEL Bit Offset */
#define ADV_TIMER_TIM_TISEL_TI3SEL(regval)               (BITS(16,19) & ((uint32_t)(regval) << 16))        /*!< ADV TIMER TIM TISEL: TI3SEL Bit Value */  
#define ADV_TIMER_TIM_TISEL_TI4SEL_MASK                  BITS(24,27)                                   /*!< ADV TIMER TIM TISEL: TI4SEL Bit Mask */  
#define ADV_TIMER_TIM_TISEL_TI4SEL_OFS                   24U                                          /*!< ADV TIMER TIM TISEL: TI4SEL Bit Offset */
#define ADV_TIMER_TIM_TISEL_TI4SEL(regval)               (BITS(24,27) & ((uint32_t)(regval) << 24))        /*!< ADV TIMER TIM TISEL: TI4SEL Bit Value */  
 
 /* ===== ADV TIMER TIM_OUTPUT_DELAY Register definition ===== */
#define ADV_TIMER_TIM_OUTPUT_DELAY_OC1_DELAY_MASK               BITS(0,7)                                   /*!< ADV TIMER TIM OUTPUT DELAY: OC1_DELAY Bit Mask */  
#define ADV_TIMER_TIM_OUTPUT_DELAY_OC1_DELAY_OFS                0U                                          /*!< ADV TIMER TIM OUTPUT DELAY: OC1_DELAY Bit Offset */
#define ADV_TIMER_TIM_OUTPUT_DELAY_OC1_DELAY(regval)            (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< ADV TIMER TIM OUTPUT DELAY: OC1_DELAY Bit Value */  
#define ADV_TIMER_TIM_OUTPUT_DELAY_OC2_DELAY_MASK               BITS(8,15)                                   /*!< ADV TIMER TIM OUTPUT DELAY: OC2_DELAY Bit Mask */  
#define ADV_TIMER_TIM_OUTPUT_DELAY_OC2_DELAY_OFS                8U                                          /*!< ADV TIMER TIM OUTPUT DELAY: OC2_DELAY Bit Offset */
#define ADV_TIMER_TIM_OUTPUT_DELAY_OC2_DELAY(regval)            (BITS(8,15) & ((uint32_t)(regval) << 8))        /*!< ADV TIMER TIM OUTPUT DELAY: OC2_DELAY Bit Value */  
#define ADV_TIMER_TIM_OUTPUT_DELAY_OC3_DELAY_MASK               BITS(16,23)                                   /*!< ADV TIMER TIM OUTPUT DELAY: OC3_DELAY Bit Mask */  
#define ADV_TIMER_TIM_OUTPUT_DELAY_OC3_DELAY_OFS                16U                                          /*!< ADV TIMER TIM OUTPUT DELAY: OC3_DELAY Bit Offset */
#define ADV_TIMER_TIM_OUTPUT_DELAY_OC3_DELAY(regval)            (BITS(16,23) & ((uint32_t)(regval) << 16))        /*!< ADV TIMER TIM OUTPUT DELAY: OC3_DELAY Bit Value */  
#define ADV_TIMER_TIM_OUTPUT_DELAY_OC4_DELAY_MASK               BITS(24,31)                                   /*!< ADV TIMER TIM OUTPUT DELAY: OC4_DELAY Bit Mask */  
#define ADV_TIMER_TIM_OUTPUT_DELAY_OC4_DELAY_OFS                24U                                          /*!< ADV TIMER TIM OUTPUT DELAY: OC4_DELAY Bit Offset */
#define ADV_TIMER_TIM_OUTPUT_DELAY_OC4_DELAY(regval)            (BITS(24,31) & ((uint32_t)(regval) << 24))        /*!< ADV TIMER TIM OUTPUT DELAY: OC4_DELAY Bit Value */  

/* function declarations */
/* TIMER timebase */
/* initialize TIMER init parameter struct */
void Timer_StructParaInit(TIMER_Init_TypeDef* initpara);
/* initialize TIMER counter */
void Timer_Init(ADVANCED_TIMER_TypeDef* timer, TIMER_Init_TypeDef* initpara);
/* enable a timer */
void Timer_Enable(ADVANCED_TIMER_TypeDef* timer);
/* disable a timer */
void Timer_Disable(ADVANCED_TIMER_TypeDef* timer);
/* enable the auto reload shadow function */
void Timer_AutoReloadShadowEnable(ADVANCED_TIMER_TypeDef* timer);
/* disable the auto reload shadow function */
void Timer_AutoReloadShadowDisable(ADVANCED_TIMER_TypeDef* timer);
/* enable the update event */
void Timer_UpdateEventEnable(ADVANCED_TIMER_TypeDef* timer);
/* disable the update event */
void Timer_UpdateEventDisable(ADVANCED_TIMER_TypeDef* timer);
/* set TIMER counter alignment mode */
void Timer_CounterAlignment(ADVANCED_TIMER_TypeDef* timer, uint32_t aligned);
/* set TIMER counter up direction */
void Timer_CounterUpDirection(ADVANCED_TIMER_TypeDef* timer);
/* set TIMER counter down direction */
void Timer_CounterDownDirection(ADVANCED_TIMER_TypeDef* timer);

/* configure TIMER prescaler */
void Timer_PrescalerConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t pscreload);
/* configure TIMER repetition register value */
void Timer_RepetitionValueConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t repetition);
/* configure TIMER autoreload register value */
void Timer_AutoReloadValueConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t autoreload);
/* configure TIMER counter register value */
void Timer_CounterValueConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t counter);
/* read TIMER counter value */
uint32_t Timer_CounterRead(ADVANCED_TIMER_TypeDef* timer);
/* read TIMER prescaler value */
uint32_t Timer_PrescalerRead(ADVANCED_TIMER_TypeDef* timer);
/* configure TIMER single pulse mode */
void Timer_SinglePulseModeConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t spmode);
/* configure TIMER update source */
void Timer_UpdateSourceConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t update);

/* TIMER DMA and event */
/* enable the TIMER DMA */
void Timer_DmaEnable(ADVANCED_TIMER_TypeDef* timer, uint32_t dma);
/* disable the TIMER DMA */
void Timer_DmaDisable(ADVANCED_TIMER_TypeDef* timer, uint32_t dma);
/* channel DMA request source selection */
void Timer_ChannelDmaRequestSourceSelect(ADVANCED_TIMER_TypeDef* timer, uint32_t dma_request);
/* configure the TIMER DMA transfer */
void Timer_DmaTransferConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t dma_baseaddr, uint32_t dma_lenth);
/* software generate events */
void Timer_EventSoftwareGenerate(ADVANCED_TIMER_TypeDef* timer, uint32_t event);

/* TIMER channel complementary protection */
/* initialize TIMER break parameter struct */
void Timer_BreakStructParaInit(TIMER_Break_TypeDef* breakpara);
/* configure TIMER break function */
void Timer_BreakConfig(ADVANCED_TIMER_TypeDef* timer, TIMER_Break_TypeDef* breakpara);
/* enable TIMER break function */
void Timer_BreakEnable(ADVANCED_TIMER_TypeDef* timer);
/* disable TIMER break function */
void Timer_BreakDisable(ADVANCED_TIMER_TypeDef* timer);
/* enable TIMER break2 function */
void Timer_Break2Enable(ADVANCED_TIMER_TypeDef* timer);
/* disable TIMER break2 function */
void Timer_Break2Disable(ADVANCED_TIMER_TypeDef* timer);
/* enable TIMER output automatic function */
void Timer_AutoMaticOutputEnable(ADVANCED_TIMER_TypeDef* timer);
/* disable TIMER output automatic function */
void Timer_AutoMaticOutputDisable(ADVANCED_TIMER_TypeDef* timer);
/* enable or disable TIMER primary output function */
void Timer_PrimaryOutputConfig(ADVANCED_TIMER_TypeDef* timer, ControlStatus newvalue);
/* enable or disable channel capture/compare control shadow register */
void Timer_ChannelControlShadowConfig(ADVANCED_TIMER_TypeDef* timer, ControlStatus newvalue);
/* configure TIMER channel control shadow register update control */
void Timer_ChannelControlShadowUpdateConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t ccuctl);
void Timer_ChannelFastOutput(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ocxfe);
void Timer_ChannelFastOutputDelay(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ofdelay);
/* TIMER channel output */
/* initialize TIMER channel output parameter struct */
void Timer_ChannelOutputStructParaInit(TIMER_Output_TypeDef* ocpara);
/* configure TIMER channel output function */
void Timer_ChannelOutputConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, TIMER_Output_TypeDef* ocpara);
/* configure TIMER channel output compare mode */
void Timer_ChannelOutputModeConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ocmode);
/* configure TIMER channel output pulse value */
void Timer_ChannelOutputPulseValueConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t pulse);
/* configure TIMER channel output shadow function */
void Timer_ChannelOutputShadowConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ocshadow);

/* configure TIMER channel output clear function */
void Timer_ChannelOutputClearConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t occlear);
/* configure TIMER channel output polarity */
void Timer_ChannelOutputPolarityConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ocpolarity);
/* configure TIMER channel complementary output polarity */
void Timer_ChannelComplementaryOutputPolarityConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ocnpolarity);
/* configure TIMER channel enable state */
void Timer_ChannelOutputStateConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t state);
/* configure TIMER channel complementary output enable state */
void Timer_ChannelComplementaryOutputStateConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t ocnstate);

/* TIMER channel input */
/* initialize TIMER channel input parameter struct */
void Timer_ChannelInputStructParaInit(TIMER_Input_TypeDef* icpara);
/* configure TIMER input capture parameter */
void Timer_InputCaptureConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, TIMER_Input_TypeDef* icpara);
/* configure TIMER channel input capture prescaler value */
void Timer_ChannelInputCapturePrescalerConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, uint32_t prescaler);
/* read TIMER channel capture compare register value */
uint32_t Timer_ChannelCaptureValueRegisterRead(ADVANCED_TIMER_TypeDef* timer, uint32_t channel);
/* configure TIMER input pwm capture function */
void Timer_InputPwmCaptureConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t channel, TIMER_Input_TypeDef* icpwm);
/* configure TIMER hall sensor mode */
void Timer_HallModeConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t hallmode);

/* TIMER master and slave mode */
/* select TIMER input trigger source */
void Timer_InputTriggerSourceSelect(ADVANCED_TIMER_TypeDef* timer, uint32_t intrigger);
/* select TIMER master mode output trigger source */
void Timer_MasterOutputTriggerSourceSelect(ADVANCED_TIMER_TypeDef* timer, uint32_t outrigger);
/* select TIMER slave mode */
void Timer_SlaveModeSelect(ADVANCED_TIMER_TypeDef* timer, uint32_t slavemode);
/* configure TIMER master slave mode */
void Timer_MasterSlaveModeConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t masterslave);
/* configure TIMER external trigger input */
void Timer_ExternalTriggerConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t extprescaler, uint32_t extpolarity, uint32_t extfilter);
/* configure TIMER quadrature decoder mode */
void Timer_QuadratureDecoderModeConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t decomode, uint32_t ic0polarity, uint32_t ic1polarity);
/* configure TIMER internal clock mode */
void Timer_InternalClockConfig(ADVANCED_TIMER_TypeDef* timer);
/* configure TIMER the internal trigger as external clock input */
void Timer_InternalTriggerAsExternalClockConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t intrigger);
/* configure TIMER the external trigger as external clock input */
void Timer_ExternalTriggerAsExternalClockConfig(ADVANCED_TIMER_TypeDef* timer, uint32_t extrigger, uint32_t extpolarity, uint32_t extfilter);
/* configure TIMER the external clock mode 0 */
void Timer_ExternalClockMode0Config(ADVANCED_TIMER_TypeDef* timer, uint32_t extprescaler, uint32_t extpolarity, uint32_t extfilter);
/* configure TIMER the external clock mode 1 */
void Timer_ExternalClockMode1Config(ADVANCED_TIMER_TypeDef* timer, uint32_t extprescaler, uint32_t extpolarity, uint32_t extfilter);
/* disable TIMER the external clock mode 1 */
void Timer_ExternalClockMode1Disable(ADVANCED_TIMER_TypeDef* timer);

/* TIMER interrupt and flag */
/* enable the TIMER interrupt */
void Timer_InterruptEnable(ADVANCED_TIMER_TypeDef* timer, uint32_t interrupt);
/* disable the TIMER interrupt */
void Timer_InterruptDisable(ADVANCED_TIMER_TypeDef* timer, uint32_t interrupt);
/* get TIMER interrupt flag */
FlagStatus Timer_InterruptFlagGet(ADVANCED_TIMER_TypeDef* timer, uint32_t interrupt);
/* clear TIMER interrupt flag */
void Timer_InterruptFlagClear(ADVANCED_TIMER_TypeDef* timer, uint32_t interrupt);
/* get TIMER flag */
FlagStatus Timer_FlagGet(ADVANCED_TIMER_TypeDef* timer, uint32_t flag);
/* clear TIMER flag */
void Timer_FlagClear(ADVANCED_TIMER_TypeDef* timer, uint32_t flag);
/* set ckdiv */
void Timer_CKDIV(ADVANCED_TIMER_TypeDef* timer, uint32_t div);

void TIM_PWM_Start(ADVANCED_TIMER_TypeDef* timer, uint32_t Channel,uint32_t state);
void TIMEx_PWMN_Start(ADVANCED_TIMER_TypeDef *timer, uint32_t Channel,uint32_t state);
void TIM_PWM_Stop(ADVANCED_TIMER_TypeDef* timer, uint32_t Channel,uint32_t state);
void TIMEx_PWMN_Stop(ADVANCED_TIMER_TypeDef *timer, uint32_t Channel ,uint32_t state);

void Timer_BKAF1(ADVANCED_TIMER_TypeDef* timer,ControlStatus state);
void Timer_BK2AF2(ADVANCED_TIMER_TypeDef* timer,ControlStatus state);

#ifdef __cplusplus
}
#endif

#endif /* NS__TIMER_H */

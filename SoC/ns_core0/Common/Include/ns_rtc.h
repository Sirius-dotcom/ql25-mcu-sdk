
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
  * \file csg_rtc.h
  * \brief definitions for the RTC
  */

#ifndef _NS_RTC_H_
#define _NS_RTC_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "ns.h"

/* RTC definitions */
#define RTC                          RTC2_BASE

#define RTC_BKP0                     REG32(RTC + 0x40U)      /*!< backup register */
#define RTC_BKP1                     REG32(RTC + 0x44U)      /*!< backup register */
#define RTC_BKP2                     REG32(RTC + 0x48U)      /*!< backup register */
#define RTC_BKP3                     REG32(RTC + 0x4CU)      /*!< backup register */
#define RTC_BKP4                     REG32(RTC + 0x50U)      /*!< backup register */
#define RTC_BKP5                     REG32(RTC + 0x54U)      /*!< backup register */
#define RTC_BKP6                     REG32(RTC + 0x58U)      /*!< backup register */
#define RTC_BKP7                     REG32(RTC + 0x5CU)      /*!< backup register */
#define RTC_BKP8                     REG32(RTC + 0x60U)      /*!< backup register */
#define RTC_BKP9                     REG32(RTC + 0x64U)      /*!< backup register */
#define RTC_BKP10                    REG32(RTC + 0x68U)      /*!< backup register */
#define RTC_BKP11                    REG32(RTC + 0x6CU)      /*!< backup register */
#define RTC_BKP12                    REG32(RTC + 0x70U)      /*!< backup register */
#define RTC_BKP13                    REG32(RTC + 0x74U)      /*!< backup register */
#define RTC_BKP14                    REG32(RTC + 0x78U)      /*!< backup register */
#define RTC_BKP15                    REG32(RTC + 0x7CU)      /*!< backup register */
#define RTC_BKP16                    REG32(RTC + 0x80U)      /*!< backup register */
#define RTC_BKP17                    REG32(RTC + 0x84U)      /*!< backup register */
#define RTC_BKP18                    REG32(RTC + 0x88U)      /*!< backup register */
#define RTC_BKP19                    REG32(RTC + 0x8CU)      /*!< backup register */
#define RTC_BKP20                    REG32(RTC + 0x90U)      /*!< backup register */
#define RTC_BKP21                    REG32(RTC + 0x94U)      /*!< backup register */
#define RTC_BKP22                    REG32(RTC + 0x98U)      /*!< backup register */
#define RTC_BKP23                    REG32(RTC + 0x9CU)      /*!< backup register */
#define RTC_BKP24                    REG32(RTC + 0xA0U)      /*!< backup register */
#define RTC_BKP25                    REG32(RTC + 0xA4U)      /*!< backup register */
#define RTC_BKP26                    REG32(RTC + 0xA8U)      /*!< backup register */
#define RTC_BKP27                    REG32(RTC + 0xACU)      /*!< backup register */
#define RTC_BKP28                    REG32(RTC + 0xB0U)      /*!< backup register */
#define RTC_BKP29                    REG32(RTC + 0xB4U)      /*!< backup register */
#define RTC_BKP30                    REG32(RTC + 0xB8U)      /*!< backup register */
#define RTC_BKP31                    REG32(RTC + 0xBCU)      /*!< backup register */

#define RTC_CRH_OFFSET                                                0x0 /*!< RTC Control register high. */
#define RTC_CRL_OFFSET                                                0x4 /*!< RTC Control register low. */
#define RTC_PRL_OFFSET                                                0x8 /*!< RTC prescaler register. */
#define RTC_DIV_OFFSET                                                0xc /*!< RTC prescaler counter register. */
#define RTC_CNTH_OFFSET                                               0x10 /*!< RTC counter register high. */
#define RTC_CNTL_OFFSET                                               0x14 /*!< RTC counter register low. */
#define RTC_ALRH_OFFSET                                               0x18 /*!< RTC alarm register high. */
#define RTC_ALRL_OFFSET                                               0x1c /*!< RTC alarm register low. */
#define RTC_CALR_OFFSET                                               0x20 /*!< RTC calibration register low. */
#define RTC_VERSION_OFFSET                                            0x24 /*!< IP version */

 /* ===== RTC CRH Register definition ===== */
#define RTC_CRH_SECIE                        BIT(0)                                      /*!< SECIE: Second interrupt enable. -1'b0:    disabled:  Disable second interrupt. -1'b1:    enabled: Enable second interrupt. */
#define RTC_CRH_SECIE_OFS                    0U                                          /*!< RTC CRH: SECIE Bit Offset */
#define RTC_CRH_SECIE_VAL(regval)                (BIT(0) & ((uint32_t)(regval) << 0))        /*!< RTC CRH: SECIE Bit Value */  
#define RTC_CRH_SECIE_DISABLED                    0x0UL                                                 /*!< DISABLED */
#define RTC_CRH_SECIE_ENABLED                     BIT(0)                                                   /*!< ENABLED */

/**
  * \brief Check the RTC crh secie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid secie bit.
  * \retval 1 This is a valid secie bit.
  */
#define IS_RTC_CRH_SECIE(regval)                  (\
                                            ((regval) == RTC_CRH_SECIE_DISABLED            ) || \
                                            ((regval) == RTC_CRH_SECIE_ENABLED             )  \
                                                 )

#define RTC_CRH_ALRIE                        BIT(1)                                      /*!< ALRIE: Alarm interrupt enable. -1'b0:    disabled:  Disable alarm interrupt. -1'b1:    enabled:  Enable alarm interrupt. */
#define RTC_CRH_ALRIE_OFS                    1U                                          /*!< RTC CRH: ALRIE Bit Offset */
#define RTC_CRH_ALRIE_VAL(regval)                (BIT(1) & ((uint32_t)(regval) << 1))        /*!< RTC CRH: ALRIE Bit Value */  
#define RTC_CRH_ALRIE_DISABLED                    0x0UL                                                 /*!< DISABLED */
#define RTC_CRH_ALRIE_ENABLED                     BIT(1)                                                   /*!< ENABLED */

/**
  * \brief Check the RTC crh alrie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid alrie bit.
  * \retval 1 This is a valid alrie bit.
  */
#define IS_RTC_CRH_ALRIE(regval)                  (\
                                            ((regval) == RTC_CRH_ALRIE_DISABLED            ) || \
                                            ((regval) == RTC_CRH_ALRIE_ENABLED             )  \
                                                 )

#define RTC_CRH_OWIE                         BIT(2)                                      /*!< OWIE: Overflow interrupt enable. -1'b0:    disabled:  Disable overflow interrupt. -1'b1:    enabled: Enable overflow interrupt. */
#define RTC_CRH_OWIE_OFS                     2U                                          /*!< RTC CRH: OWIE Bit Offset */
#define RTC_CRH_OWIE_VAL(regval)                 (BIT(2) & ((uint32_t)(regval) << 2))        /*!< RTC CRH: OWIE Bit Value */  
#define RTC_CRH_OWIE_DISABLED                     0x0UL                                                 /*!< DISABLED */
#define RTC_CRH_OWIE_ENABLED                      BIT(2)                                                   /*!< ENABLED */

/**
  * \brief Check the RTC crh owie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid owie bit.
  * \retval 1 This is a valid owie bit.
  */
#define IS_RTC_CRH_OWIE(regval)                   (\
                                             ((regval) == RTC_CRH_OWIE_DISABLED            ) || \
                                             ((regval) == RTC_CRH_OWIE_ENABLED             )  \
                                                 )

#define RTC_CRH_CALIBRATION                  BIT(3)                                      /*!< Calibration enable:  -1'b0:    disabled: RTC smooth digital calibration disable. -1'b1:    enabled:  RTC smooth digital calibration enable. */
#define RTC_CRH_CALIBRATION_OFS              3U                                          /*!< RTC CRH: CALIBRATION Bit Offset */
#define RTC_CRH_CALIBRATION_VAL(regval)          (BIT(3) & ((uint32_t)(regval) << 3))        /*!< RTC CRH: CALIBRATION Bit Value */  
#define RTC_CRH_CALIBRATION_DISABLED              0x0UL                                                 /*!< DISABLED */
#define RTC_CRH_CALIBRATION_ENABLED               BIT(3)                                                   /*!< ENABLED */

/**
  * \brief Check the RTC crh calibration bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid calibration bit.
  * \retval 1 This is a valid calibration bit.
  */
#define IS_RTC_CRH_CALIBRATION(regval)            (\
                                      ((regval) == RTC_CRH_CALIBRATION_DISABLED            ) || \
                                      ((regval) == RTC_CRH_CALIBRATION_ENABLED             )  \
                                                 )

 /* ===== RTC CRL Register definition ===== */
#define RTC_CRL_SECF                         BIT(0)                                      /*!< SECF: Second flag. This bit is set by hardware when the 32-bit programmable prescaler overflows, thus incrementing the RTC counter. Hence this flag provides a periodic signal with a period corresponding to the resolution programmed for the RTC counter (usually one second). An interrupt is generated if SECIE=1 in the RTC_CRH register. It can be cleared only by software. Writing "1" has no effect. -1'b0: Second flag condition not met. -1'b1: Second flag condition met. */
#define RTC_CRL_SECF_OFS                     0U                                          /*!< RTC CRL: SECF Bit Offset */
#define RTC_CRL_SECF_VAL(regval)                 (BIT(0) & ((uint32_t)(regval) << 0))        /*!< RTC CRL: SECF Bit Value */  

/**
  * \brief Check the RTC crl secf bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid secf bit.
  * \retval 1 This is a valid secf bit.
  */
#define IS_RTC_CRL_SECF(regval)                   (\
                                                 )

#define RTC_CRL_ALRF                         BIT(1)                                      /*!< ALRF: Alarm flag. This bit is set by hardware when the 64-bit programmable counter reaches the threshold set in the RTC_ALRL/RTC_ALRH register. An interrupt is generated if ALRIE=1 in the RTC_CRH register. It can be cleared only by software. Writing "1" has no effect. -1'b0: Alarm not detected. -1'b1: Alarm detected. */
#define RTC_CRL_ALRF_OFS                     1U                                          /*!< RTC CRL: ALRF Bit Offset */
#define RTC_CRL_ALRF_VAL(regval)                 (BIT(1) & ((uint32_t)(regval) << 1))        /*!< RTC CRL: ALRF Bit Value */  

/**
  * \brief Check the RTC crl alrf bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid alrf bit.
  * \retval 1 This is a valid alrf bit.
  */
#define IS_RTC_CRL_ALRF(regval)                   (\
                                                 )

#define RTC_CRL_OWF                          BIT(2)                                      /*!< OWF: Overflow flag. This bit is set by hardware when the 64-bit programmable counter overflows. An interrupt is generated if OWIE=1 in the RTC_CRH register. It can be cleared only by software. Writing "1" has no effect. -1'b0: Overflow not detected. -1'b1: 64-bit programmable counter overflow occurred. */
#define RTC_CRL_OWF_OFS                      2U                                          /*!< RTC CRL: OWF Bit Offset */
#define RTC_CRL_OWF_VAL(regval)                  (BIT(2) & ((uint32_t)(regval) << 2))        /*!< RTC CRL: OWF Bit Value */  

/**
  * \brief Check the RTC crl owf bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid owf bit.
  * \retval 1 This is a valid owf bit.
  */
#define IS_RTC_CRL_OWF(regval)                    (\
                                                 )

#define RTC_CRL_CNF                          BIT(4)                                      /*!< CNF: Configuration flag. This bit must be set by software to enter in configuration mode so as to allow new values to be written in the RTC_CNT, RTC_ALR or RTC_PRL registers. The write operation is only executed when the CNF bit is reset by software after has been set. -1'b0:    enabled: Exit configuration mode (start update of RTC registers). -1'b1:    disabled: Enter configuration mode. */
#define RTC_CRL_CNF_OFS                      4U                                          /*!< RTC CRL: CNF Bit Offset */
#define RTC_CRL_CNF_VAL(regval)                  (BIT(4) & ((uint32_t)(regval) << 4))        /*!< RTC CRL: CNF Bit Value */  
#define RTC_CRL_CNF_ENABLED                       0x0UL                                                 /*!< ENABLED */
#define RTC_CRL_CNF_DISABLED                      BIT(4)                                                   /*!< DISABLED */

/**
  * \brief Check the RTC crl cnf bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cnf bit.
  * \retval 1 This is a valid cnf bit.
  */
#define IS_RTC_CRL_CNF(regval)                    (\
                                              ((regval) == RTC_CRL_CNF_ENABLED             ) || \
                                              ((regval) == RTC_CRL_CNF_DISABLED            )  \
                                                 )

#define RTC_CRL_RTOFF                        BIT(5)                                      /*!< RTOFF: RTC operation OFF The ICB data sync to rtc clk domain done. */
 
 /* ===== RTC PRL Register definition ===== */
#define RTC_PRL_CLK_MASK                     BITS(0,19)                                   /*!< RTC PRL: CLK Bit Mask */  
#define RTC_PRL_CLK_OFS                      0U                                          /*!< RTC PRL: CLK Bit Offset */
#define RTC_PRL_CLK(regval)                  (BITS(0,19) & ((uint32_t)(regval) << 0))        /*!< RTC PRL: CLK Bit Value */  
 
 /* ===== RTC DIV Register definition ===== */
#define RTC_DIV_PRE                          BITS(0,31)                
 
 /* ===== RTC CNTH Register definition ===== */
#define RTC_CNTH_NUM_MASK                     BITS(0,31)                                   /*!< RTC CNTH: NUM Bit Mask */  
#define RTC_CNTH_NUM_OFS                      0U                                          /*!< RTC CNTH: NUM Bit Offset */
#define RTC_CNTH_NUM(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< RTC CNTH: NUM Bit Value */  
 
 /* ===== RTC CNTL Register definition ===== */
#define RTC_CNTL_NUM_MASK                     BITS(0,31)                                   /*!< RTC CNTL: NUM Bit Mask */  
#define RTC_CNTL_NUM_OFS                      0U                                          /*!< RTC CNTL: NUM Bit Offset */
#define RTC_CNTL_NUM(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< RTC CNTL: NUM Bit Value */  
 
 /* ===== RTC ALRH Register definition ===== */
#define RTC_ALRH_NUM_MASK                     BITS(0,31)                                   /*!< RTC ALRH: NUM Bit Mask */  
#define RTC_ALRH_NUM_OFS                      0U                                          /*!< RTC ALRH: NUM Bit Offset */
#define RTC_ALRH_NUM(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< RTC ALRH: NUM Bit Value */  
 
 /* ===== RTC ALRL Register definition ===== */
#define RTC_ALRL_NUM_MASK                     BITS(0,31)                                   /*!< RTC ALRL: NUM Bit Mask */  
#define RTC_ALRL_NUM_OFS                      0U                                          /*!< RTC ALRL: NUM Bit Offset */
#define RTC_ALRL_NUM(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< RTC ALRL: NUM Bit Value */  
 
 /* ===== RTC CALR Register definition ===== */
#define RTC_CALR_CAL_NUM_MASK                 BITS(0,8)                                   /*!< RTC CALR: CAL_NUM Bit Mask */  
#define RTC_CALR_CAL_NUM_OFS                  0U                                          /*!< RTC CALR: CAL_NUM Bit Offset */
#define RTC_CALR_CAL_NUM(regval)              (BITS(0,8) & ((uint32_t)(regval) << 0))        /*!< RTC CALR: CAL_NUM Bit Value */  
#define RTC_CALR_CALW8                        BIT(12)                                      /*!< Use an 8-second calibration cycle period. */
#define RTC_CALR_CALW8_OFS                    12U                                          /*!< RTC CALR: CALW8 Bit Offset */
#define RTC_CALR_CALW8_VAL(regval)                (BIT(12) & ((uint32_t)(regval) << 12))        /*!< RTC CALR: CALW8 Bit Value */  
#define RTC_CALR_CALW16                       BIT(13)                                      /*!< Use an 16-second calibration cycle period. */
#define RTC_CALR_CALW16_OFS                   13U                                          /*!< RTC CALR: CALW16 Bit Offset */
#define RTC_CALR_CALW16_VAL(regval)               (BIT(13) & ((uint32_t)(regval) << 13))        /*!< RTC CALR: CALW16 Bit Value */  
#define RTC_CALR_CAL_DIR                      BIT(15)                                      /*!< Calibration direction -1'b0: Insert rtc_clk -1'b1: Mask rtc_clk */
#define RTC_CALR_CAL_DIR_OFS                  15U                                          /*!< RTC CALR: CAL_DIR Bit Offset */
#define RTC_CALR_CAL_DIR_VAL(regval)              (BIT(15) & ((uint32_t)(regval) << 15))        /*!< RTC CALR: CAL_DIR Bit Value */  
#define RTC_CALR_CAL_DIR_INSERT                    0x0UL                                                /*!< INSERT */
#define RTC_CALR_CAL_DIR_MASK                      BIT(15)                                                  /*!< MASK */

/**
  * \brief Check the RTC calr cal_dir bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cal_dir bit.
  * \retval 1 This is a valid cal_dir bit.
  */
#define IS_RTC_CALR_CAL_DIR(regval)                (\
                                          ((regval) == RTC_CALR_CAL_DIR_INSERT              ) || \
                                          ((regval) == RTC_CALR_CAL_DIR_MASK                )  \
                                                 )

 /* ===== RTC VERSION Register definition ===== */
#define RTC_VERSION_NUM                          BITS(0,31)                

/* function declarations */
/* initialization functions */
/* enter RTC configuration mode */
void RTC_ConfigModeEnter(RTC_TypeDef *RTCx);
/* exit RTC configuration mode */
void RTC_ConfigModeExit(RTC_TypeDef *RTCx);
/* set RTC counter value */
void RTC_CounerSet(RTC_TypeDef *RTCx, uint64_t cnt);
/* set RTC prescaler value */
void RTC_PrescalerSet(RTC_TypeDef *RTCx, uint32_t prl);

/* operation functions */
/* wait RTC last write operation finished flag set */
void RTC_LwOffWait(RTC_TypeDef *RTCx);
/* set RTC alarm value */
void RTC_AlarmSet(RTC_TypeDef *RTCx, uint64_t alarm);
/* get RTC counter value */
uint64_t RTC_CounterGet(RTC_TypeDef *RTCx);
/* get RTC divider value */
uint32_t RTC_DividerGet(RTC_TypeDef *RTCx);

/* flag & Mode functions */
/* get RTC flag status */
FlagStatus RTC_FlagGet(RTC_TypeDef *RTCx, uint32_t flag);
/* clear RTC flag status */
void RTC_FlagClear(RTC_TypeDef *RTCx, uint32_t flag);
/* get RTC interrupt flag status */
FlagStatus RTC_InterruptFlagGet(RTC_TypeDef *RTCx, uint32_t flag);
/* clear RTC interrupt flag status */
void RTC_InterruptFlagClear(RTC_TypeDef *RTCx, uint32_t flag);
/* enable RTC Mode */
void RTC_InterruptEnable(RTC_TypeDef *RTCx, uint32_t interrupt);
/* disable RTC Mode */
void RTC_InterruptDisable(RTC_TypeDef *RTCx, uint32_t interrupt);

#ifdef __cplusplus
}
#endif

#endif /* end of _NS__RTC_H */

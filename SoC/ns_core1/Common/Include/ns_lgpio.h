/*
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

#ifndef _NS_LGPIO_H_
#define _NS_LGPIO_H_

#include "ns.h"
#ifdef __cplusplus
 extern "C" {
#endif

#define HIGHZ  0
#define PP  1
#define OD  2

#define HIGH_Z 3
#define WPUE 4
#define SPUE 5
#define PDE 6

typedef enum iof_func {
    IOF_DISABLE =0,
    IOF_SEL_ANALOG = 1,
    IOF_SEL_DIGIT = 2,
} IOF_FUNC;

typedef enum lgpio_int_type {
    LGPIO_IT_RISE = 0,
    LGPIO_IT_FALL = 1,
    LGPIO_IT_HIGH = 2,
    LGPIO_IT_LOW = 3
} LGPIO_INT_TYPE;

/* ===== GPIO pin source definition ===== */
#define LGPIO_PinSource0                 BIT(0)      /*!< LGPIO PIN : Selects Pin Source 0 */
#define LGPIO_PinSource1                 BIT(1)      /*!< LGPIO PIN : Selects Pin Source 1 */
#define LGPIO_PinSource2                 BIT(2)      /*!< LGPIO PIN : Selects Pin Source 2 */
#define LGPIO_PinSource3                 BIT(3)      /*!< LGPIO PIN : Selects Pin Source 3 */
#define LGPIO_PinSource4                 BIT(4)      /*!< LGPIO PIN : Selects Pin Source 4 */
#define LGPIO_PinSource5                 BIT(5)      /*!< LGPIO PIN : Selects Pin Source 5 */
#define LGPIO_PinSource6                 BIT(6)      /*!< LGPIO PIN : Selects Pin Source 6 */
#define LGPIO_PinSource7                 BIT(7)      /*!< LGPIO PIN : Selects Pin Source 7 */
#define LGPIO_PinSource8                 BIT(8)      /*!< LGPIO PIN : Selects Pin Source 8 */
#define LGPIO_PinSource9                 BIT(9)      /*!< LGPIO PIN : Selects Pin Source 9 */
#define LGPIO_PinSource10                BIT(10)     /*!< LGPIO PIN : Selects Pin Source 10 */
#define LGPIO_PinSource11                BIT(11)     /*!< LGPIO PIN : Selects Pin Source 11 */
#define LGPIO_PinSource12                BIT(12)     /*!< LGPIO PIN : Selects Pin Source 12 */
#define LGPIO_PinSource13                BIT(13)     /*!< LGPIO PIN : Selects Pin Source 13 */
#define LGPIO_PinSource14                BIT(14)     /*!< LGPIO PIN : Selects Pin Source 14 */
#define LGPIO_PinSource15                BIT(15)     /*!< LGPIO PIN : Selects Pin Source 15 */
#define LGPIO_PinSource16                BIT(16)     /*!< LGPIO PIN : Selects Pin Source 16 */
#define LGPIO_PinSource17                BIT(17)     /*!< LGPIO PIN : Selects Pin Source 17 */
#define LGPIO_PinSource18                BIT(18)     /*!< LGPIO PIN : Selects Pin Source 18 */
#define LGPIO_PinSource19                BIT(19)     /*!< LGPIO PIN : Selects Pin Source 19 */
#define LGPIO_PinSource20                BIT(20)     /*!< LGPIO PIN : Selects Pin Source 20 */
#define LGPIO_PinSource21                BIT(21)     /*!< LGPIO PIN : Selects Pin Source 21 */
#define LGPIO_PinSource22                BIT(22)     /*!< LGPIO PIN : Selects Pin Source 22 */
#define LGPIO_PinSource23                BIT(23)     /*!< LGPIO PIN : Selects Pin Source 23 */
#define LGPIO_PinSource24                BIT(24)     /*!< LGPIO PIN : Selects Pin Source 24 */
#define LGPIO_PinSource25                BIT(25)     /*!< LGPIO PIN : Selects Pin Source 25 */
#define LGPIO_PinSource26                BIT(26)     /*!< LGPIO PIN : Selects Pin Source 26 */
#define LGPIO_PinSource27                BIT(27)     /*!< LGPIO PIN : Selects Pin Source 27 */
#define LGPIO_PinSource28                BIT(28)     /*!< LGPIO PIN : Selects Pin Source 28 */
#define LGPIO_PinSource29                BIT(29)     /*!< LGPIO PIN : Selects Pin Source 29 */
#define LGPIO_PinSource30                BIT(30)     /*!< LGPIO PIN : Selects Pin Source 30 */
#define LGPIO_PinSource31                BIT(31)     /*!< LGPIO PIN : Selects Pin Source 31 */

#define LGPIO_PAD_GRP0_IVAL_OFFSET                                      0x0 /*!< N/A */
#define LGPIO_GPIO_GRP0_MODE0_OFFSET                                    0x8 /*!< N/A */
#define LGPIO_GPIO_GRP0_MODE1_OFFSET                                    0xc /*!< N/A */
#define LGPIO_PAD_GRP0_OVAL_OFFSET                                      0x10 /*!< N/A */
#define LGPIO_PAD_GRP0_RISE_IE_OFFSET                                   0x14 /*!< N/A */
#define LGPIO_PAD_GRP0_RISE_IP_OFFSET                                   0x18 /*!< N/A */
#define LGPIO_PAD_GRP0_FALL_IE_OFFSET                                   0x1c /*!< N/A */
#define LGPIO_PAD_GRP0_FALL_IP_OFFSET                                   0x20 /*!< N/A */
#define LGPIO_PAD_GRP0_HIGH_IE_OFFSET                                   0x24 /*!< N/A */
#define LGPIO_PAD_GRP0_HIGH_IP_OFFSET                                   0x28 /*!< N/A */
#define LGPIO_PAD_GRP0_LOW_IE_OFFSET                                    0x2c /*!< N/A */
#define LGPIO_PAD_GRP0_LOW_IP_OFFSET                                    0x30 /*!< N/A */
#define LGPIO_PAD_GRP0_POUT_MASK_OFFSET                                 0x44 /*!< N/A */
#define LGPIO_PAD_GRP0_SET_OFFSET                                       0x48 /*!< N/A */
#define LGPIO_PAD_GRP0_CLR_OFFSET                                       0x4c /*!< N/A */
#define LGPIO_PAD_GRP0_TOGGLE_OFFSET                                    0x50 /*!< N/A */
#define LGPIO_PAD_GRP0_PULL_MODE0_OFFSET                                0x54 /*!< N/A */
#define LGPIO_PAD_GRP0_PULL_MODE1_OFFSET                                0x58 /*!< N/A */
#define LGPIO_PAD_GRP0_SW_OFFSET                                        0x74 /*!< N/A */
#define LGPIO_PAD_GRP0_EVENT_RISE_OFFSET                                0x84 /*!< N/A */
#define LGPIO_PAD_GRP0_EVENT_FALL_OFFSET                                0x88 /*!< N/A */
#define LGPIO_PAD_GRP0_DIGITAL_NOISE_FILTER_OFFSET                      0x8c /*!< N/A */
#define LGPIO_GPIO_GRP0_OFFSET                                          0x90 /*!< N/A */
#define LGPIO_PAD_GRP0_EVENT_HIGH_OFFSET                                0x98 /*!< N/A */
#define LGPIO_PAD_GRP0_EVENT_LOW_OFFSET                                 0x9c /*!< N/A */
#define LGPIO_IP_VERSION_OFFSET                                         0x94 /*!< The IP version of LGPIO */
#define LGPIO_DIGITAL_NOISE_FILTER_OFFSET                               0xa0 /*!< N/A */

 /* ===== LGPIO PAD_GRP0_IVAL Register definition ===== */
#define LGPIO_PAD_GRP0_IVAL_IVAL                         BITS(0,7)                
 
 /* ===== LGPIO GPIO_GRP0_MODE0 Register definition ===== */
#define LGPIO_GPIO_GRP0_MODE0_MODE0_MASK                   BITS(0,7)                                   /*!< LGPIO GPIO GRP0 MODE0: MODE0 Bit Mask */  
#define LGPIO_GPIO_GRP0_MODE0_MODE0_OFS                    0U                                          /*!< LGPIO GPIO GRP0 MODE0: MODE0 Bit Offset */
#define LGPIO_GPIO_GRP0_MODE0_MODE0(regval)                (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO GPIO GRP0 MODE0: MODE0 Bit Value */  
 
 /* ===== LGPIO GPIO_GRP0_MODE1 Register definition ===== */
#define LGPIO_GPIO_GRP0_MODE1_MODE1_MASK                   BITS(0,7)                                   /*!< LGPIO GPIO GRP0 MODE1: MODE1 Bit Mask */  
#define LGPIO_GPIO_GRP0_MODE1_MODE1_OFS                    0U                                          /*!< LGPIO GPIO GRP0 MODE1: MODE1 Bit Offset */
#define LGPIO_GPIO_GRP0_MODE1_MODE1(regval)                (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO GPIO GRP0 MODE1: MODE1 Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_OVAL Register definition ===== */
#define LGPIO_PAD_GRP0_OVAL_OVAL_MASK                    BITS(0,7)                                   /*!< LGPIO PAD GRP0 OVAL: OVAL Bit Mask */  
#define LGPIO_PAD_GRP0_OVAL_OVAL_OFS                     0U                                          /*!< LGPIO PAD GRP0 OVAL: OVAL Bit Offset */
#define LGPIO_PAD_GRP0_OVAL_OVAL(regval)                 (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 OVAL: OVAL Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_RISE_IE Register definition ===== */
#define LGPIO_PAD_GRP0_RISE_IE_IE_MASK                      BITS(0,7)                                   /*!< LGPIO PAD GRP0 RISE IE: IE Bit Mask */  
#define LGPIO_PAD_GRP0_RISE_IE_IE_OFS                       0U                                          /*!< LGPIO PAD GRP0 RISE IE: IE Bit Offset */
#define LGPIO_PAD_GRP0_RISE_IE_IE(regval)                   (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 RISE IE: IE Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_RISE_IP Register definition ===== */
#define LGPIO_PAD_GRP0_RISE_IP_IP_MASK                      BITS(0,7)                                   /*!< LGPIO PAD GRP0 RISE IP: IP Bit Mask */  
#define LGPIO_PAD_GRP0_RISE_IP_IP_OFS                       0U                                          /*!< LGPIO PAD GRP0 RISE IP: IP Bit Offset */
#define LGPIO_PAD_GRP0_RISE_IP_IP(regval)                   (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 RISE IP: IP Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_FALL_IE Register definition ===== */
#define LGPIO_PAD_GRP0_FALL_IE_IE_MASK                      BITS(0,7)                                   /*!< LGPIO PAD GRP0 FALL IE: IE Bit Mask */  
#define LGPIO_PAD_GRP0_FALL_IE_IE_OFS                       0U                                          /*!< LGPIO PAD GRP0 FALL IE: IE Bit Offset */
#define LGPIO_PAD_GRP0_FALL_IE_IE(regval)                   (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 FALL IE: IE Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_FALL_IP Register definition ===== */
#define LGPIO_PAD_GRP0_FALL_IP_IP_MASK                      BITS(0,7)                                   /*!< LGPIO PAD GRP0 FALL IP: IP Bit Mask */  
#define LGPIO_PAD_GRP0_FALL_IP_IP_OFS                       0U                                          /*!< LGPIO PAD GRP0 FALL IP: IP Bit Offset */
#define LGPIO_PAD_GRP0_FALL_IP_IP(regval)                   (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 FALL IP: IP Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_HIGH_IE Register definition ===== */
#define LGPIO_PAD_GRP0_HIGH_IE_IE_MASK                      BITS(0,7)                                   /*!< LGPIO PAD GRP0 HIGH IE: IE Bit Mask */  
#define LGPIO_PAD_GRP0_HIGH_IE_IE_OFS                       0U                                          /*!< LGPIO PAD GRP0 HIGH IE: IE Bit Offset */
#define LGPIO_PAD_GRP0_HIGH_IE_IE(regval)                   (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 HIGH IE: IE Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_HIGH_IP Register definition ===== */
#define LGPIO_PAD_GRP0_HIGH_IP_IP_MASK                      BITS(0,7)                                   /*!< LGPIO PAD GRP0 HIGH IP: IP Bit Mask */  
#define LGPIO_PAD_GRP0_HIGH_IP_IP_OFS                       0U                                          /*!< LGPIO PAD GRP0 HIGH IP: IP Bit Offset */
#define LGPIO_PAD_GRP0_HIGH_IP_IP(regval)                   (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 HIGH IP: IP Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_LOW_IE Register definition ===== */
#define LGPIO_PAD_GRP0_LOW_IE_IE_MASK                      BITS(0,7)                                   /*!< LGPIO PAD GRP0 LOW IE: IE Bit Mask */  
#define LGPIO_PAD_GRP0_LOW_IE_IE_OFS                       0U                                          /*!< LGPIO PAD GRP0 LOW IE: IE Bit Offset */
#define LGPIO_PAD_GRP0_LOW_IE_IE(regval)                   (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 LOW IE: IE Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_LOW_IP Register definition ===== */
#define LGPIO_PAD_GRP0_LOW_IP_IP_MASK                      BITS(0,7)                                   /*!< LGPIO PAD GRP0 LOW IP: IP Bit Mask */  
#define LGPIO_PAD_GRP0_LOW_IP_IP_OFS                       0U                                          /*!< LGPIO PAD GRP0 LOW IP: IP Bit Offset */
#define LGPIO_PAD_GRP0_LOW_IP_IP(regval)                   (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 LOW IP: IP Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_POUT_MASK Register definition ===== */
#define LGPIO_PAD_GRP0_POUT_MASK_MASK_MASK                    BITS(0,7)                                   /*!< LGPIO PAD GRP0 POUT MASK: MASK Bit Mask */  
#define LGPIO_PAD_GRP0_POUT_MASK_MASK_OFS                     0U                                          /*!< LGPIO PAD GRP0 POUT MASK: MASK Bit Offset */
#define LGPIO_PAD_GRP0_POUT_MASK_MASK(regval)                 (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 POUT MASK: MASK Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_SET Register definition ===== */
#define LGPIO_PAD_GRP0_SET_SET_MASK                     BITS(0,7)                                   /*!< LGPIO PAD GRP0 SET: SET Bit Mask */  
#define LGPIO_PAD_GRP0_SET_SET_OFS                      0U                                          /*!< LGPIO PAD GRP0 SET: SET Bit Offset */
#define LGPIO_PAD_GRP0_SET_SET(regval)                  (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 SET: SET Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_CLR Register definition ===== */
#define LGPIO_PAD_GRP0_CLR_CLR_MASK                     BITS(0,7)                                   /*!< LGPIO PAD GRP0 CLR: CLR Bit Mask */  
#define LGPIO_PAD_GRP0_CLR_CLR_OFS                      0U                                          /*!< LGPIO PAD GRP0 CLR: CLR Bit Offset */
#define LGPIO_PAD_GRP0_CLR_CLR(regval)                  (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 CLR: CLR Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_TOGGLE Register definition ===== */
#define LGPIO_PAD_GRP0_TOGGLE_TOGGLE_MASK                  BITS(0,7)                                   /*!< LGPIO PAD GRP0 TOGGLE: TOGGLE Bit Mask */  
#define LGPIO_PAD_GRP0_TOGGLE_TOGGLE_OFS                   0U                                          /*!< LGPIO PAD GRP0 TOGGLE: TOGGLE Bit Offset */
#define LGPIO_PAD_GRP0_TOGGLE_TOGGLE(regval)               (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 TOGGLE: TOGGLE Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_PULL_MODE0 Register definition ===== */
#define LGPIO_PAD_GRP0_PULL_MODE0_MODE0_MASK                   BITS(0,7)                                   /*!< LGPIO PAD GRP0 PULL MODE0: MODE0 Bit Mask */  
#define LGPIO_PAD_GRP0_PULL_MODE0_MODE0_OFS                    0U                                          /*!< LGPIO PAD GRP0 PULL MODE0: MODE0 Bit Offset */
#define LGPIO_PAD_GRP0_PULL_MODE0_MODE0(regval)                (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 PULL MODE0: MODE0 Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_PULL_MODE1 Register definition ===== */
#define LGPIO_PAD_GRP0_PULL_MODE1_MODE1_MASK                   BITS(0,7)                                   /*!< LGPIO PAD GRP0 PULL MODE1: MODE1 Bit Mask */  
#define LGPIO_PAD_GRP0_PULL_MODE1_MODE1_OFS                    0U                                          /*!< LGPIO PAD GRP0 PULL MODE1: MODE1 Bit Offset */
#define LGPIO_PAD_GRP0_PULL_MODE1_MODE1(regval)                (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 PULL MODE1: MODE1 Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_SW Register definition ===== */
#define LGPIO_PAD_GRP0_SW_KEEP_MASK                    BITS(0,7)                                   /*!< LGPIO PAD GRP0 SW: KEEP Bit Mask */  
#define LGPIO_PAD_GRP0_SW_KEEP_OFS                     0U                                          /*!< LGPIO PAD GRP0 SW: KEEP Bit Offset */
#define LGPIO_PAD_GRP0_SW_KEEP(regval)                 (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 SW: KEEP Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_EVENT_RISE Register definition ===== */
#define LGPIO_PAD_GRP0_EVENT_RISE_RISE_EN_MASK                 BITS(0,7)                                   /*!< LGPIO PAD GRP0 EVENT RISE: RISE_EN Bit Mask */  
#define LGPIO_PAD_GRP0_EVENT_RISE_RISE_EN_OFS                  0U                                          /*!< LGPIO PAD GRP0 EVENT RISE: RISE_EN Bit Offset */
#define LGPIO_PAD_GRP0_EVENT_RISE_RISE_EN(regval)              (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 EVENT RISE: RISE_EN Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_EVENT_FALL Register definition ===== */
#define LGPIO_PAD_GRP0_EVENT_FALL_FALL_EN_MASK                 BITS(0,7)                                   /*!< LGPIO PAD GRP0 EVENT FALL: FALL_EN Bit Mask */  
#define LGPIO_PAD_GRP0_EVENT_FALL_FALL_EN_OFS                  0U                                          /*!< LGPIO PAD GRP0 EVENT FALL: FALL_EN Bit Offset */
#define LGPIO_PAD_GRP0_EVENT_FALL_FALL_EN(regval)              (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 EVENT FALL: FALL_EN Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_DIGITAL_NOISE_FILTER Register definition ===== */
#define LGPIO_PAD_GRP0_DIGITAL_NOISE_FILTER_EN_MASK                      BITS(0,7)                                   /*!< LGPIO PAD GRP0 DIGITAL NOISE FILTER: EN Bit Mask */  
#define LGPIO_PAD_GRP0_DIGITAL_NOISE_FILTER_EN_OFS                       0U                                          /*!< LGPIO PAD GRP0 DIGITAL NOISE FILTER: EN Bit Offset */
#define LGPIO_PAD_GRP0_DIGITAL_NOISE_FILTER_EN(regval)                   (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 DIGITAL NOISE FILTER: EN Bit Value */  
 
 /* ===== LGPIO GPIO_GRP0 Register definition ===== */
#define LGPIO_GPIO_GRP0_IRQ                          BITS(0,7)                
 
 /* ===== LGPIO PAD_GRP0_EVENT_HIGH Register definition ===== */
#define LGPIO_PAD_GRP0_EVENT_HIGH_HIGH_EN_MASK                 BITS(0,7)                                   /*!< LGPIO PAD GRP0 EVENT HIGH: HIGH_EN Bit Mask */  
#define LGPIO_PAD_GRP0_EVENT_HIGH_HIGH_EN_OFS                  0U                                          /*!< LGPIO PAD GRP0 EVENT HIGH: HIGH_EN Bit Offset */
#define LGPIO_PAD_GRP0_EVENT_HIGH_HIGH_EN(regval)              (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 EVENT HIGH: HIGH_EN Bit Value */  
 
 /* ===== LGPIO PAD_GRP0_EVENT_LOW Register definition ===== */
#define LGPIO_PAD_GRP0_EVENT_LOW_LOW_EN_MASK                  BITS(0,7)                                   /*!< LGPIO PAD GRP0 EVENT LOW: LOW_EN Bit Mask */  
#define LGPIO_PAD_GRP0_EVENT_LOW_LOW_EN_OFS                   0U                                          /*!< LGPIO PAD GRP0 EVENT LOW: LOW_EN Bit Offset */
#define LGPIO_PAD_GRP0_EVENT_LOW_LOW_EN(regval)               (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< LGPIO PAD GRP0 EVENT LOW: LOW_EN Bit Value */  
 
 /* ===== LGPIO IP_VERSION Register definition ===== */
#define LGPIO_IP_VERSION_VERSION_NUM                  BITS(0,31)                
 
 /* ===== LGPIO DIGITAL_NOISE_FILTER Register definition ===== */
#define LGPIO_DIGITAL_NOISE_FILTER_NUM_MASK                     BITS(0,24)                                   /*!< LGPIO DIGITAL NOISE FILTER: NUM Bit Mask */  
#define LGPIO_DIGITAL_NOISE_FILTER_NUM_OFS                      0U                                          /*!< LGPIO DIGITAL NOISE FILTER: NUM Bit Offset */
#define LGPIO_DIGITAL_NOISE_FILTER_NUM(regval)                  (BITS(0,24) & ((uint32_t)(regval) << 0))        /*!< LGPIO DIGITAL NOISE FILTER: NUM Bit Value */  

void LGPIO_IOFConfig(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, uint32_t IOF_Func);
void LGPIO_Output_Enable(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin);
void LGPIO_Input_Enable(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin);
void LGPIO_EventRiseConfig(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status);
void LGPIO_EventFallConfig(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status);
void LGPIO_WriteBit(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, uint32_t BitVal);

void LGPIO_Mode(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin,uint32_t mode);
void LGPIO_Toggle(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin);
void PAD_GRP0_SET(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin);

void LGPIO_PULLMode(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin,uint32_t pullmode);

void PAD_GRP0_CLR(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin);

void GPIO_SW_KEEP(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status);
uint32_t LGPIO_ReadInputDataBit(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin);
uint32_t LGPIO_GetITStatus(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, uint32_t LGPIO_IT);
uint32_t LGPIO_GetIRQStatus(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin);
uint32_t LGPIO_GetLGpioSt(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin);
void LGPIO_SetLGpioSt(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status);

void LGPIO_ITConfig(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, uint32_t LGPIO_IE, ControlStatus Status);
void LGPIO_ITClear(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, uint32_t LGPIO_IT);
void LGPIO_SetSoftIT(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, uint32_t LGPIO_IT);
void LGPIO_DsConfig(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status);
void LGPIO_OutXorConfig(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status);
void LGPIO_EVENT_HIGH(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status);
void LGPIO_EVENT_LOW(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status);
void LGPIO_DN_Filter_Num(LGPIO_TypeDef *LGPIOx, uint32_t num);
void LGPIO_Digital_Noise_Filter(LGPIO_TypeDef *LGPIOx, uint32_t LGPIO_Pin, ControlStatus Status);
#ifdef __cplusplus
}
#endif
#endif /* _NS_LGPIO_H */

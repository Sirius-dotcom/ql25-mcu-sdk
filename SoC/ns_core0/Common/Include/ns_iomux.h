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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _NS_IOMUX_H
#define _NS_IOMUX_H

/*!
 * @file     ns_iomux.h
 * @brief    This file contains all the functions prototypes for the IOMUX firmware
 */
#ifdef __cplusplus
 extern "C" {
#endif

#include "ns_iomux_reg.h"

#define PAD_NUM      54
#define LS_IOF_NUM      74

#define PAD_NUM                    54

/************************PAD Info Start***********************************/  

#define PAD0                          0
#define PAD0_LS_GROUP                 0
#define PAD0_HS0_PAD_QSPI1_SCK       PAD_QSPI1_SCK
#define PAD0_HS1_PAD_QSPI3_SCK       PAD_QSPI3_SCK
#define PAD0_HS2_PAD_QSPI5_SCK       PAD_QSPI5_SCK

#define PAD1                          1
#define PAD1_LS_GROUP                 0
#define PAD1_HS0_PAD_QSPI1_CS_0       PAD_QSPI1_CS_0
#define PAD1_HS1_PAD_QSPI3_CS_0       PAD_QSPI3_CS_0
#define PAD1_HS2_PAD_QSPI5_CS_0       PAD_QSPI5_CS_0

#define PAD2                          2
#define PAD2_LS_GROUP                 0
#define PAD2_HS0_PAD_QSPI1_DQ_0       PAD_QSPI1_DQ_0
#define PAD2_HS1_PAD_QSPI3_DQ_0       PAD_QSPI3_DQ_0
#define PAD2_HS2_PAD_QSPI5_DQ_0       PAD_QSPI5_DQ_0

#define PAD3                          3
#define PAD3_LS_GROUP                 0
#define PAD3_HS0_PAD_QSPI1_DQ_1       PAD_QSPI1_DQ_1
#define PAD3_HS1_PAD_QSPI3_DQ_1       PAD_QSPI3_DQ_1
#define PAD3_HS2_PAD_QSPI5_DQ_1       PAD_QSPI5_DQ_1

#define PAD4                          4
#define PAD4_LS_GROUP                 0
#define PAD4_HS0_PAD_QSPI1_DQ_2       PAD_QSPI1_DQ_2
#define PAD4_HS1_PAD_QSPI3_DQ_2       PAD_QSPI3_DQ_2
#define PAD4_HS2_PAD_QSPI5_DQ_2       PAD_QSPI5_DQ_2

#define PAD5                          5
#define PAD5_LS_GROUP                 0
#define PAD5_HS0_PAD_QSPI1_DQ_3       PAD_QSPI1_DQ_3
#define PAD5_HS1_PAD_QSPI3_DQ_3       PAD_QSPI3_DQ_3
#define PAD5_HS2_PAD_QSPI5_DQ_3       PAD_QSPI5_DQ_3

#define PAD6                          6
#define PAD6_LS_GROUP                 0
#define PAD6_HS0_PAD_USART1_RX       PAD_USART1_RX
#define PAD6_HS1_PAD_USART3_RX       PAD_USART3_RX
#define PAD6_HS2_PAD_USART5_RX       PAD_USART5_RX

#define PAD7                          7
#define PAD7_LS_GROUP                 0
#define PAD7_HS0_PAD_USART1_TX       PAD_USART1_TX
#define PAD7_HS1_PAD_USART3_TX       PAD_USART3_TX
#define PAD7_HS2_PAD_USART5_TX       PAD_USART5_TX

#define PAD8                          8
#define PAD8_LS_GROUP                 0
#define PAD8_HS0_PAD_USART1_CTS       PAD_USART1_CTS
#define PAD8_HS1_PAD_USART3_CTS       PAD_USART3_CTS
#define PAD8_HS2_PAD_USART5_CTS       PAD_USART5_CTS

#define PAD9                          9
#define PAD9_LS_GROUP                 0
#define PAD9_HS0_PAD_USART1_RTS       PAD_USART1_RTS
#define PAD9_HS1_PAD_USART3_RTS       PAD_USART3_RTS
#define PAD9_HS2_PAD_USART5_RTS       PAD_USART5_RTS

#define PAD10                          10
#define PAD10_LS_GROUP                 0
#define PAD10_HS0_PAD_USART1_SCLK       PAD_USART1_SCLK
#define PAD10_HS1_PAD_USART3_SCLK       PAD_USART3_SCLK
#define PAD10_HS2_PAD_USART5_SCLK       PAD_USART5_SCLK

#define PAD11                          11
#define PAD11_LS_GROUP                 0
#define PAD11_HS0_PAD_I2C1_SCL       PAD_I2C1_SCL
#define PAD11_HS1_PAD_I2C3_SCL       PAD_I2C3_SCL
#define PAD11_HS2_PAD_I2C5_SCL       PAD_I2C5_SCL

#define PAD12                          12
#define PAD12_LS_GROUP                 0
#define PAD12_HS0_PAD_I2C1_SDA       PAD_I2C1_SDA
#define PAD12_HS1_PAD_I2C3_SDA       PAD_I2C3_SDA
#define PAD12_HS2_PAD_I2C5_SDA       PAD_I2C5_SDA

#define PAD13                          13
#define PAD13_LS_GROUP                 0
#define PAD13_HS0_PAD_I2C1_SMBALERT       PAD_I2C1_SMBALERT
#define PAD13_HS1_PAD_I2C3_SMBALERT       PAD_I2C3_SMBALERT
#define PAD13_HS2_PAD_I2C5_SMBALERT       PAD_I2C5_SMBALERT

#define PAD14                          14
#define PAD14_LS_GROUP                 0
#define PAD14_HS0_NONE       NONE
#define PAD14_HS1_NONE       NONE
#define PAD14_HS2_NONE       NONE

#define PAD15                          15
#define PAD15_LS_GROUP                 0
#define PAD15_HS0_NONE       NONE
#define PAD15_HS1_NONE       NONE
#define PAD15_HS2_NONE       NONE

#define PAD16                          16
#define PAD16_LS_GROUP                 0
#define PAD16_HS0_NONE       NONE
#define PAD16_HS1_NONE       NONE
#define PAD16_HS2_NONE       NONE

#define PAD17                          17
#define PAD17_LS_GROUP                 0
#define PAD17_HS0_NONE       NONE
#define PAD17_HS1_NONE       NONE
#define PAD17_HS2_NONE       NONE

#define PAD18                          18
#define PAD18_LS_GROUP                 0
#define PAD18_HS0_NONE       NONE
#define PAD18_HS1_NONE       NONE
#define PAD18_HS2_NONE       NONE

#define PAD19                          19
#define PAD19_LS_GROUP                 0
#define PAD19_HS0_NONE       NONE
#define PAD19_HS1_NONE       NONE
#define PAD19_HS2_NONE       NONE

#define PAD20                          20
#define PAD20_LS_GROUP                 0
#define PAD20_HS0_NONE       NONE
#define PAD20_HS1_NONE       NONE
#define PAD20_HS2_NONE       NONE

#define PAD21                          21
#define PAD21_LS_GROUP                 0
#define PAD21_HS0_PAD_QSPI2_SCK       PAD_QSPI2_SCK
#define PAD21_HS1_PAD_QSPI4_SCK       PAD_QSPI4_SCK
#define PAD21_HS2_PAD_QSPI6_SCK       PAD_QSPI6_SCK

#define PAD22                          22
#define PAD22_LS_GROUP                 0
#define PAD22_HS0_PAD_QSPI2_CS_0       PAD_QSPI2_CS_0
#define PAD22_HS1_PAD_QSPI4_CS_0       PAD_QSPI4_CS_0
#define PAD22_HS2_PAD_QSPI6_CS_0       PAD_QSPI6_CS_0

#define PAD23                          23
#define PAD23_LS_GROUP                 0
#define PAD23_HS0_PAD_QSPI2_DQ_0       PAD_QSPI2_DQ_0
#define PAD23_HS1_PAD_QSPI4_DQ_0       PAD_QSPI4_DQ_0
#define PAD23_HS2_PAD_QSPI6_DQ_0       PAD_QSPI6_DQ_0

#define PAD24                          24
#define PAD24_LS_GROUP                 0
#define PAD24_HS0_PAD_QSPI2_DQ_1       PAD_QSPI2_DQ_1
#define PAD24_HS1_PAD_QSPI4_DQ_1       PAD_QSPI4_DQ_1
#define PAD24_HS2_PAD_QSPI6_DQ_1       PAD_QSPI6_DQ_1

#define PAD25                          25
#define PAD25_LS_GROUP                 0
#define PAD25_HS0_PAD_QSPI2_DQ_2       PAD_QSPI2_DQ_2
#define PAD25_HS1_PAD_QSPI4_DQ_2       PAD_QSPI4_DQ_2
#define PAD25_HS2_PAD_QSPI6_DQ_2       PAD_QSPI6_DQ_2

#define PAD26                          26
#define PAD26_LS_GROUP                 0
#define PAD26_HS0_PAD_QSPI2_DQ_3       PAD_QSPI2_DQ_3
#define PAD26_HS1_PAD_QSPI4_DQ_3       PAD_QSPI4_DQ_3
#define PAD26_HS2_PAD_QSPI6_DQ_3       PAD_QSPI6_DQ_3

#define PAD27                          27
#define PAD27_LS_GROUP                 0
#define PAD27_HS0_PAD_USART2_RX       PAD_USART2_RX
#define PAD27_HS1_PAD_USART4_RX       PAD_USART4_RX
#define PAD27_HS2_PAD_USART6_RX       PAD_USART6_RX

#define PAD28                          28
#define PAD28_LS_GROUP                 0
#define PAD28_HS0_PAD_USART2_TX       PAD_USART2_TX
#define PAD28_HS1_PAD_USART4_TX       PAD_USART4_TX
#define PAD28_HS2_PAD_USART6_TX       PAD_USART6_TX

#define PAD29                          29
#define PAD29_LS_GROUP                 0
#define PAD29_HS0_PAD_USART2_CTS       PAD_USART2_CTS
#define PAD29_HS1_PAD_USART4_CTS       PAD_USART4_CTS
#define PAD29_HS2_PAD_USART6_CTS       PAD_USART6_CTS

#define PAD30                          30
#define PAD30_LS_GROUP                 0
#define PAD30_HS0_PAD_USART2_RTS       PAD_USART2_RTS
#define PAD30_HS1_PAD_USART4_RTS       PAD_USART4_RTS
#define PAD30_HS2_PAD_USART6_RTS       PAD_USART6_RTS

#define PAD31                          31
#define PAD31_LS_GROUP                 0
#define PAD31_HS0_PAD_USART2_SCLK       PAD_USART2_SCLK
#define PAD31_HS1_PAD_USART4_SCLK       PAD_USART4_SCLK
#define PAD31_HS2_PAD_USART6_SCLK       PAD_USART6_SCLK

#define PAD32                          32
#define PAD32_LS_GROUP                 0
#define PAD32_HS0_PAD_I2C2_SCL       PAD_I2C2_SCL
#define PAD32_HS1_PAD_I2C4_SCL       PAD_I2C4_SCL
#define PAD32_HS2_PAD_I2C6_SCL       PAD_I2C6_SCL

#define PAD33                          33
#define PAD33_LS_GROUP                 0
#define PAD33_HS0_PAD_I2C2_SDA       PAD_I2C2_SDA
#define PAD33_HS1_PAD_I2C4_SDA       PAD_I2C4_SDA
#define PAD33_HS2_PAD_I2C6_SDA       PAD_I2C6_SDA

#define PAD34                          34
#define PAD34_LS_GROUP                 0
#define PAD34_HS0_PAD_I2C2_SMBALERT       PAD_I2C2_SMBALERT
#define PAD34_HS1_PAD_I2C4_SMBALERT       PAD_I2C4_SMBALERT
#define PAD34_HS2_PAD_I2C6_SMBALERT       PAD_I2C6_SMBALERT

#define PAD35                          35
#define PAD35_LS_GROUP                 0
#define PAD35_HS0_PAD_QSPI1_CS_1       PAD_QSPI1_CS_1
#define PAD35_HS1_PAD_QSPI3_CS_1       PAD_QSPI3_CS_1
#define PAD35_HS2_PAD_QSPI5_CS_1       PAD_QSPI5_CS_1

#define PAD36                          36
#define PAD36_LS_GROUP                 0
#define PAD36_HS0_PAD_QSPI1_CS_2       PAD_QSPI1_CS_2
#define PAD36_HS1_PAD_QSPI3_CS_2       PAD_QSPI3_CS_2
#define PAD36_HS2_PAD_QSPI5_CS_2       PAD_QSPI5_CS_2

#define PAD37                          37
#define PAD37_LS_GROUP                 0
#define PAD37_HS0_PAD_QSPI1_CS_3       PAD_QSPI1_CS_3
#define PAD37_HS1_PAD_QSPI3_CS_3       PAD_QSPI3_CS_3
#define PAD37_HS2_PAD_QSPI5_CS_3       PAD_QSPI5_CS_3

#define PAD38                          38
#define PAD38_LS_GROUP                 0
#define PAD38_HS0_PAD_QSPI2_CS_1       PAD_QSPI2_CS_1
#define PAD38_HS1_PAD_QSPI4_CS_1       PAD_QSPI4_CS_1
#define PAD38_HS2_PAD_QSPI6_CS_1       PAD_QSPI6_CS_1

#define PAD39                          39
#define PAD39_LS_GROUP                 0
#define PAD39_HS0_PAD_QSPI2_CS_2       PAD_QSPI2_CS_2
#define PAD39_HS1_PAD_QSPI4_CS_2       PAD_QSPI4_CS_2
#define PAD39_HS2_PAD_QSPI6_CS_2       PAD_QSPI6_CS_2

#define PAD40                          40
#define PAD40_LS_GROUP                 0
#define PAD40_HS0_PAD_QSPI2_CS_3       PAD_QSPI2_CS_3
#define PAD40_HS1_PAD_QSPI4_CS_3       PAD_QSPI4_CS_3
#define PAD40_HS2_PAD_QSPI6_CS_3       PAD_QSPI6_CS_3

#define PAD41                          41
#define PAD41_LS_GROUP                 0
#define PAD41_HS0_PAD_QSPI1_DQ_4       PAD_QSPI1_DQ_4
#define PAD41_HS1_PAD_QSPI3_DQ_4       PAD_QSPI3_DQ_4
#define PAD41_HS2_PAD_QSPI5_DQ_4       PAD_QSPI5_DQ_4

#define PAD42                          42
#define PAD42_LS_GROUP                 0
#define PAD42_HS0_PAD_QSPI1_DQ_5       PAD_QSPI1_DQ_5
#define PAD42_HS1_PAD_QSPI3_DQ_5       PAD_QSPI3_DQ_5
#define PAD42_HS2_PAD_QSPI5_DQ_5       PAD_QSPI5_DQ_5

#define PAD43                          43
#define PAD43_LS_GROUP                 0
#define PAD43_HS0_PAD_QSPI1_DQ_6       PAD_QSPI1_DQ_6
#define PAD43_HS1_PAD_QSPI3_DQ_6       PAD_QSPI3_DQ_6
#define PAD43_HS2_PAD_QSPI5_DQ_6       PAD_QSPI5_DQ_6

#define PAD44                          44
#define PAD44_LS_GROUP                 0
#define PAD44_HS0_PAD_QSPI1_DQ_7       PAD_QSPI1_DQ_7
#define PAD44_HS1_PAD_QSPI3_DQ_7       PAD_QSPI3_DQ_7
#define PAD44_HS2_PAD_QSPI5_DQ_7       PAD_QSPI5_DQ_7

#define PAD45                          45
#define PAD45_LS_GROUP                 0
#define PAD45_HS0_PAD_QSPI2_DQ_4       PAD_QSPI2_DQ_4
#define PAD45_HS1_PAD_QSPI4_DQ_4       PAD_QSPI4_DQ_4
#define PAD45_HS2_PAD_QSPI6_DQ_4       PAD_QSPI6_DQ_4

#define PAD46                          46
#define PAD46_LS_GROUP                 0
#define PAD46_HS0_PAD_QSPI2_DQ_5       PAD_QSPI2_DQ_5
#define PAD46_HS1_PAD_QSPI4_DQ_5       PAD_QSPI4_DQ_5
#define PAD46_HS2_PAD_QSPI6_DQ_5       PAD_QSPI6_DQ_5

#define PAD47                          47
#define PAD47_LS_GROUP                 0
#define PAD47_HS0_PAD_QSPI2_DQ_6       PAD_QSPI2_DQ_6
#define PAD47_HS1_PAD_QSPI4_DQ_6       PAD_QSPI4_DQ_6
#define PAD47_HS2_PAD_QSPI6_DQ_6       PAD_QSPI6_DQ_6

#define PAD48                          48
#define PAD48_LS_GROUP                 0
#define PAD48_HS0_PAD_QSPI2_DQ_7       PAD_QSPI2_DQ_7
#define PAD48_HS1_PAD_QSPI4_DQ_7       PAD_QSPI4_DQ_7
#define PAD48_HS2_PAD_QSPI6_DQ_7       PAD_QSPI6_DQ_7

#define PAD49                          49
#define PAD49_LS_GROUP                 0
#define PAD49_HS0_PAD_USART7_RX       PAD_USART7_RX
#define PAD49_HS1_PAD_I2C7_SCL       PAD_I2C7_SCL
#define PAD49_HS2_NONE       NONE

#define PAD50                          50
#define PAD50_LS_GROUP                 0
#define PAD50_HS0_PAD_USART7_TX       PAD_USART7_TX
#define PAD50_HS1_PAD_I2C7_SDA       PAD_I2C7_SDA
#define PAD50_HS2_NONE       NONE

#define PAD51                          51
#define PAD51_LS_GROUP                 0
#define PAD51_HS0_PAD_USART7_CTS       PAD_USART7_CTS
#define PAD51_HS1_PAD_I2C7_SMBALERT       PAD_I2C7_SMBALERT
#define PAD51_HS2_NONE       NONE

#define PAD52                          52
#define PAD52_LS_GROUP                 0
#define PAD52_HS0_PAD_USART7_RTS       PAD_USART7_RTS
#define PAD52_HS1_NONE       NONE
#define PAD52_HS2_NONE       NONE

#define PAD53                          53
#define PAD53_LS_GROUP                 0
#define PAD53_HS0_PAD_USART7_SCLK       PAD_USART7_SCLK
#define PAD53_HS1_NONE       NONE
#define PAD53_HS2_NONE       NONE

struct pad_group{
    uint32_t pad_id;
    uint32_t group_id;
};

struct ls_iof_t{
    uint32_t iof_id;
    uint32_t grp_id;
};

struct ls_grp_iof_t{
    uint32_t grp_iof_start_id;
    uint32_t grp_iof_end_id;
};

enum hs_function_e{
                
    FUNC_USART1_RX = 6,  /*!< USART1 RX  */
                
    FUNC_USART1_TX = 7,  /*!< USART1 TX  */
                
    FUNC_USART1_SCLK = 10,  /*!< USART1 SCLK  */
                
    FUNC_USART1_CTS = 8,  /*!< USART1 CTS  */
                
    FUNC_USART1_RTS = 9,  /*!< USART1 RTS  */
                
    FUNC_USART2_RX = 27,  /*!< USART2 RX  */
                
    FUNC_USART2_TX = 28,  /*!< USART2 TX  */
                
    FUNC_USART2_SCLK = 31,  /*!< USART2 SCLK  */
                
    FUNC_USART2_CTS = 29,  /*!< USART2 CTS  */
                
    FUNC_USART2_RTS = 30,  /*!< USART2 RTS  */
                
    FUNC_USART3_RX = 6,  /*!< USART3 RX  */
                
    FUNC_USART3_TX = 7,  /*!< USART3 TX  */
                
    FUNC_USART3_SCLK = 10,  /*!< USART3 SCLK  */
                
    FUNC_USART3_CTS = 8,  /*!< USART3 CTS  */
                
    FUNC_USART3_RTS = 9,  /*!< USART3 RTS  */
                
    FUNC_USART4_RX = 27,  /*!< USART4 RX  */
                
    FUNC_USART4_TX = 28,  /*!< USART4 TX  */
                
    FUNC_USART4_SCLK = 31,  /*!< USART4 SCLK  */
                
    FUNC_USART4_CTS = 29,  /*!< USART4 CTS  */
                
    FUNC_USART4_RTS = 30,  /*!< USART4 RTS  */
                
    FUNC_I2C1_SCL = 11,  /*!< I2C1 SCL  */
                
    FUNC_I2C1_SDA = 12,  /*!< I2C1 SDA  */
                
    FUNC_I2C1_SMBALERT = 13,  /*!< I2C1 SMBALERT  */
                
    FUNC_QSPI1_SCK = 0,  /*!< QSPI1 SCK  */
                
    FUNC_QSPI1_CS_0 = 1,  /*!< QSPI1 CS 0  */
                
    FUNC_QSPI1_DQ_0 = 2,  /*!< QSPI1 DQ 0  */
                
    FUNC_QSPI1_DQ_1 = 3,  /*!< QSPI1 DQ 1  */
                
    FUNC_QSPI1_DQ_2 = 4,  /*!< QSPI1 DQ 2  */
                
    FUNC_QSPI1_DQ_3 = 5,  /*!< QSPI1 DQ 3  */
                
    FUNC_QSPI2_SCK = 21,  /*!< QSPI2 SCK  */
                
    FUNC_QSPI2_CS_0 = 22,  /*!< QSPI2 CS 0  */
                
    FUNC_QSPI2_DQ_0 = 23,  /*!< QSPI2 DQ 0  */
                
    FUNC_QSPI2_DQ_1 = 24,  /*!< QSPI2 DQ 1  */
                
    FUNC_QSPI2_DQ_2 = 25,  /*!< QSPI2 DQ 2  */
                
    FUNC_QSPI2_DQ_3 = 26,  /*!< QSPI2 DQ 3  */
                
    FUNC_QSPI3_SCK = 0,  /*!< QSPI3 SCK  */
                
    FUNC_QSPI3_CS_0 = 1,  /*!< QSPI3 CS 0  */
                
    FUNC_QSPI3_DQ_0 = 2,  /*!< QSPI3 DQ 0  */
                
    FUNC_QSPI3_DQ_1 = 3,  /*!< QSPI3 DQ 1  */
                
    FUNC_QSPI3_DQ_2 = 4,  /*!< QSPI3 DQ 2  */
                
    FUNC_QSPI3_DQ_3 = 5,  /*!< QSPI3 DQ 3  */
};

/************************PAD Info End***********************************/  

/************************LS Info Start**********************************/     
                    
/************************LS Info End**********************************/ 

#define USART1_RX_IOF_OVAL     6
#define USART1_RX_IOF_IVAL     6
#define USART1_RX_HS_SEL       1
#define USART1_RX_IOMUX_SEL    IOMUX
                
#define USART1_TX_IOF_OVAL     7
#define USART1_TX_IOF_IVAL     7
#define USART1_TX_HS_SEL       1
#define USART1_TX_IOMUX_SEL    IOMUX
                
#define USART1_SCLK_IOF_OVAL     10
#define USART1_SCLK_IOF_IVAL     10
#define USART1_SCLK_HS_SEL       1
#define USART1_SCLK_IOMUX_SEL    IOMUX
                
#define USART1_CTS_IOF_OVAL     8
#define USART1_CTS_IOF_IVAL     8
#define USART1_CTS_HS_SEL       1
#define USART1_CTS_IOMUX_SEL    IOMUX
                
#define USART1_RTS_IOF_OVAL     9
#define USART1_RTS_IOF_IVAL     9
#define USART1_RTS_HS_SEL       1
#define USART1_RTS_IOMUX_SEL    IOMUX
                
#define USART2_RX_IOF_OVAL     27
#define USART2_RX_IOF_IVAL     27
#define USART2_RX_HS_SEL       1
#define USART2_RX_IOMUX_SEL    IOMUX
                
#define USART2_TX_IOF_OVAL     28
#define USART2_TX_IOF_IVAL     28
#define USART2_TX_HS_SEL       1
#define USART2_TX_IOMUX_SEL    IOMUX
                
#define USART2_SCLK_IOF_OVAL     31
#define USART2_SCLK_IOF_IVAL     31
#define USART2_SCLK_HS_SEL       1
#define USART2_SCLK_IOMUX_SEL    IOMUX
                
#define USART2_CTS_IOF_OVAL     29
#define USART2_CTS_IOF_IVAL     29
#define USART2_CTS_HS_SEL       1
#define USART2_CTS_IOMUX_SEL    IOMUX
                
#define USART2_RTS_IOF_OVAL     30
#define USART2_RTS_IOF_IVAL     30
#define USART2_RTS_HS_SEL       1
#define USART2_RTS_IOMUX_SEL    IOMUX
                
#define USART3_RX_IOF_OVAL     6
#define USART3_RX_IOF_IVAL     6
#define USART3_RX_HS_SEL       2
#define USART3_RX_IOMUX_SEL    IOMUX
                
#define USART3_TX_IOF_OVAL     7
#define USART3_TX_IOF_IVAL     7
#define USART3_TX_HS_SEL       2
#define USART3_TX_IOMUX_SEL    IOMUX
                
#define USART3_SCLK_IOF_OVAL     10
#define USART3_SCLK_IOF_IVAL     10
#define USART3_SCLK_HS_SEL       2
#define USART3_SCLK_IOMUX_SEL    IOMUX
                
#define USART3_CTS_IOF_OVAL     8
#define USART3_CTS_IOF_IVAL     8
#define USART3_CTS_HS_SEL       2
#define USART3_CTS_IOMUX_SEL    IOMUX
                
#define USART3_RTS_IOF_OVAL     9
#define USART3_RTS_IOF_IVAL     9
#define USART3_RTS_HS_SEL       2
#define USART3_RTS_IOMUX_SEL    IOMUX
                
#define USART4_RX_IOF_OVAL     27
#define USART4_RX_IOF_IVAL     27
#define USART4_RX_HS_SEL       2
#define USART4_RX_IOMUX_SEL    IOMUX
                
#define USART4_TX_IOF_OVAL     28
#define USART4_TX_IOF_IVAL     28
#define USART4_TX_HS_SEL       2
#define USART4_TX_IOMUX_SEL    IOMUX
                
#define USART4_SCLK_IOF_OVAL     31
#define USART4_SCLK_IOF_IVAL     31
#define USART4_SCLK_HS_SEL       2
#define USART4_SCLK_IOMUX_SEL    IOMUX
                
#define USART4_CTS_IOF_OVAL     29
#define USART4_CTS_IOF_IVAL     29
#define USART4_CTS_HS_SEL       2
#define USART4_CTS_IOMUX_SEL    IOMUX
                
#define USART4_RTS_IOF_OVAL     30
#define USART4_RTS_IOF_IVAL     30
#define USART4_RTS_HS_SEL       2
#define USART4_RTS_IOMUX_SEL    IOMUX
                
#define I2C1_SCL_IOF_OVAL     11
#define I2C1_SCL_IOF_IVAL     11
#define I2C1_SCL_HS_SEL       1
#define I2C1_SCL_IOMUX_SEL    IOMUX
                
#define I2C1_SDA_IOF_OVAL     12
#define I2C1_SDA_IOF_IVAL     12
#define I2C1_SDA_HS_SEL       1
#define I2C1_SDA_IOMUX_SEL    IOMUX
                
#define I2C1_SMBALERT_IOF_OVAL     13
#define I2C1_SMBALERT_IOF_IVAL     13
#define I2C1_SMBALERT_HS_SEL       1
#define I2C1_SMBALERT_IOMUX_SEL    IOMUX
                
#define QSPI1_SCK_IOF_OVAL     0
#define QSPI1_SCK_IOF_IVAL     0
#define QSPI1_SCK_HS_SEL       1
#define QSPI1_SCK_IOMUX_SEL    IOMUX
                
#define QSPI1_CS_0_IOF_OVAL     1
#define QSPI1_CS_0_IOF_IVAL     1
#define QSPI1_CS_0_HS_SEL       1
#define QSPI1_CS_0_IOMUX_SEL    IOMUX
                
#define QSPI1_DQ_0_IOF_OVAL     2
#define QSPI1_DQ_0_IOF_IVAL     2
#define QSPI1_DQ_0_HS_SEL       1
#define QSPI1_DQ_0_IOMUX_SEL    IOMUX
                
#define QSPI1_DQ_1_IOF_OVAL     3
#define QSPI1_DQ_1_IOF_IVAL     3
#define QSPI1_DQ_1_HS_SEL       1
#define QSPI1_DQ_1_IOMUX_SEL    IOMUX
                
#define QSPI1_DQ_2_IOF_OVAL     4
#define QSPI1_DQ_2_IOF_IVAL     4
#define QSPI1_DQ_2_HS_SEL       1
#define QSPI1_DQ_2_IOMUX_SEL    IOMUX
                
#define QSPI1_DQ_3_IOF_OVAL     5
#define QSPI1_DQ_3_IOF_IVAL     5
#define QSPI1_DQ_3_HS_SEL       1
#define QSPI1_DQ_3_IOMUX_SEL    IOMUX
                
#define QSPI2_SCK_IOF_OVAL     21
#define QSPI2_SCK_IOF_IVAL     21
#define QSPI2_SCK_HS_SEL       1
#define QSPI2_SCK_IOMUX_SEL    IOMUX
                
#define QSPI2_CS_0_IOF_OVAL     22
#define QSPI2_CS_0_IOF_IVAL     22
#define QSPI2_CS_0_HS_SEL       1
#define QSPI2_CS_0_IOMUX_SEL    IOMUX
                
#define QSPI2_DQ_0_IOF_OVAL     23
#define QSPI2_DQ_0_IOF_IVAL     23
#define QSPI2_DQ_0_HS_SEL       1
#define QSPI2_DQ_0_IOMUX_SEL    IOMUX
                
#define QSPI2_DQ_1_IOF_OVAL     24
#define QSPI2_DQ_1_IOF_IVAL     24
#define QSPI2_DQ_1_HS_SEL       1
#define QSPI2_DQ_1_IOMUX_SEL    IOMUX
                
#define QSPI2_DQ_2_IOF_OVAL     25
#define QSPI2_DQ_2_IOF_IVAL     25
#define QSPI2_DQ_2_HS_SEL       1
#define QSPI2_DQ_2_IOMUX_SEL    IOMUX
                
#define QSPI2_DQ_3_IOF_OVAL     26
#define QSPI2_DQ_3_IOF_IVAL     26
#define QSPI2_DQ_3_HS_SEL       1
#define QSPI2_DQ_3_IOMUX_SEL    IOMUX
                
#define QSPI3_SCK_IOF_OVAL     0
#define QSPI3_SCK_IOF_IVAL     0
#define QSPI3_SCK_HS_SEL       2
#define QSPI3_SCK_IOMUX_SEL    IOMUX
                
#define QSPI3_CS_0_IOF_OVAL     1
#define QSPI3_CS_0_IOF_IVAL     1
#define QSPI3_CS_0_HS_SEL       2
#define QSPI3_CS_0_IOMUX_SEL    IOMUX
                
#define QSPI3_DQ_0_IOF_OVAL     2
#define QSPI3_DQ_0_IOF_IVAL     2
#define QSPI3_DQ_0_HS_SEL       2
#define QSPI3_DQ_0_IOMUX_SEL    IOMUX
                
#define QSPI3_DQ_1_IOF_OVAL     3
#define QSPI3_DQ_1_IOF_IVAL     3
#define QSPI3_DQ_1_HS_SEL       2
#define QSPI3_DQ_1_IOMUX_SEL    IOMUX
                
#define QSPI3_DQ_2_IOF_OVAL     4
#define QSPI3_DQ_2_IOF_IVAL     4
#define QSPI3_DQ_2_HS_SEL       2
#define QSPI3_DQ_2_IOMUX_SEL    IOMUX
                
#define QSPI3_DQ_3_IOF_OVAL     5
#define QSPI3_DQ_3_IOF_IVAL     5
#define QSPI3_DQ_3_HS_SEL       2
#define QSPI3_DQ_3_IOMUX_SEL    IOMUX

#define ADVANCED_TIMER0_TIM_ETR_0_IOF_OVAL     0
#define ADVANCED_TIMER0_TIM_ETR_0_IOF_IVAL     0
#define ADVANCED_TIMER0_TIM_ETR_0_HS_SEL       0
#define ADVANCED_TIMER0_TIM_ETR_0_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER0_TIM_CH1_IOF_OVAL     1
#define ADVANCED_TIMER0_TIM_CH1_IOF_IVAL     1
#define ADVANCED_TIMER0_TIM_CH1_HS_SEL       0
#define ADVANCED_TIMER0_TIM_CH1_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER0_TIM_CH2_IOF_OVAL     2
#define ADVANCED_TIMER0_TIM_CH2_IOF_IVAL     2
#define ADVANCED_TIMER0_TIM_CH2_HS_SEL       0
#define ADVANCED_TIMER0_TIM_CH2_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER0_TIM_CH3_IOF_OVAL     3
#define ADVANCED_TIMER0_TIM_CH3_IOF_IVAL     3
#define ADVANCED_TIMER0_TIM_CH3_HS_SEL       0
#define ADVANCED_TIMER0_TIM_CH3_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER0_TIM_CH4_IOF_OVAL     4
#define ADVANCED_TIMER0_TIM_CH4_IOF_IVAL     4
#define ADVANCED_TIMER0_TIM_CH4_HS_SEL       0
#define ADVANCED_TIMER0_TIM_CH4_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER0_TIM_BKIN_0_IOF_OVAL     5
#define ADVANCED_TIMER0_TIM_BKIN_0_IOF_IVAL     5
#define ADVANCED_TIMER0_TIM_BKIN_0_HS_SEL       0
#define ADVANCED_TIMER0_TIM_BKIN_0_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER0_TIM_BKIN2_0_IOF_OVAL     6
#define ADVANCED_TIMER0_TIM_BKIN2_0_IOF_IVAL     6
#define ADVANCED_TIMER0_TIM_BKIN2_0_HS_SEL       0
#define ADVANCED_TIMER0_TIM_BKIN2_0_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER0_TIM_CH1N_IOF_OVAL     7
#define ADVANCED_TIMER0_TIM_CH1N_IOF_IVAL     7
#define ADVANCED_TIMER0_TIM_CH1N_HS_SEL       0
#define ADVANCED_TIMER0_TIM_CH1N_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER0_TIM_CH2N_IOF_OVAL     8
#define ADVANCED_TIMER0_TIM_CH2N_IOF_IVAL     8
#define ADVANCED_TIMER0_TIM_CH2N_HS_SEL       0
#define ADVANCED_TIMER0_TIM_CH2N_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER0_TIM_CH3N_IOF_OVAL     9
#define ADVANCED_TIMER0_TIM_CH3N_IOF_IVAL     9
#define ADVANCED_TIMER0_TIM_CH3N_HS_SEL       0
#define ADVANCED_TIMER0_TIM_CH3N_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER1_TIM_ETR_0_IOF_OVAL     10
#define ADVANCED_TIMER1_TIM_ETR_0_IOF_IVAL     10
#define ADVANCED_TIMER1_TIM_ETR_0_HS_SEL       0
#define ADVANCED_TIMER1_TIM_ETR_0_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER1_TIM_CH1_IOF_OVAL     11
#define ADVANCED_TIMER1_TIM_CH1_IOF_IVAL     11
#define ADVANCED_TIMER1_TIM_CH1_HS_SEL       0
#define ADVANCED_TIMER1_TIM_CH1_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER1_TIM_CH2_IOF_OVAL     12
#define ADVANCED_TIMER1_TIM_CH2_IOF_IVAL     12
#define ADVANCED_TIMER1_TIM_CH2_HS_SEL       0
#define ADVANCED_TIMER1_TIM_CH2_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER1_TIM_CH3_IOF_OVAL     13
#define ADVANCED_TIMER1_TIM_CH3_IOF_IVAL     13
#define ADVANCED_TIMER1_TIM_CH3_HS_SEL       0
#define ADVANCED_TIMER1_TIM_CH3_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER1_TIM_CH4_IOF_OVAL     14
#define ADVANCED_TIMER1_TIM_CH4_IOF_IVAL     14
#define ADVANCED_TIMER1_TIM_CH4_HS_SEL       0
#define ADVANCED_TIMER1_TIM_CH4_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER1_TIM_BKIN_0_IOF_OVAL     15
#define ADVANCED_TIMER1_TIM_BKIN_0_IOF_IVAL     15
#define ADVANCED_TIMER1_TIM_BKIN_0_HS_SEL       0
#define ADVANCED_TIMER1_TIM_BKIN_0_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER1_TIM_BKIN2_0_IOF_OVAL     16
#define ADVANCED_TIMER1_TIM_BKIN2_0_IOF_IVAL     16
#define ADVANCED_TIMER1_TIM_BKIN2_0_HS_SEL       0
#define ADVANCED_TIMER1_TIM_BKIN2_0_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER1_TIM_CH1N_IOF_OVAL     17
#define ADVANCED_TIMER1_TIM_CH1N_IOF_IVAL     17
#define ADVANCED_TIMER1_TIM_CH1N_HS_SEL       0
#define ADVANCED_TIMER1_TIM_CH1N_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER1_TIM_CH2N_IOF_OVAL     18
#define ADVANCED_TIMER1_TIM_CH2N_IOF_IVAL     18
#define ADVANCED_TIMER1_TIM_CH2N_HS_SEL       0
#define ADVANCED_TIMER1_TIM_CH2N_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER1_TIM_CH3N_IOF_OVAL     19
#define ADVANCED_TIMER1_TIM_CH3N_IOF_IVAL     19
#define ADVANCED_TIMER1_TIM_CH3N_HS_SEL       0
#define ADVANCED_TIMER1_TIM_CH3N_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER2_TIM_ETR_0_IOF_OVAL     20
#define ADVANCED_TIMER2_TIM_ETR_0_IOF_IVAL     20
#define ADVANCED_TIMER2_TIM_ETR_0_HS_SEL       0
#define ADVANCED_TIMER2_TIM_ETR_0_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER2_TIM_CH1_IOF_OVAL     21
#define ADVANCED_TIMER2_TIM_CH1_IOF_IVAL     21
#define ADVANCED_TIMER2_TIM_CH1_HS_SEL       0
#define ADVANCED_TIMER2_TIM_CH1_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER2_TIM_CH2_IOF_OVAL     22
#define ADVANCED_TIMER2_TIM_CH2_IOF_IVAL     22
#define ADVANCED_TIMER2_TIM_CH2_HS_SEL       0
#define ADVANCED_TIMER2_TIM_CH2_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER2_TIM_CH3_IOF_OVAL     23
#define ADVANCED_TIMER2_TIM_CH3_IOF_IVAL     23
#define ADVANCED_TIMER2_TIM_CH3_HS_SEL       0
#define ADVANCED_TIMER2_TIM_CH3_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER2_TIM_CH4_IOF_OVAL     24
#define ADVANCED_TIMER2_TIM_CH4_IOF_IVAL     24
#define ADVANCED_TIMER2_TIM_CH4_HS_SEL       0
#define ADVANCED_TIMER2_TIM_CH4_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER2_TIM_BKIN_0_IOF_OVAL     25
#define ADVANCED_TIMER2_TIM_BKIN_0_IOF_IVAL     25
#define ADVANCED_TIMER2_TIM_BKIN_0_HS_SEL       0
#define ADVANCED_TIMER2_TIM_BKIN_0_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER2_TIM_BKIN2_0_IOF_OVAL     26
#define ADVANCED_TIMER2_TIM_BKIN2_0_IOF_IVAL     26
#define ADVANCED_TIMER2_TIM_BKIN2_0_HS_SEL       0
#define ADVANCED_TIMER2_TIM_BKIN2_0_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER2_TIM_CH1N_IOF_OVAL     27
#define ADVANCED_TIMER2_TIM_CH1N_IOF_IVAL     27
#define ADVANCED_TIMER2_TIM_CH1N_HS_SEL       0
#define ADVANCED_TIMER2_TIM_CH1N_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER2_TIM_CH2N_IOF_OVAL     28
#define ADVANCED_TIMER2_TIM_CH2N_IOF_IVAL     28
#define ADVANCED_TIMER2_TIM_CH2N_HS_SEL       0
#define ADVANCED_TIMER2_TIM_CH2N_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER2_TIM_CH3N_IOF_OVAL     29
#define ADVANCED_TIMER2_TIM_CH3N_IOF_IVAL     29
#define ADVANCED_TIMER2_TIM_CH3N_HS_SEL       0
#define ADVANCED_TIMER2_TIM_CH3N_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER3_TIM_ETR_0_IOF_OVAL     30
#define ADVANCED_TIMER3_TIM_ETR_0_IOF_IVAL     30
#define ADVANCED_TIMER3_TIM_ETR_0_HS_SEL       0
#define ADVANCED_TIMER3_TIM_ETR_0_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER3_TIM_CH1_IOF_OVAL     31
#define ADVANCED_TIMER3_TIM_CH1_IOF_IVAL     31
#define ADVANCED_TIMER3_TIM_CH1_HS_SEL       0
#define ADVANCED_TIMER3_TIM_CH1_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER3_TIM_CH2_IOF_OVAL     32
#define ADVANCED_TIMER3_TIM_CH2_IOF_IVAL     32
#define ADVANCED_TIMER3_TIM_CH2_HS_SEL       0
#define ADVANCED_TIMER3_TIM_CH2_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER3_TIM_CH3_IOF_OVAL     33
#define ADVANCED_TIMER3_TIM_CH3_IOF_IVAL     33
#define ADVANCED_TIMER3_TIM_CH3_HS_SEL       0
#define ADVANCED_TIMER3_TIM_CH3_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER3_TIM_CH4_IOF_OVAL     34
#define ADVANCED_TIMER3_TIM_CH4_IOF_IVAL     34
#define ADVANCED_TIMER3_TIM_CH4_HS_SEL       0
#define ADVANCED_TIMER3_TIM_CH4_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER3_TIM_BKIN_0_IOF_OVAL     35
#define ADVANCED_TIMER3_TIM_BKIN_0_IOF_IVAL     35
#define ADVANCED_TIMER3_TIM_BKIN_0_HS_SEL       0
#define ADVANCED_TIMER3_TIM_BKIN_0_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER3_TIM_BKIN2_0_IOF_OVAL     36
#define ADVANCED_TIMER3_TIM_BKIN2_0_IOF_IVAL     36
#define ADVANCED_TIMER3_TIM_BKIN2_0_HS_SEL       0
#define ADVANCED_TIMER3_TIM_BKIN2_0_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER3_TIM_CH1N_IOF_OVAL     37
#define ADVANCED_TIMER3_TIM_CH1N_IOF_IVAL     37
#define ADVANCED_TIMER3_TIM_CH1N_HS_SEL       0
#define ADVANCED_TIMER3_TIM_CH1N_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER3_TIM_CH2N_IOF_OVAL     38
#define ADVANCED_TIMER3_TIM_CH2N_IOF_IVAL     38
#define ADVANCED_TIMER3_TIM_CH2N_HS_SEL       0
#define ADVANCED_TIMER3_TIM_CH2N_IOMUX_SEL    IOMUX
                
#define ADVANCED_TIMER3_TIM_CH3N_IOF_OVAL     39
#define ADVANCED_TIMER3_TIM_CH3N_IOF_IVAL     39
#define ADVANCED_TIMER3_TIM_CH3N_HS_SEL       0
#define ADVANCED_TIMER3_TIM_CH3N_IOMUX_SEL    IOMUX
                
#define LGPIO0_GPIO0_IOF_OVAL     40
#define LGPIO0_GPIO0_IOF_IVAL     40
#define LGPIO0_GPIO0_HS_SEL       0
#define LGPIO0_GPIO0_IOMUX_SEL    IOMUX
                
#define LGPIO0_GPIO1_IOF_OVAL     41
#define LGPIO0_GPIO1_IOF_IVAL     41
#define LGPIO0_GPIO1_HS_SEL       0
#define LGPIO0_GPIO1_IOMUX_SEL    IOMUX
                
#define LGPIO0_GPIO2_IOF_OVAL     42
#define LGPIO0_GPIO2_IOF_IVAL     42
#define LGPIO0_GPIO2_HS_SEL       0
#define LGPIO0_GPIO2_IOMUX_SEL    IOMUX
                
#define LGPIO0_GPIO3_IOF_OVAL     43
#define LGPIO0_GPIO3_IOF_IVAL     43
#define LGPIO0_GPIO3_HS_SEL       0
#define LGPIO0_GPIO3_IOMUX_SEL    IOMUX
                
#define LGPIO0_GPIO4_IOF_OVAL     44
#define LGPIO0_GPIO4_IOF_IVAL     44
#define LGPIO0_GPIO4_HS_SEL       0
#define LGPIO0_GPIO4_IOMUX_SEL    IOMUX
                
#define LGPIO0_GPIO5_IOF_OVAL     45
#define LGPIO0_GPIO5_IOF_IVAL     45
#define LGPIO0_GPIO5_HS_SEL       0
#define LGPIO0_GPIO5_IOMUX_SEL    IOMUX
                
#define LGPIO0_GPIO6_IOF_OVAL     46
#define LGPIO0_GPIO6_IOF_IVAL     46
#define LGPIO0_GPIO6_HS_SEL       0
#define LGPIO0_GPIO6_IOMUX_SEL    IOMUX
                
#define LGPIO0_GPIO7_IOF_OVAL     47
#define LGPIO0_GPIO7_IOF_IVAL     47
#define LGPIO0_GPIO7_HS_SEL       0
#define LGPIO0_GPIO7_IOMUX_SEL    IOMUX
                
#define LGPIO1_GPIO0_IOF_OVAL     48
#define LGPIO1_GPIO0_IOF_IVAL     48
#define LGPIO1_GPIO0_HS_SEL       0
#define LGPIO1_GPIO0_IOMUX_SEL    IOMUX
                
#define LGPIO1_GPIO1_IOF_OVAL     49
#define LGPIO1_GPIO1_IOF_IVAL     49
#define LGPIO1_GPIO1_HS_SEL       0
#define LGPIO1_GPIO1_IOMUX_SEL    IOMUX
                
#define LGPIO1_GPIO2_IOF_OVAL     50
#define LGPIO1_GPIO2_IOF_IVAL     50
#define LGPIO1_GPIO2_HS_SEL       0
#define LGPIO1_GPIO2_IOMUX_SEL    IOMUX
                
#define LGPIO1_GPIO3_IOF_OVAL     51
#define LGPIO1_GPIO3_IOF_IVAL     51
#define LGPIO1_GPIO3_HS_SEL       0
#define LGPIO1_GPIO3_IOMUX_SEL    IOMUX
                
#define LGPIO1_GPIO4_IOF_OVAL     52
#define LGPIO1_GPIO4_IOF_IVAL     52
#define LGPIO1_GPIO4_HS_SEL       0
#define LGPIO1_GPIO4_IOMUX_SEL    IOMUX
                
#define LGPIO1_GPIO5_IOF_OVAL     53
#define LGPIO1_GPIO5_IOF_IVAL     53
#define LGPIO1_GPIO5_HS_SEL       0
#define LGPIO1_GPIO5_IOMUX_SEL    IOMUX
                
#define LGPIO1_GPIO6_IOF_OVAL     54
#define LGPIO1_GPIO6_IOF_IVAL     54
#define LGPIO1_GPIO6_HS_SEL       0
#define LGPIO1_GPIO6_IOMUX_SEL    IOMUX
                
#define LGPIO1_GPIO7_IOF_OVAL     55
#define LGPIO1_GPIO7_IOF_IVAL     55
#define LGPIO1_GPIO7_HS_SEL       0
#define LGPIO1_GPIO7_IOMUX_SEL    IOMUX
                
#define LGPIO2_GPIO0_IOF_OVAL     56
#define LGPIO2_GPIO0_IOF_IVAL     56
#define LGPIO2_GPIO0_HS_SEL       0
#define LGPIO2_GPIO0_IOMUX_SEL    IOMUX
                
#define LGPIO2_GPIO1_IOF_OVAL     57
#define LGPIO2_GPIO1_IOF_IVAL     57
#define LGPIO2_GPIO1_HS_SEL       0
#define LGPIO2_GPIO1_IOMUX_SEL    IOMUX
                
#define LGPIO2_GPIO2_IOF_OVAL     58
#define LGPIO2_GPIO2_IOF_IVAL     58
#define LGPIO2_GPIO2_HS_SEL       0
#define LGPIO2_GPIO2_IOMUX_SEL    IOMUX
                
#define LGPIO2_GPIO3_IOF_OVAL     59
#define LGPIO2_GPIO3_IOF_IVAL     59
#define LGPIO2_GPIO3_HS_SEL       0
#define LGPIO2_GPIO3_IOMUX_SEL    IOMUX
                
#define LGPIO2_GPIO4_IOF_OVAL     60
#define LGPIO2_GPIO4_IOF_IVAL     60
#define LGPIO2_GPIO4_HS_SEL       0
#define LGPIO2_GPIO4_IOMUX_SEL    IOMUX
                
#define LGPIO2_GPIO5_IOF_OVAL     61
#define LGPIO2_GPIO5_IOF_IVAL     61
#define LGPIO2_GPIO5_HS_SEL       0
#define LGPIO2_GPIO5_IOMUX_SEL    IOMUX
                
#define LGPIO2_GPIO6_IOF_OVAL     62
#define LGPIO2_GPIO6_IOF_IVAL     62
#define LGPIO2_GPIO6_HS_SEL       0
#define LGPIO2_GPIO6_IOMUX_SEL    IOMUX
                
#define LGPIO2_GPIO7_IOF_OVAL     63
#define LGPIO2_GPIO7_IOF_IVAL     63
#define LGPIO2_GPIO7_HS_SEL       0
#define LGPIO2_GPIO7_IOMUX_SEL    IOMUX
                
#define LGPIO3_GPIO0_IOF_OVAL     64
#define LGPIO3_GPIO0_IOF_IVAL     64
#define LGPIO3_GPIO0_HS_SEL       0
#define LGPIO3_GPIO0_IOMUX_SEL    IOMUX
                
#define LGPIO3_GPIO1_IOF_OVAL     65
#define LGPIO3_GPIO1_IOF_IVAL     65
#define LGPIO3_GPIO1_HS_SEL       0
#define LGPIO3_GPIO1_IOMUX_SEL    IOMUX
                
#define LGPIO3_GPIO2_IOF_OVAL     66
#define LGPIO3_GPIO2_IOF_IVAL     66
#define LGPIO3_GPIO2_HS_SEL       0
#define LGPIO3_GPIO2_IOMUX_SEL    IOMUX
                
#define LGPIO3_GPIO3_IOF_OVAL     67
#define LGPIO3_GPIO3_IOF_IVAL     67
#define LGPIO3_GPIO3_HS_SEL       0
#define LGPIO3_GPIO3_IOMUX_SEL    IOMUX
                
#define LGPIO3_GPIO4_IOF_OVAL     68
#define LGPIO3_GPIO4_IOF_IVAL     68
#define LGPIO3_GPIO4_HS_SEL       0
#define LGPIO3_GPIO4_IOMUX_SEL    IOMUX
                
#define LGPIO3_GPIO5_IOF_OVAL     69
#define LGPIO3_GPIO5_IOF_IVAL     69
#define LGPIO3_GPIO5_HS_SEL       0
#define LGPIO3_GPIO5_IOMUX_SEL    IOMUX
                
#define LGPIO3_GPIO6_IOF_OVAL     70
#define LGPIO3_GPIO6_IOF_IVAL     70
#define LGPIO3_GPIO6_HS_SEL       0
#define LGPIO3_GPIO6_IOMUX_SEL    IOMUX
                
#define LGPIO3_GPIO7_IOF_OVAL     71
#define LGPIO3_GPIO7_IOF_IVAL     71
#define LGPIO3_GPIO7_HS_SEL       0
#define LGPIO3_GPIO7_IOMUX_SEL    IOMUX
                
#define XKAN1_XKAN_TX_IOF_OVAL     72
#define XKAN1_XKAN_TX_IOF_IVAL     72
#define XKAN1_XKAN_TX_HS_SEL       0
#define XKAN1_XKAN_TX_IOMUX_SEL    IOMUX
                
#define XKAN1_XKAN_RX_IOF_OVAL     73
#define XKAN1_XKAN_RX_IOF_IVAL     73
#define XKAN1_XKAN_RX_HS_SEL       0
#define XKAN1_XKAN_RX_IOMUX_SEL    IOMUX
            
uint8_t iomux_set_io_ival(unsigned long IO_MUX_BASE, uint32_t per_iof_id,uint32_t pad_id, uint8_t hs_ls);
uint8_t iomux_set_io_oval(unsigned long IO_MUX_BASE, uint32_t per_iof_id,uint32_t pad_id, uint8_t hs_ls);
uint32_t iomux_set_io_pull(unsigned long IO_MUX_BASE, uint32_t pad_id, enum iomux_pull_e pull);
void iomux_ls_iof_oval_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_num,uint32_t pad_num, uint8_t hs_ls,uint8_t  phy_cntr_sel,uint32_t phy_cntr);
void iomux_ls_iof_ival_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_num,uint32_t pad_num, uint8_t hs_ls,uint8_t  phy_cntr_sel,uint32_t phy_cntr);
uint8_t iomux_iof_oval_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_num,uint32_t pad_num, uint8_t hs_ls,uint8_t  pad_grp,uint8_t ls_per_grp);
uint8_t iomux_iof_ival_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_num,uint32_t pad_num, uint8_t hs_ls,uint8_t  pad_grp,uint8_t ls_per_grp);
void iomux_ls_iof_pullup_cfg(unsigned long IO_MUX_BASE, uint32_t pad_num, uint8_t hs_ls,uint8_t  phy_cntr_sel,uint32_t phy_cntr);
void iomux_ls_iof_pulldown_cfg(unsigned long IO_MUX_BASE, uint32_t pad_num, uint8_t hs_ls,uint8_t  phy_cntr_sel,uint32_t phy_cntr);
void iomux_ls_iof_loopback_cfg(unsigned long IO_MUX_BASE, uint32_t PER_iof_ival_num ,uint32_t PER_iof_oval_num ,uint32_t pad_num, uint8_t hs_ls ,uint8_t  phy_cntr_sel,uint32_t phy_cntr);
void iomux_pu_sel_assert(unsigned long IO_MUX_BASE, uint32_t pad_num);
void iomux_pd_sel_assert(unsigned long IO_MUX_BASE, uint32_t pad_num);
void iomux_pu_sel_deassert(unsigned long IO_MUX_BASE, uint32_t pad_num);
void iomux_pd_sel_deassert(unsigned long IO_MUX_BASE, uint32_t pad_num);
void iomux_pad_phy_ctrl(unsigned long IO_MUX_BASE, uint32_t pad_num ,uint32_t ctrl );
uint32_t iomux_get_io_pull(unsigned long IO_MUX_BASE, uint32_t pad_id);

#ifdef __cplusplus
}
#endif
#endif

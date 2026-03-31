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
#ifndef _NS_IOMUX_REG_H
#define _NS_IOMUX_REG_H

/*!
 * @file     ns_iomux.h
 * @brief    This file contains all the functions prototypes for the IOMUX firmware
 */
#ifdef __cplusplus
 extern "C" {
#endif

#include "ns.h"

#define   LS_SRC_SEL_OFS(grp_id)                    (0x0000+grp_id*262144)
#define   HS_CHNL_SEL_OFS(grp_id)                   (0x2000+grp_id*262144)
#define   LS_SRC0_IVAL_SEL_OFS(grp_id)              (0x4000+grp_id*262144)

#define   PHY_CNTRL_OFS(grp_id)                     (0x6000+grp_id*262144)
#define   CNTRL_SEL_OFS(grp_id)                     (0x08000+grp_id*262144)
#define   DBG_SRC_SEL_OFS(grp_id)                   (0x0C000+grp_id*262144)

#define   LS_IOF_IVAL_FILTER(grp_id)                (0x10000+grp_id*262144)
#define   PAD_SPW(grp_id)                           (0x12000+grp_id*262144)
#define   PAD_INT_EN(grp_id)                        (0x14000+grp_id*262144)
#define   PAD_INT_STATUS(grp_id)                    (0x16000+grp_id*262144)
#define   DBG_SRC_SEL(grp_id)                       DBG_SRC_SEL_OFS

#define   DI_VAL                BIT(0)
#define   DI_INV                BIT(1)
#define   IE_EN                 BIT(2)
#define   IE_INV                BIT(3)
#define   OE_EN                 BIT(4)
#define   OE_INV                BIT(5)
#define   DO_VAL                BIT(6)
#define   DO_INV                BIT(7)
#define   DS                    BITS(8,11)
#define   PD                    BIT(16)
#define   PU                    BIT(18)
#define   SPU                   BIT(19)
#define   ST                    BIT(20)
#define   SL                    BIT(21)
#define   ANALOG_EN             BIT(23)
#define   BUS_KEEP              BIT(24)

#define  IE_SEL                 BIT(0)
#define  PD_SEL                 BIT(1)
#define  PU_SEL                 BIT(2)

#define OE_SEL(regval)                                  (BITS(3,4) & ((uint32_t)(regval) << 3))
#define OE_SEL_PAD_PHYSICAL                             OE_SEL(0)              /*!<  PAD_PHYSICAL */
#define OE_SEL_HS_CHANNEL                               OE_SEL(1)              /*!<  OE_HS_CHANNEL */
#define OE_SEL_OVAL_HS_CHANNEL                          OE_SEL(2)              /*!<  OVAL_HS_CHANNEL */
#define DO_SEL_OFS                                      5UL
         
#define DO_SEL(regval)                                  (BITS(5,6) & ((uint32_t)(regval) << 5))
#define DO_SEL_PAD_PHYSICAL                             DO_SEL(0)              /*!<  DO_SEL_PHY   */
#define DO_SEL_HS                                       DO_SEL(1)              /*!<  DO_SEL_HS*/
#define DO_SEL_OE                                       DO_SEL(2)              /*!<  DO_SEL_OE */

#define BUS_KEEP_SEL    BIT(7)
#define OSCILLATOR_SEL    BIT(8)

enum iomux_pull_e{
    IOMUX_PULL_NONE,
    IOMUX_PULL_DOWN,
    IOMUX_PULL_UP,
    IOMUX_STRONG_PULL_UP
};

struct pad_physical_config_t {
    /* Read current IO pad's data input. */
	uint32_t di_val : 1;
	/*!< Invert the input signal from IO pad. */
	uint32_t di_inv : 1;
	/*!< Driving selector. */
	uint32_t ie_en : 1;
	/*!< Static output enable, will AND with OE_INV. */
	uint32_t ie_inv : 1;
	/*!< Invert output enable. */
	uint32_t oe_en : 1;
	/*!< Data output select: 0 for DO, 1 for OE. */
	uint32_t oe_inv : 1;
	/*!< Invert the result of data output select (DO_SEL). */
	uint32_t do_val : 1;
	/*!< Pull enable. */
	uint32_t do_inv : 1;
	/*!< Pull select: 0 for pull down, 1 for pull up. */
	uint32_t ds : 4;
	/*!< Strong pull up. */
	uint32_t Reserved : 4;
	/*!< Slew rate control enable. */
	uint32_t pd : 1;
	/*!< Static input enable, will AND with IE_INV. */
	uint32_t Reserved1 : 1;
	/*!< Invert input enable. */
	uint32_t pu : 1;
	/*!< Invert Data input. */
	uint32_t spu : 1;
	/*!< Schmitt trigger. */
	uint32_t st : 1;
	/*!< Input tie enable, 1 for enable, 0 for disable. */
	uint32_t sl : 1;
	/*!< Input tie value, 1 for high, 0 for low. */
	uint32_t Reserved2 : 1;
	/*!< Reserved bits. */
	uint32_t analog_en : 1;
	/*!< Reserved bits. */
	uint32_t sw_bus_keep : 1;
    /*!< Reserved bits. */
	uint32_t Reserved3 : 7;
	/*!< Read current PAD's data input. */
} __attribute__((packed, aligned(4)));

#ifdef __cplusplus
}
#endif
#endif

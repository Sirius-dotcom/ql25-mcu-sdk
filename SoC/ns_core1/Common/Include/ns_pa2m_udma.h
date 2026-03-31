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

#ifndef _NS_PA2M_UDMA_H_
#define _NS_PA2M_UDMA_H_
/*!
 * \file     ns_pa2m_udma.h
 * \brief    This file contains all the functions prototypes for the UDMA firmware
  */
#ifdef __cplusplus
 extern "C" {
#endif
#include "ns.h"

/* =====PA DMA MCTRL Register definition ===== */
#define PA2M_MCTRL_TRANS_EN_OFS                  (0UL)                                            /*!< DMA MCTRL : Transfer EN Bit Offset */
#define PA2M_MCTRL_TRANS_EN_VAL(regval)          (BIT(0) & ((uint32_t)(regval) << 0))             /*!< DMA MCTRL : Transfer EN Bit Value */
#define PA2M_TRANS_ENABLE                        PA2M_MCTRL_TRANS_EN_VAL(1)                       /*!< DMA Transfer Enable */
#define PA2M_TRANS_DISABLE                       PA2M_MCTRL_TRANS_EN_VAL(0)                       /*!< DMA Transfer Disable */

#define PA2M_MCTRL_TRANS_MODE_OFS                (6UL)                                            /*!< DMA MCTRL : Transfer Mode Bits Offset */
#define PA2M_MCTRL_TRANS_MODE_VAL(regval)        (BITS(6,7) & ((uint32_t)(regval) << 6))          /*!< DMA MCTRL : Transfer Mode Bits Mask */
#define PA2M_MODE_NORMAL                         PA2M_MCTRL_TRANS_MODE_VAL(0)                     /*!< DMA MCTRL : Single Mode Transfer */
#define PA2M_MODE_CONTINUOUS                     PA2M_MCTRL_TRANS_MODE_VAL(1)                     /*!< DMA MCTRL : Continuous Mode Transfer */
#define PA2M_MODE_REPEAT                         PA2M_MCTRL_TRANS_MODE_VAL(2) 

/**
 * \brief Check the PA DMA TRANSMODE bits parameters.
 * \param TRANSMODE TRANSMODE bits value to be checked.
 * \retval 0 This is not a valid TRANSMODE bits.
 * \retval 1 This is a valid TRANSMODE bits.
 */
#define IS_PA_DMA_TRANSMODE(TRANSMODE)           ( ((TRANSMODE) == PA2M_MODE_NORMAL)      ||                                                    ((TRANSMODE) == PA2M_MODE_CONTINUOUS)  ||                                                    ((TRANSMODE) == PA2M_MODE_REPEAT) )

#define PA2M_PER_CHANNEL_ALL
#define PA2M_PA_MCTRL_PER_SEL_OFS                (1UL)                                           /*!< DMA PA MCTRL : Periphral Selection Bits Offset */
#define PA2M_PA_MCTRL_PER_SEL_VAL(regval)        (BITS(24,31) & ((uint32_t)(regval) << 24))         /*!< DMA PA MCTRL : Periphral Selection Bits Value */
#define PA2M_PER_CHANNEL0                        PA2M_PA_MCTRL_PER_SEL_VAL(0)                     /*!< DMA PA MCTRL : Periphral Selection Channel 0 */
#define PA2M_PER_CHANNEL1                        PA2M_PA_MCTRL_PER_SEL_VAL(1)                     /*!< DMA PA MCTRL : Periphral Selection Channel 1 */
#define PA2M_PER_CHANNEL2                        PA2M_PA_MCTRL_PER_SEL_VAL(2)                     /*!< DMA PA MCTRL : Periphral Selection Channel 2 */
#define PA2M_PER_CHANNEL3                        PA2M_PA_MCTRL_PER_SEL_VAL(3)                     /*!< DMA PA MCTRL : Periphral Selection Channel 3 */
#define PA2M_PER_CHANNEL4                        PA2M_PA_MCTRL_PER_SEL_VAL(4)                     /*!< DMA PA MCTRL : Periphral Selection Channel 4 */
#define PA2M_PER_CHANNEL5                        PA2M_PA_MCTRL_PER_SEL_VAL(5)                     /*!< DMA PA MCTRL : Periphral Selection Channel 5 */
#define PA2M_PER_CHANNEL6                        PA2M_PA_MCTRL_PER_SEL_VAL(6)                     /*!< DMA PA MCTRL : Periphral Selection Channel 6 */
#define PA2M_PER_CHANNEL7                        PA2M_PA_MCTRL_PER_SEL_VAL(7)                     /*!< DMA PA MCTRL : Periphral Selection Channel 7 */
#define PA2M_PER_CHANNEL(n)                      PA2M_PA_MCTRL_PER_SEL_VAL(n)
/**
 * \brief Check the PA DMA TRANS_PER_SEL bits parameters.
 * \param TRANS_PER_SEL TRANS_PER_SEL bits value to be checked.
 * \retval 0 This is not a valid TRANS_PER_SEL bits.
 * \retval 1 This is a valid TRANS_PER_SEL bits.
 */
#define IS_PA_DMA_TRANS_PER(TRANS_PER_SEL)       ( ((TRANS_PER_SEL) == PA2M_PER_CHANNEL0)   ||                                                    ((TRANS_PER_SEL) == PA2M_PER_CHANNEL1)   ||                                                    ((TRANS_PER_SEL) == PA2M_PER_CHANNEL2)   ||                                                    ((TRANS_PER_SEL) == PA2M_PER_CHANNEL3)   ||                                                    ((TRANS_PER_SEL) == PA2M_PER_CHANNEL4)   ||                                                    ((TRANS_PER_SEL) == PA2M_PER_CHANNEL5)   ||                                                    ((TRANS_PER_SEL) == PA2M_PER_CHANNEL6)   ||                                                    ((TRANS_PER_SEL) == PA2M_PER_CHANNEL7)  )

/**
 * \brief Check the DMA MDNA bit parameters.
 * \param MDNA MDNA bit value to be checked.
 * \retval 0 This is not a valid MDNA bit.
 * \retval 1 This is a valid MDNA bit.
 */
#define PA2M_MCTRL_MDNA_OFS                      (12UL)                                          /*!< DMA MCTRL : MDNA Bit Offset */
#define PA2M_MDNA_ENABLE                         (0UL)                                           /*!< DMA MCTRL : MDNA Increasing Address Mode */
#define PA2M_MDNA_DISABLE                        BIT(12)                                         /*!< DMA MCTRL : MDNA Fixed Address Mode */

/**
 * \brief Check the PA DMA MDNA bit parameters.
 * \param MDNA MDNA bit value to be checked.
 * \retval 0 This is not a valid MDNA bit.
 * \retval 1 This is a valid MDNA bit.
 */
#define IS_PA_DMA_MDNA(MDNA)                     ( ((MDNA) == PA2M_MDNA_ENABLE)   ||                                                    ((MDNA) == PA2M_MDNA_DISABLE) )

/**
 * \brief Check the DMA MSNA bit parameters.
 * \param MSNA MSNA bit value to be checked.
 * \retval 0 This is not a valid MSNA bit.
 * \retval 1 This is a valid MSNA bit.
 */
#define PA2M_MCTRL_MSNA_OFS                      (13UL)                                          /*!< DMA MCTRL : MSNA Bit Offset */
#define PA2M_MSNA_ENABLE                         (0UL)                                           /*!< DMA MCTRL : MSNA Increasing Address Mode */
#define PA2M_MSNA_DISABLE                         BIT(13)                                         /*!< DMA MCTRL : MSNA Fixed Address Mode */

/**
 * \brief Check the PA DMA MSNA bit parameters.
 * \param MSNA MSNA bit value to be checked.
 * \retval 0 This is not a valid MSNA bit.
 * \retval 1 This is a valid MSNA bit.
 */
#define IS_PA_DMA_MSNA(MSNA)                      ( ((MSNA) == PA2M_MSNA_ENABLE)   ||                                                     ((MSNA) == PA2M_MSNA_DISABLE) )

#define PA2M_MCTRL_MDWIDTH_OFS                   (16UL)                                           /*!< DMA MCTRL : MDWIDTH Bits Offset */
#define PA2M_MCTRL_MDWIDTH(regval)               (BITS(16,18) & ((uint32_t)(regval) << 16))       /*!< DMA MCTRL : MDWIDTH Bits Value */
#define PA2M_MDWIDTH_8BIT                        PA2M_MCTRL_MDWIDTH(0)                            /*!< DMA MCTRL : MDWIDTH 8 Bit */
#define PA2M_MDWIDTH_16BIT                       PA2M_MCTRL_MDWIDTH(1)                            /*!< DMA MCTRL : MDWIDTH 16 Bit */
#define PA2M_MDWIDTH_32BIT                       PA2M_MCTRL_MDWIDTH(2)                            /*!< DMA MCTRL : MDWIDTH 32 Bit */

/**
 * \brief Check the PA DMA MDWIDTH bits parameters.
 * \param MDWIDTH MDWIDTH bits value to be checked.
 * \retval 0 This is not a valid MDWIDTH bits.
 * \retval 1 This is a valid MDWIDTH bits.
 */
#define IS_PA_DMA_MDWIDTH(MDWIDTH)               ( ((MDWIDTH) == PA2M_MDWIDTH_8BIT)  ||                                                    ((MDWIDTH) == PA2M_MDWIDTH_16BIT) ||                                                    ((MDWIDTH) == PA2M_MDWIDTH_32BIT))

/* =====PA DMA IRQ_ENABLE Register definition ===== */
#define PA2M_FTRANS_IRQ_EN                       BIT(0)                                           /*!< PA_DMA IRQ_ENABLE : interrupt enable for Channel0 full transfer */
#define PA2M_HTRANS_IRQ_EN                       BIT(1)                                           /*!< PA_DMA IRQ_ENABLE : interrupt enable for Channel0 half transfer*/
#define PA2M_RSP_ERR_IRQ_EN                      BIT(2)                                           /*!< PA_DMA IRQ_ENABLE : interrupt enable for Channel0 DMA access error */

/**
 * \brief Check the PA UDMA IE bit parameters.
 * \param IE IE bit value to be checked.
 * \retval 0 This is not a valid IE bit.
 * \retval 1 This is a valid IE bit.
 */
#define IS_PA_UDMA_CONFIG_IE(IE)                  ( ((IE) == PA2M_FTRANS_IRQ_EN)   ||                                                     ((IE) == PA2M_HTRANS_IRQ_EN)   ||                                                     ((IE) == PA2M_RSP_ERR_IRQ_EN) )

/* =====PA DMA IRQ_FLAG_STATUS Register definition ===== */
#define PA2M_FTRANS_IRQ_STAT                     BIT(0)                                           /*!< PA_DMA IRQ_FLAG_STATUS : interrupt irq status flag for Channel0 full transfer */
#define PA2M_HTRANS_IRQ_STAT                     BIT(1)                                           /*!< PA_DMA IRQ_FLAG_STATUS : interrupt irq status flag for Channel0 full transfer */
#define PA2M_RSP_ERR_IRQ_STAT                    BIT(2)                                           /*!< PA_DMA IRQ_FLAG_STATUS : interrupt irq status flag for Channel0 DMA access error */

/**
 * \brief Check the PA UDMA IF bit parameters.
 * \param IF IF bit value to be checked.
 * \retval 0 This is not a valid IF bit.
 * \retval 1 This is a valid IF bit.
 */
#define IS_PA_UDMA_GET_IT(IF)                     ( ((IF) == PA2M_FTRANS_IRQ_STAT)   ||                                                     ((IF) == PA2M_HTRANS_IRQ_STAT)   ||                                                     ((IF) == PA2M_RSP_ERR_IRQ_STAT) )

/* =====PA DMA IRQ_CLEAR_STATUS Register definition ===== */
#define PA2M_FTRANS_IRQ_CLEAR_STAT                     BIT(0)                                     /*!< PA_DMA IRQ_CLEAR_STATUS : clear irq status flag for Channel0 full transfer */
#define PA2M_HTRANS_IRQ_CLEAR_STAT                     BIT(1)                                     /*!< PA_DMA IRQ_CLEAR_STATUS : clear irq status flag for Channel0 half transfer */
#define PA2M_RSP_ERR_IRQ_CLEAR_STAT                    BIT(2)                                     /*!< PA_DMA IRQ_CLEAR_STATUS : clear irq status flag for Channel0 DMA access error */

/**
 * \brief Check the PA UDMA Clear IT bit parameters.
 * \param IT Clear IT bit value to be checked.
 * \retval 0 This is not a valid Clear IT bit.
 * \retval 1 This is a valid Clear IT bit.
 */
#define IS_PA_UDMA_CLR_IT(IT)                     ( ((IT) == PA2M_FTRANS_IRQ_CLEAR_STAT)   ||                                                     ((IT) == PA2M_HTRANS_IRQ_CLEAR_STAT)   ||                                                     ((IT) == PA2M_RSP_ERR_IRQ_CLEAR_STAT) )

/**
  * \brief  DMA PA CHx Config Register structure definition
  */
typedef struct {
    uint32_t MSRCADDR;              /*!< PA CHx Source Data Base Address */
    uint32_t MDSTADDR;              /*!< PA CHx Destination Data Base Address */
    uint32_t MCTRL;                 /*!< PA CHx M2M Transfer Type Config */
    uint32_t MSIZE;                 /*!< PA CHx M2M Transfer Size */
    uint32_t MSRCADDR_H;            /*!< PA CHx Source Data Base Address High Bits */
    uint32_t MDSTADDR_H;            /*!< PA CHx Source Data Base Address High Bits */

} UDMA_PA_CHxCfg_TypeDef;

/**
  * \brief  DMA CHx IRQ Register structure definition
  */
typedef struct {
    uint32_t CHX_IRQ_EN;             /*!< M2M IRQ Enable */
    uint32_t CHX_IRQ_STAT;           /*!< M2M IRQ Flag */
    uint32_t CHX_IRQ_CLR;            /*!< M2M IRQ Clear */
} UDMA_PA_CHx_IRQ_TypeDef;

/**
  * \brief  DMA P2M Init Structure definition
  */
typedef struct {
    uint32_t UDMA_TransEn;               /*!< Specifies enable/disable DMAy CHannelx */
    uint32_t UDMA_DstBaseAddr;           /*!< Specifies the peripheral base address for DMAy CHannelx */
    uint32_t UDMA_SrcBaseAddr;           /*!< Specifies the memory base address for DMAy CHannelx */
    uint32_t UDMA_PER_SEL;               /*!< Specifies the peripheral selection for DMAy CHannelx */
    uint32_t UDMA_BufferSize;            /*!< Specifies the buffer size, in data unit, of the specified CHannel */
    uint32_t UDMA_DstInc;                /*!< Specifies whether the Peripheral address register is incremented or not */
    uint32_t UDMA_SrcInc;                /*!< Specifies whether the memory address register is incremented or not */
    uint32_t UDMA_Width;                 /*!< Specifies the data width */
    uint32_t UDMA_Mode;                  /*!< Specifies the operation mode of the DMAy CHannelx */
	  uint32_t UDMA_DstBaseAddr_H;         /*!< Specifies the source base high address for DMAy CHannelx */
    uint32_t UDMA_SrcBaseAddr_H;          /*!< Specifies the dest base high address for DMAy CHannelx */
} UDMA_PAM2MTypeDef;

void UDMA_PAM2M_Init(UDMA_PA_CHxCfg_TypeDef* UDMAy_CHannelx, UDMA_PAM2MTypeDef* UDMA_PAM2MStruct);
void UDMA_PA2M_StructInit(UDMA_PAM2MTypeDef* UDMA_PAM2MStruct);
void UDMA_Cmd(UDMA_PA_CHxCfg_TypeDef* UDMAy_CHannelx, FunctionalState NewState);
FlagStatus UDMA_PA2M_GetITStatus(UDMA_PA_CHx_IRQ_TypeDef * UDMAChannelx, uint32_t UDMA_IT);
void UDMA_PA2M_ClearITStatus(UDMA_PA_CHx_IRQ_TypeDef* UDMAChannelx, uint32_t UDMA_IT);
void UDMA_PA2M_ITConfig(UDMA_PA_CHx_IRQ_TypeDef* UDMAChannelx, uint32_t UDMA_IE, ControlStatus Status);
#ifdef __cplusplus
}
#endif

#endif

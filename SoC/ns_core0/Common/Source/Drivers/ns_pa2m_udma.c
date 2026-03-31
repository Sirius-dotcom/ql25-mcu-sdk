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

#include "ns_pa2m_udma.h"

#include "ns_conf.h"
/*!
 * \file     ns_pa2m_udma.c
 * \brief    This file contains all the functions prototypes for the PA UDMA firmware
 */

/**
  * \brief  Initializes the UDMA PA memory to memory.
  *
  * \param  UDMAy_CHannelx the configuration of the UDMA PA memory to memory.
  * \param  UDMA_PAM2MStruct the init structure of the UDMA PA memory to memory.
  */
void UDMA_PAM2M_Init(UDMA_PA_CHxCfg_TypeDef* UDMAy_CHannelx, UDMA_PAM2MTypeDef* UDMA_PAM2MStruct)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_PA_DMA_TRANSMODE(UDMA_PAM2MStruct->UDMA_Mode));
    assert_param(IS_PA_DMA_MDWIDTH(UDMA_PAM2MStruct->UDMA_Width));
    assert_param(IS_PA_DMA_MDNA(UDMA_PAM2MStruct->UDMA_DstInc));
    assert_param(IS_PA_DMA_MSNA(UDMA_PAM2MStruct->UDMA_SrcInc));

    /*--------------------------- DMAy CHannelx Configuration -----------------*/
    /* Configure DMAy CHannelx: data transfer, data size, priority level and mode */
    tmpreg |= UDMA_PAM2MStruct->UDMA_Mode | UDMA_PAM2MStruct->UDMA_Width |
            UDMA_PAM2MStruct->UDMA_DstInc | UDMA_PAM2MStruct->UDMA_SrcInc |
            UDMA_PAM2MStruct->UDMA_PER_SEL | UDMA_PAM2MStruct->UDMA_TransEn;

    /*--------------------------- DMAy CHannelx Configuration ---------------*/
    /* Write to DMAy CHannelx  */
    UDMAy_CHannelx->MSIZE = UDMA_PAM2MStruct->UDMA_BufferSize;

    /*--------------------------- DMAy CHannelx  Configuration ----------------*/
    /* Write to DMAy CHannelx */
    UDMAy_CHannelx->MDSTADDR = CAL_ADDR(UDMA_PAM2MStruct->UDMA_DstBaseAddr);
    UDMAy_CHannelx->MSRCADDR = CAL_ADDR(UDMA_PAM2MStruct->UDMA_SrcBaseAddr);

    /*--------------------------- DMAy CHannelx  Configuration ----------------*/
    /* Write to DMAy CHannelx */
    UDMAy_CHannelx->MDSTADDR_H = UDMA_PAM2MStruct->UDMA_DstBaseAddr_H;

    /*--------------------------- DMAy CHannelx  Configuration ----------------*/
    /* Write to DMAy CHannelx */
    UDMAy_CHannelx->MSRCADDR_H = UDMA_PAM2MStruct->UDMA_SrcBaseAddr_H;

    /* Write to DMAy CHannelx */
    UDMAy_CHannelx->MCTRL = tmpreg;
}

/**
  * \brief  Fills each UDMA_PAM2MStruct member with its default value.
  *
  * \param  UDMA_PAM2MStruct: pointer to a UDMA_PAM2MStruct structure which will be initialized.
  */
void UDMA_PA2M_StructInit(UDMA_PAM2MTypeDef* UDMA_PAM2MStruct)
{
    /*-------------- Reset DMA init structure parameters values ------------------*/
    /* Initialize the UDMA_SrcBaseAddr member */
    UDMA_PAM2MStruct->UDMA_SrcBaseAddr = 0;
    UDMA_PAM2MStruct->UDMA_SrcBaseAddr_H = 0;
    /* Initialize the UDMA_DestBaseAddr member */
    UDMA_PAM2MStruct->UDMA_DstBaseAddr = 0;
    UDMA_PAM2MStruct->UDMA_DstBaseAddr_H = 0;
    /* Initialize the UDMA_BufferSize member */
    UDMA_PAM2MStruct->UDMA_Width = 0;
    /* Initialize the UDMA_Mode member */
    UDMA_PAM2MStruct->UDMA_Mode = 0;
    /* Initialize the UDMA_PeripheralInc mode */
    UDMA_PAM2MStruct->UDMA_DstInc = 0;
    /* Initialize the UDMA_MemoryInc member */
    UDMA_PAM2MStruct->UDMA_SrcInc = 0;
    /* Initialize the UDMA_PeripheralSel*/
    UDMA_PAM2MStruct->UDMA_PER_SEL = 0;
    /* Initialize the UDMA_Transmit Enable*/
    UDMA_PAM2MStruct->UDMA_TransEn = 0;
    /* Initialize the UDMA_PeripheralDataSize member */
    UDMA_PAM2MStruct->UDMA_BufferSize = 0;
}

/**
  * @brief  Enables or disables the specified DMAy Streamx.
  *
  * @param  UDMAy_CHannelx where y can be 1 or 2 to select the DMA and x can be 0
  *         to 7 to select the DMA Stream.
  * @param  NewState new state of the DMAy Streamx. 
  *          This parameter can be: ENABLE or DISABLE.
  *
  * @note  DMA transfer enable, asserting the bit will enable a PA mem2mem DMA 
  *        transfer, it can be cleared by SW and the transfer will  stop after  
  *        current transfer is finished.
  *                  
  * @note  After configuring the DMA Stream (DMA_Init() function) and enabling the
  *        stream, it is recommended to check (or wait until) the DMA Stream is
  *        effectively enabled. A Stream may remain disabled if a configuration 
  *        parameter is wrong.
  *        After disabling a DMA Stream, it is also recommended to check (or wait
  *        until) the DMA Stream is effectively disabled. If a Stream is disabled 
  *        while a data transfer is ongoing, the current data will be transferred
  *        and the Stream will be effectively disabled only after the transfer of
  *        this single data is finished.            
  *    
  * @retval None
  */
void UDMA_Cmd(UDMA_PA_CHxCfg_TypeDef* UDMAy_CHannelx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* Enable the selected DMAy Streamx by setting EN bit */
        UDMAy_CHannelx->MCTRL |= (uint32_t)PA2M_TRANS_ENABLE;
    } else{
        /* Disable the selected DMAy Streamx by clearing EN bit */
        UDMAy_CHannelx->MCTRL &= ~(uint32_t)PA2M_TRANS_ENABLE;
    }
}

/**
  * \brief  Checks whether the specified PA2M DMAy CHannelx interrupt has occurred or not.
  * \param  UDMAChannelx the PA2M UDMA channel interrupt.
  *   This parameter can be one of the following values:
  *     \arg DMA_CH0_PA2M_IRQ
  *     \arg DMA_CH1_PA2M_IRQ
  *     \arg DMA_CH2_PA2M_IRQ
  *     \arg DMA_CH3_PA2M_IRQ
  *     \arg DMA_CH4_PA2M_IRQ
  *     \arg DMA_CH5_PA2M_IRQ
  *     \arg DMA_CH6_PA2M_IRQ
  *     \arg DMA_CH7_PA2M_IRQ
  * \param  UDMA_IT specifies the UDMA interrupt source to check.
  *   This parameter can be one of the following values:
  *     \arg PA2M_FTRANS_IRQ_STAT
  *     \arg PA2M_HTRANS_IRQ_STAT
  *     \arg PA2M_RSP_ERR_IRQ_STAT
  * \retval SET the specified DMA PA2M Interrupt status is SET.
  * \retval RESET the specified DMA PA2M Interrupt status is RESET.
  */
FlagStatus UDMA_PA2M_GetITStatus(UDMA_PA_CHx_IRQ_TypeDef * UDMAChannelx, uint32_t UDMA_IT)
{
    /* Check the parameters */
    assert_param(IS_PA_UDMA_GET_IT(UDMA_IT));

    FlagStatus bitstatus = RESET;
    if (UDMAChannelx->CHX_IRQ_STAT & UDMA_IT) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

/**
  * \brief  Clears the specified M2M UDMA interrupt source.
  * \param  UDMAChannelx the M2M UDMA channel interrupt.
  *   This parameter can be one of the following values:
  *     \arg DMA_CH0_PA2M_IRQ
  *     \arg DMA_CH1_PA2M_IRQ
  *     \arg DMA_CH2_PA2M_IRQ
  *     \arg DMA_CH3_PA2M_IRQ
  *     \arg DMA_CH4_PA2M_IRQ
  *     \arg DMA_CH5_PA2M_IRQ
  *     \arg DMA_CH6_PA2M_IRQ
  *     \arg DMA_CH7_PA2M_IRQ
  * \param  UDMA_IT specifies the UDMA interrupt source to check.
  *   This parameter can be one of the following values:
  *     \arg PA2M_FTRANS_IRQ_CLEAR_STAT
  *     \arg PA2M_HTRANS_IRQ_CLEAR_STAT
  *     \arg PA2M_RSP_ERR_IRQ_CLEAR_STAT
  */
void UDMA_PA2M_ClearITStatus(UDMA_PA_CHx_IRQ_TypeDef* UDMAChannelx, uint32_t UDMA_IT)
{
    /* Check the parameters */
    assert_param(IS_PA_UDMA_CLR_IT(UDMA_IT));

    UDMAChannelx->CHX_IRQ_CLR |= UDMA_IT;
}

/**
  * \brief  Enables or disables the specified M2M UDMA interrupts.
  * \param  UDMAChannelx the M2M UDMA channel interrupt.
  *   This parameter can be one of the following values:
  *     \arg DMA_CH0_PA2M_IRQ
  *     \arg DMA_CH1_PA2M_IRQ
  *     \arg DMA_CH2_PA2M_IRQ
  *     \arg DMA_CH3_PA2M_IRQ
  *     \arg DMA_CH4_PA2M_IRQ
  *     \arg DMA_CH5_PA2M_IRQ
  *     \arg DMA_CH6_PA2M_IRQ
  *     \arg DMA_CH7_PA2M_IRQ
  * \param  UDMA_IE specifies the M2M UDMA interrupt sources to be enabled or disabled.
  *   This parameter can be one of the following values:
  *     \arg PA2M_FTRANS_IRQ_EN
  *     \arg PA2M_HTRANS_IRQ_EN
  *     \arg PA2M_RSP_ERR_IRQ_EN
  * \param  Status Status of the specified UDMA interrupts.
  *                 This parameter can be: ENABLE or DISABLE.
  */
void UDMA_PA2M_ITConfig(UDMA_PA_CHx_IRQ_TypeDef* UDMAChannelx, uint32_t UDMA_IE, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_UDMA_CONFIG_IE(UDMA_IE));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        UDMAChannelx->CHX_IRQ_EN |= UDMA_IE;
    } else {
        UDMAChannelx->CHX_IRQ_EN &= ~UDMA_IE;
    }
}

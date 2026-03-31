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
#include "ns.h"
#include "ns_usart.h"
#include "ns_conf.h"

/*****************************************************************************************************************************/

/*!
 * \file     ns_usart.c
 * \brief    This file contains all the functions prototypes for the USART firmware
  */

/**
  * \brief  Initializes the USARTx peripheral according to the specified
  *         parameters in the USART_InitStruct.
  * \param  USARTx Select the USART or the UART peripheral.
  *         This parameter can be one of the following values:
  *         USART0, USART1.
  * \param  USART_InitStruct pointer to a USART_InitTypeDef structure
  *         that contains the configuration information for the specified USART periphera.
  */
void USART_Init(USART_TypeDef *USARTx, USART_InitTypeDef* USART_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_UART_TXCTRL_CFG_STOP_BIT(USART_InitStruct->USART_StopBits));
    assert_param(IS_USART_UART_TXCTRL_TXEN(USART_InitStruct->USART_TX_CTL));
    assert_param(IS_USART_UART_RXCTRL_RXEN(USART_InitStruct->USART_RX_CTL));
    assert_param(IS_USART_UART_SETUP_BIT_LENGTH(USART_InitStruct->USART_WordLength));
    assert_param(IS_USART_UART_SETUP_PARITY_EN(USART_InitStruct->USART_Parity));
    uint32_t version = USARTx->IP_VERSION;
    uint32_t reg = 0U;
#ifdef USE_USART_V3_0_0
    if(USART_InitStruct->USART_Fra_Baud)  {
        USARTx->DIV = (USART_InitStruct->USART_CLK_Feq << 3) / USART_InitStruct->USART_BaudRate - 1;
    } 
    else {
        USARTx->DIV = USART_InitStruct->USART_CLK_Feq / USART_InitStruct->USART_BaudRate - 1;
    }
#else
    if(USART_InitStruct->USART_Fra_Baud)  {
        USARTx->DIV = (SystemCoreClock << 3) / USART_InitStruct->USART_BaudRate - 1;
    } 
    else {
        USARTx->DIV = SystemCoreClock / USART_InitStruct->USART_BaudRate - 1;
    }
#endif

    /* Configure USARTx TXCTRL */
    reg = USARTx->TXCTRL;
    reg &= ~(USART_TXCTRL_TXEN );
    reg |= (uint32_t)(USART_InitStruct->USART_TX_CTL );
    USARTx->TXCTRL = reg;

    /* Configure USARTx RXCTRL */
    reg = 0;
    reg = USARTx->RXCTRL;
    reg &= ~USART_RXCTRL_RXEN;
    reg |= (uint32_t)(USART_InitStruct->USART_RX_CTL);
    USARTx->RXCTRL = reg;

    /* Configure USARTx ADV_SETUP */

    reg = 0;
    reg = USARTx->ADVANCED_SETUP;
    reg &= ~(USART_ADVANCED_SETUP_ABR_EN | USART_ADVANCED_SETUP_ABR_MOD_MASK);
    reg |= (uint32_t)(USART_InitStruct->AutoBaudRateEnable | USART_InitStruct->AutoBaudRateMode);
    USARTx->ADVANCED_SETUP = reg;

    /* Configure USARTx SETUP */
    reg = 0;
    reg = USARTx->SETUP;
    reg &= ~(USART_SETUP_BIT_LENGTH_MASK | USART_SETUP_PARITY_EN | USART_SETUP_PARITY_SEL | USART_SETUP_RTSE | USART_SETUP_CTSE |USART_SETUP_FRACTION_BAUD| USART_SETUP_CFG_STOP_BIT_MASK);
    if(USART_InitStruct->USART_Fra_Baud) {
        reg |= USART_SETUP_FRACTION_BAUD;   
    }else{
        reg &= (~USART_SETUP_FRACTION_BAUD);       
    }
    reg |= USART_SETUP_SEPARATE_EN_VAL(1);
    reg |= (uint32_t)(USART_InitStruct->USART_SC_EN |USART_InitStruct->USART_WordLength | USART_InitStruct->USART_Parity | USART_InitStruct->USART_HardwareFlowControl |            USART_InitStruct->USART_DMA_EN | USART_InitStruct->USART_RX_DMA_EN | USART_InitStruct->USART_SEPARATE_EN| USART_InitStruct->USART_StopBits);
    
    USARTx->RX_IDLE = USART_InitStruct->USART_RX_IDLE_Timeout;
    USARTx->RX_WM = USART_InitStruct->USART_RX_WM_Timeout;
    USARTx->TX_DATASIZE = USART_InitStruct->USART_TX_Data_Size;
    USARTx->RX_DATASIZE = USART_InitStruct->USART_RX_Data_Size;
    USARTx->SETUP = reg;
}

/**
  * \brief  Fills each USART_InitStruct member with its default value.
  * \param  USART_InitStruct pointer to a USART_InitTypeDef structure
  *         which will be initialized.
  */
void USART_StructInit(USART_InitTypeDef* USART_InitStruct)
{
    /* USART_InitStruct members default value */
    USART_InitStruct->USART_BaudRate = 115200;
    USART_InitStruct->USART_WordLength = USART_SETUP_BIT_LENGTH_8;
    USART_InitStruct->USART_StopBits = USART_SETUP_CFG_STOP_BIT_1BIT;
    USART_InitStruct->USART_TX_CTL = USART_TXCTRL_TXEN_ENABLE;
    USART_InitStruct->USART_RX_CTL = USART_RXCTRL_RXEN_ENABLE;
    USART_InitStruct->USART_Parity = USART_SETUP_PARITY_EN_DISABLE;
    USART_InitStruct->USART_HardwareFlowControl = USART_SETUP_RTSE_DISABLE | USART_SETUP_CTSE_DISABLE;
    USART_InitStruct->USART_RX_IDLE_Timeout = 0xFF;
    USART_InitStruct->USART_RX_WM_Timeout = 0xFF;
    USART_InitStruct->USART_TX_Data_Size = USART_TX_DATASIZE_NUM_BYTE;
    USART_InitStruct->USART_RX_Data_Size = USART_RX_DATASIZE_NUM_BYTE;
    USART_InitStruct->USART_Fra_Baud = 1;
#ifdef USE_USART_V3_0_0
    USART_InitStruct->USART_CLK_Feq = 32000000;
#endif
    USART_InitStruct->USART_DMA_EN = 0;
    USART_InitStruct->USART_RX_DMA_EN = 0 ;
    USART_InitStruct->USART_SEPARATE_EN = 0;
}

/**
  * \brief  Initializes the USARTx peripheral Clock according to the
  *         vspecified parameters in the USART_ClockInitStruct.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_ClockInitStruct pointer to a USART_ClockInitTypeDef
  *         structure that contains the configuration information for the specified
  *         USART peripheral.
  */
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct)
{
    uint32_t reg = 0;

    /* Check the parameters */
    assert_param(IS_USART_UART_SETUP_CLK_EN(USART_ClockInitStruct->USART_Clock));
    assert_param(IS_USART_UART_SETUP_CPOL(USART_ClockInitStruct->USART_CPOL));
    assert_param(IS_USART_UART_SETUP_CPHA(USART_ClockInitStruct->USART_CPHA));
    assert_param(IS_USART_UART_SETUP_LBCL(USART_ClockInitStruct->USART_LastBit));

    /* Configure USARTx CLOCK*/
    reg = USARTx->SETUP;
    reg &= ~(USART_SETUP_CLK_EN | USART_SETUP_CPOL | USART_SETUP_CPHA | USART_SETUP_LBCL);
    reg |= (uint32_t)(USART_ClockInitStruct->USART_Clock | USART_ClockInitStruct->USART_CPOL |     USART_ClockInitStruct->USART_CPHA | USART_ClockInitStruct->USART_LastBit);
    USARTx->SETUP = reg;
}

/**
  * \brief  
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_485InitTypeDef
  */
void USART_485Init(USART_TypeDef* USARTx, USART_485InitTypeDef* USART_485InitStruct)
{
    uint32_t reg = 0;
    /* Configure USARTx CLOCK*/
    reg = USARTx->SETUP;
    reg &= ~(USART_SETUP_RS485_EN | USART_SETUP_DE_ASSERTION_EARLY_EN | USART_SETUP_DE_DEASSERTION_DELAY_EN | USART_SETUP_DE_INV);
    reg |= (uint32_t)(USART_485InitStruct->USART_485EN | USART_485InitStruct->USART_485Start_Early |     USART_485InitStruct->USART_485Stop_Delay | USART_485InitStruct->USART_485DE);
    USARTx->SETUP = reg;
}
/**
  * \brief  Enables or disables the USART clock pin output.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  Status Status of the DMA Request sources.
  *                 This parameter can be: ENABLE or DISABLE.
  */
void USART_SynClockConfig(USART_TypeDef *USARTx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        USARTx->SETUP |= USART_SETUP_CLK_EN_ENABLE;
    } else {
        USARTx->SETUP &= (~USART_SETUP_CLK_EN_ENABLE);
    }
}

/**
  * \brief  Enables or disables the USART simplex mode.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  Status Status of the USART simplex mode.
  *                 This parameter can be: ENABLE or DISABLE.
  */
void USART_SimplexConfig(USART_TypeDef *USARTx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        USARTx->SETUP |= USART_SETUP_HDSEL;
    } else {
        USARTx->SETUP &= ~USART_SETUP_HDSEL;
    }
}

/**
  * \brief  Checks whether the specified USART interrupt has occurred or not.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_IT specifies the USART interrupt source to check.
  * \retval SET the specified USART interrupt is SET.
  * \retval RESET the specified USART interrupt is RESET.
  */
FlagStatus USART_GetITStatus(USART_TypeDef *USARTx, uint32_t USART_IT)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    if (USARTx->STATUS & USART_IT) {
        return SET;
    } else {
        return RESET;
    }
}
/**
  * \brief  Checks whether the specified USART interrupt has occurred or not.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_ABR Auto baud rate error,when auto baud rate failure.
  * \retval SET the specified USART interrupt is SET.
  * \retval RESET the specified USART interrupt is RESET.
  */
FlagStatus USART_GetABRStatus(USART_TypeDef *USARTx, uint32_t USART_ABR) 
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    if (USARTx->ADVANCED_STATUS & USART_ABR) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief  Checks whether the specified USART flag is set or not.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_FLAG specifies the USART flag to check.
  * \retval SET the specified USART flag is SET.
  * \retval RESET the specified USART flag is RESET.
  */
FlagStatus USART_GetFlag(USART_TypeDef *USARTx, uint32_t USART_FLAG)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    if (USARTx->STATUS & USART_FLAG) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief  Clears the specified USART interrupt source.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_IT specifies the USART interrupt source to clear.
  */
void USART_ClearITStatus(USART_TypeDef *USARTx, uint32_t USART_IT)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    USARTx->STATUS = USART_IT;
}

/**
  * \brief  Checks whether the specified USART interrupt has occurred or not.
  * \param  USARTx where x can be 0, 1... to select the USART peripheral.
  * \param  USART_STATUS specifies the USART_CTRL  to check.
  * \retval SET the specified USART interrupt is SET.
  * \retval RESET the specified USART interrupt is RESET.
  */
FlagStatus USART_GetTXCTRL_Status(USART_TypeDef *USARTx, uint32_t USART_STATUS)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    if (USARTx->TXCTRL & USART_STATUS) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief  Checks whether the specified USART interrupt has occurred or not.
  * \param  USARTx where x can be 0, 1... to select the USART peripheral.
  * \param  USART_STATUS specifies the USART_CTRL  to check.
  * \retval SET the specified USART interrupt is SET.
  * \retval RESET the specified USART interrupt is RESET.
  */
FlagStatus USART_GetRXCTRL_Status(USART_TypeDef *USARTx, uint32_t USART_STATUS)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    if (USARTx->RXCTRL & USART_STATUS) {
        return SET;
    } else {
        return RESET;
    }
}
/**
  * \brief  Enables or disables the specified USART interrupts.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_IE specifies the USART interrupt sources to be enabled or disabled.
  * \param  Status Status of the specified USARTx interrupts.
  *                 This parameter can be: ENABLE or DISABLE.
  */
void USART_ITConfig(USART_TypeDef *USARTx, uint32_t USART_IE, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    if (Status) {
        USARTx->INT_EN |= (USART_IE);
    } else {
        USARTx->INT_EN &= ~(USART_IE);
    }
}

/**
  * \brief  Enables or disables the specified USART interrupts.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_IE specifies the USART interrupt sources to be enabled or disabled.
  * \retval SET the specified USART IT is SET.
  * \retval RESET the specified USART IT is RESET.
  */
FlagStatus USART_GetITConfig(USART_TypeDef *USARTx, uint32_t USART_IE)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    if (USARTx->INT_EN & USART_IE) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief  Transmits single data through the USARTx peripheral.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  Data the data to transmit.
  */
void USART_SendData(USART_TypeDef *USARTx, uint8_t Data)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_DATA(Data));

    while (USARTx->STATUS & USART_STATUS_FULL);
    USARTx->TXDATA = Data & 0xFF;
}

/**
  * \brief  Returns the most recent received data by the USARTx peripheral.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \return the received data.
  */
uint8_t USART_ReceiveData(USART_TypeDef *USARTx)
{
    uint32_t reg;

    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    do {
        reg = USARTx->STATUS;
    }
    while (reg & USART_STATUS_EMPTY);
    return (uint8_t)(USARTx->RXDATA & 0xFF);
}

/**
  * \brief  Transmits 9 bit data through the USARTx peripheral.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  Data the data to transmit.
  */
void USART_Send9Bit(USART_TypeDef *USARTx, uint16_t Data)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_DATA(Data));

    while (USARTx->STATUS & USART_STATUS_FULL);
    USARTx->TXDATA = Data & 0x1FF ;
}

/**
  * \brief  Returns the most recent received 9 bit data by the USARTx peripheral.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \return the received 9 bit data.
  */
uint16_t USART_Receive9Bit(USART_TypeDef *USARTx)
{
    uint32_t reg;

    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    do {
        reg = USARTx->STATUS;
    }
    while (reg & USART_STATUS_EMPTY);
    return (uint16_t)(USARTx->RXDATA & 0x1FF);
}

/**
  * \brief  Checks whether the specified USART transfer done flag is set or not.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_FLAG specifies the USART flag to check.
  * \retval SET the specified USART transfer done flag is SET.
  * \retval RESET the specified USART transfer done flag is RESET.
  */
FlagStatus USART_GetTransferFlag(USART_TypeDef *USARTx, uint32_t USART_FLAG)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    if (USARTx->STATUS & USART_FLAG) {
        USART_CleanTransferSize(USARTx, USART_FLAG);
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief  Initializes the USART watermark when sending data.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  Watermark transmit watermark value,can not more than 8.
  */
void USART_Set_TxWaterMark(USART_TypeDef *USARTx, uint32_t Watermark)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_TX_RX_WaterMark(Watermark));

    USARTx->TXCTRL &= ~USART_TXCTRL_TXCNT_MASK;
    USARTx->TXCTRL |= USART_TXCTRL_TXCNT(Watermark);
}

/**
  * \brief  Initializes the USART watermark when receiving data.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  Watermark receive watermark value,can not more than 8.
  */
void USART_Set_RxWaterMark(USART_TypeDef *USARTx, uint32_t Watermark)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_USART_TX_RX_WaterMark(Watermark));

    USARTx->RXCTRL &= ~USART_RXCTRL_RXCNT_MASK;
    USARTx->RXCTRL |= USART_RXCTRL_RXCNT(Watermark);
}

/**
  * \brief  Cleans the USART RX fifo.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  */
void USART_CleanFifo_RX(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    USARTx->RXCTRL |= USART_RXCTRL_CLR;
    USARTx->RXCTRL &= ~USART_RXCTRL_CLR;
}
/**
  * \brief  Cleans the USART TX fifo.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  */
void USART_CleanFifo_TX(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    USARTx->TXCTRL |= USART_TXCTRL_CLR;
    USARTx->TXCTRL &= ~USART_TXCTRL_CLR;
}

/**
  * \brief      Read Tx FIFO Entry.
  * \param[in]  USARTx where x can be 0 or 1 in USART mode.
  *
  * \retval the num of fifo entry.
  */
uint32_t USART_TxReadEntry(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    return USARTx->TX_FIFO_LEFT_ENTRY & 0x1F;
}

/**
  * \brief      Read Rx FIFO Entry.
  * \param[in]  USARTx where x can be 0 or 1 in USART mode.
  *
  * \retval the num of fifo entry.
  */
uint32_t USART_RxReadEntry(USART_TypeDef *USARTx)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    return USARTx->RX_FIFO_LEFT_ENTRY & 0x1F;
}

/**
  * \brief Set RXSIZE
  *
  * \param USARTx where x can be 0 or 1 in USART mode.
  * \param data set rx size
  */
void USART_Set_RXSize(USART_TypeDef *USARTx,uint32_t data)
{
    USARTx->RX_SIZE = data; 
}
/**
  * \brief  get RXSIZE
  *
  * \param  USARTx where x can be 0 or 1 in USART mode.
  *
  * \retval data the num of rx size
  */
uint32_t USART_Get_RXSize(USART_TypeDef *USARTx)
{
    uint32_t data = USARTx->RX_SIZE;
    return data;
}
/**
  * \brief Set TXSIZE
  *
  * \param USARTx where x can be 0 or 1 in USART mode.
  * \param data set tx size
  */
void USART_Set_TXSize(USART_TypeDef *USARTx,uint32_t data)
{
    USARTx->TX_SIZE = data; 
}
/**
  * \brief  get TXSIZE
  *
  * \param  USARTx where x can be 0 or 1 in USART mode.
  *
  * \retval data the num of tx size
  */
uint32_t USART_Get_TXSize(USART_TypeDef *USARTx)
{
    uint32_t data = USARTx->TX_SIZE;
    return data;
}

/**
  * \brief  Clean reg with write 1.
  * \param  USARTx where x can be 0, 1 ... to select the USART peripheral.
  * \param  USART_FLAG specifies the USART flag to check.
  *     \arg USART_STATUS_TX_EOT
  *     \arg USART_STATUS_RX_EOT
  */
void USART_CleanTransferSize(USART_TypeDef *USARTx, uint32_t USART_FLAG)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    if(USART_FLAG & USART_STATUS_TX_EOT) {
        USARTx->STATUS = USART_STATUS_TX_EOT;
    } else if(USART_FLAG & USART_STATUS_RX_EOT) {
        USARTx->STATUS = USART_STATUS_RX_EOT;
    }
}

/**
  * \brief   Setup SMARTCARD Txpue.
  * \param  USARTx where x can be 0, 1 ...to select the USART peripheral.
  * \param  Status SMARTCARD_SETUP.
  */
void USART_SetupTxpue(USART_TypeDef* USARTx, uint32_t Status)
{ 
    if (Status) {
        USARTx->SMARTCARD_SETUP |= USART_SMARTCARD_SETUP_TX_PUE;
    } else {
        USARTx->SMARTCARD_SETUP &= (~USART_SMARTCARD_SETUP_TX_PUE);
    }
}

/**
  * \brief  Set SMARTCARD USART_SetPrescaler reg.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_Prescaler Prescaler.
  */
void USART_SetPrescaler(USART_TypeDef* USARTx, uint32_t USART_Prescaler)
{ 
  /* Clear the USART prescaler */
  USARTx->SMARTCARD_TIMING &= ~USART_SMARTCARD_TIMING_PSC_MASK;
  /* Set the USART prescaler */
  USARTx->SMARTCARD_TIMING |= USART_SMARTCARD_TIMING_PSC(USART_Prescaler);
}
/**
  * \brief  Set SetGuardTime reg.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_GuardTime GuardTime.
  */
void USART_SetGuardTime(USART_TypeDef* USARTx, uint32_t USART_GuardTime)
{    
  /* Clear the USART Guard time */
  USARTx->SMARTCARD_TIMING &= ~USART_SMARTCARD_TIMING_GUARD_TIME_MASK;
  /* Set the USART guard time */
  USARTx->SMARTCARD_TIMING |= USART_SMARTCARD_TIMING_GUARD_TIME(USART_GuardTime);
}

/**
  * \brief  Set LINBreak reg.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  Status LIN_LBDL.
  */
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, uint32_t Status)
{
    if (Status==USART_ADVANCED_SETUP_LIN_LBDL_11) {
        USARTx->ADVANCED_SETUP |= USART_ADVANCED_SETUP_LIN_LBDL_11;
    } else {
        USARTx->ADVANCED_SETUP &= (~USART_ADVANCED_SETUP_LIN_LBDL_11);
    }

}

/**
  * \brief  Set LIN_EN reg.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  Status LIN_EN.
  */
void USART_LIN_EN(USART_TypeDef* USARTx, uint32_t Status)
{
    if (Status) {
        USARTx->ADVANCED_SETUP |= USART_ADVANCED_SETUP_LIN_EN;
    } else {
        USARTx->ADVANCED_SETUP &= (~USART_ADVANCED_SETUP_LIN_EN);
    }

}

/**
  * \brief  Set LINSendBreak reg.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  */
void USART_SendBreak(USART_TypeDef* USARTx)
{
    USARTx->ADVANCED_STATUS|=USART_ADVANCED_STATUS_LIN_SBKF;
}

/**
  * \brief  Set LIN_GET_INT reg.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  */
uint32_t USART_LIN_GET_INT(USART_TypeDef* USARTx)
{
    uint32_t data = (USARTx->ADVANCED_STATUS) & USART_ADVANCED_STATUS_LIN_LBDF;
    return data;
}

/**
  * \brief  Set WakeUp reg.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_FLAG WAKE_MOD.
  */

void USART_WakeUpConfig(USART_TypeDef* USARTx, uint32_t USART_FLAG)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    if(USART_FLAG==USART_ADVANCED_SETUP_WAKE_MOD_ADDR ) {
        USARTx->ADVANCED_SETUP |= USART_ADVANCED_SETUP_WAKE_MOD_ADDR;
    } else  {
        USARTx->ADVANCED_SETUP &= (~USART_ADVANCED_SETUP_WAKE_MOD_ADDR);
    } 
}
/**
  * \brief  Set ReceiverWakeUp reg.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  Status MM_EN.
  */
void USART_ReceiverWakeUpCmd(USART_TypeDef* USARTx, ControlStatus Status)
{
        /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        USARTx->ADVANCED_SETUP |= USART_ADVANCED_SETUP_MM_EN_ENABLE;
    } else {
        USARTx->ADVANCED_SETUP &= ~USART_ADVANCED_SETUP_MM_EN_ENABLE;
    }
}
/**
  * \brief  Set WriteWakeUp reg.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  */
void USART_WriteWakeUp(USART_TypeDef* USARTx)
{
    
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    USARTx->ADVANCED_STATUS |= USART_ADVANCED_STATUS_RWUFMM;
    
}
/**
  * \brief  Set READWakeUp reg.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  */
uint32_t USART_READWakeUp(USART_TypeDef* USARTx)
{
    uint32_t data;
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    data = USARTx->ADVANCED_STATUS & USART_ADVANCED_STATUS_RWUFMM;
    return data;

}
/**
 * \brief  Set SetRXAdd reg.
 * \param  USARTx where x can be 0, 1 to select the USART peripheral.
 * \param  USART_FLAG ADD7_7/4_BIT.
 * \param  data ADD data.
 */
void USART_SetRXAddress(USART_TypeDef* USARTx,uint32_t USART_FLAG, uint32_t data)
{
            /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    if (USART_FLAG == USART_ADVANCED_SETUP_ADD7_7_BIT) {
        USARTx->ADVANCED_SETUP |= USART_ADVANCED_SETUP_ADD7_7_BIT;
    } else {
        USARTx->ADVANCED_SETUP &= ~USART_ADVANCED_SETUP_ADD7_7_BIT;
    }

    USARTx->ADVANCED_SETUP &= (~ USART_ADVANCED_SETUP_ADD_MASK);
    USARTx->ADVANCED_SETUP |= (USART_ADVANCED_SETUP_ADD(data));

}
/**
 * \brief  Set Irda reg.
 * \param  USARTx where x can be 0, 1 to select the USART peripheral.
 * \param  Status IRDA_EN.
 */
void USART_Irda_En(USART_TypeDef* USARTx, ControlStatus Status)
{
        /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        USARTx->ADVANCED_SETUP |= USART_ADVANCED_SETUP_IRDA_EN_ENABLE;
    } else {
        USARTx->ADVANCED_SETUP &= ~USART_ADVANCED_SETUP_IRDA_EN_ENABLE;
    }
}

/**
 * \brief  Set Irdalp reg.
 * \param  USARTx where x can be 0, 1 to select the USART peripheral.
 * \param  Status RDALP_EN.
 */
void USART_IrdaLp_En(USART_TypeDef* USARTx, ControlStatus Status)
{
        /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        USARTx->ADVANCED_SETUP |= USART_ADVANCED_SETUP_IRDALP_EN_ENABLE;
    } else {
        USARTx->ADVANCED_SETUP &= ~USART_ADVANCED_SETUP_IRDALP_EN_ENABLE;
    }
}

/**
  * \brief  Set RTU ASCII.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  Status .
  */
void USART_Set_RTU(USART_TypeDef *USARTx, ControlStatus Status)
{ 
    if (Status) {
        USARTx->ADVANCED_SETUP |= USART_ADVANCED_SETUP_RTU_EN_ENABLE;
    } else {
        USARTx->ADVANCED_SETUP &= ~USART_ADVANCED_SETUP_RTU_EN_ENABLE;
    }
}

/**
  * \brief  Clears the specified USART interrupt source.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  USART_IT specifies the USART interrupt source to clear.
  */
void USART_ClearAdvancedITStatus(USART_TypeDef *USARTx, uint32_t USART_IT)
{
    /* Check the parameters */
    assert_param(IS_USART_ALL_PERIPH(USARTx));

    USARTx->ADVANCED_STATUS = USART_IT;
}

/**
  * \brief  Set SLV_EN reg.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  Status SLV_EN.
  */
void USART_SLV_EN(USART_TypeDef* USARTx, uint32_t Status)
{
    if (Status) {
        USARTx->SPI_SLAVE |= USART_SPI_SLAVE_SLV_EN;
    } else {
        USARTx->SPI_SLAVE &= (~USART_SPI_SLAVE_SLV_EN);
    }

}

/**
  * \brief  Set DIS_NSS_EN reg.
  * \param  USARTx where x can be 0, 1 to select the USART peripheral.
  * \param  Status DIS_NSS_EN.
  */
void USART_DIS_NSS_EN(USART_TypeDef* USARTx, uint32_t Status)
{
    if (Status) {
        USARTx->SPI_SLAVE |= USART_SPI_SLAVE_DIS_NSS;
    } else {
        USARTx->SPI_SLAVE &= (~USART_SPI_SLAVE_DIS_NSS);
    }

}

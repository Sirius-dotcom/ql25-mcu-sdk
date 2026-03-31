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

#ifndef _NS_USART_H_
#define _NS_USART_H_
/*!
 * \file     ns_usart.h
 * \brief    This file contains all the functions prototypes for the USART firmware
 */

#ifdef __cplusplus
 extern "C" {
#endif

#include "ns.h"

/**
  * \brief  USART Init Structure definition
  */
typedef struct {
    uint32_t USART_BaudRate;                /*!< Specifies the USART communication baud rate */
    uint32_t USART_WordLength;              /*!< Specifies the number of data bits transmitted or received in a frame */
    uint32_t USART_StopBits;                /*!< Specifies the number of stop bits transmitted */
    uint32_t USART_Parity;                  /*!< Specifies the parity mode */
    uint32_t USART_TX_CTL;                  /*!< Specifies whether the Transmit mode is enabled or disabled */
    uint32_t USART_RX_CTL;                  /*!< Specifies whether the Receive mode is enabled or disabled */
    uint32_t USART_HardwareFlowControl;     /*!< Specifies whether the hardware flow control mode is enabled */
    uint32_t USART_RX_IDLE_Timeout;         /*!< Specifies the receive idle timeout num */
    uint32_t USART_RX_WM_Timeout;           /*!< Specifies the receive watermark timeout num */
    uint32_t USART_TX_Data_Size;            /*!< Specifies the dma tx data size */
    uint32_t USART_RX_Data_Size;            /*!< Specifies the dma rx data size */
    uint32_t USART_SC_EN;                   /*!< Specifies smartcard en */
    uint32_t USART_DMA_EN;                  /*!< Tx/Rx channel dma mode enable */
    uint32_t USART_RX_DMA_EN;               /*!< Rx channel dma mode enable, this bit is actived when SEPARATE_EN bit is enable */
    uint32_t USART_SEPARATE_EN;             /*!< Enable this bit can make tx and rx worked in cpu or dma mode independently */

    uint32_t AutoBaudRateEnable;
    uint32_t AutoBaudRateMode;

    uint32_t USART_Fra_Baud;
    uint32_t USART_CLK_Feq;

} USART_InitTypeDef;

typedef struct {   
    uint32_t USART_485EN;                            /*!< Specifies the RS-485 mode enable */
    uint32_t USART_485Start_Early;                   /*!< Specifies  Enable RS-485's DE signal assert before START condition one bit-time early*/       
    uint32_t USART_485Stop_Delay;                    /*!< Specifies  Enable RS-485's DE signal dessert after STOP condition one bit-time later*/
    uint32_t USART_485DE;                            /*!< Specifies  Enable RS-485's DE signal polarity*/
} USART_485InitTypeDef;

/**
  * \brief  USART Clock Init Structure definition
  */
typedef struct {
    uint32_t USART_Clock;                   /*!< Specifies whether the USART clock is enabled or disabled */
    uint32_t USART_CPOL;                    /*!< Specifies the steady state value of the serial clock */
    uint32_t USART_CPHA;                    /*!< Specifies the clock transition on which the bit capture is made */
    uint32_t USART_LastBit;                 /*!< Specifies whether the clock pulse corresponding to the last transmitted data bit (MSB) has to be output on the SCLK pin in synchronous mode */
} USART_ClockInitTypeDef;
#define USART_CLK_3_0_0                          1
#define USART_WordLength_8b                 USART_SETUP_BIT_LENGTH_8
#define USART_STOP_BIT_1                    USART_SETUP_CFG_STOP_BIT_1BIT
#define USART_TXCTRL_ENABLE                 USART_TXCTRL_TXEN_ENABLE
#define USART_RXCTRL_ENABLE                 USART_RXCTRL_RXEN_ENABLE
#define USART_PARITY_DISABLE                USART_SETUP_PARITY_EN_DISABLE
#define USART_HardwareFlowControl_NONE      USART_SETUP_RTSE_DISABLE |USART_SETUP_CTSE_DISABLE
/**
 * \brief Check the USART periphral parameters.
 * \param PERIPH: USART periphral to be checked.
 * \retval 0: This is not a valid USART periphral.
 * \retval 1: This is a valid USART periphral.
 */

#define USART_BUF_DEPTH 16

#define IS_USART_ALL_PERIPH(PERIPH)                 ( ((PERIPH) == USART0)                                                        || ((PERIPH) == USART1)                                                       || ((PERIPH) == USART2)                                                       || ((PERIPH) == USART3)                                                       || ((PERIPH) == USART4)                                                       || ((PERIPH) == USART5)                                                       || ((PERIPH) == USART6)                                                       || ((PERIPH) == USART7)                                                       || ((PERIPH) == USART8)                                                       || ((PERIPH) == USART9))

#define IS_USART_DATA(DATA)                         ((DATA) <= 0x1FF)                               /*!< USART Transmit/Receive data[8:0] */

#define IS_USART_TX_RX_WaterMark(WaterMark)         ((WaterMark) <= USART_BUF_DEPTH) 

#define USART_TXDATA_OFFSET                                             0x0 /*!< Transmit data and fifo full */
#define USART_RXDATA_OFFSET                                             0x4 /*!< Receive data and fifo empty */
#define USART_TXCTRL_OFFSET                                             0x8 /*!< Transmit control */
#define USART_RXCTRL_OFFSET                                             0xc /*!< Receive control */
#define USART_INT_EN_OFFSET                                             0x10 /*!< Interrupt control */
#define USART_STATUS_OFFSET                                             0x14 /*!< Interrupt status */
#define USART_DIV_OFFSET                                                0x18 /*!< Baud rate divider */
#define USART_SETUP_OFFSET                                              0x1c /*!< UART configurations and settings */
#define USART_RX_SIZE_OFFSET                                            0x24 /*!< Total data size in rx-transmit mode */
#define USART_TX_SIZE_OFFSET                                            0x30 /*!< Total data size in tx-transmit mode */
#define USART_SPI_SLAVE_OFFSET                                          0x34 /*!< N/A */
#define USART_RX_IDLE_OFFSET                                            0x44 /*!< RX line idle time out setting */
#define USART_RX_WM_OFFSET                                              0x48 /*!< RX water mark time out setting */
#define USART_RX_FIFO_LEFT_ENTRY_OFFSET                                 0x4c /*!< RX FIFO entry count */
#define USART_TX_FIFO_LEFT_ENTRY_OFFSET                                 0x50 /*!< TX FIFO entry count */
#define USART_TX_DATASIZE_OFFSET                                        0x54 /*!< TX data size register */
#define USART_RX_DATASIZE_OFFSET                                        0x58 /*!< RX data size register */
#define USART_SMARTCARD_SETUP_OFFSET                                    0x60 /*!< Smartcard mode setup */
#define USART_SMARTCARD_TIMING_OFFSET                                   0x64 /*!< Smartcard timing configuration */
#define USART_IP_VERSION_OFFSET                                         0x68 /*!< Version number register */
#define USART_ADVANCED_SETUP_OFFSET                                     0x7c /*!< UART advanced feature configurations and settings */
#define USART_ADVANCED_STATUS_OFFSET                                    0x80 /*!< UART advanced feature status register */

 /* ===== USART TXDATA Register definition ===== */
#define USART_TXDATA_TX_DATA_MASK                 BITS(0,8)                                   /*!< USART TXDATA: TX_DATA Bit Mask */  
#define USART_TXDATA_TX_DATA_OFS                  0U                                          /*!< USART TXDATA: TX_DATA Bit Offset */
#define USART_TXDATA_TX_DATA(regval)              (BITS(0,8) & ((uint32_t)(regval) << 0))        /*!< USART TXDATA: TX_DATA Bit Value */  
 
 /* ===== USART RXDATA Register definition ===== */
#define USART_RXDATA_RX_DATA                      BITS(0,8)                
 
 /* ===== USART TXCTRL Register definition ===== */
#define USART_TXCTRL_TXEN                         BIT(0)                                      /*!< Transmit enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_TXCTRL_TXEN_OFS                     0U                                          /*!< USART TXCTRL: TXEN Bit Offset */
#define USART_TXCTRL_TXEN_VAL(regval)                 (BIT(0) & ((uint32_t)(regval) << 0))        /*!< USART TXCTRL: TXEN Bit Value */  
#define USART_TXCTRL_TXEN_DISABLE                      0x0UL                                              /*!< DISABLE */
#define USART_TXCTRL_TXEN_ENABLE                       BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_txctrl txen bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid txen bit.
  * \retval 1 This is a valid txen bit.
  */
#define IS_USART_UART_TXCTRL_TXEN(regval)                   (\
                                             ((regval) == USART_TXCTRL_TXEN_DISABLE             ) || \
                                             ((regval) == USART_TXCTRL_TXEN_ENABLE              )  \
                                                 )

#define USART_TXCTRL_CONTINUOUS                   BIT(1)                                      /*!< TX channel continuous mode bitfield: -1'b0:  disable:  Disabled uDMA reloads -1'b1: enable: Enabled uDMA reloads and starts a new transfer At the end of the buffer transfer, the uDMA reloads the address / buffer size and starts a new transfer. */
#define USART_TXCTRL_CONTINUOUS_OFS               1U                                          /*!< USART TXCTRL: CONTINUOUS Bit Offset */
#define USART_TXCTRL_CONTINUOUS_VAL(regval)           (BIT(1) & ((uint32_t)(regval) << 1))        /*!< USART TXCTRL: CONTINUOUS Bit Value */  
#define USART_TXCTRL_CONTINUOUS_DISABLE                0x0UL                                              /*!< DISABLE */
#define USART_TXCTRL_CONTINUOUS_ENABLE                 BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_txctrl continuous bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid continuous bit.
  * \retval 1 This is a valid continuous bit.
  */
#define IS_USART_UART_TXCTRL_CONTINUOUS(regval)             (\
                                       ((regval) == USART_TXCTRL_CONTINUOUS_DISABLE             ) || \
                                       ((regval) == USART_TXCTRL_CONTINUOUS_ENABLE              )  \
                                                 )

#define USART_TXCTRL_CLR                          BIT(2)                                      /*!< TX channel clear and stop transfer: -1'b0:  disable Disable -1'b1:  enable Enable stop and clear the on-going transfer, which also used for clean tx-fifo. */
#define USART_TXCTRL_CLR_OFS                      2U                                          /*!< USART TXCTRL: CLR Bit Offset */
#define USART_TXCTRL_CLR_VAL(regval)                  (BIT(2) & ((uint32_t)(regval) << 2))        /*!< USART TXCTRL: CLR Bit Value */  
#define USART_TXCTRL_CLR_DISABLE                       0x0UL                                              /*!< DISABLE */
#define USART_TXCTRL_CLR_ENABLE                        BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_txctrl clr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid clr bit.
  * \retval 1 This is a valid clr bit.
  */
#define IS_USART_UART_TXCTRL_CLR(regval)                    (\
                                              ((regval) == USART_TXCTRL_CLR_DISABLE             ) || \
                                              ((regval) == USART_TXCTRL_CLR_ENABLE              )  \
                                                 )

#define USART_TXCTRL_TXCNT_MASK                   BITS(16,20)                                   /*!< USART TXCTRL: TXCNT Bit Mask */  
#define USART_TXCTRL_TXCNT_OFS                    16U                                          /*!< USART TXCTRL: TXCNT Bit Offset */
#define USART_TXCTRL_TXCNT(regval)                (BITS(16,20) & ((uint32_t)(regval) << 16))        /*!< USART TXCTRL: TXCNT Bit Value */  
 
 /* ===== USART RXCTRL Register definition ===== */
#define USART_RXCTRL_RXEN                         BIT(0)                                      /*!< Receive enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_RXCTRL_RXEN_OFS                     0U                                          /*!< USART RXCTRL: RXEN Bit Offset */
#define USART_RXCTRL_RXEN_VAL(regval)                 (BIT(0) & ((uint32_t)(regval) << 0))        /*!< USART RXCTRL: RXEN Bit Value */  
#define USART_RXCTRL_RXEN_DISABLE                      0x0UL                                              /*!< DISABLE */
#define USART_RXCTRL_RXEN_ENABLE                       BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_rxctrl rxen bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rxen bit.
  * \retval 1 This is a valid rxen bit.
  */
#define IS_USART_UART_RXCTRL_RXEN(regval)                   (\
                                             ((regval) == USART_RXCTRL_RXEN_DISABLE             ) || \
                                             ((regval) == USART_RXCTRL_RXEN_ENABLE              )  \
                                                 )

#define USART_RXCTRL_CONTINUOUS                   BIT(1)                                      /*!< RX channel continuous mode bitfield: -1'b0:  disable: Disabled uDMA reloads -1'b1:  enable: Enabled uDMA reloads and starts a new transfer At the end of the buffer transfer, the uDMA reloads the address / buffer size and starts a new transfer. */
#define USART_RXCTRL_CONTINUOUS_OFS               1U                                          /*!< USART RXCTRL: CONTINUOUS Bit Offset */
#define USART_RXCTRL_CONTINUOUS_VAL(regval)           (BIT(1) & ((uint32_t)(regval) << 1))        /*!< USART RXCTRL: CONTINUOUS Bit Value */  
#define USART_RXCTRL_CONTINUOUS_DISABLE                0x0UL                                              /*!< DISABLE */
#define USART_RXCTRL_CONTINUOUS_ENABLE                 BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_rxctrl continuous bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid continuous bit.
  * \retval 1 This is a valid continuous bit.
  */
#define IS_USART_UART_RXCTRL_CONTINUOUS(regval)             (\
                                       ((regval) == USART_RXCTRL_CONTINUOUS_DISABLE             ) || \
                                       ((regval) == USART_RXCTRL_CONTINUOUS_ENABLE              )  \
                                                 )

#define USART_RXCTRL_CLR                          BIT(2)                                      /*!< RX channel clear and stop transfer: 0:  disable Disable 1:  enable Enable: stop and clear the on-going transfer, which also used for clean rx-fifo. */
#define USART_RXCTRL_CLR_OFS                      2U                                          /*!< USART RXCTRL: CLR Bit Offset */
#define USART_RXCTRL_CLR_VAL(regval)                  (BIT(2) & ((uint32_t)(regval) << 2))        /*!< USART RXCTRL: CLR Bit Value */  
#define USART_RXCTRL_RXCNT_MASK                   BITS(16,20)                                   /*!< USART RXCTRL: RXCNT Bit Mask */  
#define USART_RXCTRL_RXCNT_OFS                    16U                                          /*!< USART RXCTRL: RXCNT Bit Offset */
#define USART_RXCTRL_RXCNT(regval)                (BITS(16,20) & ((uint32_t)(regval) << 16))        /*!< USART RXCTRL: RXCNT Bit Value */  
 
 /* ===== USART INT_EN Register definition ===== */
#define USART_INT_EN_TXIE                         BIT(0)                                      /*!< Transmit watermark interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_TXIE_OFS                     0U                                          /*!< USART INT EN: TXIE Bit Offset */
#define USART_INT_EN_TXIE_VAL(regval)                 (BIT(0) & ((uint32_t)(regval) << 0))        /*!< USART INT EN: TXIE Bit Value */  
#define USART_INT_EN_TXIE_DISABLE                      0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_TXIE_ENABLE                       BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en txie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid txie bit.
  * \retval 1 This is a valid txie bit.
  */
#define IS_USART_UART_INT_EN_TXIE(regval)                   (\
                                             ((regval) == USART_INT_EN_TXIE_DISABLE             ) || \
                                             ((regval) == USART_INT_EN_TXIE_ENABLE              )  \
                                                 )

#define USART_INT_EN_RXIE                         BIT(1)                                      /*!< Receive watermark interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_RXIE_OFS                     1U                                          /*!< USART INT EN: RXIE Bit Offset */
#define USART_INT_EN_RXIE_VAL(regval)                 (BIT(1) & ((uint32_t)(regval) << 1))        /*!< USART INT EN: RXIE Bit Value */  
#define USART_INT_EN_RXIE_DISABLE                      0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_RXIE_ENABLE                       BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en rxie bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rxie bit.
  * \retval 1 This is a valid rxie bit.
  */
#define IS_USART_UART_INT_EN_RXIE(regval)                   (\
                                             ((regval) == USART_INT_EN_RXIE_DISABLE             ) || \
                                             ((regval) == USART_INT_EN_RXIE_ENABLE              )  \
                                                 )

#define USART_INT_EN_OW_IRQ_EN                    BIT(2)                                      /*!< RX overflow irq enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_OW_IRQ_EN_OFS                2U                                          /*!< USART INT EN: OW_IRQ_EN Bit Offset */
#define USART_INT_EN_OW_IRQ_EN_VAL(regval)            (BIT(2) & ((uint32_t)(regval) << 2))        /*!< USART INT EN: OW_IRQ_EN Bit Value */  
#define USART_INT_EN_OW_IRQ_EN_DISABLE                 0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_OW_IRQ_EN_ENABLE                  BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en ow_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ow_irq_en bit.
  * \retval 1 This is a valid ow_irq_en bit.
  */
#define IS_USART_UART_INT_EN_OW_IRQ_EN(regval)              (\
                                        ((regval) == USART_INT_EN_OW_IRQ_EN_DISABLE             ) || \
                                        ((regval) == USART_INT_EN_OW_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_PARITY_IRQ_EN                BIT(3)                                      /*!< RX parity irq enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_PARITY_IRQ_EN_OFS            3U                                          /*!< USART INT EN: PARITY_IRQ_EN Bit Offset */
#define USART_INT_EN_PARITY_IRQ_EN_VAL(regval)        (BIT(3) & ((uint32_t)(regval) << 3))        /*!< USART INT EN: PARITY_IRQ_EN Bit Value */  
#define USART_INT_EN_PARITY_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_PARITY_IRQ_EN_ENABLE              BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en parity_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid parity_irq_en bit.
  * \retval 1 This is a valid parity_irq_en bit.
  */
#define IS_USART_UART_INT_EN_PARITY_IRQ_EN(regval)          (\
                                    ((regval) == USART_INT_EN_PARITY_IRQ_EN_DISABLE             ) || \
                                    ((regval) == USART_INT_EN_PARITY_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_CTSIE_RISE_EN                BIT(4)                                      /*!< CTS rise interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_CTSIE_RISE_EN_OFS            4U                                          /*!< USART INT EN: CTSIE_RISE_EN Bit Offset */
#define USART_INT_EN_CTSIE_RISE_EN_VAL(regval)        (BIT(4) & ((uint32_t)(regval) << 4))        /*!< USART INT EN: CTSIE_RISE_EN Bit Value */  
#define USART_INT_EN_CTSIE_RISE_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_CTSIE_RISE_EN_ENABLE              BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en ctsie_rise_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ctsie_rise_en bit.
  * \retval 1 This is a valid ctsie_rise_en bit.
  */
#define IS_USART_UART_INT_EN_CTSIE_RISE_EN(regval)          (\
                                    ((regval) == USART_INT_EN_CTSIE_RISE_EN_DISABLE             ) || \
                                    ((regval) == USART_INT_EN_CTSIE_RISE_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_CTSIE_FALL_EN                BIT(5)                                      /*!< CTS fall interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_CTSIE_FALL_EN_OFS            5U                                          /*!< USART INT EN: CTSIE_FALL_EN Bit Offset */
#define USART_INT_EN_CTSIE_FALL_EN_VAL(regval)        (BIT(5) & ((uint32_t)(regval) << 5))        /*!< USART INT EN: CTSIE_FALL_EN Bit Value */  
#define USART_INT_EN_CTSIE_FALL_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_CTSIE_FALL_EN_ENABLE              BIT(5)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en ctsie_fall_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ctsie_fall_en bit.
  * \retval 1 This is a valid ctsie_fall_en bit.
  */
#define IS_USART_UART_INT_EN_CTSIE_FALL_EN(regval)          (\
                                    ((regval) == USART_INT_EN_CTSIE_FALL_EN_DISABLE             ) || \
                                    ((regval) == USART_INT_EN_CTSIE_FALL_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_STOP_BIT_IRQ_EN              BIT(6)                                      /*!< Stop bit interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_STOP_BIT_IRQ_EN_OFS          6U                                          /*!< USART INT EN: STOP_BIT_IRQ_EN Bit Offset */
#define USART_INT_EN_STOP_BIT_IRQ_EN_VAL(regval)      (BIT(6) & ((uint32_t)(regval) << 6))        /*!< USART INT EN: STOP_BIT_IRQ_EN Bit Value */  
#define USART_INT_EN_STOP_BIT_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_STOP_BIT_IRQ_EN_ENABLE             BIT(6)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en stop_bit_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid stop_bit_irq_en bit.
  * \retval 1 This is a valid stop_bit_irq_en bit.
  */
#define IS_USART_UART_INT_EN_STOP_BIT_IRQ_EN(regval)        (\
                                  ((regval) == USART_INT_EN_STOP_BIT_IRQ_EN_DISABLE             ) || \
                                  ((regval) == USART_INT_EN_STOP_BIT_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_SPI_SLV_UNDER_RUN_IRQ_EN         BIT(7)                                      /*!< SPI slave under-run interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_SPI_SLV_UNDER_RUN_IRQ_EN_OFS     7U                                          /*!< USART INT EN: SPI_SLV_UNDER_RUN_IRQ_EN Bit Offset */
#define USART_INT_EN_SPI_SLV_UNDER_RUN_IRQ_EN_VAL(regval) (BIT(7) & ((uint32_t)(regval) << 7))        /*!< USART INT EN: SPI_SLV_UNDER_RUN_IRQ_EN Bit Value */  
#define USART_INT_EN_SPI_SLV_UNDER_RUN_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_SPI_SLV_UNDER_RUN_IRQ_EN_ENABLE             BIT(7)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en spi_slv_under_run_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid spi_slv_under_run_irq_en bit.
  * \retval 1 This is a valid spi_slv_under_run_irq_en bit.
  */
#define IS_USART_UART_INT_EN_SPI_SLV_UNDER_RUN_IRQ_EN(regval)   (\
                             ((regval) == USART_INT_EN_SPI_SLV_UNDER_RUN_IRQ_EN_DISABLE             ) || \
                             ((regval) == USART_INT_EN_SPI_SLV_UNDER_RUN_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_SPI_SLV_OVER_RUN_IRQ_EN         BIT(8)                                      /*!< SPI slave over-run interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_SPI_SLV_OVER_RUN_IRQ_EN_OFS     8U                                          /*!< USART INT EN: SPI_SLV_OVER_RUN_IRQ_EN Bit Offset */
#define USART_INT_EN_SPI_SLV_OVER_RUN_IRQ_EN_VAL(regval) (BIT(8) & ((uint32_t)(regval) << 8))        /*!< USART INT EN: SPI_SLV_OVER_RUN_IRQ_EN Bit Value */  
#define USART_INT_EN_SPI_SLV_OVER_RUN_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_SPI_SLV_OVER_RUN_IRQ_EN_ENABLE             BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en spi_slv_over_run_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid spi_slv_over_run_irq_en bit.
  * \retval 1 This is a valid spi_slv_over_run_irq_en bit.
  */
#define IS_USART_UART_INT_EN_SPI_SLV_OVER_RUN_IRQ_EN(regval)   (\
                             ((regval) == USART_INT_EN_SPI_SLV_OVER_RUN_IRQ_EN_DISABLE             ) || \
                             ((regval) == USART_INT_EN_SPI_SLV_OVER_RUN_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_RX_IDLE_TOUT_IRQ_EN          BIT(9)                                      /*!< RX idle timeout interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_RX_IDLE_TOUT_IRQ_EN_OFS      9U                                          /*!< USART INT EN: RX_IDLE_TOUT_IRQ_EN Bit Offset */
#define USART_INT_EN_RX_IDLE_TOUT_IRQ_EN_VAL(regval)  (BIT(9) & ((uint32_t)(regval) << 9))        /*!< USART INT EN: RX_IDLE_TOUT_IRQ_EN Bit Value */  
#define USART_INT_EN_RX_IDLE_TOUT_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_RX_IDLE_TOUT_IRQ_EN_ENABLE             BIT(9)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en rx_idle_tout_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_idle_tout_irq_en bit.
  * \retval 1 This is a valid rx_idle_tout_irq_en bit.
  */
#define IS_USART_UART_INT_EN_RX_IDLE_TOUT_IRQ_EN(regval)    (\
                              ((regval) == USART_INT_EN_RX_IDLE_TOUT_IRQ_EN_DISABLE             ) || \
                              ((regval) == USART_INT_EN_RX_IDLE_TOUT_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_RX_WM_TOUT_IRQ_EN            BIT(10)                                      /*!< RX watermark interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_RX_WM_TOUT_IRQ_EN_OFS        10U                                          /*!< USART INT EN: RX_WM_TOUT_IRQ_EN Bit Offset */
#define USART_INT_EN_RX_WM_TOUT_IRQ_EN_VAL(regval)    (BIT(10) & ((uint32_t)(regval) << 10))        /*!< USART INT EN: RX_WM_TOUT_IRQ_EN Bit Value */  
#define USART_INT_EN_RX_WM_TOUT_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_RX_WM_TOUT_IRQ_EN_ENABLE             BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en rx_wm_tout_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_wm_tout_irq_en bit.
  * \retval 1 This is a valid rx_wm_tout_irq_en bit.
  */
#define IS_USART_UART_INT_EN_RX_WM_TOUT_IRQ_EN(regval)      (\
                                ((regval) == USART_INT_EN_RX_WM_TOUT_IRQ_EN_DISABLE             ) || \
                                ((regval) == USART_INT_EN_RX_WM_TOUT_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_TX_EOT_IRQ_EN                BIT(11)                                      /*!< TX transfer done interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_TX_EOT_IRQ_EN_OFS            11U                                          /*!< USART INT EN: TX_EOT_IRQ_EN Bit Offset */
#define USART_INT_EN_TX_EOT_IRQ_EN_VAL(regval)        (BIT(11) & ((uint32_t)(regval) << 11))        /*!< USART INT EN: TX_EOT_IRQ_EN Bit Value */  
#define USART_INT_EN_TX_EOT_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_TX_EOT_IRQ_EN_ENABLE              BIT(11)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en tx_eot_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_eot_irq_en bit.
  * \retval 1 This is a valid tx_eot_irq_en bit.
  */
#define IS_USART_UART_INT_EN_TX_EOT_IRQ_EN(regval)          (\
                                    ((regval) == USART_INT_EN_TX_EOT_IRQ_EN_DISABLE             ) || \
                                    ((regval) == USART_INT_EN_TX_EOT_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_RX_EOT_IRQ_EN                BIT(12)                                      /*!< RX transfer done interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_RX_EOT_IRQ_EN_OFS            12U                                          /*!< USART INT EN: RX_EOT_IRQ_EN Bit Offset */
#define USART_INT_EN_RX_EOT_IRQ_EN_VAL(regval)        (BIT(12) & ((uint32_t)(regval) << 12))        /*!< USART INT EN: RX_EOT_IRQ_EN Bit Value */  
#define USART_INT_EN_RX_EOT_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_RX_EOT_IRQ_EN_ENABLE              BIT(12)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en rx_eot_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_eot_irq_en bit.
  * \retval 1 This is a valid rx_eot_irq_en bit.
  */
#define IS_USART_UART_INT_EN_RX_EOT_IRQ_EN(regval)          (\
                                    ((regval) == USART_INT_EN_RX_EOT_IRQ_EN_DISABLE             ) || \
                                    ((regval) == USART_INT_EN_RX_EOT_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN         BIT(13)                                      /*!< TX_FIFO empty interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN_OFS     13U                                          /*!< USART INT EN: TX_FIFO_EMPTY_IRQ_EN Bit Offset */
#define USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN_VAL(regval) (BIT(13) & ((uint32_t)(regval) << 13))        /*!< USART INT EN: TX_FIFO_EMPTY_IRQ_EN Bit Value */  
#define USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN_ENABLE             BIT(13)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en tx_fifo_empty_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_fifo_empty_irq_en bit.
  * \retval 1 This is a valid tx_fifo_empty_irq_en bit.
  */
#define IS_USART_UART_INT_EN_TX_FIFO_EMPTY_IRQ_EN(regval)   (\
                             ((regval) == USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN_DISABLE             ) || \
                             ((regval) == USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_TX_FIFO_FULL_IRQ_EN          BIT(14)                                      /*!< TX_FIFO full interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_TX_FIFO_FULL_IRQ_EN_OFS      14U                                          /*!< USART INT EN: TX_FIFO_FULL_IRQ_EN Bit Offset */
#define USART_INT_EN_TX_FIFO_FULL_IRQ_EN_VAL(regval)  (BIT(14) & ((uint32_t)(regval) << 14))        /*!< USART INT EN: TX_FIFO_FULL_IRQ_EN Bit Value */  
#define USART_INT_EN_TX_FIFO_FULL_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_TX_FIFO_FULL_IRQ_EN_ENABLE             BIT(14)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en tx_fifo_full_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_fifo_full_irq_en bit.
  * \retval 1 This is a valid tx_fifo_full_irq_en bit.
  */
#define IS_USART_UART_INT_EN_TX_FIFO_FULL_IRQ_EN(regval)    (\
                              ((regval) == USART_INT_EN_TX_FIFO_FULL_IRQ_EN_DISABLE             ) || \
                              ((regval) == USART_INT_EN_TX_FIFO_FULL_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_RX_FIFO_EMPTY_IRQ_EN         BIT(15)                                      /*!< RX_FIFO empty interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_RX_FIFO_EMPTY_IRQ_EN_OFS     15U                                          /*!< USART INT EN: RX_FIFO_EMPTY_IRQ_EN Bit Offset */
#define USART_INT_EN_RX_FIFO_EMPTY_IRQ_EN_VAL(regval) (BIT(15) & ((uint32_t)(regval) << 15))        /*!< USART INT EN: RX_FIFO_EMPTY_IRQ_EN Bit Value */  
#define USART_INT_EN_RX_FIFO_EMPTY_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_RX_FIFO_EMPTY_IRQ_EN_ENABLE             BIT(15)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en rx_fifo_empty_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_fifo_empty_irq_en bit.
  * \retval 1 This is a valid rx_fifo_empty_irq_en bit.
  */
#define IS_USART_UART_INT_EN_RX_FIFO_EMPTY_IRQ_EN(regval)   (\
                             ((regval) == USART_INT_EN_RX_FIFO_EMPTY_IRQ_EN_DISABLE             ) || \
                             ((regval) == USART_INT_EN_RX_FIFO_EMPTY_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_RX_FIFO_FULL_IRQ_EN          BIT(16)                                      /*!< RX_FIFO full interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_RX_FIFO_FULL_IRQ_EN_OFS      16U                                          /*!< USART INT EN: RX_FIFO_FULL_IRQ_EN Bit Offset */
#define USART_INT_EN_RX_FIFO_FULL_IRQ_EN_VAL(regval)  (BIT(16) & ((uint32_t)(regval) << 16))        /*!< USART INT EN: RX_FIFO_FULL_IRQ_EN Bit Value */  
#define USART_INT_EN_RX_FIFO_FULL_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_RX_FIFO_FULL_IRQ_EN_ENABLE             BIT(16)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en rx_fifo_full_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_fifo_full_irq_en bit.
  * \retval 1 This is a valid rx_fifo_full_irq_en bit.
  */
#define IS_USART_UART_INT_EN_RX_FIFO_FULL_IRQ_EN(regval)    (\
                              ((regval) == USART_INT_EN_RX_FIFO_FULL_IRQ_EN_DISABLE             ) || \
                              ((regval) == USART_INT_EN_RX_FIFO_FULL_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_FRAME_ERR_IRQ_EN             BIT(17)                                      /*!< SMARTCARD TX / RX break field detected Frame error interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_FRAME_ERR_IRQ_EN_OFS         17U                                          /*!< USART INT EN: FRAME_ERR_IRQ_EN Bit Offset */
#define USART_INT_EN_FRAME_ERR_IRQ_EN_VAL(regval)     (BIT(17) & ((uint32_t)(regval) << 17))        /*!< USART INT EN: FRAME_ERR_IRQ_EN Bit Value */  
#define USART_INT_EN_FRAME_ERR_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_FRAME_ERR_IRQ_EN_ENABLE             BIT(17)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en frame_err_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid frame_err_irq_en bit.
  * \retval 1 This is a valid frame_err_irq_en bit.
  */
#define IS_USART_UART_INT_EN_FRAME_ERR_IRQ_EN(regval)       (\
                                 ((regval) == USART_INT_EN_FRAME_ERR_IRQ_EN_DISABLE             ) || \
                                 ((regval) == USART_INT_EN_FRAME_ERR_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_LIN_LBDIE_IRQ_EN             BIT(18)                                      /*!< LIN mode break detection interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_LIN_LBDIE_IRQ_EN_OFS         18U                                          /*!< USART INT EN: LIN_LBDIE_IRQ_EN Bit Offset */
#define USART_INT_EN_LIN_LBDIE_IRQ_EN_VAL(regval)     (BIT(18) & ((uint32_t)(regval) << 18))        /*!< USART INT EN: LIN_LBDIE_IRQ_EN Bit Value */  
#define USART_INT_EN_LIN_LBDIE_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_LIN_LBDIE_IRQ_EN_ENABLE             BIT(18)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en lin_lbdie_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid lin_lbdie_irq_en bit.
  * \retval 1 This is a valid lin_lbdie_irq_en bit.
  */
#define IS_USART_UART_INT_EN_LIN_LBDIE_IRQ_EN(regval)       (\
                                 ((regval) == USART_INT_EN_LIN_LBDIE_IRQ_EN_DISABLE             ) || \
                                 ((regval) == USART_INT_EN_LIN_LBDIE_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_MODBUS_RTU_TOUT_IRQ_EN         BIT(19)                                      /*!< MODBUS receiver timeout interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_MODBUS_RTU_TOUT_IRQ_EN_OFS     19U                                          /*!< USART INT EN: MODBUS_RTU_TOUT_IRQ_EN Bit Offset */
#define USART_INT_EN_MODBUS_RTU_TOUT_IRQ_EN_VAL(regval) (BIT(19) & ((uint32_t)(regval) << 19))        /*!< USART INT EN: MODBUS_RTU_TOUT_IRQ_EN Bit Value */  
#define USART_INT_EN_MODBUS_RTU_TOUT_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_MODBUS_RTU_TOUT_IRQ_EN_ENABLE             BIT(19)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en modbus_rtu_tout_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid modbus_rtu_tout_irq_en bit.
  * \retval 1 This is a valid modbus_rtu_tout_irq_en bit.
  */
#define IS_USART_UART_INT_EN_MODBUS_RTU_TOUT_IRQ_EN(regval)   (\
                             ((regval) == USART_INT_EN_MODBUS_RTU_TOUT_IRQ_EN_DISABLE             ) || \
                             ((regval) == USART_INT_EN_MODBUS_RTU_TOUT_IRQ_EN_ENABLE              )  \
                                                 )

#define USART_INT_EN_MODBUS_ASCII_MATCH_IRQ_EN         BIT(20)                                      /*!< MODBUS ASCII character match interrupt enable -1'b0:  disable:  -1'b1:  enable:  */
#define USART_INT_EN_MODBUS_ASCII_MATCH_IRQ_EN_OFS     20U                                          /*!< USART INT EN: MODBUS_ASCII_MATCH_IRQ_EN Bit Offset */
#define USART_INT_EN_MODBUS_ASCII_MATCH_IRQ_EN_VAL(regval) (BIT(20) & ((uint32_t)(regval) << 20))        /*!< USART INT EN: MODBUS_ASCII_MATCH_IRQ_EN Bit Value */  
#define USART_INT_EN_MODBUS_ASCII_MATCH_IRQ_EN_DISABLE             0x0UL                                              /*!< DISABLE */
#define USART_INT_EN_MODBUS_ASCII_MATCH_IRQ_EN_ENABLE             BIT(20)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_int_en modbus_ascii_match_irq_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid modbus_ascii_match_irq_en bit.
  * \retval 1 This is a valid modbus_ascii_match_irq_en bit.
  */
#define IS_USART_UART_INT_EN_MODBUS_ASCII_MATCH_IRQ_EN(regval)   (\
                             ((regval) == USART_INT_EN_MODBUS_ASCII_MATCH_IRQ_EN_DISABLE             ) || \
                             ((regval) == USART_INT_EN_MODBUS_ASCII_MATCH_IRQ_EN_ENABLE              )  \
                                                 )

 /* ===== USART STATUS Register definition ===== */
#define USART_STATUS_TXIP                         BIT(0)                                      /*!< Transmit watermark interrupt pending */
#define USART_STATUS_RXIP                         BIT(1)                                      /*!< Receive watermark interrupt pending */
#define USART_STATUS_TX_BUSY                      BIT(2)                                      /*!< TX busy status flag: -0: No TX transfer on-going -1: TX transfer on-going */
#define USART_STATUS_RX_BUSY                      BIT(3)                                      /*!< RX busy status flag: -0: No RX transfer on-going -1: RX transfer on-going */
#define USART_STATUS_RX_ERROR_FLOW                BIT(4)                                      /*!< RX overflow error status flag(by writing 1 clr 0): -0: No error -1: RX overflow error occurred  */
#define USART_STATUS_RX_ERROR_FLOW_OFS            4U                                          /*!< USART STATUS: RX_ERROR_FLOW Bit Offset */
#define USART_STATUS_RX_ERROR_FLOW_VAL(regval)        (BIT(4) & ((uint32_t)(regval) << 4))        /*!< USART STATUS: RX_ERROR_FLOW Bit Value */  
#define USART_STATUS_RX_ERROR_PARITY              BIT(5)                                      /*!< RX parity error status flag(by writing 1 clr 0): -0: No error -1: RX parity error occurred  */
#define USART_STATUS_RX_ERROR_PARITY_OFS          5U                                          /*!< USART STATUS: RX_ERROR_PARITY Bit Offset */
#define USART_STATUS_RX_ERROR_PARITY_VAL(regval)      (BIT(5) & ((uint32_t)(regval) << 5))        /*!< USART STATUS: RX_ERROR_PARITY Bit Value */  
#define USART_STATUS_CTS_RISE_FLAG                BIT(6)                                      /*!< CTS rise flag:  -0: No rise change(from 0 to 1) occurred on the CTS status line -1: A rise change(from 0  to 1) occurred on the CTS status line */
#define USART_STATUS_CTS_RISE_FLAG_OFS            6U                                          /*!< USART STATUS: CTS_RISE_FLAG Bit Offset */
#define USART_STATUS_CTS_RISE_FLAG_VAL(regval)        (BIT(6) & ((uint32_t)(regval) << 6))        /*!< USART STATUS: CTS_RISE_FLAG Bit Value */  
#define USART_STATUS_CTS_FALL_FLAG                BIT(7)                                      /*!< CTS fall flag:  -1'b0: No fall change(from 1 to 0) occurred on the CTS status line -1'b1: A fall change(from 1 to 0) occurred on the CTS status line */
#define USART_STATUS_CTS_FALL_FLAG_OFS            7U                                          /*!< USART STATUS: CTS_FALL_FLAG Bit Offset */
#define USART_STATUS_CTS_FALL_FLAG_VAL(regval)        (BIT(7) & ((uint32_t)(regval) << 7))        /*!< USART STATUS: CTS_FALL_FLAG Bit Value */  

/**
  * \brief Check the USART uart_status cts_fall_flag bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cts_fall_flag bit.
  * \retval 1 This is a valid cts_fall_flag bit.
  */
#define IS_USART_UART_STATUS_CTS_FALL_FLAG(regval)          (\
                                                 )

#define USART_STATUS_STOP_BIT_ERR_FLAG            BIT(8)                                      /*!< If rx line is low level in stop_bit mode, there will be a stop_bit error status and write 1 to clear this status. */
#define USART_STATUS_STOP_BIT_ERR_FLAG_OFS        8U                                          /*!< USART STATUS: STOP_BIT_ERR_FLAG Bit Offset */
#define USART_STATUS_STOP_BIT_ERR_FLAG_VAL(regval)    (BIT(8) & ((uint32_t)(regval) << 8))        /*!< USART STATUS: STOP_BIT_ERR_FLAG Bit Value */  
#define USART_STATUS_RX_IDLE_TOUT_STATUS          BIT(12)                                      /*!< The receive timeout interrupt is asserted when the receive fifo in not empty, and no more data is receive during a RX_IDLE_TOUT_NUM. */
#define USART_STATUS_RX_IDLE_TOUT_STATUS_OFS      12U                                          /*!< USART STATUS: RX_IDLE_TOUT_STATUS Bit Offset */
#define USART_STATUS_RX_IDLE_TOUT_STATUS_VAL(regval)  (BIT(12) & ((uint32_t)(regval) << 12))        /*!< USART STATUS: RX_IDLE_TOUT_STATUS Bit Value */  
#define USART_STATUS_RX_WM_TOUT_STATUS            BIT(13)                                      /*!< The rx watermark timeout interrupt is asserted when the time of rx watermark interrupt is asseted than RX_WM_TOUT_NUM register */
#define USART_STATUS_RX_WM_TOUT_STATUS_OFS        13U                                          /*!< USART STATUS: RX_WM_TOUT_STATUS Bit Offset */
#define USART_STATUS_RX_WM_TOUT_STATUS_VAL(regval)    (BIT(13) & ((uint32_t)(regval) << 13))        /*!< USART STATUS: RX_WM_TOUT_STATUS Bit Value */  
#define USART_STATUS_FULL                         BIT(14)                                      /*!< Transmit FIFO full, the status bit is cleared when USART had fetched tx-data from transmit-fifo. */
#define USART_STATUS_EMPTY                        BIT(15)                                      /*!< Receive FIFO empty, the status bit is cleared when USART had receive first rx-data successfully. */
#define USART_STATUS_TX_EOT                       BIT(16)                                      /*!< TX transfer done status */
#define USART_STATUS_TX_EOT_OFS                   16U                                          /*!< USART STATUS: TX_EOT Bit Offset */
#define USART_STATUS_TX_EOT_VAL(regval)               (BIT(16) & ((uint32_t)(regval) << 16))        /*!< USART STATUS: TX_EOT Bit Value */  
#define USART_STATUS_RX_EOT                       BIT(17)                                      /*!< RX transfer done status */
#define USART_STATUS_RX_EOT_OFS                   17U                                          /*!< USART STATUS: RX_EOT Bit Offset */
#define USART_STATUS_RX_EOT_VAL(regval)               (BIT(17) & ((uint32_t)(regval) << 17))        /*!< USART STATUS: RX_EOT Bit Value */  
#define USART_STATUS_SPI_SLV_UNDER_RUN            BIT(18)                                      /*!< In slave transmission mode, this flag is set when the first clock pulse for data transmission appears while the software has not yet loaded any value into TX_FIFO. */
#define USART_STATUS_SPI_SLV_UNDER_RUN_OFS        18U                                          /*!< USART STATUS: SPI_SLV_UNDER_RUN Bit Offset */
#define USART_STATUS_SPI_SLV_UNDER_RUN_VAL(regval)    (BIT(18) & ((uint32_t)(regval) << 18))        /*!< USART STATUS: SPI_SLV_UNDER_RUN Bit Value */  
#define USART_STATUS_SPI_SLV_OVER_RUN             BIT(19)                                      /*!< In slave receive mode, this flag is set when the first clock pulse for data transmission appears while RX_FIFO is full already. */
#define USART_STATUS_SPI_SLV_OVER_RUN_OFS         19U                                          /*!< USART STATUS: SPI_SLV_OVER_RUN Bit Offset */
#define USART_STATUS_SPI_SLV_OVER_RUN_VAL(regval)     (BIT(19) & ((uint32_t)(regval) << 19))        /*!< USART STATUS: SPI_SLV_OVER_RUN Bit Value */  
#define USART_STATUS_CTS_PIN                      BIT(20)                                      /*!< This bit is set/reset by hardware. It is the status of the CTS input pin. -0: CTS pin is low. -1: CTS pin is high. */
#define USART_STATUS_TX_FIFO_EMPTY                BIT(21)                                      /*!< Transmit FIFO empty, the status bit is cleared when soft-ware had sent first tx-data into transmit-fifo. */
#define USART_STATUS_RX_FIFO_FULL                 BIT(22)                                      /*!< Receive FIFO full, the status bit is cleared when soft-ware had fetched rx-data from receive-fifo. */
#define USART_STATUS_FRAME_ERR_FLAG               BIT(23)                                      /*!< When transmitting data in Smartcard mode, this bit is set when the maximum number of transmit attempts is reached without success (the card NACKs the data frame).When detected break field in LIN mode,this bit will be set. -0: No Framing error is detected/No break field -1: Framing error or break character is detected */
#define USART_STATUS_FRAME_ERR_FLAG_OFS           23U                                          /*!< USART STATUS: FRAME_ERR_FLAG Bit Offset */
#define USART_STATUS_FRAME_ERR_FLAG_VAL(regval)       (BIT(23) & ((uint32_t)(regval) << 23))        /*!< USART STATUS: FRAME_ERR_FLAG Bit Value */  
#define USART_STATUS_NE_FLAG                      BIT(24)                                      /*!< Receive noise detection flag.This bit set by hardware when noise detected on a received frame(voting incomplete equality).It clear by software via writting 1 to this bit. */
#define USART_STATUS_NE_FLAG_OFS                  24U                                          /*!< USART STATUS: NE_FLAG Bit Offset */
#define USART_STATUS_NE_FLAG_VAL(regval)              (BIT(24) & ((uint32_t)(regval) << 24))        /*!< USART STATUS: NE_FLAG Bit Value */  
 
 /* ===== USART DIV Register definition ===== */
#define USART_DIV_BAUD_DIV_MASK                BITS(0,20)                                   /*!< USART DIV: BAUD_DIV Bit Mask */  
#define USART_DIV_BAUD_DIV_OFS                 0U                                          /*!< USART DIV: BAUD_DIV Bit Offset */
#define USART_DIV_BAUD_DIV(regval)             (BITS(0,20) & ((uint32_t)(regval) << 0))        /*!< USART DIV: BAUD_DIV Bit Value */  
#define USART_DIV_BAUD_DIV_0                        USART_DIV_BAUD_DIV(0)                                                   /*!< 0 */
#define USART_DIV_BAUD_DIV_0_125                    USART_DIV_BAUD_DIV(1)                                                   /*!< 0_125 */
#define USART_DIV_BAUD_DIV_0_25                     USART_DIV_BAUD_DIV(2)                                                   /*!< 0_25 */
#define USART_DIV_BAUD_DIV_0_375                    USART_DIV_BAUD_DIV(3)                                                   /*!< 0_375 */
#define USART_DIV_BAUD_DIV_0_5                      USART_DIV_BAUD_DIV(4)                                                   /*!< 0_5 */
#define USART_DIV_BAUD_DIV_0_625                    USART_DIV_BAUD_DIV(5)                                                   /*!< 0_625 */
#define USART_DIV_BAUD_DIV_0_75                     USART_DIV_BAUD_DIV(6)                                                   /*!< 0_75 */
#define USART_DIV_BAUD_DIV_0_875                    USART_DIV_BAUD_DIV(7)                                                   /*!< 0_875 */

/**
  * \brief Check the USART uart_div baud_div bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid baud_div bits.
  * \retval 1 This is a valid baud_div bits.
  */
#define IS_USART_UART_DIV_BAUD_DIV(regval)               (\
                                         ((regval) == USART_DIV_BAUD_DIV_0                   ) || \
                                         ((regval) == USART_DIV_BAUD_DIV_0_125               ) || \
                                         ((regval) == USART_DIV_BAUD_DIV_0_25                ) || \
                                         ((regval) == USART_DIV_BAUD_DIV_0_375               ) || \
                                         ((regval) == USART_DIV_BAUD_DIV_0_5                 ) || \
                                         ((regval) == USART_DIV_BAUD_DIV_0_625               ) || \
                                         ((regval) == USART_DIV_BAUD_DIV_0_75                ) || \
                                         ((regval) == USART_DIV_BAUD_DIV_0_875               )  \
                                                 )

 /* ===== USART SETUP Register definition ===== */
#define USART_SETUP_PARITY_EN                    BIT(0)                                      /*!< Parity bit generation and check configuration bitfield: -1'b0:  disable: Disabled parity bit generation and check -1'b1:  enable:Enabled parity bit generation and check */
#define USART_SETUP_PARITY_EN_OFS                0U                                          /*!< USART SETUP: PARITY_EN Bit Offset */
#define USART_SETUP_PARITY_EN_VAL(regval)            (BIT(0) & ((uint32_t)(regval) << 0))        /*!< USART SETUP: PARITY_EN Bit Value */  
#define USART_SETUP_PARITY_EN_DISABLE                 0x0UL                                               /*!< DISABLE */
#define USART_SETUP_PARITY_EN_ENABLE                  BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_setup parity_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid parity_en bit.
  * \retval 1 This is a valid parity_en bit.
  */
#define IS_USART_UART_SETUP_PARITY_EN(regval)              (\
                                        ((regval) == USART_SETUP_PARITY_EN_DISABLE             ) || \
                                        ((regval) == USART_SETUP_PARITY_EN_ENABLE              )  \
                                                 )

#define USART_SETUP_PARITY_SEL                   BIT(1)                                      /*!< Parity selection -1'b0:  disable:even parity -1'b1:  enable:odd parity */
#define USART_SETUP_PARITY_SEL_OFS               1U                                          /*!< USART SETUP: PARITY_SEL Bit Offset */
#define USART_SETUP_PARITY_SEL_VAL(regval)           (BIT(1) & ((uint32_t)(regval) << 1))        /*!< USART SETUP: PARITY_SEL Bit Value */  
#define USART_SETUP_PARITY_SEL_DISABLE                0x0UL                                               /*!< DISABLE */
#define USART_SETUP_PARITY_SEL_ENABLE                 BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_setup parity_sel bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid parity_sel bit.
  * \retval 1 This is a valid parity_sel bit.
  */
#define IS_USART_UART_SETUP_PARITY_SEL(regval)             (\
                                       ((regval) == USART_SETUP_PARITY_SEL_DISABLE             ) || \
                                       ((regval) == USART_SETUP_PARITY_SEL_ENABLE              )  \
                                                 )

#define USART_SETUP_SEPARATE_EN                  BIT(2)                                      /*!< Enable this bit can make tx and rx worked in cpu or dma mode independently -1'b0:  disable:Both tx and rx is controled by the DMA_EN bit in USART_SETUP[15] register -1'b1:  enable:The rx control by RX_DMA_EN bit USART_SETUP[20] in register, and tx control by DMA_EN in USART_SETUP[15] Register */
#define USART_SETUP_SEPARATE_EN_OFS              2U                                          /*!< USART SETUP: SEPARATE_EN Bit Offset */
#define USART_SETUP_SEPARATE_EN_VAL(regval)          (BIT(2) & ((uint32_t)(regval) << 2))        /*!< USART SETUP: SEPARATE_EN Bit Value */  
#define USART_SETUP_SEPARATE_EN_DISABLE               0x0UL                                               /*!< DISABLE */
#define USART_SETUP_SEPARATE_EN_ENABLE                BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_setup separate_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid separate_en bit.
  * \retval 1 This is a valid separate_en bit.
  */
#define IS_USART_UART_SETUP_SEPARATE_EN(regval)            (\
                                      ((regval) == USART_SETUP_SEPARATE_EN_DISABLE             ) || \
                                      ((regval) == USART_SETUP_SEPARATE_EN_ENABLE              )  \
                                                 )

#define USART_SETUP_BIT_LENGTH_MASK              BITS(4,6)                                   /*!< USART SETUP: BIT_LENGTH Bit Mask */  
#define USART_SETUP_BIT_LENGTH_OFS               4U                                          /*!< USART SETUP: BIT_LENGTH Bit Offset */
#define USART_SETUP_BIT_LENGTH(regval)           (BITS(4,6) & ((uint32_t)(regval) << 4))        /*!< USART SETUP: BIT_LENGTH Bit Value */  
#define USART_SETUP_BIT_LENGTH_5                      USART_SETUP_BIT_LENGTH(0)                                                 /*!< 5 */
#define USART_SETUP_BIT_LENGTH_6                      USART_SETUP_BIT_LENGTH(1)                                                 /*!< 6 */
#define USART_SETUP_BIT_LENGTH_7                      USART_SETUP_BIT_LENGTH(2)                                                 /*!< 7 */
#define USART_SETUP_BIT_LENGTH_8                      USART_SETUP_BIT_LENGTH(3)                                                 /*!< 8 */
#define USART_SETUP_BIT_LENGTH_9                      USART_SETUP_BIT_LENGTH(4)                                                 /*!< 9 */

/**
  * \brief Check the USART uart_setup bit_length bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid bit_length bits.
  * \retval 1 This is a valid bit_length bits.
  */
#define IS_USART_UART_SETUP_BIT_LENGTH(regval)             (\
                                       ((regval) == USART_SETUP_BIT_LENGTH_5                   ) || \
                                       ((regval) == USART_SETUP_BIT_LENGTH_6                   ) || \
                                       ((regval) == USART_SETUP_BIT_LENGTH_7                   ) || \
                                       ((regval) == USART_SETUP_BIT_LENGTH_8                   ) || \
                                       ((regval) == USART_SETUP_BIT_LENGTH_9                   )  \
                                                 )

#define USART_SETUP_CLK_EN                       BIT(7)                                      /*!< Clock enable: this bit allows the user to enable the CK pin -1'b0:  disable:CK pin disabled -1'b1:  enable:CK pin enabled Only when TX mode and clk_en =1 ,tx_clk_o can output. */
#define USART_SETUP_CLK_EN_OFS                   7U                                          /*!< USART SETUP: CLK_EN Bit Offset */
#define USART_SETUP_CLK_EN_VAL(regval)               (BIT(7) & ((uint32_t)(regval) << 7))        /*!< USART SETUP: CLK_EN Bit Value */  
#define USART_SETUP_CLK_EN_DISABLE                    0x0UL                                               /*!< DISABLE */
#define USART_SETUP_CLK_EN_ENABLE                     BIT(7)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_setup clk_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid clk_en bit.
  * \retval 1 This is a valid clk_en bit.
  */
#define IS_USART_UART_SETUP_CLK_EN(regval)                 (\
                                           ((regval) == USART_SETUP_CLK_EN_DISABLE             ) || \
                                           ((regval) == USART_SETUP_CLK_EN_ENABLE              )  \
                                                 )

#define USART_SETUP_CPOL                         BIT(8)                                      /*!< Clock polarity: this bit allows the user to select the polarity of the clock output on the CK pin in synchronous mode.  It works in conjunction with CPHA bit to produce the desired clock/data -1'b0:  disable:Steady LOW: Steady low value on CK pin outside transmission window. -1'b1:  enable:Steady HIGH: Steady high value on CK pin outside transmission window. */
#define USART_SETUP_CPOL_OFS                     8U                                          /*!< USART SETUP: CPOL Bit Offset */
#define USART_SETUP_CPOL_VAL(regval)                 (BIT(8) & ((uint32_t)(regval) << 8))        /*!< USART SETUP: CPOL Bit Value */  
#define USART_SETUP_CPOL_DISABLE                      0x0UL                                               /*!< DISABLE */
#define USART_SETUP_CPOL_ENABLE                       BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_setup cpol bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cpol bit.
  * \retval 1 This is a valid cpol bit.
  */
#define IS_USART_UART_SETUP_CPOL(regval)                   (\
                                             ((regval) == USART_SETUP_CPOL_DISABLE             ) || \
                                             ((regval) == USART_SETUP_CPOL_ENABLE              )  \
                                                 )

#define USART_SETUP_CPHA                         BIT(9)                                      /*!< Clock phase: this bit allows the user to select the phase of the clock output on the CK pin in synchronous mode.  It works in conjunction with CPOL bit to produce the desired clock/data relationship -1'b0:  disable:First transition: The first clock transition is the first data caputure edge. -1'b1:  enable: Second transition:The scond clock transition is the first data caputre edge */
#define USART_SETUP_CPHA_OFS                     9U                                          /*!< USART SETUP: CPHA Bit Offset */
#define USART_SETUP_CPHA_VAL(regval)                 (BIT(9) & ((uint32_t)(regval) << 9))        /*!< USART SETUP: CPHA Bit Value */  
#define USART_SETUP_CPHA_DISABLE                      0x0UL                                               /*!< DISABLE */
#define USART_SETUP_CPHA_ENABLE                       BIT(9)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_setup cpha bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cpha bit.
  * \retval 1 This is a valid cpha bit.
  */
#define IS_USART_UART_SETUP_CPHA(regval)                   (\
                                             ((regval) == USART_SETUP_CPHA_DISABLE             ) || \
                                             ((regval) == USART_SETUP_CPHA_ENABLE              )  \
                                                 )

#define USART_SETUP_LBCL                         BIT(10)                                      /*!< Last bit clock pulse: this bit allows the user to select whether the clock pulse assocaited with the  Last data bit transmitted(MSB) has to be output on the CK pin in synchronous mode. -1'b0:  disable:Not output: The clock pulse of the last data bit is not output to the CK pin -1'b1:  enable:Output: The clock pulse of the last data bit is output to the CK pin */
#define USART_SETUP_LBCL_OFS                     10U                                          /*!< USART SETUP: LBCL Bit Offset */
#define USART_SETUP_LBCL_VAL(regval)                 (BIT(10) & ((uint32_t)(regval) << 10))        /*!< USART SETUP: LBCL Bit Value */  
#define USART_SETUP_LBCL_DISABLE                      0x0UL                                               /*!< DISABLE */
#define USART_SETUP_LBCL_ENABLE                       BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup lbcl bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid lbcl bit.
  * \retval 1 This is a valid lbcl bit.
  */
#define IS_USART_UART_SETUP_LBCL(regval)                   (\
                                             ((regval) == USART_SETUP_LBCL_DISABLE             ) || \
                                             ((regval) == USART_SETUP_LBCL_ENABLE              )  \
                                                 )

#define USART_SETUP_HDSEL                        BIT(11)                                      /*!< HDSEL: simplex model selection -1'b0:  disable:Disable: simplex model is not selected -1'b1:  enable:Enable: simplex model is selected */
#define USART_SETUP_HDSEL_OFS                    11U                                          /*!< USART SETUP: HDSEL Bit Offset */
#define USART_SETUP_HDSEL_VAL(regval)                (BIT(11) & ((uint32_t)(regval) << 11))        /*!< USART SETUP: HDSEL Bit Value */  
#define USART_SETUP_HDSEL_DISABLE                     0x0UL                                               /*!< DISABLE */
#define USART_SETUP_HDSEL_ENABLE                      BIT(11)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup hdsel bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid hdsel bit.
  * \retval 1 This is a valid hdsel bit.
  */
#define IS_USART_UART_SETUP_HDSEL(regval)                  (\
                                            ((regval) == USART_SETUP_HDSEL_DISABLE             ) || \
                                            ((regval) == USART_SETUP_HDSEL_ENABLE              )  \
                                                 )

#define USART_SETUP_SMARTCARD_EN                 BIT(12)                                      /*!< Smartcard mode enable This bit is used for enabling Smartcard mode. -1'b0:  disable:Smartcard Mode disabled -1'b1:  enable:Smartcard Mode enabled */
#define USART_SETUP_SMARTCARD_EN_OFS             12U                                          /*!< USART SETUP: SMARTCARD_EN Bit Offset */
#define USART_SETUP_SMARTCARD_EN_VAL(regval)         (BIT(12) & ((uint32_t)(regval) << 12))        /*!< USART SETUP: SMARTCARD_EN Bit Value */  
#define USART_SETUP_SMARTCARD_EN_DISABLE              0x0UL                                               /*!< DISABLE */
#define USART_SETUP_SMARTCARD_EN_ENABLE               BIT(12)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup smartcard_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid smartcard_en bit.
  * \retval 1 This is a valid smartcard_en bit.
  */
#define IS_USART_UART_SETUP_SMARTCARD_EN(regval)           (\
                                     ((regval) == USART_SETUP_SMARTCARD_EN_DISABLE             ) || \
                                     ((regval) == USART_SETUP_SMARTCARD_EN_ENABLE              )  \
                                                 )

#define USART_SETUP_RTSE                         BIT(13)                                      /*!< RTS enable: -1'b0:  disable:RTS hardware flow control disabled -1'b1:  enable:RTS mode enable, data is only requested when there is space in the RX FIFO,  The transmission of data is expected to cease after the current character has been transmitted.  The RTS output is asserted(tie to 0)when a data can be received. */
#define USART_SETUP_RTSE_OFS                     13U                                          /*!< USART SETUP: RTSE Bit Offset */
#define USART_SETUP_RTSE_VAL(regval)                 (BIT(13) & ((uint32_t)(regval) << 13))        /*!< USART SETUP: RTSE Bit Value */  
#define USART_SETUP_RTSE_DISABLE                      0x0UL                                               /*!< DISABLE */
#define USART_SETUP_RTSE_ENABLE                       BIT(13)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup rtse bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rtse bit.
  * \retval 1 This is a valid rtse bit.
  */
#define IS_USART_UART_SETUP_RTSE(regval)                   (\
                                             ((regval) == USART_SETUP_RTSE_DISABLE             ) || \
                                             ((regval) == USART_SETUP_RTSE_ENABLE              )  \
                                                 )

#define USART_SETUP_CTSE                         BIT(14)                                      /*!< CTS enable: -1'b0:  disable:CTS hardware flow control disable -1'b1:  enable:CTS mode enable, data is only transmitted when the CTS input is asserted(tied to 0).  If the CTS input is deasserted while a data is being transmitted, the the transmission is complete before stopping.  If a data is written into the RX FIFO while CTS is deasserted, the transmission is postponed until CTS is asserted. */
#define USART_SETUP_CTSE_OFS                     14U                                          /*!< USART SETUP: CTSE Bit Offset */
#define USART_SETUP_CTSE_VAL(regval)                 (BIT(14) & ((uint32_t)(regval) << 14))        /*!< USART SETUP: CTSE Bit Value */  
#define USART_SETUP_CTSE_DISABLE                      0x0UL                                               /*!< DISABLE */
#define USART_SETUP_CTSE_ENABLE                       BIT(14)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup ctse bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ctse bit.
  * \retval 1 This is a valid ctse bit.
  */
#define IS_USART_UART_SETUP_CTSE(regval)                   (\
                                             ((regval) == USART_SETUP_CTSE_DISABLE             ) || \
                                             ((regval) == USART_SETUP_CTSE_ENABLE              )  \
                                                 )

#define USART_SETUP_DMA_EN                       BIT(15)                                      /*!< DMA_EN:select mode  -1'b0:  disable:cpu mode, tx data come from icb or other bus  -1'b1:  enable:dma mode, tx data come frome dma Note: this bit control rx channel is based on SEPARATE_EN register, when SEPARATE_EN is enable, this bit is used for tx-channle only */
#define USART_SETUP_DMA_EN_OFS                   15U                                          /*!< USART SETUP: DMA_EN Bit Offset */
#define USART_SETUP_DMA_EN_VAL(regval)               (BIT(15) & ((uint32_t)(regval) << 15))        /*!< USART SETUP: DMA_EN Bit Value */  
#define USART_SETUP_DMA_EN_DISABLE                    0x0UL                                               /*!< DISABLE */
#define USART_SETUP_DMA_EN_ENABLE                     BIT(15)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup dma_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dma_en bit.
  * \retval 1 This is a valid dma_en bit.
  */
#define IS_USART_UART_SETUP_DMA_EN(regval)                 (\
                                           ((regval) == USART_SETUP_DMA_EN_DISABLE             ) || \
                                           ((regval) == USART_SETUP_DMA_EN_ENABLE              )  \
                                                 )

#define USART_SETUP_VOTE_MAJORITY_EN             BIT(16)                                      /*!< Enable the 2/3 majority logic in data frame -1'b0:  disable:Turn OFF 2/3 majority logic -1'b1:  enable:Turn ON 2/3 majority logic */
#define USART_SETUP_VOTE_MAJORITY_EN_OFS         16U                                          /*!< USART SETUP: VOTE_MAJORITY_EN Bit Offset */
#define USART_SETUP_VOTE_MAJORITY_EN_VAL(regval)     (BIT(16) & ((uint32_t)(regval) << 16))        /*!< USART SETUP: VOTE_MAJORITY_EN Bit Value */  
#define USART_SETUP_VOTE_MAJORITY_EN_DISABLE             0x0UL                                               /*!< DISABLE */
#define USART_SETUP_VOTE_MAJORITY_EN_ENABLE             BIT(16)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup vote_majority_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid vote_majority_en bit.
  * \retval 1 This is a valid vote_majority_en bit.
  */
#define IS_USART_UART_SETUP_VOTE_MAJORITY_EN(regval)       (\
                                 ((regval) == USART_SETUP_VOTE_MAJORITY_EN_DISABLE             ) || \
                                 ((regval) == USART_SETUP_VOTE_MAJORITY_EN_ENABLE              )  \
                                                 )

#define USART_SETUP_SAMPLE_POSITION_MASK         BITS(17,18)                                   /*!< USART SETUP: SAMPLE_POSITION Bit Mask */  
#define USART_SETUP_SAMPLE_POSITION_OFS          17U                                          /*!< USART SETUP: SAMPLE_POSITION Bit Offset */
#define USART_SETUP_SAMPLE_POSITION(regval)      (BITS(17,18) & ((uint32_t)(regval) << 17))        /*!< USART SETUP: SAMPLE_POSITION Bit Value */  
#define USART_SETUP_SAMPLE_POSITION_DITHER             USART_SETUP_SAMPLE_POSITION(0)                                                 /*!< DITHER */
#define USART_SETUP_SAMPLE_POSITION_DOWNWARD             USART_SETUP_SAMPLE_POSITION(1)                                                 /*!< DOWNWARD */
#define USART_SETUP_SAMPLE_POSITION_UPWARD             USART_SETUP_SAMPLE_POSITION(2)                                                 /*!< UPWARD */

/**
  * \brief Check the USART uart_setup sample_position bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid sample_position bits.
  * \retval 1 This is a valid sample_position bits.
  */
#define IS_USART_UART_SETUP_SAMPLE_POSITION(regval)        (\
                                  ((regval) == USART_SETUP_SAMPLE_POSITION_DITHER              ) || \
                                  ((regval) == USART_SETUP_SAMPLE_POSITION_DOWNWARD            ) || \
                                  ((regval) == USART_SETUP_SAMPLE_POSITION_UPWARD              )  \
                                                 )

#define USART_SETUP_FRACTION_BAUD                BIT(19)                                      /*!< Fractional baud rate -1'b0:  disable:turn off fraction baud generator -1'b1:  enable:turn on fraction baud generator */
#define USART_SETUP_FRACTION_BAUD_OFS            19U                                          /*!< USART SETUP: FRACTION_BAUD Bit Offset */
#define USART_SETUP_FRACTION_BAUD_VAL(regval)        (BIT(19) & ((uint32_t)(regval) << 19))        /*!< USART SETUP: FRACTION_BAUD Bit Value */  
#define USART_SETUP_FRACTION_BAUD_DISABLE             0x0UL                                               /*!< DISABLE */
#define USART_SETUP_FRACTION_BAUD_ENABLE              BIT(19)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup fraction_baud bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid fraction_baud bit.
  * \retval 1 This is a valid fraction_baud bit.
  */
#define IS_USART_UART_SETUP_FRACTION_BAUD(regval)          (\
                                    ((regval) == USART_SETUP_FRACTION_BAUD_DISABLE             ) || \
                                    ((regval) == USART_SETUP_FRACTION_BAUD_ENABLE              )  \
                                                 )

#define USART_SETUP_RX_DMA_EN                    BIT(20)                                      /*!< Rx channel dma mode enable, when SEPARATE_EN bit is enable, this bit is assert or de-asserted by soft-ware;   when SEPARATE_EN bit is disable, this bit is assert or de-asserted by register SETUP[15].   -1'b0:  disable:cpu mode -1'b1:  enable:dma mode */
#define USART_SETUP_RX_DMA_EN_OFS                20U                                          /*!< USART SETUP: RX_DMA_EN Bit Offset */
#define USART_SETUP_RX_DMA_EN_VAL(regval)            (BIT(20) & ((uint32_t)(regval) << 20))        /*!< USART SETUP: RX_DMA_EN Bit Value */  
#define USART_SETUP_RX_DMA_EN_DISABLE                 0x0UL                                               /*!< DISABLE */
#define USART_SETUP_RX_DMA_EN_ENABLE                  BIT(20)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup rx_dma_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_dma_en bit.
  * \retval 1 This is a valid rx_dma_en bit.
  */
#define IS_USART_UART_SETUP_RX_DMA_EN(regval)              (\
                                        ((regval) == USART_SETUP_RX_DMA_EN_DISABLE             ) || \
                                        ((regval) == USART_SETUP_RX_DMA_EN_ENABLE              )  \
                                                 )

#define USART_SETUP_RS485_EN                     BIT(21)                                      /*!< RS-485 mode enable -1'b0:  disable:disable RS-485 mode -1'b1:  enable: enable RS-485 mode */
#define USART_SETUP_RS485_EN_OFS                 21U                                          /*!< USART SETUP: RS485_EN Bit Offset */
#define USART_SETUP_RS485_EN_VAL(regval)             (BIT(21) & ((uint32_t)(regval) << 21))        /*!< USART SETUP: RS485_EN Bit Value */  
#define USART_SETUP_RS485_EN_DISABLE                  0x0UL                                               /*!< DISABLE */
#define USART_SETUP_RS485_EN_ENABLE                   BIT(21)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup rs485_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rs485_en bit.
  * \retval 1 This is a valid rs485_en bit.
  */
#define IS_USART_UART_SETUP_RS485_EN(regval)               (\
                                         ((regval) == USART_SETUP_RS485_EN_DISABLE             ) || \
                                         ((regval) == USART_SETUP_RS485_EN_ENABLE              )  \
                                                 )

#define USART_SETUP_DE_ASSERTION_EARLY_EN         BIT(22)                                      /*!< Enable RS-485's DE signal assert before START condition one bit-time early -1'b0:  disable: DE will assert when START condition happened -1'b1:  enable: DE will assert before START condition one bit-time early */
#define USART_SETUP_DE_ASSERTION_EARLY_EN_OFS     22U                                          /*!< USART SETUP: DE_ASSERTION_EARLY_EN Bit Offset */
#define USART_SETUP_DE_ASSERTION_EARLY_EN_VAL(regval) (BIT(22) & ((uint32_t)(regval) << 22))        /*!< USART SETUP: DE_ASSERTION_EARLY_EN Bit Value */  
#define USART_SETUP_DE_ASSERTION_EARLY_EN_DISABLE             0x0UL                                               /*!< DISABLE */
#define USART_SETUP_DE_ASSERTION_EARLY_EN_ENABLE             BIT(22)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup de_assertion_early_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid de_assertion_early_en bit.
  * \retval 1 This is a valid de_assertion_early_en bit.
  */
#define IS_USART_UART_SETUP_DE_ASSERTION_EARLY_EN(regval)   (\
                             ((regval) == USART_SETUP_DE_ASSERTION_EARLY_EN_DISABLE             ) || \
                             ((regval) == USART_SETUP_DE_ASSERTION_EARLY_EN_ENABLE              )  \
                                                 )

#define USART_SETUP_DE_DEASSERTION_DELAY_EN         BIT(23)                                      /*!< Enable RS-485's DE signal dessert after STOP condition one bit-time later -1'b0:  disable: DE will assert when STOP condition happened -1'b1:  enable: DE will assert after STOP condition one bit-time later */
#define USART_SETUP_DE_DEASSERTION_DELAY_EN_OFS     23U                                          /*!< USART SETUP: DE_DEASSERTION_DELAY_EN Bit Offset */
#define USART_SETUP_DE_DEASSERTION_DELAY_EN_VAL(regval) (BIT(23) & ((uint32_t)(regval) << 23))        /*!< USART SETUP: DE_DEASSERTION_DELAY_EN Bit Value */  
#define USART_SETUP_DE_DEASSERTION_DELAY_EN_DISABLE             0x0UL                                               /*!< DISABLE */
#define USART_SETUP_DE_DEASSERTION_DELAY_EN_ENABLE             BIT(23)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup de_deassertion_delay_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid de_deassertion_delay_en bit.
  * \retval 1 This is a valid de_deassertion_delay_en bit.
  */
#define IS_USART_UART_SETUP_DE_DEASSERTION_DELAY_EN(regval)   (\
                             ((regval) == USART_SETUP_DE_DEASSERTION_DELAY_EN_DISABLE             ) || \
                             ((regval) == USART_SETUP_DE_DEASSERTION_DELAY_EN_ENABLE              )  \
                                                 )

#define USART_SETUP_DE_INV                       BIT(24)                                      /*!< Enable RS-485's DE signal polarity -1'b0:  disable: DE pin signal works using the standard logic levels (VDD =1/idle, Gnd=0/mark) -1'b1:  enable: DE pin signal values are inverted. ((VDD =0/mark, Gnd=1/idle). */
#define USART_SETUP_DE_INV_OFS                   24U                                          /*!< USART SETUP: DE_INV Bit Offset */
#define USART_SETUP_DE_INV_VAL(regval)               (BIT(24) & ((uint32_t)(regval) << 24))        /*!< USART SETUP: DE_INV Bit Value */  
#define USART_SETUP_DE_INV_DISABLE                    0x0UL                                               /*!< DISABLE */
#define USART_SETUP_DE_INV_ENABLE                     BIT(24)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup de_inv bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid de_inv bit.
  * \retval 1 This is a valid de_inv bit.
  */
#define IS_USART_UART_SETUP_DE_INV(regval)                 (\
                                           ((regval) == USART_SETUP_DE_INV_DISABLE             ) || \
                                           ((regval) == USART_SETUP_DE_INV_ENABLE              )  \
                                                 )

#define USART_SETUP_MSBFIRST                     BIT(25)                                      /*!< Most significant bit first. This bit is set and cleared by software. -1'b0:  disable: data is transmitted/received with data bit 0 first, following the start bit. -1'b1:  enable: data is transmitted/received with the MSB (bit 7/8) first, following the start bit. */
#define USART_SETUP_MSBFIRST_OFS                 25U                                          /*!< USART SETUP: MSBFIRST Bit Offset */
#define USART_SETUP_MSBFIRST_VAL(regval)             (BIT(25) & ((uint32_t)(regval) << 25))        /*!< USART SETUP: MSBFIRST Bit Value */  
#define USART_SETUP_MSBFIRST_DISABLE                  0x0UL                                               /*!< DISABLE */
#define USART_SETUP_MSBFIRST_ENABLE                   BIT(25)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup msbfirst bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid msbfirst bit.
  * \retval 1 This is a valid msbfirst bit.
  */
#define IS_USART_UART_SETUP_MSBFIRST(regval)               (\
                                         ((regval) == USART_SETUP_MSBFIRST_DISABLE             ) || \
                                         ((regval) == USART_SETUP_MSBFIRST_ENABLE              )  \
                                                 )

#define USART_SETUP_DATAINV                      BIT(26)                                      /*!< Binary data inversion. This bit is set and cleared by software. -1'b0:  disable: Logical data from the data register are send/received in positive/direct logic. (1=H, 0=L) -1'b1:  enable: Logical data from the data register are send/received in negative/inverse logic. (1=L, 0=H). The parity bit is also inverted. */
#define USART_SETUP_DATAINV_OFS                  26U                                          /*!< USART SETUP: DATAINV Bit Offset */
#define USART_SETUP_DATAINV_VAL(regval)              (BIT(26) & ((uint32_t)(regval) << 26))        /*!< USART SETUP: DATAINV Bit Value */  
#define USART_SETUP_DATAINV_DISABLE                   0x0UL                                               /*!< DISABLE */
#define USART_SETUP_DATAINV_ENABLE                    BIT(26)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup datainv bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid datainv bit.
  * \retval 1 This is a valid datainv bit.
  */
#define IS_USART_UART_SETUP_DATAINV(regval)                (\
                                          ((regval) == USART_SETUP_DATAINV_DISABLE             ) || \
                                          ((regval) == USART_SETUP_DATAINV_ENABLE              )  \
                                                 )

#define USART_SETUP_TXINV                        BIT(27)                                      /*!< TX pin active level inversion. This bit is set and cleared by software. -1'b0:  disable: TX pin signal works using the standard logic levels (VDD =1/idle, Gnd=0/mark) -1'b1:  enable: TX pin signal values are inverted. ((VDD =0/mark, Gnd=1/idle). */
#define USART_SETUP_TXINV_OFS                    27U                                          /*!< USART SETUP: TXINV Bit Offset */
#define USART_SETUP_TXINV_VAL(regval)                (BIT(27) & ((uint32_t)(regval) << 27))        /*!< USART SETUP: TXINV Bit Value */  
#define USART_SETUP_TXINV_DISABLE                     0x0UL                                               /*!< DISABLE */
#define USART_SETUP_TXINV_ENABLE                      BIT(27)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup txinv bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid txinv bit.
  * \retval 1 This is a valid txinv bit.
  */
#define IS_USART_UART_SETUP_TXINV(regval)                  (\
                                            ((regval) == USART_SETUP_TXINV_DISABLE             ) || \
                                            ((regval) == USART_SETUP_TXINV_ENABLE              )  \
                                                 )

#define USART_SETUP_RXINV                        BIT(28)                                      /*!< RX pin active level inversion. This bit is set and cleared by software. -1'b0:  disable: RX pin signal works using the standard logic levels (VDD =1/idle, Gnd=0/mark) -1'b1:  enable: RX pin signal values are inverted. ((VDD =0/mark, Gnd=1/idle). */
#define USART_SETUP_RXINV_OFS                    28U                                          /*!< USART SETUP: RXINV Bit Offset */
#define USART_SETUP_RXINV_VAL(regval)                (BIT(28) & ((uint32_t)(regval) << 28))        /*!< USART SETUP: RXINV Bit Value */  
#define USART_SETUP_RXINV_DISABLE                     0x0UL                                               /*!< DISABLE */
#define USART_SETUP_RXINV_ENABLE                      BIT(28)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup rxinv bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rxinv bit.
  * \retval 1 This is a valid rxinv bit.
  */
#define IS_USART_UART_SETUP_RXINV(regval)                  (\
                                            ((regval) == USART_SETUP_RXINV_DISABLE             ) || \
                                            ((regval) == USART_SETUP_RXINV_ENABLE              )  \
                                                 )

#define USART_SETUP_SWAP                         BIT(29)                                      /*!< Swap TX/RX pins. This bit is set and cleared by software. -1'b0:  disable: TX/RX pins are used as defined in standard pinout -1'b1:  enable: The TX and RX pins functions are swapped. This enables to work in the case of a cross-wired connection to another UART. */
#define USART_SETUP_SWAP_OFS                     29U                                          /*!< USART SETUP: SWAP Bit Offset */
#define USART_SETUP_SWAP_VAL(regval)                 (BIT(29) & ((uint32_t)(regval) << 29))        /*!< USART SETUP: SWAP Bit Value */  
#define USART_SETUP_SWAP_DISABLE                      0x0UL                                               /*!< DISABLE */
#define USART_SETUP_SWAP_ENABLE                       BIT(29)                                                  /*!< ENABLE */

/**
  * \brief Check the USART uart_setup swap bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid swap bit.
  * \retval 1 This is a valid swap bit.
  */
#define IS_USART_UART_SETUP_SWAP(regval)                   (\
                                             ((regval) == USART_SETUP_SWAP_DISABLE             ) || \
                                             ((regval) == USART_SETUP_SWAP_ENABLE              )  \
                                                 )

#define USART_SETUP_CFG_STOP_BIT_MASK            BITS(30,31)                                   /*!< USART SETUP: CFG_STOP_BIT Bit Mask */  
#define USART_SETUP_CFG_STOP_BIT_OFS             30U                                          /*!< USART SETUP: CFG_STOP_BIT Bit Offset */
#define USART_SETUP_CFG_STOP_BIT(regval)         (BITS(30,31) & ((uint32_t)(regval) << 30))        /*!< USART SETUP: CFG_STOP_BIT Bit Value */  
#define USART_SETUP_CFG_STOP_BIT_0_5BIT               USART_SETUP_CFG_STOP_BIT(0)                                                 /*!< 0_5BIT */
#define USART_SETUP_CFG_STOP_BIT_1BIT                 USART_SETUP_CFG_STOP_BIT(1)                                                 /*!< 1BIT */
#define USART_SETUP_CFG_STOP_BIT_1_5BIT               USART_SETUP_CFG_STOP_BIT(2)                                                 /*!< 1_5BIT */
#define USART_SETUP_CFG_STOP_BIT_2BIT                 USART_SETUP_CFG_STOP_BIT(3)                                                 /*!< 2BIT */

/**
  * \brief Check the USART uart_setup cfg_stop_bit bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid cfg_stop_bit bits.
  * \retval 1 This is a valid cfg_stop_bit bits.
  */
#define IS_USART_UART_SETUP_CFG_STOP_BIT(regval)           (\
                                     ((regval) == USART_SETUP_CFG_STOP_BIT_0_5BIT              ) || \
                                     ((regval) == USART_SETUP_CFG_STOP_BIT_1BIT                ) || \
                                     ((regval) == USART_SETUP_CFG_STOP_BIT_1_5BIT              ) || \
                                     ((regval) == USART_SETUP_CFG_STOP_BIT_2BIT                )  \
                                                 )

 /* ===== USART RX_SIZE Register definition ===== */
#define USART_RX_SIZE_NUM_MASK                     BITS(0,19)                                   /*!< USART RX SIZE: NUM Bit Mask */  
#define USART_RX_SIZE_NUM_OFS                      0U                                          /*!< USART RX SIZE: NUM Bit Offset */
#define USART_RX_SIZE_NUM(regval)                  (BITS(0,19) & ((uint32_t)(regval) << 0))        /*!< USART RX SIZE: NUM Bit Value */  
 
 /* ===== USART TX_SIZE Register definition ===== */
#define USART_TX_SIZE_NUM_MASK                     BITS(0,19)                                   /*!< USART TX SIZE: NUM Bit Mask */  
#define USART_TX_SIZE_NUM_OFS                      0U                                          /*!< USART TX SIZE: NUM Bit Offset */
#define USART_TX_SIZE_NUM(regval)                  (BITS(0,19) & ((uint32_t)(regval) << 0))        /*!< USART TX SIZE: NUM Bit Value */  
 
 /* ===== USART SPI_SLAVE Register definition ===== */
#define USART_SPI_SLAVE_SLV_EN                       BIT(0)                                      /*!< Synchronous Slave mode enable -1'b0:  disable: Disable synchronous slave mode. -1'b1:  enable: Enable synchronous slave mode. */
#define USART_SPI_SLAVE_SLV_EN_OFS                   0U                                          /*!< USART SPI SLAVE: SLV_EN Bit Offset */
#define USART_SPI_SLAVE_SLV_EN_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< USART SPI SLAVE: SLV_EN Bit Value */  
#define USART_SPI_SLAVE_SLV_EN_DISABLE                    0x0UL                                           /*!< DISABLE */
#define USART_SPI_SLAVE_SLV_EN_ENABLE                     BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_spi_slave slv_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid slv_en bit.
  * \retval 1 This is a valid slv_en bit.
  */
#define IS_USART_UART_SPI_SLAVE_SLV_EN(regval)                 (\
                                           ((regval) == USART_SPI_SLAVE_SLV_EN_DISABLE             ) || \
                                           ((regval) == USART_SPI_SLAVE_SLV_EN_ENABLE              )  \
                                                 )

#define USART_SPI_SLAVE_DIS_NSS                      BIT(1)                                      /*!< When the DIS_NSS bit is set, the NSS pin input is ignored. -1'b0:  disable: SPI slave selection depends on NSS input pin. -1'b1:  enable: SPI slave is always selected and NSS input pin is ignored. Note: The NSS and CTS share same pad. */
#define USART_SPI_SLAVE_DIS_NSS_OFS                  1U                                          /*!< USART SPI SLAVE: DIS_NSS Bit Offset */
#define USART_SPI_SLAVE_DIS_NSS_VAL(regval)              (BIT(1) & ((uint32_t)(regval) << 1))        /*!< USART SPI SLAVE: DIS_NSS Bit Value */  
#define USART_SPI_SLAVE_DIS_NSS_DISABLE                   0x0UL                                           /*!< DISABLE */
#define USART_SPI_SLAVE_DIS_NSS_ENABLE                    BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the USART uart_spi_slave dis_nss bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dis_nss bit.
  * \retval 1 This is a valid dis_nss bit.
  */
#define IS_USART_UART_SPI_SLAVE_DIS_NSS(regval)                (\
                                          ((regval) == USART_SPI_SLAVE_DIS_NSS_DISABLE             ) || \
                                          ((regval) == USART_SPI_SLAVE_DIS_NSS_ENABLE              )  \
                                                 )

 /* ===== USART RX_IDLE Register definition ===== */
#define USART_RX_IDLE_IDLE_TOUT_NUM_MASK           BITS(0,31)                                   /*!< USART RX IDLE: IDLE_TOUT_NUM Bit Mask */  
#define USART_RX_IDLE_IDLE_TOUT_NUM_OFS            0U                                          /*!< USART RX IDLE: IDLE_TOUT_NUM Bit Offset */
#define USART_RX_IDLE_IDLE_TOUT_NUM(regval)        (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< USART RX IDLE: IDLE_TOUT_NUM Bit Value */  
 
 /* ===== USART RX_WM Register definition ===== */
#define USART_RX_WM_WM_TOUT_NUM_MASK             BITS(0,31)                                   /*!< USART RX WM: WM_TOUT_NUM Bit Mask */  
#define USART_RX_WM_WM_TOUT_NUM_OFS              0U                                          /*!< USART RX WM: WM_TOUT_NUM Bit Offset */
#define USART_RX_WM_WM_TOUT_NUM(regval)          (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< USART RX WM: WM_TOUT_NUM Bit Value */  
 
 /* ===== USART RX_FIFO_LEFT_ENTRY Register definition ===== */
#define USART_RX_FIFO_LEFT_ENTRY_LEFT_ENTRY                   BITS(0,4)                
 
 /* ===== USART TX_FIFO_LEFT_ENTRY Register definition ===== */
#define USART_TX_FIFO_LEFT_ENTRY_LEFT_ENTRY                   BITS(0,4)                
 
 /* ===== USART TX_DATASIZE Register definition ===== */
#define USART_TX_DATASIZE_NUM_MASK                     BITS(0,1)                                   /*!< USART TX DATASIZE: NUM Bit Mask */  
#define USART_TX_DATASIZE_NUM_OFS                      0U                                          /*!< USART TX DATASIZE: NUM Bit Offset */
#define USART_TX_DATASIZE_NUM(regval)                  (BITS(0,1) & ((uint32_t)(regval) << 0))        /*!< USART TX DATASIZE: NUM Bit Value */  
#define USART_TX_DATASIZE_NUM_BYTE                          USART_TX_DATASIZE_NUM(0)                                           /*!< BYTE */
#define USART_TX_DATASIZE_NUM_WORD                          USART_TX_DATASIZE_NUM(2)                                           /*!< WORD */

/**
  * \brief Check the USART uart_tx_datasize num bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid num bits.
  * \retval 1 This is a valid num bits.
  */
#define IS_USART_UART_TX_DATASIZE_NUM(regval)                    (\
                                              ((regval) == USART_TX_DATASIZE_NUM_BYTE                ) || \
                                              ((regval) == USART_TX_DATASIZE_NUM_WORD                )  \
                                                 )

 /* ===== USART RX_DATASIZE Register definition ===== */
#define USART_RX_DATASIZE_NUM_MASK                     BITS(0,1)                                   /*!< USART RX DATASIZE: NUM Bit Mask */  
#define USART_RX_DATASIZE_NUM_OFS                      0U                                          /*!< USART RX DATASIZE: NUM Bit Offset */
#define USART_RX_DATASIZE_NUM(regval)                  (BITS(0,1) & ((uint32_t)(regval) << 0))        /*!< USART RX DATASIZE: NUM Bit Value */  
#define USART_RX_DATASIZE_NUM_BYTE                          USART_RX_DATASIZE_NUM(0)                                           /*!< BYTE */
#define USART_RX_DATASIZE_NUM_WORD                          USART_RX_DATASIZE_NUM(2)                                           /*!< WORD */

/**
  * \brief Check the USART uart_rx_datasize num bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid num bits.
  * \retval 1 This is a valid num bits.
  */
#define IS_USART_UART_RX_DATASIZE_NUM(regval)                    (\
                                              ((regval) == USART_RX_DATASIZE_NUM_BYTE                ) || \
                                              ((regval) == USART_RX_DATASIZE_NUM_WORD                )  \
                                                 )

 /* ===== USART SMARTCARD_SETUP Register definition ===== */
#define USART_SMARTCARD_SETUP_AUTO_RETRY_CNT_MASK          BITS(0,2)                                   /*!< USART SMARTCARD SETUP: AUTO_RETRY_CNT Bit Mask */  
#define USART_SMARTCARD_SETUP_AUTO_RETRY_CNT_OFS           0U                                          /*!< USART SMARTCARD SETUP: AUTO_RETRY_CNT Bit Offset */
#define USART_SMARTCARD_SETUP_AUTO_RETRY_CNT(regval)       (BITS(0,2) & ((uint32_t)(regval) << 0))        /*!< USART SMARTCARD SETUP: AUTO_RETRY_CNT Bit Value */  
#define USART_SMARTCARD_SETUP_NACK_EN                      BIT(3)                                      /*!< Smartcard NACK enable -1'b0:  disable: NACK transmission in case of parity error is disabled -1'b1:  enable: NACK transmission during parity error is enabled */
#define USART_SMARTCARD_SETUP_NACK_EN_OFS                  3U                                          /*!< USART SMARTCARD SETUP: NACK_EN Bit Offset */
#define USART_SMARTCARD_SETUP_NACK_EN_VAL(regval)              (BIT(3) & ((uint32_t)(regval) << 3))        /*!< USART SMARTCARD SETUP: NACK_EN Bit Value */  
#define USART_SMARTCARD_SETUP_NACK_EN_DISABLE                   0x0UL                                     /*!< DISABLE */
#define USART_SMARTCARD_SETUP_NACK_EN_ENABLE                    BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the USART smartcard_setup nack_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid nack_en bit.
  * \retval 1 This is a valid nack_en bit.
  */
#define IS_USART_SMARTCARD_SETUP_NACK_EN(regval)                (\
                                          ((regval) == USART_SMARTCARD_SETUP_NACK_EN_DISABLE             ) || \
                                          ((regval) == USART_SMARTCARD_SETUP_NACK_EN_ENABLE              )  \
                                                 )

#define USART_SMARTCARD_SETUP_TX_PUE                       BIT(4)                                      /*!< TX pin pull-up enable -1'b0:  disable: disable tx-pin pull-up resistor -1'b1:  enable: enable tx-pin pull-up resistor */
#define USART_SMARTCARD_SETUP_TX_PUE_OFS                   4U                                          /*!< USART SMARTCARD SETUP: TX_PUE Bit Offset */
#define USART_SMARTCARD_SETUP_TX_PUE_VAL(regval)               (BIT(4) & ((uint32_t)(regval) << 4))        /*!< USART SMARTCARD SETUP: TX_PUE Bit Value */  
#define USART_SMARTCARD_SETUP_TX_PUE_DISABLE                    0x0UL                                     /*!< DISABLE */
#define USART_SMARTCARD_SETUP_TX_PUE_ENABLE                     BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the USART smartcard_setup tx_pue bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_pue bit.
  * \retval 1 This is a valid tx_pue bit.
  */
#define IS_USART_SMARTCARD_SETUP_TX_PUE(regval)                 (\
                                           ((regval) == USART_SMARTCARD_SETUP_TX_PUE_DISABLE             ) || \
                                           ((regval) == USART_SMARTCARD_SETUP_TX_PUE_ENABLE              )  \
                                                 )

 /* ===== USART SMARTCARD_TIMING Register definition ===== */
#define USART_SMARTCARD_TIMING_GUARD_TIME_MASK              BITS(0,7)                                   /*!< USART SMARTCARD TIMING: GUARD_TIME Bit Mask */  
#define USART_SMARTCARD_TIMING_GUARD_TIME_OFS               0U                                          /*!< USART SMARTCARD TIMING: GUARD_TIME Bit Offset */
#define USART_SMARTCARD_TIMING_GUARD_TIME(regval)           (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< USART SMARTCARD TIMING: GUARD_TIME Bit Value */  
#define USART_SMARTCARD_TIMING_PSC_MASK                     BITS(8,15)                                   /*!< USART SMARTCARD TIMING: PSC Bit Mask */  
#define USART_SMARTCARD_TIMING_PSC_OFS                      8U                                          /*!< USART SMARTCARD TIMING: PSC Bit Offset */
#define USART_SMARTCARD_TIMING_PSC(regval)                  (BITS(8,15) & ((uint32_t)(regval) << 8))        /*!< USART SMARTCARD TIMING: PSC Bit Value */  
 
 /* ===== USART IP_VERSION Register definition ===== */
#define USART_IP_VERSION_VERSION                      BITS(0,31)                
 
 /* ===== USART ADVANCED_SETUP Register definition ===== */
#define USART_ADVANCED_SETUP_MM_EN                        BIT(0)                                      /*!< The multiprocesser mute mode enable - 1'b1:  enable: Mute mode enable,rx can switch to mute mode. - 1'b0:  disable: Mute mode disable,rx can not switch to mute mode. */
#define USART_ADVANCED_SETUP_MM_EN_OFS                    0U                                          /*!< USART ADVANCED SETUP: MM_EN Bit Offset */
#define USART_ADVANCED_SETUP_MM_EN_VAL(regval)                (BIT(0) & ((uint32_t)(regval) << 0))        /*!< USART ADVANCED SETUP: MM_EN Bit Value */  
#define USART_ADVANCED_SETUP_MM_EN_ENABLE                      BIT(0)                                                   /*!< ENABLE */
#define USART_ADVANCED_SETUP_MM_EN_DISABLE                     0x0UL                                      /*!< DISABLE */

/**
  * \brief Check the USART uart_advanced_setup mm_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid mm_en bit.
  * \retval 1 This is a valid mm_en bit.
  */
#define IS_USART_UART_ADVANCED_SETUP_MM_EN(regval)                  (\
                                            ((regval) == USART_ADVANCED_SETUP_MM_EN_ENABLE              ) || \
                                            ((regval) == USART_ADVANCED_SETUP_MM_EN_DISABLE             )  \
                                                 )

#define USART_ADVANCED_SETUP_WAKE_MOD                     BIT(1)                                      /*!< Multiprocesser receiver wakeup mode - 1'b0:  idle: rx idle line mode - 1'b1:  addr: addr mark mode */
#define USART_ADVANCED_SETUP_WAKE_MOD_OFS                 1U                                          /*!< USART ADVANCED SETUP: WAKE_MOD Bit Offset */
#define USART_ADVANCED_SETUP_WAKE_MOD_VAL(regval)             (BIT(1) & ((uint32_t)(regval) << 1))        /*!< USART ADVANCED SETUP: WAKE_MOD Bit Value */  
#define USART_ADVANCED_SETUP_WAKE_MOD_IDLE                     0x0UL                                      /*!< IDLE */
#define USART_ADVANCED_SETUP_WAKE_MOD_ADDR                     BIT(1)                                                   /*!< ADDR */

/**
  * \brief Check the USART uart_advanced_setup wake_mod bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid wake_mod bit.
  * \retval 1 This is a valid wake_mod bit.
  */
#define IS_USART_UART_ADVANCED_SETUP_WAKE_MOD(regval)               (\
                                         ((regval) == USART_ADVANCED_SETUP_WAKE_MOD_IDLE                ) || \
                                         ((regval) == USART_ADVANCED_SETUP_WAKE_MOD_ADDR                )  \
                                                 )

#define USART_ADVANCED_SETUP_ADD7                         BIT(2)                                      /*!< 7-bit address detection or 4-bit address detection mode - 1'b0:  4-bit: 4-bit address detection mode - 1'b1:  7-bit: 7-bit address detection mode */
#define USART_ADVANCED_SETUP_ADD7_OFS                     2U                                          /*!< USART ADVANCED SETUP: ADD7 Bit Offset */
#define USART_ADVANCED_SETUP_ADD7_VAL(regval)                 (BIT(2) & ((uint32_t)(regval) << 2))        /*!< USART ADVANCED SETUP: ADD7 Bit Value */  
#define USART_ADVANCED_SETUP_ADD7_4_BIT                        0x0UL                                      /*!< 4_BIT */
#define USART_ADVANCED_SETUP_ADD7_7_BIT                        BIT(2)                                                   /*!< 7_BIT */

/**
  * \brief Check the USART uart_advanced_setup add7 bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid add7 bit.
  * \retval 1 This is a valid add7 bit.
  */
#define IS_USART_UART_ADVANCED_SETUP_ADD7(regval)                   (\
                                             ((regval) == USART_ADVANCED_SETUP_ADD7_4_BIT               ) || \
                                             ((regval) == USART_ADVANCED_SETUP_ADD7_7_BIT               )  \
                                                 )

#define USART_ADVANCED_SETUP_ADD_MASK                     BITS(3,10)                                   /*!< USART ADVANCED SETUP: ADD Bit Mask */  
#define USART_ADVANCED_SETUP_ADD_OFS                      3U                                          /*!< USART ADVANCED SETUP: ADD Bit Offset */
#define USART_ADVANCED_SETUP_ADD(regval)                  (BITS(3,10) & ((uint32_t)(regval) << 3))        /*!< USART ADVANCED SETUP: ADD Bit Value */  
#define USART_ADVANCED_SETUP_LIN_EN                       BIT(11)                                      /*!< LIN mode enable - 1'b1:  enable: LIN mode enable - 1'b0:  disable: LIN mode disable */
#define USART_ADVANCED_SETUP_LIN_EN_OFS                   11U                                          /*!< USART ADVANCED SETUP: LIN_EN Bit Offset */
#define USART_ADVANCED_SETUP_LIN_EN_VAL(regval)               (BIT(11) & ((uint32_t)(regval) << 11))        /*!< USART ADVANCED SETUP: LIN_EN Bit Value */  
#define USART_ADVANCED_SETUP_LIN_EN_ENABLE                     BIT(11)                                                  /*!< ENABLE */
#define USART_ADVANCED_SETUP_LIN_EN_DISABLE                    0x0UL                                      /*!< DISABLE */

/**
  * \brief Check the USART uart_advanced_setup lin_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid lin_en bit.
  * \retval 1 This is a valid lin_en bit.
  */
#define IS_USART_UART_ADVANCED_SETUP_LIN_EN(regval)                 (\
                                           ((regval) == USART_ADVANCED_SETUP_LIN_EN_ENABLE              ) || \
                                           ((regval) == USART_ADVANCED_SETUP_LIN_EN_DISABLE             )  \
                                                 )

#define USART_ADVANCED_SETUP_LIN_LBDL                     BIT(12)                                      /*!< LIN mode break detection length - 1'b1:  11: 11-bit break detection - 1'b0:  10: 10-bit break detection  */
#define USART_ADVANCED_SETUP_LIN_LBDL_OFS                 12U                                          /*!< USART ADVANCED SETUP: LIN_LBDL Bit Offset */
#define USART_ADVANCED_SETUP_LIN_LBDL_VAL(regval)             (BIT(12) & ((uint32_t)(regval) << 12))        /*!< USART ADVANCED SETUP: LIN_LBDL Bit Value */  
#define USART_ADVANCED_SETUP_LIN_LBDL_11                       BIT(12)                                                  /*!< 11 */
#define USART_ADVANCED_SETUP_LIN_LBDL_10                       0x0UL                                      /*!< 10 */

/**
  * \brief Check the USART uart_advanced_setup lin_lbdl bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid lin_lbdl bit.
  * \retval 1 This is a valid lin_lbdl bit.
  */
#define IS_USART_UART_ADVANCED_SETUP_LIN_LBDL(regval)               (\
                                         ((regval) == USART_ADVANCED_SETUP_LIN_LBDL_11                  ) || \
                                         ((regval) == USART_ADVANCED_SETUP_LIN_LBDL_10                  )  \
                                                 )

#define USART_ADVANCED_SETUP_IRDA_EN                      BIT(13)                                      /*!< IRDA mode enable - 1'b1:  enable:IRDA mode enable - 1'b0:  disable:IRDA mode disable */
#define USART_ADVANCED_SETUP_IRDA_EN_OFS                  13U                                          /*!< USART ADVANCED SETUP: IRDA_EN Bit Offset */
#define USART_ADVANCED_SETUP_IRDA_EN_VAL(regval)              (BIT(13) & ((uint32_t)(regval) << 13))        /*!< USART ADVANCED SETUP: IRDA_EN Bit Value */  
#define USART_ADVANCED_SETUP_IRDA_EN_ENABLE                    BIT(13)                                                  /*!< ENABLE */
#define USART_ADVANCED_SETUP_IRDA_EN_DISABLE                   0x0UL                                      /*!< DISABLE */

/**
  * \brief Check the USART uart_advanced_setup irda_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid irda_en bit.
  * \retval 1 This is a valid irda_en bit.
  */
#define IS_USART_UART_ADVANCED_SETUP_IRDA_EN(regval)                (\
                                          ((regval) == USART_ADVANCED_SETUP_IRDA_EN_ENABLE              ) || \
                                          ((regval) == USART_ADVANCED_SETUP_IRDA_EN_DISABLE             )  \
                                                 )

#define USART_ADVANCED_SETUP_IRDALP_EN                    BIT(14)                                      /*!< IRDA low power mode enable - 1'b1 :  enable:IRDA low power enable - 1'b0 :  disable:IRDA low power disable */
#define USART_ADVANCED_SETUP_IRDALP_EN_OFS                14U                                          /*!< USART ADVANCED SETUP: IRDALP_EN Bit Offset */
#define USART_ADVANCED_SETUP_IRDALP_EN_VAL(regval)            (BIT(14) & ((uint32_t)(regval) << 14))        /*!< USART ADVANCED SETUP: IRDALP_EN Bit Value */  
#define USART_ADVANCED_SETUP_IRDALP_EN_ENABLE                  BIT(14)                                                  /*!< ENABLE */
#define USART_ADVANCED_SETUP_IRDALP_EN_DISABLE                 0x0UL                                      /*!< DISABLE */

/**
  * \brief Check the USART uart_advanced_setup irdalp_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid irdalp_en bit.
  * \retval 1 This is a valid irdalp_en bit.
  */
#define IS_USART_UART_ADVANCED_SETUP_IRDALP_EN(regval)              (\
                                        ((regval) == USART_ADVANCED_SETUP_IRDALP_EN_ENABLE              ) || \
                                        ((regval) == USART_ADVANCED_SETUP_IRDALP_EN_DISABLE             )  \
                                                 )

#define USART_ADVANCED_SETUP_RTU_EN                       BIT(15)                                      /*!< MODBUS mode enable - 1'b1:  enable:MODBUS RTU mode enable - 1'b0:  disable:MODBUS RTU mode disable */
#define USART_ADVANCED_SETUP_RTU_EN_OFS                   15U                                          /*!< USART ADVANCED SETUP: RTU_EN Bit Offset */
#define USART_ADVANCED_SETUP_RTU_EN_VAL(regval)               (BIT(15) & ((uint32_t)(regval) << 15))        /*!< USART ADVANCED SETUP: RTU_EN Bit Value */  
#define USART_ADVANCED_SETUP_RTU_EN_ENABLE                     BIT(15)                                                  /*!< ENABLE */
#define USART_ADVANCED_SETUP_RTU_EN_DISABLE                    0x0UL                                      /*!< DISABLE */

/**
  * \brief Check the USART uart_advanced_setup rtu_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rtu_en bit.
  * \retval 1 This is a valid rtu_en bit.
  */
#define IS_USART_UART_ADVANCED_SETUP_RTU_EN(regval)                 (\
                                           ((regval) == USART_ADVANCED_SETUP_RTU_EN_ENABLE              ) || \
                                           ((regval) == USART_ADVANCED_SETUP_RTU_EN_DISABLE             )  \
                                                 )

#define USART_ADVANCED_SETUP_ABR_EN                       BIT(19)                                      /*!< Auto baud rate enable - 1'b1:  enable: Auto baud rate enable - 1'b0:  disable: Auto baud rate disable */
#define USART_ADVANCED_SETUP_ABR_EN_OFS                   19U                                          /*!< USART ADVANCED SETUP: ABR_EN Bit Offset */
#define USART_ADVANCED_SETUP_ABR_EN_VAL(regval)               (BIT(19) & ((uint32_t)(regval) << 19))        /*!< USART ADVANCED SETUP: ABR_EN Bit Value */  
#define USART_ADVANCED_SETUP_ABR_EN_ENABLE                     BIT(19)                                                  /*!< ENABLE */
#define USART_ADVANCED_SETUP_ABR_EN_DISABLE                    0x0UL                                      /*!< DISABLE */

/**
  * \brief Check the USART uart_advanced_setup abr_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid abr_en bit.
  * \retval 1 This is a valid abr_en bit.
  */
#define IS_USART_UART_ADVANCED_SETUP_ABR_EN(regval)                 (\
                                           ((regval) == USART_ADVANCED_SETUP_ABR_EN_ENABLE              ) || \
                                           ((regval) == USART_ADVANCED_SETUP_ABR_EN_DISABLE             )  \
                                                 )

#define USART_ADVANCED_SETUP_ABR_MOD_MASK                 BITS(20,21)                                   /*!< USART ADVANCED SETUP: ABR_MOD Bit Mask */  
#define USART_ADVANCED_SETUP_ABR_MOD_OFS                  20U                                          /*!< USART ADVANCED SETUP: ABR_MOD Bit Offset */
#define USART_ADVANCED_SETUP_ABR_MOD(regval)              (BITS(20,21) & ((uint32_t)(regval) << 20))        /*!< USART ADVANCED SETUP: ABR_MOD Bit Value */  
#define USART_ADVANCED_SETUP_ABR_MOD_START_BIT                 USART_ADVANCED_SETUP_ABR_MOD(0)                                        /*!< START_BIT */
#define USART_ADVANCED_SETUP_ABR_MOD_NEGEDGE                   USART_ADVANCED_SETUP_ABR_MOD(1)                                        /*!< NEGEDGE */
#define USART_ADVANCED_SETUP_ABR_MOD_0X7F                      USART_ADVANCED_SETUP_ABR_MOD(2)                                        /*!< 0X7F */
#define USART_ADVANCED_SETUP_ABR_MOD_0X55                      USART_ADVANCED_SETUP_ABR_MOD(3)                                        /*!< 0X55 */

/**
  * \brief Check the USART uart_advanced_setup abr_mod bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid abr_mod bits.
  * \retval 1 This is a valid abr_mod bits.
  */
#define IS_USART_UART_ADVANCED_SETUP_ABR_MOD(regval)                (\
                                          ((regval) == USART_ADVANCED_SETUP_ABR_MOD_START_BIT           ) || \
                                          ((regval) == USART_ADVANCED_SETUP_ABR_MOD_NEGEDGE             ) || \
                                          ((regval) == USART_ADVANCED_SETUP_ABR_MOD_0X7F                ) || \
                                          ((regval) == USART_ADVANCED_SETUP_ABR_MOD_0X55                )  \
                                                 )

 /* ===== USART ADVANCED_STATUS Register definition ===== */
#define USART_ADVANCED_STATUS_RWUFMM                       BIT(0)                                      /*!< Rx wakeup from mute mode status: - 1'b1:  mute: rx in mute mode. - 1'b0:  active: rx in active mode. */
#define USART_ADVANCED_STATUS_RWUFMM_OFS                   0U                                          /*!< USART ADVANCED STATUS: RWUFMM Bit Offset */
#define USART_ADVANCED_STATUS_RWUFMM_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< USART ADVANCED STATUS: RWUFMM Bit Value */  
#define USART_ADVANCED_STATUS_RWUFMM_MUTE                       BIT(0)                                                   /*!< MUTE */
#define USART_ADVANCED_STATUS_RWUFMM_ACTIVE                     0x0UL                                     /*!< ACTIVE */

/**
  * \brief Check the USART uart_advanced_status rwufmm bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rwufmm bit.
  * \retval 1 This is a valid rwufmm bit.
  */
#define IS_USART_UART_ADVANCED_STATUS_RWUFMM(regval)                 (\
                                           ((regval) == USART_ADVANCED_STATUS_RWUFMM_MUTE                ) || \
                                           ((regval) == USART_ADVANCED_STATUS_RWUFMM_ACTIVE              )  \
                                                 )

#define USART_ADVANCED_STATUS_LIN_SBKF                     BIT(1)                                      /*!< LIN mode send break field flag - 1'b1: Break field transmitted - 1'b0: No break fidle transmitted */
#define USART_ADVANCED_STATUS_LIN_SBKF_OFS                 1U                                          /*!< USART ADVANCED STATUS: LIN_SBKF Bit Offset */
#define USART_ADVANCED_STATUS_LIN_SBKF_VAL(regval)             (BIT(1) & ((uint32_t)(regval) << 1))        /*!< USART ADVANCED STATUS: LIN_SBKF Bit Value */  
#define USART_ADVANCED_STATUS_LIN_SBKF_BREAK                    BIT(1)                                                   /*!< BREAK */
#define USART_ADVANCED_STATUS_LIN_SBKF_NO                       0x0UL                                     /*!< NO */

/**
  * \brief Check the USART uart_advanced_status lin_sbkf bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid lin_sbkf bit.
  * \retval 1 This is a valid lin_sbkf bit.
  */
#define IS_USART_UART_ADVANCED_STATUS_LIN_SBKF(regval)               (\
                                         ((regval) == USART_ADVANCED_STATUS_LIN_SBKF_BREAK               ) || \
                                         ((regval) == USART_ADVANCED_STATUS_LIN_SBKF_NO                  )  \
                                                 )

#define USART_ADVANCED_STATUS_LIN_LBDF                     BIT(2)                                      /*!< LIN mode break detection flag - 1'b1 :Break field detected - 1'b0 :Break field not detected  */
#define USART_ADVANCED_STATUS_LIN_LBDF_OFS                 2U                                          /*!< USART ADVANCED STATUS: LIN_LBDF Bit Offset */
#define USART_ADVANCED_STATUS_LIN_LBDF_VAL(regval)             (BIT(2) & ((uint32_t)(regval) << 2))        /*!< USART ADVANCED STATUS: LIN_LBDF Bit Value */  

/**
  * \brief Check the USART uart_advanced_status lin_lbdf bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid lin_lbdf bit.
  * \retval 1 This is a valid lin_lbdf bit.
  */
#define IS_USART_UART_ADVANCED_STATUS_LIN_LBDF(regval)               (\
                                                 )

#define USART_ADVANCED_STATUS_LFCMF                        BIT(3)                                      /*!< MODBUS mode LF match flag - 1'b1:  enable:MODBUS ASCII LF match - 1'b0:  disable:MODBUS ASCII LF not match */
#define USART_ADVANCED_STATUS_LFCMF_OFS                    3U                                          /*!< USART ADVANCED STATUS: LFCMF Bit Offset */
#define USART_ADVANCED_STATUS_LFCMF_VAL(regval)                (BIT(3) & ((uint32_t)(regval) << 3))        /*!< USART ADVANCED STATUS: LFCMF Bit Value */  

/**
  * \brief Check the USART uart_advanced_status lfcmf bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid lfcmf bit.
  * \retval 1 This is a valid lfcmf bit.
  */
#define IS_USART_UART_ADVANCED_STATUS_LFCMF(regval)                  (\
                                                 )

#define USART_ADVANCED_STATUS_ABRF                         BIT(5)                                      /*!< Auto baud rate flag,when auto baud rate completed,set this bit. */
#define USART_ADVANCED_STATUS_ABRF_OFS                     5U                                          /*!< USART ADVANCED STATUS: ABRF Bit Offset */
#define USART_ADVANCED_STATUS_ABRF_VAL(regval)                 (BIT(5) & ((uint32_t)(regval) << 5))        /*!< USART ADVANCED STATUS: ABRF Bit Value */  
#define USART_ADVANCED_STATUS_ABRE                         BIT(6)                                      /*!< Auto baud rate error,when auto baud rate failure,set this bit. */
#define USART_ADVANCED_STATUS_ABRE_OFS                     6U                                          /*!< USART ADVANCED STATUS: ABRE Bit Offset */
#define USART_ADVANCED_STATUS_ABRE_VAL(regval)                 (BIT(6) & ((uint32_t)(regval) << 6))        /*!< USART ADVANCED STATUS: ABRE Bit Value */  

/* ===== USART exported functions ===== */
void USART_Init(USART_TypeDef *USARTx, USART_InitTypeDef *USART_InitStruct);
void USART_StructInit(USART_InitTypeDef *USART_InitStruct);
void USART_ClockInit(USART_TypeDef *USARTx, USART_ClockInitTypeDef *USART_ClockInitStruct);
void USART_485Init(USART_TypeDef* USARTx, USART_485InitTypeDef* USART_485InitStruct);
uint32_t USART_GetTxSizeByDMA(USART_TypeDef *USARTx);
uint32_t USART_GetRxSizeByDMA(USART_TypeDef *USARTx);
void USART_SynClockConfig(USART_TypeDef *USARTx, ControlStatus Status);
void USART_SimplexConfig(USART_TypeDef *USARTx, ControlStatus Status);
FlagStatus USART_GetITStatus(USART_TypeDef *USARTx, uint32_t USART_IT);
FlagStatus USART_GetFlag(USART_TypeDef *USARTx, uint32_t USART_FLAG);
FlagStatus USART_GetABRStatus(USART_TypeDef *USARTx, uint32_t USART_ABR);
FlagStatus USART_GetTXCTRL_Status(USART_TypeDef *USARTx, uint32_t USART_STATUS);
FlagStatus USART_GetRXCTRL_Status(USART_TypeDef *USARTx, uint32_t USART_STATUS);
void USART_ClearITStatus(USART_TypeDef *USARTx, uint32_t USART_IT);
void USART_ITConfig(USART_TypeDef *USARTx, uint32_t USART_IE, ControlStatus Status);
FlagStatus USART_GetITConfig(USART_TypeDef *USARTx, uint32_t USART_IE);
void USART_SendData(USART_TypeDef *USARTx, uint8_t Data);
uint8_t USART_ReceiveData(USART_TypeDef *USARTx);
void USART_Send9Bit(USART_TypeDef *USARTx, uint16_t Data);
uint16_t USART_Receive9Bit(USART_TypeDef *USARTx);
void USART_Set_TxWaterMark(USART_TypeDef *USARTx, uint32_t Watermark);
void USART_Set_RxWaterMark(USART_TypeDef *USARTx, uint32_t Watermark);
void USART_CleanFifo_TX(USART_TypeDef *USARTx);
void USART_CleanFifo_RX(USART_TypeDef *USARTx);
void USART_Set_RXSize(USART_TypeDef *USARTx,uint32_t data);
uint32_t USART_Get_RXSize(USART_TypeDef *USARTx);
void USART_Set_TXSize(USART_TypeDef *USARTx,uint32_t data);
uint32_t USART_Get_TXSize(USART_TypeDef *USARTx);
FlagStatus USART_GetTransferFlag(USART_TypeDef *USARTx, uint32_t USART_FLAG);
void USART_CleanTransferSize(USART_TypeDef *USARTx, uint32_t USART_FLAG);

uint32_t USART_TxReadEntry(USART_TypeDef *USARTx);
uint32_t USART_RxReadEntry(USART_TypeDef *USARTx);
void USART_SetGuardTime(USART_TypeDef* USARTx, uint32_t USART_GuardTime);
void USART_SetPrescaler(USART_TypeDef* USARTx, uint32_t USART_Prescaler);
void USART_SetupTxpue(USART_TypeDef* USARTx, uint32_t Status);
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, uint32_t Status);
void USART_LIN_EN(USART_TypeDef* USARTx, uint32_t Status);
void USART_SendBreak(USART_TypeDef* USARTx);
uint32_t USART_LIN_GET_INT(USART_TypeDef* USARTx);
void USART_WakeUpConfig(USART_TypeDef* USARTx, uint32_t USART_FLAG);
void USART_ReceiverWakeUpCmd(USART_TypeDef* USARTx, ControlStatus Status);
void USART_SetRXAddress(USART_TypeDef* USARTx,uint32_t USART_FLAG, uint32_t data);
uint32_t USART_READWakeUp(USART_TypeDef* USARTx);
void USART_WriteWakeUp(USART_TypeDef* USARTx);
void USART_Irda_En(USART_TypeDef* USARTx, ControlStatus Status);
void USART_IrdaLp_En(USART_TypeDef* USARTx, ControlStatus Status);
void USART_ClearAdvancedITStatus(USART_TypeDef *USARTx, uint32_t USART_IT);
void USART_SLV_EN(USART_TypeDef* USARTx, uint32_t Status);
void USART_DIS_NSS_EN(USART_TypeDef* USARTx, uint32_t Status);
void USART_Set_RTU(USART_TypeDef *USARTx, ControlStatus Status);
#ifdef __cplusplus
}
#endif

#endif
/* _NS_USART_H */

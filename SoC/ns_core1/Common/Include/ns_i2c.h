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
#ifndef _NS_I2C_H
#define _NS_I2C_H
#ifdef __cplusplus
extern "C" {
#endif

/* macro define for 10bit mode */
#define ADDR_10BIT_HIGH_MASK      0x300
#define ADDR_10BIT_LOW_MASK       0xFF
#define ADDR_FIX                  0x78

#define ADDR_10BIT_1ST(x)   ({  \
                            typeof(x) _x = (x); \
                            ((_x & ADDR_10BIT_HIGH_MASK) >> 8 | ADDR_FIX);})

#define ADDR_10BIT_2ND(x)   ({  \
                              typeof(x) _x = (x); \
                              _x & ADDR_10BIT_LOW_MASK;})

/* for general address call */
#define ADDR_GENERAL_ADDR1ST      0x00
#define ADDR_GENERAL_RSTCMD       0x06

#define SMBUS_ADDR_DEFAULT       ((uint8_t)0xC2 >> 1) /*!< not valid device address, used for ARP */
#define SMBUS_ADDR_HOST          ((uint8_t)0x10 >> 1) /*!< Host address used in ARP */
#define SMBUS_ADDR_ARA           ((uint8_t)0x18 >> 1) /*!< Address used in alert response */

#define I2C_TIMEOUT_VAL     0x00FFFFFF

/**
  * \brief  I2C return status Define
  */
typedef enum {
    I2C_OK            = 0,
    I2C_ERROR          = 1,
    I2C_TIMEOUT       = 2,
} I2C_StateTypedef;

/**
  * \brief  I2C Speed Define
  */
typedef enum {
    I2C_SPEED_100K    = 0,
    I2C_SPEED_400K    = 1,
    I2C_SPEED_1M      = 2,
    I2C_SPEED_4M      = 3,
    I2C_SPEED_MAX
} I2C_SpeedTypedef;

/**
  * \brief  System clk Define
  */
typedef enum {
    I2C_SYS_CLK_8M    = 0,
    I2C_SYS_CLK_16M   = 1,
    I2C_SYS_CLK_32M   = 2,
    I2C_SYS_CLK_100M  = 3,
    I2C_SYS_CLK_MAX
} I2C_SysClkTypedef;

/**
  * \brief  I2C Data Direction Define
  */
typedef enum {
    I2C_DIR_WRITE                       = 0,/*!< I2C Write Flag */
    I2C_DIR_READ                        = 1,/*!< I2C Read Flag */
} I2C_DIRTypedef;

/**
  * \brief  I2C Mode Define
  */
typedef enum {
    I2C_MASTER_MODE                     = 0,/*!< I2C Master mode */
    I2C_SLAVE_MODE                      = 1,/*!< I2C Slave mode */
} I2C_ModeTypedef;

/**
  * \brief  I2C Time Cfg Struct Define
  */
typedef struct {
    uint16_t presc;
    uint16_t start_setup_time;
    uint16_t start_hold_time;
    uint16_t sda_setup_time;
    uint16_t sda_hold_time;
    uint16_t scl_high_period;
    uint16_t scl_low_period;
    uint16_t stop_setup_time;
    uint16_t bus_free_time;
} I2C_TimeCfgTypedef;

/**
  * \brief  I2C Slave Addr Struct Define
  */
typedef struct {
    uint16_t slave_addr;
    uint16_t slave_addr_mask;
    ControlStatus slave_addr_en;
} I2C_SlvAddrTypedef;

/**
  * \brief  I2C Dma Data Size Define
  */
typedef enum {
    I2C_DMA_DATA_SIZE_BYTE       = 0x0,/*!< I2C DMA Data Size Byte */
    I2C_DMA_DATA_SIZE_WORD       = 0x2,/*!< I2C DMA Data Size Word */
} I2C_DmaDataSizeTypedef;

/**
  * \brief  SMB Control Struct Define
  */
typedef struct {
    ControlStatus slv_byte_ctrl;    /*!< slave bate control */
    ControlStatus pec_en;           /*!< pec_en config */
    ControlStatus smbd_en;          /*!< SMBus device default address enable */
    ControlStatus smbh_en;          /*!< SMBus host address enable */
    ControlStatus alert_en;         /*!< SMBus alert transfer enable */
    ControlStatus tidle_en;         /*!< Idle clock timeout detection enable */
    ControlStatus text_en;          /*!< SMBUS Extended clock timeout enable */
    uint32_t text_num;              /*!< SMBUS Extended clock timeout number. */
} SMBUS_ConfigTypedef;

/**
  * \brief  I2C Init Struct Define
  */
typedef struct
{
    uint8_t speed;                          /*!< I2C speed config */
    I2C_ModeTypedef mode;                   /*!< I2C mode config */
    ControlStatus time_cfg;                 /*!< I2C auto_end en */
    ControlStatus auto_end;                 /*!< I2C auto_end en */
    ControlStatus scl_pull;                 /*!< SCL line pull up */
    ControlStatus sda_pull;                 /*!< SDA line pull up */
    ControlStatus dma_mode;                 /*!< dma mode config */
    ControlStatus addr_10bit;               /*!< 10bit_addr config */
    SMBUS_ConfigTypedef smbus_cfg;          /*!< smbus config */
    I2C_SlvAddrTypedef slave_info1;         /*!< Slave addr1 config */
    I2C_SlvAddrTypedef slave_info2;
    I2C_TimeCfgTypedef timecfg;             /*!< i2c time cfg struct */
    I2C_DmaDataSizeTypedef dma_txdata_size; /*!< dma txdata size */
    I2C_DmaDataSizeTypedef dma_rxdata_size; /*!< dma rxdata size */
} I2C_InitTypeDef;

#define I2C_RX_SIZE_OFFSET                                            0x4 /*!< Total rx data number in bytes */
#define I2C_RX_CFG_OFFSET                                             0x8 /*!< RX channel configuration  */
#define I2C_TX_SIZE_OFFSET                                            0x10 /*!< Total tx data number in bytes */
#define I2C_TX_CFG_OFFSET                                             0x14 /*!< TX channel configuration field */
#define I2C_STATUS_OFFSET                                             0x24 /*!< I2C status register */
#define I2C_TIMING_OFFSET                                             0x28 /*!< Timing prescaler */
#define I2C_SETUP_OFFSET                                              0x2c /*!< I2C setup configure register */
#define I2C_TXDATA_OFFSET                                             0x30 /*!< Transmit data register */
#define I2C_RXDATA_OFFSET                                             0x34 /*!< Receive data register */
#define I2C_INT_IE_OFFSET                                             0x38 /*!< Interrupt enable register */
#define I2C_SLAVE_ADDRESS1_OFFSET                                     0x3c /*!< Slave address */
#define I2C_SLAVE_ADDRESS2_OFFSET                                     0x44 /*!< Slave address */
#define I2C_TIME_OUT_OFFSET                                           0x48 /*!< SCL time out number */
#define I2C_DMA_RX_DATASIZE_OFFSET                                    0x64 /*!< The datasize of dma rx channel */
#define I2C_DMA_TX_DATASIZE_OFFSET                                    0x68 /*!< The datasize of dma tx channe */
#define I2C_TXFIFO_WM_OFFSET                                          0x7c /*!< The TXFIFO  watermark level */
#define I2C_RXFIFO_WM_OFFSET                                          0x80 /*!< The RXFIFO  watermark level */
#define I2C_DEBUG_OFFSET                                              0x98 /*!< The I2C internal signal for systerm debug */
#define I2C_IP_VERSION_OFFSET                                         0x9c /*!< The IP Version of I2C-Controller */
#define I2C_START_SETUP_TIME_OFFSET                                   0xa0 /*!< The START setup time  */
#define I2C_START_HOLD_TIME_OFFSET                                    0xa4 /*!< The START hold time  */
#define I2C_SDA_SETUP_TIME_OFFSET                                     0xa8 /*!< The SDA setup time  */
#define I2C_SDA_HOLD_TIME_OFFSET                                      0xac /*!< The SDA hold time  */
#define I2C_SCL_HIGH_PERIOD_OFFSET                                    0xb0 /*!< SCL high period (master mode) */
#define I2C_SCL_LOW_PERIOD_OFFSET                                     0xb4 /*!< SCL low period (master mode) */
#define I2C_STOP_SETUP_TIME_OFFSET                                    0xb8 /*!< The STOP setup time  */
#define I2C_BUS_FREE_TIME_OFFSET                                      0xbc /*!< The bus free time between a STOP and START condition */
#define I2C_ADVANCED_SETUP_OFFSET                                     0x100 /*!< I2C advanced feature setup configure register */
#define I2C_ADVANCED_STATUS_OFFSET                                    0x104 /*!< I2C advanced feature status register */

 /* ===== I2C RX_SIZE Register definition ===== */
#define I2C_RX_SIZE_NUM_MASK                     BITS(0,19)                                   /*!< I2C RX SIZE: NUM Bit Mask */  
#define I2C_RX_SIZE_NUM_OFS                      0U                                          /*!< I2C RX SIZE: NUM Bit Offset */
#define I2C_RX_SIZE_NUM(regval)                  (BITS(0,19) & ((uint32_t)(regval) << 0))        /*!< I2C RX SIZE: NUM Bit Value */  
 
 /* ===== I2C RX_CFG Register definition ===== */
#define I2C_RX_CFG_CONTINUOUS                   BIT(0)                                      /*!< RX channel continuous mode bitfield -1'b0: disable -1'b1: enable */
#define I2C_RX_CFG_CONTINUOUS_OFS               0U                                          /*!< I2C RX CFG: CONTINUOUS Bit Offset */
#define I2C_RX_CFG_CONTINUOUS_VAL(regval)           (BIT(0) & ((uint32_t)(regval) << 0))        /*!< I2C RX CFG: CONTINUOUS Bit Value */  
#define I2C_RX_CFG_CONTINUOUS_DISABLE                0x0UL                                              /*!< DISABLE */
#define I2C_RX_CFG_CONTINUOUS_ENABLE                 BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C rx_cfg continuous bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid continuous bit.
  * \retval 1 This is a valid continuous bit.
  */
#define IS_I2C_RX_CFG_CONTINUOUS(regval)             (\
                                       ((regval) == I2C_RX_CFG_CONTINUOUS_DISABLE             ) || \
                                       ((regval) == I2C_RX_CFG_CONTINUOUS_ENABLE              )  \
                                                 )

#define I2C_RX_CFG_EN                           BIT(1)                                      /*!< RX channel enable and start transfer bitfield: -1'b0: disable -1'b1: enable */
#define I2C_RX_CFG_EN_OFS                       1U                                          /*!< I2C RX CFG: EN Bit Offset */
#define I2C_RX_CFG_EN_VAL(regval)                   (BIT(1) & ((uint32_t)(regval) << 1))        /*!< I2C RX CFG: EN Bit Value */  
#define I2C_RX_CFG_EN_DISABLE                        0x0UL                                              /*!< DISABLE */
#define I2C_RX_CFG_EN_ENABLE                         BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C rx_cfg en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid en bit.
  * \retval 1 This is a valid en bit.
  */
#define IS_I2C_RX_CFG_EN(regval)                     (\
                                               ((regval) == I2C_RX_CFG_EN_DISABLE             ) || \
                                               ((regval) == I2C_RX_CFG_EN_ENABLE              )  \
                                                 )

#define I2C_RX_CFG_CLR                          BIT(3)                                      /*!< RX channel clear and stop transfer: -1'b0: disable -1'b1: enable stop and clear the on-going transfer */
#define I2C_RX_CFG_CLR_OFS                      3U                                          /*!< I2C RX CFG: CLR Bit Offset */
#define I2C_RX_CFG_CLR_VAL(regval)                  (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I2C RX CFG: CLR Bit Value */  
#define I2C_RX_CFG_CLR_DISABLE                       0x0UL                                              /*!< DISABLE */
#define I2C_RX_CFG_CLR_ENABLE                        BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C rx_cfg clr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid clr bit.
  * \retval 1 This is a valid clr bit.
  */
#define IS_I2C_RX_CFG_CLR(regval)                    (\
                                              ((regval) == I2C_RX_CFG_CLR_DISABLE             ) || \
                                              ((regval) == I2C_RX_CFG_CLR_ENABLE              )  \
                                                 )

 /* ===== I2C TX_SIZE Register definition ===== */
#define I2C_TX_SIZE_NUM_MASK                     BITS(0,19)                                   /*!< I2C TX SIZE: NUM Bit Mask */  
#define I2C_TX_SIZE_NUM_OFS                      0U                                          /*!< I2C TX SIZE: NUM Bit Offset */
#define I2C_TX_SIZE_NUM(regval)                  (BITS(0,19) & ((uint32_t)(regval) << 0))        /*!< I2C TX SIZE: NUM Bit Value */  
 
 /* ===== I2C TX_CFG Register definition ===== */
#define I2C_TX_CFG_CONTINUOUS                   BIT(0)                                      /*!< TX channel continuous mode bitfield -1'b0: disable -1'b1: enable */
#define I2C_TX_CFG_CONTINUOUS_OFS               0U                                          /*!< I2C TX CFG: CONTINUOUS Bit Offset */
#define I2C_TX_CFG_CONTINUOUS_VAL(regval)           (BIT(0) & ((uint32_t)(regval) << 0))        /*!< I2C TX CFG: CONTINUOUS Bit Value */  
#define I2C_TX_CFG_CONTINUOUS_DISABLE                0x0UL                                              /*!< DISABLE */
#define I2C_TX_CFG_CONTINUOUS_ENABLE                 BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C tx_cfg continuous bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid continuous bit.
  * \retval 1 This is a valid continuous bit.
  */
#define IS_I2C_TX_CFG_CONTINUOUS(regval)             (\
                                       ((regval) == I2C_TX_CFG_CONTINUOUS_DISABLE             ) || \
                                       ((regval) == I2C_TX_CFG_CONTINUOUS_ENABLE              )  \
                                                 )

#define I2C_TX_CFG_EN                           BIT(1)                                      /*!< TX channel enable and start transfer bitfield: -1'b0: disable -1'b1: enable */
#define I2C_TX_CFG_EN_OFS                       1U                                          /*!< I2C TX CFG: EN Bit Offset */
#define I2C_TX_CFG_EN_VAL(regval)                   (BIT(1) & ((uint32_t)(regval) << 1))        /*!< I2C TX CFG: EN Bit Value */  
#define I2C_TX_CFG_EN_DISABLE                        0x0UL                                              /*!< DISABLE */
#define I2C_TX_CFG_EN_ENABLE                         BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C tx_cfg en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid en bit.
  * \retval 1 This is a valid en bit.
  */
#define IS_I2C_TX_CFG_EN(regval)                     (\
                                               ((regval) == I2C_TX_CFG_EN_DISABLE             ) || \
                                               ((regval) == I2C_TX_CFG_EN_ENABLE              )  \
                                                 )

#define I2C_TX_CFG_CLR                          BIT(3)                                      /*!< TX channel clear and stop transfer: -1'b0: disable -1'b1: enable stop and clear the on-going transfer */
#define I2C_TX_CFG_CLR_OFS                      3U                                          /*!< I2C TX CFG: CLR Bit Offset */
#define I2C_TX_CFG_CLR_VAL(regval)                  (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I2C TX CFG: CLR Bit Value */  
#define I2C_TX_CFG_CLR_DISABLE                       0x0UL                                              /*!< DISABLE */
#define I2C_TX_CFG_CLR_ENABLE                        BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C tx_cfg clr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid clr bit.
  * \retval 1 This is a valid clr bit.
  */
#define IS_I2C_TX_CFG_CLR(regval)                    (\
                                              ((regval) == I2C_TX_CFG_CLR_DISABLE             ) || \
                                              ((regval) == I2C_TX_CFG_CLR_ENABLE              )  \
                                                 )

 /* ===== I2C STATUS Register definition ===== */
#define I2C_STATUS_BUSY                         BIT(0)                                      /*!< The i2c bus busy status flag 0: I2C-bus is idle 1: I2C-bus data-transfer is on progress */
#define I2C_STATUS_ARB_LOST                     BIT(1)                                      /*!< I2C arbitration lost status flag 0: no arbitration lost error 1: arbitration lost error */
#define I2C_STATUS_ARB_LOST_OFS                 1U                                          /*!< I2C STATUS: ARB_LOST Bit Offset */
#define I2C_STATUS_ARB_LOST_VAL(regval)             (BIT(1) & ((uint32_t)(regval) << 1))        /*!< I2C STATUS: ARB_LOST Bit Value */  
#define I2C_STATUS_NACK                         BIT(2)                                      /*!< The ack status bit in data transfer phase (not include address phase) of i2c-bus. 0: Detect ACK in i2c bus 1: Detect NACK in i2c bus Note: this status bit will be cleared by soft-ware when write 1 to this bit or cleared by stop condition in i2c bus.  */
#define I2C_STATUS_NACK_OFS                     2U                                          /*!< I2C STATUS: NACK Bit Offset */
#define I2C_STATUS_NACK_VAL(regval)                 (BIT(2) & ((uint32_t)(regval) << 2))        /*!< I2C STATUS: NACK Bit Value */  
#define I2C_STATUS_EOT                          BIT(3)                                      /*!< The end of transfer NOTE:This bit needs to be cleared after restart transmission in pec mode in slave write mode. */
#define I2C_STATUS_EOT_OFS                      3U                                          /*!< I2C STATUS: EOT Bit Offset */
#define I2C_STATUS_EOT_VAL(regval)                  (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I2C STATUS: EOT Bit Value */  
#define I2C_STATUS_BYTE_DONE                    BIT(4)                                      /*!< This bit is set by hardware when the byte is transmitted or received. */
#define I2C_STATUS_TIME_OUT                     BIT(5)                                      /*!< This bit is assert when the length of low level in SCL line is bigger than config number */
#define I2C_STATUS_TIME_OUT_OFS                 5U                                          /*!< I2C STATUS: TIME_OUT Bit Offset */
#define I2C_STATUS_TIME_OUT_VAL(regval)             (BIT(5) & ((uint32_t)(regval) << 5))        /*!< I2C STATUS: TIME_OUT Bit Value */  
#define I2C_STATUS_SLAVE_RECEIVE                BIT(9)                                      /*!< The slave receive mode 1: master send write operate to slave, so slave will read from master soon. 0: no master write operate happen.  Note:this bit is only used in I2C slave mode */
#define I2C_STATUS_SLAVE_TRANSMIT               BIT(10)                                      /*!< The slave transmit mode 1: master send read operate to slave, so slave will write to master soon. 0: no master read operate happen.  Note:this bit is only used in I2C slave mode */
#define I2C_STATUS_TXFIFO_EMPTY                 BIT(12)                                      /*!< This bit assert when tx-fifo is empty */
#define I2C_STATUS_TXFIFO_FULL                  BIT(13)                                      /*!< This bit assert when tx-fifo is full */
#define I2C_STATUS_RXFIFO_EMPTY                 BIT(15)                                      /*!< This bit assert when rx-fifo is empty */
#define I2C_STATUS_RXFIFO_FULL                  BIT(16)                                      /*!< This bit assert when rx-fifo is full */
#define I2C_STATUS_TXFIFO_WM                    BIT(25)                                      /*!< TX-FIFO watermark status bit */
#define I2C_STATUS_RXFIFO_WM                    BIT(26)                                      /*!< RX-FIFO watermark status bit */
#define I2C_STATUS_ADDR_MATCH                   BIT(27)                                      /*!< The ack status in addr-phase of i2c-bus, this bit can be used in master and slave mode. 0: The address is not match by slave and send response nack on i2c-bus 1: The address is match by slave and send response ack i2c-bus */
#define I2C_STATUS_ADDR_MATCH_OFS               27U                                          /*!< I2C STATUS: ADDR_MATCH Bit Offset */
#define I2C_STATUS_ADDR_MATCH_VAL(regval)           (BIT(27) & ((uint32_t)(regval) << 27))        /*!< I2C STATUS: ADDR_MATCH Bit Value */  
#define I2C_STATUS_BUS_ERROR                    BIT(28)                                      /*!< The bit is set to 1 when bus error occur */
#define I2C_STATUS_BUS_ERROR_OFS                28U                                          /*!< I2C STATUS: BUS_ERROR Bit Offset */
#define I2C_STATUS_BUS_ERROR_VAL(regval)            (BIT(28) & ((uint32_t)(regval) << 28))        /*!< I2C STATUS: BUS_ERROR Bit Value */  
#define I2C_STATUS_MASTER_STATE                 BITS(29,31)                
#define I2C_STATUS_MASTER_STATE_IDLE                 ((uint32_t)(0) << 29)                                                /*!< IDLE */
#define I2C_STATUS_MASTER_STATE_START                ((uint32_t)(1) << 29)                                                /*!< START */
#define I2C_STATUS_MASTER_STATE_READ                 ((uint32_t)(2) << 29)                                                /*!< READ */
#define I2C_STATUS_MASTER_STATE_WRITE                ((uint32_t)(3) << 29)                                                /*!< WRITE */
#define I2C_STATUS_MASTER_STATE_ACK                  ((uint32_t)(4) << 29)                                                /*!< ACK */
#define I2C_STATUS_MASTER_STATE_WAIT                 ((uint32_t)(5) << 29)                                                /*!< WAIT */
#define I2C_STATUS_MASTER_STATE_STOP                 ((uint32_t)(6) << 29)                                                /*!< STOP */

/**
  * \brief Check the I2C status master_state bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid master_state bits.
  * \retval 1 This is a valid master_state bits.
  */
#define IS_I2C_STATUS_MASTER_STATE(regval)           (\
                                     ((regval) == I2C_STATUS_MASTER_STATE_IDLE                ) || \
                                     ((regval) == I2C_STATUS_MASTER_STATE_START               ) || \
                                     ((regval) == I2C_STATUS_MASTER_STATE_READ                ) || \
                                     ((regval) == I2C_STATUS_MASTER_STATE_WRITE               ) || \
                                     ((regval) == I2C_STATUS_MASTER_STATE_ACK                 ) || \
                                     ((regval) == I2C_STATUS_MASTER_STATE_WAIT                ) || \
                                     ((regval) == I2C_STATUS_MASTER_STATE_STOP                )  \
                                                 )

 /* ===== I2C TIMING Register definition ===== */
#define I2C_TIMING_PRESC_MASK                   BITS(0,31)                                   /*!< I2C TIMING: PRESC Bit Mask */  
#define I2C_TIMING_PRESC_OFS                    0U                                          /*!< I2C TIMING: PRESC Bit Offset */
#define I2C_TIMING_PRESC(regval)                (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I2C TIMING: PRESC Bit Value */  
 
 /* ===== I2C SETUP Register definition ===== */
#define I2C_SETUP_WORK_MODE                    BIT(0)                                      /*!< I2C-Core work mode selected -1'b0: cpu mode -1'b1: dma mode */
#define I2C_SETUP_WORK_MODE_OFS                0U                                          /*!< I2C SETUP: WORK_MODE Bit Offset */
#define I2C_SETUP_WORK_MODE_VAL(regval)            (BIT(0) & ((uint32_t)(regval) << 0))        /*!< I2C SETUP: WORK_MODE Bit Value */  
#define I2C_SETUP_WORK_MODE_CPU                     0x0UL                                               /*!< CPU */
#define I2C_SETUP_WORK_MODE_DMA                     BIT(0)                                                   /*!< DMA */

/**
  * \brief Check the I2C setup work_mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid work_mode bit.
  * \retval 1 This is a valid work_mode bit.
  */
#define IS_I2C_SETUP_WORK_MODE(regval)              (\
                                        ((regval) == I2C_SETUP_WORK_MODE_CPU                 ) || \
                                        ((regval) == I2C_SETUP_WORK_MODE_DMA                 )  \
                                                 )

#define I2C_SETUP_ENABLE                       BIT(1)                                      /*!< I2C-Core enable in cpu mode 0: i2c-core disable 1: i2c-core enable */
#define I2C_SETUP_ENABLE_OFS                   1U                                          /*!< I2C SETUP: ENABLE Bit Offset */
#define I2C_SETUP_ENABLE_VAL(regval)               (BIT(1) & ((uint32_t)(regval) << 1))        /*!< I2C SETUP: ENABLE Bit Value */  
#define I2C_SETUP_START                        BIT(2)                                      /*!< Send start condition in I2C-bus 0: start condition in I2C-bus is finish 1: send start condition in I2C-bus */
#define I2C_SETUP_START_OFS                    2U                                          /*!< I2C SETUP: START Bit Offset */
#define I2C_SETUP_START_VAL(regval)                (BIT(2) & ((uint32_t)(regval) << 2))        /*!< I2C SETUP: START Bit Value */  
#define I2C_SETUP_STOP                         BIT(3)                                      /*!< Send stop condition in I2C-bus 0: stop condition in I2C-bus is finish 1: send stop condition in I2C-bus */
#define I2C_SETUP_STOP_OFS                     3U                                          /*!< I2C SETUP: STOP Bit Offset */
#define I2C_SETUP_STOP_VAL(regval)                 (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I2C SETUP: STOP Bit Value */  
#define I2C_SETUP_DIR                          BIT(4)                                      /*!< Transfer direction (master mode) -1'b0: write: Master requests a write transfer. -1'b1: read: Master requests a read transfer. Note: Changing this bit when the START bit is set is not allowed. */
#define I2C_SETUP_DIR_OFS                      4U                                          /*!< I2C SETUP: DIR Bit Offset */
#define I2C_SETUP_DIR_VAL(regval)                  (BIT(4) & ((uint32_t)(regval) << 4))        /*!< I2C SETUP: DIR Bit Value */  
#define I2C_SETUP_DIR_WRITE                         0x0UL                                               /*!< WRITE */
#define I2C_SETUP_DIR_READ                          BIT(4)                                                   /*!< READ */

/**
  * \brief Check the I2C setup dir bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dir bit.
  * \retval 1 This is a valid dir bit.
  */
#define IS_I2C_SETUP_DIR(regval)                    (\
                                              ((regval) == I2C_SETUP_DIR_WRITE               ) || \
                                              ((regval) == I2C_SETUP_DIR_READ                )  \
                                                 )

#define I2C_SETUP_ACK                          BIT(6)                                      /*!< The ack bit for hand shake which used in master or slave mode, this bit is cleared by start or stop condition after each transfer. 0: ack 1: nack */
#define I2C_SETUP_ACK_OFS                      6U                                          /*!< I2C SETUP: ACK Bit Offset */
#define I2C_SETUP_ACK_VAL(regval)                  (BIT(6) & ((uint32_t)(regval) << 6))        /*!< I2C SETUP: ACK Bit Value */  
#define I2C_SETUP_SOFT_RESET                   BIT(7)                                      /*!< The soft-reset command to I2C-Core logic, the soft-ware write this bit high to generate a sync Note: The soft-reset is a pulse and this bit is write-only field */
#define I2C_SETUP_SOFT_RESET_OFS               7U                                          /*!< I2C SETUP: SOFT_RESET Bit Offset */
#define I2C_SETUP_SOFT_RESET_VAL(regval)           (BIT(7) & ((uint32_t)(regval) << 7))        /*!< I2C SETUP: SOFT_RESET Bit Value */  
#define I2C_SETUP_TIME_OUT                     BIT(8)                                      /*!< The SCL line timeout enable -1'b0: disable SCL timeout -1'b1: enable SCL timeout */
#define I2C_SETUP_TIME_OUT_OFS                 8U                                          /*!< I2C SETUP: TIME_OUT Bit Offset */
#define I2C_SETUP_TIME_OUT_VAL(regval)             (BIT(8) & ((uint32_t)(regval) << 8))        /*!< I2C SETUP: TIME_OUT Bit Value */  
#define I2C_SETUP_TIME_OUT_DISABLE                  0x0UL                                               /*!< DISABLE */
#define I2C_SETUP_TIME_OUT_ENABLE                   BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C setup time_out bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid time_out bit.
  * \retval 1 This is a valid time_out bit.
  */
#define IS_I2C_SETUP_TIME_OUT(regval)               (\
                                         ((regval) == I2C_SETUP_TIME_OUT_DISABLE             ) || \
                                         ((regval) == I2C_SETUP_TIME_OUT_ENABLE              )  \
                                                 )

#define I2C_SETUP_10BIT_ADDR                   BIT(9)                                      /*!< The 10-bit address mode enable -1'b0: disable 10-bit address mode(7-bit mode) -1'b1: enable 10-bit address mode */
#define I2C_SETUP_10BIT_ADDR_OFS               9U                                          /*!< I2C SETUP: 10BIT_ADDR Bit Offset */
#define I2C_SETUP_10BIT_ADDR_VAL(regval)           (BIT(9) & ((uint32_t)(regval) << 9))        /*!< I2C SETUP: 10BIT_ADDR Bit Value */  
#define I2C_SETUP_10BIT_ADDR_DISABLE                0x0UL                                               /*!< DISABLE */
#define I2C_SETUP_10BIT_ADDR_ENABLE                 BIT(9)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C setup 10bit_addr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid 10bit_addr bit.
  * \retval 1 This is a valid 10bit_addr bit.
  */
#define IS_I2C_SETUP_10BIT_ADDR(regval)             (\
                                       ((regval) == I2C_SETUP_10BIT_ADDR_DISABLE             ) || \
                                       ((regval) == I2C_SETUP_10BIT_ADDR_ENABLE              )  \
                                                 )

#define I2C_SETUP_ROLE_MODE                    BIT(11)                                      /*!< The I2C-Controller is worked in Slave or Master mode -1'b0: Master mode -1'b1: Slave mode */
#define I2C_SETUP_ROLE_MODE_OFS                11U                                          /*!< I2C SETUP: ROLE_MODE Bit Offset */
#define I2C_SETUP_ROLE_MODE_VAL(regval)            (BIT(11) & ((uint32_t)(regval) << 11))        /*!< I2C SETUP: ROLE_MODE Bit Value */  
#define I2C_SETUP_ROLE_MODE_MASTER                  0x0UL                                               /*!< MASTER */
#define I2C_SETUP_ROLE_MODE_SLAVE                   BIT(11)                                                  /*!< SLAVE */

/**
  * \brief Check the I2C setup role_mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid role_mode bit.
  * \retval 1 This is a valid role_mode bit.
  */
#define IS_I2C_SETUP_ROLE_MODE(regval)              (\
                                        ((regval) == I2C_SETUP_ROLE_MODE_MASTER              ) || \
                                        ((regval) == I2C_SETUP_ROLE_MODE_SLAVE               )  \
                                                 )

#define I2C_SETUP_SCL_PULLUP                   BIT(14)                                      /*!< The SCL line pull-up enable -1'b0: disable pullup resistor in scl bus -1'b1: enable pullup resistor in scl bus */
#define I2C_SETUP_SCL_PULLUP_OFS               14U                                          /*!< I2C SETUP: SCL_PULLUP Bit Offset */
#define I2C_SETUP_SCL_PULLUP_VAL(regval)           (BIT(14) & ((uint32_t)(regval) << 14))        /*!< I2C SETUP: SCL_PULLUP Bit Value */  
#define I2C_SETUP_SCL_PULLUP_DISABLE                0x0UL                                               /*!< DISABLE */
#define I2C_SETUP_SCL_PULLUP_ENABLE                 BIT(14)                                                  /*!< ENABLE */

/**
  * \brief Check the I2C setup scl_pullup bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid scl_pullup bit.
  * \retval 1 This is a valid scl_pullup bit.
  */
#define IS_I2C_SETUP_SCL_PULLUP(regval)             (\
                                       ((regval) == I2C_SETUP_SCL_PULLUP_DISABLE             ) || \
                                       ((regval) == I2C_SETUP_SCL_PULLUP_ENABLE              )  \
                                                 )

#define I2C_SETUP_SDA_PULLUP                   BIT(15)                                      /*!< The SDA line pull-up enable -1'b0: disable pullup resistor in sda bus -1'b1: enable pullup resistor in sda bus */
#define I2C_SETUP_SDA_PULLUP_OFS               15U                                          /*!< I2C SETUP: SDA_PULLUP Bit Offset */
#define I2C_SETUP_SDA_PULLUP_VAL(regval)           (BIT(15) & ((uint32_t)(regval) << 15))        /*!< I2C SETUP: SDA_PULLUP Bit Value */  
#define I2C_SETUP_SDA_PULLUP_DISABLE                0x0UL                                               /*!< DISABLE */
#define I2C_SETUP_SDA_PULLUP_ENABLE                 BIT(15)                                                  /*!< ENABLE */

/**
  * \brief Check the I2C setup sda_pullup bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid sda_pullup bit.
  * \retval 1 This is a valid sda_pullup bit.
  */
#define IS_I2C_SETUP_SDA_PULLUP(regval)             (\
                                       ((regval) == I2C_SETUP_SDA_PULLUP_DISABLE             ) || \
                                       ((regval) == I2C_SETUP_SDA_PULLUP_ENABLE              )  \
                                                 )

#define I2C_SETUP_DNF_NUM_MASK                 BITS(18,21)                                   /*!< I2C SETUP: DNF_NUM Bit Mask */  
#define I2C_SETUP_DNF_NUM_OFS                  18U                                          /*!< I2C SETUP: DNF_NUM Bit Offset */
#define I2C_SETUP_DNF_NUM(regval)              (BITS(18,21) & ((uint32_t)(regval) << 18))        /*!< I2C SETUP: DNF_NUM Bit Value */  
#define I2C_SETUP_AUTO_END                     BIT(22)                                      /*!< Automatic end mode (master mode) 0: software end mode: The STOP condition in i2c-bus is asserted by soft-ware send stop-command(i2c_setup[3]). 1: Automatic end mode: a STOP condition is automatically sent when TX_SIZE or RX_SIZE data are transferred or received. */
#define I2C_SETUP_AUTO_END_OFS                 22U                                          /*!< I2C SETUP: AUTO_END Bit Offset */
#define I2C_SETUP_AUTO_END_VAL(regval)             (BIT(22) & ((uint32_t)(regval) << 22))        /*!< I2C SETUP: AUTO_END Bit Value */  
 
 /* ===== I2C TXDATA Register definition ===== */
#define I2C_TXDATA_TXDATA_MASK                  BITS(0,7)                                   /*!< I2C TXDATA: TXDATA Bit Mask */  
#define I2C_TXDATA_TXDATA_OFS                   0U                                          /*!< I2C TXDATA: TXDATA Bit Offset */
#define I2C_TXDATA_TXDATA(regval)               (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< I2C TXDATA: TXDATA Bit Value */  
 
 /* ===== I2C RXDATA Register definition ===== */
#define I2C_RXDATA_RXDATA                       BITS(0,31)                
 
 /* ===== I2C INT_IE Register definition ===== */
#define I2C_INT_IE_EOT                          BIT(0)                                      /*!< The last transfer interrupt enable -1'b0: disable -1'b1: enable */
#define I2C_INT_IE_EOT_OFS                      0U                                          /*!< I2C INT IE: EOT Bit Offset */
#define I2C_INT_IE_EOT_VAL(regval)                  (BIT(0) & ((uint32_t)(regval) << 0))        /*!< I2C INT IE: EOT Bit Value */  
#define I2C_INT_IE_EOT_DISABLE                       0x0UL                                              /*!< DISABLE */
#define I2C_INT_IE_EOT_ENABLE                        BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C int_ie eot bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid eot bit.
  * \retval 1 This is a valid eot bit.
  */
#define IS_I2C_INT_IE_EOT(regval)                    (\
                                              ((regval) == I2C_INT_IE_EOT_DISABLE             ) || \
                                              ((regval) == I2C_INT_IE_EOT_ENABLE              )  \
                                                 )

#define I2C_INT_IE_ARBITRATION_LOST             BIT(1)                                      /*!< Atribtion lost interrupt enable -1'b0: disable -1'b1: enable */
#define I2C_INT_IE_ARBITRATION_LOST_OFS         1U                                          /*!< I2C INT IE: ARBITRATION_LOST Bit Offset */
#define I2C_INT_IE_ARBITRATION_LOST_VAL(regval)     (BIT(1) & ((uint32_t)(regval) << 1))        /*!< I2C INT IE: ARBITRATION_LOST Bit Value */  
#define I2C_INT_IE_ARBITRATION_LOST_DISABLE             0x0UL                                              /*!< DISABLE */
#define I2C_INT_IE_ARBITRATION_LOST_ENABLE             BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C int_ie arbitration_lost bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid arbitration_lost bit.
  * \retval 1 This is a valid arbitration_lost bit.
  */
#define IS_I2C_INT_IE_ARBITRATION_LOST(regval)       (\
                                 ((regval) == I2C_INT_IE_ARBITRATION_LOST_DISABLE             ) || \
                                 ((regval) == I2C_INT_IE_ARBITRATION_LOST_ENABLE              )  \
                                                 )

#define I2C_INT_IE_TXFIFO_WM                    BIT(2)                                      /*!< I2C TX-FIFO WM interrupt enable -1'b0: disable -1'b1: enable */
#define I2C_INT_IE_TXFIFO_WM_OFS                2U                                          /*!< I2C INT IE: TXFIFO_WM Bit Offset */
#define I2C_INT_IE_TXFIFO_WM_VAL(regval)            (BIT(2) & ((uint32_t)(regval) << 2))        /*!< I2C INT IE: TXFIFO_WM Bit Value */  
#define I2C_INT_IE_TXFIFO_WM_DISABLE                 0x0UL                                              /*!< DISABLE */
#define I2C_INT_IE_TXFIFO_WM_ENABLE                  BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C int_ie txfifo_wm bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid txfifo_wm bit.
  * \retval 1 This is a valid txfifo_wm bit.
  */
#define IS_I2C_INT_IE_TXFIFO_WM(regval)              (\
                                        ((regval) == I2C_INT_IE_TXFIFO_WM_DISABLE             ) || \
                                        ((regval) == I2C_INT_IE_TXFIFO_WM_ENABLE              )  \
                                                 )

#define I2C_INT_IE_RXFIFO_WM                    BIT(3)                                      /*!< I2C RX-FIFO WM interrupt enable -1'b0: disable -1'b1: enable */
#define I2C_INT_IE_RXFIFO_WM_OFS                3U                                          /*!< I2C INT IE: RXFIFO_WM Bit Offset */
#define I2C_INT_IE_RXFIFO_WM_VAL(regval)            (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I2C INT IE: RXFIFO_WM Bit Value */  
#define I2C_INT_IE_RXFIFO_WM_DISABLE                 0x0UL                                              /*!< DISABLE */
#define I2C_INT_IE_RXFIFO_WM_ENABLE                  BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C int_ie rxfifo_wm bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rxfifo_wm bit.
  * \retval 1 This is a valid rxfifo_wm bit.
  */
#define IS_I2C_INT_IE_RXFIFO_WM(regval)              (\
                                        ((regval) == I2C_INT_IE_RXFIFO_WM_DISABLE             ) || \
                                        ((regval) == I2C_INT_IE_RXFIFO_WM_ENABLE              )  \
                                                 )

#define I2C_INT_IE_TIME_OUT                     BIT(4)                                      /*!< SCL line time out interrupt enable -1'b0: disable -1'b1: enable */
#define I2C_INT_IE_TIME_OUT_OFS                 4U                                          /*!< I2C INT IE: TIME_OUT Bit Offset */
#define I2C_INT_IE_TIME_OUT_VAL(regval)             (BIT(4) & ((uint32_t)(regval) << 4))        /*!< I2C INT IE: TIME_OUT Bit Value */  
#define I2C_INT_IE_TIME_OUT_DISABLE                  0x0UL                                              /*!< DISABLE */
#define I2C_INT_IE_TIME_OUT_ENABLE                   BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C int_ie time_out bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid time_out bit.
  * \retval 1 This is a valid time_out bit.
  */
#define IS_I2C_INT_IE_TIME_OUT(regval)               (\
                                         ((regval) == I2C_INT_IE_TIME_OUT_DISABLE             ) || \
                                         ((regval) == I2C_INT_IE_TIME_OUT_ENABLE              )  \
                                                 )

#define I2C_INT_IE_ADDR_MATCH                   BIT(5)                                      /*!< The address matched interrupt enable: -1'b0: disable -1'b1: enable */
#define I2C_INT_IE_ADDR_MATCH_OFS               5U                                          /*!< I2C INT IE: ADDR_MATCH Bit Offset */
#define I2C_INT_IE_ADDR_MATCH_VAL(regval)           (BIT(5) & ((uint32_t)(regval) << 5))        /*!< I2C INT IE: ADDR_MATCH Bit Value */  
#define I2C_INT_IE_ADDR_MATCH_DISABLE                0x0UL                                              /*!< DISABLE */
#define I2C_INT_IE_ADDR_MATCH_ENABLE                 BIT(5)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C int_ie addr_match bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid addr_match bit.
  * \retval 1 This is a valid addr_match bit.
  */
#define IS_I2C_INT_IE_ADDR_MATCH(regval)             (\
                                       ((regval) == I2C_INT_IE_ADDR_MATCH_DISABLE             ) || \
                                       ((regval) == I2C_INT_IE_ADDR_MATCH_ENABLE              )  \
                                                 )

#define I2C_INT_IE_NACK_RECEPTION               BIT(6)                                      /*!< The nack reception interrupt enable: -1'b0: disable -1'b1: enable */
#define I2C_INT_IE_NACK_RECEPTION_OFS           6U                                          /*!< I2C INT IE: NACK_RECEPTION Bit Offset */
#define I2C_INT_IE_NACK_RECEPTION_VAL(regval)       (BIT(6) & ((uint32_t)(regval) << 6))        /*!< I2C INT IE: NACK_RECEPTION Bit Value */  
#define I2C_INT_IE_NACK_RECEPTION_DISABLE             0x0UL                                              /*!< DISABLE */
#define I2C_INT_IE_NACK_RECEPTION_ENABLE             BIT(6)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C int_ie nack_reception bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid nack_reception bit.
  * \retval 1 This is a valid nack_reception bit.
  */
#define IS_I2C_INT_IE_NACK_RECEPTION(regval)         (\
                                   ((regval) == I2C_INT_IE_NACK_RECEPTION_DISABLE             ) || \
                                   ((regval) == I2C_INT_IE_NACK_RECEPTION_ENABLE              )  \
                                                 )

#define I2C_INT_IE_ERROR                        BIT(8)                                      /*!< The error interrupts enable -1'b0: disable -1'b1: enable */
#define I2C_INT_IE_ERROR_OFS                    8U                                          /*!< I2C INT IE: ERROR Bit Offset */
#define I2C_INT_IE_ERROR_VAL(regval)                (BIT(8) & ((uint32_t)(regval) << 8))        /*!< I2C INT IE: ERROR Bit Value */  
#define I2C_INT_IE_ERROR_DISABLE                     0x0UL                                              /*!< DISABLE */
#define I2C_INT_IE_ERROR_ENABLE                      BIT(8)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C int_ie error bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid error bit.
  * \retval 1 This is a valid error bit.
  */
#define IS_I2C_INT_IE_ERROR(regval)                  (\
                                            ((regval) == I2C_INT_IE_ERROR_DISABLE             ) || \
                                            ((regval) == I2C_INT_IE_ERROR_ENABLE              )  \
                                                 )

#define I2C_INT_IE_SBC                          BIT(9)                                      /*!< The slave byte control transfer done interrupts enable -1'b0: disable -1'b1: enable */
#define I2C_INT_IE_SBC_OFS                      9U                                          /*!< I2C INT IE: SBC Bit Offset */
#define I2C_INT_IE_SBC_VAL(regval)                  (BIT(9) & ((uint32_t)(regval) << 9))        /*!< I2C INT IE: SBC Bit Value */  
#define I2C_INT_IE_SBC_DISABLE                       0x0UL                                              /*!< DISABLE */
#define I2C_INT_IE_SBC_ENABLE                        BIT(9)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C int_ie sbc bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid sbc bit.
  * \retval 1 This is a valid sbc bit.
  */
#define IS_I2C_INT_IE_SBC(regval)                    (\
                                              ((regval) == I2C_INT_IE_SBC_DISABLE             ) || \
                                              ((regval) == I2C_INT_IE_SBC_ENABLE              )  \
                                                 )

#define I2C_INT_IE_PEC                          BIT(10)                                      /*!< The PEC byte transfer done interrupts enable -1'b0: disable -1'b1: enable */
#define I2C_INT_IE_PEC_OFS                      10U                                          /*!< I2C INT IE: PEC Bit Offset */
#define I2C_INT_IE_PEC_VAL(regval)                  (BIT(10) & ((uint32_t)(regval) << 10))        /*!< I2C INT IE: PEC Bit Value */  
#define I2C_INT_IE_PEC_DISABLE                       0x0UL                                              /*!< DISABLE */
#define I2C_INT_IE_PEC_ENABLE                        BIT(10)                                                  /*!< ENABLE */

/**
  * \brief Check the I2C int_ie pec bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid pec bit.
  * \retval 1 This is a valid pec bit.
  */
#define IS_I2C_INT_IE_PEC(regval)                    (\
                                              ((regval) == I2C_INT_IE_PEC_DISABLE             ) || \
                                              ((regval) == I2C_INT_IE_PEC_ENABLE              )  \
                                                 )

 /* ===== I2C SLAVE_ADDRESS1 Register definition ===== */
#define I2C_SLAVE_ADDRESS1_NUM_MASK                     BITS(0,9)                                   /*!< I2C SLAVE ADDRESS1: NUM Bit Mask */  
#define I2C_SLAVE_ADDRESS1_NUM_OFS                      0U                                          /*!< I2C SLAVE ADDRESS1: NUM Bit Offset */
#define I2C_SLAVE_ADDRESS1_NUM(regval)                  (BITS(0,9) & ((uint32_t)(regval) << 0))        /*!< I2C SLAVE ADDRESS1: NUM Bit Value */  
#define I2C_SLAVE_ADDRESS1_MASK_MASK                    BITS(10,19)                                   /*!< I2C SLAVE ADDRESS1: MASK Bit Mask */  
#define I2C_SLAVE_ADDRESS1_MASK_OFS                     10U                                          /*!< I2C SLAVE ADDRESS1: MASK Bit Offset */
#define I2C_SLAVE_ADDRESS1_MASK(regval)                 (BITS(10,19) & ((uint32_t)(regval) << 10))        /*!< I2C SLAVE ADDRESS1: MASK Bit Value */  
#define I2C_SLAVE_ADDRESS1_EN                           BIT(31)                                      /*!< The slave address1 enable 0: The slave address1 disabled. The slave will send NACK at i2c address phase. 1: The slave address1 enabled. The slave will send ACK when receive address is mapping with slave address1. */
#define I2C_SLAVE_ADDRESS1_EN_OFS                       31U                                          /*!< I2C SLAVE ADDRESS1: EN Bit Offset */
#define I2C_SLAVE_ADDRESS1_EN_VAL(regval)                   (BIT(31) & ((uint32_t)(regval) << 31))        /*!< I2C SLAVE ADDRESS1: EN Bit Value */  
 
 /* ===== I2C SLAVE_ADDRESS2 Register definition ===== */
#define I2C_SLAVE_ADDRESS2_NUM_MASK                     BITS(0,9)                                   /*!< I2C SLAVE ADDRESS2: NUM Bit Mask */  
#define I2C_SLAVE_ADDRESS2_NUM_OFS                      0U                                          /*!< I2C SLAVE ADDRESS2: NUM Bit Offset */
#define I2C_SLAVE_ADDRESS2_NUM(regval)                  (BITS(0,9) & ((uint32_t)(regval) << 0))        /*!< I2C SLAVE ADDRESS2: NUM Bit Value */  
#define I2C_SLAVE_ADDRESS2_MASK_MASK                    BITS(10,19)                                   /*!< I2C SLAVE ADDRESS2: MASK Bit Mask */  
#define I2C_SLAVE_ADDRESS2_MASK_OFS                     10U                                          /*!< I2C SLAVE ADDRESS2: MASK Bit Offset */
#define I2C_SLAVE_ADDRESS2_MASK(regval)                 (BITS(10,19) & ((uint32_t)(regval) << 10))        /*!< I2C SLAVE ADDRESS2: MASK Bit Value */  
#define I2C_SLAVE_ADDRESS2_EN                           BIT(31)                                      /*!< The slave address2 enable 0: The slave address2 disabled. The slave will send NACK at i2c address phase. 1: The slave address2 enabled. The slave will send ACK when receive address is mapping with slave address2. */
#define I2C_SLAVE_ADDRESS2_EN_OFS                       31U                                          /*!< I2C SLAVE ADDRESS2: EN Bit Offset */
#define I2C_SLAVE_ADDRESS2_EN_VAL(regval)                   (BIT(31) & ((uint32_t)(regval) << 31))        /*!< I2C SLAVE ADDRESS2: EN Bit Value */  
 
 /* ===== I2C TIME_OUT Register definition ===== */
#define I2C_TIME_OUT_NUM_MASK                     BITS(0,15)                                   /*!< I2C TIME OUT: NUM Bit Mask */  
#define I2C_TIME_OUT_NUM_OFS                      0U                                          /*!< I2C TIME OUT: NUM Bit Offset */
#define I2C_TIME_OUT_NUM(regval)                  (BITS(0,15) & ((uint32_t)(regval) << 0))        /*!< I2C TIME OUT: NUM Bit Value */  
 
 /* ===== I2C DMA_RX_DATASIZE Register definition ===== */
#define I2C_DMA_RX_DATASIZE_TYPE_MASK                    BITS(0,1)                                   /*!< I2C DMA RX DATASIZE: TYPE Bit Mask */  
#define I2C_DMA_RX_DATASIZE_TYPE_OFS                     0U                                          /*!< I2C DMA RX DATASIZE: TYPE Bit Offset */
#define I2C_DMA_RX_DATASIZE_TYPE(regval)                 (BITS(0,1) & ((uint32_t)(regval) << 0))        /*!< I2C DMA RX DATASIZE: TYPE Bit Value */  
#define I2C_DMA_RX_DATASIZE_TYPE_BYTE                         I2C_DMA_RX_DATASIZE_TYPE(0)                                       /*!< BYTE */
#define I2C_DMA_RX_DATASIZE_TYPE_WORD                         I2C_DMA_RX_DATASIZE_TYPE(2)                                       /*!< WORD */

/**
  * \brief Check the I2C dma_rx_datasize type bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid type bits.
  * \retval 1 This is a valid type bits.
  */
#define IS_I2C_DMA_RX_DATASIZE_TYPE(regval)                   (\
                                             ((regval) == I2C_DMA_RX_DATASIZE_TYPE_BYTE                ) || \
                                             ((regval) == I2C_DMA_RX_DATASIZE_TYPE_WORD                )  \
                                                 )

 /* ===== I2C DMA_TX_DATASIZE Register definition ===== */
#define I2C_DMA_TX_DATASIZE_TYPE_MASK                    BITS(0,1)                                   /*!< I2C DMA TX DATASIZE: TYPE Bit Mask */  
#define I2C_DMA_TX_DATASIZE_TYPE_OFS                     0U                                          /*!< I2C DMA TX DATASIZE: TYPE Bit Offset */
#define I2C_DMA_TX_DATASIZE_TYPE(regval)                 (BITS(0,1) & ((uint32_t)(regval) << 0))        /*!< I2C DMA TX DATASIZE: TYPE Bit Value */  
#define I2C_DMA_TX_DATASIZE_TYPE_BYTE                         I2C_DMA_TX_DATASIZE_TYPE(0)                                       /*!< BYTE */
#define I2C_DMA_TX_DATASIZE_TYPE_WORD                         I2C_DMA_TX_DATASIZE_TYPE(2)                                       /*!< WORD */

/**
  * \brief Check the I2C dma_tx_datasize type bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid type bits.
  * \retval 1 This is a valid type bits.
  */
#define IS_I2C_DMA_TX_DATASIZE_TYPE(regval)                   (\
                                             ((regval) == I2C_DMA_TX_DATASIZE_TYPE_BYTE                ) || \
                                             ((regval) == I2C_DMA_TX_DATASIZE_TYPE_WORD                )  \
                                                 )

 /* ===== I2C TXFIFO_WM Register definition ===== */
#define I2C_TXFIFO_WM_CNT_MASK                     BITS(0,3)                                   /*!< I2C TXFIFO WM: CNT Bit Mask */  
#define I2C_TXFIFO_WM_CNT_OFS                      0U                                          /*!< I2C TXFIFO WM: CNT Bit Offset */
#define I2C_TXFIFO_WM_CNT(regval)                  (BITS(0,3) & ((uint32_t)(regval) << 0))        /*!< I2C TXFIFO WM: CNT Bit Value */  
 
 /* ===== I2C RXFIFO_WM Register definition ===== */
#define I2C_RXFIFO_WM_CNT_MASK                     BITS(0,3)                                   /*!< I2C RXFIFO WM: CNT Bit Mask */  
#define I2C_RXFIFO_WM_CNT_OFS                      0U                                          /*!< I2C RXFIFO WM: CNT Bit Offset */
#define I2C_RXFIFO_WM_CNT(regval)                  (BITS(0,3) & ((uint32_t)(regval) << 0))        /*!< I2C RXFIFO WM: CNT Bit Value */  
 
 /* ===== I2C DEBUG Register definition ===== */
#define I2C_DEBUG_STATUS                       BITS(0,14)                
 
 /* ===== I2C IP_VERSION Register definition ===== */
#define I2C_IP_VERSION_VERSION_NUM                  BITS(0,31)                
 
 /* ===== I2C START_SETUP_TIME Register definition ===== */
#define I2C_START_SETUP_TIME_VALUE_MASK                   BITS(0,31)                                   /*!< I2C START SETUP TIME: VALUE Bit Mask */  
#define I2C_START_SETUP_TIME_VALUE_OFS                    0U                                          /*!< I2C START SETUP TIME: VALUE Bit Offset */
#define I2C_START_SETUP_TIME_VALUE(regval)                (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I2C START SETUP TIME: VALUE Bit Value */  
 
 /* ===== I2C START_HOLD_TIME Register definition ===== */
#define I2C_START_HOLD_TIME_VALUE_MASK                   BITS(0,31)                                   /*!< I2C START HOLD TIME: VALUE Bit Mask */  
#define I2C_START_HOLD_TIME_VALUE_OFS                    0U                                          /*!< I2C START HOLD TIME: VALUE Bit Offset */
#define I2C_START_HOLD_TIME_VALUE(regval)                (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I2C START HOLD TIME: VALUE Bit Value */  
 
 /* ===== I2C SDA_SETUP_TIME Register definition ===== */
#define I2C_SDA_SETUP_TIME_VALUE_MASK                   BITS(0,31)                                   /*!< I2C SDA SETUP TIME: VALUE Bit Mask */  
#define I2C_SDA_SETUP_TIME_VALUE_OFS                    0U                                          /*!< I2C SDA SETUP TIME: VALUE Bit Offset */
#define I2C_SDA_SETUP_TIME_VALUE(regval)                (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I2C SDA SETUP TIME: VALUE Bit Value */  
 
 /* ===== I2C SDA_HOLD_TIME Register definition ===== */
#define I2C_SDA_HOLD_TIME_VALUE_MASK                   BITS(0,31)                                   /*!< I2C SDA HOLD TIME: VALUE Bit Mask */  
#define I2C_SDA_HOLD_TIME_VALUE_OFS                    0U                                          /*!< I2C SDA HOLD TIME: VALUE Bit Offset */
#define I2C_SDA_HOLD_TIME_VALUE(regval)                (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I2C SDA HOLD TIME: VALUE Bit Value */  
 
 /* ===== I2C SCL_HIGH_PERIOD Register definition ===== */
#define I2C_SCL_HIGH_PERIOD_VALUE_MASK                   BITS(0,31)                                   /*!< I2C SCL HIGH PERIOD: VALUE Bit Mask */  
#define I2C_SCL_HIGH_PERIOD_VALUE_OFS                    0U                                          /*!< I2C SCL HIGH PERIOD: VALUE Bit Offset */
#define I2C_SCL_HIGH_PERIOD_VALUE(regval)                (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I2C SCL HIGH PERIOD: VALUE Bit Value */  
 
 /* ===== I2C SCL_LOW_PERIOD Register definition ===== */
#define I2C_SCL_LOW_PERIOD_VALUE_MASK                   BITS(0,31)                                   /*!< I2C SCL LOW PERIOD: VALUE Bit Mask */  
#define I2C_SCL_LOW_PERIOD_VALUE_OFS                    0U                                          /*!< I2C SCL LOW PERIOD: VALUE Bit Offset */
#define I2C_SCL_LOW_PERIOD_VALUE(regval)                (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I2C SCL LOW PERIOD: VALUE Bit Value */  
 
 /* ===== I2C STOP_SETUP_TIME Register definition ===== */
#define I2C_STOP_SETUP_TIME_VALUE_MASK                   BITS(0,31)                                   /*!< I2C STOP SETUP TIME: VALUE Bit Mask */  
#define I2C_STOP_SETUP_TIME_VALUE_OFS                    0U                                          /*!< I2C STOP SETUP TIME: VALUE Bit Offset */
#define I2C_STOP_SETUP_TIME_VALUE(regval)                (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I2C STOP SETUP TIME: VALUE Bit Value */  
 
 /* ===== I2C BUS_FREE_TIME Register definition ===== */
#define I2C_BUS_FREE_TIME_VALUE_MASK                   BITS(0,31)                                   /*!< I2C BUS FREE TIME: VALUE Bit Mask */  
#define I2C_BUS_FREE_TIME_VALUE_OFS                    0U                                          /*!< I2C BUS FREE TIME: VALUE Bit Offset */
#define I2C_BUS_FREE_TIME_VALUE(regval)                (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< I2C BUS FREE TIME: VALUE Bit Value */  
 
 /* ===== I2C ADVANCED_SETUP Register definition ===== */
#define I2C_ADVANCED_SETUP_SBCEN                        BIT(1)                                      /*!< The slave device byte control mode enable: -1'b0:disable slave device byte control mode. -1'b1:enable slave device byte control mode. */
#define I2C_ADVANCED_SETUP_SBCEN_OFS                    1U                                          /*!< I2C ADVANCED SETUP: SBCEN Bit Offset */
#define I2C_ADVANCED_SETUP_SBCEN_VAL(regval)                (BIT(1) & ((uint32_t)(regval) << 1))        /*!< I2C ADVANCED SETUP: SBCEN Bit Value */  
#define I2C_ADVANCED_SETUP_SBCEN_DISABLE                     0x0UL                                      /*!< DISABLE */
#define I2C_ADVANCED_SETUP_SBCEN_ENABLE                      BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C advanced_setup sbcen bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid sbcen bit.
  * \retval 1 This is a valid sbcen bit.
  */
#define IS_I2C_ADVANCED_SETUP_SBCEN(regval)                  (\
                                            ((regval) == I2C_ADVANCED_SETUP_SBCEN_DISABLE             ) || \
                                            ((regval) == I2C_ADVANCED_SETUP_SBCEN_ENABLE              )  \
                                                 )

#define I2C_ADVANCED_SETUP_PECEN                        BIT(2)                                      /*!< The packet error check enable: -1'b0:disable packet error check. -1'b1:enable packet error check. */
#define I2C_ADVANCED_SETUP_PECEN_OFS                    2U                                          /*!< I2C ADVANCED SETUP: PECEN Bit Offset */
#define I2C_ADVANCED_SETUP_PECEN_VAL(regval)                (BIT(2) & ((uint32_t)(regval) << 2))        /*!< I2C ADVANCED SETUP: PECEN Bit Value */  
#define I2C_ADVANCED_SETUP_PECEN_DISABLE                     0x0UL                                      /*!< DISABLE */
#define I2C_ADVANCED_SETUP_PECEN_ENABLE                      BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the I2C advanced_setup pecen bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid pecen bit.
  * \retval 1 This is a valid pecen bit.
  */
#define IS_I2C_ADVANCED_SETUP_PECEN(regval)                  (\
                                            ((regval) == I2C_ADVANCED_SETUP_PECEN_DISABLE             ) || \
                                            ((regval) == I2C_ADVANCED_SETUP_PECEN_ENABLE              )  \
                                                 )

#define I2C_ADVANCED_SETUP_PECIRQ                       BIT(3)                                      /*!< The packet error check byte request flag -1'b0:turn off the transmission of packet error check byte.  -1'b1:request the transmission of packet error check byte. Note: writting 0 is invalid.            this bit is invalid when continuous=1            this bit is invalid when sbc=1 This bit is cleared by hardware when PEC transmission is completed. */
#define I2C_ADVANCED_SETUP_PECIRQ_OFS                   3U                                          /*!< I2C ADVANCED SETUP: PECIRQ Bit Offset */
#define I2C_ADVANCED_SETUP_PECIRQ_VAL(regval)               (BIT(3) & ((uint32_t)(regval) << 3))        /*!< I2C ADVANCED SETUP: PECIRQ Bit Value */  
#define I2C_ADVANCED_SETUP_PECIRQ_TURN                       0x0UL                                      /*!< TURN */
#define I2C_ADVANCED_SETUP_PECIRQ_REQUEST                    BIT(3)                                                   /*!< REQUEST */

/**
  * \brief Check the I2C advanced_setup pecirq bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid pecirq bit.
  * \retval 1 This is a valid pecirq bit.
  */
#define IS_I2C_ADVANCED_SETUP_PECIRQ(regval)                 (\
                                           ((regval) == I2C_ADVANCED_SETUP_PECIRQ_TURN                ) || \
                                           ((regval) == I2C_ADVANCED_SETUP_PECIRQ_REQUEST             )  \
                                                 )

#define I2C_ADVANCED_SETUP_SMDEN                        BIT(4)                                      /*!< The SMBus default address enable: -1'b1:enable default address. -1'b0:disable default address. */
#define I2C_ADVANCED_SETUP_SMDEN_OFS                    4U                                          /*!< I2C ADVANCED SETUP: SMDEN Bit Offset */
#define I2C_ADVANCED_SETUP_SMDEN_VAL(regval)                (BIT(4) & ((uint32_t)(regval) << 4))        /*!< I2C ADVANCED SETUP: SMDEN Bit Value */  
#define I2C_ADVANCED_SETUP_SMDEN_ENABLE                      BIT(4)                                                   /*!< ENABLE */
#define I2C_ADVANCED_SETUP_SMDEN_DISABLE                     0x0UL                                      /*!< DISABLE */

/**
  * \brief Check the I2C advanced_setup smden bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid smden bit.
  * \retval 1 This is a valid smden bit.
  */
#define IS_I2C_ADVANCED_SETUP_SMDEN(regval)                  (\
                                            ((regval) == I2C_ADVANCED_SETUP_SMDEN_ENABLE              ) || \
                                            ((regval) == I2C_ADVANCED_SETUP_SMDEN_DISABLE             )  \
                                                 )

#define I2C_ADVANCED_SETUP_SMHEN                        BIT(5)                                      /*!< The SMBus host address enable: -1'b1:enable host address. -1'b0:disable host address. */
#define I2C_ADVANCED_SETUP_SMHEN_OFS                    5U                                          /*!< I2C ADVANCED SETUP: SMHEN Bit Offset */
#define I2C_ADVANCED_SETUP_SMHEN_VAL(regval)                (BIT(5) & ((uint32_t)(regval) << 5))        /*!< I2C ADVANCED SETUP: SMHEN Bit Value */  
#define I2C_ADVANCED_SETUP_SMHEN_ENABLE                      BIT(5)                                                   /*!< ENABLE */
#define I2C_ADVANCED_SETUP_SMHEN_DISABLE                     0x0UL                                      /*!< DISABLE */

/**
  * \brief Check the I2C advanced_setup smhen bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid smhen bit.
  * \retval 1 This is a valid smhen bit.
  */
#define IS_I2C_ADVANCED_SETUP_SMHEN(regval)                  (\
                                            ((regval) == I2C_ADVANCED_SETUP_SMHEN_ENABLE              ) || \
                                            ((regval) == I2C_ADVANCED_SETUP_SMHEN_DISABLE             )  \
                                                 )

#define I2C_ADVANCED_SETUP_TIDLEN                       BIT(6)                                      /*!< The SMBus IDLE timeout or SCL low timeout enable: -1'b1:IDLE timeout -1'b0:SCL low timeout */
#define I2C_ADVANCED_SETUP_TIDLEN_OFS                   6U                                          /*!< I2C ADVANCED SETUP: TIDLEN Bit Offset */
#define I2C_ADVANCED_SETUP_TIDLEN_VAL(regval)               (BIT(6) & ((uint32_t)(regval) << 6))        /*!< I2C ADVANCED SETUP: TIDLEN Bit Value */  
#define I2C_ADVANCED_SETUP_TIDLEN_IDLE                       BIT(6)                                                   /*!< IDLE */
#define I2C_ADVANCED_SETUP_TIDLEN_SCL                        0x0UL                                      /*!< SCL */

/**
  * \brief Check the I2C advanced_setup tidlen bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tidlen bit.
  * \retval 1 This is a valid tidlen bit.
  */
#define IS_I2C_ADVANCED_SETUP_TIDLEN(regval)                 (\
                                           ((regval) == I2C_ADVANCED_SETUP_TIDLEN_IDLE                ) || \
                                           ((regval) == I2C_ADVANCED_SETUP_TIDLEN_SCL                 )  \
                                                 )

#define I2C_ADVANCED_SETUP_TEXTEN                       BIT(7)                                      /*!< The SMBus stretch SCL low timeout enable: -1'b1:enable -1'b0:disable */
#define I2C_ADVANCED_SETUP_TEXTEN_OFS                   7U                                          /*!< I2C ADVANCED SETUP: TEXTEN Bit Offset */
#define I2C_ADVANCED_SETUP_TEXTEN_VAL(regval)               (BIT(7) & ((uint32_t)(regval) << 7))        /*!< I2C ADVANCED SETUP: TEXTEN Bit Value */  
#define I2C_ADVANCED_SETUP_TEXTEN_ENABLE                     BIT(7)                                                   /*!< ENABLE */
#define I2C_ADVANCED_SETUP_TEXTEN_DISABLE                    0x0UL                                      /*!< DISABLE */

/**
  * \brief Check the I2C advanced_setup texten bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid texten bit.
  * \retval 1 This is a valid texten bit.
  */
#define IS_I2C_ADVANCED_SETUP_TEXTEN(regval)                 (\
                                           ((regval) == I2C_ADVANCED_SETUP_TEXTEN_ENABLE              ) || \
                                           ((regval) == I2C_ADVANCED_SETUP_TEXTEN_DISABLE             )  \
                                                 )

#define I2C_ADVANCED_SETUP_TEXT_NUM_MASK                BITS(8,19)                                   /*!< I2C ADVANCED SETUP: TEXT_NUM Bit Mask */  
#define I2C_ADVANCED_SETUP_TEXT_NUM_OFS                 8U                                          /*!< I2C ADVANCED SETUP: TEXT_NUM Bit Offset */
#define I2C_ADVANCED_SETUP_TEXT_NUM(regval)             (BITS(8,19) & ((uint32_t)(regval) << 8))        /*!< I2C ADVANCED SETUP: TEXT_NUM Bit Value */  
#define I2C_ADVANCED_SETUP_ALERTEN                      BIT(20)                                      /*!< The SMBus alert transfer enable: -1'b1:enable -1'b0:disable */
#define I2C_ADVANCED_SETUP_ALERTEN_OFS                  20U                                          /*!< I2C ADVANCED SETUP: ALERTEN Bit Offset */
#define I2C_ADVANCED_SETUP_ALERTEN_VAL(regval)              (BIT(20) & ((uint32_t)(regval) << 20))        /*!< I2C ADVANCED SETUP: ALERTEN Bit Value */  
#define I2C_ADVANCED_SETUP_ALERTEN_ENABLE                    BIT(20)                                                  /*!< ENABLE */
#define I2C_ADVANCED_SETUP_ALERTEN_DISABLE                   0x0UL                                      /*!< DISABLE */

/**
  * \brief Check the I2C advanced_setup alerten bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid alerten bit.
  * \retval 1 This is a valid alerten bit.
  */
#define IS_I2C_ADVANCED_SETUP_ALERTEN(regval)                (\
                                          ((regval) == I2C_ADVANCED_SETUP_ALERTEN_ENABLE              ) || \
                                          ((regval) == I2C_ADVANCED_SETUP_ALERTEN_DISABLE             )  \
                                                 )

 /* ===== I2C ADVANCED_STATUS Register definition ===== */
#define I2C_ADVANCED_STATUS_PECERR                       BIT(0)                                      /*!< The packet error check status: -1'b0:correct PEC. -1'b1:incorrect PEC. */
#define I2C_ADVANCED_STATUS_PECERR_OFS                   0U                                          /*!< I2C ADVANCED STATUS: PECERR Bit Offset */
#define I2C_ADVANCED_STATUS_PECERR_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< I2C ADVANCED STATUS: PECERR Bit Value */  

/**
  * \brief Check the I2C advanced_status pecerr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid pecerr bit.
  * \retval 1 This is a valid pecerr bit.
  */
#define IS_I2C_ADVANCED_STATUS_PECERR(regval)                 (\
                                                 )

#define I2C_ADVANCED_STATUS_PECBYTE                      BITS(1,8)                
#define I2C_ADVANCED_STATUS_ALERT_STUS                   BIT(9)                                      /*!< The SMBus alert status: -1'b1:smbus alert detected -1'b0:smbus not alert detected */
#define I2C_ADVANCED_STATUS_ALERT_STUS_OFS               9U                                          /*!< I2C ADVANCED STATUS: ALERT_STUS Bit Offset */
#define I2C_ADVANCED_STATUS_ALERT_STUS_VAL(regval)           (BIT(9) & ((uint32_t)(regval) << 9))        /*!< I2C ADVANCED STATUS: ALERT_STUS Bit Value */  

/**
  * \brief Check the I2C advanced_status alert_stus bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid alert_stus bit.
  * \retval 1 This is a valid alert_stus bit.
  */
#define IS_I2C_ADVANCED_STATUS_ALERT_STUS(regval)             (\
                                                 )

#define I2C_ADVANCED_STATUS_SBC_TCR                      BIT(10)                                      /*!< Slave byte control transfer done flag: -1'b1:transfer done -1'b0:transfer not done */
#define I2C_ADVANCED_STATUS_SBC_TCR_OFS                  10U                                          /*!< I2C ADVANCED STATUS: SBC_TCR Bit Offset */
#define I2C_ADVANCED_STATUS_SBC_TCR_VAL(regval)              (BIT(10) & ((uint32_t)(regval) << 10))        /*!< I2C ADVANCED STATUS: SBC_TCR Bit Value */  

/**
  * \brief Check the I2C advanced_status sbc_tcr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid sbc_tcr bit.
  * \retval 1 This is a valid sbc_tcr bit.
  */
#define IS_I2C_ADVANCED_STATUS_SBC_TCR(regval)                (\
                                                 )

#define I2C_ADVANCED_STATUS_PEC_BYTE_TC                  BIT(11)                                      /*!< PEC byte control transfer done flag: -1'b1:transfer done -1'b0:transfer not done */
#define I2C_ADVANCED_STATUS_PEC_BYTE_TC_OFS              11U                                          /*!< I2C ADVANCED STATUS: PEC_BYTE_TC Bit Offset */
#define I2C_ADVANCED_STATUS_PEC_BYTE_TC_VAL(regval)          (BIT(11) & ((uint32_t)(regval) << 11))        /*!< I2C ADVANCED STATUS: PEC_BYTE_TC Bit Value */  

/**
  * \brief Check the I2C advanced_status pec_byte_tc bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid pec_byte_tc bit.
  * \retval 1 This is a valid pec_byte_tc bit.
  */
#define IS_I2C_ADVANCED_STATUS_PEC_BYTE_TC(regval)            (\
                                                 )

/* I2C control enable status input parameter check */
#define IS_I2C_ControlStatus(status)          (((status) == DISABLE) || ((status) == ENABLE))
#define IS_I2C_SPEED(speed)     (((speed) == I2C_SPEED_100K) || ((speed) == I2C_SPEED_400K) || ((speed) == I2C_SPEED_1M) || ((speed) == I2C_SPEED_4M))
#define ROUND(x) ((int)((x) + 0.5))
#define I2C_CHECK_MST_STATE(I2Cx, MST_STATE)  (((I2Cx)->STATUS & I2C_STATUS_MASTER_STATE) == MST_STATE)
void I2C_Restart_Cmd(void);
void I2C_Calcu_Time(I2C_TypeDef* I2Cx, I2C_InitTypeDef* init);
void I2C_Set_Time(I2C_TypeDef* I2Cx, I2C_InitTypeDef* init);
void I2C_TimeOut_En(I2C_TypeDef* I2Cx, ControlStatus status);
void I2C_TimeOut_Cfg(I2C_TypeDef* I2Cx, uint16_t num);
void I2C_Interrupt_En(I2C_TypeDef* I2Cx, uint32_t interrupt, ControlStatus status);
void I2C_IntStatus_Clear(I2C_TypeDef* I2Cx, uint32_t interrupt);
FlagStatus I2C_Get_IntStatus(I2C_TypeDef* I2Cx, uint32_t interrupt);
void I2C_SoftReset_En(I2C_TypeDef* I2Cx);
void I2C_Dma_Cfg(I2C_TypeDef* I2Cx, ControlStatus status);
FlagStatus I2C_Get_Status(I2C_TypeDef* I2Cx, uint32_t status);
I2C_StateTypedef I2C_Start(I2C_TypeDef* I2Cx);
void I2C_Restart(I2C_TypeDef* I2Cx);
void I2C_Stop(I2C_TypeDef* I2Cx);
I2C_StateTypedef I2C_WaitStopOver(I2C_TypeDef* I2Cx);
void I2C_ACK(I2C_TypeDef* I2Cx, ControlStatus status);
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* init);
I2C_StateTypedef I2C_Send_SlaveAddr(I2C_TypeDef* I2Cx, uint8_t addr, I2C_DIRTypedef dir, uint32_t timeout);
I2C_StateTypedef I2C_Write_Done(I2C_TypeDef* I2Cx, uint8_t data, uint32_t timeout);
I2C_StateTypedef I2C_Write(I2C_TypeDef* I2Cx, uint8_t data, uint32_t timeout);
I2C_StateTypedef I2C_Read(I2C_TypeDef* I2Cx, uint8_t* data, uint32_t timeout);
void I2C_Write_En(I2C_TypeDef* I2Cx);
void I2C_Read_En(I2C_TypeDef* I2Cx);
void I2C_AutoEnd_En(I2C_TypeDef* I2Cx, ControlStatus status);
void I2C_Clear_Status(I2C_TypeDef* I2Cx, uint32_t status);

void I2C_Set_RxSize(I2C_TypeDef* I2Cx,uint32_t rx_size);
uint32_t I2C_Get_RxSize(I2C_TypeDef* I2Cx);
void I2C_Set_TxSize(I2C_TypeDef* I2Cx,uint32_t tx_size);
uint32_t I2C_Get_TxSize(I2C_TypeDef* I2Cx);

void I2C_Set_RxWaterMark(I2C_TypeDef* I2Cx,uint8_t rx_watermark);
uint8_t I2C_Get_RxWaterMark(I2C_TypeDef* I2Cx);
void I2C_Set_TxWaterMark(I2C_TypeDef* I2Cx,uint8_t tx_watermark);
uint8_t I2C_Get_TxWaterMark(I2C_TypeDef* I2Cx);

/*==== The following API is I2C usage example ====*/

uint32_t I2C_MasterWriteBytes(I2C_TypeDef* I2Cx, uint8_t* data, uint32_t bytes);
uint32_t I2C_MasterReadBytes(I2C_TypeDef* I2Cx, uint8_t* data, uint32_t bytes);

/*==== The following API is for I2C Test Case ====*/
I2C_StateTypedef I2C_MasterRecvSlaveSend_Onebyone(I2C_TypeDef* I2Cx_m, I2C_TypeDef* I2Cx_s, uint8_t slave_addr, uint8_t* send, uint8_t* recv, uint32_t len);
I2C_StateTypedef I2C_MasterSendSlaveRecv_Onebyone(I2C_TypeDef* I2Cx_m, I2C_TypeDef* I2Cx_s, uint8_t slave_addr, uint8_t* send, uint8_t* recv, uint32_t len);
I2C_StateTypedef I2C_MasterRecvSlaveSend_10bit(I2C_TypeDef* I2Cx_m, I2C_TypeDef* I2Cx_s, uint16_t slave_addr, uint8_t* send, uint8_t* recv, uint32_t len);
I2C_StateTypedef I2C_MasterSendSlaveRecv_10bit(I2C_TypeDef* I2Cx_m, I2C_TypeDef* I2Cx_s, uint16_t slave_addr, uint8_t* send, uint8_t* recv, uint32_t len);

void I2C_TxDma_Start(I2C_TypeDef* I2Cx, ControlStatus status);
void I2C_RxDma_Start(I2C_TypeDef* I2Cx, ControlStatus status);
void I2C_TxDma_ContinuousEn(I2C_TypeDef* I2Cx, ControlStatus status);
void I2C_RxDma_ContinuousEn(I2C_TypeDef* I2Cx, ControlStatus status);
void I2C_TxChannel_Clear(I2C_TypeDef* I2Cx);
void I2C_RxChannel_Clear(I2C_TypeDef* I2Cx);

/*==== The following API is I2C adv feature ====*/
FlagStatus I2C_Get_AdvStatus(I2C_TypeDef* I2Cx, uint32_t status);
void I2C_Clear_AdvStatus(I2C_TypeDef* I2Cx, uint32_t status);
void I2C_PEC_Request(I2C_TypeDef* I2Cx);
uint8_t I2C_Get_PECode(I2C_TypeDef* I2Cx);
void I2C_Slave_Send_Alert(I2C_TypeDef* I2Cx);
#ifdef __cplusplus
}
#endif

#endif /* _NS_I2C_H */

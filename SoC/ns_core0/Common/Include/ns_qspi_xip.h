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

#ifndef _NS_QSPI_XIP_H_
#define _NS_QSPI_XIP_H_
/*!
 * @file     ns_qspi_xip.h
 * @brief    This file contains all the functions prototypes for the QSPI_XIP firmware
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ns.h"

#define QSPI_XIP_BUF_DEPTH 4

/** 
  * \brief  QSPI_XIP proto mode enum definition
  */
typedef enum {
    QSPI_XIP_SINGLE_LINE = 0,
    QSPI_XIP_DUAL_LINE = 1,
    QSPI_XIP_QUAD_LINE = 2,
    QSPI_XIP_OCTOL_LINE = 3,
    QSPI_XIP_X16_LINE = 4
} QSPI_XIP_ProtoTypedef;

#define DADA_PROTO_LO   0x3
#define DADA_PROTO_HI   0x4

/** 
  * \brief  QSPI_XIP endian enum definition
  */
typedef enum {
    QSPI_XIP_ENDIAN_MSB = 0,
    QSPI_XIP_ENDIAN_LSB = 1,
} QSPI_XIP_Endianypedef;

#define QSPI_XIP_3BYTE_MODE    3
#define QSPI_XIP_4BYTE_MODE    4

/**
  * \brief  QSPI_XIP Init Structure definition
  */
typedef struct {
    uint32_t SCKDIV;                                    /*!< Specifies the QSPI_XIP Clock Divider */

    uint32_t DevMode;                                   /*!< Specifies the QSPI_XIP Operating Mode : master/slave */

    uint32_t CSMode;                                    /*!< Specifies the QSPI_XIP Chip Select Mode */
    uint32_t CPOL;                                      /*!< Specifies the QSPI_XIP Serial Clock Steady State */
    uint32_t CPHA;                                      /*!< Specifies the QSPI_XIP Clock Active Edge for the Bit Capture */

    uint32_t SSM;                                       /*!< Specifies the Software slave management */

    uint32_t ProtolMode;                                /*!< Specifies the QSPI_XIP Protocol */
    uint32_t Endian;                                    /*!< Specifies the QSPI_XIP Big Endian or Little Endian */
    uint32_t DataSize;                                  /*!< Specifies the QSPI_XIP Data Size */
    uint32_t Force;                                     /*!< Specifies the QSPI_XIP OE Crtl when not Use Other Pad */
} QSPI_XIP_InitTypeDef;

/**
  * \brief  QSPI_XIP ddr enable structure definition
  */
typedef struct {
    ControlStatus CMD_DDR_EN;                                   /*!< xip cmd ddr enable */
    ControlStatus ADDR_DDR_EN;                                  /*!< xip addr ddr enable */
    ControlStatus DATA_DDR_EN;                                  /*!< data cmd ddr enable */
    ControlStatus MODE_DDR_EN;                                  /*!< mode cmd ddr enable */
} QSPI_XIP_DdrEnableTypedef;

typedef struct {
    union {
        uint32_t reg_ffmt;
        struct {
            uint32_t spi_cmd_en         :1;             /*!< Enable sending of command */
            uint32_t spi_addr_len       :3;             /*!< Number of address bytes */
            uint32_t spi_pad_cnt        :4;             /*!< Number of dummy cycles */
            uint32_t spi_cmd_proto      :2;             /*!< Protocol for transmitting command */
            uint32_t spi_addr_proto     :2;             /*!< Protocol for transmitting address and padding */
            uint32_t spi_data_proto     :2;             /*!< Protocol for receiving data bytes(low bit) */
            uint32_t spi_endina_f       :1;             /*!< SPI xip endianness */
            uint32_t spi_data_proto_hi  :1;             /*!< Protocol for receiving data bytes(high bit) */
            uint32_t spi_cmd_code       :8;             /*!< Value of command byte */
            uint32_t spi_pad_code       :8;             /*!< First 8 bits to transmit during dummy cycles */
        } bit_ffmt;
    } QSPI_XIP_FFMT_u;

    union {
        uint32_t reg_ffmt1;
        struct {
            uint32_t spi_wcmd_code      :8;             /*!< Value of command byte */
            uint32_t spi_wpad_cnt       :5;             /*!< Number of dummy cycles */
            uint32_t spi_pad_cnt_h      :1;             /*!< dummy cycle high bit */
            uint32_t spi_ddr_en         :4;             /*!< cmd [14], addr[15], mode[16], data[17](ddr mode ctrl) */
            uint32_t spi_mode_proto     :2;             /*!< Protocol for transmitting mode */
            uint32_t spi_mode_code      :8;             /*!< Value of mode byte */
            uint32_t spi_mode_cnt       :4;             /*!< Number of mode bit len for xip(diff to other cnt) */
        } bit_ffmt1;
    } QSPI_XIP_FFMT1_u;

} QSPI_XIP_TimingTypedef;

/**
 * \brief Check the QSPI_XIP periphral parameters.
 * \param PERIPH QSPI_XIP periphral to be checked.
 * \retval 0 This is not a valid QSPI_XIP periphral.
 * \retval 1 This is a valid QSPI_XIP periphral.
 */
#define IS_QSPI_XIP_ALL_PERIPH(PERIPH)                     (((PERIPH) == QSPI_XIP0) || ((PERIPH) == QSPI1) || ((PERIPH) == QSPI2) || ((PERIPH) == QSPI3))
#define IS_QSPI_XIP_TX_RX_WaterMark(WaterMark)             ((WaterMark) <= QSPI_XIP_BUF_DEPTH)                    /*!< QSPI_XIP WaterMark[4:0] */
#define IS_FUNCTIONAL_STATE(Status)                                     (((Status) == ENABLE) || ((Status) == DISABLE))
#define IS_QSPI_XIP_DATA(DATA)                             ((DATA) <= 0xFFFFFFFF)                             /*!< USART Transmit/Receive data[31:0] */

#define QSPI_XIP_TX_FIFO   0
#define QSPI_XIP_RX_FIFO   1

#define QSPI_XIP_SCKDIV_OFFSET                                             0x0 /*!< Serial clock divisor */
#define QSPI_XIP_SCKMODE_OFFSET                                            0x4 /*!< Serial clock mode (master/slave) */
#define QSPI_XIP_DDR_SCKSAMPLE_OFFSET                                      0x8 /*!< SPI data sampling divisor (ddr mode) */
#define QSPI_XIP_FORCE_OFFSET                                              0xc /*!< SPI oe crtl when not use other pad */
#define QSPI_XIP_CSID_OFFSET                                               0x10 /*!< Chip select ID */
#define QSPI_XIP_CSDEF_OFFSET                                              0x14 /*!< Chip select default */
#define QSPI_XIP_CSMODE_OFFSET                                             0x18 /*!< Chip select mode */
#define QSPI_XIP_VERSION_OFFSET                                            0x1c /*!< Indicate version 2.2.3 */
#define QSPI_XIP_ADDR_WRAP_OFFSET                                          0x20 /*!< SPI flash offset address  */
#define QSPI_XIP_BOUNDARY_CFG_OFFSET                                       0x24 /*!< SPI flash boundry address config */
#define QSPI_XIP_DELAY0_OFFSET                                             0x28 /*!< SPI_DELAY0,Delay control 0 */
#define QSPI_XIP_DELAY1_OFFSET                                             0x2c /*!< SPI_DELAY1,Delay control 1 */
#define QSPI_XIP_FIFO_NUM_OFFSET                                           0x30 /*!< SPI fifo data valid number */
#define QSPI_XIP_TSIZE_OFFSET                                              0x34 /*!< TX SPI dma size configuration register */
#define QSPI_XIP_RSIZE_OFFSET                                              0x38 /*!< RX SPI dma size configuration register */
#define QSPI_XIP_FMT_OFFSET                                                0x40 /*!< Frame format(master/slave)(normal mode non-xip) */
#define QSPI_XIP_TXDATA_OFFSET                                             0x48 /*!< Tx FIFO Data(master/slave) */
#define QSPI_XIP_RXDATA_OFFSET                                             0x4c /*!< Rx FIFO data(master/slave) */
#define QSPI_XIP_TX_MARK_OFFSET                                            0x50 /*!< Tx FIFO watermark(master/slave)(normal mode non-xip) */
#define QSPI_XIP_RX_MARK_OFFSET                                            0x54 /*!< Rx FIFO watermark(master/slave)(normal mode non-xip) */
#define QSPI_XIP_FCTRL_OFFSET                                              0x60 /*!< SPI XIP interface control* (XIP mode) */
#define QSPI_XIP_FFMT_OFFSET                                               0x64 /*!< SPI XIP instruction format*(XIP mode) */
#define QSPI_XIP_IE_OFFSET                                                 0x70 /*!< SPI interrupt enable(master/slave)(normal mode non-xip) */
#define QSPI_XIP_IP_OFFSET                                                 0x74 /*!< SPI interrupt pending(master/slave)(normal mode non-xip) */
#define QSPI_XIP_FFMT1_OFFSET                                              0x78 /*!< SPI flash instruction format1*(XIP mode) */
#define QSPI_XIP_STATUS_OFFSET                                             0x7c /*!< SPI busy status(normal mode non-xip) */
#define QSPI_XIP_SDR_SCKSAMPLE_OFFSET                                      0x80 /*!< SPI data sampling divisor (sdr mode) */
#define QSPI_XIP_CR_OFFSET                                                 0x84 /*!< SPI control register */
#define QSPI_XIP_SYS_DELAY_OFFSET                                          0xd0 /*!< SPI system clk delay config register */

 /* ===== QSPI XIP SCKDIV Register definition ===== */
#define QSPI_XIP_SCKDIV_PRESCALER_MASK               BITS(0,11)                                   /*!< QSPI XIP SCKDIV: PRESCALER Bit Mask */  
#define QSPI_XIP_SCKDIV_PRESCALER_OFS                0U                                          /*!< QSPI XIP SCKDIV: PRESCALER Bit Offset */
#define QSPI_XIP_SCKDIV_PRESCALER(regval)            (BITS(0,11) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP SCKDIV: PRESCALER Bit Value */  
#define QSPI_XIP_SCKDIV_PRESCALER_2                       QSPI_XIP_SCKDIV_PRESCALER(0)                                                /*!< 2 */
#define QSPI_XIP_SCKDIV_PRESCALER_4                       QSPI_XIP_SCKDIV_PRESCALER(1)                                                /*!< 4 */
#define QSPI_XIP_SCKDIV_PRESCALER_6                       QSPI_XIP_SCKDIV_PRESCALER(2)                                                /*!< 6 */
#define QSPI_XIP_SCKDIV_PRESCALER_8                       QSPI_XIP_SCKDIV_PRESCALER(3)                                                /*!< 8 */
#define QSPI_XIP_SCKDIV_PRESCALER_10                      QSPI_XIP_SCKDIV_PRESCALER(4)                                                /*!< 10 */
#define QSPI_XIP_SCKDIV_PRESCALER_12                      QSPI_XIP_SCKDIV_PRESCALER(5)                                                /*!< 12 */
#define QSPI_XIP_SCKDIV_PRESCALER_14                      QSPI_XIP_SCKDIV_PRESCALER(6)                                                /*!< 14 */
#define QSPI_XIP_SCKDIV_PRESCALER_16                      QSPI_XIP_SCKDIV_PRESCALER(7)                                                /*!< 16 */
#define QSPI_XIP_SCKDIV_PRESCALER_32                      QSPI_XIP_SCKDIV_PRESCALER(15)                                                /*!< 32 */
#define QSPI_XIP_SCKDIV_PRESCALER_64                      QSPI_XIP_SCKDIV_PRESCALER(31)                                                /*!< 64 */

/**
  * \brief Check the QSPI_XIP spi_sckdiv prescaler bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid prescaler bits.
  * \retval 1 This is a valid prescaler bits.
  */
#define IS_QSPI_XIP_SPI_SCKDIV_PRESCALER(regval)              (\
                                        ((regval) == QSPI_XIP_SCKDIV_PRESCALER_2                   ) || \
                                        ((regval) == QSPI_XIP_SCKDIV_PRESCALER_4                   ) || \
                                        ((regval) == QSPI_XIP_SCKDIV_PRESCALER_6                   ) || \
                                        ((regval) == QSPI_XIP_SCKDIV_PRESCALER_8                   ) || \
                                        ((regval) == QSPI_XIP_SCKDIV_PRESCALER_10                  ) || \
                                        ((regval) == QSPI_XIP_SCKDIV_PRESCALER_12                  ) || \
                                        ((regval) == QSPI_XIP_SCKDIV_PRESCALER_14                  ) || \
                                        ((regval) == QSPI_XIP_SCKDIV_PRESCALER_16                  ) || \
                                        ((regval) == QSPI_XIP_SCKDIV_PRESCALER_32                  ) || \
                                        ((regval) == QSPI_XIP_SCKDIV_PRESCALER_64                  )  \
                                                 )

 /* ===== QSPI XIP SCKMODE Register definition ===== */
#define QSPI_XIP_SCKMODE_CPHA                         BIT(0)                                      /*!< Serial clock phase - 1'b0: edge1 - 1'b1: edge2 Usage scenario: If you want to change the SPI phase, you can control it through this register .Note not support ddr mode */
#define QSPI_XIP_SCKMODE_CPHA_OFS                     0U                                          /*!< QSPI XIP SCKMODE: CPHA Bit Offset */
#define QSPI_XIP_SCKMODE_CPHA_VAL(regval)                 (BIT(0) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP SCKMODE: CPHA Bit Value */  
#define QSPI_XIP_SCKMODE_CPHA_EDGE1                        0x0UL                                             /*!< EDGE1 */
#define QSPI_XIP_SCKMODE_CPHA_EDGE2                        BIT(0)                                                   /*!< EDGE2 */

/**
  * \brief Check the QSPI_XIP spi_sckmode cpha bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cpha bit.
  * \retval 1 This is a valid cpha bit.
  */
#define IS_QSPI_XIP_SPI_SCKMODE_CPHA(regval)                   (\
                                             ((regval) == QSPI_XIP_SCKMODE_CPHA_EDGE1               ) || \
                                             ((regval) == QSPI_XIP_SCKMODE_CPHA_EDGE2               )  \
                                                 )

#define QSPI_XIP_SCKMODE_CPOL                         BIT(1)                                      /*!< Serial clock polarity - 1'b0: low levle is idle state of SCK - 1'b1: high levle is idle state of SCK Usage scenario: If you want to change the polarity of SPI, you can control it through this register . Note not support ddr mode */
#define QSPI_XIP_SCKMODE_CPOL_OFS                     1U                                          /*!< QSPI XIP SCKMODE: CPOL Bit Offset */
#define QSPI_XIP_SCKMODE_CPOL_VAL(regval)                 (BIT(1) & ((uint32_t)(regval) << 1))        /*!< QSPI XIP SCKMODE: CPOL Bit Value */  
#define QSPI_XIP_SCKMODE_CPOL_LOW                          0x0UL                                             /*!< LOW */
#define QSPI_XIP_SCKMODE_CPOL_HIGH                         BIT(1)                                                   /*!< HIGH */

/**
  * \brief Check the QSPI_XIP spi_sckmode cpol bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cpol bit.
  * \retval 1 This is a valid cpol bit.
  */
#define IS_QSPI_XIP_SPI_SCKMODE_CPOL(regval)                   (\
                                             ((regval) == QSPI_XIP_SCKMODE_CPOL_LOW                 ) || \
                                             ((regval) == QSPI_XIP_SCKMODE_CPOL_HIGH                )  \
                                                 )

 /* ===== QSPI XIP DDR_SCKSAMPLE Register definition ===== */
#define QSPI_XIP_DDR_SCKSAMPLE_SCKSAMPLE_MASK               BITS(0,11)                                   /*!< QSPI XIP DDR SCKSAMPLE: SCKSAMPLE Bit Mask */  
#define QSPI_XIP_DDR_SCKSAMPLE_SCKSAMPLE_OFS                0U                                          /*!< QSPI XIP DDR SCKSAMPLE: SCKSAMPLE Bit Offset */
#define QSPI_XIP_DDR_SCKSAMPLE_SCKSAMPLE(regval)            (BITS(0,11) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP DDR SCKSAMPLE: SCKSAMPLE Bit Value */  
 
 /* ===== QSPI XIP FORCE Register definition ===== */
#define QSPI_XIP_FORCE_EN                           BIT(0)                                      /*!< SPI oe crtl when not use other pad Note:If the register switch is turned on, except for the WP pad, all unused SPI pads will be forced to output 1, and the output value of the WP pad will be determined by spi_force_wp */
#define QSPI_XIP_FORCE_EN_OFS                       0U                                          /*!< QSPI XIP FORCE: EN Bit Offset */
#define QSPI_XIP_FORCE_EN_VAL(regval)                   (BIT(0) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP FORCE: EN Bit Value */  
#define QSPI_XIP_FORCE_WP                           BIT(1)                                      /*!< SPI write protect ctrl Note:This register determines the value output by the WP pad when force is in effect */
#define QSPI_XIP_FORCE_WP_OFS                       1U                                          /*!< QSPI XIP FORCE: WP Bit Offset */
#define QSPI_XIP_FORCE_WP_VAL(regval)                   (BIT(1) & ((uint32_t)(regval) << 1))        /*!< QSPI XIP FORCE: WP Bit Value */  
 
 /* ===== QSPI XIP CSID Register definition ===== */
#define QSPI_XIP_CSID_NUM_MASK                     BITS(0,2)                                   /*!< QSPI XIP CSID: NUM Bit Mask */  
#define QSPI_XIP_CSID_NUM_OFS                      0U                                          /*!< QSPI XIP CSID: NUM Bit Offset */
#define QSPI_XIP_CSID_NUM(regval)                  (BITS(0,2) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP CSID: NUM Bit Value */  
#define QSPI_XIP_CSID_NUM_CS                            QSPI_XIP_CSID_NUM(0)                                                  /*!< CS */
#define QSPI_XIP_CSID_NUM_CS0                           QSPI_XIP_CSID_NUM(1)                                                  /*!< CS0 */
#define QSPI_XIP_CSID_NUM_CS1                           QSPI_XIP_CSID_NUM(2)                                                  /*!< CS1 */
#define QSPI_XIP_CSID_NUM_CS2                           QSPI_XIP_CSID_NUM(3)                                                  /*!< CS2 */
#define QSPI_XIP_CSID_NUM_CS3                           QSPI_XIP_CSID_NUM(4)                                                  /*!< CS3 */

/**
  * \brief Check the QSPI_XIP spi_csid num bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid num bits.
  * \retval 1 This is a valid num bits.
  */
#define IS_QSPI_XIP_SPI_CSID_NUM(regval)                    (\
                                              ((regval) == QSPI_XIP_CSID_NUM_CS                  ) || \
                                              ((regval) == QSPI_XIP_CSID_NUM_CS0                 ) || \
                                              ((regval) == QSPI_XIP_CSID_NUM_CS1                 ) || \
                                              ((regval) == QSPI_XIP_CSID_NUM_CS2                 ) || \
                                              ((regval) == QSPI_XIP_CSID_NUM_CS3                 )  \
                                                 )

 /* ===== QSPI XIP CSDEF Register definition ===== */
#define QSPI_XIP_CSDEF_WIDTH_MASK                   BITS(0,3)                                   /*!< QSPI XIP CSDEF: WIDTH Bit Mask */  
#define QSPI_XIP_CSDEF_WIDTH_OFS                    0U                                          /*!< QSPI XIP CSDEF: WIDTH Bit Offset */
#define QSPI_XIP_CSDEF_WIDTH(regval)                (BITS(0,3) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP CSDEF: WIDTH Bit Value */  
 
 /* ===== QSPI XIP CSMODE Register definition ===== */
#define QSPI_XIP_CSMODE_MODE_MASK                    BITS(0,1)                                   /*!< QSPI XIP CSMODE: MODE Bit Mask */  
#define QSPI_XIP_CSMODE_MODE_OFS                     0U                                          /*!< QSPI XIP CSMODE: MODE Bit Offset */
#define QSPI_XIP_CSMODE_MODE(regval)                 (BITS(0,1) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP CSMODE: MODE Bit Value */  
#define QSPI_XIP_CSMODE_MODE_AUTO                         QSPI_XIP_CSMODE_MODE(0)                                                /*!< AUTO */
#define QSPI_XIP_CSMODE_MODE_HOLD                         QSPI_XIP_CSMODE_MODE(2)                                                /*!< HOLD */
#define QSPI_XIP_CSMODE_MODE_OFF                          QSPI_XIP_CSMODE_MODE(3)                                                /*!< OFF */

/**
  * \brief Check the QSPI_XIP spi_csmode mode bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid mode bits.
  * \retval 1 This is a valid mode bits.
  */
#define IS_QSPI_XIP_SPI_CSMODE_MODE(regval)                   (\
                                             ((regval) == QSPI_XIP_CSMODE_MODE_AUTO                ) || \
                                             ((regval) == QSPI_XIP_CSMODE_MODE_HOLD                ) || \
                                             ((regval) == QSPI_XIP_CSMODE_MODE_OFF                 )  \
                                                 )

 /* ===== QSPI XIP VERSION Register definition ===== */
#define QSPI_XIP_VERSION_NUM                          BITS(0,31)                
 
 /* ===== QSPI XIP ADDR_WRAP Register definition ===== */
#define QSPI_XIP_ADDR_WRAP_OFFSET_ADDR_MASK             BITS(0,31)                                   /*!< QSPI XIP ADDR WRAP: OFFSET_ADDR Bit Mask */  
#define QSPI_XIP_ADDR_WRAP_OFFSET_ADDR_OFS              0U                                          /*!< QSPI XIP ADDR WRAP: OFFSET_ADDR Bit Offset */
#define QSPI_XIP_ADDR_WRAP_OFFSET_ADDR(regval)          (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP ADDR WRAP: OFFSET_ADDR Bit Value */  
 
 /* ===== QSPI XIP BOUNDARY_CFG Register definition ===== */
#define QSPI_XIP_BOUNDARY_CFG_ADDR_MASK                    BITS(0,31)                                   /*!< QSPI XIP BOUNDARY CFG: ADDR Bit Mask */  
#define QSPI_XIP_BOUNDARY_CFG_ADDR_OFS                     0U                                          /*!< QSPI XIP BOUNDARY CFG: ADDR Bit Offset */
#define QSPI_XIP_BOUNDARY_CFG_ADDR(regval)                 (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP BOUNDARY CFG: ADDR Bit Value */  
 
 /* ===== QSPI XIP DELAY0 Register definition ===== */
#define QSPI_XIP_DELAY0_CSSCK_MASK                   BITS(0,7)                                   /*!< QSPI XIP DELAY0: CSSCK Bit Mask */  
#define QSPI_XIP_DELAY0_CSSCK_OFS                    0U                                          /*!< QSPI XIP DELAY0: CSSCK Bit Offset */
#define QSPI_XIP_DELAY0_CSSCK(regval)                (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP DELAY0: CSSCK Bit Value */  
#define QSPI_XIP_DELAY0_SCKCS_MASK                   BITS(16,23)                                   /*!< QSPI XIP DELAY0: SCKCS Bit Mask */  
#define QSPI_XIP_DELAY0_SCKCS_OFS                    16U                                          /*!< QSPI XIP DELAY0: SCKCS Bit Offset */
#define QSPI_XIP_DELAY0_SCKCS(regval)                (BITS(16,23) & ((uint32_t)(regval) << 16))        /*!< QSPI XIP DELAY0: SCKCS Bit Value */  
 
 /* ===== QSPI XIP DELAY1 Register definition ===== */
#define QSPI_XIP_DELAY1_INTERCS_MASK                 BITS(0,7)                                   /*!< QSPI XIP DELAY1: INTERCS Bit Mask */  
#define QSPI_XIP_DELAY1_INTERCS_OFS                  0U                                          /*!< QSPI XIP DELAY1: INTERCS Bit Offset */
#define QSPI_XIP_DELAY1_INTERCS(regval)              (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP DELAY1: INTERCS Bit Value */  
#define QSPI_XIP_DELAY1_INTERXFR_MASK                BITS(16,23)                                   /*!< QSPI XIP DELAY1: INTERXFR Bit Mask */  
#define QSPI_XIP_DELAY1_INTERXFR_OFS                 16U                                          /*!< QSPI XIP DELAY1: INTERXFR Bit Offset */
#define QSPI_XIP_DELAY1_INTERXFR(regval)             (BITS(16,23) & ((uint32_t)(regval) << 16))        /*!< QSPI XIP DELAY1: INTERXFR Bit Value */  
 
 /* ===== QSPI XIP FIFO_NUM Register definition ===== */
#define QSPI_XIP_FIFO_NUM_TX                           BITS(0,2)                
#define QSPI_XIP_FIFO_NUM_RX                           BITS(16,18)                
 
 /* ===== QSPI XIP TSIZE Register definition ===== */
#define QSPI_XIP_TSIZE_NUM_MASK                     BITS(0,31)                                   /*!< QSPI XIP TSIZE: NUM Bit Mask */  
#define QSPI_XIP_TSIZE_NUM_OFS                      0U                                          /*!< QSPI XIP TSIZE: NUM Bit Offset */
#define QSPI_XIP_TSIZE_NUM(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP TSIZE: NUM Bit Value */  
 
 /* ===== QSPI XIP RSIZE Register definition ===== */
#define QSPI_XIP_RSIZE_NUM_MASK                     BITS(0,31)                                   /*!< QSPI XIP RSIZE: NUM Bit Mask */  
#define QSPI_XIP_RSIZE_NUM_OFS                      0U                                          /*!< QSPI XIP RSIZE: NUM Bit Offset */
#define QSPI_XIP_RSIZE_NUM(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP RSIZE: NUM Bit Value */  
 
 /* ===== QSPI XIP FMT Register definition ===== */
#define QSPI_XIP_FMT_PROTO_MASK                   BITS(0,1)                                   /*!< QSPI XIP FMT: PROTO Bit Mask */  
#define QSPI_XIP_FMT_PROTO_OFS                    0U                                          /*!< QSPI XIP FMT: PROTO Bit Offset */
#define QSPI_XIP_FMT_PROTO(regval)                (BITS(0,1) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP FMT: PROTO Bit Value */  
#define QSPI_XIP_FMT_PROTO_SINGLE                      QSPI_XIP_FMT_PROTO(0)                                                   /*!< SINGLE */
#define QSPI_XIP_FMT_PROTO_DUAL                        QSPI_XIP_FMT_PROTO(1)                                                   /*!< DUAL */
#define QSPI_XIP_FMT_PROTO_QUAD                        QSPI_XIP_FMT_PROTO(2)                                                   /*!< QUAD */
#define QSPI_XIP_FMT_PROTO_OCTOL                       QSPI_XIP_FMT_PROTO(3)                                                   /*!< OCTOL */
#define QSPI_XIP_FMT_PROTO_X16                         QSPI_XIP_FMT_PROTO(4)                                                   /*!< X16 */
#define QSPI_XIP_FMT_PROTO_HDS                         QSPI_XIP_FMT_PROTO(7)                                                   /*!< HDS */

/**
  * \brief Check the QSPI_XIP spi_fmt proto bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid proto bits.
  * \retval 1 This is a valid proto bits.
  */
#define IS_QSPI_XIP_SPI_FMT_PROTO(regval)                  (\
                                            ((regval) == QSPI_XIP_FMT_PROTO_SINGLE              ) || \
                                            ((regval) == QSPI_XIP_FMT_PROTO_DUAL                ) || \
                                            ((regval) == QSPI_XIP_FMT_PROTO_QUAD                ) || \
                                            ((regval) == QSPI_XIP_FMT_PROTO_OCTOL               ) || \
                                            ((regval) == QSPI_XIP_FMT_PROTO_X16                 ) || \
                                            ((regval) == QSPI_XIP_FMT_PROTO_HDS                 )  \
                                                 )

#define QSPI_XIP_FMT_ENDIAN                       BIT(2)                                      /*!< SPI endianness bit:(normal mode non-xip) - 1'b0: MSB: Transmit bit most-significant bit (MSB) first - 1'b1: LSB: Transmit bit least-significant bit (LSB) first */
#define QSPI_XIP_FMT_ENDIAN_OFS                   2U                                          /*!< QSPI XIP FMT: ENDIAN Bit Offset */
#define QSPI_XIP_FMT_ENDIAN_VAL(regval)               (BIT(2) & ((uint32_t)(regval) << 2))        /*!< QSPI XIP FMT: ENDIAN Bit Value */  
#define QSPI_XIP_FMT_ENDIAN_MSB                        0x0UL                                                 /*!< MSB */
#define QSPI_XIP_FMT_ENDIAN_LSB                        BIT(2)                                                   /*!< LSB */

/**
  * \brief Check the QSPI_XIP spi_fmt endian bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid endian bit.
  * \retval 1 This is a valid endian bit.
  */
#define IS_QSPI_XIP_SPI_FMT_ENDIAN(regval)                 (\
                                           ((regval) == QSPI_XIP_FMT_ENDIAN_MSB                 ) || \
                                           ((regval) == QSPI_XIP_FMT_ENDIAN_LSB                 )  \
                                                 )

#define QSPI_XIP_FMT_DIR                          BIT(3)                                      /*!< SPI I/O direction. This is reset to 1 for flash-enabled SPI controllers, 0 otherwise.(normal mode non-xip) - 1'b0: RX:  Rx: For dual and quad protocols, the DQ pins are tri-stated. For the single protocol, the DQ0 pin is driven with the transmit data as normal. - 1'b1: TX:  Tx: The receive FIFO is not populated. */
#define QSPI_XIP_FMT_DIR_OFS                      3U                                          /*!< QSPI XIP FMT: DIR Bit Offset */
#define QSPI_XIP_FMT_DIR_VAL(regval)                  (BIT(3) & ((uint32_t)(regval) << 3))        /*!< QSPI XIP FMT: DIR Bit Value */  
#define QSPI_XIP_FMT_DIR_RX                            0x0UL                                                 /*!< RX */
#define QSPI_XIP_FMT_DIR_TX                            BIT(3)                                                   /*!< TX */

/**
  * \brief Check the QSPI_XIP spi_fmt dir bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dir bit.
  * \retval 1 This is a valid dir bit.
  */
#define IS_QSPI_XIP_SPI_FMT_DIR(regval)                    (\
                                              ((regval) == QSPI_XIP_FMT_DIR_RX                  ) || \
                                              ((regval) == QSPI_XIP_FMT_DIR_TX                  )  \
                                                 )

#define QSPI_XIP_FMT_PROTO_HI                     BIT(4)                                      /*!< This bit field is used to extend the SPI_FMT.spi_proto register to the high bits belonging to SPI_FMT.spi_proto */
#define QSPI_XIP_FMT_PROTO_HI_OFS                 4U                                          /*!< QSPI XIP FMT: PROTO_HI Bit Offset */
#define QSPI_XIP_FMT_PROTO_HI_VAL(regval)             (BIT(4) & ((uint32_t)(regval) << 4))        /*!< QSPI XIP FMT: PROTO_HI Bit Value */  
#define QSPI_XIP_FMT_ENDIAN_BYTE                  BIT(5)                                      /*!< SPI endianness for byte:(normal mode non-xip) - 1'b0: MSB: Transmit byte most-significant bit (MSB) first - 1'b1: LSB: Transmit byte least-significant bit (LSB) first */
#define QSPI_XIP_FMT_ENDIAN_BYTE_OFS              5U                                          /*!< QSPI XIP FMT: ENDIAN_BYTE Bit Offset */
#define QSPI_XIP_FMT_ENDIAN_BYTE_VAL(regval)          (BIT(5) & ((uint32_t)(regval) << 5))        /*!< QSPI XIP FMT: ENDIAN_BYTE Bit Value */  
#define QSPI_XIP_FMT_ENDIAN_BYTE_MSB                   0x0UL                                                 /*!< MSB */
#define QSPI_XIP_FMT_ENDIAN_BYTE_LSB                   BIT(5)                                                   /*!< LSB */

/**
  * \brief Check the QSPI_XIP spi_fmt endian_byte bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid endian_byte bit.
  * \retval 1 This is a valid endian_byte bit.
  */
#define IS_QSPI_XIP_SPI_FMT_ENDIAN_BYTE(regval)            (\
                                      ((regval) == QSPI_XIP_FMT_ENDIAN_BYTE_MSB                 ) || \
                                      ((regval) == QSPI_XIP_FMT_ENDIAN_BYTE_LSB                 )  \
                                                 )

#define QSPI_XIP_FMT_LEN_MASK                     BITS(16,21)                                   /*!< QSPI XIP FMT: LEN Bit Mask */  
#define QSPI_XIP_FMT_LEN_OFS                      16U                                          /*!< QSPI XIP FMT: LEN Bit Offset */
#define QSPI_XIP_FMT_LEN(regval)                  (BITS(16,21) & ((uint32_t)(regval) << 16))        /*!< QSPI XIP FMT: LEN Bit Value */  
#define QSPI_XIP_FMT_LEN_8B                            QSPI_XIP_FMT_LEN(8)                                                   /*!< 8B */
#define QSPI_XIP_FMT_LEN_16B                           QSPI_XIP_FMT_LEN(16)                                                   /*!< 16B */
#define QSPI_XIP_FMT_LEN_32B                           QSPI_XIP_FMT_LEN(32)                                                   /*!< 32B */

/**
  * \brief Check the QSPI_XIP spi_fmt len bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid len bits.
  * \retval 1 This is a valid len bits.
  */
#define IS_QSPI_XIP_SPI_FMT_LEN(regval)                    (\
                                              ((regval) == QSPI_XIP_FMT_LEN_8B                  ) || \
                                              ((regval) == QSPI_XIP_FMT_LEN_16B                 ) || \
                                              ((regval) == QSPI_XIP_FMT_LEN_32B                 )  \
                                                 )

 /* ===== QSPI XIP TXDATA Register definition ===== */
#define QSPI_XIP_TXDATA_VAL_MASK                     BITS(0,31)                                   /*!< QSPI XIP TXDATA: VAL Bit Mask */  
#define QSPI_XIP_TXDATA_VAL_OFS                      0U                                          /*!< QSPI XIP TXDATA: VAL Bit Offset */
#define QSPI_XIP_TXDATA_VAL(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP TXDATA: VAL Bit Value */  
 
 /* ===== QSPI XIP RXDATA Register definition ===== */
#define QSPI_XIP_RXDATA_VAL                          BITS(0,31)                
 
 /* ===== QSPI XIP TX_MARK Register definition ===== */
#define QSPI_XIP_TX_MARK_NUM_MASK                     BITS(0,2)                                   /*!< QSPI XIP TX MARK: NUM Bit Mask */  
#define QSPI_XIP_TX_MARK_NUM_OFS                      0U                                          /*!< QSPI XIP TX MARK: NUM Bit Offset */
#define QSPI_XIP_TX_MARK_NUM(regval)                  (BITS(0,2) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP TX MARK: NUM Bit Value */  
 
 /* ===== QSPI XIP RX_MARK Register definition ===== */
#define QSPI_XIP_RX_MARK_NUM_MASK                     BITS(0,2)                                   /*!< QSPI XIP RX MARK: NUM Bit Mask */  
#define QSPI_XIP_RX_MARK_NUM_OFS                      0U                                          /*!< QSPI XIP RX MARK: NUM Bit Offset */
#define QSPI_XIP_RX_MARK_NUM(regval)                  (BITS(0,2) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP RX MARK: NUM Bit Value */  
 
 /* ===== QSPI XIP FCTRL Register definition ===== */
#define QSPI_XIP_FCTRL_FLASH                        BIT(0)                                      /*!< SPI XIP Mode Select  - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_FCTRL_FLASH_OFS                    0U                                          /*!< QSPI XIP FCTRL: FLASH Bit Offset */
#define QSPI_XIP_FCTRL_FLASH_VAL(regval)                (BIT(0) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP FCTRL: FLASH Bit Value */  
#define QSPI_XIP_FCTRL_FLASH_DISABLE                     0x0UL                                               /*!< DISABLE */
#define QSPI_XIP_FCTRL_FLASH_ENABLE                      BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_fctrl flash bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid flash bit.
  * \retval 1 This is a valid flash bit.
  */
#define IS_QSPI_XIP_SPI_FCTRL_FLASH(regval)                  (\
                                            ((regval) == QSPI_XIP_FCTRL_FLASH_DISABLE             ) || \
                                            ((regval) == QSPI_XIP_FCTRL_FLASH_ENABLE              )  \
                                                 )

#define QSPI_XIP_FCTRL_FLASH_WMASK                  BIT(1)                                      /*!< SPI XIP Data mask Select.If the function is turned on, our bus write operation will look at the wmask signal of the bus, and turning on this function will affect the XIP write performance, and if the function is turned off, the XIP write will ignore the bus wmask signal - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_FCTRL_FLASH_WMASK_OFS              1U                                          /*!< QSPI XIP FCTRL: FLASH_WMASK Bit Offset */
#define QSPI_XIP_FCTRL_FLASH_WMASK_VAL(regval)          (BIT(1) & ((uint32_t)(regval) << 1))        /*!< QSPI XIP FCTRL: FLASH_WMASK Bit Value */  
#define QSPI_XIP_FCTRL_FLASH_WMASK_DISABLE               0x0UL                                               /*!< DISABLE */
#define QSPI_XIP_FCTRL_FLASH_WMASK_ENABLE                BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_fctrl flash_wmask bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid flash_wmask bit.
  * \retval 1 This is a valid flash_wmask bit.
  */
#define IS_QSPI_XIP_SPI_FCTRL_FLASH_WMASK(regval)            (\
                                      ((regval) == QSPI_XIP_FCTRL_FLASH_WMASK_DISABLE             ) || \
                                      ((regval) == QSPI_XIP_FCTRL_FLASH_WMASK_ENABLE              )  \
                                                 )

#define QSPI_XIP_FCTRL_FLASHW                       BIT(2)                                      /*!< SPI XIP mode write enable - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_FCTRL_FLASHW_OFS                   2U                                          /*!< QSPI XIP FCTRL: FLASHW Bit Offset */
#define QSPI_XIP_FCTRL_FLASHW_VAL(regval)               (BIT(2) & ((uint32_t)(regval) << 2))        /*!< QSPI XIP FCTRL: FLASHW Bit Value */  
#define QSPI_XIP_FCTRL_FLASHW_DISABLE                    0x0UL                                               /*!< DISABLE */
#define QSPI_XIP_FCTRL_FLASHW_ENABLE                     BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_fctrl flashw bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid flashw bit.
  * \retval 1 This is a valid flashw bit.
  */
#define IS_QSPI_XIP_SPI_FCTRL_FLASHW(regval)                 (\
                                           ((regval) == QSPI_XIP_FCTRL_FLASHW_DISABLE             ) || \
                                           ((regval) == QSPI_XIP_FCTRL_FLASHW_ENABLE              )  \
                                                 )

#define QSPI_XIP_FCTRL_FLASH_BURST                  BIT(3)                                      /*!< SPI XIP burst enable - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_FCTRL_FLASH_BURST_OFS              3U                                          /*!< QSPI XIP FCTRL: FLASH_BURST Bit Offset */
#define QSPI_XIP_FCTRL_FLASH_BURST_VAL(regval)          (BIT(3) & ((uint32_t)(regval) << 3))        /*!< QSPI XIP FCTRL: FLASH_BURST Bit Value */  
#define QSPI_XIP_FCTRL_FLASH_BURST_DISABLE               0x0UL                                               /*!< DISABLE */
#define QSPI_XIP_FCTRL_FLASH_BURST_ENABLE                BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_fctrl flash_burst bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid flash_burst bit.
  * \retval 1 This is a valid flash_burst bit.
  */
#define IS_QSPI_XIP_SPI_FCTRL_FLASH_BURST(regval)            (\
                                      ((regval) == QSPI_XIP_FCTRL_FLASH_BURST_DISABLE             ) || \
                                      ((regval) == QSPI_XIP_FCTRL_FLASH_BURST_ENABLE              )  \
                                                 )

#define QSPI_XIP_FCTRL_FLASH_WRAP_EN                BIT(4)                                      /*!< SPI XIP wrap enable: - 1'b0: disable if slave don't support warp - 1'b1: enable if slave support wrap  Note: 1.Both burst-en and wrap-en are disabled: When these two options are not enabled, regardless of whether the CPU issues a burst or single read request, the QSPI-XIP (Execute In Place) controller will break down these requests into individual read requests to access the external memory (such as flash or psram). This means that even if the CPU attempts to improve data transfer efficiency through burst mode, the actual operation is still performed as individual accesses, potentially reducing overall performance. 2.Both burst-en and wrap-en are enabled: With this setup, if the CPU issues a burst request that fits the wrap mode (i.e., cyclic reading of data within a 64-byte boundary), the QSPI-XIP controller will automatically resend the address to the external memory based on the CPU's burst wrap behavior. Notably, in this case, there is no need for additional configuration of the wrap characteristics of the external memory (flash/psram), as they work by default in incremental (incr) mode, which meets the requirements. 3.Only burst-en is enabled with matching external memory characteristics: If the burst wrap type issued by the CPU matches the burst wrap characteristics supported by the external memory (flash/psram), then only burst-en needs to be activated without enabling wrap-en. Additionally, it is necessary to pre-activate the wrap features of the external memory. This approach can provide higher performance compared to the second scenario because it allows more efficient data transfers, reducing unnecessary address resends, thereby increasing the data transfer rate. */
#define QSPI_XIP_FCTRL_FLASH_WRAP_EN_OFS            4U                                          /*!< QSPI XIP FCTRL: FLASH_WRAP_EN Bit Offset */
#define QSPI_XIP_FCTRL_FLASH_WRAP_EN_VAL(regval)        (BIT(4) & ((uint32_t)(regval) << 4))        /*!< QSPI XIP FCTRL: FLASH_WRAP_EN Bit Value */  
#define QSPI_XIP_FCTRL_FLASH_WRAP_EN_DISABLE             0x0UL                                               /*!< DISABLE */
#define QSPI_XIP_FCTRL_FLASH_WRAP_EN_ENABLE              BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_fctrl flash_wrap_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid flash_wrap_en bit.
  * \retval 1 This is a valid flash_wrap_en bit.
  */
#define IS_QSPI_XIP_SPI_FCTRL_FLASH_WRAP_EN(regval)          (\
                                    ((regval) == QSPI_XIP_FCTRL_FLASH_WRAP_EN_DISABLE             ) || \
                                    ((regval) == QSPI_XIP_FCTRL_FLASH_WRAP_EN_ENABLE              )  \
                                                 )

#define QSPI_XIP_FCTRL_BOUNDARY_EN                  BIT(5)                                      /*!< SPI XIP boundary config enable - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_FCTRL_BOUNDARY_EN_OFS              5U                                          /*!< QSPI XIP FCTRL: BOUNDARY_EN Bit Offset */
#define QSPI_XIP_FCTRL_BOUNDARY_EN_VAL(regval)          (BIT(5) & ((uint32_t)(regval) << 5))        /*!< QSPI XIP FCTRL: BOUNDARY_EN Bit Value */  
#define QSPI_XIP_FCTRL_BOUNDARY_EN_DISABLE               0x0UL                                               /*!< DISABLE */
#define QSPI_XIP_FCTRL_BOUNDARY_EN_ENABLE                BIT(5)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_fctrl boundary_en bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid boundary_en bit.
  * \retval 1 This is a valid boundary_en bit.
  */
#define IS_QSPI_XIP_SPI_FCTRL_BOUNDARY_EN(regval)            (\
                                      ((regval) == QSPI_XIP_FCTRL_BOUNDARY_EN_DISABLE             ) || \
                                      ((regval) == QSPI_XIP_FCTRL_BOUNDARY_EN_ENABLE              )  \
                                                 )

 /* ===== QSPI XIP FFMT Register definition ===== */
#define QSPI_XIP_FFMT_CMD                          BIT(0)                                      /*!< Enable sending of command(XIP mode) - 1'b0: disable - 1'b1: enable Note:enable hardware sending */
#define QSPI_XIP_FFMT_CMD_OFS                      0U                                          /*!< QSPI XIP FFMT: CMD Bit Offset */
#define QSPI_XIP_FFMT_CMD_VAL(regval)                  (BIT(0) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP FFMT: CMD Bit Value */  
#define QSPI_XIP_FFMT_CMD_DISABLE                       0x0UL                                                /*!< DISABLE */
#define QSPI_XIP_FFMT_CMD_ENABLE                        BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_ffmt cmd bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cmd bit.
  * \retval 1 This is a valid cmd bit.
  */
#define IS_QSPI_XIP_SPI_FFMT_CMD(regval)                    (\
                                              ((regval) == QSPI_XIP_FFMT_CMD_DISABLE             ) || \
                                              ((regval) == QSPI_XIP_FFMT_CMD_ENABLE              )  \
                                                 )

#define QSPI_XIP_FFMT_ADDR_LEN_MASK                BITS(1,3)                                   /*!< QSPI XIP FFMT: ADDR_LEN Bit Mask */  
#define QSPI_XIP_FFMT_ADDR_LEN_OFS                 1U                                          /*!< QSPI XIP FFMT: ADDR_LEN Bit Offset */
#define QSPI_XIP_FFMT_ADDR_LEN(regval)             (BITS(1,3) & ((uint32_t)(regval) << 1))        /*!< QSPI XIP FFMT: ADDR_LEN Bit Value */  
#define QSPI_XIP_FFMT_PAD_CNT_MASK                 BITS(4,7)                                   /*!< QSPI XIP FFMT: PAD_CNT Bit Mask */  
#define QSPI_XIP_FFMT_PAD_CNT_OFS                  4U                                          /*!< QSPI XIP FFMT: PAD_CNT Bit Offset */
#define QSPI_XIP_FFMT_PAD_CNT(regval)              (BITS(4,7) & ((uint32_t)(regval) << 4))        /*!< QSPI XIP FFMT: PAD_CNT Bit Value */  
#define QSPI_XIP_FFMT_CMD_PROTO_MASK               BITS(8,9)                                   /*!< QSPI XIP FFMT: CMD_PROTO Bit Mask */  
#define QSPI_XIP_FFMT_CMD_PROTO_OFS                8U                                          /*!< QSPI XIP FFMT: CMD_PROTO Bit Offset */
#define QSPI_XIP_FFMT_CMD_PROTO(regval)            (BITS(8,9) & ((uint32_t)(regval) << 8))        /*!< QSPI XIP FFMT: CMD_PROTO Bit Value */  
#define QSPI_XIP_FFMT_CMD_PROTO_SINGLE                  QSPI_XIP_FFMT_CMD_PROTO(0)                                                  /*!< SINGLE */
#define QSPI_XIP_FFMT_CMD_PROTO_DUAL                    QSPI_XIP_FFMT_CMD_PROTO(1)                                                  /*!< DUAL */
#define QSPI_XIP_FFMT_CMD_PROTO_QUAD                    QSPI_XIP_FFMT_CMD_PROTO(2)                                                  /*!< QUAD */
#define QSPI_XIP_FFMT_CMD_PROTO_OCTOL                   QSPI_XIP_FFMT_CMD_PROTO(3)                                                  /*!< OCTOL */

/**
  * \brief Check the QSPI_XIP spi_ffmt cmd_proto bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid cmd_proto bits.
  * \retval 1 This is a valid cmd_proto bits.
  */
#define IS_QSPI_XIP_SPI_FFMT_CMD_PROTO(regval)              (\
                                        ((regval) == QSPI_XIP_FFMT_CMD_PROTO_SINGLE              ) || \
                                        ((regval) == QSPI_XIP_FFMT_CMD_PROTO_DUAL                ) || \
                                        ((regval) == QSPI_XIP_FFMT_CMD_PROTO_QUAD                ) || \
                                        ((regval) == QSPI_XIP_FFMT_CMD_PROTO_OCTOL               )  \
                                                 )

#define QSPI_XIP_FFMT_ADDR_PROTO_MASK              BITS(10,11)                                   /*!< QSPI XIP FFMT: ADDR_PROTO Bit Mask */  
#define QSPI_XIP_FFMT_ADDR_PROTO_OFS               10U                                          /*!< QSPI XIP FFMT: ADDR_PROTO Bit Offset */
#define QSPI_XIP_FFMT_ADDR_PROTO(regval)           (BITS(10,11) & ((uint32_t)(regval) << 10))        /*!< QSPI XIP FFMT: ADDR_PROTO Bit Value */  
#define QSPI_XIP_FFMT_ADDR_PROTO_SINGLE                 QSPI_XIP_FFMT_ADDR_PROTO(0)                                                  /*!< SINGLE */
#define QSPI_XIP_FFMT_ADDR_PROTO_DUAL                   QSPI_XIP_FFMT_ADDR_PROTO(1)                                                  /*!< DUAL */
#define QSPI_XIP_FFMT_ADDR_PROTO_QUAD                   QSPI_XIP_FFMT_ADDR_PROTO(2)                                                  /*!< QUAD */
#define QSPI_XIP_FFMT_ADDR_PROTO_OCTOL                  QSPI_XIP_FFMT_ADDR_PROTO(3)                                                  /*!< OCTOL */

/**
  * \brief Check the QSPI_XIP spi_ffmt addr_proto bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid addr_proto bits.
  * \retval 1 This is a valid addr_proto bits.
  */
#define IS_QSPI_XIP_SPI_FFMT_ADDR_PROTO(regval)             (\
                                       ((regval) == QSPI_XIP_FFMT_ADDR_PROTO_SINGLE              ) || \
                                       ((regval) == QSPI_XIP_FFMT_ADDR_PROTO_DUAL                ) || \
                                       ((regval) == QSPI_XIP_FFMT_ADDR_PROTO_QUAD                ) || \
                                       ((regval) == QSPI_XIP_FFMT_ADDR_PROTO_OCTOL               )  \
                                                 )

#define QSPI_XIP_FFMT_DATA_PROTO_MASK              BITS(12,13)                                   /*!< QSPI XIP FFMT: DATA_PROTO Bit Mask */  
#define QSPI_XIP_FFMT_DATA_PROTO_OFS               12U                                          /*!< QSPI XIP FFMT: DATA_PROTO Bit Offset */
#define QSPI_XIP_FFMT_DATA_PROTO(regval)           (BITS(12,13) & ((uint32_t)(regval) << 12))        /*!< QSPI XIP FFMT: DATA_PROTO Bit Value */  
#define QSPI_XIP_FFMT_DATA_PROTO_SINGLE                 QSPI_XIP_FFMT_DATA_PROTO(0)                                                  /*!< SINGLE */
#define QSPI_XIP_FFMT_DATA_PROTO_DUAL                   QSPI_XIP_FFMT_DATA_PROTO(1)                                                  /*!< DUAL */
#define QSPI_XIP_FFMT_DATA_PROTO_QUAD                   QSPI_XIP_FFMT_DATA_PROTO(2)                                                  /*!< QUAD */
#define QSPI_XIP_FFMT_DATA_PROTO_OCTOL                  QSPI_XIP_FFMT_DATA_PROTO(3)                                                  /*!< OCTOL */
#define QSPI_XIP_FFMT_DATA_PROTO_X16                    QSPI_XIP_FFMT_DATA_PROTO(4)                                                  /*!< X16 */
#define QSPI_XIP_FFMT_DATA_PROTO_HDS                    QSPI_XIP_FFMT_DATA_PROTO(7)                                                  /*!< HDS */

/**
  * \brief Check the QSPI_XIP spi_ffmt data_proto bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid data_proto bits.
  * \retval 1 This is a valid data_proto bits.
  */
#define IS_QSPI_XIP_SPI_FFMT_DATA_PROTO(regval)             (\
                                       ((regval) == QSPI_XIP_FFMT_DATA_PROTO_SINGLE              ) || \
                                       ((regval) == QSPI_XIP_FFMT_DATA_PROTO_DUAL                ) || \
                                       ((regval) == QSPI_XIP_FFMT_DATA_PROTO_QUAD                ) || \
                                       ((regval) == QSPI_XIP_FFMT_DATA_PROTO_OCTOL               ) || \
                                       ((regval) == QSPI_XIP_FFMT_DATA_PROTO_X16                 ) || \
                                       ((regval) == QSPI_XIP_FFMT_DATA_PROTO_HDS                 )  \
                                                 )

#define QSPI_XIP_FFMT_ENDINA_F                     BIT(14)                                      /*!< SPI xip endianness:(XIP mode) - 1'b0: MSB: Transmit most-significant bit (MSB) first - 1'b1: LSB: Transmit least-significant bit (LSB) first */
#define QSPI_XIP_FFMT_ENDINA_F_OFS                 14U                                          /*!< QSPI XIP FFMT: ENDINA_F Bit Offset */
#define QSPI_XIP_FFMT_ENDINA_F_VAL(regval)             (BIT(14) & ((uint32_t)(regval) << 14))        /*!< QSPI XIP FFMT: ENDINA_F Bit Value */  
#define QSPI_XIP_FFMT_ENDINA_F_MSB                      0x0UL                                                /*!< MSB */
#define QSPI_XIP_FFMT_ENDINA_F_LSB                      BIT(14)                                                  /*!< LSB */

/**
  * \brief Check the QSPI_XIP spi_ffmt endina_f bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid endina_f bit.
  * \retval 1 This is a valid endina_f bit.
  */
#define IS_QSPI_XIP_SPI_FFMT_ENDINA_F(regval)               (\
                                         ((regval) == QSPI_XIP_FFMT_ENDINA_F_MSB                 ) || \
                                         ((regval) == QSPI_XIP_FFMT_ENDINA_F_LSB                 )  \
                                                 )

#define QSPI_XIP_FFMT_DATA_PROTO_HI                BIT(15)                                      /*!<  This bit field is used to extend the SPI_FFMT.spi_data_proto register to the high bits belonging to SPI_FFMT.spi_data_proto  */
#define QSPI_XIP_FFMT_DATA_PROTO_HI_OFS            15U                                          /*!< QSPI XIP FFMT: DATA_PROTO_HI Bit Offset */
#define QSPI_XIP_FFMT_DATA_PROTO_HI_VAL(regval)        (BIT(15) & ((uint32_t)(regval) << 15))        /*!< QSPI XIP FFMT: DATA_PROTO_HI Bit Value */  
#define QSPI_XIP_FFMT_CMD_CODE_MASK                BITS(16,23)                                   /*!< QSPI XIP FFMT: CMD_CODE Bit Mask */  
#define QSPI_XIP_FFMT_CMD_CODE_OFS                 16U                                          /*!< QSPI XIP FFMT: CMD_CODE Bit Offset */
#define QSPI_XIP_FFMT_CMD_CODE(regval)             (BITS(16,23) & ((uint32_t)(regval) << 16))        /*!< QSPI XIP FFMT: CMD_CODE Bit Value */  
#define QSPI_XIP_FFMT_PAD_CODE_MASK                BITS(24,31)                                   /*!< QSPI XIP FFMT: PAD_CODE Bit Mask */  
#define QSPI_XIP_FFMT_PAD_CODE_OFS                 24U                                          /*!< QSPI XIP FFMT: PAD_CODE Bit Offset */
#define QSPI_XIP_FFMT_PAD_CODE(regval)             (BITS(24,31) & ((uint32_t)(regval) << 24))        /*!< QSPI XIP FFMT: PAD_CODE Bit Value */  
 
 /* ===== QSPI XIP IE Register definition ===== */
#define QSPI_XIP_IE_TX                           BIT(0)                                      /*!< Transmit watermark enable(normal mode non-xip) - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_IE_TX_OFS                       0U                                          /*!< QSPI XIP IE: TX Bit Offset */
#define QSPI_XIP_IE_TX_VAL(regval)                   (BIT(0) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP IE: TX Bit Value */  
#define QSPI_XIP_IE_TX_DISABLE                        0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_IE_TX_ENABLE                         BIT(0)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_ie tx bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx bit.
  * \retval 1 This is a valid tx bit.
  */
#define IS_QSPI_XIP_SPI_IE_TX(regval)                     (\
                                               ((regval) == QSPI_XIP_IE_TX_DISABLE             ) || \
                                               ((regval) == QSPI_XIP_IE_TX_ENABLE              )  \
                                                 )

#define QSPI_XIP_IE_RX                           BIT(1)                                      /*!< Receive watermark enable(normal mode non-xip) - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_IE_RX_OFS                       1U                                          /*!< QSPI XIP IE: RX Bit Offset */
#define QSPI_XIP_IE_RX_VAL(regval)                   (BIT(1) & ((uint32_t)(regval) << 1))        /*!< QSPI XIP IE: RX Bit Value */  
#define QSPI_XIP_IE_RX_DISABLE                        0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_IE_RX_ENABLE                         BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_ie rx bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx bit.
  * \retval 1 This is a valid rx bit.
  */
#define IS_QSPI_XIP_SPI_IE_RX(regval)                     (\
                                               ((regval) == QSPI_XIP_IE_RX_DISABLE             ) || \
                                               ((regval) == QSPI_XIP_IE_RX_ENABLE              )  \
                                                 )

#define QSPI_XIP_IE_TX_UDR                       BIT(2)                                      /*!< txudr error irq enable(normal mode non-xip) - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_IE_TX_UDR_OFS                   2U                                          /*!< QSPI XIP IE: TX_UDR Bit Offset */
#define QSPI_XIP_IE_TX_UDR_VAL(regval)               (BIT(2) & ((uint32_t)(regval) << 2))        /*!< QSPI XIP IE: TX_UDR Bit Value */  
#define QSPI_XIP_IE_TX_UDR_DISABLE                    0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_IE_TX_UDR_ENABLE                     BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_ie tx_udr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_udr bit.
  * \retval 1 This is a valid tx_udr bit.
  */
#define IS_QSPI_XIP_SPI_IE_TX_UDR(regval)                 (\
                                           ((regval) == QSPI_XIP_IE_TX_UDR_DISABLE             ) || \
                                           ((regval) == QSPI_XIP_IE_TX_UDR_ENABLE              )  \
                                                 )

#define QSPI_XIP_IE_RX_OVR                       BIT(3)                                      /*!< rxovr error irq enable(normal mode non-xip) - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_IE_RX_OVR_OFS                   3U                                          /*!< QSPI XIP IE: RX_OVR Bit Offset */
#define QSPI_XIP_IE_RX_OVR_VAL(regval)               (BIT(3) & ((uint32_t)(regval) << 3))        /*!< QSPI XIP IE: RX_OVR Bit Value */  
#define QSPI_XIP_IE_RX_OVR_DISABLE                    0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_IE_RX_OVR_ENABLE                     BIT(3)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_ie rx_ovr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_ovr bit.
  * \retval 1 This is a valid rx_ovr bit.
  */
#define IS_QSPI_XIP_SPI_IE_RX_OVR(regval)                 (\
                                           ((regval) == QSPI_XIP_IE_RX_OVR_DISABLE             ) || \
                                           ((regval) == QSPI_XIP_IE_RX_OVR_ENABLE              )  \
                                                 )

#define QSPI_XIP_IE_RX_UDR                       BIT(4)                                      /*!< rxudr error irq enable(normal mode non-xip) - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_IE_RX_UDR_OFS                   4U                                          /*!< QSPI XIP IE: RX_UDR Bit Offset */
#define QSPI_XIP_IE_RX_UDR_VAL(regval)               (BIT(4) & ((uint32_t)(regval) << 4))        /*!< QSPI XIP IE: RX_UDR Bit Value */  
#define QSPI_XIP_IE_RX_UDR_DISABLE                    0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_IE_RX_UDR_ENABLE                     BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_ie rx_udr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_udr bit.
  * \retval 1 This is a valid rx_udr bit.
  */
#define IS_QSPI_XIP_SPI_IE_RX_UDR(regval)                 (\
                                           ((regval) == QSPI_XIP_IE_RX_UDR_DISABLE             ) || \
                                           ((regval) == QSPI_XIP_IE_RX_UDR_ENABLE              )  \
                                                 )

#define QSPI_XIP_IE_TX_OVR                       BIT(5)                                      /*!< txovr error irq enable(normal mode non-xip) - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_IE_TX_OVR_OFS                   5U                                          /*!< QSPI XIP IE: TX_OVR Bit Offset */
#define QSPI_XIP_IE_TX_OVR_VAL(regval)               (BIT(5) & ((uint32_t)(regval) << 5))        /*!< QSPI XIP IE: TX_OVR Bit Value */  
#define QSPI_XIP_IE_TX_OVR_DISABLE                    0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_IE_TX_OVR_ENABLE                     BIT(5)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_ie tx_ovr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_ovr bit.
  * \retval 1 This is a valid tx_ovr bit.
  */
#define IS_QSPI_XIP_SPI_IE_TX_OVR(regval)                 (\
                                           ((regval) == QSPI_XIP_IE_TX_OVR_DISABLE             ) || \
                                           ((regval) == QSPI_XIP_IE_TX_OVR_ENABLE              )  \
                                                 )

#define QSPI_XIP_IE_DONE                         BIT(7)                                      /*!< spi tx done & TX fifo is empty irq enable(normal mode non-xip) - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_IE_DONE_OFS                     7U                                          /*!< QSPI XIP IE: DONE Bit Offset */
#define QSPI_XIP_IE_DONE_VAL(regval)                 (BIT(7) & ((uint32_t)(regval) << 7))        /*!< QSPI XIP IE: DONE Bit Value */  
#define QSPI_XIP_IE_DONE_DISABLE                      0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_IE_DONE_ENABLE                       BIT(7)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_ie done bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid done bit.
  * \retval 1 This is a valid done bit.
  */
#define IS_QSPI_XIP_SPI_IE_DONE(regval)                   (\
                                             ((regval) == QSPI_XIP_IE_DONE_DISABLE             ) || \
                                             ((regval) == QSPI_XIP_IE_DONE_ENABLE              )  \
                                                 )

#define QSPI_XIP_IE_TX_DONE                      BIT(11)                                      /*!< spi tx done irq enable(normal mode non-xip) - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_IE_TX_DONE_OFS                  11U                                          /*!< QSPI XIP IE: TX_DONE Bit Offset */
#define QSPI_XIP_IE_TX_DONE_VAL(regval)              (BIT(11) & ((uint32_t)(regval) << 11))        /*!< QSPI XIP IE: TX_DONE Bit Value */  
#define QSPI_XIP_IE_TX_DONE_DISABLE                   0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_IE_TX_DONE_ENABLE                    BIT(11)                                                  /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_ie tx_done bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_done bit.
  * \retval 1 This is a valid tx_done bit.
  */
#define IS_QSPI_XIP_SPI_IE_TX_DONE(regval)                (\
                                          ((regval) == QSPI_XIP_IE_TX_DONE_DISABLE             ) || \
                                          ((regval) == QSPI_XIP_IE_TX_DONE_ENABLE              )  \
                                                 )

#define QSPI_XIP_IE_RX_DONE                      BIT(12)                                      /*!< spi rx done irq enable(normal mode non-xip) - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_IE_RX_DONE_OFS                  12U                                          /*!< QSPI XIP IE: RX_DONE Bit Offset */
#define QSPI_XIP_IE_RX_DONE_VAL(regval)              (BIT(12) & ((uint32_t)(regval) << 12))        /*!< QSPI XIP IE: RX_DONE Bit Value */  
#define QSPI_XIP_IE_RX_DONE_DISABLE                   0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_IE_RX_DONE_ENABLE                    BIT(12)                                                  /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_ie rx_done bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_done bit.
  * \retval 1 This is a valid rx_done bit.
  */
#define IS_QSPI_XIP_SPI_IE_RX_DONE(regval)                (\
                                          ((regval) == QSPI_XIP_IE_RX_DONE_DISABLE             ) || \
                                          ((regval) == QSPI_XIP_IE_RX_DONE_ENABLE              )  \
                                                 )

#define QSPI_XIP_IE_CFG_ERR                      BIT(14)                                      /*!< spi config fame error irq enable(normal mode and-xip) - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_IE_CFG_ERR_OFS                  14U                                          /*!< QSPI XIP IE: CFG_ERR Bit Offset */
#define QSPI_XIP_IE_CFG_ERR_VAL(regval)              (BIT(14) & ((uint32_t)(regval) << 14))        /*!< QSPI XIP IE: CFG_ERR Bit Value */  
#define QSPI_XIP_IE_CFG_ERR_DISABLE                   0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_IE_CFG_ERR_ENABLE                    BIT(14)                                                  /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_ie cfg_err bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid cfg_err bit.
  * \retval 1 This is a valid cfg_err bit.
  */
#define IS_QSPI_XIP_SPI_IE_CFG_ERR(regval)                (\
                                          ((regval) == QSPI_XIP_IE_CFG_ERR_DISABLE             ) || \
                                          ((regval) == QSPI_XIP_IE_CFG_ERR_ENABLE              )  \
                                                 )

 /* ===== QSPI XIP IP Register definition ===== */
#define QSPI_XIP_IP_TX                           BIT(0)                                      /*!< Transmit watermark irq pending(normal mode non-xip) */
#define QSPI_XIP_IP_RX                           BIT(1)                                      /*!< Receive watermark irq pending(normal mode non-xip) */
 
 /* ===== QSPI XIP FFMT1 Register definition ===== */
#define QSPI_XIP_FFMT1_WCMD_CODE_MASK               BITS(0,7)                                   /*!< QSPI XIP FFMT1: WCMD_CODE Bit Mask */  
#define QSPI_XIP_FFMT1_WCMD_CODE_OFS                0U                                          /*!< QSPI XIP FFMT1: WCMD_CODE Bit Offset */
#define QSPI_XIP_FFMT1_WCMD_CODE(regval)            (BITS(0,7) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP FFMT1: WCMD_CODE Bit Value */  
#define QSPI_XIP_FFMT1_WPAD_CNT_MASK                BITS(8,12)                                   /*!< QSPI XIP FFMT1: WPAD_CNT Bit Mask */  
#define QSPI_XIP_FFMT1_WPAD_CNT_OFS                 8U                                          /*!< QSPI XIP FFMT1: WPAD_CNT Bit Offset */
#define QSPI_XIP_FFMT1_WPAD_CNT(regval)             (BITS(8,12) & ((uint32_t)(regval) << 8))        /*!< QSPI XIP FFMT1: WPAD_CNT Bit Value */  
#define QSPI_XIP_FFMT1_PAD_CNT_H                    BIT(13)                                      /*!< dummy cycle high bit(XIP mode) */
#define QSPI_XIP_FFMT1_PAD_CNT_H_OFS                13U                                          /*!< QSPI XIP FFMT1: PAD_CNT_H Bit Offset */
#define QSPI_XIP_FFMT1_PAD_CNT_H_VAL(regval)            (BIT(13) & ((uint32_t)(regval) << 13))        /*!< QSPI XIP FFMT1: PAD_CNT_H Bit Value */  
#define QSPI_XIP_FFMT1_DDR_EN_MASK                  BITS(14,17)                                   /*!< QSPI XIP FFMT1: DDR_EN Bit Mask */  
#define QSPI_XIP_FFMT1_DDR_EN_OFS                   14U                                          /*!< QSPI XIP FFMT1: DDR_EN Bit Offset */
#define QSPI_XIP_FFMT1_DDR_EN(regval)               (BITS(14,17) & ((uint32_t)(regval) << 14))        /*!< QSPI XIP FFMT1: DDR_EN Bit Value */  
#define QSPI_XIP_FFMT1_MODE_PROTO_MASK              BITS(18,19)                                   /*!< QSPI XIP FFMT1: MODE_PROTO Bit Mask */  
#define QSPI_XIP_FFMT1_MODE_PROTO_OFS               18U                                          /*!< QSPI XIP FFMT1: MODE_PROTO Bit Offset */
#define QSPI_XIP_FFMT1_MODE_PROTO(regval)           (BITS(18,19) & ((uint32_t)(regval) << 18))        /*!< QSPI XIP FFMT1: MODE_PROTO Bit Value */  
#define QSPI_XIP_FFMT1_MODE_PROTO_SINGLE                 QSPI_XIP_FFMT1_MODE_PROTO(0)                                                 /*!< SINGLE */
#define QSPI_XIP_FFMT1_MODE_PROTO_DUAL                   QSPI_XIP_FFMT1_MODE_PROTO(1)                                                 /*!< DUAL */
#define QSPI_XIP_FFMT1_MODE_PROTO_QUAD                   QSPI_XIP_FFMT1_MODE_PROTO(2)                                                 /*!< QUAD */
#define QSPI_XIP_FFMT1_MODE_PROTO_OCTOL                  QSPI_XIP_FFMT1_MODE_PROTO(3)                                                 /*!< OCTOL */

/**
  * \brief Check the QSPI_XIP spi_ffmt1 mode_proto bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid mode_proto bits.
  * \retval 1 This is a valid mode_proto bits.
  */
#define IS_QSPI_XIP_SPI_FFMT1_MODE_PROTO(regval)             (\
                                       ((regval) == QSPI_XIP_FFMT1_MODE_PROTO_SINGLE              ) || \
                                       ((regval) == QSPI_XIP_FFMT1_MODE_PROTO_DUAL                ) || \
                                       ((regval) == QSPI_XIP_FFMT1_MODE_PROTO_QUAD                ) || \
                                       ((regval) == QSPI_XIP_FFMT1_MODE_PROTO_OCTOL               )  \
                                                 )

#define QSPI_XIP_FFMT1_MODE_CODE_MASK               BITS(20,27)                                   /*!< QSPI XIP FFMT1: MODE_CODE Bit Mask */  
#define QSPI_XIP_FFMT1_MODE_CODE_OFS                20U                                          /*!< QSPI XIP FFMT1: MODE_CODE Bit Offset */
#define QSPI_XIP_FFMT1_MODE_CODE(regval)            (BITS(20,27) & ((uint32_t)(regval) << 20))        /*!< QSPI XIP FFMT1: MODE_CODE Bit Value */  
#define QSPI_XIP_FFMT1_MODE_CNT_MASK                BITS(28,31)                                   /*!< QSPI XIP FFMT1: MODE_CNT Bit Mask */  
#define QSPI_XIP_FFMT1_MODE_CNT_OFS                 28U                                          /*!< QSPI XIP FFMT1: MODE_CNT Bit Offset */
#define QSPI_XIP_FFMT1_MODE_CNT(regval)             (BITS(28,31) & ((uint32_t)(regval) << 28))        /*!< QSPI XIP FFMT1: MODE_CNT Bit Value */  
 
 /* ===== QSPI XIP STATUS Register definition ===== */
#define QSPI_XIP_STATUS_BUSY                         BIT(0)                                      /*!< SPI Transmit is busy (master/slave)(normal mode non-xip) */
#define QSPI_XIP_STATUS_RX_OVR                       BIT(2)                                      /*!< pi rx fifo Overrun flag:(normal mode non-xip) - 1'b0: no over - 1'b1: yes: over Note: Software write 1 clear 0 */
#define QSPI_XIP_STATUS_RX_OVR_OFS                   2U                                          /*!< QSPI XIP STATUS: RX_OVR Bit Offset */
#define QSPI_XIP_STATUS_RX_OVR_VAL(regval)               (BIT(2) & ((uint32_t)(regval) << 2))        /*!< QSPI XIP STATUS: RX_OVR Bit Value */  
#define QSPI_XIP_STATUS_RX_OVR_NO                         0x0UL                                              /*!< NO */
#define QSPI_XIP_STATUS_RX_OVR_YES                        BIT(2)                                                   /*!< YES */

/**
  * \brief Check the QSPI_XIP spi_status rx_ovr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_ovr bit.
  * \retval 1 This is a valid rx_ovr bit.
  */
#define IS_QSPI_XIP_SPI_STATUS_RX_OVR(regval)                 (\
                                           ((regval) == QSPI_XIP_STATUS_RX_OVR_NO                  ) || \
                                           ((regval) == QSPI_XIP_STATUS_RX_OVR_YES                 )  \
                                                 )

#define QSPI_XIP_STATUS_TX_UDR                       BIT(3)                                      /*!< spi tx fifo Underrun flag:(normal mode non-xip) - 1'b0: no under  err - 1'b1: yes: under err Note: Software write 1 clear 0 */
#define QSPI_XIP_STATUS_TX_UDR_OFS                   3U                                          /*!< QSPI XIP STATUS: TX_UDR Bit Offset */
#define QSPI_XIP_STATUS_TX_UDR_VAL(regval)               (BIT(3) & ((uint32_t)(regval) << 3))        /*!< QSPI XIP STATUS: TX_UDR Bit Value */  
#define QSPI_XIP_STATUS_TX_UDR_NO                         0x0UL                                              /*!< NO */
#define QSPI_XIP_STATUS_TX_UDR_YES                        BIT(3)                                                   /*!< YES */

/**
  * \brief Check the QSPI_XIP spi_status tx_udr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_udr bit.
  * \retval 1 This is a valid tx_udr bit.
  */
#define IS_QSPI_XIP_SPI_STATUS_TX_UDR(regval)                 (\
                                           ((regval) == QSPI_XIP_STATUS_TX_UDR_NO                  ) || \
                                           ((regval) == QSPI_XIP_STATUS_TX_UDR_YES                 )  \
                                                 )

#define QSPI_XIP_STATUS_TX_FULL                      BIT(4)                                      /*!< tx_fifo full flag(normal mode non-xip) */
#define QSPI_XIP_STATUS_RX_EMPTY                     BIT(5)                                      /*!< rx fifo empty flag(normal mode non-xip) */
#define QSPI_XIP_STATUS_RX_UDR                       BIT(9)                                      /*!< spi rx fifo udr flag:(normal mode non-xip) - 1'b0: no over - 1'b1: yes: over Note: Software write 1 clear 0 */
#define QSPI_XIP_STATUS_RX_UDR_OFS                   9U                                          /*!< QSPI XIP STATUS: RX_UDR Bit Offset */
#define QSPI_XIP_STATUS_RX_UDR_VAL(regval)               (BIT(9) & ((uint32_t)(regval) << 9))        /*!< QSPI XIP STATUS: RX_UDR Bit Value */  
#define QSPI_XIP_STATUS_RX_UDR_NO                         0x0UL                                              /*!< NO */
#define QSPI_XIP_STATUS_RX_UDR_YES                        BIT(9)                                                   /*!< YES */

/**
  * \brief Check the QSPI_XIP spi_status rx_udr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_udr bit.
  * \retval 1 This is a valid rx_udr bit.
  */
#define IS_QSPI_XIP_SPI_STATUS_RX_UDR(regval)                 (\
                                           ((regval) == QSPI_XIP_STATUS_RX_UDR_NO                  ) || \
                                           ((regval) == QSPI_XIP_STATUS_RX_UDR_YES                 )  \
                                                 )

#define QSPI_XIP_STATUS_TX_OVR                       BIT(10)                                      /*!< spi tx fifo ovr flag:(normal mode non-xip) - 1'b0: no under  err - 1'b1: yes: under err Note: Software write 1 clear 0 */
#define QSPI_XIP_STATUS_TX_OVR_OFS                   10U                                          /*!< QSPI XIP STATUS: TX_OVR Bit Offset */
#define QSPI_XIP_STATUS_TX_OVR_VAL(regval)               (BIT(10) & ((uint32_t)(regval) << 10))        /*!< QSPI XIP STATUS: TX_OVR Bit Value */  
#define QSPI_XIP_STATUS_TX_OVR_NO                         0x0UL                                              /*!< NO */
#define QSPI_XIP_STATUS_TX_OVR_YES                        BIT(10)                                                  /*!< YES */

/**
  * \brief Check the QSPI_XIP spi_status tx_ovr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_ovr bit.
  * \retval 1 This is a valid tx_ovr bit.
  */
#define IS_QSPI_XIP_SPI_STATUS_TX_OVR(regval)                 (\
                                           ((regval) == QSPI_XIP_STATUS_TX_OVR_NO                  ) || \
                                           ((regval) == QSPI_XIP_STATUS_TX_OVR_YES                 )  \
                                                 )

#define QSPI_XIP_STATUS_TX_EMPTY                     BIT(11)                                      /*!< tx fifo empty flag(normal mode non-xip) */
#define QSPI_XIP_STATUS_RX_FULL                      BIT(12)                                      /*!< rx fifo full flag(normal mode non-xip) */
#define QSPI_XIP_STATUS_DONE                         BIT(13)                                      /*!< spi tx done & tx fifo is empty(normal mode non-xip) Note: Software write 1 clear 0 */
#define QSPI_XIP_STATUS_DONE_OFS                     13U                                          /*!< QSPI XIP STATUS: DONE Bit Offset */
#define QSPI_XIP_STATUS_DONE_VAL(regval)                 (BIT(13) & ((uint32_t)(regval) << 13))        /*!< QSPI XIP STATUS: DONE Bit Value */  
#define QSPI_XIP_STATUS_TX_DONE                      BIT(17)                                      /*!< spi tx done(normal mode non-xip) Note: Software write 1 clear 0 */
#define QSPI_XIP_STATUS_TX_DONE_OFS                  17U                                          /*!< QSPI XIP STATUS: TX_DONE Bit Offset */
#define QSPI_XIP_STATUS_TX_DONE_VAL(regval)              (BIT(17) & ((uint32_t)(regval) << 17))        /*!< QSPI XIP STATUS: TX_DONE Bit Value */  
#define QSPI_XIP_STATUS_RX_DONE                      BIT(18)                                      /*!< spi rx done(normal mode non-xip) Note: Software write 1 clear 0 */
#define QSPI_XIP_STATUS_RX_DONE_OFS                  18U                                          /*!< QSPI XIP STATUS: RX_DONE Bit Offset */
#define QSPI_XIP_STATUS_RX_DONE_VAL(regval)              (BIT(18) & ((uint32_t)(regval) << 18))        /*!< QSPI XIP STATUS: RX_DONE Bit Value */  
#define QSPI_XIP_STATUS_CFG_ERR                      BIT(20)                                      /*!< spi_cfg_err(normal mode or xip) Note: Software write 1 clear 0 */
#define QSPI_XIP_STATUS_CFG_ERR_OFS                  20U                                          /*!< QSPI XIP STATUS: CFG_ERR Bit Offset */
#define QSPI_XIP_STATUS_CFG_ERR_VAL(regval)              (BIT(20) & ((uint32_t)(regval) << 20))        /*!< QSPI XIP STATUS: CFG_ERR Bit Value */  
 
 /* ===== QSPI XIP SDR_SCKSAMPLE Register definition ===== */
#define QSPI_XIP_SDR_SCKSAMPLE_SDR_SCKSAMPLE_MASK           BITS(0,11)                                   /*!< QSPI XIP SDR SCKSAMPLE: SDR_SCKSAMPLE Bit Mask */  
#define QSPI_XIP_SDR_SCKSAMPLE_SDR_SCKSAMPLE_OFS            0U                                          /*!< QSPI XIP SDR SCKSAMPLE: SDR_SCKSAMPLE Bit Offset */
#define QSPI_XIP_SDR_SCKSAMPLE_SDR_SCKSAMPLE(regval)        (BITS(0,11) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP SDR SCKSAMPLE: SDR_SCKSAMPLE Bit Value */  
 
 /* ===== QSPI XIP CR Register definition ===== */
#define QSPI_XIP_CR_MODE                         BIT(0)                                      /*!< SPI Master Mode selection Select - 1'b0: slave - 1'b1: master */
#define QSPI_XIP_CR_MODE_OFS                     0U                                          /*!< QSPI XIP CR: MODE Bit Offset */
#define QSPI_XIP_CR_MODE_VAL(regval)                 (BIT(0) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP CR: MODE Bit Value */  
#define QSPI_XIP_CR_MODE_SLAVE                        0x0UL                                                  /*!< SLAVE */
#define QSPI_XIP_CR_MODE_MASTER                       BIT(0)                                                   /*!< MASTER */

/**
  * \brief Check the QSPI_XIP spi_cr mode bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid mode bit.
  * \retval 1 This is a valid mode bit.
  */
#define IS_QSPI_XIP_SPI_CR_MODE(regval)                   (\
                                             ((regval) == QSPI_XIP_CR_MODE_SLAVE               ) || \
                                             ((regval) == QSPI_XIP_CR_MODE_MASTER              )  \
                                                 )

#define QSPI_XIP_CR_DMA                          BIT(1)                                      /*!< DMA mode enable - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_CR_DMA_OFS                      1U                                          /*!< QSPI XIP CR: DMA Bit Offset */
#define QSPI_XIP_CR_DMA_VAL(regval)                  (BIT(1) & ((uint32_t)(regval) << 1))        /*!< QSPI XIP CR: DMA Bit Value */  
#define QSPI_XIP_CR_DMA_DISABLE                       0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_CR_DMA_ENABLE                        BIT(1)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_cr dma bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid dma bit.
  * \retval 1 This is a valid dma bit.
  */
#define IS_QSPI_XIP_SPI_CR_DMA(regval)                    (\
                                              ((regval) == QSPI_XIP_CR_DMA_DISABLE             ) || \
                                              ((regval) == QSPI_XIP_CR_DMA_ENABLE              )  \
                                                 )

#define QSPI_XIP_CR_DDR                          BIT(2)                                      /*!< Double Data Rate (DDR) mode enable , slave mode not support - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_CR_DDR_OFS                      2U                                          /*!< QSPI XIP CR: DDR Bit Offset */
#define QSPI_XIP_CR_DDR_VAL(regval)                  (BIT(2) & ((uint32_t)(regval) << 2))        /*!< QSPI XIP CR: DDR Bit Value */  
#define QSPI_XIP_CR_DDR_DISABLE                       0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_CR_DDR_ENABLE                        BIT(2)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_cr ddr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ddr bit.
  * \retval 1 This is a valid ddr bit.
  */
#define IS_QSPI_XIP_SPI_CR_DDR(regval)                    (\
                                              ((regval) == QSPI_XIP_CR_DDR_DISABLE             ) || \
                                              ((regval) == QSPI_XIP_CR_DDR_ENABLE              )  \
                                                 )

#define QSPI_XIP_CR_CSI                          BIT(3)                                      /*!< Internal slave select (slave mode vaild) - 1'b0: on - 1'b1: off */
#define QSPI_XIP_CR_CSI_OFS                      3U                                          /*!< QSPI XIP CR: CSI Bit Offset */
#define QSPI_XIP_CR_CSI_VAL(regval)                  (BIT(3) & ((uint32_t)(regval) << 3))        /*!< QSPI XIP CR: CSI Bit Value */  
#define QSPI_XIP_CR_CSI_ON                            0x0UL                                                  /*!< ON */
#define QSPI_XIP_CR_CSI_OFF                           BIT(3)                                                   /*!< OFF */

/**
  * \brief Check the QSPI_XIP spi_cr csi bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid csi bit.
  * \retval 1 This is a valid csi bit.
  */
#define IS_QSPI_XIP_SPI_CR_CSI(regval)                    (\
                                              ((regval) == QSPI_XIP_CR_CSI_ON                  ) || \
                                              ((regval) == QSPI_XIP_CR_CSI_OFF                 )  \
                                                 )

#define QSPI_XIP_CR_CSOE                         BIT(4)                                      /*!< CS output enable - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_CR_CSOE_OFS                     4U                                          /*!< QSPI XIP CR: CSOE Bit Offset */
#define QSPI_XIP_CR_CSOE_VAL(regval)                 (BIT(4) & ((uint32_t)(regval) << 4))        /*!< QSPI XIP CR: CSOE Bit Value */  
#define QSPI_XIP_CR_CSOE_DISABLE                      0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_CR_CSOE_ENABLE                       BIT(4)                                                   /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_cr csoe bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid csoe bit.
  * \retval 1 This is a valid csoe bit.
  */
#define IS_QSPI_XIP_SPI_CR_CSOE(regval)                   (\
                                             ((regval) == QSPI_XIP_CR_CSOE_DISABLE             ) || \
                                             ((regval) == QSPI_XIP_CR_CSOE_ENABLE              )  \
                                                 )

#define QSPI_XIP_CR_SSM                          BIT(5)                                      /*!< Software slave management - 1'b0: hard - 1'b1: soft */
#define QSPI_XIP_CR_SSM_OFS                      5U                                          /*!< QSPI XIP CR: SSM Bit Offset */
#define QSPI_XIP_CR_SSM_VAL(regval)                  (BIT(5) & ((uint32_t)(regval) << 5))        /*!< QSPI XIP CR: SSM Bit Value */  
#define QSPI_XIP_CR_SSM_HARD                          0x0UL                                                  /*!< HARD */
#define QSPI_XIP_CR_SSM_SOFT                          BIT(5)                                                   /*!< SOFT */

/**
  * \brief Check the QSPI_XIP spi_cr ssm bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid ssm bit.
  * \retval 1 This is a valid ssm bit.
  */
#define IS_QSPI_XIP_SPI_CR_SSM(regval)                    (\
                                              ((regval) == QSPI_XIP_CR_SSM_HARD                ) || \
                                              ((regval) == QSPI_XIP_CR_SSM_SOFT                )  \
                                                 )

#define QSPI_XIP_CR_RECEIVE_DATA                 BIT(13)                                      /*!< RX fifo receive data enable - 1'b0: disable - 1'b1: enable */
#define QSPI_XIP_CR_RECEIVE_DATA_OFS             13U                                          /*!< QSPI XIP CR: RECEIVE_DATA Bit Offset */
#define QSPI_XIP_CR_RECEIVE_DATA_VAL(regval)         (BIT(13) & ((uint32_t)(regval) << 13))        /*!< QSPI XIP CR: RECEIVE_DATA Bit Value */  
#define QSPI_XIP_CR_RECEIVE_DATA_DISABLE              0x0UL                                                  /*!< DISABLE */
#define QSPI_XIP_CR_RECEIVE_DATA_ENABLE               BIT(13)                                                  /*!< ENABLE */

/**
  * \brief Check the QSPI_XIP spi_cr receive_data bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid receive_data bit.
  * \retval 1 This is a valid receive_data bit.
  */
#define IS_QSPI_XIP_SPI_CR_RECEIVE_DATA(regval)           (\
                                     ((regval) == QSPI_XIP_CR_RECEIVE_DATA_DISABLE             ) || \
                                     ((regval) == QSPI_XIP_CR_RECEIVE_DATA_ENABLE              )  \
                                                 )

#define QSPI_XIP_CR_RX_FIFO_CLR                  BIT(26)                                      /*!< SPI clear rx fifo for fifo mode write 1 clear - 1'b0: none - 1'b1: clear Usage scenario: SPI clear rx fifo for fifo mode */
#define QSPI_XIP_CR_RX_FIFO_CLR_OFS              26U                                          /*!< QSPI XIP CR: RX_FIFO_CLR Bit Offset */
#define QSPI_XIP_CR_RX_FIFO_CLR_VAL(regval)          (BIT(26) & ((uint32_t)(regval) << 26))        /*!< QSPI XIP CR: RX_FIFO_CLR Bit Value */  
#define QSPI_XIP_CR_RX_FIFO_CLR_NONE                  0x0UL                                                  /*!< NONE */
#define QSPI_XIP_CR_RX_FIFO_CLR_CLEAR                 BIT(26)                                                  /*!< CLEAR */

/**
  * \brief Check the QSPI_XIP spi_cr rx_fifo_clr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid rx_fifo_clr bit.
  * \retval 1 This is a valid rx_fifo_clr bit.
  */
#define IS_QSPI_XIP_SPI_CR_RX_FIFO_CLR(regval)            (\
                                      ((regval) == QSPI_XIP_CR_RX_FIFO_CLR_NONE                ) || \
                                      ((regval) == QSPI_XIP_CR_RX_FIFO_CLR_CLEAR               )  \
                                                 )

#define QSPI_XIP_CR_TX_FIFO_CLR                  BIT(27)                                      /*!< SPI clear tx fifo for fifo mode write 1 clear - 1'b0: none - 1'b1: clear Usage scenario: SPI clear tx fifo for fifo mode */
#define QSPI_XIP_CR_TX_FIFO_CLR_OFS              27U                                          /*!< QSPI XIP CR: TX_FIFO_CLR Bit Offset */
#define QSPI_XIP_CR_TX_FIFO_CLR_VAL(regval)          (BIT(27) & ((uint32_t)(regval) << 27))        /*!< QSPI XIP CR: TX_FIFO_CLR Bit Value */  
#define QSPI_XIP_CR_TX_FIFO_CLR_NONE                  0x0UL                                                  /*!< NONE */
#define QSPI_XIP_CR_TX_FIFO_CLR_CLEAR                 BIT(27)                                                  /*!< CLEAR */

/**
  * \brief Check the QSPI_XIP spi_cr tx_fifo_clr bit parameters.
  * \param regval bit value to be checked.
  * \retval 0 This is not a valid tx_fifo_clr bit.
  * \retval 1 This is a valid tx_fifo_clr bit.
  */
#define IS_QSPI_XIP_SPI_CR_TX_FIFO_CLR(regval)            (\
                                      ((regval) == QSPI_XIP_CR_TX_FIFO_CLR_NONE                ) || \
                                      ((regval) == QSPI_XIP_CR_TX_FIFO_CLR_CLEAR               )  \
                                                 )

 /* ===== QSPI XIP SYS_DELAY Register definition ===== */
#define QSPI_XIP_SYS_DELAY_SPI_SYS_SAMPLE_DELAY_MASK    BITS(0,4)                                   /*!< QSPI XIP SYS DELAY: SPI_SYS_SAMPLE_DELAY Bit Mask */  
#define QSPI_XIP_SYS_DELAY_SPI_SYS_SAMPLE_DELAY_OFS     0U                                          /*!< QSPI XIP SYS DELAY: SPI_SYS_SAMPLE_DELAY Bit Offset */
#define QSPI_XIP_SYS_DELAY_SPI_SYS_SAMPLE_DELAY(regval) (BITS(0,4) & ((uint32_t)(regval) << 0))        /*!< QSPI XIP SYS DELAY: SPI_SYS_SAMPLE_DELAY Bit Value */  
#define QSPI_XIP_SYS_DELAY_EN                           BIT(31)                                      /*!< The register configuration becomes active only if the enable bit is set to 1. */
#define QSPI_XIP_SYS_DELAY_EN_OFS                       31U                                          /*!< QSPI XIP SYS DELAY: EN Bit Offset */
#define QSPI_XIP_SYS_DELAY_EN_VAL(regval)                   (BIT(31) & ((uint32_t)(regval) << 31))        /*!< QSPI XIP SYS DELAY: EN Bit Value */  

#ifdef QSPI_XIP_FMT_PROTO_X16
#undef QSPI_XIP_FMT_PROTO_X16
#define QSPI_XIP_FMT_PROTO_X16     (QSPI_XIP_FMT_PROTO(4) | QSPI_XIP_FMT_PROTO_HI_VAL(1))
#endif

#ifdef QSPI_XIP_FMT_PROTO_HDS
#undef QSPI_XIP_FMT_PROTO_HDS
#define QSPI_XIP_FMT_PROTO_HDS     (QSPI_XIP_FMT_PROTO(7) | QSPI_XIP_FMT_PROTO_HI_VAL(1))
#endif

/* ===== QSPI_XIP exported functions ===== */
void QSPI_XIP_Init(QSPI_XIP_TypeDef* QSPI_XIPx, QSPI_XIP_InitTypeDef* QSPI_XIP_Struct);
void QSPI_XIP_StructInit(QSPI_XIP_InitTypeDef* QSPI_XIP_Struct);

void QSPI_XIP_DdrModeEnable(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status);
void QSPI_XIP_SSMSwitch(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status);
void QSPI_XIP_FrameSizeConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t FrameSize);
void QSPI_XIP_BusWidthSwitch(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t BusWidth);
void QSPI_XIP_ForceEnable(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus HrStatus, ControlStatus WpStatus);

void QSPI_XIP_RxSdrSampleConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t SamplePoint);
void QSPI_XIP_TxmarkConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Mark);
void QSPI_XIP_RxmarkConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Mark);
void QSPI_XIP_TxsizeConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Size);
void QSPI_XIP_RxsizeConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Size);
void QSPI_XIP_DirectionConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Direction);
void QSPI_XIP_ITConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_IE, ControlStatus Status);
FlagStatus QSPI_XIP_GetITConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_IE);
FlagStatus QSPI_XIP_GetIPStatus(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_IT);
FlagStatus QSPI_XIP_GetFlag(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_FLAG);
void QSPI_XIP_ClearFlag(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_FLAG);
void QSPI_XIP_CSModeConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t CsMode);
void QSPI_XIP_CSIDConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t CsID);
void QSPI_XIP_Delay0Config(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Sckcs, uint32_t Cssck);
void QSPI_XIP_Delay1Config(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Intercs, uint32_t Interxfr);
void QSPI_XIP_SendData(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Data);
uint32_t QSPI_XIP_ReceiveData(QSPI_XIP_TypeDef* QSPI_XIPx);
void QSPI_XIP_DataWidthConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t DateWidth);

void QSPI_XIP_EnableXipWrite(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status);
void QSPI_XIP_EnableBoundaryMode(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status);
void QSPI_XIP_EnableBurstMode(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status);
void QSPI_XIP_EnableWrapMode(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status);
void QSPI_XIP_EnableXipWrite(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status);
void QSPI_XIP_Enable(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status);

void QSPI_XIP_DdrEnable(QSPI_XIP_TypeDef* QSPI_XIPx, QSPI_XIP_DdrEnableTypedef XipDdrMode);
void QSPI_XIP_TimingConfig(QSPI_XIP_TypeDef* QSPI_XIPx, QSPI_XIP_TimingTypedef* QSPI_XIP_Timing);

uint32_t QSPI_XIP_TxReadEntry(QSPI_XIP_TypeDef* QSPI_XIPx);
uint32_t QSPI_XIP_RxReadEntry(QSPI_XIP_TypeDef* QSPI_XIPx);

void QSPI_XIP_TransmitReceive(QSPI_XIP_TypeDef *QSPI_XIPx, uint8_t *pTxData, uint8_t *pRxData);
void QSPI_XIP_CSEnable(QSPI_XIP_TypeDef* QSPI_XIPx, uint8_t csid, ControlStatus Status);
void QSPI_XIP_RxFifoEnable(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status);
void QSPI_XIP_ClearFifo(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Channel);

#ifdef __cplusplus
}
#endif

#endif
/* _NS_QSPI_XIP_H_ */

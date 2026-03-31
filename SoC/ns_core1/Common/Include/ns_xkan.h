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

#ifndef _NS_XKAN_H_
#define _NS_XKAN_H_
/*!
 * \file     ns_xkan.h
 * \brief    This file contains all the functions prototypes for the xKan firmware
 */

#ifdef __cplusplus
 extern "C" {
#endif

#include "ns.h"

#ifdef CORE_ARCH_x64
typedef uint64_t ramtype_t;
#else
typedef uint32_t ramtype_t;
#endif

/* NOTE: Do not modify any macro defined values */

/* xKan timeout */
#define XKAN_TIMEOUT                 ((uint32_t)0x0000FFFFU) /*!< timeout value */

/* xkan inter ram size(word) */
#define XKAN_RAM_SIZE                ((uint32_t)0x800U) /* inter RAM size: 8K bytes */

#define XKAN_RX_LOW_RSP_SIZE               ((uint32_t)0x20) /*!< response reg value */
#define XKAN_RX_HIGH_RSP_SIZE              ((uint32_t)0x20) /*!< response reg value */

/* xKan_id_type xKan ID Type */
#define XKAN_STANDARD_ID ((uint32_t)0x00000000U) /*!< Standard ID element */
#define XKAN_EXTENDED_ID ((uint32_t)0x40000000U) /*!< Extended ID element */

/* xKan_frame_type xKan Frame Type */
#define XKAN_DATA_FRAME   ((uint32_t)0x00000000U)  /*!< Data frame   */
#define XKAN_REMOTE_FRAME ((uint32_t)0x20000000U)  /*!< Remote frame */

/* xKan fifo operation mode*/
#define XKAN_RXFIFO_OP_BLOCK      ((uint32_t)0x00000000U)  /*!< Blocking mode  */
#define XKAN_RXFIFO_OP_OVERWRITE  ((uint32_t)0x80000000U)  /*!< overwrite mode */

/* xKan_filter_type xKan Filter Type */
#define XKAN_FILTER_RANGE         ((uint32_t)0x00000000U) /*!< Range filter from FilterID1 to FilterID2                        */
#define XKAN_FILTER_DUAL          ((uint32_t)0x00000001U) /*!< Dual ID filter for FilterID1 or FilterID2                       */
#define XKAN_FILTER_MASK          ((uint32_t)0x00000002U) /*!< Classic filter: FilterID1 = filter, FilterID2 = mask            */

/* xKan_filter_config xKan Filter Configuration */
#define XKAN_FILTER_TO_RXFIFO0    ((uint32_t)0x00000000U) /*!< Store in Rx FIFO 0 if filter matches                      */
#define XKAN_FILTER_TO_RXFIFO1    ((uint32_t)0x00000001U) /*!< Store in Rx FIFO 1 if filter matches                      */
#define XKAN_FILTER_TO_RXBUFFER   ((uint32_t)0x00000002U) /*!< Store into Rx Buffer, configuration of FilterType ignored */

/* xKan_data_field_size xKan Data Field Size */
#define XKAN_DATA_BYTES_8  ((uint32_t)0x00000004U) /*!< 8 bytes data field  */
#define XKAN_DATA_BYTES_12 ((uint32_t)0x00000005U) /*!< 12 bytes data field */
#define XKAN_DATA_BYTES_16 ((uint32_t)0x00000006U) /*!< 16 bytes data field */
#define XKAN_DATA_BYTES_20 ((uint32_t)0x00000007U) /*!< 20 bytes data field */
#define XKAN_DATA_BYTES_24 ((uint32_t)0x00000008U) /*!< 24 bytes data field */
#define XKAN_DATA_BYTES_32 ((uint32_t)0x0000000AU) /*!< 32 bytes data field */
#define XKAN_DATA_BYTES_48 ((uint32_t)0x0000000EU) /*!< 48 bytes data field */
#define XKAN_DATA_BYTES_64 ((uint32_t)0x00000012U) /*!< 64 bytes data field */

#define XKAN_DLC_BYTES_0  ((uint32_t)0x00000000U) /*!< 0 bytes data field  */
#define XKAN_DLC_BYTES_1  ((uint32_t)0x00010000U) /*!< 1 bytes data field  */
#define XKAN_DLC_BYTES_2  ((uint32_t)0x00020000U) /*!< 2 bytes data field  */
#define XKAN_DLC_BYTES_3  ((uint32_t)0x00030000U) /*!< 3 bytes data field  */
#define XKAN_DLC_BYTES_4  ((uint32_t)0x00040000U) /*!< 4 bytes data field  */
#define XKAN_DLC_BYTES_5  ((uint32_t)0x00050000U) /*!< 5 bytes data field  */
#define XKAN_DLC_BYTES_6  ((uint32_t)0x00060000U) /*!< 6 bytes data field  */
#define XKAN_DLC_BYTES_7  ((uint32_t)0x00070000U) /*!< 7 bytes data field  */
#define XKAN_DLC_BYTES_8  ((uint32_t)0x00080000U) /*!< 8 bytes data field  */
#define XKAN_DLC_BYTES_12 ((uint32_t)0x00090000U) /*!< 12 bytes data field */
#define XKAN_DLC_BYTES_16 ((uint32_t)0x000A0000U) /*!< 16 bytes data field */
#define XKAN_DLC_BYTES_20 ((uint32_t)0x000B0000U) /*!< 20 bytes data field */
#define XKAN_DLC_BYTES_24 ((uint32_t)0x000C0000U) /*!< 24 bytes data field */
#define XKAN_DLC_BYTES_32 ((uint32_t)0x000D0000U) /*!< 32 bytes data field */
#define XKAN_DLC_BYTES_48 ((uint32_t)0x000E0000U) /*!< 48 bytes data field */
#define XKAN_DLC_BYTES_64 ((uint32_t)0x000F0000U) /*!< 64 bytes data field */

#define XKAN_ESI_ACTIVE  ((uint32_t)0x00000000U) /*!< Transmitting node is error active  */
#define XKAN_ESI_PASSIVE ((uint32_t)0x00200000U) /*!< Transmitting node is error passive */

/* XKAN_bit_rate_switching XKAN Bit Rate Switching */
#define XKAN_BRS_OFF ((uint32_t)0x00000000U) /*!< XKAN frames transmitted/received without bit rate switching */
#define XKAN_BRS_ON  ((uint32_t)0x00100000U) /*!< XKAN frames transmitted/received with bit rate switching */

/* XKAN_format XKAN format */
#define XKAN_CLASSIC_CAN ((uint32_t)0x00000000U) /*!< Frame transmitted/received in Classic CAN format */
#define XKAN_FD_CAN      ((uint32_t)0x80000000U) /*!< Frame transmitted/received in XKAN format */

/* XKAN_FIFO_watermark XKAN FIFO watermark */
#define XKAN_CFG_RX_FIFO0      ((uint32_t)0x00000000U) /*!< Rx FIFO0 */
#define XKAN_CFG_RX_FIFO1      ((uint32_t)0x00000001U) /*!< Rx FIFO1 */

#define XKAN_ACCEPT_IN_RX_FIFO0 ((uint32_t)0x00000000U) /*!< Accept in Rx FIFO 0 */
#define XKAN_ACCEPT_IN_RX_FIFO1 ((uint32_t)0x00000001U) /*!< Accept in Rx FIFO 1 */
#define XKAN_REJECT             ((uint32_t)0x00000002U) /*!< Reject */

#define XKAN_FILTER_REMOTE ((uint32_t)0x00000000U) /*!< Filter remote frames */
#define XKAN_REJECT_REMOTE ((uint32_t)0x00000001U) /*!< Reject all remote frames */

#define XKAN_TX_BUFFER0  ((uint32_t)0x00000001U) /*!< Add message to Tx Buffer 0  */
#define XKAN_TX_BUFFER1  ((uint32_t)0x00000002U) /*!< Add message to Tx Buffer 1  */
#define XKAN_TX_BUFFER2  ((uint32_t)0x00000004U) /*!< Add message to Tx Buffer 2  */
#define XKAN_TX_BUFFER3  ((uint32_t)0x00000008U) /*!< Add message to Tx Buffer 3  */
#define XKAN_TX_BUFFER4  ((uint32_t)0x00000010U) /*!< Add message to Tx Buffer 4  */
#define XKAN_TX_BUFFER5  ((uint32_t)0x00000020U) /*!< Add message to Tx Buffer 5  */
#define XKAN_TX_BUFFER6  ((uint32_t)0x00000040U) /*!< Add message to Tx Buffer 6  */
#define XKAN_TX_BUFFER7  ((uint32_t)0x00000080U) /*!< Add message to Tx Buffer 7  */
#define XKAN_TX_BUFFER8  ((uint32_t)0x00000100U) /*!< Add message to Tx Buffer 8  */
#define XKAN_TX_BUFFER9  ((uint32_t)0x00000200U) /*!< Add message to Tx Buffer 9  */
#define XKAN_TX_BUFFER10 ((uint32_t)0x00000400U) /*!< Add message to Tx Buffer 10 */
#define XKAN_TX_BUFFER11 ((uint32_t)0x00000800U) /*!< Add message to Tx Buffer 11 */
#define XKAN_TX_BUFFER12 ((uint32_t)0x00001000U) /*!< Add message to Tx Buffer 12 */
#define XKAN_TX_BUFFER13 ((uint32_t)0x00002000U) /*!< Add message to Tx Buffer 13 */
#define XKAN_TX_BUFFER14 ((uint32_t)0x00004000U) /*!< Add message to Tx Buffer 14 */
#define XKAN_TX_BUFFER15 ((uint32_t)0x00008000U) /*!< Add message to Tx Buffer 15 */
#define XKAN_TX_BUFFER16 ((uint32_t)0x00010000U) /*!< Add message to Tx Buffer 16 */
#define XKAN_TX_BUFFER17 ((uint32_t)0x00020000U) /*!< Add message to Tx Buffer 17 */
#define XKAN_TX_BUFFER18 ((uint32_t)0x00040000U) /*!< Add message to Tx Buffer 18 */
#define XKAN_TX_BUFFER19 ((uint32_t)0x00080000U) /*!< Add message to Tx Buffer 19 */
#define XKAN_TX_BUFFER20 ((uint32_t)0x00100000U) /*!< Add message to Tx Buffer 20 */
#define XKAN_TX_BUFFER21 ((uint32_t)0x00200000U) /*!< Add message to Tx Buffer 21 */
#define XKAN_TX_BUFFER22 ((uint32_t)0x00400000U) /*!< Add message to Tx Buffer 22 */
#define XKAN_TX_BUFFER23 ((uint32_t)0x00800000U) /*!< Add message to Tx Buffer 23 */
#define XKAN_TX_BUFFER24 ((uint32_t)0x01000000U) /*!< Add message to Tx Buffer 24 */
#define XKAN_TX_BUFFER25 ((uint32_t)0x02000000U) /*!< Add message to Tx Buffer 25 */
#define XKAN_TX_BUFFER26 ((uint32_t)0x04000000U) /*!< Add message to Tx Buffer 26 */
#define XKAN_TX_BUFFER27 ((uint32_t)0x08000000U) /*!< Add message to Tx Buffer 27 */
#define XKAN_TX_BUFFER28 ((uint32_t)0x10000000U) /*!< Add message to Tx Buffer 28 */
#define XKAN_TX_BUFFER29 ((uint32_t)0x20000000U) /*!< Add message to Tx Buffer 29 */
#define XKAN_TX_BUFFER30 ((uint32_t)0x40000000U) /*!< Add message to Tx Buffer 30 */
#define XKAN_TX_BUFFER31 ((uint32_t)0x80000000U) /*!< Add message to Tx Buffer 31 */

#define XKAN_RX_FIFO0    ((uint32_t)0x00000040U) /*!< Get received message from Rx FIFO 0    */
#define XKAN_RX_FIFO1    ((uint32_t)0x00000041U) /*!< Get received message from Rx FIFO 1    */
#define XKAN_RX_BUFFER0  ((uint32_t)0x00000000U) /*!< Get received message from Rx Buffer 0  */
#define XKAN_RX_BUFFER1  ((uint32_t)0x00000001U) /*!< Get received message from Rx Buffer 1  */
#define XKAN_RX_BUFFER2  ((uint32_t)0x00000002U) /*!< Get received message from Rx Buffer 2  */
#define XKAN_RX_BUFFER3  ((uint32_t)0x00000003U) /*!< Get received message from Rx Buffer 3  */
#define XKAN_RX_BUFFER4  ((uint32_t)0x00000004U) /*!< Get received message from Rx Buffer 4  */
#define XKAN_RX_BUFFER5  ((uint32_t)0x00000005U) /*!< Get received message from Rx Buffer 5  */
#define XKAN_RX_BUFFER6  ((uint32_t)0x00000006U) /*!< Get received message from Rx Buffer 6  */
#define XKAN_RX_BUFFER7  ((uint32_t)0x00000007U) /*!< Get received message from Rx Buffer 7  */
#define XKAN_RX_BUFFER8  ((uint32_t)0x00000008U) /*!< Get received message from Rx Buffer 8  */
#define XKAN_RX_BUFFER9  ((uint32_t)0x00000009U) /*!< Get received message from Rx Buffer 9  */
#define XKAN_RX_BUFFER10 ((uint32_t)0x0000000AU) /*!< Get received message from Rx Buffer 10 */
#define XKAN_RX_BUFFER11 ((uint32_t)0x0000000BU) /*!< Get received message from Rx Buffer 11 */
#define XKAN_RX_BUFFER12 ((uint32_t)0x0000000CU) /*!< Get received message from Rx Buffer 12 */
#define XKAN_RX_BUFFER13 ((uint32_t)0x0000000DU) /*!< Get received message from Rx Buffer 13 */
#define XKAN_RX_BUFFER14 ((uint32_t)0x0000000EU) /*!< Get received message from Rx Buffer 14 */
#define XKAN_RX_BUFFER15 ((uint32_t)0x0000000FU) /*!< Get received message from Rx Buffer 15 */
#define XKAN_RX_BUFFER16 ((uint32_t)0x00000010U) /*!< Get received message from Rx Buffer 16 */
#define XKAN_RX_BUFFER17 ((uint32_t)0x00000011U) /*!< Get received message from Rx Buffer 17 */
#define XKAN_RX_BUFFER18 ((uint32_t)0x00000012U) /*!< Get received message from Rx Buffer 18 */
#define XKAN_RX_BUFFER19 ((uint32_t)0x00000013U) /*!< Get received message from Rx Buffer 19 */
#define XKAN_RX_BUFFER20 ((uint32_t)0x00000014U) /*!< Get received message from Rx Buffer 20 */
#define XKAN_RX_BUFFER21 ((uint32_t)0x00000015U) /*!< Get received message from Rx Buffer 21 */
#define XKAN_RX_BUFFER22 ((uint32_t)0x00000016U) /*!< Get received message from Rx Buffer 22 */
#define XKAN_RX_BUFFER23 ((uint32_t)0x00000017U) /*!< Get received message from Rx Buffer 23 */
#define XKAN_RX_BUFFER24 ((uint32_t)0x00000018U) /*!< Get received message from Rx Buffer 24 */
#define XKAN_RX_BUFFER25 ((uint32_t)0x00000019U) /*!< Get received message from Rx Buffer 25 */
#define XKAN_RX_BUFFER26 ((uint32_t)0x0000001AU) /*!< Get received message from Rx Buffer 26 */
#define XKAN_RX_BUFFER27 ((uint32_t)0x0000001BU) /*!< Get received message from Rx Buffer 27 */
#define XKAN_RX_BUFFER28 ((uint32_t)0x0000001CU) /*!< Get received message from Rx Buffer 28 */
#define XKAN_RX_BUFFER29 ((uint32_t)0x0000001DU) /*!< Get received message from Rx Buffer 29 */
#define XKAN_RX_BUFFER30 ((uint32_t)0x0000001EU) /*!< Get received message from Rx Buffer 30 */
#define XKAN_RX_BUFFER31 ((uint32_t)0x0000001FU) /*!< Get received message from Rx Buffer 31 */
#define XKAN_RX_BUFFER32 ((uint32_t)0x00000020U) /*!< Get received message from Rx Buffer 32 */
#define XKAN_RX_BUFFER33 ((uint32_t)0x00000021U) /*!< Get received message from Rx Buffer 33 */
#define XKAN_RX_BUFFER34 ((uint32_t)0x00000022U) /*!< Get received message from Rx Buffer 34 */
#define XKAN_RX_BUFFER35 ((uint32_t)0x00000023U) /*!< Get received message from Rx Buffer 35 */
#define XKAN_RX_BUFFER36 ((uint32_t)0x00000024U) /*!< Get received message from Rx Buffer 36 */
#define XKAN_RX_BUFFER37 ((uint32_t)0x00000025U) /*!< Get received message from Rx Buffer 37 */
#define XKAN_RX_BUFFER38 ((uint32_t)0x00000026U) /*!< Get received message from Rx Buffer 38 */
#define XKAN_RX_BUFFER39 ((uint32_t)0x00000027U) /*!< Get received message from Rx Buffer 39 */
#define XKAN_RX_BUFFER40 ((uint32_t)0x00000028U) /*!< Get received message from Rx Buffer 40 */
#define XKAN_RX_BUFFER41 ((uint32_t)0x00000029U) /*!< Get received message from Rx Buffer 41 */
#define XKAN_RX_BUFFER42 ((uint32_t)0x0000002AU) /*!< Get received message from Rx Buffer 42 */
#define XKAN_RX_BUFFER43 ((uint32_t)0x0000002BU) /*!< Get received message from Rx Buffer 43 */
#define XKAN_RX_BUFFER44 ((uint32_t)0x0000002CU) /*!< Get received message from Rx Buffer 44 */
#define XKAN_RX_BUFFER45 ((uint32_t)0x0000002DU) /*!< Get received message from Rx Buffer 45 */
#define XKAN_RX_BUFFER46 ((uint32_t)0x0000002EU) /*!< Get received message from Rx Buffer 46 */
#define XKAN_RX_BUFFER47 ((uint32_t)0x0000002FU) /*!< Get received message from Rx Buffer 47 */
#define XKAN_RX_BUFFER48 ((uint32_t)0x00000030U) /*!< Get received message from Rx Buffer 48 */
#define XKAN_RX_BUFFER49 ((uint32_t)0x00000031U) /*!< Get received message from Rx Buffer 49 */
#define XKAN_RX_BUFFER50 ((uint32_t)0x00000032U) /*!< Get received message from Rx Buffer 50 */
#define XKAN_RX_BUFFER51 ((uint32_t)0x00000033U) /*!< Get received message from Rx Buffer 51 */
#define XKAN_RX_BUFFER52 ((uint32_t)0x00000034U) /*!< Get received message from Rx Buffer 52 */
#define XKAN_RX_BUFFER53 ((uint32_t)0x00000035U) /*!< Get received message from Rx Buffer 53 */
#define XKAN_RX_BUFFER54 ((uint32_t)0x00000036U) /*!< Get received message from Rx Buffer 54 */
#define XKAN_RX_BUFFER55 ((uint32_t)0x00000037U) /*!< Get received message from Rx Buffer 55 */
#define XKAN_RX_BUFFER56 ((uint32_t)0x00000038U) /*!< Get received message from Rx Buffer 56 */
#define XKAN_RX_BUFFER57 ((uint32_t)0x00000039U) /*!< Get received message from Rx Buffer 57 */
#define XKAN_RX_BUFFER58 ((uint32_t)0x0000003AU) /*!< Get received message from Rx Buffer 58 */
#define XKAN_RX_BUFFER59 ((uint32_t)0x0000003BU) /*!< Get received message from Rx Buffer 59 */
#define XKAN_RX_BUFFER60 ((uint32_t)0x0000003CU) /*!< Get received message from Rx Buffer 60 */
#define XKAN_RX_BUFFER61 ((uint32_t)0x0000003DU) /*!< Get received message from Rx Buffer 61 */
#define XKAN_RX_BUFFER62 ((uint32_t)0x0000003EU) /*!< Get received message from Rx Buffer 62 */
#define XKAN_RX_BUFFER63 ((uint32_t)0x0000003FU) /*!< Get received message from Rx Buffer 63 */

#define IS_XKAN_MAX_VALUE(VALUE, MAX) ((VALUE) <= (MAX))
#define IS_XKAN_MIN_VALUE(VALUE, MIN) ((VALUE) >= (MIN))
#define IS_XKAN_GREATER(MAX, MIN) ((MAX) > (MIN))

/** 
  * \brief  xKan communication mode enum definition
  */
typedef enum {
    XKAN_NORMAL_MODE = 0,              /*!< normal communication mode */
    XKAN_LOOPBACK_MODE = 1,            /*!< loopback communication mode */
    XKAN_SILENT_MODE = 2,              /*!< silent communication mode */
    XKAN_SILENT_LOOPBACK_MODE = 3,     /*!< loopback and silent communication mode */
    XKAN_RESTRICTED_OPERATION_MODE = 4,/*!< restricted Operation mode */
} xKan_TestModeTypeDef;

/** 
  * \brief  xKan protocol exception handling mode enum definition
  */
typedef enum {
    XKAN_EXCEPT_ACK = 0,                /*!< treat a frame (r0 bit is recessive) as a valid frame and acknowledge this frame. */
    XKAN_EXCEPT_NACK = 1,               /*!< treat a frame (r0 bit is recessive) as a protocol exception event and shall not acknowledge this frame. */
    XKAN_EXCEPT_RSP_FMT_ERR = 2,        /*!< l treat a frame (r0 bit is recessive) as a form error and will respond with an error frame. */
} xKan_ProtoExceptHandleTypeDef;

/** 
  * \brief  xKan work mode enum definition
  */
typedef enum {
    XKAN_FRAME_CLASSIC = 0,             /*!< Classic mode */
    XKAN_FRAME_FD_NO_BRS = 1,           /*!< FD mode without BitRate Switshing */
    XKAN_FRAME_FD_BRS = 2,              /*!< FD mode with BitRate Switshing */
} xKan_FrameTypeDef;

/** 
  * \brief  xKan err code enum definition
  */
typedef enum {
    XKAN_NO_ERROR = 0,              /*!< No Error */
    XKAN_STUFF_ERROR = 1,           /*!< Stuff Error */
    XKAN_FORMAT_ERROR = 2,          /*!< Form Error */
    XKAN_NACK_ERROR = 3,            /*!< Acknowledgment Error */
    XKAN_RECESSIVE_ERROR = 4,       /*!< Bit recessive Error */
    XKAN_DOMINANT_ERROR = 5,        /*!< Bit dominant Error */
    XKAN_CRC_ERROR = 6,             /*!< CRC Error */
} xKan_ErrCodeTypeDef;

/** 
  * \brief  xKan nominal time structure definition
  */
typedef struct {
    uint32_t NominalPrescaler;
    uint32_t NominalSyncJumpWidth;
    uint32_t NominalTimeSeg1;
    uint32_t NominalTimeSeg2;
} xKan_NominalTypeDef;

/** 
  * \brief  xKan data time structure definition
  */
typedef struct {
    uint32_t DataPrescaler;
    uint32_t DataSyncJumpWidth;
    uint32_t DataTimeSeg1;
    uint32_t DataTimeSeg2;
} xKan_DataTypeDef;

/**
  * \brief  xKan Message RAM definition
  */
typedef struct
{
    uint32_t StandardFilterSA; /*!< Specifies the Standard Filter List Start Address.
                                    This parameter must be a 32-bit word address      */
    uint32_t ExtendedFilterSA; /*!< Specifies the Extended Filter List Start Address.
                                    This parameter must be a 32-bit word address      */
    uint32_t RxFIFO0SA;        /*!< Specifies the Rx FIFO 0 Start Address.
                                    This parameter must be a 32-bit word address      */
    uint32_t RxFIFO1SA;        /*!< Specifies the Rx FIFO 1 Start Address.
                                    This parameter must be a 32-bit word address      */
    uint32_t RxBufferSA;       /*!< Specifies the Rx Buffer Start Address.
                                    This parameter must be a 32-bit word address      */
    uint32_t TxBufferSA;       /*!< Specifies the Tx Buffers Start Address.
                                    This parameter must be a 32-bit word address      */
    uint32_t TxFIFOSA;         /*!< Specifies the Tx FIFO Start Address.
                                    This parameter must be a 32-bit word address      */
    uint32_t EndAddress;       /*!< Specifies the End Address of the allocated RAM.
                                    This parameter must be a 32-bit word address      */
} xKan_RamAddressTypeDef;

/**
  * \brief xKan Init structure definition
  */
typedef struct
{
    uint32_t MessageRAMOffset;             /*!< Specifies the message RAM start address.
                                                This parameter must be a number between 0 and 2560           */
    uint32_t StdFiltersNbr;                /*!< Specifies the number of standard Message ID filters.
                                                This parameter must be a number between 0 and 128            */
    uint32_t ExtFiltersNbr;                /*!< Specifies the number of extended Message ID filters.
                                                This parameter must be a number between 0 and 64             */
    uint32_t RxFifo0ElmtsNbr;              /*!< Specifies the number of Rx FIFO0 Elements.
                                               This parameter must be a number between 0 and 64             */
    uint32_t RxFifo0ElmtSize;              /*!< Specifies the Data Field Size in an Rx FIFO 0 element.
                                                This parameter can be a value of xKan_data_field_size  */
    uint32_t RxFifo1ElmtsNbr;              /*!< Specifies the number of Rx FIFO 1 Elements.
                                                This parameter must be a number between 0 and 64             */
    uint32_t RxFifo1ElmtSize;              /*!< Specifies the Data Field Size in an Rx FIFO 1 element.
                                                This parameter can be a value of xKan_data_field_size  */
    uint32_t RxBuffersNbr;                 /*!< Specifies the number of Dedicated Rx Buffer elements.
                                                This parameter must be a number between 0 and 64             */
    uint32_t RxBufferSize;                 /*!< Specifies the Data Field Size in an Rx Buffer element.
                                                This parameter can be a value of xKan_data_field_size  */
    uint32_t TxBuffersNbr;                 /*!< Specifies the number of Dedicated Tx Buffers.
                                                This parameter must be a number between 0 and 32             */
    uint32_t TxFifoElmtsNbr;               /*!< Specifies the number of Tx Buffers used for Tx FIFO.
                                                This parameter must be a number between 0 and 32             */
    
    uint32_t TxElmtSize;                   /*!< Specifies the Data Field Size in a Tx Element.
                                                This parameter can be a value of xKan_data_field_size  */
    xKan_RamAddressTypeDef ram_addr;
} xKan_RamInfoTypeDef;

/** 
  * \brief  xKan init structure definition
  */

typedef struct {
    uint8_t working_mode;                     /*!< xKan working mode */
    uint8_t frame_format;                     /*!< xKan frame format */
    uint8_t protocol_except_handle;           /*!< xKan protocol exception handling */
    ControlStatus receive_in_arb_loss;        /*!< xKan become receive mode in case of loss arbitration*/
    ControlStatus tx_delay_compensation;      /*!< transmit delay compensation */
    
    ControlStatus auto_retransmit;            /*!< hardware auto retransmit function */
    ControlStatus time_stamp;                 /*!< time triggered communication mode */
    ControlStatus transmit_pause;             /*!< tx pause */
    xKan_NominalTypeDef normal_time;          /*!< normal timing struct */
    xKan_DataTypeDef data_time;               /*!< data timing struct */
    xKan_RamInfoTypeDef ram_info;             /*!< ram info struct */
} xKan_InitTypeDef;

/**
  * \brief  xKan filter structure definition
  */
typedef struct
{
    uint32_t IdType;           /*!< Specifies the identifier type.
                                    This parameter can be a value of xKan_id_type       */
    uint32_t FilterIndex;      /*!< Specifies the filter which will be initialized.
                                    This parameter must be a number between:
                                    - 0 and 127, if IdType is xKan_STANDARD_ID
                                    - 0 and 63, if IdType is xKan_EXTENDED_ID               */
    uint32_t FilterType;       /*!< Specifies the filter type.
                                    This parameter can be a value of xKan_filter_type.
                                    The value xKan_EXT_FILTER_RANGE_NO_EIDM is permitted
                                    only when IdType is xKan_EXTENDED_ID.
                                    This parameter is ignored if FilterConfig is set to
                                    xKan_FILTER_TO_RXBUFFER                                  */
    uint32_t FilterConfig;     /*!< Specifies the filter configuration.
                                    This parameter can be a value of xKan_filter_config */
    uint32_t FilterID1;        /*!< Specifies the filter identification 1.
                                    This parameter must be a number between:
                                    - 0 and 0x7FF, if IdType is xKan_STANDARD_ID
                                    - 0 and 0x1FFFFFFF, if IdType is xKan_EXTENDED_ID       */
    uint32_t FilterID2;        /*!< Specifies the filter identification 2.
                                    This parameter is ignored if FilterConfig is set to
                                    xKan_FILTER_TO_RXBUFFER.
                                    This parameter must be a number between:
                                    - 0 and 0x7FF, if IdType is xKan_STANDARD_ID
                                    - 0 and 0x1FFFFFFF, if IdType is xKan_EXTENDED_ID       */
    uint32_t RxBufferIndex;    /*!< Contains the index of the Rx buffer in which the
                                    matching message will be stored.
                                    This parameter must be a number between 0 and 63.
                                    This parameter is ignored if FilterConfig is different
                                    from xKan_FILTER_TO_RXBUFFER                             */
    
} xKan_FilterTypeDef;

/**
  * \brief  xKan Tx header structure definition
  */
typedef struct
{
    uint32_t Identifier;          /*!< Specifies the identifier.
                                      This parameter must be a number between:
                                        - 0 and 0x7FF, if IdType is xKan_STANDARD_ID
                                        - 0 and 0x1FFFFFFF, if IdType is xKan_EXTENDED_ID               */
    uint32_t IdType;              /*!< Specifies the identifier type for the message that will be
                                      transmitted.
                                      This parameter can be a value of xKan_id_type               */
    uint32_t TxFrameType;         /*!< Specifies the frame type of the message that will be transmitted.
                                      This parameter can be a value of xKan_frame_type            */
    uint32_t DataLength;          /*!< Specifies the length of the frame that will be transmitted.
                                        This parameter can be a value of xKan_data_length_code     */
    uint32_t ErrorStateIndicator; /*!< Specifies the error state indicator.
                                      This parameter can be a value of xKan_error_state_indicator */
    uint32_t BitRateSwitch;       /*!< Specifies whether the Tx frame will be transmitted with or without
                                      bit rate switching.
                                      This parameter can be a value of xKan_bit_rate_switching    */
    uint32_t FDFormat;            /*!< Specifies whether the Tx frame will be transmitted in classic or
                                      FD format.
                                      This parameter can be a value of xKan_format                */
    
} xKan_TxHeaderTypeDef;

/**
  * \brief  xKan Rx header structure definition
  */
typedef struct
{
    uint32_t Identifier;            /*!< Specifies the identifier.
                                        This parameter must be a number between:
                                          - 0 and 0x7FF, if IdType is xKan_STANDARD_ID
                                          - 0 and 0x1FFFFFFF, if IdType is xKan_EXTENDED_ID               */
    uint32_t IdType;                /*!< Specifies the identifier type of the received message.
                                        This parameter can be a value of xKan_id_type               */
    uint32_t RxFrameType;           /*!< Specifies the the received message frame type.
                                        This parameter can be a value of xKan_frame_type            */
    uint32_t DataLength;            /*!< Specifies the received frame length.
                                          This parameter can be a value of xKan_data_length_code     */
    uint32_t ErrorStateIndicator;   /*!< Specifies the error state indicator.
                                        This parameter can be a value of xKan_error_state_indicator */
    uint32_t BitRateSwitch;         /*!< Specifies whether the Rx frame is received with or without bit
                                        rate switching.
                                        This parameter can be a value of xKan_bit_rate_switching    */
    uint32_t FDFormat;              /*!< Specifies whether the Rx frame is received in classic or FD
                                        format.
                                        This parameter can be a value of xKan_format                */
    uint32_t RxTimestamp;           /*!< Specifies the timestamp counter value captured on start of frame
                                        reception.
                                        This parameter must be a number between 0 and 0xFFFF              */
    uint32_t FilterIndex;           /*!< Specifies the index of matching Rx acceptance filter element.
                                        This parameter must be a number between:
                                          - 0 and 127, if IdType is xKan_STANDARD_ID
                                          - 0 and 63, if IdType is xKan_EXTENDED_ID                       */
    uint32_t IsFilterMatchingFrame; /*!< Specifies whether the accepted frame did not match any Rx filter.
                                          Acceptance of non-matching frames may be enabled via
                                          HAL_xKan_ConfigGlobalFilter().
                                          This parameter can be 0 or 1                                    */
} xKan_RxHeaderTypeDef;

#define XKAN_RELEASE_OFFSET                                            0x0 /*!< XKAN Core release version register */
#define XKAN_CFG_MCR_OFFSET                                            0x4 /*!< XKAN master control register */
#define XKAN_CFG_TEST_OFFSET                                           0x8 /*!< XKAN master control register */
#define XKAN_CFG_PSR_OFFSET                                            0xc /*!< XKAN protocol status register */
#define XKAN_CFG_MSR_OFFSET                                            0x10 /*!< XKAN master status register */
#define XKAN_CFG_GLOBAL_FILTER_OFFSET                                  0x14 /*!< XKAN global filter configuration register */
#define XKAN_CFG_ST_FILTER_OFFSET                                      0x18 /*!< XKAN standard ID filter configuration register */
#define XKAN_CFG_EX_FILTER_OFFSET                                      0x1c /*!< XKAN extended ID filter configuration register */
#define XKAN_CFG_STIDM_OFFSET                                          0x20 /*!< XKAN standard ID and mask register */
#define XKAN_CFG_EXIDM_OFFSET                                          0x24 /*!< XKAN extended ID and mask register */
#define XKAN_CFG_NBTR_OFFSET                                           0x28 /*!< XKAN nominal bit timing register */
#define XKAN_CFG_DBTR_OFFSET                                           0x2c /*!< XKAN data bit timing register */
#define XKAN_CFG_ISO_MODE_OFFSET                                       0x30 /*!< ISO11898 mode register */
#define XKAN_CFG_RXDATL_RSP_OFFSET                                     0x40 /*!< XKAN rx low buffer (0 to 31) update status bit */
#define XKAN_CFG_RXDATH_RSP_OFFSET                                     0x44 /*!< XKAN rx high buffer (32 to 63) update status bit */
#define XKAN_CFG_RXFIFO0_OFFSET                                        0x48 /*!< XKAN Rx FIFO 0 configuration register */
#define XKAN_CFG_RXFIFO0_STATUS_OFFSET                                 0x4c /*!< XKAN Rx FIFO 0 status register */
#define XKAN_CFG_RXFIFO0_ACK_OFFSET                                    0x50 /*!< XKAN Rx FIFO 0 acknowledge register */
#define XKAN_CFG_RXBUF_OFFSET                                          0x54 /*!< XKAN Rx buffer configuration register */
#define XKAN_CFG_RXFIFO1_OFFSET                                        0x58 /*!< XKAN Rx FIFO 1 configuration register */
#define XKAN_CFG_RXFIFO1_STATUS_OFFSET                                 0x5c /*!< XKAN Rx FIFO 1 status register */
#define XKAN_CFG_RXFIFO1_ACK_OFFSET                                    0x60 /*!< XKAN Rx FIFO 1 acknowledge register */
#define XKAN_CFG_RX_DSIZE_OFFSET                                       0x64 /*!< XKAN Rx buffer element size configuration register */
#define XKAN_CFG_TXBUF_OFFSET                                          0x70 /*!< XKAN Tx buffer configuration register */
#define XKAN_CFG_TXFIFO_STATUS_OFFSET                                  0x74 /*!< XKAN Tx FIFO status register */
#define XKAN_CFG_TX_DSIZE_OFFSET                                       0x78 /*!< XKAN Tx buffer element size configuration register */
#define XKAN_CFG_TXBAR_OFFSET                                          0x7c /*!< XKAN Tx buffer request register */
#define XKAN_CFG_TXABORT_REQ_OFFSET                                    0x80 /*!< XKAN Tx buffer abort request register */
#define XKAN_CFG_TXABORT_FINISH_OFFSET                                 0x84 /*!< Tx buffer abort finished register */
#define XKAN_CFG_TXREQ_PENDING_OFFSET                                  0x88 /*!< XKAN Tx buffer request pending register */
#define XKAN_CFG_TXB_TRANS_OFFSET                                      0x8c /*!< XKAN Tx buffer transmission complete register */
#define XKAN_CFG_TIME_STAMP_OFFSET                                     0x90 /*!< Message time stamp */
#define XKAN_CFG_TIME_DIV_OFFSET                                       0x94 /*!< Timestamp counter divider */
#define XKAN_CFG_TRANS_DELAY_COMP_OFFSET                               0x98 /*!< Transmitter delay compensation register */
#define XKAN_CFG_ESR_OFFSET                                            0x9c /*!< XKAN error status register */
#define XKAN_CFG_TRANS_ERR_OFFSET                                      0xa0 /*!< Error transmit buffer number */
#define XKAN_CFG_INT_EN_OFFSET                                         0xb4 /*!< XKAN interrupt enable register */
#define XKAN_CFG_INT_STATUS_OFFSET                                     0xb8 /*!< XKAN interrupt status register */

 /* ===== XKAN RELEASE Register definition ===== */
#define XKAN_RELEASE_VERSION                      BITS(0,31)                
 
 /* ===== XKAN CFG_MCR Register definition ===== */
#define XKAN_CFG_MCR_ENABLE                       BIT(0)                                      /*!< XKAN work enable 0: XKAN kernel disable 1: XKAN kernel enable */
#define XKAN_CFG_MCR_ENABLE_OFS                   0U                                          /*!< XKAN CFG MCR: ENABLE Bit Offset */
#define XKAN_CFG_MCR_ENABLE_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG MCR: ENABLE Bit Value */  
#define XKAN_CFG_MCR_INITIAL_REQ                  BIT(1)                                      /*!< Initialization mode request Software sets this bit to request the XKAN hardware to enter initialization mode for initial XKAN Controller, The software clears this bit to switch the hardware into normal mode.  Once 11 consecutive recessive bits have been monitored on the Rx signal the XKAN hardware is synchronized and ready for transmission and reception.  Hardware signals this event by clearing the INI_ACK bit in the CFG_MSR register.  */
#define XKAN_CFG_MCR_INITIAL_REQ_OFS              1U                                          /*!< XKAN CFG MCR: INITIAL_REQ Bit Offset */
#define XKAN_CFG_MCR_INITIAL_REQ_VAL(regval)          (BIT(1) & ((uint32_t)(regval) << 1))        /*!< XKAN CFG MCR: INITIAL_REQ Bit Value */  
#define XKAN_CFG_MCR_SLEEP_REQ                    BIT(2)                                      /*!< Sleep mode request This bit is set by software to request the XKAN hardware to enter the Sleep mode. Sleep mode will be entered as soon as the current CAN activity (transmission or reception of a CAN frame) has been completed. This bit is cleared by software to exit Sleep mode or by hardware when the CFG_MSR.WAKE_UP bit is set and a SOF bit is detected on the XKAN Rx signal. */
#define XKAN_CFG_MCR_SLEEP_REQ_OFS                2U                                          /*!< XKAN CFG MCR: SLEEP_REQ Bit Offset */
#define XKAN_CFG_MCR_SLEEP_REQ_VAL(regval)            (BIT(2) & ((uint32_t)(regval) << 2))        /*!< XKAN CFG MCR: SLEEP_REQ Bit Value */  
#define XKAN_CFG_MCR_TDC_EN                       BIT(3)                                      /*!< Transceiver Delay Compensation 0: Transceiver Delay Compensation disabled 1: Transceiver Delay Compensation enabled */
#define XKAN_CFG_MCR_TDC_EN_OFS                   3U                                          /*!< XKAN CFG MCR: TDC_EN Bit Offset */
#define XKAN_CFG_MCR_TDC_EN_VAL(regval)               (BIT(3) & ((uint32_t)(regval) << 3))        /*!< XKAN CFG MCR: TDC_EN Bit Value */  
#define XKAN_CFG_MCR_AUTO_RETRANS_EN              BIT(4)                                      /*!< Automatic re-transmission enable 0: Disable automatic re-transmission of messages not transmitted successfully, which due to arbitration lost or some protocal errors. 1: Enable automatic re-transmission  */
#define XKAN_CFG_MCR_AUTO_RETRANS_EN_OFS          4U                                          /*!< XKAN CFG MCR: AUTO_RETRANS_EN Bit Offset */
#define XKAN_CFG_MCR_AUTO_RETRANS_EN_VAL(regval)      (BIT(4) & ((uint32_t)(regval) << 4))        /*!< XKAN CFG MCR: AUTO_RETRANS_EN Bit Value */  
#define XKAN_CFG_MCR_TIMESTAMP_EN                 BIT(6)                                      /*!< Time stamp mode enable 0: Time stamp mode disabled. 1: Time stamp mode enabled */
#define XKAN_CFG_MCR_TIMESTAMP_EN_OFS             6U                                          /*!< XKAN CFG MCR: TIMESTAMP_EN Bit Offset */
#define XKAN_CFG_MCR_TIMESTAMP_EN_VAL(regval)         (BIT(6) & ((uint32_t)(regval) << 6))        /*!< XKAN CFG MCR: TIMESTAMP_EN Bit Value */  
#define XKAN_CFG_MCR_FD_EN                        BIT(7)                                      /*!< Flexible Data-Rate operation enable 0: Flexible Data-Rate operation disabled 1: Flexible Data-Rate operation enabled */
#define XKAN_CFG_MCR_FD_EN_OFS                    7U                                          /*!< XKAN CFG MCR: FD_EN Bit Offset */
#define XKAN_CFG_MCR_FD_EN_VAL(regval)                (BIT(7) & ((uint32_t)(regval) << 7))        /*!< XKAN CFG MCR: FD_EN Bit Value */  
#define XKAN_CFG_MCR_BRS_EN                       BIT(8)                                      /*!< CAN Flexible Data-Rate Switching 0: Bitrate switching for transmissions disabled 1: Bitrate switching for transmissions enabled */
#define XKAN_CFG_MCR_BRS_EN_OFS                   8U                                          /*!< XKAN CFG MCR: BRS_EN Bit Offset */
#define XKAN_CFG_MCR_BRS_EN_VAL(regval)               (BIT(8) & ((uint32_t)(regval) << 8))        /*!< XKAN CFG MCR: BRS_EN Bit Value */  
#define XKAN_CFG_MCR_TXPAUSE                      BIT(9)                                      /*!< If this bit is set, the XKAN pauses for two CAN bit times before starting the next transmission after successfully transmitting a frame. 0: disabled 1: enabled */
#define XKAN_CFG_MCR_TXPAUSE_OFS                  9U                                          /*!< XKAN CFG MCR: TXPAUSE Bit Offset */
#define XKAN_CFG_MCR_TXPAUSE_VAL(regval)              (BIT(9) & ((uint32_t)(regval) << 9))        /*!< XKAN CFG MCR: TXPAUSE Bit Value */  
#define XKAN_CFG_MCR_CLK_STOP_REQ                 BIT(11)                                      /*!< Clock stop request 0: No clock stop is requested 1: Clock stop requested. When clock stop is requested, first INIT and then CSA will be set after all pending transfer requests have been completed and the CAN bus reached idle. */
#define XKAN_CFG_MCR_CLK_STOP_REQ_OFS             11U                                          /*!< XKAN CFG MCR: CLK_STOP_REQ Bit Offset */
#define XKAN_CFG_MCR_CLK_STOP_REQ_VAL(regval)         (BIT(11) & ((uint32_t)(regval) << 11))        /*!< XKAN CFG MCR: CLK_STOP_REQ Bit Value */  
#define XKAN_CFG_MCR_CLK_STOP_ACK                 BIT(12)                                      /*!< Clock stop acknowledge 0: No clock stop acknowledged 1: XKAN may be set in power down by stopping icb clock and kernel clock */
#define XKAN_CFG_MCR_TX_PIN_PULL_UP               BIT(13)                                      /*!< XKAN TX pin pull-up 0: TX pin pull-up disable 1: TX pin pull-up enable */
#define XKAN_CFG_MCR_TX_PIN_PULL_UP_OFS           13U                                          /*!< XKAN CFG MCR: TX_PIN_PULL_UP Bit Offset */
#define XKAN_CFG_MCR_TX_PIN_PULL_UP_VAL(regval)       (BIT(13) & ((uint32_t)(regval) << 13))        /*!< XKAN CFG MCR: TX_PIN_PULL_UP Bit Value */  
#define XKAN_CFG_MCR_RX_PIN_PULL_UP               BIT(14)                                      /*!< XKAN RX pin pull-up 0: RX pin pull-up disable 1: RX pin pull-up enable */
#define XKAN_CFG_MCR_RX_PIN_PULL_UP_OFS           14U                                          /*!< XKAN CFG MCR: RX_PIN_PULL_UP Bit Offset */
#define XKAN_CFG_MCR_RX_PIN_PULL_UP_VAL(regval)       (BIT(14) & ((uint32_t)(regval) << 14))        /*!< XKAN CFG MCR: RX_PIN_PULL_UP Bit Value */  
#define XKAN_CFG_MCR_RESTRICTED_OPERATION         BIT(15)                                      /*!< Restricted operation enable 0: Restricted operation disable 1: Restricted operation enable */
#define XKAN_CFG_MCR_RESTRICTED_OPERATION_OFS     15U                                          /*!< XKAN CFG MCR: RESTRICTED_OPERATION Bit Offset */
#define XKAN_CFG_MCR_RESTRICTED_OPERATION_VAL(regval) (BIT(15) & ((uint32_t)(regval) << 15))        /*!< XKAN CFG MCR: RESTRICTED_OPERATION Bit Value */  
#define XKAN_CFG_MCR_BE_RECEIVER_IN_LOSS          BIT(16)                                      /*!< Become receive mode in case of loss arbitration 0: XKAN will not receive current frame and keep IDLE state in loss arbitration  1: XKAN will receive current frame when loss arbitration */
#define XKAN_CFG_MCR_BE_RECEIVER_IN_LOSS_OFS      16U                                          /*!< XKAN CFG MCR: BE_RECEIVER_IN_LOSS Bit Offset */
#define XKAN_CFG_MCR_BE_RECEIVER_IN_LOSS_VAL(regval)  (BIT(16) & ((uint32_t)(regval) << 16))        /*!< XKAN CFG MCR: BE_RECEIVER_IN_LOSS Bit Value */  
#define XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING_MASK    BITS(17,18)                                   /*!< XKAN CFG MCR: PROTOCOL_EXCEPTION_HANDLING Bit Mask */  
#define XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING_OFS     17U                                          /*!< XKAN CFG MCR: PROTOCOL_EXCEPTION_HANDLING Bit Offset */
#define XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING(regval) (BITS(17,18) & ((uint32_t)(regval) << 17))        /*!< XKAN CFG MCR: PROTOCOL_EXCEPTION_HANDLING Bit Value */  
#define XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING_PRO_EXE_AS_VALID             XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING(0)                                               /*!< PRO_EXE_AS_VALID */
#define XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING_PRO_EXE_AS_PXE             XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING(1)                                               /*!< PRO_EXE_AS_PXE */
#define XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING_PRO_EXE_AS_FORM_ERR             XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING(2)                                               /*!< PRO_EXE_AS_FORM_ERR */

/**
  * \brief Check the XKAN cfg_mcr protocol_exception_handling bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid protocol_exception_handling bits.
  * \retval 1 This is a valid protocol_exception_handling bits.
  */
#define IS_XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING(regval)   (\
                             ((regval) == XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING_PRO_EXE_AS_VALID    ) || \
                             ((regval) == XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING_PRO_EXE_AS_PXE      ) || \
                             ((regval) == XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING_PRO_EXE_AS_FORM_ERR )  \
                                                 )

 /* ===== XKAN CFG_TEST Register definition ===== */
#define XKAN_CFG_TEST_LBKM                         BIT(0)                                      /*!< Loop back mode enable 0: Loop back mode is disabled 1: Loop back mode is enabled */
#define XKAN_CFG_TEST_LBKM_OFS                     0U                                          /*!< XKAN CFG TEST: LBKM Bit Offset */
#define XKAN_CFG_TEST_LBKM_VAL(regval)                 (BIT(0) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG TEST: LBKM Bit Value */  
#define XKAN_CFG_TEST_SILM                         BIT(1)                                      /*!< Silent mode (debug) 0: Normal operation 1: Silent Mode */
#define XKAN_CFG_TEST_SILM_OFS                     1U                                          /*!< XKAN CFG TEST: SILM Bit Offset */
#define XKAN_CFG_TEST_SILM_VAL(regval)                 (BIT(1) & ((uint32_t)(regval) << 1))        /*!< XKAN CFG TEST: SILM Bit Value */  
 
 /* ===== XKAN CFG_PSR Register definition ===== */
#define XKAN_CFG_PSR_IDE                          BIT(0)                                      /*!< IDE flag of last received CAN Flexible Data-Rate message This bit is set together when IDE is detected, independent of acceptance filtering. 0: Last received CAN or CAN Flexible Data-Rate message did not have its IDE flag set 1: Last received CAN or CAN Flexible Data-Rate message had its IDE flag set */
#define XKAN_CFG_PSR_IDE_OFS                      0U                                          /*!< XKAN CFG PSR: IDE Bit Offset */
#define XKAN_CFG_PSR_IDE_VAL(regval)                  (BIT(0) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG PSR: IDE Bit Value */  
#define XKAN_CFG_PSR_EDL                          BIT(1)                                      /*!< Received CAN Flexible Data-Rate message This bit is set together when EDL is detected, independent of acceptance filtering. 0: Since this bit was reset by the CPU, no CAN Flexible Data-Rate message has been received 1: Message in CAN Flexible Data-Rate format with EDL flag set has been received */
#define XKAN_CFG_PSR_EDL_OFS                      1U                                          /*!< XKAN CFG PSR: EDL Bit Offset */
#define XKAN_CFG_PSR_EDL_VAL(regval)                  (BIT(1) & ((uint32_t)(regval) << 1))        /*!< XKAN CFG PSR: EDL Bit Value */  
#define XKAN_CFG_PSR_BRS                          BIT(2)                                      /*!< BRS flag of last received CAN Flexible Data-Rate message This bit is set together when BRS is detected, independent of acceptance filtering. 0: Last received CAN Flexible Data-Rate message did not have its BRS flag set 1: Last received CAN Flexible Data-Rate message had its BRS flag set */
#define XKAN_CFG_PSR_BRS_OFS                      2U                                          /*!< XKAN CFG PSR: BRS Bit Offset */
#define XKAN_CFG_PSR_BRS_VAL(regval)                  (BIT(2) & ((uint32_t)(regval) << 2))        /*!< XKAN CFG PSR: BRS Bit Value */  
#define XKAN_CFG_PSR_ESI                          BIT(3)                                      /*!< ESI flag of last received CAN Flexible Data-Rate message This bit is set together when ESI is detected, independent of acceptance filtering. 0: Last received CAN Flexible Data-Rate message did not have its ESI flag set 1: Last received CAN Flexible Data-Rate message had its ESI flag set */
#define XKAN_CFG_PSR_ESI_OFS                      3U                                          /*!< XKAN CFG PSR: ESI Bit Offset */
#define XKAN_CFG_PSR_ESI_VAL(regval)                  (BIT(3) & ((uint32_t)(regval) << 3))        /*!< XKAN CFG PSR: ESI Bit Value */  
#define XKAN_CFG_PSR_PXE                          BIT(4)                                      /*!< Protocol exception event 0: No protocol exception event occurred  1: Protocol exception event occurred  */
#define XKAN_CFG_PSR_PXE_OFS                      4U                                          /*!< XKAN CFG PSR: PXE Bit Offset */
#define XKAN_CFG_PSR_PXE_VAL(regval)                  (BIT(4) & ((uint32_t)(regval) << 4))        /*!< XKAN CFG PSR: PXE Bit Value */  
#define XKAN_CFG_PSR_TRANS_DELAY_COMP             BITS(16,22)                
 
 /* ===== XKAN CFG_MSR Register definition ===== */
#define XKAN_CFG_MSR_ICB_FUNC_RESET_STATUS         BIT(0)                                      /*!< XKAN function reset status, which triggered by transmit error The error on can bus, just as frames that have not been acknowledge and frames that have been disturbed by errors during transmission will trigger the hard-ware reset for some icb clock domain function unit, This bit indicates the status of XKAN function reset.   0: XKAN function reset is no-active. 1: XKAN function reset is active. */
#define XKAN_CFG_MSR_INITIAL_ACK                  BIT(1)                                      /*!< Initialization acknowledge This bit is set by hardware and indicates to the software that the XKAN hardware is now in initialization mode.  This bit acknowledges the initialization request from the software (set INRQ bit in CFG_MCR register). This bit is cleared by hardware when the XCAN hardware has left the initialization mode (to be synchronized on the CAN bus).  To be synchronized the hardware has to monitor a sequence of 11 consecutive recessive bits on the XKAN RX signal. */
#define XKAN_CFG_MSR_SLEEP_ACK                    BIT(2)                                      /*!< Sleep acknowledge This bit is set by hardware and indicates to the software that the XKAN hardware is now in Sleep mode.  This bit acknowledges the Sleep mode request from the software (set SLEEP bit in CFG_MCR register). This bit is cleared by hardware when the XKAN hardware has left Sleep mode (to be synchronized on the CAN bus). To be synchronized the hardware has to monitor a sequence of 11 consecutive recessive bits on the XKAN RX signal. */
#define XKAN_CFG_MSR_NORMAL_ACK                   BIT(3)                                      /*!< Normal acknowledge This bit is set by hardware and indicates to the software that the XKAN hardware is now in Normal mode.  This bit is cleared by hardware when the XKAN hardware has left Normal mode. */
#define XKAN_CFG_MSR_WAKE_UP                      BIT(5)                                      /*!< Wakeup status This bit is set by hardware to signal that a SOF bit has been detected while the XKAN hardware was in Sleep mode.  This bit is cleared by software. */
#define XKAN_CFG_MSR_WAKE_UP_OFS                  5U                                          /*!< XKAN CFG MSR: WAKE_UP Bit Offset */
#define XKAN_CFG_MSR_WAKE_UP_VAL(regval)              (BIT(5) & ((uint32_t)(regval) << 5))        /*!< XKAN CFG MSR: WAKE_UP Bit Value */  
#define XKAN_CFG_MSR_TX_MODE                      BIT(6)                                      /*!< Transmit mode The XKAN hardware is currently transmitter. */
#define XKAN_CFG_MSR_RX_MODE                      BIT(7)                                      /*!< Receive mode The XKAN hardware is currently receiver. */
#define XKAN_CFG_MSR_RX_PREV_VALUE                BIT(8)                                      /*!< The RX previous value  The value of RX which is sampled by sample point (current received bit value). */
#define XKAN_CFG_MSR_RX_BUS                       BIT(9)                                      /*!< XKAN Rx bus signal Monitors the actual value of the XKAN_RX Pin. */
#define XKAN_CFG_MSR_TX_BUF_IDX                   BITS(10,14)                
 
 /* ===== XKAN CFG_GLOBAL_FILTER Register definition ===== */
#define XKAN_CFG_GLOBAL_FILTER_REJECT_EX_REMOTE             BIT(0)                                      /*!< Reject remote frames extended 0: Filter remote frames with 29-bit standard ID 1: Reject all remote frames with 29-bit standard ID */
#define XKAN_CFG_GLOBAL_FILTER_REJECT_EX_REMOTE_OFS         0U                                          /*!< XKAN CFG GLOBAL FILTER: REJECT_EX_REMOTE Bit Offset */
#define XKAN_CFG_GLOBAL_FILTER_REJECT_EX_REMOTE_VAL(regval)     (BIT(0) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG GLOBAL FILTER: REJECT_EX_REMOTE Bit Value */  
#define XKAN_CFG_GLOBAL_FILTER_REJECT_ST_REMOTE             BIT(1)                                      /*!< Reject remote frames standard 0: Filter remote frames with 11-bit standard ID 1: Reject all remote frames with 11-bit standard ID */
#define XKAN_CFG_GLOBAL_FILTER_REJECT_ST_REMOTE_OFS         1U                                          /*!< XKAN CFG GLOBAL FILTER: REJECT_ST_REMOTE Bit Offset */
#define XKAN_CFG_GLOBAL_FILTER_REJECT_ST_REMOTE_VAL(regval)     (BIT(1) & ((uint32_t)(regval) << 1))        /*!< XKAN CFG GLOBAL FILTER: REJECT_ST_REMOTE Bit Value */  
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME_MASK    BITS(2,3)                                   /*!< XKAN CFG GLOBAL FILTER: ACCEPT_MISS_EX_FRAME Bit Mask */  
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME_OFS     2U                                          /*!< XKAN CFG GLOBAL FILTER: ACCEPT_MISS_EX_FRAME Bit Offset */
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME(regval) (BITS(2,3) & ((uint32_t)(regval) << 2))        /*!< XKAN CFG GLOBAL FILTER: ACCEPT_MISS_EX_FRAME Bit Value */  
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME_FIFO0_ENABLE             XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME(0)                                     /*!< FIFO0_ENABLE */
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME_FIFO1_ENABLE             XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME(1)                                     /*!< FIFO1_ENABLE */
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME_FIFO_DISABLE0             XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME(2)                                     /*!< FIFO_DISABLE0 */
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME_FIFO_DISABLE1             XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME(3)                                     /*!< FIFO_DISABLE1 */

/**
  * \brief Check the XKAN cfg_global_filter accept_miss_ex_frame bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid accept_miss_ex_frame bits.
  * \retval 1 This is a valid accept_miss_ex_frame bits.
  */
#define IS_XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME(regval)   (\
                             ((regval) == XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME_FIFO0_ENABLE        ) || \
                             ((regval) == XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME_FIFO1_ENABLE        ) || \
                             ((regval) == XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME_FIFO_DISABLE0       ) || \
                             ((regval) == XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_EX_FRAME_FIFO_DISABLE1       )  \
                                                 )

#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME_MASK    BITS(4,5)                                   /*!< XKAN CFG GLOBAL FILTER: ACCEPT_MISS_ST_FRAME Bit Mask */  
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME_OFS     4U                                          /*!< XKAN CFG GLOBAL FILTER: ACCEPT_MISS_ST_FRAME Bit Offset */
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME(regval) (BITS(4,5) & ((uint32_t)(regval) << 4))        /*!< XKAN CFG GLOBAL FILTER: ACCEPT_MISS_ST_FRAME Bit Value */  
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME_FIFO0_ENABLE             XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME(0)                                     /*!< FIFO0_ENABLE */
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME_FIFO1_ENABLE             XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME(1)                                     /*!< FIFO1_ENABLE */
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME_FIFO_DISABLE0             XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME(2)                                     /*!< FIFO_DISABLE0 */
#define XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME_FIFO_DISABLE1             XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME(3)                                     /*!< FIFO_DISABLE1 */

/**
  * \brief Check the XKAN cfg_global_filter accept_miss_st_frame bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid accept_miss_st_frame bits.
  * \retval 1 This is a valid accept_miss_st_frame bits.
  */
#define IS_XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME(regval)   (\
                             ((regval) == XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME_FIFO0_ENABLE        ) || \
                             ((regval) == XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME_FIFO1_ENABLE        ) || \
                             ((regval) == XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME_FIFO_DISABLE0       ) || \
                             ((regval) == XKAN_CFG_GLOBAL_FILTER_ACCEPT_MISS_ST_FRAME_FIFO_DISABLE1       )  \
                                                 )

 /* ===== XKAN CFG_ST_FILTER Register definition ===== */
#define XKAN_CFG_ST_FILTER_STFLT_ADDR_MASK              BITS(0,10)                                   /*!< XKAN CFG ST FILTER: STFLT_ADDR Bit Mask */  
#define XKAN_CFG_ST_FILTER_STFLT_ADDR_OFS               0U                                          /*!< XKAN CFG ST FILTER: STFLT_ADDR Bit Offset */
#define XKAN_CFG_ST_FILTER_STFLT_ADDR(regval)           (BITS(0,10) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG ST FILTER: STFLT_ADDR Bit Value */  
#define XKAN_CFG_ST_FILTER_STFLT_NUM_MASK               BITS(12,19)                                   /*!< XKAN CFG ST FILTER: STFLT_NUM Bit Mask */  
#define XKAN_CFG_ST_FILTER_STFLT_NUM_OFS                12U                                          /*!< XKAN CFG ST FILTER: STFLT_NUM Bit Offset */
#define XKAN_CFG_ST_FILTER_STFLT_NUM(regval)            (BITS(12,19) & ((uint32_t)(regval) << 12))        /*!< XKAN CFG ST FILTER: STFLT_NUM Bit Value */  
 
 /* ===== XKAN CFG_EX_FILTER Register definition ===== */
#define XKAN_CFG_EX_FILTER_EXFLT_ADDR_MASK              BITS(0,10)                                   /*!< XKAN CFG EX FILTER: EXFLT_ADDR Bit Mask */  
#define XKAN_CFG_EX_FILTER_EXFLT_ADDR_OFS               0U                                          /*!< XKAN CFG EX FILTER: EXFLT_ADDR Bit Offset */
#define XKAN_CFG_EX_FILTER_EXFLT_ADDR(regval)           (BITS(0,10) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG EX FILTER: EXFLT_ADDR Bit Value */  
#define XKAN_CFG_EX_FILTER_EXFLT_NUM_MASK               BITS(12,19)                                   /*!< XKAN CFG EX FILTER: EXFLT_NUM Bit Mask */  
#define XKAN_CFG_EX_FILTER_EXFLT_NUM_OFS                12U                                          /*!< XKAN CFG EX FILTER: EXFLT_NUM Bit Offset */
#define XKAN_CFG_EX_FILTER_EXFLT_NUM(regval)            (BITS(12,19) & ((uint32_t)(regval) << 12))        /*!< XKAN CFG EX FILTER: EXFLT_NUM Bit Value */  
 
 /* ===== XKAN CFG_STIDM Register definition ===== */
#define XKAN_CFG_STIDM_SIDM_MASK                    BITS(0,10)                                   /*!< XKAN CFG STIDM: SIDM Bit Mask */  
#define XKAN_CFG_STIDM_SIDM_OFS                     0U                                          /*!< XKAN CFG STIDM: SIDM Bit Offset */
#define XKAN_CFG_STIDM_SIDM(regval)                 (BITS(0,10) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG STIDM: SIDM Bit Value */  
 
 /* ===== XKAN CFG_EXIDM Register definition ===== */
#define XKAN_CFG_EXIDM_EIDM_MASK                    BITS(0,28)                                   /*!< XKAN CFG EXIDM: EIDM Bit Mask */  
#define XKAN_CFG_EXIDM_EIDM_OFS                     0U                                          /*!< XKAN CFG EXIDM: EIDM Bit Offset */
#define XKAN_CFG_EXIDM_EIDM(regval)                 (BITS(0,28) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG EXIDM: EIDM Bit Value */  
 
 /* ===== XKAN CFG_NBTR Register definition ===== */
#define XKAN_CFG_NBTR_NBRP_MASK                    BITS(0,8)                                   /*!< XKAN CFG NBTR: NBRP Bit Mask */  
#define XKAN_CFG_NBTR_NBRP_OFS                     0U                                          /*!< XKAN CFG NBTR: NBRP Bit Offset */
#define XKAN_CFG_NBTR_NBRP(regval)                 (BITS(0,8) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG NBTR: NBRP Bit Value */  
#define XKAN_CFG_NBTR_NTSEG1_MASK                  BITS(9,16)                                   /*!< XKAN CFG NBTR: NTSEG1 Bit Mask */  
#define XKAN_CFG_NBTR_NTSEG1_OFS                   9U                                          /*!< XKAN CFG NBTR: NTSEG1 Bit Offset */
#define XKAN_CFG_NBTR_NTSEG1(regval)               (BITS(9,16) & ((uint32_t)(regval) << 9))        /*!< XKAN CFG NBTR: NTSEG1 Bit Value */  
#define XKAN_CFG_NBTR_NTSEG2_MASK                  BITS(17,23)                                   /*!< XKAN CFG NBTR: NTSEG2 Bit Mask */  
#define XKAN_CFG_NBTR_NTSEG2_OFS                   17U                                          /*!< XKAN CFG NBTR: NTSEG2 Bit Offset */
#define XKAN_CFG_NBTR_NTSEG2(regval)               (BITS(17,23) & ((uint32_t)(regval) << 17))        /*!< XKAN CFG NBTR: NTSEG2 Bit Value */  
#define XKAN_CFG_NBTR_NSJW_MASK                    BITS(24,30)                                   /*!< XKAN CFG NBTR: NSJW Bit Mask */  
#define XKAN_CFG_NBTR_NSJW_OFS                     24U                                          /*!< XKAN CFG NBTR: NSJW Bit Offset */
#define XKAN_CFG_NBTR_NSJW(regval)                 (BITS(24,30) & ((uint32_t)(regval) << 24))        /*!< XKAN CFG NBTR: NSJW Bit Value */  
 
 /* ===== XKAN CFG_DBTR Register definition ===== */
#define XKAN_CFG_DBTR_DBRP_MASK                    BITS(0,4)                                   /*!< XKAN CFG DBTR: DBRP Bit Mask */  
#define XKAN_CFG_DBTR_DBRP_OFS                     0U                                          /*!< XKAN CFG DBTR: DBRP Bit Offset */
#define XKAN_CFG_DBTR_DBRP(regval)                 (BITS(0,4) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG DBTR: DBRP Bit Value */  
#define XKAN_CFG_DBTR_DTSEG1_MASK                  BITS(5,9)                                   /*!< XKAN CFG DBTR: DTSEG1 Bit Mask */  
#define XKAN_CFG_DBTR_DTSEG1_OFS                   5U                                          /*!< XKAN CFG DBTR: DTSEG1 Bit Offset */
#define XKAN_CFG_DBTR_DTSEG1(regval)               (BITS(5,9) & ((uint32_t)(regval) << 5))        /*!< XKAN CFG DBTR: DTSEG1 Bit Value */  
#define XKAN_CFG_DBTR_DTSEG2_MASK                  BITS(10,13)                                   /*!< XKAN CFG DBTR: DTSEG2 Bit Mask */  
#define XKAN_CFG_DBTR_DTSEG2_OFS                   10U                                          /*!< XKAN CFG DBTR: DTSEG2 Bit Offset */
#define XKAN_CFG_DBTR_DTSEG2(regval)               (BITS(10,13) & ((uint32_t)(regval) << 10))        /*!< XKAN CFG DBTR: DTSEG2 Bit Value */  
#define XKAN_CFG_DBTR_DSJW_MASK                    BITS(14,17)                                   /*!< XKAN CFG DBTR: DSJW Bit Mask */  
#define XKAN_CFG_DBTR_DSJW_OFS                     14U                                          /*!< XKAN CFG DBTR: DSJW Bit Offset */
#define XKAN_CFG_DBTR_DSJW(regval)                 (BITS(14,17) & ((uint32_t)(regval) << 14))        /*!< XKAN CFG DBTR: DSJW Bit Value */  
 
 /* ===== XKAN CFG_ISO_MODE Register definition ===== */
#define XKAN_CFG_ISO_MODE_ENABLE                       BIT(0)                                      /*!< ISO 11898 Mode enable 0: CRC calculations as per BOSCH CAN with Flexible Data-Rate Specification Version 1.0. 1: CRC calculations as per ISO specification. Note: In ISO mode:  1. The initial value shall be  (0,....,0) for CRC15 and shall be (1,0,...,0) for CRC17 and for CRC21; where the single "1" is at the most significant bit position. 2. In FD Frames, the 3-bits stuff count and its parity bit shall be at the beginning of the CRC field. 3. Include stuff bit count in CRC calculation bit stream. In BOSCH mode:  1. The initial value shall be  (0,....,0) for CRC15 , for CRC17 and for CRC21. 2. In FD Frames, There is no stuff count shall be at the beginning of the CRC field. */
#define XKAN_CFG_ISO_MODE_ENABLE_OFS                   0U                                          /*!< XKAN CFG ISO MODE: ENABLE Bit Offset */
#define XKAN_CFG_ISO_MODE_ENABLE_VAL(regval)               (BIT(0) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG ISO MODE: ENABLE Bit Value */  
 
 /* ===== XKAN CFG_RXDATL_RSP Register definition ===== */
#define XKAN_CFG_RXDATL_RSP_LOW_MASK                     BITS(0,31)                                   /*!< XKAN CFG RXDATL RSP: LOW Bit Mask */  
#define XKAN_CFG_RXDATL_RSP_LOW_OFS                      0U                                          /*!< XKAN CFG RXDATL RSP: LOW Bit Offset */
#define XKAN_CFG_RXDATL_RSP_LOW(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG RXDATL RSP: LOW Bit Value */  
 
 /* ===== XKAN CFG_RXDATH_RSP Register definition ===== */
#define XKAN_CFG_RXDATH_RSP_HIGH_MASK                    BITS(0,31)                                   /*!< XKAN CFG RXDATH RSP: HIGH Bit Mask */  
#define XKAN_CFG_RXDATH_RSP_HIGH_OFS                     0U                                          /*!< XKAN CFG RXDATH RSP: HIGH Bit Offset */
#define XKAN_CFG_RXDATH_RSP_HIGH(regval)                 (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG RXDATH RSP: HIGH Bit Value */  
 
 /* ===== XKAN CFG_RXFIFO0 Register definition ===== */
#define XKAN_CFG_RXFIFO0_RXFF0_ADDR_MASK              BITS(0,10)                                   /*!< XKAN CFG RXFIFO0: RXFF0_ADDR Bit Mask */  
#define XKAN_CFG_RXFIFO0_RXFF0_ADDR_OFS               0U                                          /*!< XKAN CFG RXFIFO0: RXFF0_ADDR Bit Offset */
#define XKAN_CFG_RXFIFO0_RXFF0_ADDR(regval)           (BITS(0,10) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG RXFIFO0: RXFF0_ADDR Bit Value */  
#define XKAN_CFG_RXFIFO0_RXFF0_NUM_MASK               BITS(12,18)                                   /*!< XKAN CFG RXFIFO0: RXFF0_NUM Bit Mask */  
#define XKAN_CFG_RXFIFO0_RXFF0_NUM_OFS                12U                                          /*!< XKAN CFG RXFIFO0: RXFF0_NUM Bit Offset */
#define XKAN_CFG_RXFIFO0_RXFF0_NUM(regval)            (BITS(12,18) & ((uint32_t)(regval) << 12))        /*!< XKAN CFG RXFIFO0: RXFF0_NUM Bit Value */  
#define XKAN_CFG_RXFIFO0_RXFF0_WM_MASK                BITS(19,25)                                   /*!< XKAN CFG RXFIFO0: RXFF0_WM Bit Mask */  
#define XKAN_CFG_RXFIFO0_RXFF0_WM_OFS                 19U                                          /*!< XKAN CFG RXFIFO0: RXFF0_WM Bit Offset */
#define XKAN_CFG_RXFIFO0_RXFF0_WM(regval)             (BITS(19,25) & ((uint32_t)(regval) << 19))        /*!< XKAN CFG RXFIFO0: RXFF0_WM Bit Value */  
#define XKAN_CFG_RXFIFO0_RXFF0_CLR                    BIT(30)                                      /*!< FIFO 0 Clear When soft-ware write "1" to this bit, The status of Rx FIFO0 are all return to default value. */
#define XKAN_CFG_RXFIFO0_RXFF0_CLR_OFS                30U                                          /*!< XKAN CFG RXFIFO0: RXFF0_CLR Bit Offset */
#define XKAN_CFG_RXFIFO0_RXFF0_CLR_VAL(regval)            (BIT(30) & ((uint32_t)(regval) << 30))        /*!< XKAN CFG RXFIFO0: RXFF0_CLR Bit Value */  
#define XKAN_CFG_RXFIFO0_RXFF0_OPM                    BIT(31)                                      /*!< FIFO 0 operation mode FIFO 0 can be operated in blocking or in overwrite mode. 0: FIFO 0 blocking mode 1: FIFO 0 overwrite mode */
#define XKAN_CFG_RXFIFO0_RXFF0_OPM_OFS                31U                                          /*!< XKAN CFG RXFIFO0: RXFF0_OPM Bit Offset */
#define XKAN_CFG_RXFIFO0_RXFF0_OPM_VAL(regval)            (BIT(31) & ((uint32_t)(regval) << 31))        /*!< XKAN CFG RXFIFO0: RXFF0_OPM Bit Value */  
 
 /* ===== XKAN CFG_RXFIFO0_STATUS Register definition ===== */
#define XKAN_CFG_RXFIFO0_STATUS_RXFF0_ENTRY_NUM              BITS(1,7)                
#define XKAN_CFG_RXFIFO0_STATUS_RXFF0_RPTR                   BITS(8,13)                
#define XKAN_CFG_RXFIFO0_STATUS_RXFF0_WPTR                   BITS(16,21)                
#define XKAN_CFG_RXFIFO0_STATUS_RXFF0_FULL                   BIT(24)                                      /*!< Rx FIFO 0 Full 0: Rx FIFO 0 not full 1: Rx FIFO 0 full */
#define XKAN_CFG_RXFIFO0_STATUS_RXFF0_LOST                   BIT(25)                                      /*!< Rx FIFO 0 message Lost 0: No Rx FIFO 0 message lost 1: Rx FIFO 0 message lost, Which can be cleared by soft-ware write "1" to this bit. Note: This bit can be triggered by two source  1. When Rx FIFO 0 is working in the blocking mode, If new CAN frame is coming when Rx FIFO 0 is full, This bit is stand for new message has lost. 2. When Rx FIFO 0 is working in the overwrite mode, If new CAN frame is coming when Rx FIFO 0 is full, This bit is stand for the oldest entry in Rx FIFO 0 is overwrite by new message. */
#define XKAN_CFG_RXFIFO0_STATUS_RXFF0_LOST_OFS               25U                                          /*!< XKAN CFG RXFIFO0 STATUS: RXFF0_LOST Bit Offset */
#define XKAN_CFG_RXFIFO0_STATUS_RXFF0_LOST_VAL(regval)           (BIT(25) & ((uint32_t)(regval) << 25))        /*!< XKAN CFG RXFIFO0 STATUS: RXFF0_LOST Bit Value */  
 
 /* ===== XKAN CFG_RXFIFO0_ACK Register definition ===== */
#define XKAN_CFG_RXFIFO0_ACK_RXF0_ACK                     BIT(31)                                      /*!< Rx FIFO 0 acknowledge  After the Host has read a message or a sequence of messages from Rx FIFO 0 it has to write 1 to this bit for acknowledge.  This will set the Rx FIFO 0 read pointer CFG_RXFIFO0_STATUS.RXFF0_RPTR to move next address and update the Rx FIFO 0 fill level CFG_RXFIFO0_STATUS.RXFF0_ENTRY_NUM. */
#define XKAN_CFG_RXFIFO0_ACK_RXF0_ACK_OFS                 31U                                          /*!< XKAN CFG RXFIFO0 ACK: RXF0_ACK Bit Offset */
#define XKAN_CFG_RXFIFO0_ACK_RXF0_ACK_VAL(regval)             (BIT(31) & ((uint32_t)(regval) << 31))        /*!< XKAN CFG RXFIFO0 ACK: RXF0_ACK Bit Value */  
 
 /* ===== XKAN CFG_RXBUF Register definition ===== */
#define XKAN_CFG_RXBUF_RXBUF_ADDR_MASK              BITS(0,10)                                   /*!< XKAN CFG RXBUF: RXBUF_ADDR Bit Mask */  
#define XKAN_CFG_RXBUF_RXBUF_ADDR_OFS               0U                                          /*!< XKAN CFG RXBUF: RXBUF_ADDR Bit Offset */
#define XKAN_CFG_RXBUF_RXBUF_ADDR(regval)           (BITS(0,10) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG RXBUF: RXBUF_ADDR Bit Value */  
 
 /* ===== XKAN CFG_RXFIFO1 Register definition ===== */
#define XKAN_CFG_RXFIFO1_RXFF1_ADDR_MASK              BITS(0,10)                                   /*!< XKAN CFG RXFIFO1: RXFF1_ADDR Bit Mask */  
#define XKAN_CFG_RXFIFO1_RXFF1_ADDR_OFS               0U                                          /*!< XKAN CFG RXFIFO1: RXFF1_ADDR Bit Offset */
#define XKAN_CFG_RXFIFO1_RXFF1_ADDR(regval)           (BITS(0,10) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG RXFIFO1: RXFF1_ADDR Bit Value */  
#define XKAN_CFG_RXFIFO1_RXFF1_NUM_MASK               BITS(12,18)                                   /*!< XKAN CFG RXFIFO1: RXFF1_NUM Bit Mask */  
#define XKAN_CFG_RXFIFO1_RXFF1_NUM_OFS                12U                                          /*!< XKAN CFG RXFIFO1: RXFF1_NUM Bit Offset */
#define XKAN_CFG_RXFIFO1_RXFF1_NUM(regval)            (BITS(12,18) & ((uint32_t)(regval) << 12))        /*!< XKAN CFG RXFIFO1: RXFF1_NUM Bit Value */  
#define XKAN_CFG_RXFIFO1_RXFF1_WM_MASK                BITS(19,25)                                   /*!< XKAN CFG RXFIFO1: RXFF1_WM Bit Mask */  
#define XKAN_CFG_RXFIFO1_RXFF1_WM_OFS                 19U                                          /*!< XKAN CFG RXFIFO1: RXFF1_WM Bit Offset */
#define XKAN_CFG_RXFIFO1_RXFF1_WM(regval)             (BITS(19,25) & ((uint32_t)(regval) << 19))        /*!< XKAN CFG RXFIFO1: RXFF1_WM Bit Value */  
#define XKAN_CFG_RXFIFO1_RXFF1_CLR                    BIT(30)                                      /*!< FIFO 1 Clear When soft-ware write "1" to this bit, The status of Rx FIFO1 are all return to default value. */
#define XKAN_CFG_RXFIFO1_RXFF1_CLR_OFS                30U                                          /*!< XKAN CFG RXFIFO1: RXFF1_CLR Bit Offset */
#define XKAN_CFG_RXFIFO1_RXFF1_CLR_VAL(regval)            (BIT(30) & ((uint32_t)(regval) << 30))        /*!< XKAN CFG RXFIFO1: RXFF1_CLR Bit Value */  
#define XKAN_CFG_RXFIFO1_RXFF1_OPM                    BIT(31)                                      /*!< FIFO 1 operation mode FIFO 1 can be operated in blocking or in overwrite mode. 0: FIFO 1 blocking mode 1: FIFO 1 overwrite mode */
#define XKAN_CFG_RXFIFO1_RXFF1_OPM_OFS                31U                                          /*!< XKAN CFG RXFIFO1: RXFF1_OPM Bit Offset */
#define XKAN_CFG_RXFIFO1_RXFF1_OPM_VAL(regval)            (BIT(31) & ((uint32_t)(regval) << 31))        /*!< XKAN CFG RXFIFO1: RXFF1_OPM Bit Value */  
 
 /* ===== XKAN CFG_RXFIFO1_STATUS Register definition ===== */
#define XKAN_CFG_RXFIFO1_STATUS_RXFF1_ENTRY_NUM              BITS(1,7)                
#define XKAN_CFG_RXFIFO1_STATUS_RXFF1_RPTR                   BITS(8,13)                
#define XKAN_CFG_RXFIFO1_STATUS_RXFF1_WPTR                   BITS(16,21)                
#define XKAN_CFG_RXFIFO1_STATUS_RXFF1_FULL                   BIT(24)                                      /*!< Rx FIFO 1 Full 0: Rx FIFO 1 not full 1: Rx FIFO 1 full */
#define XKAN_CFG_RXFIFO1_STATUS_RXFF1_LOST                   BIT(25)                                      /*!< Rx FIFO 1 message Lost 0: No Rx FIFO 1 message lost 1: Rx FIFO 1 message lost,  Which can be cleared by soft-ware write "1" to this bit. Note: This bit can be triggered by two source  1. When Rx FIFO 1 is working in the blocking mode, If new CAN frame is coming when Rx FIFO 1 is full, This bit is stand for new message has lost. 2. When Rx FIFO 1 is working in the overwrite mode, If new CAN frame is coming when Rx FIFO 1 is full, This bit is stand for the oldest entry in Rx FIFO 1 is overwrite by new message. */
#define XKAN_CFG_RXFIFO1_STATUS_RXFF1_LOST_OFS               25U                                          /*!< XKAN CFG RXFIFO1 STATUS: RXFF1_LOST Bit Offset */
#define XKAN_CFG_RXFIFO1_STATUS_RXFF1_LOST_VAL(regval)           (BIT(25) & ((uint32_t)(regval) << 25))        /*!< XKAN CFG RXFIFO1 STATUS: RXFF1_LOST Bit Value */  
 
 /* ===== XKAN CFG_RXFIFO1_ACK Register definition ===== */
#define XKAN_CFG_RXFIFO1_ACK_RXF1_ACK                     BIT(31)                                      /*!< Rx FIFO 1 acknowledge  After the Host has read a message or a sequence of messages from Rx FIFO 1 it has to write 1 to this bit for acknowledge.  This will set the Rx FIFO 1 read pointer CFG_RXFIFO1_STATUS.RXFF1_RPTR to move next address and update the Rx FIFO 1 fill level CFG_RXFIFO1_STATUS.RXFF1_ENTRY_NUM. */
#define XKAN_CFG_RXFIFO1_ACK_RXF1_ACK_OFS                 31U                                          /*!< XKAN CFG RXFIFO1 ACK: RXF1_ACK Bit Offset */
#define XKAN_CFG_RXFIFO1_ACK_RXF1_ACK_VAL(regval)             (BIT(31) & ((uint32_t)(regval) << 31))        /*!< XKAN CFG RXFIFO1 ACK: RXF1_ACK Bit Value */  
 
 /* ===== XKAN CFG_RX_DSIZE Register definition ===== */
#define XKAN_CFG_RX_DSIZE_F0_DSIZE_MASK                BITS(0,2)                                   /*!< XKAN CFG RX DSIZE: F0_DSIZE Bit Mask */  
#define XKAN_CFG_RX_DSIZE_F0_DSIZE_OFS                 0U                                          /*!< XKAN CFG RX DSIZE: F0_DSIZE Bit Offset */
#define XKAN_CFG_RX_DSIZE_F0_DSIZE(regval)             (BITS(0,2) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG RX DSIZE: F0_DSIZE Bit Value */  
#define XKAN_CFG_RX_DSIZE_F0_DSIZE_8                        XKAN_CFG_RX_DSIZE_F0_DSIZE(0)                                          /*!< 8 */
#define XKAN_CFG_RX_DSIZE_F0_DSIZE_12                       XKAN_CFG_RX_DSIZE_F0_DSIZE(1)                                          /*!< 12 */
#define XKAN_CFG_RX_DSIZE_F0_DSIZE_16                       XKAN_CFG_RX_DSIZE_F0_DSIZE(2)                                          /*!< 16 */
#define XKAN_CFG_RX_DSIZE_F0_DSIZE_20                       XKAN_CFG_RX_DSIZE_F0_DSIZE(3)                                          /*!< 20 */
#define XKAN_CFG_RX_DSIZE_F0_DSIZE_24                       XKAN_CFG_RX_DSIZE_F0_DSIZE(4)                                          /*!< 24 */
#define XKAN_CFG_RX_DSIZE_F0_DSIZE_32                       XKAN_CFG_RX_DSIZE_F0_DSIZE(5)                                          /*!< 32 */
#define XKAN_CFG_RX_DSIZE_F0_DSIZE_48                       XKAN_CFG_RX_DSIZE_F0_DSIZE(6)                                          /*!< 48 */
#define XKAN_CFG_RX_DSIZE_F0_DSIZE_64                       XKAN_CFG_RX_DSIZE_F0_DSIZE(7)                                          /*!< 64 */

/**
  * \brief Check the XKAN cfg_rx_dsize f0_dsize bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid f0_dsize bits.
  * \retval 1 This is a valid f0_dsize bits.
  */
#define IS_XKAN_CFG_RX_DSIZE_F0_DSIZE(regval)               (\
                                         ((regval) == XKAN_CFG_RX_DSIZE_F0_DSIZE_8                   ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F0_DSIZE_12                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F0_DSIZE_16                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F0_DSIZE_20                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F0_DSIZE_24                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F0_DSIZE_32                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F0_DSIZE_48                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F0_DSIZE_64                  )  \
                                                 )

#define XKAN_CFG_RX_DSIZE_F1_DSIZE_MASK                BITS(4,6)                                   /*!< XKAN CFG RX DSIZE: F1_DSIZE Bit Mask */  
#define XKAN_CFG_RX_DSIZE_F1_DSIZE_OFS                 4U                                          /*!< XKAN CFG RX DSIZE: F1_DSIZE Bit Offset */
#define XKAN_CFG_RX_DSIZE_F1_DSIZE(regval)             (BITS(4,6) & ((uint32_t)(regval) << 4))        /*!< XKAN CFG RX DSIZE: F1_DSIZE Bit Value */  
#define XKAN_CFG_RX_DSIZE_F1_DSIZE_8                        XKAN_CFG_RX_DSIZE_F1_DSIZE(0)                                          /*!< 8 */
#define XKAN_CFG_RX_DSIZE_F1_DSIZE_12                       XKAN_CFG_RX_DSIZE_F1_DSIZE(1)                                          /*!< 12 */
#define XKAN_CFG_RX_DSIZE_F1_DSIZE_16                       XKAN_CFG_RX_DSIZE_F1_DSIZE(2)                                          /*!< 16 */
#define XKAN_CFG_RX_DSIZE_F1_DSIZE_20                       XKAN_CFG_RX_DSIZE_F1_DSIZE(3)                                          /*!< 20 */
#define XKAN_CFG_RX_DSIZE_F1_DSIZE_24                       XKAN_CFG_RX_DSIZE_F1_DSIZE(4)                                          /*!< 24 */
#define XKAN_CFG_RX_DSIZE_F1_DSIZE_32                       XKAN_CFG_RX_DSIZE_F1_DSIZE(5)                                          /*!< 32 */
#define XKAN_CFG_RX_DSIZE_F1_DSIZE_48                       XKAN_CFG_RX_DSIZE_F1_DSIZE(6)                                          /*!< 48 */
#define XKAN_CFG_RX_DSIZE_F1_DSIZE_64                       XKAN_CFG_RX_DSIZE_F1_DSIZE(7)                                          /*!< 64 */

/**
  * \brief Check the XKAN cfg_rx_dsize f1_dsize bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid f1_dsize bits.
  * \retval 1 This is a valid f1_dsize bits.
  */
#define IS_XKAN_CFG_RX_DSIZE_F1_DSIZE(regval)               (\
                                         ((regval) == XKAN_CFG_RX_DSIZE_F1_DSIZE_8                   ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F1_DSIZE_12                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F1_DSIZE_16                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F1_DSIZE_20                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F1_DSIZE_24                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F1_DSIZE_32                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F1_DSIZE_48                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_F1_DSIZE_64                  )  \
                                                 )

#define XKAN_CFG_RX_DSIZE_RB_DSIZE_MASK                BITS(8,10)                                   /*!< XKAN CFG RX DSIZE: RB_DSIZE Bit Mask */  
#define XKAN_CFG_RX_DSIZE_RB_DSIZE_OFS                 8U                                          /*!< XKAN CFG RX DSIZE: RB_DSIZE Bit Offset */
#define XKAN_CFG_RX_DSIZE_RB_DSIZE(regval)             (BITS(8,10) & ((uint32_t)(regval) << 8))        /*!< XKAN CFG RX DSIZE: RB_DSIZE Bit Value */  
#define XKAN_CFG_RX_DSIZE_RB_DSIZE_8                        XKAN_CFG_RX_DSIZE_RB_DSIZE(0)                                          /*!< 8 */
#define XKAN_CFG_RX_DSIZE_RB_DSIZE_12                       XKAN_CFG_RX_DSIZE_RB_DSIZE(1)                                          /*!< 12 */
#define XKAN_CFG_RX_DSIZE_RB_DSIZE_16                       XKAN_CFG_RX_DSIZE_RB_DSIZE(2)                                          /*!< 16 */
#define XKAN_CFG_RX_DSIZE_RB_DSIZE_20                       XKAN_CFG_RX_DSIZE_RB_DSIZE(3)                                          /*!< 20 */
#define XKAN_CFG_RX_DSIZE_RB_DSIZE_24                       XKAN_CFG_RX_DSIZE_RB_DSIZE(4)                                          /*!< 24 */
#define XKAN_CFG_RX_DSIZE_RB_DSIZE_32                       XKAN_CFG_RX_DSIZE_RB_DSIZE(5)                                          /*!< 32 */
#define XKAN_CFG_RX_DSIZE_RB_DSIZE_48                       XKAN_CFG_RX_DSIZE_RB_DSIZE(6)                                          /*!< 48 */
#define XKAN_CFG_RX_DSIZE_RB_DSIZE_64                       XKAN_CFG_RX_DSIZE_RB_DSIZE(7)                                          /*!< 64 */

/**
  * \brief Check the XKAN cfg_rx_dsize rb_dsize bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid rb_dsize bits.
  * \retval 1 This is a valid rb_dsize bits.
  */
#define IS_XKAN_CFG_RX_DSIZE_RB_DSIZE(regval)               (\
                                         ((regval) == XKAN_CFG_RX_DSIZE_RB_DSIZE_8                   ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_RB_DSIZE_12                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_RB_DSIZE_16                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_RB_DSIZE_20                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_RB_DSIZE_24                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_RB_DSIZE_32                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_RB_DSIZE_48                  ) || \
                                         ((regval) == XKAN_CFG_RX_DSIZE_RB_DSIZE_64                  )  \
                                                 )

 /* ===== XKAN CFG_TXBUF Register definition ===== */
#define XKAN_CFG_TXBUF_TXBUF_ADDR_MASK              BITS(0,10)                                   /*!< XKAN CFG TXBUF: TXBUF_ADDR Bit Mask */  
#define XKAN_CFG_TXBUF_TXBUF_ADDR_OFS               0U                                          /*!< XKAN CFG TXBUF: TXBUF_ADDR Bit Offset */
#define XKAN_CFG_TXBUF_TXBUF_ADDR(regval)           (BITS(0,10) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG TXBUF: TXBUF_ADDR Bit Value */  
#define XKAN_CFG_TXBUF_TXBUF_NUM_MASK               BITS(16,21)                                   /*!< XKAN CFG TXBUF: TXBUF_NUM Bit Mask */  
#define XKAN_CFG_TXBUF_TXBUF_NUM_OFS                16U                                          /*!< XKAN CFG TXBUF: TXBUF_NUM Bit Offset */
#define XKAN_CFG_TXBUF_TXBUF_NUM(regval)            (BITS(16,21) & ((uint32_t)(regval) << 16))        /*!< XKAN CFG TXBUF: TXBUF_NUM Bit Value */  
#define XKAN_CFG_TXBUF_TXFIFO_NUM_MASK              BITS(24,29)                                   /*!< XKAN CFG TXBUF: TXFIFO_NUM Bit Mask */  
#define XKAN_CFG_TXBUF_TXFIFO_NUM_OFS               24U                                          /*!< XKAN CFG TXBUF: TXFIFO_NUM Bit Offset */
#define XKAN_CFG_TXBUF_TXFIFO_NUM(regval)           (BITS(24,29) & ((uint32_t)(regval) << 24))        /*!< XKAN CFG TXBUF: TXFIFO_NUM Bit Value */  
#define XKAN_CFG_TXBUF_TXFIFO_CLR                   BIT(30)                                      /*!< Tx FIFO Clear When soft-ware write "1" to this bit, The status of Tx FIFO are all return to default value. */
#define XKAN_CFG_TXBUF_TXFIFO_CLR_OFS               30U                                          /*!< XKAN CFG TXBUF: TXFIFO_CLR Bit Offset */
#define XKAN_CFG_TXBUF_TXFIFO_CLR_VAL(regval)           (BIT(30) & ((uint32_t)(regval) << 30))        /*!< XKAN CFG TXBUF: TXFIFO_CLR Bit Value */  
 
 /* ===== XKAN CFG_TXFIFO_STATUS Register definition ===== */
#define XKAN_CFG_TXFIFO_STATUS_TXFF_ENTRY_NUM               BITS(0,5)                
#define XKAN_CFG_TXFIFO_STATUS_TXFF_RPTR                    BITS(8,12)                
#define XKAN_CFG_TXFIFO_STATUS_TXFF_WPTR                    BITS(16,20)                
#define XKAN_CFG_TXFIFO_STATUS_TXFF_FULL                    BIT(21)                                      /*!< Tx FIFO Full 0 Tx FIFO not full 1 Tx FIFO full */
 
 /* ===== XKAN CFG_TX_DSIZE Register definition ===== */
#define XKAN_CFG_TX_DSIZE_TXB_DSIZE_MASK               BITS(0,2)                                   /*!< XKAN CFG TX DSIZE: TXB_DSIZE Bit Mask */  
#define XKAN_CFG_TX_DSIZE_TXB_DSIZE_OFS                0U                                          /*!< XKAN CFG TX DSIZE: TXB_DSIZE Bit Offset */
#define XKAN_CFG_TX_DSIZE_TXB_DSIZE(regval)            (BITS(0,2) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG TX DSIZE: TXB_DSIZE Bit Value */  
#define XKAN_CFG_TX_DSIZE_TXB_DSIZE_8                       XKAN_CFG_TX_DSIZE_TXB_DSIZE(0)                                          /*!< 8 */
#define XKAN_CFG_TX_DSIZE_TXB_DSIZE_12                      XKAN_CFG_TX_DSIZE_TXB_DSIZE(1)                                          /*!< 12 */
#define XKAN_CFG_TX_DSIZE_TXB_DSIZE_16                      XKAN_CFG_TX_DSIZE_TXB_DSIZE(2)                                          /*!< 16 */
#define XKAN_CFG_TX_DSIZE_TXB_DSIZE_20                      XKAN_CFG_TX_DSIZE_TXB_DSIZE(3)                                          /*!< 20 */
#define XKAN_CFG_TX_DSIZE_TXB_DSIZE_24                      XKAN_CFG_TX_DSIZE_TXB_DSIZE(4)                                          /*!< 24 */
#define XKAN_CFG_TX_DSIZE_TXB_DSIZE_32                      XKAN_CFG_TX_DSIZE_TXB_DSIZE(5)                                          /*!< 32 */
#define XKAN_CFG_TX_DSIZE_TXB_DSIZE_48                      XKAN_CFG_TX_DSIZE_TXB_DSIZE(6)                                          /*!< 48 */
#define XKAN_CFG_TX_DSIZE_TXB_DSIZE_64                      XKAN_CFG_TX_DSIZE_TXB_DSIZE(7)                                          /*!< 64 */

/**
  * \brief Check the XKAN cfg_tx_dsize txb_dsize bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid txb_dsize bits.
  * \retval 1 This is a valid txb_dsize bits.
  */
#define IS_XKAN_CFG_TX_DSIZE_TXB_DSIZE(regval)              (\
                                        ((regval) == XKAN_CFG_TX_DSIZE_TXB_DSIZE_8                   ) || \
                                        ((regval) == XKAN_CFG_TX_DSIZE_TXB_DSIZE_12                  ) || \
                                        ((regval) == XKAN_CFG_TX_DSIZE_TXB_DSIZE_16                  ) || \
                                        ((regval) == XKAN_CFG_TX_DSIZE_TXB_DSIZE_20                  ) || \
                                        ((regval) == XKAN_CFG_TX_DSIZE_TXB_DSIZE_24                  ) || \
                                        ((regval) == XKAN_CFG_TX_DSIZE_TXB_DSIZE_32                  ) || \
                                        ((regval) == XKAN_CFG_TX_DSIZE_TXB_DSIZE_48                  ) || \
                                        ((regval) == XKAN_CFG_TX_DSIZE_TXB_DSIZE_64                  )  \
                                                 )

 /* ===== XKAN CFG_TXBAR Register definition ===== */
#define XKAN_CFG_TXBAR_TX_REQ_MASK                  BITS(0,31)                                   /*!< XKAN CFG TXBAR: TX_REQ Bit Mask */  
#define XKAN_CFG_TXBAR_TX_REQ_OFS                   0U                                          /*!< XKAN CFG TXBAR: TX_REQ Bit Offset */
#define XKAN_CFG_TXBAR_TX_REQ(regval)               (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG TXBAR: TX_REQ Bit Value */  
 
 /* ===== XKAN CFG_TXABORT_REQ Register definition ===== */
#define XKAN_CFG_TXABORT_REQ_REQ_MASK                     BITS(0,31)                                   /*!< XKAN CFG TXABORT REQ: REQ Bit Mask */  
#define XKAN_CFG_TXABORT_REQ_REQ_OFS                      0U                                          /*!< XKAN CFG TXABORT REQ: REQ Bit Offset */
#define XKAN_CFG_TXABORT_REQ_REQ(regval)                  (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG TXABORT REQ: REQ Bit Value */  
 
 /* ===== XKAN CFG_TXABORT_FINISH Register definition ===== */
#define XKAN_CFG_TXABORT_FINISH_FINISH_MASK                  BITS(0,31)                                   /*!< XKAN CFG TXABORT FINISH: FINISH Bit Mask */  
#define XKAN_CFG_TXABORT_FINISH_FINISH_OFS                   0U                                          /*!< XKAN CFG TXABORT FINISH: FINISH Bit Offset */
#define XKAN_CFG_TXABORT_FINISH_FINISH(regval)               (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG TXABORT FINISH: FINISH Bit Value */  
 
 /* ===== XKAN CFG_TXREQ_PENDING Register definition ===== */
#define XKAN_CFG_TXREQ_PENDING_REQ_PENDING_MASK             BITS(0,31)                                   /*!< XKAN CFG TXREQ PENDING: REQ_PENDING Bit Mask */  
#define XKAN_CFG_TXREQ_PENDING_REQ_PENDING_OFS              0U                                          /*!< XKAN CFG TXREQ PENDING: REQ_PENDING Bit Offset */
#define XKAN_CFG_TXREQ_PENDING_REQ_PENDING(regval)          (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG TXREQ PENDING: REQ_PENDING Bit Value */  
 
 /* ===== XKAN CFG_TXB_TRANS Register definition ===== */
#define XKAN_CFG_TXB_TRANS_TX_COMPLETE_MASK             BITS(0,31)                                   /*!< XKAN CFG TXB TRANS: TX_COMPLETE Bit Mask */  
#define XKAN_CFG_TXB_TRANS_TX_COMPLETE_OFS              0U                                          /*!< XKAN CFG TXB TRANS: TX_COMPLETE Bit Offset */
#define XKAN_CFG_TXB_TRANS_TX_COMPLETE(regval)          (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG TXB TRANS: TX_COMPLETE Bit Value */  
 
 /* ===== XKAN CFG_TIME_STAMP Register definition ===== */
#define XKAN_CFG_TIME_STAMP_VALUE_MASK                   BITS(0,15)                                   /*!< XKAN CFG TIME STAMP: VALUE Bit Mask */  
#define XKAN_CFG_TIME_STAMP_VALUE_OFS                    0U                                          /*!< XKAN CFG TIME STAMP: VALUE Bit Offset */
#define XKAN_CFG_TIME_STAMP_VALUE(regval)                (BITS(0,15) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG TIME STAMP: VALUE Bit Value */  
 
 /* ===== XKAN CFG_TIME_DIV Register definition ===== */
#define XKAN_CFG_TIME_DIV_DIV_NUM_MASK                 BITS(0,3)                                   /*!< XKAN CFG TIME DIV: DIV_NUM Bit Mask */  
#define XKAN_CFG_TIME_DIV_DIV_NUM_OFS                  0U                                          /*!< XKAN CFG TIME DIV: DIV_NUM Bit Offset */
#define XKAN_CFG_TIME_DIV_DIV_NUM(regval)              (BITS(0,3) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG TIME DIV: DIV_NUM Bit Value */  
 
 /* ===== XKAN CFG_TRANS_DELAY_COMP Register definition ===== */
#define XKAN_CFG_TRANS_DELAY_COMP_OFFSET_VALUE_MASK            BITS(0,6)                                   /*!< XKAN CFG TRANS DELAY COMP: OFFSET_VALUE Bit Mask */  
#define XKAN_CFG_TRANS_DELAY_COMP_OFFSET_VALUE_OFS             0U                                          /*!< XKAN CFG TRANS DELAY COMP: OFFSET_VALUE Bit Offset */
#define XKAN_CFG_TRANS_DELAY_COMP_OFFSET_VALUE(regval)         (BITS(0,6) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG TRANS DELAY COMP: OFFSET_VALUE Bit Value */  
 
 /* ===== XKAN CFG_ESR Register definition ===== */
#define XKAN_CFG_ESR_TEC                          BITS(0,7)                
#define XKAN_CFG_ESR_REC                          BITS(8,15)                
#define XKAN_CFG_ESR_ERR_ACTIVE_FLAG              BIT(16)                                      /*!< Error active flag This bit is set by hardware when the XKAN is in the error active state. */
#define XKAN_CFG_ESR_ERR_PASS_FLAG                BIT(17)                                      /*!< Error passive flag This bit is set by hardware when the XKAN is in the error passive state. */
#define XKAN_CFG_ESR_BUS_OFF                      BIT(18)                                      /*!< Bus-off flag This bit is set by hardware when it enters the bus-off state. The bus-off state is entered on TEC is greater than 255. */
#define XKAN_CFG_ESR_LEC_MASK                     BITS(19,21)                                   /*!< XKAN CFG ESR: LEC Bit Mask */  
#define XKAN_CFG_ESR_LEC_OFS                      19U                                          /*!< XKAN CFG ESR: LEC Bit Offset */
#define XKAN_CFG_ESR_LEC(regval)                  (BITS(19,21) & ((uint32_t)(regval) << 19))        /*!< XKAN CFG ESR: LEC Bit Value */  

/**
  * \brief Check the XKAN cfg_esr lec bits parameters.
  * \param regval bits value to be checked.
  * \retval 0 This is not a valid lec bits.
  * \retval 1 This is a valid lec bits.
  */
#define IS_XKAN_CFG_ESR_LEC(regval)                    (\
                                                 )

 /* ===== XKAN CFG_TRANS_ERR Register definition ===== */
#define XKAN_CFG_TRANS_ERR_TX_NUM_MASK                  BITS(0,31)                                   /*!< XKAN CFG TRANS ERR: TX_NUM Bit Mask */  
#define XKAN_CFG_TRANS_ERR_TX_NUM_OFS                   0U                                          /*!< XKAN CFG TRANS ERR: TX_NUM Bit Offset */
#define XKAN_CFG_TRANS_ERR_TX_NUM(regval)               (BITS(0,31) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG TRANS ERR: TX_NUM Bit Value */  
 
 /* ===== XKAN CFG_INT_EN Register definition ===== */
#define XKAN_CFG_INT_EN_RXB_UPD_EN                   BIT(0)                                      /*!< Rx buffer update interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_RXB_UPD_EN_OFS               0U                                          /*!< XKAN CFG INT EN: RXB_UPD_EN Bit Offset */
#define XKAN_CFG_INT_EN_RXB_UPD_EN_VAL(regval)           (BIT(0) & ((uint32_t)(regval) << 0))        /*!< XKAN CFG INT EN: RXB_UPD_EN Bit Value */  
#define XKAN_CFG_INT_EN_RXFF_OF_EN                   BIT(1)                                      /*!< Rx cdc fifo over flow interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_RXFF_OF_EN_OFS               1U                                          /*!< XKAN CFG INT EN: RXFF_OF_EN Bit Offset */
#define XKAN_CFG_INT_EN_RXFF_OF_EN_VAL(regval)           (BIT(1) & ((uint32_t)(regval) << 1))        /*!< XKAN CFG INT EN: RXFF_OF_EN Bit Value */  
#define XKAN_CFG_INT_EN_TXFF_UF_EN                   BIT(2)                                      /*!< Tx cdc fifo under flow interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_TXFF_UF_EN_OFS               2U                                          /*!< XKAN CFG INT EN: TXFF_UF_EN Bit Offset */
#define XKAN_CFG_INT_EN_TXFF_UF_EN_VAL(regval)           (BIT(2) & ((uint32_t)(regval) << 2))        /*!< XKAN CFG INT EN: TXFF_UF_EN Bit Value */  
#define XKAN_CFG_INT_EN_TIME_STAMP_WRAP_EN           BIT(3)                                      /*!< Time stamp counter wrap interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_TIME_STAMP_WRAP_EN_OFS       3U                                          /*!< XKAN CFG INT EN: TIME_STAMP_WRAP_EN Bit Offset */
#define XKAN_CFG_INT_EN_TIME_STAMP_WRAP_EN_VAL(regval)   (BIT(3) & ((uint32_t)(regval) << 3))        /*!< XKAN CFG INT EN: TIME_STAMP_WRAP_EN Bit Value */  
#define XKAN_CFG_INT_EN_ERROR_DET_INT_EN             BIT(4)                                      /*!< Error detection interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_ERROR_DET_INT_EN_OFS         4U                                          /*!< XKAN CFG INT EN: ERROR_DET_INT_EN Bit Offset */
#define XKAN_CFG_INT_EN_ERROR_DET_INT_EN_VAL(regval)     (BIT(4) & ((uint32_t)(regval) << 4))        /*!< XKAN CFG INT EN: ERROR_DET_INT_EN Bit Value */  
#define XKAN_CFG_INT_EN_BUS_OFF_INT_EN               BIT(5)                                      /*!< Bus-off interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_BUS_OFF_INT_EN_OFS           5U                                          /*!< XKAN CFG INT EN: BUS_OFF_INT_EN Bit Offset */
#define XKAN_CFG_INT_EN_BUS_OFF_INT_EN_VAL(regval)       (BIT(5) & ((uint32_t)(regval) << 5))        /*!< XKAN CFG INT EN: BUS_OFF_INT_EN Bit Value */  
#define XKAN_CFG_INT_EN_ARBITRATION_LOST_INT_EN         BIT(6)                                      /*!< Arbitration lost interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_ARBITRATION_LOST_INT_EN_OFS     6U                                          /*!< XKAN CFG INT EN: ARBITRATION_LOST_INT_EN Bit Offset */
#define XKAN_CFG_INT_EN_ARBITRATION_LOST_INT_EN_VAL(regval) (BIT(6) & ((uint32_t)(regval) << 6))        /*!< XKAN CFG INT EN: ARBITRATION_LOST_INT_EN Bit Value */  
#define XKAN_CFG_INT_EN_TX_TRANS_COMPLETE_EN         BIT(7)                                      /*!< Tx trans complete interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_TX_TRANS_COMPLETE_EN_OFS     7U                                          /*!< XKAN CFG INT EN: TX_TRANS_COMPLETE_EN Bit Offset */
#define XKAN_CFG_INT_EN_TX_TRANS_COMPLETE_EN_VAL(regval) (BIT(7) & ((uint32_t)(regval) << 7))        /*!< XKAN CFG INT EN: TX_TRANS_COMPLETE_EN Bit Value */  
#define XKAN_CFG_INT_EN_RXF0_FILL_EN                 BIT(15)                                      /*!< New message is write into rxfifo0 interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_RXF0_FILL_EN_OFS             15U                                          /*!< XKAN CFG INT EN: RXF0_FILL_EN Bit Offset */
#define XKAN_CFG_INT_EN_RXF0_FILL_EN_VAL(regval)         (BIT(15) & ((uint32_t)(regval) << 15))        /*!< XKAN CFG INT EN: RXF0_FILL_EN Bit Value */  
#define XKAN_CFG_INT_EN_RXF0_WM_EN                   BIT(16)                                      /*!< Rx FIFO 0 watermark reached interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_RXF0_WM_EN_OFS               16U                                          /*!< XKAN CFG INT EN: RXF0_WM_EN Bit Offset */
#define XKAN_CFG_INT_EN_RXF0_WM_EN_VAL(regval)           (BIT(16) & ((uint32_t)(regval) << 16))        /*!< XKAN CFG INT EN: RXF0_WM_EN Bit Value */  
#define XKAN_CFG_INT_EN_RXF0_FULL_EN                 BIT(17)                                      /*!< Rx FIFO 0 full interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_RXF0_FULL_EN_OFS             17U                                          /*!< XKAN CFG INT EN: RXF0_FULL_EN Bit Offset */
#define XKAN_CFG_INT_EN_RXF0_FULL_EN_VAL(regval)         (BIT(17) & ((uint32_t)(regval) << 17))        /*!< XKAN CFG INT EN: RXF0_FULL_EN Bit Value */  
#define XKAN_CFG_INT_EN_RXF0_LOST_EN                 BIT(18)                                      /*!< Rx FIFO 0 message lost interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_RXF0_LOST_EN_OFS             18U                                          /*!< XKAN CFG INT EN: RXF0_LOST_EN Bit Offset */
#define XKAN_CFG_INT_EN_RXF0_LOST_EN_VAL(regval)         (BIT(18) & ((uint32_t)(regval) << 18))        /*!< XKAN CFG INT EN: RXF0_LOST_EN Bit Value */  
#define XKAN_CFG_INT_EN_RXF1_FILL_EN                 BIT(19)                                      /*!< Rx FIFO 1 new message interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_RXF1_FILL_EN_OFS             19U                                          /*!< XKAN CFG INT EN: RXF1_FILL_EN Bit Offset */
#define XKAN_CFG_INT_EN_RXF1_FILL_EN_VAL(regval)         (BIT(19) & ((uint32_t)(regval) << 19))        /*!< XKAN CFG INT EN: RXF1_FILL_EN Bit Value */  
#define XKAN_CFG_INT_EN_RXF1_WM_EN                   BIT(20)                                      /*!< Rx FIFO 1 watermark reached interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_RXF1_WM_EN_OFS               20U                                          /*!< XKAN CFG INT EN: RXF1_WM_EN Bit Offset */
#define XKAN_CFG_INT_EN_RXF1_WM_EN_VAL(regval)           (BIT(20) & ((uint32_t)(regval) << 20))        /*!< XKAN CFG INT EN: RXF1_WM_EN Bit Value */  
#define XKAN_CFG_INT_EN_RXF1_FULL_EN                 BIT(21)                                      /*!< Rx FIFO 1 full interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_RXF1_FULL_EN_OFS             21U                                          /*!< XKAN CFG INT EN: RXF1_FULL_EN Bit Offset */
#define XKAN_CFG_INT_EN_RXF1_FULL_EN_VAL(regval)         (BIT(21) & ((uint32_t)(regval) << 21))        /*!< XKAN CFG INT EN: RXF1_FULL_EN Bit Value */  
#define XKAN_CFG_INT_EN_RXF1_LOST_EN                 BIT(22)                                      /*!< Rx FIFO 1 message lost interrupt enable 0: Interrupt disabled 1: Interrupt enabled */
#define XKAN_CFG_INT_EN_RXF1_LOST_EN_OFS             22U                                          /*!< XKAN CFG INT EN: RXF1_LOST_EN Bit Offset */
#define XKAN_CFG_INT_EN_RXF1_LOST_EN_VAL(regval)         (BIT(22) & ((uint32_t)(regval) << 22))        /*!< XKAN CFG INT EN: RXF1_LOST_EN Bit Value */  
 
 /* ===== XKAN CFG_INT_STATUS Register definition ===== */
#define XKAN_CFG_INT_STATUS_RXB_UPD_INT                  BIT(0)                                      /*!< Rx buffer update interrupt status 0: Interrupt not occurred 1: Interrupt occurred */
#define XKAN_CFG_INT_STATUS_RXFF_OF_INT                  BIT(1)                                      /*!< Rx cdc fifo over flow interrupt status, This bit  is set by rx cdc fifo overflow and software can write "1" to clear this bit. 0: Interrupt not occurred 1: Interrupt occurred Note: The frequency configuration of two clk (icb_clk and can_clk) is illegal, for example: the frequency of icb_clk is more slower than can_clk. */
#define XKAN_CFG_INT_STATUS_RXFF_OF_INT_OFS              1U                                          /*!< XKAN CFG INT STATUS: RXFF_OF_INT Bit Offset */
#define XKAN_CFG_INT_STATUS_RXFF_OF_INT_VAL(regval)          (BIT(1) & ((uint32_t)(regval) << 1))        /*!< XKAN CFG INT STATUS: RXFF_OF_INT Bit Value */  
#define XKAN_CFG_INT_STATUS_TXFF_UF_INT                  BIT(2)                                      /*!< Tx cdc fifo under flow interrupt status, This bit  is set by tx cdc fifo underflow and software can write "1" to clear this bit. 0: Interrupt not occurred 1: Interrupt occurred Note: The frequency configuration of two clk (icb_clk and can_clk) is illegal, for example: the frequency of icb_clk is more slower than can_clk. */
#define XKAN_CFG_INT_STATUS_TXFF_UF_INT_OFS              2U                                          /*!< XKAN CFG INT STATUS: TXFF_UF_INT Bit Offset */
#define XKAN_CFG_INT_STATUS_TXFF_UF_INT_VAL(regval)          (BIT(2) & ((uint32_t)(regval) << 2))        /*!< XKAN CFG INT STATUS: TXFF_UF_INT Bit Value */  
#define XKAN_CFG_INT_STATUS_TIME_STAMP_WRAP_INT          BIT(3)                                      /*!< Time stamp counter wrap interrupt status, This bit is set by time stamp counter is wrapper around and  software can write "1" to clear this bit. 0: Interrupt not occurred 1: Interrupt occurred */
#define XKAN_CFG_INT_STATUS_TIME_STAMP_WRAP_INT_OFS      3U                                          /*!< XKAN CFG INT STATUS: TIME_STAMP_WRAP_INT Bit Offset */
#define XKAN_CFG_INT_STATUS_TIME_STAMP_WRAP_INT_VAL(regval)  (BIT(3) & ((uint32_t)(regval) << 3))        /*!< XKAN CFG INT STATUS: TIME_STAMP_WRAP_INT Bit Value */  
#define XKAN_CFG_INT_STATUS_ERROR_DET_INT                BIT(4)                                      /*!< Error detection interrupt status 0: Interrupt not occurred 1: Interrupt occurred */
#define XKAN_CFG_INT_STATUS_ERROR_DET_INT_OFS            4U                                          /*!< XKAN CFG INT STATUS: ERROR_DET_INT Bit Offset */
#define XKAN_CFG_INT_STATUS_ERROR_DET_INT_VAL(regval)        (BIT(4) & ((uint32_t)(regval) << 4))        /*!< XKAN CFG INT STATUS: ERROR_DET_INT Bit Value */  
#define XKAN_CFG_INT_STATUS_BUS_OFF_INT                  BIT(5)                                      /*!< Bus-off interrupt status 0: Interrupt not occurred 1: Interrupt occurred */
#define XKAN_CFG_INT_STATUS_BUS_OFF_INT_OFS              5U                                          /*!< XKAN CFG INT STATUS: BUS_OFF_INT Bit Offset */
#define XKAN_CFG_INT_STATUS_BUS_OFF_INT_VAL(regval)          (BIT(5) & ((uint32_t)(regval) << 5))        /*!< XKAN CFG INT STATUS: BUS_OFF_INT Bit Value */  
#define XKAN_CFG_INT_STATUS_ARBITRATION_LOST_INT         BIT(6)                                      /*!< Arbitration lost interrupt status 0: Interrupt not occurred 1: Interrupt occurred */
#define XKAN_CFG_INT_STATUS_ARBITRATION_LOST_INT_OFS     6U                                          /*!< XKAN CFG INT STATUS: ARBITRATION_LOST_INT Bit Offset */
#define XKAN_CFG_INT_STATUS_ARBITRATION_LOST_INT_VAL(regval) (BIT(6) & ((uint32_t)(regval) << 6))        /*!< XKAN CFG INT STATUS: ARBITRATION_LOST_INT Bit Value */  
#define XKAN_CFG_INT_STATUS_TX_TRANS_COMPLETE_INT         BIT(7)                                      /*!< Tx trans complete interrupt status 0: Interrupt not occurred 1: Interrupt occurred */
#define XKAN_CFG_INT_STATUS_RXF0_FILL_INT                BIT(15)                                      /*!< Rx FIFO 0 new message interrupt status 0: Interrupt not occurred 1: Interrupt occurred */
#define XKAN_CFG_INT_STATUS_RXF0_FILL_INT_OFS            15U                                          /*!< XKAN CFG INT STATUS: RXF0_FILL_INT Bit Offset */
#define XKAN_CFG_INT_STATUS_RXF0_FILL_INT_VAL(regval)        (BIT(15) & ((uint32_t)(regval) << 15))        /*!< XKAN CFG INT STATUS: RXF0_FILL_INT Bit Value */  
#define XKAN_CFG_INT_STATUS_RXF0_WM_INT                  BIT(16)                                      /*!< Rx FIFO 0 watermark interrupt status 0: Interrupt not occurred 1: Interrupt occurred (The Number of elements stored in RX FIFO 0 is bigger than RX FIFO 0's watermark configuration). */
#define XKAN_CFG_INT_STATUS_RXF0_FULL_INT                BIT(17)                                      /*!< Rx FIFO 0 full interrupt status 0: Interrupt not occurred 1: Interrupt occurred Note: This interrupt can be cleared by software read rxfifo entry only. */
#define XKAN_CFG_INT_STATUS_RXF0_LOST_INT                BIT(18)                                      /*!< Rx FIFO 0 message lost interrupt status 0: Interrupt not occurred 1: Interrupt occurred */
#define XKAN_CFG_INT_STATUS_RXF1_FILL_INT                BIT(19)                                      /*!< Rx FIFO 1 new message interrupt status 0: Interrupt not occurred 1: Interrupt occurred */
#define XKAN_CFG_INT_STATUS_RXF1_FILL_INT_OFS            19U                                          /*!< XKAN CFG INT STATUS: RXF1_FILL_INT Bit Offset */
#define XKAN_CFG_INT_STATUS_RXF1_FILL_INT_VAL(regval)        (BIT(19) & ((uint32_t)(regval) << 19))        /*!< XKAN CFG INT STATUS: RXF1_FILL_INT Bit Value */  
#define XKAN_CFG_INT_STATUS_RXF1_WM_INT                  BIT(20)                                      /*!< Rx FIFO 1 watermark interrupt status 0: Interrupt not occurred 1: Interrupt occurred (The Number of elements stored in RX FIFO 1 is bigger than RX FIFO 1's watermark configuration). */
#define XKAN_CFG_INT_STATUS_RXF1_FULL_INT                BIT(21)                                      /*!< Rx FIFO 1 full interrupt status 0: Interrupt not occurred 1: Interrupt occurred Note: This interrupt can be cleared by software read rxfifo entry only. */
#define XKAN_CFG_INT_STATUS_RXF1_LOST_INT                BIT(22)                                      /*!< Rx FIFO 1 message lost interrupt status 0: Interrupt not occurred 1: Interrupt occurred */

/* ------------------------------------------- Configuration functions ------------------------------------------- */
void xKan_StructInit(xKan_InitTypeDef *xkan_parameter_init);
ErrStatus xKan_Init(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init);
void xKan_ConfigGlobalFilter(XKAN_TypeDef *xKanx, uint32_t NonMatchingStd, uint32_t NonMatchingExt, uint32_t RejectRemoteStd, uint32_t RejectRemoteExt);
ErrStatus xKan_ConfigFilter(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init, xKan_FilterTypeDef* sFilterConfig, ramtype_t xkanRamAddr);

ErrStatus xKan_AddMessageToTxFifo(xKan_InitTypeDef *xkan_parameter_init, XKAN_TypeDef *xKanx, xKan_TxHeaderTypeDef *pxKan_TxHeader, uint8_t* pTxData, ramtype_t xkanRamAddr);
ErrStatus XKAN_AddMessageToTxBuffer(xKan_InitTypeDef *xkan_parameter_init, XKAN_TypeDef *xKanx, xKan_TxHeaderTypeDef *pTxHeader, uint8_t *pTxData, uint32_t BufferIndex, ramtype_t xkanRamAddr);
ErrStatus xKan_GetRxMessage(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init, uint32_t RxLocation, xKan_RxHeaderTypeDef *pRxHeader, uint8_t *pRxData, ramtype_t xkanRamAddr);
ErrStatus xKan_IsNewMessageRecv(XKAN_TypeDef *xKanx);
void xKan_ConfigRxFifoOperationMode(XKAN_TypeDef *xKanx, uint32_t FIFO, uint32_t operation_mode);
void xKan_ConfigISOMode(XKAN_TypeDef *xKanx, ControlStatus ena);
void xKan_ConfigRxFifoWatermark(XKAN_TypeDef *xKanx, uint32_t FIFO, uint32_t watermark);
ErrStatus xKan_EnterInit(XKAN_TypeDef *xKanx);
void xKan_ConfigTdcOffset(XKAN_TypeDef *xKanx, uint8_t offset);
/* ------------------------------------------- Control functions ------------------------------------------- */
ErrStatus xKan_Start(XKAN_TypeDef *xKanx);
ErrStatus xKan_Sleep(XKAN_TypeDef *xKanx, ControlStatus ena);
ErrStatus xKan_AbortTxRequest(XKAN_TypeDef *xKanx, uint32_t BufferIndex);
ErrStatus xKan_ClkStop(XKAN_TypeDef *xKanx, ControlStatus ena);
ErrStatus xKan_GetClkStopStatus(XKAN_TypeDef *xKanx, uint32_t timeout);
void xKan_EnableKernel(XKAN_TypeDef *xKanx, ControlStatus ena);
void xKan_ClearClkStopStatus(XKAN_TypeDef *xKanx);
void xKan_EnableTxBufferRequest(XKAN_TypeDef *xKanx, uint32_t BufferIndex);
void xKan_EnableInterrupt(XKAN_TypeDef *xKanx, uint32_t interrupt, ControlStatus status);
FlagStatus xKan_GetInterruptEnable(XKAN_TypeDef *xKanx, uint32_t interrupt);
FlagStatus xKan_GetInterruptStatus(XKAN_TypeDef *xKanx, uint32_t status);
void xKan_ClearInterruptStatus(XKAN_TypeDef *xKanx, uint32_t status);
FlagStatus xKan_GetMsrStatus(XKAN_TypeDef *xKanx, uint32_t status);
uint16_t xKan_GetTimeStamp(XKAN_TypeDef *xKanx);
void xKan_ClearTxComplete(XKAN_TypeDef *xKanx);
void xKan_ClearTxReqPending(XKAN_TypeDef *xKanx, uint32_t BufferIndex);
void xKan_ClearErrCode(XKAN_TypeDef *xKanx);
/* ------------------------------------------- Callback functions ------------------------------------------- */
void xKan_RxBufferNewMessageCallback(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init, xKan_RxHeaderTypeDef *xKan_RxHeader, uint8_t *pRxData, ramtype_t xkanRamAddr);
void xKan_RxFifo0Callback(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init, xKan_RxHeaderTypeDef *xKan_RxHeader, uint8_t *pRxData, ramtype_t xkanRamAddr);
void xKan_RxFifo1Callback(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init, xKan_RxHeaderTypeDef *xKan_RxHeader, uint8_t *pRxData, ramtype_t xkanRamAddr);
/* ------------------------------------------- Macro functions ------------------------------------------- */
/* Get the highest bit of data */
#define __HBIT_POSITION(num)  \
({  \
    typeof(num) _num = (num); \
    typeof(num) _hbit_position = 0; \
    while(_num != 1) {  \
        _hbit_position ++;  \
        _num >>=1;  \
    } \
    (_hbit_position); \
})  \

#define DECLARE_PRIVATE_ARRAY(type, name, ...) static type name[] = {__VA_ARGS__}; \
                                                 static const size_t name##_SIZE = sizeof(name) / sizeof(name[0])

#ifdef __cplusplus
}
#endif
#endif /* _NS__H */

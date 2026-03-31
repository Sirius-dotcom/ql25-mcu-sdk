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
/*!
 * @file     nor_flash.h
 * @brief    This file contains all the functions prototypes for the SPI Nor-Flash firmware
 */

#ifndef _NS__SPI_NOR_FLASH_H_
#define _NS__SPI_NOR_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ns_sdk_hal.h"

#define DUMMY_BYTE                0xFF

/* flash byte mode */
#define BYTE_MODE3    0x03
#define BYTE_MODE4    0x04

/* flash mode control byte */
#define CONTI_MODE_ENABLE   (1<<5)
#define CONTI_MODE_DISABLE  0xFF

#define CS_ON         QSPI_CSMODE_MODE_HOLD
#define CS_OFF        QSPI_CSMODE_MODE_OFF

#define FLASH_STATUS_REG1     0x1
#define FLASH_STATUS_REG2     0x2
#define FLASH_STATUS_REG3     0x3

/* the follow status flag is used for wq25xx flash */
/* flash status define (used) */
#define FLASH_STATUS1_BUSY            (0x01 << 0)
#define FLASH_STATUS1_WEN             (0x01 << 1)

#define FLASH_STATUS2_QE              (0x01 << 1)

/* the follow status flag is used for mx25xx flash */
#define FLASH_STATUS1_QE              (0x01 << 6)

/* flash operation cmd instruction */
#define FLASH_WriteEnable               0x06
#define FLASH_WriteDisable              0x04
#define FLASH_ReadStatusReg1            0x05
#define FLASH_ReadStatusReg2            0x35
#define FLASH_ReadStatusReg3            0x15
#define FLASH_WriteStatusReg1           0x01
#define FLASH_WriteStatusReg2           0x31
#define FLASH_WriteStatusReg3           0x11
#define FLASH_ReadData                  0x03
#define FLASH_FastReadData              0x0B
#define FLASH_FastReadDual              0x3B
#define FLASH_FastReadQuad              0x6B
#define FLASH_PageProgram               0x02
#define FLASH_PageProgramQuad           0x32
#define FLASH_BlockErase                0xD8
#define FLASH_SectorErase               0x20
#define FLASH_ChipErase                 0xC7
#define FLASH_PowerDown                 0xB9
#define FLASH_ReleasePowerDown          0xAB
#define FLASH_DeviceID                  0xAB
#define FLASH_ManufactDeviceID          0x90
#define FLASH_JedecDeviceID             0x9F
#define FLASH_Enable4ByteAddr           0xB7
#define FLASH_Exit4ByteAddr             0xE9

/* ddr read cmd */
#define FLASH_DdrReadData               0x0D
#define FLASH_DdrReadDual               0xBD
#define FLASH_DdrReadQuad               0xED

void Spi_NorFlash_Init(QSPI_TypeDef* QSPIx);                                 /*!< QSPI init */
uint16_t Spi_NorFlash_ReadID(QSPI_TypeDef* QSPIx);                           /*!< Read flash ID */
uint32_t Spi_NorFlash_ReadJEDECID(QSPI_TypeDef* QSPIx);                           /*!< Read flash JEDEC */
uint8_t Spi_NorFlash_ReadSR(QSPI_TypeDef* QSPIx, uint8_t RegNo);             /*!< Read flash status register */
void Spi_NorFlash_Enter4ByteMode(QSPI_TypeDef* QSPIx);                       /*!< Enter flash 4 byte modes */
void Spi_NorFlash_Exit4ByteMode(QSPI_TypeDef* QSPIx);                        /*!< Exit flash 4 byte modes */
void Spi_NorFlash_Write_SR(QSPI_TypeDef* QSPIx, uint8_t RegNo,uint8_t Status);   /*!< write to flash status register */
void Spi_NorFlash_Write_Enable(QSPI_TypeDef* QSPIx);                         /*!< Enable flash write enable */
void Spi_NorFlash_Write_Disable(QSPI_TypeDef* QSPIx);                        /*!< flash write protection */

void Spi_NorFlash_Write_Check(QSPI_TypeDef* QSPIx, uint16_t Command, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); /*!< write data to flash with checking */
void Spi_NorFlash_Read(QSPI_TypeDef* QSPIx, uint16_t Command, uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);            /*!< read data from flash */
void Spi_NorFlash_Write(QSPI_TypeDef* QSPIx, uint16_t Command, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);       /*!< Writes block of data to the FLASH */
void Spi_NorFlash_Write_Page(QSPI_TypeDef* QSPIx, uint16_t Command, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);    /*!< Writes more than one byte to the FLASH with a single WRITE
                                                                                                                                         cycle(Page WRITE sequence). The number of byte can't exceed
                                                                                                                                         the FLASH page size. */
void Spi_NorFlash_Erase_Chip(QSPI_TypeDef* QSPIx);                           /*!< erase the whole flash chip ram */ 
void Spi_NorFlash_Erase_Sector(QSPI_TypeDef* QSPIx, uint32_t Addr);          /*!< Erase sector flash chip ram */ 
void Spi_NorFlash_Wait_Busy(QSPI_TypeDef* QSPIx);                            /*!< Wait for flash enter into idle */
void Spi_NorFlash_PowerDown(QSPI_TypeDef* QSPIx);                            /*!< Enter power down mode */
void Spi_NorFlash_WAKEUP(QSPI_TypeDef* QSPIx);                               /*!< wake up flash chip */ 
void Spi_NorFlash_CS(QSPI_TypeDef* QSPIx, uint8_t CsMode);                   /*!< CS bus control */

void Spi_NorFlash_EnableQuadMode_WQ25xx(QSPI_TypeDef* QSPIx);
void Spi_NorFlash_EnableQuadMode_MX25xx(QSPI_TypeDef* QSPIx);

uint8_t SPI_ReadByte(QSPI_TypeDef* QSPIx);
void SPI_WriteByte(QSPI_TypeDef* QSPIx, uint8_t TxData);

#ifdef __cplusplus
}
#endif

#endif

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

/* Includes ------------------------------------------------------------------*/
#include "nor_flash.h"
#include "ns_qspi.h"
/* ----------------------------------------private api--------------------------------------*/
static void Spi_DivSet(QSPI_TypeDef* QSPIx, uint8_t Speed);
static uint8_t Spi_ReadWrite(QSPI_TypeDef* QSPIx, uint8_t Txdata);
static uint8_t SPI_ReadData(QSPI_TypeDef* QSPIx, uint8_t* RxData, uint32_t Counts);
static void SPI_WriteData(QSPI_TypeDef* QSPIx, uint8_t* TxData, uint32_t Counts);
static uint8_t SPI_SendDummyData(QSPI_TypeDef* QSPIx, uint8_t DummyCycle);

/* Default is 3 bytes mode */
uint16_t flash_addr_mode = BYTE_MODE3;
uint8_t tmp_buffer[4096];

static void Spi_DivSet(QSPI_TypeDef* QSPIx, uint8_t Speed)
{
    QSPIx->SCKDIV = Speed;
}

void SPI_WriteByte(QSPI_TypeDef* QSPIx, uint8_t TxData)
{
    QSPI_DirectionConfig(QSPIx, QSPI_FMT_DIR_TX);
    while (SET == QSPI_GetFlag(QSPIx, QSPI_STATUS_TX_FULL)) {}
    QSPI_SendData(QSPIx, TxData);
    while (SET == QSPI_GetFlag(QSPIx, QSPI_STATUS_BUSY)) {}
}

uint8_t SPI_ReadByte(QSPI_TypeDef* QSPIx)
{
    uint8_t rxdata = 0;
    QSPI_DirectionConfig(QSPIx, QSPI_FMT_DIR_RX);
    QSPI_SendData(QSPIx, 0xff);
    while (SET == QSPI_GetFlag(QSPIx, QSPI_STATUS_RX_EMPTY)) {}
    rxdata = QSPI_ReceiveData(QSPIx) & 0xFF;
    return rxdata;
}

static void SPI_WriteData(QSPI_TypeDef* QSPIx, uint8_t* TxData, uint32_t Counts)
{
    QSPI_DirectionConfig(QSPIx, QSPI_FMT_DIR_TX);
    for(int i = 0; i < Counts; i++) {
        while (SET == QSPI_GetFlag(QSPIx, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPIx, *TxData++);
    }
    while (SET == QSPI_GetFlag(QSPIx, QSPI_STATUS_BUSY)) {}
}

static uint8_t SPI_ReadData(QSPI_TypeDef* QSPIx, uint8_t* RxData, uint32_t Counts)
{
    QSPI_DirectionConfig(QSPIx, QSPI_FMT_DIR_RX);
    for(int i = 0; i < Counts; i++) {
        QSPI_SendData(QSPIx, 0xff);
        while (SET == QSPI_GetFlag(QSPIx, QSPI_STATUS_RX_EMPTY)) {}
        *RxData++ = QSPI_ReceiveData(QSPIx) & 0xFF;
    }
}

static uint8_t SPI_SendDummyData(QSPI_TypeDef* QSPIx, uint8_t DummyCycle)
{
    /* If you want to process the received dummy data, fellow the stop:(otherwise don't change the next code)
     * 1.turn on rxfifo reception enable
     * 2.read data from rxfifo
     */
    QSPI_DirectionConfig(QSPIx, QSPI_FMT_DIR_RX);
    QSPI_BusWidthSwitch(QSPIx, QSPI_FMT_PROTO_HDS);
    QSPI_RxFifoEnable(QSPIx, DISABLE);

    QSPI_FrameSizeConfig(QSPIx, DummyCycle);

    while (SET == QSPI_GetFlag(QSPIx, QSPI_STATUS_TX_FULL)) {}
    QSPI_SendData(QSPIx, DUMMY_BYTE);
    while (SET == QSPI_GetFlag(QSPIx, QSPI_STATUS_BUSY)) {}

    QSPI_FrameSizeConfig(QSPIx, 8);
    QSPI_RxFifoEnable(QSPIx, ENABLE);
}

static uint8_t Spi_ReadWrite(QSPI_TypeDef* QSPIx, uint8_t Txdata)
{
    uint8_t Rxdata;
    QSPI_TransmitReceive(QSPIx, &Txdata, &Rxdata);
    return Rxdata;
}

void Spi_NorFlash_CS(QSPI_TypeDef* QSPIx, uint8_t CsMode)
{
    if(CsMode == CS_ON){
        QSPI_CSModeConfig(QSPIx, CS_ON);
    }else{
        QSPI_CSModeConfig(QSPIx, CS_OFF);
    }
}

void Spi_NorFlash_Init(QSPI_TypeDef* QSPIx)
{
    uint8_t sta_tmpid;
    Spi_NorFlash_CS(QSPIx, CS_OFF);
    QSPI_InitTypeDef spi_init_struct;
    /* deinitilize SPI and the parameters */
    QSPI_StructInit(&spi_init_struct);
    /* QSPIx parameter config */
    spi_init_struct.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    spi_init_struct.DataSize = QSPI_FMT_LEN_8B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_HIGH;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE2;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    /* QSPIx parameter config */
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    QSPI_Init(QSPIx, &spi_init_struct);
    QSPIx->CSMODE |= QSPI_CSMODE_MODE_HOLD;

    Spi_DivSet(QSPIx, QSPI_SCKDIV_PRESCALER_4);
    /* we can know flash mode via reading it's id for different flash.
    flash_addr_mode = Spi_NorFlash_ReadID(QSPIx);
    */
    if(flash_addr_mode == BYTE_MODE4) {
        sta_tmpid = Spi_NorFlash_ReadSR(QSPIx, FLASH_STATUS_REG3);
        if((sta_tmpid & 0X01) != 1)  
        {
            Spi_NorFlash_CS(QSPIx, CS_ON);
            SPI_WriteByte(QSPIx, FLASH_Enable4ByteAddr);
            Spi_NorFlash_CS(QSPIx, CS_OFF);
        }
    }
}

uint8_t Spi_NorFlash_ReadSR(QSPI_TypeDef* QSPIx, uint8_t RegNo)
{
    uint8_t status = 0, command = 0;
    switch(RegNo) {
        case FLASH_STATUS_REG1:
            command = FLASH_ReadStatusReg1; 
            break;
        case FLASH_STATUS_REG2:
            command = FLASH_ReadStatusReg2; 
            break;
        case FLASH_STATUS_REG3:
            command = FLASH_ReadStatusReg3; 
            break;
        default:
            command = FLASH_ReadStatusReg1;
            break;
    }
    Spi_NorFlash_CS(QSPIx, CS_ON); 
    SPI_WriteByte(QSPIx, command); 
    status = SPI_ReadByte(QSPIx); 
    Spi_NorFlash_CS(QSPIx, CS_OFF); 
    return status;
}

void Spi_NorFlash_Write_SR(QSPI_TypeDef* QSPIx, uint8_t RegNo, uint8_t Status)
{
    uint8_t command = 0;
    switch(RegNo) {
        case FLASH_STATUS_REG1:
            command=FLASH_WriteStatusReg1;
            break;
        case FLASH_STATUS_REG2:
            command=FLASH_WriteStatusReg2;
            break;
        case FLASH_STATUS_REG3:
            command=FLASH_WriteStatusReg3;
            break;
        default:
            command=FLASH_WriteStatusReg1;
            break;
    }
    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, command);
    SPI_WriteByte(QSPIx, Status);
    Spi_NorFlash_CS(QSPIx, CS_OFF);
}

void Spi_NorFlash_Write_Enable(QSPI_TypeDef* QSPIx)
{
    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, FLASH_WriteEnable);
    Spi_NorFlash_CS(QSPIx, CS_OFF);
}

void Spi_NorFlash_Write_Disable(QSPI_TypeDef* QSPIx)
{
    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, FLASH_WriteDisable);
    Spi_NorFlash_CS(QSPIx, CS_OFF);
}

uint16_t Spi_NorFlash_ReadID(QSPI_TypeDef* QSPIx)
{
    uint16_t tmpid = 0;
    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, FLASH_ManufactDeviceID);
    SPI_WriteByte(QSPIx, 0x00);
    SPI_WriteByte(QSPIx, 0x00);
    SPI_WriteByte(QSPIx, 0x00);
    tmpid |= SPI_ReadByte(QSPIx) << 8;
    tmpid |= SPI_ReadByte(QSPIx);
    Spi_NorFlash_CS(QSPIx, CS_OFF);
    return tmpid;
}

uint32_t Spi_NorFlash_ReadJEDECID(QSPI_TypeDef* QSPIx)
{
    uint8_t jedec_id[3];
    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, FLASH_JedecDeviceID);
    for (int i = 0; i < 3; i++) {
        jedec_id[i] =  SPI_ReadByte(QSPIx);
    }
    Spi_NorFlash_CS(QSPIx, CS_OFF);
    return (jedec_id[0] | jedec_id[1] << 8 | jedec_id[1] << 16);
}

void Spi_NorFlash_Read(QSPI_TypeDef* QSPIx, uint16_t Command, uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
    uint32_t bus_width;
    uint8_t dummy_cycle = 8;
    Spi_NorFlash_CS(QSPIx, CS_ON);
    /* spi mode send cmd */
    SPI_WriteByte(QSPIx, Command);

    /* ddr mode */
    switch (Command) {
        case FLASH_DdrReadQuad:
            bus_width = QSPI_FMT_PROTO_QUAD;
            break;
        case FLASH_DdrReadDual:
            bus_width = QSPI_FMT_PROTO_DUAL;
            break;
        case FLASH_DdrReadData:
            bus_width = QSPI_FMT_PROTO_SINGLE;
            break;          
        default:
            goto normal;
            break;
    }
    QSPI_DdrModeEnable(QSPIx, ENABLE);
    QSPI_BusWidthSwitch(QSPIx, bus_width);

normal:

    if(flash_addr_mode == BYTE_MODE4) {
        SPI_WriteByte(QSPIx, (uint8_t)((ReadAddr) >> 24));
    }
    SPI_WriteByte(QSPIx, (uint8_t)((ReadAddr) >> 16));
    SPI_WriteByte(QSPIx, (uint8_t)((ReadAddr) >> 8));
    SPI_WriteByte(QSPIx, (uint8_t)ReadAddr);

    if(Command == (FLASH_DdrReadQuad) || (Command == FLASH_DdrReadDual)) {
        SPI_WriteByte(QSPIx, CONTI_MODE_DISABLE);
    }

    if (Command == FLASH_FastReadData) {
        bus_width = QSPI_FMT_PROTO_SINGLE;
    } else if (Command == FLASH_FastReadDual) {
        bus_width = QSPI_FMT_PROTO_DUAL;
    } else if (Command == FLASH_FastReadQuad) {
        bus_width = QSPI_FMT_PROTO_QUAD;
    } else if (Command == FLASH_DdrReadQuad) {
        dummy_cycle = 7*2;
    } else if (Command == FLASH_DdrReadDual) {
        dummy_cycle = 4*2;
    } else if (Command == FLASH_DdrReadData) {
        dummy_cycle = 6*2;
    } else {
        /* more read-cmd need to handle */
    }

    SPI_SendDummyData(QSPIx, dummy_cycle);

    /* sdr mode switch bus width */
    QSPI_BusWidthSwitch(QSPIx, bus_width);

    SPI_ReadData(QSPIx, pBuffer, NumByteToRead);

    Spi_NorFlash_CS(QSPIx, CS_OFF);
}

void Spi_NorFlash_Write_Page(QSPI_TypeDef* QSPIx, uint16_t Command, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint32_t bus_width;
    Spi_NorFlash_Write_Enable(QSPIx);

    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, Command);
    if(flash_addr_mode == BYTE_MODE4)
    {
        SPI_WriteByte(QSPIx, (uint8_t)((WriteAddr) >> 24));
    }
    SPI_WriteByte(QSPIx, (uint8_t)((WriteAddr) >> 16));
    SPI_WriteByte(QSPIx, (uint8_t)((WriteAddr) >> 8));
    SPI_WriteByte(QSPIx, (uint8_t)WriteAddr);

    if (Command == FLASH_PageProgram) {
        bus_width = QSPI_FMT_PROTO_SINGLE;
    } else if (Command == FLASH_PageProgramQuad) {
        bus_width = QSPI_FMT_PROTO_QUAD;
    } else {
        /* program does not expect to enter */
    }

    QSPI_BusWidthSwitch(QSPIx, bus_width);

    SPI_WriteData(QSPIx, pBuffer, NumByteToWrite);

    Spi_NorFlash_CS(QSPIx, CS_OFF);
    Spi_NorFlash_Wait_Busy(QSPIx);
}

void Spi_NorFlash_Write_Check(QSPI_TypeDef* QSPIx, uint16_t Command, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256;
    if(NumByteToWrite <= pageremain) {
        pageremain = NumByteToWrite;
    }
    while(1) {
        Spi_NorFlash_Write_Page(QSPIx, Command, pBuffer,WriteAddr,pageremain);
        if(NumByteToWrite == pageremain) {
            break;
        } else { 
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain;
            if(NumByteToWrite > 256) {
                pageremain=256;
            } else {
                pageremain = NumByteToWrite;
            }
        }
    }
}

void Spi_NorFlash_Write(QSPI_TypeDef* QSPIx, uint16_t Command, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t * secbuf;
    secbuf = tmp_buffer;
    secpos = WriteAddr / 4096;
    secoff = WriteAddr % 4096;
    secremain = 4096-secoff;
    
    if(NumByteToWrite <= secremain) {
        secremain = NumByteToWrite;
    }
    while(1) {
        Spi_NorFlash_Read(QSPIx, FLASH_FastReadData, secbuf, secpos*4096,4096);
        for(i = 0; i < secremain; i++) {
            if(secbuf[secoff+i] != 0XFF) {
                break;
            }
        }
        if(i < secremain) { 
            Spi_NorFlash_Erase_Sector(QSPIx, secpos);
            for(i = 0;i < secremain;i++) {
                secbuf[i+secoff] = pBuffer[i];
            }
            Spi_NorFlash_Write_Check(QSPIx, Command, secbuf, secpos*4096, 4096);
        } else {
            Spi_NorFlash_Write_Check(QSPIx, Command, pBuffer, WriteAddr, secremain);
        }
        if(NumByteToWrite == secremain) {
            break;
        } else {
            secpos ++;
            secoff = 0;

            pBuffer += secremain; 
            WriteAddr += secremain;
            NumByteToWrite -= secremain;
            if(NumByteToWrite > 4096) {
                secremain = 4096;
            } else {
                secremain = NumByteToWrite;
            }
        }
    }
}

void Spi_NorFlash_Erase_Chip(QSPI_TypeDef* QSPIx)
{
    Spi_NorFlash_Write_Enable(QSPIx);
    Spi_NorFlash_Wait_Busy(QSPIx);
    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, FLASH_ChipErase);
    Spi_NorFlash_CS(QSPIx, CS_OFF);
    Spi_NorFlash_Wait_Busy(QSPIx);
}

void Spi_NorFlash_Erase_Sector(QSPI_TypeDef* QSPIx, uint32_t Addr)
{
    Addr*=4096;
    Spi_NorFlash_Write_Enable(QSPIx);
    Spi_NorFlash_Wait_Busy(QSPIx);
    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, FLASH_SectorErase);
    if(flash_addr_mode == BYTE_MODE4) {
        SPI_WriteByte(QSPIx, (uint8_t)((Addr) >> 24));
    }
    SPI_WriteByte(QSPIx, (uint8_t)((Addr) >> 16));
    SPI_WriteByte(QSPIx, (uint8_t)((Addr) >> 8));
    SPI_WriteByte(QSPIx, (uint8_t)Addr);
    Spi_NorFlash_CS(QSPIx, CS_OFF);
    Spi_NorFlash_Wait_Busy(QSPIx);
}

void Spi_NorFlash_Enter4ByteMode(QSPI_TypeDef* QSPIx)
{
    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, FLASH_Enable4ByteAddr);
    Spi_NorFlash_CS(QSPIx, CS_OFF);
}

void Spi_NorFlash_Exit4ByteMode(QSPI_TypeDef* QSPIx)
{
    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, FLASH_Exit4ByteAddr);
    Spi_NorFlash_CS(QSPIx, CS_OFF);
}

void Spi_NorFlash_Wait_Busy(QSPI_TypeDef* QSPIx)
{
    uint8_t temp = 0;
    QSPI_BusWidthSwitch(QSPIx, QSPI_FMT_PROTO_SINGLE);
    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, FLASH_ReadStatusReg1);
    do {
        temp = SPI_ReadByte(QSPIx) & FLASH_STATUS1_BUSY;
    } while (temp);
    Spi_NorFlash_CS(QSPIx, CS_OFF);
}

void Spi_NorFlash_PowerDown(QSPI_TypeDef* QSPIx)
{
    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, FLASH_PowerDown);
    Spi_NorFlash_CS(QSPIx, CS_OFF);
    delay_1ms(3);
}

void Spi_NorFlash_WAKEUP(QSPI_TypeDef* QSPIx)
{
    Spi_NorFlash_CS(QSPIx, CS_ON);
    SPI_WriteByte(QSPIx, FLASH_ReleasePowerDown);
    Spi_NorFlash_CS(QSPIx, CS_OFF);
    delay_1ms(3);
}

/* --------------------------------------- device diff api --------------------------------------- */
void Spi_NorFlash_EnableQuadMode_WQ25xx(QSPI_TypeDef* QSPIx)
{
    QSPI_BusWidthSwitch(QSPIx, QSPI_FMT_PROTO_SINGLE);
    Spi_NorFlash_Write_Enable(QSPIx);
    Spi_NorFlash_Write_SR(QSPIx, FLASH_STATUS_REG2, FLASH_STATUS2_QE);
    Spi_NorFlash_Wait_Busy(QSPIx);
}

void Spi_NorFlash_EnableQuadMode_MX25xx(QSPI_TypeDef* QSPIx)
{
    QSPI_BusWidthSwitch(QSPIx, QSPI_FMT_PROTO_SINGLE);
    Spi_NorFlash_Write_Enable(QSPIx);
    Spi_NorFlash_Write_SR(QSPIx, FLASH_STATUS_REG1, FLASH_STATUS1_QE);
    Spi_NorFlash_Wait_Busy(QSPIx);
}

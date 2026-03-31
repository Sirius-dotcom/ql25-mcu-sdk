/**
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

#include <stdio.h>
#include <string.h>
#include "ns_sdk_hal.h"

#define  sFLASH_ID              0XC22019

#ifndef QSPI_XIP0_MEM_BASE
#ifdef QSPI_XIP0_FRTDEC_XIP_BASE
#define QSPI_XIP0_MEM_BASE QSPI_XIP0_FRTDEC_XIP_BASE
#else
#define QSPI_XIP0_MEM_BASE QSPI_FLASH_BASE
#endif
#endif

/* flash operation cmd instruction */
#define FLASH_WriteEnable               0x06
#define FLASH_WriteDisable              0x04
#define FLASH_ReadStatusReg1            0x05
#define FLASH_ReadStatusReg2            0x35
#define FLASH_ReadStatusReg3            0x15
#define FLASH_WriteStatusReg            0x01
#define FLASH_ReadData                  0x03
#define FLASH_FastReadData              0x0B
#define FLASH_FastReadDual              0x3B
#define FLASH_FastReadQuad              0x6B
#define FLASH_DdrReadQuad               0xED
#define FLASH_PageProgram               0x02 
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

/* WIP(busy) flag */
#define WIP_Flag                  0x01
/* QE(quad enable)*/
#define QE_Flag                   (0x01 << 6)
#define Dummy_Byte                0xFF

#define SPI_CS_ON  (QSPI_XIP_CSModeConfig(QSPI_XIP0, QSPI_XIP_CSMODE_MODE_HOLD))
#define SPI_CS_OFF (QSPI_XIP_CSModeConfig(QSPI_XIP0, QSPI_XIP_CSMODE_MODE_OFF))

/**
  * \brief configure the SPI peripheral
  */
void SPI_Init(void)
{
    QSPI_XIP_InitTypeDef spixip_init_struct = {0};
    /* deinitilize SPI and the parameters */
    QSPI_XIP_StructInit(&spixip_init_struct);
    
    spixip_init_struct.ProtolMode = QSPI_XIP_FMT_PROTO_SINGLE;
    spixip_init_struct.DataSize = QSPI_XIP_FMT_LEN_8B;
    spixip_init_struct.CPOL = QSPI_XIP_SCKMODE_CPOL_LOW;
    spixip_init_struct.CPHA = QSPI_XIP_SCKMODE_CPHA_EDGE1;
    spixip_init_struct.SCKDIV = QSPI_XIP_SCKDIV_PRESCALER_8;
    spixip_init_struct.Endian = QSPI_XIP_FMT_ENDIAN_MSB;
    spixip_init_struct.SSM = QSPI_XIP_CR_SSM_HARD;
    spixip_init_struct.DevMode = QSPI_XIP_CR_MODE_MASTER;
    spixip_init_struct.Force = (QSPI_XIP_FORCE_EN | QSPI_XIP_FORCE_WP);
    QSPI_XIP_Init(QSPI_XIP0, &spixip_init_struct);
}

void SPI_WriteByte(uint8_t txdata)
{
    QSPI_XIP_DirectionConfig(QSPI_XIP0, QSPI_XIP_FMT_DIR_TX);
    while (SET == QSPI_XIP_GetFlag(QSPI_XIP0, QSPI_XIP_STATUS_TX_FULL)) {}
    QSPI_XIP_SendData(QSPI_XIP0, txdata);
    while (SET == QSPI_XIP_GetFlag(QSPI_XIP0, QSPI_XIP_STATUS_BUSY)) {}
}

uint8_t SPI_ReadByte(void)
{
    uint8_t rxdata = 0;
    QSPI_XIP_DirectionConfig(QSPI_XIP0, QSPI_XIP_FMT_DIR_RX);
    QSPI_XIP_SendData(QSPI_XIP0, Dummy_Byte);
    while (SET == QSPI_XIP_GetFlag(QSPI_XIP0, QSPI_XIP_STATUS_RX_EMPTY)) {}
    rxdata = QSPI_XIP_ReceiveData(QSPI_XIP0) & 0xFF;
    return rxdata;
}

void SPI_ReadJEDEC(void)
{
    uint32_t flashid = 0;
    SPI_CS_ON;
    SPI_WriteByte(FLASH_JedecDeviceID);
    for (int i = 0; i < 3; i++) {
        flashid |= (SPI_ReadByte() << ((2-i)*8));
    }
    printf("JEDEC: %#x\n", flashid);
    SPI_CS_OFF;
}

void SPI_Wip(void)
{
    uint8_t temp = 0;
    SPI_CS_ON;
    SPI_WriteByte(FLASH_ReadStatusReg1);
    do {
        temp = SPI_ReadByte() & WIP_Flag;
    } while (temp);
    SPI_CS_OFF;
}

void SPI_WriteEnable(void)
{
    SPI_CS_ON;
    SPI_WriteByte(FLASH_WriteEnable);
    SPI_CS_OFF;
}

void SPI_Quad(void)
{
    uint32_t value = QSPI_XIP0->FMT;
    value &= ~QSPI_XIP_FMT_PROTO_MASK;
    QSPI_XIP0->FMT = value | QSPI_XIP_FMT_PROTO_SINGLE;
    SPI_WriteEnable();
    SPI_CS_ON;
    SPI_WriteByte(FLASH_WriteStatusReg);
    SPI_WriteByte(QE_Flag);
    SPI_CS_OFF;
    SPI_Wip();
}

void SPI_Erase(uint8_t cmd, uint32_t addr)
{
    SPI_WriteEnable();
    SPI_CS_ON;
    SPI_WriteByte(cmd);
    SPI_WriteByte(addr >> 16);
    SPI_WriteByte(addr >> 8);
    SPI_WriteByte(addr);
    SPI_CS_OFF;
    SPI_Wip();
}

void SPI_PageWrite(uint8_t cmd, uint8_t* buf, uint32_t addr)
{
    SPI_WriteEnable();
    SPI_CS_ON;
    SPI_WriteByte(cmd);
    SPI_WriteByte(addr >> 16);
    SPI_WriteByte(addr >> 8);
    SPI_WriteByte(addr);
    for (int i = 0; i < 256; i++) {
        SPI_WriteByte(buf[i]);
    }
    SPI_CS_OFF;
    SPI_Wip();
}

void SPI_PageRead(uint8_t cmd, uint8_t* buf, uint32_t addr)
{
    uint32_t value = QSPI_XIP0->FMT;
    value &= ~QSPI_XIP_FMT_PROTO_MASK;
    SPI_CS_ON;
    SPI_WriteByte(cmd);
    /* Refer to the timing of different commands */
    if (FLASH_DdrReadQuad == cmd) {
        QSPI_XIP0->FMT = value | QSPI_XIP_FMT_PROTO_QUAD;
        QSPI_XIP0->CR |= QSPI_XIP_CR_DDR_ENABLE;
        QSPI_XIP0->CR &= (~QSPI_XIP_CR_RECEIVE_DATA_ENABLE); 
    }
    SPI_WriteByte(addr >> 16);
    SPI_WriteByte(addr >> 8);
    SPI_WriteByte(addr);
    SPI_WriteByte(Dummy_Byte);
    if (FLASH_DdrReadQuad == cmd) {
        QSPI_XIP0->FMT &= (~QSPI_XIP_FMT_DIR_TX);
        for(int i = 0 ;i<5;i++){
            QSPI_XIP_SendData(QSPI_XIP0, Dummy_Byte);
        }
        while(SET == QSPI_XIP_GetFlag(QSPI_XIP0, QSPI_XIP_STATUS_BUSY)){}
        QSPI_XIP0->CR |= (QSPI_XIP_CR_RECEIVE_DATA_ENABLE);
    }
    if (FLASH_FastReadQuad == cmd) {
        QSPI_XIP0->FMT = value | QSPI_XIP_FMT_PROTO_QUAD;
    }

    for (int i = 0; i < 256; i++) {
        buf[i] = SPI_ReadByte();
    }
    SPI_CS_OFF;
}

void Xip_RxTimingConfig(uint32_t cmd_code, uint32_t data_proto)
{
    QSPI_XIP_TimingTypedef xip_timing;
    QSPI_XIP_DdrEnableTypedef xip_ddr_en = {0};
    xip_timing.QSPI_XIP_FFMT_u.reg_ffmt = 0;
    xip_timing.QSPI_XIP_FFMT1_u.reg_ffmt1 = 0;

    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_en = ENABLE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_endina_f = QSPI_XIP_ENDIAN_MSB;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_addr_len = QSPI_XIP_3BYTE_MODE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_proto = QSPI_XIP_SINGLE_LINE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_addr_proto = data_proto;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_data_proto = data_proto;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_code = cmd_code;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_pad_code = 0x00;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_pad_cnt = 6;
    QSPI_XIP_TimingConfig(QSPI_XIP0, &xip_timing);

    xip_ddr_en.CMD_DDR_EN = DISABLE;
    xip_ddr_en.ADDR_DDR_EN = ENABLE;
    xip_ddr_en.MODE_DDR_EN = DISABLE;
    xip_ddr_en.DATA_DDR_EN = ENABLE;
    
    QSPI_XIP_DdrEnable(QSPI_XIP0, xip_ddr_en);
}

void print_data(uint8_t* data)
{
    for (int i = 0; i < 16; i++) {
        printf("0x%x ", data[i]);
    }
    printf("\r\n");
}

void main(void)
{
    uint8_t write_buf[256] = {0};
    uint8_t read_buf[256] = {0};

#ifdef MISC_HAS_QSPI_XIP0_HAS_CLK
    qspi_xip0_clk_en(ENABLE);
#endif
#ifdef MISC_HAS_QSPI_XIP0_CLK_DIV
    qspi_xip0_clk_div(0);
#endif
#ifdef MISC_HAS_QSPI_XIP0_RST
    qspi_xip0_set_rst(DISABLE);
    qspi_xip0_set_rst(ENABLE);
#endif

    for (int i = 0; i < 256; i++) {
        write_buf[i] = i;
    }
    
    SPI_Init();
    SPI_ReadJEDEC();

    SPI_Erase(FLASH_SectorErase, 0x0);
    SPI_PageWrite(FLASH_PageProgram, write_buf, 0x0);

    SPI_Quad();
    QSPI_XIP0->FORCE &= ~(QSPI_XIP_FORCE_EN | QSPI_XIP_FORCE_WP);
    SPI_PageRead(FLASH_DdrReadQuad, read_buf, 0x0);
    if (0 == memcmp(write_buf, read_buf, 256)) {
        printf("4 line data cmp pass\n");
    } else {
        printf("4 line data cmp error\n");
        simulation_fail();
        printf("write_buf : ");
        print_data(write_buf);
        printf("read_buf  : ");
        print_data(read_buf);
        while (1) {}
    }

    memset(read_buf, 0x00, 0);

    Xip_RxTimingConfig(FLASH_DdrReadQuad, QSPI_XIP_QUAD_LINE);
    QSPI_XIP_Enable(QSPI_XIP0, ENABLE);
    memcpy(read_buf, (void*)QSPI_XIP0_MEM_BASE, 256);
    if (0 == memcmp(write_buf, read_buf, 256)) {
        printf("4 line XIP read data cmp pass\n");
    } else {
        printf("4 line XIP read data cmp error\n");
        simulation_fail();
        printf("write_buf : ");
        print_data(write_buf);
        printf("read_buf  : ");
        print_data(read_buf);
        while (1) {}
    }

    SPI_Init();
    simulation_pass();

    while (1) {}
}

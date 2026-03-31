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

#define  sFLASH_ID              0XEF7019    

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
#define FLASH_ReadStatusReg1		    0x05
#define FLASH_ReadStatusReg2		    0x35
#define FLASH_ReadStatusReg3		    0x15
#define FLASH_WriteStatusReg1           0x01
#define FLASH_WriteStatusReg2           0x31
#define FLASH_WriteStatusReg3           0x11
#define FLASH_ReadData                  0x03
#define FLASH_FastReadData              0x0B
#define FLASH_FastReadDual              0x3B
#define FLASH_FastReadQuad_O            0x6B
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

#define FLASH_FastReadQuad_IO           0xEB

/* WIP(busy) flag */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF

#define SPI_CS_ON  (QSPI_XIP_CSModeConfig(QSPI_XIP0, QSPI_XIP_CSMODE_MODE_HOLD))
#define SPI_CS_OFF (QSPI_XIP_CSModeConfig(QSPI_XIP0, QSPI_XIP_CSMODE_MODE_OFF))

#define PAGESIZE 256
/* if the core is 600 or 900, cache line size is 64 */
#define CACHE_LINE 32
#define CYCLE_NUM (PAGESIZE/CACHE_LINE)
uint8_t write[PAGESIZE] = {0};
uint8_t read[PAGESIZE] = {0};
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

    QSPI_XIP_EnableBurstMode(QSPI_XIP0, ENABLE);
    QSPI_XIP_EnableXipWrite(QSPI_XIP0, ENABLE);
}

void Xip_RxTimingConfig(void)
{
    QSPI_XIP_TimingTypedef xip_timing;
    xip_timing.QSPI_XIP_FFMT_u.reg_ffmt = 0;
    xip_timing.QSPI_XIP_FFMT1_u.reg_ffmt1 = 0;

    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_en = ENABLE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_endina_f = QSPI_XIP_ENDIAN_MSB;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_addr_len = QSPI_XIP_3BYTE_MODE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_proto = QSPI_XIP_SINGLE_LINE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_addr_proto = QSPI_XIP_QUAD_LINE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_data_proto = QSPI_XIP_QUAD_LINE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_code = FLASH_FastReadQuad_IO;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_pad_code = 0x00;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_pad_cnt = 4;

    xip_timing.QSPI_XIP_FFMT1_u.bit_ffmt1.spi_mode_proto = QSPI_XIP_QUAD_LINE;
    /* "Continuous Read Mode" bits (M5-4) = (1, 0); mast read 1page once */
    xip_timing.QSPI_XIP_FFMT1_u.bit_ffmt1.spi_mode_code = 0x00; /* 0x10 */
    xip_timing.QSPI_XIP_FFMT1_u.bit_ffmt1.spi_mode_cnt = 8;
    QSPI_XIP_TimingConfig(QSPI_XIP0, &xip_timing);
}

void Xip_TxTimingConfig(void)
{
    QSPI_XIP_TimingTypedef xip_timing;
    xip_timing.QSPI_XIP_FFMT_u.reg_ffmt = 0;
    xip_timing.QSPI_XIP_FFMT1_u.reg_ffmt1 = 0;

    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_en = ENABLE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_endina_f = QSPI_XIP_ENDIAN_MSB;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_addr_len = QSPI_XIP_3BYTE_MODE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_proto = QSPI_XIP_SINGLE_LINE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_addr_proto = QSPI_XIP_SINGLE_LINE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_data_proto = QSPI_XIP_SINGLE_LINE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_code = FLASH_ReadData;

    xip_timing.QSPI_XIP_FFMT1_u.bit_ffmt1.spi_wcmd_code = FLASH_PageProgram;
    QSPI_XIP_TimingConfig(QSPI_XIP0, &xip_timing);
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
    SPI_CS_ON;
    SPI_WriteByte(FLASH_JedecDeviceID);
    for (int i = 0; i < 3; i++) {
        printf("JEDEC[%d]:%#x\n", i, SPI_ReadByte());
    }
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
    SPI_WriteByte(FLASH_WriteStatusReg2);
    SPI_WriteByte(0x1 << 1);
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

void print_data(uint8_t* data)
{
    for (int i = 0; i < 16; i++) {
        printf("0x%x ", data[i]);
    }
    printf("\r\n");
}

void spi_xip_continuous_test(void)
{
    for (int i = 0; i < PAGESIZE; i++) {
        write[i] = 0x00 + i;
    }

    QSPI_XIP_Enable(QSPI_XIP0, DISABLE);
    memset(read, 0x00, PAGESIZE);
    SPI_Erase(FLASH_SectorErase, 0x0);
    SPI_WriteEnable();

    Xip_TxTimingConfig();
    /* You need to switch between xip and normal mode, xip is not recommended for write operations(demo) */
    for(int i = 0; i < CYCLE_NUM; i ++) {
        QSPI_XIP_Enable(QSPI_XIP0, ENABLE);
        memcpy((void*)(QSPI_XIP0_MEM_BASE + CACHE_LINE * i), (write + CACHE_LINE * i), CACHE_LINE);
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
        /* flush dcache */
        MFlushDCache();
#endif
        QSPI_XIP_Enable(QSPI_XIP0, DISABLE);
        SPI_Wip();
        SPI_WriteEnable();
    }
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    /* inval dcache */
    MInvalDCache();
#endif
    /* duad proto need to send cmd to flash via normal mode */
    QSPI_XIP_Enable(QSPI_XIP0, DISABLE);
    SPI_Quad();
    QSPI_XIP0->FORCE &= ~(QSPI_XIP_FORCE_EN | QSPI_XIP_FORCE_WP);

    Xip_RxTimingConfig();
    QSPI_XIP_Enable(QSPI_XIP0, ENABLE);
    for(int i = 0; i < CYCLE_NUM; i ++) {
        memcpy((read + CACHE_LINE * i), (void*)(QSPI_XIP0_MEM_BASE + CACHE_LINE * i), CACHE_LINE);
    }

    if (0 == memcmp(write, read, PAGESIZE)) {
        printf("4 line XIP read data cmp pass\n");
    } else {
        printf("4 line XIP read data cmp error\n");
        simulation_fail();
        printf("write : ");
        print_data(write);
        printf("read  : ");
        print_data(read);
        while (1) {}
    }

    QSPI_XIP_Enable(QSPI_XIP0, DISABLE);
}

void main(void)
{
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
    /* enable cache */
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    EnableDCache();
#else
    simulation_pass();
    /* The soc doesn't have a dcache, so there is no need to care about this case. */
    while (1) {}
#endif

    SPI_Init();
    
    spi_xip_continuous_test();

    SPI_Init();
    simulation_pass();

    while (1) {}
}

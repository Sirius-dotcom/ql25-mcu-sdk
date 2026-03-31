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

#define PSRAM_READ                 0x03
#define PSRAM_FAST_READ            0x0B
#define PSRAM_FAST_READ_QUAD       0xEB
#define PSRAM_WRITE                0x02
#define PSRAM_QUAD_WRITE           0x38
#define PSRAM_WRAP_READ            0x8B
#define PSRAM_WRAP_WRITE           0x82
#define PSRAM_REG_READ             0xB5
#define PSRAM_REG_WRITE            0xB1
#define PSRAM_ENTER_QMODE          0x35
#define PSRAM_EXIT_QMODE           0xF5
#define PSRAM_RESET_EN             0x66
#define PSRAM_RESET                0x99
#define PSRAM_SET_BURST_LEN        0xC0
#define PSRAM_DEVICE_ID            0x9F

#define PSRAM_ID_MFID          0xd
#define PSRAM_ID_KGD           0x5d
#define PSRAM_ID_EID           0x00

#ifndef QSPI_XIP0_MEM_BASE
#ifdef QSPI_XIP0_FRTDEC_XIP_BASE
#define QSPI_XIP0_MEM_BASE QSPI_XIP0_FRTDEC_XIP_BASE
#else
#define QSPI_XIP0_MEM_BASE QSPI_FLASH_BASE
#endif
#endif

#define SPI_CS_ON  (QSPI_XIP_CSModeConfig(QSPI_XIP0, QSPI_XIP_CSMODE_MODE_HOLD))
#define SPI_CS_OFF (QSPI_XIP_CSModeConfig(QSPI_XIP0, QSPI_XIP_CSMODE_MODE_OFF))

#define PAGESIZE 256
#define Dummy_Byte                0xFF

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
    QSPI_XIP_EnableXipWrite(QSPI_XIP0, ENABLE);
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

void SPI_ReadID(void)
{
    SPI_CS_ON;
    SPI_WriteByte(PSRAM_DEVICE_ID);
    
    SPI_WriteByte(0x00);
    SPI_WriteByte(0x00);
    SPI_WriteByte(0x00);
    printf("MFID:%#x\n",SPI_ReadByte());
    printf("KGD:%#x\n",SPI_ReadByte());
    printf("EID:%#x\n",SPI_ReadByte());
    SPI_CS_OFF;
}

void SPI_Quad(void)
{
    SPI_CS_ON;
    SPI_WriteByte(PSRAM_ENTER_QMODE);
    SPI_CS_OFF;
}

void SPI_ExitQuad(void)
{
    SPI_CS_ON;
    QSPI_XIP0->FMT |= QSPI_XIP_FMT_PROTO_QUAD;
    SPI_WriteByte(PSRAM_EXIT_QMODE);
    SPI_CS_OFF;
}

void SPI_PageWrite(uint8_t cmd, uint8_t* buf, uint32_t addr)
{
    SPI_CS_ON;
    uint32_t value = QSPI_XIP0->FMT;
    value &= ~QSPI_XIP_FMT_PROTO_MASK;
    if(PSRAM_QUAD_WRITE == cmd) {
        QSPI_XIP0->FMT = value | QSPI_XIP_FMT_PROTO_QUAD;
    } else if(PSRAM_WRITE == cmd) {
        QSPI_XIP0->FMT = value | QSPI_XIP_FMT_PROTO_SINGLE;
    }
    SPI_WriteByte(cmd);
    SPI_WriteByte(addr >> 16);
    SPI_WriteByte(addr >> 8);
    SPI_WriteByte(addr);
    for(int i = 0; i < PAGESIZE; i++) {
        SPI_WriteByte(buf[i]);
    }
    SPI_CS_OFF;
}

void SPI_PageRead(uint8_t cmd, uint8_t* buf, uint32_t addr)
{
    SPI_CS_ON;
    SPI_WriteByte(cmd);
    uint32_t value = QSPI_XIP0->FMT;
    value &= ~QSPI_XIP_FMT_PROTO_MASK;
    if(PSRAM_FAST_READ_QUAD == cmd){
        QSPI_XIP0->FMT = value | QSPI_XIP_FMT_PROTO_QUAD;
    }else if(PSRAM_READ == cmd){
        QSPI_XIP0->FMT = value | QSPI_XIP_FMT_PROTO_SINGLE;
    }
    SPI_WriteByte(addr >> 16);
    SPI_WriteByte(addr >> 8);
    SPI_WriteByte(addr);
    
    if(PSRAM_FAST_READ_QUAD == cmd){
        for(int i = 0; i < 3; i++){
            SPI_WriteByte(0x00);
        }
    }
    for(int i = 0; i < PAGESIZE; i++) {
        buf[i] = SPI_ReadByte();
    }
    SPI_CS_OFF;
}

void Xip_RxTimingConfig(uint32_t cmd_code, uint32_t proto)
{
    QSPI_XIP_TimingTypedef xip_timing;
    xip_timing.QSPI_XIP_FFMT_u.reg_ffmt = 0;
    xip_timing.QSPI_XIP_FFMT1_u.reg_ffmt1 = 0;

    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_en = ENABLE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_endina_f = QSPI_XIP_ENDIAN_MSB;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_addr_len = QSPI_XIP_3BYTE_MODE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_proto = QSPI_XIP_SINGLE_LINE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_addr_proto = proto;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_data_proto = proto;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_code = PSRAM_READ;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_pad_code = 0x00;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_pad_cnt = 6;

    QSPI_XIP_TimingConfig(QSPI_XIP0, &xip_timing);
}

void Xip_TxTimingConfig(uint32_t cmd_code, uint32_t proto)
{
    QSPI_XIP_TimingTypedef xip_timing;
    xip_timing.QSPI_XIP_FFMT_u.reg_ffmt = 0;
    xip_timing.QSPI_XIP_FFMT1_u.reg_ffmt1 = 0;

    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_en = ENABLE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_endina_f = QSPI_XIP_ENDIAN_MSB;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_addr_len = QSPI_XIP_3BYTE_MODE;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_proto = proto;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_addr_proto = proto;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_data_proto = proto;
    xip_timing.QSPI_XIP_FFMT_u.bit_ffmt.spi_cmd_code = PSRAM_READ;

    xip_timing.QSPI_XIP_FFMT1_u.bit_ffmt1.spi_wcmd_code = cmd_code;
    QSPI_XIP_TimingConfig(QSPI_XIP0, &xip_timing);
}

void print_data(uint8_t* data)
{
    for (int i = 0; i < 16; i++) {
        printf("0x%x ", data[i]);
    }
    printf("\r\n");
}

/* spi xip read-write flash via normal mode */
void spi_normal_mode_test(void){
    for(int i = 0; i < PAGESIZE; i++) {
        write[i] = i;
    }
    SPI_Init();
    SPI_ReadID();

    memset(read, 0x00, PAGESIZE);
    
    SPI_PageWrite(PSRAM_WRITE, write, 0x0);
    SPI_PageRead(PSRAM_READ, read, 0x0);
    if(0 == memcmp(write, read, PAGESIZE)) {
        printf("1 line data cmp pass\n");
    } else {
        printf("1 line data cmp error\n");
        simulation_fail();
        printf("write : ");
        print_data(write);
        printf("read  : ");
        print_data(read);
        while (1) {}
    }

    memset(read, 0x00, PAGESIZE);
    SPI_Quad();

    QSPI_XIP0->FORCE &= ~(QSPI_XIP_FORCE_EN | QSPI_XIP_FORCE_WP);
    SPI_PageWrite(PSRAM_QUAD_WRITE, write, 0x100);
    SPI_PageRead(PSRAM_FAST_READ_QUAD, read, 0x100);
    if (0 == memcmp(write, read, 256)) {
        printf("4 line data cmp pass\n");
    } else {
        printf("4 line data cmp error\n");
        simulation_fail();
        printf("write : ");
        print_data(write);
        printf("read  : ");
        print_data(read);
        while (1) {}
    }
    SPI_ExitQuad();
}

/* spi xip read-write flash via xip mode */
void spi_xip_mode_test(void){
    for (int i = 0; i < PAGESIZE; i++) {
        write[i] = i;
    }
    memset(read, 0x00, PAGESIZE);
    
    QSPI_XIP_Enable(QSPI_XIP0, DISABLE);
    SPI_Quad();
    QSPI_XIP0->FORCE &= ~(QSPI_XIP_FORCE_EN | QSPI_XIP_FORCE_WP);

    Xip_TxTimingConfig(PSRAM_QUAD_WRITE, QSPI_XIP_DUAL_LINE);
    QSPI_XIP_Enable(QSPI_XIP0, ENABLE);
    memcpy((void*)QSPI_XIP0_MEM_BASE, write, PAGESIZE);

    Xip_RxTimingConfig(PSRAM_FAST_READ_QUAD, QSPI_XIP_DUAL_LINE);
    QSPI_XIP_Enable(QSPI_XIP0, ENABLE);
    memcpy(read, (void*)QSPI_XIP0_MEM_BASE, PAGESIZE);
    if (0 == memcmp(write, read, 0)) {
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
    SPI_ExitQuad();
}

void main(void)
{
    #ifdef MISC_HAS_QSPI_XIP0_HAS_CLK
    qspi_xip0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_QSPI_XIP0_RST
    qspi_xip0_set_rst(DISABLE);
    qspi_xip0_set_rst(ENABLE);
    #endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
#endif

    spi_normal_mode_test();

    spi_xip_mode_test();

    SPI_Init();
    simulation_pass();

    while (1) {}
}

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

#define SPI_CS_ON  (QSPI_CSModeConfig(QSPI3, QSPI_CSMODE_MODE_HOLD))
#define SPI_CS_OFF (QSPI_CSModeConfig(QSPI3, QSPI_CSMODE_MODE_OFF))

#define PAGESIZE 256
#define Dummy_Byte                0xFF

/**
  * \brief configure the SPI peripheral
  */
void SPI_Init(void)
{
    QSPI_InitTypeDef spi_init_struct = {0};
    /* deinitilize SPI and the parameters */
    QSPI_StructInit(&spi_init_struct);
    
    spi_init_struct.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    spi_init_struct.DataSize = QSPI_FMT_LEN_8B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_LOW;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE1;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    
    QSPI_Init(QSPI3, &spi_init_struct);
}

void SPI_WriteByte(uint8_t txdata)
{
    QSPI_DirectionConfig(QSPI3, QSPI_FMT_DIR_TX);
    while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_TX_FULL)) {}
    QSPI_SendData(QSPI3, txdata);
    while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_BUSY)) {}
}

uint8_t SPI_ReadByte(void)
{
    uint8_t rxdata = 0;
    QSPI_DirectionConfig(QSPI3, QSPI_FMT_DIR_RX);
    QSPI_SendData(QSPI3, Dummy_Byte);
    while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_RX_EMPTY)) {}
    rxdata = QSPI_ReceiveData(QSPI3) & 0xFF;
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
    QSPI3->FMT |= QSPI_FMT_PROTO_QUAD;
    SPI_WriteByte(PSRAM_EXIT_QMODE);
    SPI_CS_OFF;
}

void SPI_PageWrite(uint8_t cmd, uint8_t* buf, uint32_t addr)
{
    SPI_CS_ON;
    uint32_t value = QSPI3->FMT;
    value &= ~QSPI_FMT_PROTO_MASK;
    if(PSRAM_QUAD_WRITE == cmd) {
        QSPI3->FMT = value | QSPI_FMT_PROTO_QUAD;
    } else if(PSRAM_WRITE == cmd) {
        QSPI3->FMT = value | QSPI_FMT_PROTO_SINGLE;
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
    uint32_t value = QSPI3->FMT;
    value &= ~QSPI_FMT_PROTO_MASK;
    if(PSRAM_FAST_READ_QUAD == cmd){
        QSPI3->FMT = value | QSPI_FMT_PROTO_QUAD;
    }else if(PSRAM_READ == cmd){
        QSPI3->FMT = value | QSPI_FMT_PROTO_SINGLE;
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

void print_data(uint8_t* data)
{
    for (int i = 0; i < 16; i++) {
        printf("0x%x ", data[i]);
    }
    printf("\r\n");
}

void iomux_config(void)
{
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_SCK_IOF_OVAL, QSPI3_SCK_PAD_SEL, QSPI3_SCK_HS_SEL, 0, 0);
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_CS_0_IOF_OVAL, QSPI3_CS_0_PAD_SEL, QSPI3_CS_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_0_IOF_IVAL, QSPI3_DQ_0_PAD_SEL, QSPI3_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_0_IOF_OVAL, QSPI3_DQ_0_PAD_SEL, QSPI3_DQ_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_1_IOF_IVAL, QSPI3_DQ_1_PAD_SEL, QSPI3_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_1_IOF_OVAL, QSPI3_DQ_1_PAD_SEL, QSPI3_DQ_1_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_2_IOF_IVAL, QSPI3_DQ_2_PAD_SEL, QSPI3_DQ_2_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_2_IOF_OVAL, QSPI3_DQ_2_PAD_SEL, QSPI3_DQ_2_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_3_IOF_IVAL, QSPI3_DQ_3_PAD_SEL, QSPI3_DQ_3_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_3_IOF_OVAL, QSPI3_DQ_3_PAD_SEL, QSPI3_DQ_3_HS_SEL, 0, 0);
}

void main(void)
{
    uint8_t write[PAGESIZE] = {0};
    uint8_t read[PAGESIZE] = {0};

    #ifdef MISC_HAS_QSPI3_HAS_CLK
    qspi3_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_QSPI3_RST
    qspi3_set_rst(DISABLE);
    qspi3_set_rst(ENABLE);
    #endif

    iomux_config();

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

    QSPI3->FORCE &= ~(QSPI_FORCE_EN | QSPI_FORCE_WP);
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

    SPI_Init();
    SPI_ExitQuad();
    simulation_pass();

    while (1) {}
}

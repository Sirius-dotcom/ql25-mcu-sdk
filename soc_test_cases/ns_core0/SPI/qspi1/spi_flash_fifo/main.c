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
#include "nor_flash.h"

#define     FLASH_JEDECID       0XEF7019    
#define     FLASH_PAGESIZE      256
#define     FLASH_ADDR          0x000000

/* WIP(busy) flag */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF

#define SPI_CS_ON  (QSPI_CSModeConfig(QSPI1, QSPI_CSMODE_MODE_HOLD))
#define SPI_CS_OFF (QSPI_CSModeConfig(QSPI1, QSPI_CSMODE_MODE_OFF))

#define FIFO_DP QSPI_BUF_DEPTH
#define TX_WATERMARK (FIFO_DP - 2)

uint8_t write_buf[FLASH_PAGESIZE] = {0};
uint8_t read_buf[FLASH_PAGESIZE] = {0};

volatile uint32_t i_tx = 0;
volatile uint32_t i_rx = 0;
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
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_16;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    
    QSPI_Init(QSPI1, &spi_init_struct);
    
    QSPI_TxmarkConfig(QSPI1, 1);
}

void SPI_PageWrite(uint8_t cmd, uint8_t* buf, uint32_t addr)
{

    Spi_NorFlash_Write_Enable(QSPI1);
    SPI_CS_ON;
    SPI_WriteByte(QSPI1, cmd);
    SPI_WriteByte(QSPI1, addr >> 16);
    SPI_WriteByte(QSPI1, addr >> 8);
    SPI_WriteByte(QSPI1, addr);

    QSPI_TxsizeConfig(QSPI1, FLASH_PAGESIZE);
    
    for(int i = 0; i < FIFO_DP; i++) {
        QSPI1->TXDATA = write_buf[i];
    }

    for(int j = FIFO_DP; j < FLASH_PAGESIZE ; j++) {
        while (QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_FULL)) {}
        QSPI1->TXDATA = write_buf[j];
    }
    while(!QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_DONE)){}

    SPI_CS_OFF;
    Spi_NorFlash_Wait_Busy(QSPI1);
}

void SPI_PageRead(uint8_t cmd, uint8_t* buf, uint32_t addr)
{
    QSPI_ForceEnable(QSPI1, DISABLE, DISABLE);
    Spi_NorFlash_EnableQuadMode_WQ25xx(QSPI1);
    SPI_CS_ON;
    SPI_WriteByte(QSPI1, cmd);
    SPI_WriteByte(QSPI1, addr >> 16);
    SPI_WriteByte(QSPI1, addr >> 8);
    SPI_WriteByte(QSPI1, addr);
    SPI_WriteByte(QSPI1, Dummy_Byte);

    QSPI_BusWidthSwitch(QSPI1, QSPI_FMT_PROTO_QUAD);
    QSPI_DirectionConfig(QSPI1, QSPI_FMT_DIR_RX);

    QSPI_RxsizeConfig(QSPI1, FLASH_PAGESIZE);
    
    for(int j = 0; j < FIFO_DP; j++) {
        QSPI1->TXDATA = Dummy_Byte; 
        
    }

    for (int i = 0; i < FLASH_PAGESIZE; i++) {
        while (QSPI1->STATUS & QSPI_STATUS_RX_EMPTY);
        buf[i] = (QSPI1->RXDATA & 0xFF);
        if(i < FLASH_PAGESIZE - FIFO_DP) {
            QSPI1->TXDATA = Dummy_Byte; 
        }
    }

    while(!QSPI_GetFlag(QSPI1, QSPI_STATUS_RX_DONE)) {}

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
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_SCK_IOF_OVAL, QSPI1_SCK_PAD_SEL, QSPI1_SCK_HS_SEL, 0, 0);
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_CS_0_IOF_OVAL, QSPI1_CS_0_PAD_SEL, QSPI1_CS_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_IVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_OVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_IVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_OVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_2_IOF_IVAL, QSPI1_DQ_2_PAD_SEL, QSPI1_DQ_2_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_2_IOF_OVAL, QSPI1_DQ_2_PAD_SEL, QSPI1_DQ_2_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_3_IOF_IVAL, QSPI1_DQ_3_PAD_SEL, QSPI1_DQ_3_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_3_IOF_OVAL, QSPI1_DQ_3_PAD_SEL, QSPI1_DQ_3_HS_SEL, 0, 0);
}

void QSPI1_IRQHandler(void)
{
    uint32_t ip_st = QSPI1->IP;
    uint32_t ie_st = QSPI1->IE;
    switch (ie_st)
    {
        case QSPI_IE_RX:
            if (ip_st & QSPI_IP_RX) {
                read_buf[i_rx] = (QSPI1->RXDATA & 0xFF);
                i_rx ++;
                read_buf[i_rx] = (QSPI1->RXDATA & 0xFF);
                i_rx ++;
                read_buf[i_rx] = (QSPI1->RXDATA & 0xFF);
                i_rx ++;
                read_buf[i_rx] = (QSPI1->RXDATA & 0xFF);
                i_rx ++;
                if(i_rx == FLASH_PAGESIZE) {
                    QSPI_ITConfig(QSPI1, QSPI_IE_RX, DISABLE);
                }
            }
            break;
        case QSPI_IE_TX:
            if (ip_st & QSPI_IP_TX) {
                for(int i =0;i<TX_WATERMARK;i++) {
                    QSPI1->TXDATA = write_buf[i_tx];
                    i_tx ++;
                }
                if(i_tx == FLASH_PAGESIZE) {
                    QSPI_ITConfig(QSPI1, QSPI_IE_TX, DISABLE);
                }
            }
        
        default:
            break;
    }
    
}

void main(void)
{

#ifdef MISC_HAS_QSPI1_HAS_CLK
    qspi1_clk_en(ENABLE);
#endif
#ifdef MISC_HAS_QSPI1_CLK_DIV
    qspi1_clk_div(0);
#endif
#ifdef MISC_HAS_QSPI1_RST
    qspi1_set_rst(DISABLE);
    qspi1_set_rst(ENABLE);
#endif

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt QSPI1_IRQn */
    ECLIC_Register_IRQ(QSPI1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    QSPI1_IRQHandler);

    iomux_config();

    for (int i = 0; i < FLASH_PAGESIZE; i++) {
        write_buf[i] = i;
    }

    SPI_Init();
    Spi_NorFlash_ReadJEDECID(QSPI1);
    Spi_NorFlash_Erase_Sector(QSPI1, FLASH_ADDR);

    SPI_PageWrite(FLASH_PageProgram, write_buf, 0x0);
    SPI_PageRead(FLASH_FastReadQuad, read_buf, 0x0);
    if (0 == memcmp(write_buf, read_buf, FLASH_PAGESIZE)) {
        printf("data cmp pass\n");
    } else {
        printf("data cmp error\n");
        simulation_fail();
        printf("write_buf\n");
        print_data(write_buf);
        printf("read_buf\n");
        print_data(read_buf);
        while (1) {}
    }

    SPI_Init();
    simulation_pass();

    while (1) {}
}

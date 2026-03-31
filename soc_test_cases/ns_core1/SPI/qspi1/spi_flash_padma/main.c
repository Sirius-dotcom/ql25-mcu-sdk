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

uint32_t *dst_base_addr;
uint32_t *src_base_addr;

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
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_4;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    spi_init_struct.DevMode = (QSPI_CR_MODE_MASTER | QSPI_CR_DMA_ENABLE);
    
    QSPI_Init(QSPI1, &spi_init_struct);
}

void SPI_PageWrite_Dma(uint8_t cmd, uint32_t addr)
{
    Spi_NorFlash_Write_Enable(QSPI1);
    Spi_NorFlash_CS(QSPI1, CS_ON);
    SPI_WriteByte(QSPI1, cmd);
    SPI_WriteByte(QSPI1, addr >> 16);
    SPI_WriteByte(QSPI1, addr >> 8);
    SPI_WriteByte(QSPI1, addr);

    UDMA_PAM2MTypeDef UDMA_PAM2MStruct = {0};
    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(&(QSPI1->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(src_base_addr);
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(&(QSPI1->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(src_base_addr);
#endif
    UDMA_PAM2MStruct.UDMA_BufferSize = FLASH_PAGESIZE;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_8BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_QSPI1_TX_DMA;  
    UDMA_PAM2M_Init(QSPI1_TX_DMA_DMA_CH, &UDMA_PAM2MStruct);

    QSPI_TxDmaStart(QSPI1, ENABLE);

    while (!UDMA_PA2M_GetITStatus(QSPI1_TX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_STAT)) {}
    while (QSPI_GetFlag(QSPI1,QSPI_STATUS_BUSY) == SET) {}
    Spi_NorFlash_CS(QSPI1, CS_OFF);
    QSPI_TxDmaStart(QSPI1, DISABLE);
    Spi_NorFlash_Wait_Busy(QSPI1);
}

void SPI_PageRead_Dma(uint8_t cmd, uint32_t addr)
{
    uint32_t bus_width;
    Spi_NorFlash_CS(QSPI1, CS_ON);
    SPI_WriteByte(QSPI1, cmd);
    SPI_WriteByte(QSPI1, addr >> 16);
    SPI_WriteByte(QSPI1, addr >> 8);
    SPI_WriteByte(QSPI1, addr);
    SPI_WriteByte(QSPI1, DUMMY_BYTE);

    uint32_t value = QSPI1->FMT;
    value &= ~QSPI_FMT_PROTO_MASK;
    if (FLASH_FastReadDual == cmd) {
        bus_width = QSPI_FMT_PROTO_DUAL;
    }
    if (FLASH_FastReadQuad == cmd) {
        bus_width = QSPI_FMT_PROTO_QUAD;
    }

    QSPI_BusWidthSwitch(QSPI1, bus_width);
    QSPI_DirectionConfig(QSPI1, QSPI_FMT_DIR_RX);

    UDMA_PAM2MTypeDef UDMA_PAM2MStruct = {0};
    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(dst_base_addr);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(&(QSPI1->RXDATA));
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(dst_base_addr);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(&(QSPI1->RXDATA));
#endif
    UDMA_PAM2MStruct.UDMA_BufferSize = FLASH_PAGESIZE;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_8BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_QSPI1_RX_DMA;  
    UDMA_PAM2M_Init(QSPI1_RX_DMA_DMA_CH, &UDMA_PAM2MStruct);

    /* for send sck */
    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(&(QSPI1->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(src_base_addr);
#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(&(QSPI1->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(src_base_addr);
#endif
    UDMA_PAM2MStruct.UDMA_BufferSize = FLASH_PAGESIZE;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_8BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_QSPI1_TX_DMA;
    UDMA_PAM2M_Init(QSPI1_TX_DMA_DMA_CH, &UDMA_PAM2MStruct);

    QSPI_RxDmaStart(QSPI1, ENABLE);
    QSPI_TxDmaStart(QSPI1, ENABLE);

    while (!UDMA_PA2M_GetITStatus(QSPI1_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_STAT)) {}
    Spi_NorFlash_CS(QSPI1, CS_OFF);
    QSPI_TxDmaStart(QSPI1, DISABLE);
    QSPI_RxDmaStart(QSPI1, DISABLE);
}

void print_data(uint32_t* data)
{
    for (int i = 0; i < FLASH_PAGESIZE/4; i++) {
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

#ifdef MISC_HAS_UDMA0_HAS_CLK
    udma0_clk_en(ENABLE);
#endif
#ifdef MISC_HAS_UDMA0_RST
    udma0_set_rst(DISABLE);
    udma0_set_rst(ENABLE);
#endif

#ifdef MISC_HAS_UDMA0_CLK_DIV
    udma0_clk_div(0);
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    MFlushInvalDCache();
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
#endif

    dst_base_addr = (uint32_t *)(0x9000000 + 32768 - 0x200);
    src_base_addr = (uint32_t *)(0x9000000 + 32768 - 0x100);

    uint32_t *src_base_addr_tmp = src_base_addr;

    for (int i = 0; i < FLASH_PAGESIZE/4; i++) {
        *src_base_addr_tmp++ = 0x12345678 + i;
    }

    iomux_config();

    UDMA_PA2M_ITConfig(QSPI1_RX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);
    UDMA_PA2M_ITConfig(QSPI1_TX_DMA_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);

    SPI_Init();
    Spi_NorFlash_ReadJEDECID(QSPI1);
    Spi_NorFlash_Erase_Sector(QSPI1, FLASH_ADDR);

    SPI_PageWrite_Dma(FLASH_PageProgram, FLASH_ADDR);

    QSPI_ForceEnable(QSPI1, DISABLE, DISABLE);
    Spi_NorFlash_EnableQuadMode_WQ25xx(QSPI1);

    SPI_PageRead_Dma(FLASH_FastReadQuad, FLASH_ADDR);
    if (0 == memcmp(dst_base_addr, src_base_addr, FLASH_PAGESIZE/4)) {
        printf("4 line data cmp pass\n");
    } else {
        printf("4 line data cmp error\n");
        simulation_fail();
        printf("dst_base_addr : ");
        print_data(dst_base_addr);
        printf("src_base_addr  : ");
        print_data(src_base_addr);
        while (1) {}
    }

    SPI_Init();
    simulation_pass();

    while (1) {}
}

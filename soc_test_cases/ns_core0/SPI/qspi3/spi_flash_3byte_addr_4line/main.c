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

/**
  * \brief configure the SPI peripheral
  */
void SPI_Init(void)
{
    QSPI_InitTypeDef spi_init_struct = {0};
    /* deinitilize SPI and the parameters */
    QSPI_StructInit(&spi_init_struct);
    /* QSPI2 parameter config */ /*Adjust the init sequence to prevent the "x" state */
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
    uint8_t write_buf[FLASH_PAGESIZE] = {0};
    uint8_t read_buf[FLASH_PAGESIZE] = {0};

#ifdef MISC_HAS_QSPI3_HAS_CLK
    qspi3_clk_en(ENABLE);
#endif
#ifdef MISC_HAS_QSPI3_CLK_DIV
    qspi3_clk_div(0);
#endif
#ifdef MISC_HAS_QSPI3_RST
    qspi3_set_rst(DISABLE);
    qspi3_set_rst(ENABLE);
#endif

    iomux_config();

    for (int i = 0; i < FLASH_PAGESIZE; i++) {
        write_buf[i] = i;
    }

    SPI_Init();

    Spi_NorFlash_ReadJEDECID(QSPI3);
    Spi_NorFlash_Erase_Sector(QSPI3, FLASH_ADDR);

    QSPI_ForceEnable(QSPI3, DISABLE, DISABLE);
    Spi_NorFlash_EnableQuadMode_WQ25xx(QSPI3);
    Spi_NorFlash_Write_Page(QSPI3, FLASH_PageProgramQuad, write_buf, FLASH_ADDR, FLASH_PAGESIZE);

    Spi_NorFlash_Read(QSPI3, FLASH_FastReadQuad, read_buf, FLASH_ADDR, FLASH_PAGESIZE);

    if (0 == memcmp(write_buf, read_buf, FLASH_PAGESIZE)) {
        printf("3byte addr 4line mode data cmp pass\n");
    } else {
        printf("3byte addr 4line mode data cmp error\n");
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

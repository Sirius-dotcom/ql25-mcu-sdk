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
#include "ns_sdk_hal.h"
#include "nor_flash.h"
#include "ff.h"
#include "diskio.h"

#define FATFS_WR_SIZE 1024 * 8

char SD_FileName[] = "hello.txt";
uint8_t write_cnt =0;
uint8_t WriteBuffer[] = "01 write buff to sd\r\n";

__attribute__((aligned(4))) uint8_t buffer_all[FATFS_WR_SIZE];
void copyfile(uint8_t *srcfilename, uint8_t *destfilename)
{

    UINT br, bw;
    uint32_t f_res;
    FIL file_src;
    FIL file_dest;

    f_res = f_open(&file_src, srcfilename,
                    FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
    if (f_res == FR_OK) {
        f_res = f_open(&file_dest, destfilename,
                        FA_CREATE_ALWAYS | FA_WRITE | FA_READ | FA_OPEN_ALWAYS);
        if (f_res != FR_OK) {
            return;
        }
    }
    printf("start copy file.... \r\n");
    while (f_res == 0) {
        f_res = f_read(&file_src, buffer_all, FATFS_WR_SIZE, &br);
        printf("f_res = %d,br= %d   ", f_res, br);
        if (f_res || br == 0)
            break;
        f_res = f_write(&file_dest, buffer_all, br, &bw);
        printf("f_res = %d,br= %d,bw=%d \r\n", f_res, br, bw);
        if (f_res || bw < br)
            break;
    }
    f_close(&file_src);
    f_close(&file_dest);
    printf("\r\ncopyfile finish\r\n");
}

/**
  * \brief configure the SPI peripheral
  */
void SPI_Config(void)
{
    QSPI_InitTypeDef spi_init_struct;
    /* deinitilize SPI and the parameters */
    QSPI_StructInit(&spi_init_struct);
    /* QSPI3 parameter config */
    spi_init_struct.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    spi_init_struct.DataSize = QSPI_FMT_LEN_8B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_LOW;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE1;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_64;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    /* QSPI3 parameter config */
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    QSPI_Init(QSPI3, &spi_init_struct);
    QSPI3->CSMODE |= QSPI_CSMODE_MODE_HOLD;
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
    FIL file;
    FATFS fatfs;
    static FRESULT res;
    UINT Bw;

    BYTE work[FF_MAX_SS]; /* Working buffer */

    MKFS_PARM fs_parm = {
        /* filesystem parameter: format = FAT32, other use default val */
        .fmt = FS_FAT32, .n_fat = 0, .au_size = 0, .align = 0, .n_root = 0,
    };

#ifdef MISC_HAS_QSPI3_HAS_CLK
    qspi3_clk_en(ENABLE);
#endif
#ifdef MISC_HAS_QSPI3_RST
    qspi3_set_rst(DISABLE);
    qspi3_set_rst(ENABLE);
#endif

    iomux_config();

    SPI_Config();

    printf("start spi-sdcard\r\n");

    res = f_mount(&fatfs, "0:", 1);

    if (res == FR_NO_FILESYSTEM) {
        printf("flahs has no file system, start make filesystem...\n");
        res = f_mkfs((const TCHAR *)"0:", &fs_parm, work, sizeof(work));
        if (res != FR_OK) {
            printf("f_mkfs fail : %d!\r\n", res);
            simulation_fail();
        } else {
            printf("f_mkfs successful!\n");
            res = f_mount(&fatfs, "0:", 1);
        }
    }
    else if(res == FR_OK){
        printf("mount success! \r\n");
    }
    else{
        printf("mount fail! \r\n");
    }

    f_setlabel((const TCHAR *)"0:Nuclei");     

    res = f_open(&file, SD_FileName, FA_OPEN_ALWAYS | FA_WRITE); 
    if((res & FR_DENIED) == FR_DENIED)
    {
        printf("Card storage is full, write failed!\r\n");
    }

    f_lseek(&file, f_size(&file));
    if(res == FR_OK)
    {
        printf("Open successfully/Create file successfully!\r\n");
        res = f_write(&file, WriteBuffer, sizeof(WriteBuffer), &Bw);
        if(res == FR_OK)
        {
            printf("file was written successfully! \r\n");
        }
        else
        {
            printf("file was written fail! \r\n");
        }
    }
    else
    {
        printf("fail to open the file!\r\n");
    }

    f_close(&file);

    copyfile("0:/hello.txt", "0:/hello_cp.txt");

    simulation_pass();

    while (1) {}
}

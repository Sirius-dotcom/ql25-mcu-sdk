/* USER CODE BEGIN Header */

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
 /* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "diskio.h"        /* Declarations of disk functions */
#include "ns_sdk_hal.h"
#include "nor_flash.h"

static volatile DSTATUS Stat = STA_NOINIT;

/* Private functions ---------------------------------------------------------*/

#define EX_FLASH  0

#define SPI_FLASH_SECTOR_SIZE     512
#define SPI_FLASH_SECTOR_COUNT    4096
#define SPI_FLASH_BLOCK_SIZE      1

DSTATUS disk_status (
    BYTE pdrv                /* Physical drive nmuber to identify the drive */
)
{
    return RES_OK;
}

DSTATUS disk_initialize (
    BYTE pdrv                /* Physical drive nmuber to identify the drive */
)
{
    uint8_t res=0;
    switch(pdrv)
    {
        case EX_FLASH:
            Spi_NorFlash_Init(QSPI3);
            break;
        default:
            res=1;
    }
    if(res)return  STA_NOINIT;
    else return 0;
}

DRESULT disk_read (
    BYTE pdrv,        /* Physical drive nmuber to identify the drive */
    BYTE *buff,        /* Data buffer to store read data */
    DWORD sector,    /* Sector address in LBA */
    UINT count        /* Number of sectors to read */
)
{
    uint8_t res=0;
    if (!count)return RES_PARERR;

    switch(pdrv)
    {
        case EX_FLASH:
            for(;count>0;count--)
            {
                Spi_NorFlash_Read(QSPI3, FLASH_FastReadData, buff, sector*SPI_FLASH_SECTOR_SIZE, SPI_FLASH_SECTOR_SIZE);
                sector++;
                buff += SPI_FLASH_SECTOR_SIZE;
            }
            res=0;
            break;

        default:
            res=1;
    }
    if(res == 0x00)return RES_OK;
    else return RES_ERROR;
}

DRESULT disk_write (
    BYTE pdrv,            /* Physical drive nmuber to identify the drive */
    const BYTE *buff,    /* Data to be written */
    DWORD sector,        /* Sector address in LBA */
    UINT count            /* Number of sectors to write */
)
{
    uint8_t res=0;
    if (!count)return RES_PARERR;

    switch(pdrv)
    {

        case EX_FLASH:
            for(;count>0;count--)
            {
                Spi_NorFlash_Write(QSPI3, FLASH_PageProgram, (uint8_t*)buff, sector*SPI_FLASH_SECTOR_SIZE, SPI_FLASH_SECTOR_SIZE);
                sector++;
                buff+=SPI_FLASH_SECTOR_SIZE;
            }
            res=0;
            break;

        default:
            res=1;
    }
    if(res == 0x00) return RES_OK;
    else return RES_ERROR;
}

DRESULT disk_ioctl (
    BYTE pdrv,        /* Physical drive nmuber (0..) */
    BYTE cmd,        /* Control code */
    void *buff        /* Buffer to send/receive control data */
)
{
DRESULT res;
    if(pdrv==EX_FLASH)
    {
        switch(cmd)
        {
            case CTRL_SYNC:
                res = RES_OK;
                break;
            case GET_SECTOR_SIZE:
                *(WORD*)buff = SPI_FLASH_SECTOR_SIZE;
                res = RES_OK;
                break;
            case GET_BLOCK_SIZE:
                *(WORD*)buff = SPI_FLASH_BLOCK_SIZE;
                res = RES_OK;
                break;
            case GET_SECTOR_COUNT:
                *(DWORD*)buff = SPI_FLASH_SECTOR_COUNT;
                res = RES_OK;
                break;
            default:
                res = RES_PARERR;
                break;
        }
    }else res = RES_ERROR;
    return res;
}

DWORD get_fattime (void)
{
    return 0;
}

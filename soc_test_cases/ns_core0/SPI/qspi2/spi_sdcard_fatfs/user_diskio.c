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

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/* 
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future. 
 * Kept to ensure backward compatibility with previous CubeMx versions when 
 * migrating projects. 
 * User code previously added there should be copied in the new user sections before 
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "diskio.h"        /* Declarations of disk functions */
#include "ns_sdk_hal.h"
#include "spi_sdcard.h"

/** 
  * @brief  Disk IO Driver structure definition  
  */ 
typedef struct
{
  DSTATUS (*disk_initialize) (BYTE);                     /*!< Initialize Disk Drive                     */
  DSTATUS (*disk_status)     (BYTE);                     /*!< Get Disk Status                           */
  DRESULT (*disk_read)       (BYTE, BYTE*, DWORD, UINT);       /*!< Read Sector(s)                            */
#if _USE_WRITE == 1 
  DRESULT (*disk_write)      (BYTE, const BYTE*, DWORD, UINT); /*!< Write Sector(s) when _USE_WRITE = 0       */
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1  
  DRESULT (*disk_ioctl)      (BYTE, BYTE, void*);              /*!< I/O control operation when _USE_IOCTL = 1 */
#endif /* _USE_IOCTL == 1 */

}Diskio_drvTypeDef;

/** 
  * @brief  Global Disk IO Drivers structure definition  
  */ 
typedef struct
{ 
    uint8_t                 is_initialized[FF_VOLUMES];
    Diskio_drvTypeDef       *drv[FF_VOLUMES];
    uint8_t                 lun[FF_VOLUMES];
    __IO uint8_t            nbr;

}Disk_drvTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t FATFS_LinkDriverEx(Diskio_drvTypeDef *drv, char *path, uint8_t lun);
uint8_t FATFS_LinkDriver(Diskio_drvTypeDef *drv, char *path);
uint8_t FATFS_UnLinkDriver(char *path);
uint8_t FATFS_LinkDriverEx(Diskio_drvTypeDef *drv, char *path, BYTE lun);
uint8_t FATFS_UnLinkDriverEx(char *path, BYTE lun);
uint8_t FATFS_GetAttachedDriversNbr(void);

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS disk_initialize (BYTE pdrv);
DSTATUS disk_status (BYTE pdrv);
DRESULT disk_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT disk_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
  disk_initialize,
  disk_status,
  disk_read, 
#if  _USE_WRITE
  disk_write,
#endif  /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  disk_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_initialize (
    BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
    /* USER CODE BEGIN INIT */
    uint8_t res;
    res = SD_init(QSPI2);
            if(res)
            {
                SPI_setspeed(QSPI2, QSPI_SCKDIV_PRESCALER_64);
                spi_readwrite(QSPI2,0xff);
                SPI_setspeed(QSPI2, QSPI_SCKDIV_PRESCALER_8);
            }
    if(res)return  STA_NOINIT;
    else return RES_OK;
  /* USER CODE END INIT */
}

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_status (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
  /* USER CODE BEGIN STATUS */
  switch (pdrv)
    {
        case 0 :
            return RES_OK;
        case 1 :
            return RES_OK;
        case 2 :
            return RES_OK;
        default:
            return STA_NOINIT;
    }
  /* USER CODE END STATUS */
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT disk_read (
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address in LBA */
    UINT count      /* Number of sectors to read */
)
{
  /* USER CODE BEGIN READ */
    uint8_t res;
    if( !count )
    {
        return RES_PARERR;
    }
    switch (pdrv)
    {
        case 0:
            res=SD_ReadDisk(QSPI2,buff,sector,count);
                if(res == 0){
                    return RES_OK;
                }else{
                    return RES_ERROR;
                }
        default:
            return RES_ERROR;
    }
  /* USER CODE END READ */
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT disk_write (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address in LBA */
    UINT count          /* Number of sectors to write */
)
{
  /* USER CODE BEGIN WRITE */
  /* USER CODE HERE */
    uint8_t  res;
    if( !count )
    {
        return RES_PARERR;  /* count²»ÄÜµÈÓÚ0£¬·ñÔò·µ»Ø²ÎÊý´íÎó */
    }
    switch (pdrv)
    {
        case 0:
            res=SD_WriteDisk(QSPI2, (uint8_t *)buff,sector,count);
                if(res == 0){
                    return RES_OK;
                }else{
                    return RES_ERROR;
                }
        default:return RES_ERROR;
    }
  /* USER CODE END WRITE */
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
  /* USER CODE BEGIN IOCTL */
    DRESULT res;
    switch(cmd)
        {
            case CTRL_SYNC:
                        SD_CS(QSPI2, 1);
                        do{
                            delay_1ms(20);
                        }while(spi_readwrite(QSPI2, 0xFF)!=0xFF);
                        res=RES_OK;
                        SD_CS(QSPI2, 0);
                break;
            case GET_SECTOR_SIZE:
                *(WORD*)buff = 512;
                res = RES_OK;
                break;
            case GET_BLOCK_SIZE:
                *(WORD*)buff = 8;
                res = RES_OK;
                break;
            case GET_SECTOR_COUNT:
                *(DWORD*)buff = SD_GetSectorCount(QSPI2);
                res = RES_OK;
                break;
            default:
                res = RES_PARERR;
                break;
        }
        return res;
  /* USER CODE END IOCTL */
}
#endif /* _USE_IOCTL == 1 */

DWORD get_fattime (void)
{
	return 0;
}

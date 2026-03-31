/*
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

/* Includes ------------------------------------------------------------------*/
#include "spi_sdcard.h"
#include "ns_qspi.h"

#define DUMMY_BYTE  0xFF

uint8_t test;
uint8_t SD_TYPE=0x00;

MSD_CARDINFO SD0_CardInfo;

void SD_CS(QSPI_TypeDef* QSPIx, uint8_t p)
{
    if(p == 0){
        QSPI_CSEnable(QSPIx, QSPI_CSID_NUM_CS0, DISABLE);
    }else{
        QSPI_CSEnable(QSPIx, QSPI_CSID_NUM_CS0, ENABLE);
    }
}

int SD_sendcmd(QSPI_TypeDef* QSPIx, uint8_t cmd,uint32_t arg,uint8_t crc)
{
    uint8_t r1;
    uint8_t retry;

    SD_CS(QSPIx, 0);
    delay_1ms(1);
    SD_CS(QSPIx, 1);
    do{
        retry=spi_readwrite(QSPIx, DUMMY_BYTE);
    }while(retry!=0xFF);

    spi_readwrite(QSPIx, cmd | 0x40);
    spi_readwrite(QSPIx, arg >> 24);
    spi_readwrite(QSPIx, arg >> 16);
    spi_readwrite(QSPIx, arg >> 8);
    spi_readwrite(QSPIx, arg);
    spi_readwrite(QSPIx, crc);

    if(cmd==CMD12)spi_readwrite(QSPIx, DUMMY_BYTE);

    do{
        r1=spi_readwrite(QSPIx, 0xFF);
    }while(r1&0X80);

    return r1;
}

uint8_t SD_init(QSPI_TypeDef* QSPIx)
{
    uint8_t r1;
    uint8_t buff[6] = {0};
    uint16_t retry;
    uint8_t i;

    SPI_setspeed(QSPIx, QSPI_SCKDIV_PRESCALER_64);
    SD_CS(QSPIx, 0);
    for(retry=0;retry<10;retry++){
            spi_readwrite(QSPIx, DUMMY_BYTE);
    }

    do{
        r1 = SD_sendcmd(QSPIx, CMD0 ,0, 0x95);
    }while(r1!=0x01);

    SD_TYPE=0;
    r1 = SD_sendcmd(QSPIx, CMD8, 0x1AA, 0x87);
    if(r1==0x01){
        for(i=0;i<4;i++)buff[i]=spi_readwrite(QSPIx, DUMMY_BYTE);    
        if(buff[2]==0X01&&buff[3]==0XAA)
        {
            retry=0XFFFE;
            do
            {
                SD_sendcmd(QSPIx, CMD55,0,0X01);    
                r1=SD_sendcmd(QSPIx, CMD41,0x40000000,0X01);
            }while(r1&&retry--);
            
            if(retry&&SD_sendcmd(QSPIx, CMD58,0,0X01)==0)
            {
                
                for(i=0;i<4;i++)buff[i]=spi_readwrite(QSPIx, 0XFF);
                if(buff[0]&0x40){
                    SD_TYPE=V2HC;
                }else {
                    SD_TYPE=V2;
                }
            }
        }else{
            SD_sendcmd(QSPIx, CMD55,0,0X01);
            r1=SD_sendcmd(QSPIx, CMD41,0,0X01);
            if(r1<=1)
            {
                SD_TYPE=V1;
                retry=0XFFFE;
                
                do
                {
                    SD_sendcmd(QSPIx, CMD55,0,0X01);
                    r1=SD_sendcmd(QSPIx, CMD41,0,0X01);
                }while(r1&&retry--);
            }else
            {
                SD_TYPE=MMC;
                retry=0XFFFE;
                do
                {
                    r1=SD_sendcmd(QSPIx, CMD1,0,0X01);
                }while(r1&&retry--);
            }
            if(retry==0||SD_sendcmd(QSPIx, CMD16,512,0X01)!=0)SD_TYPE=ERR;
        }
    }
    SD_CS(QSPIx, 0);
    SPI_setspeed(QSPIx, QSPI_SCKDIV_PRESCALER_8);
    if(SD_TYPE)return 0;
    else return 1;
}

uint8_t SD_ReceiveData(QSPI_TypeDef* QSPIx, uint8_t *data, uint16_t len)
{
    uint8_t r1;
    SD_CS(QSPIx, 1);
    do
    {
        r1 = spi_readwrite(QSPIx, 0xFF);
        delay_1ms(100);
        }while(r1 != 0xFE);
    while(len--)
    {
    *data = spi_readwrite(QSPIx, 0xFF);
    data++;
    }
    spi_readwrite(QSPIx, 0xFF);
    spi_readwrite(QSPIx, 0xFF);
    return 0;
}

uint8_t SD_SendBlock(QSPI_TypeDef* QSPIx, uint8_t*buf,uint8_t cmd)
{
    uint16_t t;
    uint8_t r1;
    do{
        r1=spi_readwrite(QSPIx, 0xFF);
    }while(r1!=0xFF);

    spi_readwrite(QSPIx, cmd);
    if(cmd!=0XFD)
    {
        for(t=0;t<512;t++)spi_readwrite(QSPIx, buf[t]);
        spi_readwrite(QSPIx, 0xFF);
        spi_readwrite(QSPIx, 0xFF);
        t=spi_readwrite(QSPIx, 0xFF);
        if((t&0x1F)!=0x05)return 2;
    }
    return 0;
}

uint8_t SD_GETCID (QSPI_TypeDef* QSPIx, uint8_t *cid_data)
{
    uint8_t r1;
    r1=SD_sendcmd(QSPIx, CMD10,0,0x01);
    if(r1==0x00){
        r1=SD_ReceiveData(QSPIx, cid_data,16);
    }
    SD_CS(QSPIx, 0);
    if(r1)return 1;
    else return 0;
}

uint8_t SD_GETCSD(QSPI_TypeDef* QSPIx, uint8_t *csd_data)
{
    uint8_t r1;
    r1=SD_sendcmd(QSPIx, CMD9,0,0x01);
    if(r1==0)
    {
        r1=SD_ReceiveData(QSPIx, csd_data, 16);
    }
    SD_CS(QSPIx, 0);
    if(r1)return 1;
    else return 0;
}

uint32_t SD_GetSectorCount(QSPI_TypeDef* QSPIx)
{
    uint8_t csd[16];
    uint32_t Capacity;
    uint8_t n;
    uint16_t csize;
    if(SD_GETCSD(QSPIx, csd)!=0) return 0;
    
    if((csd[0]&0xC0)==0x40)     
    {
        csize = csd[9] + ((uint16_t)csd[8] << 8) + 1;
        Capacity = (uint32_t)csize << 10;
    }else
    {
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
        Capacity= (uint32_t)csize << (n - 9);
    }
    return Capacity;
}

int MSD0_GetCardInfo(QSPI_TypeDef* QSPIx, PMSD_CARDINFO SD0_CardInfo)
{
    uint8_t r1;
    uint8_t CSD_Tab[16];
    uint8_t CID_Tab[16];

    /* Send CMD9, Read CSD */
    r1 = SD_sendcmd(QSPIx, CMD9, 0, 0xFF);
    if(r1 != 0x00)
    {
        return r1;
    }

    if(SD_ReceiveData(QSPIx, CSD_Tab, 16))
    {
        return 1;
    }

    /* Send CMD10, Read CID */
    r1 = SD_sendcmd(QSPIx, CMD10, 0, 0xFF);
    if(r1 != 0x00)
    {
        return r1;
    }

    if(SD_ReceiveData(QSPIx, CID_Tab, 16))
    {
        return 2;
    }

    /* Byte 0 */
    SD0_CardInfo->CSD.CSDStruct = (CSD_Tab[0] & 0xC0) >> 6;
    SD0_CardInfo->CSD.SysSpecVersion = (CSD_Tab[0] & 0x3C) >> 2;
    SD0_CardInfo->CSD.Reserved1 = CSD_Tab[0] & 0x03;
    /* Byte 1 */
    SD0_CardInfo->CSD.TAAC = CSD_Tab[1] ;
    /* Byte 2 */
    SD0_CardInfo->CSD.NSAC = CSD_Tab[2];
    /* Byte 3 */
    SD0_CardInfo->CSD.MaxBusClkFrec = CSD_Tab[3];
    /* Byte 4 */
    SD0_CardInfo->CSD.CardComdClasses = CSD_Tab[4] << 4;
    /* Byte 5 */
    SD0_CardInfo->CSD.CardComdClasses |= (CSD_Tab[5] & 0xF0) >> 4;
    SD0_CardInfo->CSD.RdBlockLen = CSD_Tab[5] & 0x0F;
    /* Byte 6 */
    SD0_CardInfo->CSD.PartBlockRead = (CSD_Tab[6] & 0x80) >> 7;
    SD0_CardInfo->CSD.WrBlockMisalign = (CSD_Tab[6] & 0x40) >> 6;
    SD0_CardInfo->CSD.RdBlockMisalign = (CSD_Tab[6] & 0x20) >> 5;
    SD0_CardInfo->CSD.DSRImpl = (CSD_Tab[6] & 0x10) >> 4;
    SD0_CardInfo->CSD.Reserved2 = 0; /* Reserved */
    SD0_CardInfo->CSD.DeviceSize = (CSD_Tab[6] & 0x03) << 10;
    /* Byte 7 */
    SD0_CardInfo->CSD.DeviceSize |= (CSD_Tab[7]) << 2;
    /* Byte 8 */
    SD0_CardInfo->CSD.DeviceSize |= (CSD_Tab[8] & 0xC0) >> 6;
    SD0_CardInfo->CSD.MaxRdCurrentVDDMin = (CSD_Tab[8] & 0x38) >> 3;
    SD0_CardInfo->CSD.MaxRdCurrentVDDMax = (CSD_Tab[8] & 0x07);
    /* Byte 9 */
    SD0_CardInfo->CSD.MaxWrCurrentVDDMin = (CSD_Tab[9] & 0xE0) >> 5;
    SD0_CardInfo->CSD.MaxWrCurrentVDDMax = (CSD_Tab[9] & 0x1C) >> 2;
    SD0_CardInfo->CSD.DeviceSizeMul = (CSD_Tab[9] & 0x03) << 1;
    /* Byte 10 */
    SD0_CardInfo->CSD.DeviceSizeMul |= (CSD_Tab[10] & 0x80) >> 7;
    SD0_CardInfo->CSD.EraseGrSize = (CSD_Tab[10] & 0x7C) >> 2;
    SD0_CardInfo->CSD.EraseGrMul = (CSD_Tab[10] & 0x03) << 3;
    /* Byte 11 */
    SD0_CardInfo->CSD.EraseGrMul |= (CSD_Tab[11] & 0xE0) >> 5;
    SD0_CardInfo->CSD.WrProtectGrSize = (CSD_Tab[11] & 0x1F);
    /* Byte 12 */
    SD0_CardInfo->CSD.WrProtectGrEnable = (CSD_Tab[12] & 0x80) >> 7;
    SD0_CardInfo->CSD.ManDeflECC = (CSD_Tab[12] & 0x60) >> 5;
    SD0_CardInfo->CSD.WrSpeedFact = (CSD_Tab[12] & 0x1C) >> 2;
    SD0_CardInfo->CSD.MaxWrBlockLen = (CSD_Tab[12] & 0x03) << 2;
    /* Byte 13 */
    SD0_CardInfo->CSD.MaxWrBlockLen |= (CSD_Tab[13] & 0xc0) >> 6;
    SD0_CardInfo->CSD.WriteBlockPaPartial = (CSD_Tab[13] & 0x20) >> 5;
    SD0_CardInfo->CSD.Reserved3 = 0;
    SD0_CardInfo->CSD.ContentProtectAppli = (CSD_Tab[13] & 0x01);
    /* Byte 14 */
    SD0_CardInfo->CSD.FileFormatGrouop = (CSD_Tab[14] & 0x80) >> 7;
    SD0_CardInfo->CSD.CopyFlag = (CSD_Tab[14] & 0x40) >> 6;
    SD0_CardInfo->CSD.PermWrProtect = (CSD_Tab[14] & 0x20) >> 5;
    SD0_CardInfo->CSD.TempWrProtect = (CSD_Tab[14] & 0x10) >> 4;
    SD0_CardInfo->CSD.FileFormat = (CSD_Tab[14] & 0x0C) >> 2;
    SD0_CardInfo->CSD.ECC = (CSD_Tab[14] & 0x03);
    /* Byte 15 */
    SD0_CardInfo->CSD.CSD_CRC = (CSD_Tab[15] & 0xFE) >> 1;
    SD0_CardInfo->CSD.Reserved4 = 1;

    if(SD0_CardInfo->CardType == V2HC)
    {
        /* Byte 7 */
        SD0_CardInfo->CSD.DeviceSize = (uint16_t)(CSD_Tab[8]) *256;
        /* Byte 8 */
        SD0_CardInfo->CSD.DeviceSize += CSD_Tab[9] ;
    }

    SD0_CardInfo->Capacity = SD0_CardInfo->CSD.DeviceSize * MSD_BLOCKSIZE * 1024;
    SD0_CardInfo->BlockSize = MSD_BLOCKSIZE;

    /* Byte 0 */
    SD0_CardInfo->CID.ManufacturerID = CID_Tab[0];
    /* Byte 1 */
    SD0_CardInfo->CID.OEM_AppliID = CID_Tab[1] << 8;
    /* Byte 2 */
    SD0_CardInfo->CID.OEM_AppliID |= CID_Tab[2];
    /* Byte 3 */
    SD0_CardInfo->CID.ProdName1 = CID_Tab[3] << 24;
    /* Byte 4 */
    SD0_CardInfo->CID.ProdName1 |= CID_Tab[4] << 16;
    /* Byte 5 */
    SD0_CardInfo->CID.ProdName1 |= CID_Tab[5] << 8;
    /* Byte 6 */
    SD0_CardInfo->CID.ProdName1 |= CID_Tab[6];
    /* Byte 7 */
    SD0_CardInfo->CID.ProdName2 = CID_Tab[7];
    /* Byte 8 */
    SD0_CardInfo->CID.ProdRev = CID_Tab[8];
    /* Byte 9 */
    SD0_CardInfo->CID.ProdSN = CID_Tab[9] << 24;
    /* Byte 10 */
    SD0_CardInfo->CID.ProdSN |= CID_Tab[10] << 16;
    /* Byte 11 */
    SD0_CardInfo->CID.ProdSN |= CID_Tab[11] << 8;
    /* Byte 12 */
    SD0_CardInfo->CID.ProdSN |= CID_Tab[12];
    /* Byte 13 */
    SD0_CardInfo->CID.Reserved1 |= (CID_Tab[13] & 0xF0) >> 4;
    /* Byte 14 */
    SD0_CardInfo->CID.ManufactDate = (CID_Tab[13] & 0x0F) << 8;
    /* Byte 15 */
    SD0_CardInfo->CID.ManufactDate |= CID_Tab[14];
    /* Byte 16 */
    SD0_CardInfo->CID.CID_CRC = (CID_Tab[15] & 0xFE) >> 1;
    SD0_CardInfo->CID.Reserved2 = 1;

    return 0;
}

uint8_t SD_WriteDisk(QSPI_TypeDef* QSPIx, uint8_t*buf,uint32_t sector,uint8_t cnt)
{
    uint8_t r1;
    if(SD_TYPE!=V2HC)sector *= 512;
    if(cnt==1)
    {
        r1=SD_sendcmd(QSPIx, CMD24,sector,0X01);
        if(r1==0)
        {
            r1=SD_SendBlock(QSPIx, buf,0xFE);
        }
    } else {
        if(SD_TYPE!=MMC)
        {
            SD_sendcmd(QSPIx, CMD55,0,0X01);
            SD_sendcmd(QSPIx, CMD23,cnt,0X01);
        }
        r1=SD_sendcmd(QSPIx, CMD25,sector,0X01);
        if(r1==0)
        {
            do
            {
                r1=SD_SendBlock(QSPIx, buf,0xFC);
                buf+=512;
            }while(--cnt && r1==0);
            r1=SD_SendBlock(QSPIx, 0,0xFD);
        }
    }
    SD_CS(QSPIx, 0);
    return r1;
}

uint8_t SD_ReadDisk(QSPI_TypeDef* QSPIx, uint8_t*buf,uint32_t sector,uint8_t cnt)
{
    uint8_t r1;
    if(SD_TYPE!=V2HC)sector <<= 9;
    if(cnt==1)
    {
        r1=SD_sendcmd(QSPIx, CMD17,sector,0X01);
        if(r1==0)
        {
            r1=SD_ReceiveData(QSPIx, buf,512);
        }
    } else {
        r1=SD_sendcmd(QSPIx, CMD18,sector,0X01);
        do
        {
            r1=SD_ReceiveData(QSPIx, buf,512);
            buf+=512;
        }while(--cnt && r1==0);
        SD_sendcmd(QSPIx, CMD12,0,0X01);
    }
    SD_CS(QSPIx, 0);
    return r1;
}

uint8_t spi_readwrite(QSPI_TypeDef* QSPIx, uint8_t Txdata){
    uint8_t Rxdata;
    QSPI_TransmitReceive(QSPIx, &Txdata, &Rxdata);
    return Rxdata;
}

void SPI_setspeed(QSPI_TypeDef* QSPIx, uint8_t speed){
    QSPIx->SCKDIV = speed;
}

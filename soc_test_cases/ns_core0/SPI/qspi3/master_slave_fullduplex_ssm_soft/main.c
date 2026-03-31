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
#include <stdlib.h>
#include "ns_sdk_hal.h"

#define  ARRAYSIZE         (10)

volatile uint32_t send_n = 0, receive_n = 0;
uint8_t QSPI3_send_array[ARRAYSIZE] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};
uint8_t QSPI2_send_array[ARRAYSIZE] = {0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA};
uint8_t QSPI3_receive_array[ARRAYSIZE] = {0};
uint8_t QSPI2_receive_array[ARRAYSIZE] = {0};
uint32_t QSPI3_send_array_32B[ARRAYSIZE] = {0x5aa5a5A1, 0x5aa5a5A2, 0x5aa5a5A3, 0x5aa5a5A4, 0x5aa5a5A5, 0x5aa5a5A6, 0x5aa5a5A7, 0x5aa5a5A8, 0x5aa5a5A9, 0x5aa5a5AA};
uint32_t QSPI2_send_array_32B[ARRAYSIZE] = {0x5aa5a5B1, 0x5aa5a5B2, 0x5aa5a5B3, 0x5aa5a5B4, 0x5aa5a5B5, 0x5aa5a5B6, 0x5aa5a5B7, 0x5aa5a5B8, 0x5aa5a5B9, 0x5aa5a5BA};
uint32_t QSPI3_receive_array_32B[ARRAYSIZE] = {0};
uint32_t QSPI2_receive_array_32B[ARRAYSIZE] = {0};
uint16_t QSPI3_send_array_16B[ARRAYSIZE] = {0xa5A1, 0xa5A2, 0xa5A3, 0xa5A4, 0xa5A5, 0xa5A6, 0xa5A7, 0xa5A8, 0xa5A9, 0xa5AA};
uint16_t QSPI2_send_array_16B[ARRAYSIZE] = {0xa5B1, 0xa5B2, 0xa5B3, 0xa5B4, 0xa5B5, 0xa5B6, 0xa5B7, 0xa5B8, 0xa5B9, 0xa5BA};
uint16_t QSPI3_receive_array_16B[ARRAYSIZE] = {0};
uint16_t QSPI2_receive_array_16B[ARRAYSIZE] = {0};

/**
  * \brief memory compare function
  *
  * \param src: source data pointer
  * \param dst: destination data pointer
  * \param length: the compare data length
  *
  * \retval ErrStatus: ERROR or SUCCESS
  */
ErrStatus MemoryCompare(uint8_t* src, uint8_t* dst, uint16_t length)
{
    while (length--) {
        if (*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

/**
  * \brief memory compare function
  *
  * \param src: source data pointer
  * \param dst: destination data pointer
  * \param length: the compare data length
  *
  * \retval ErrStatus: ERROR or SUCCESS
  */
ErrStatus MemoryCompare16B(uint16_t* src, uint16_t* dst, uint8_t length)
{
    while(length--){
        if(*src++ != *dst++)
            return ERROR;
    }
    return SUCCESS;
}

/**
  * \brief memory compare function
  *
  * \param src: source data pointer
  * \param dst: destination data pointer
  * \param length: the compare data length
  *
  * \retval ErrStatus: ERROR or SUCCESS
  */
ErrStatus MemoryCompare32B(uint32_t* src, uint32_t* dst, uint8_t length)
{
    while(length--){
        if(*src++ != *dst++)
            return ERROR;
    }
    return SUCCESS;
}

/**
  * \brief configure the SPI peripheral
  */
void SPI_Config(void)
{
    QSPI_InitTypeDef spi_init_struct;
    /* deinitilize SPI and the parameters */ 
    QSPI_StructInit(&spi_init_struct);
    /* SPI1 parameter config */ /*Adjust the init sequence to prevent the "x" state */
    spi_init_struct.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    spi_init_struct.DataSize = QSPI_FMT_LEN_8B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_LOW;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE2;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.SSM = QSPI_CR_SSM_SOFT;
    spi_init_struct.DevMode = QSPI_CR_MODE_SLAVE;
    QSPI_Init(QSPI2, &spi_init_struct);
    /* SPI0 parameter config */
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    spi_init_struct.SSM = QSPI_CR_SSM_SOFT;
    QSPI_Init(QSPI3, &spi_init_struct);
    }

void iomux_config(void)
{
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_SCK_IOF_OVAL, QSPI3_SCK_PAD_SEL, QSPI3_SCK_HS_SEL, 0, 0);
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_CS_0_IOF_OVAL, QSPI3_CS_0_PAD_SEL, QSPI3_CS_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_0_IOF_IVAL, QSPI3_DQ_0_PAD_SEL, QSPI3_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_0_IOF_OVAL, QSPI3_DQ_0_PAD_SEL, QSPI3_DQ_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_1_IOF_IVAL, QSPI3_DQ_1_PAD_SEL, QSPI3_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_1_IOF_OVAL, QSPI3_DQ_1_PAD_SEL, QSPI3_DQ_1_HS_SEL, 0, 0);

    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_SCK_IOF_IVAL, QSPI2_SCK_PAD_SEL, QSPI2_SCK_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_CS_0_IOF_IVAL, QSPI2_CS_0_PAD_SEL, QSPI2_CS_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_0_IOF_IVAL, QSPI2_DQ_0_PAD_SEL, QSPI2_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_0_IOF_OVAL, QSPI2_DQ_0_PAD_SEL, QSPI2_DQ_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_1_IOF_IVAL, QSPI2_DQ_1_PAD_SEL, QSPI2_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_1_IOF_OVAL, QSPI2_DQ_1_PAD_SEL, QSPI2_DQ_1_HS_SEL, 0, 0);
}

void main(void)
{
    #ifdef MISC_HAS_QSPI3_HAS_CLK
    qspi3_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_QSPI3_RST
    qspi3_set_rst(DISABLE);
    qspi3_set_rst(ENABLE);
    #endif
    
    #ifdef MISC_HAS_QSPI2_HAS_CLK
    qspi2_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_QSPI2_RST
    qspi2_set_rst(DISABLE);
    qspi2_set_rst(ENABLE);
    #endif

    iomux_config();

    /* SPI configure */
    SPI_Config();
    /* wait for transmit complete */
    while (send_n < ARRAYSIZE) {
        QSPI_SSMSwitch(QSPI2,ENABLE);
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI2, QSPI2_send_array[send_n]);
        while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI3, QSPI3_send_array[send_n++]);
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_EMPTY)) {}
        QSPI2_receive_array[receive_n] = QSPI_ReceiveData(QSPI2);
        while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_RX_EMPTY)) {}
        QSPI3_receive_array[receive_n++] = QSPI_ReceiveData(QSPI3);
        QSPI_SSMSwitch(QSPI2,DISABLE);
    }
    if (MemoryCompare(QSPI2_receive_array, QSPI3_send_array, ARRAYSIZE) &
        MemoryCompare(QSPI3_receive_array, QSPI2_send_array, ARRAYSIZE)) {
        printf("1 SPI_master_slave_fullduplex_polling  #### PASS ####\n");
    } else {
        for (int i = 0; i < ARRAYSIZE; i++) {
            printf("data [%d]%x  %x \r\n",i,QSPI2_receive_array[i],QSPI3_receive_array[i]);
        }
        printf("1 SPI_master_slave_fullduplex_polling  #### FAIL ####\n");
    }
    for (int i = 0 ;i < ARRAYSIZE; i++) {
        QSPI3_receive_array_32B[i] = 0;
        QSPI2_receive_array_32B[i] = 0;
    }

    QSPI_InitTypeDef spi_init_struct;
    QSPI_StructInit(&spi_init_struct);
    /* SPI0 parameter config */
    spi_init_struct.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    
    spi_init_struct.DataSize = QSPI_FMT_LEN_32B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_LOW;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE1;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.SSM = QSPI_CR_SSM_SOFT;
    QSPI_Init(QSPI3, &spi_init_struct);
    /* SPI1 parameter config */
    spi_init_struct.DevMode = QSPI_CR_MODE_SLAVE;
    spi_init_struct.SSM = QSPI_CR_SSM_SOFT;
    QSPI_Init(QSPI2, &spi_init_struct);

    send_n = 0;
    receive_n = 0;

    while (send_n < ARRAYSIZE) {
        QSPI_SSMSwitch(QSPI2,ENABLE);
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI2, QSPI2_send_array_32B[send_n]);
        while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI3, QSPI3_send_array_32B[send_n++]);
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_EMPTY)) {}
        QSPI2_receive_array_32B[receive_n] = QSPI_ReceiveData(QSPI2);
        while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_RX_EMPTY)) {}
        QSPI3_receive_array_32B[receive_n++] = QSPI_ReceiveData(QSPI3);
        QSPI_SSMSwitch(QSPI2,DISABLE);
    }
    if (MemoryCompare32B(QSPI2_receive_array_32B, QSPI3_send_array_32B, ARRAYSIZE) &
        MemoryCompare32B(QSPI3_receive_array_32B, QSPI2_send_array_32B, ARRAYSIZE)) {
        printf("2 SPI_master_slave_fullduplex_polling  #### PASS ####\n");
    } else {
        printf("2 SPI_master_slave_fullduplex_polling  #### FAIL ####\n");
    }
    for(int i = 0; i < ARRAYSIZE; i++) {
        QSPI3_send_array_16B[i] = rand() % 65535;
        QSPI2_send_array_16B[i] = rand() % 65535;
    }
    for (int i = 0; i < ARRAYSIZE; i++) {
        QSPI3_receive_array_16B[i] = 0;
        QSPI2_receive_array_16B[i] = 0;
    }
    /* SPI0 parameter config */
    spi_init_struct.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    spi_init_struct.SSM = QSPI_CR_SSM_SOFT;
    spi_init_struct.DataSize = QSPI_FMT_LEN_16B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_HIGH;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE1;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    
    QSPI_Init(QSPI3, &spi_init_struct);
    /* SPI1 parameter config */
    spi_init_struct.DevMode = QSPI_CR_MODE_SLAVE;
    QSPI_Init(QSPI2, &spi_init_struct);

    send_n = 0;
    receive_n = 0;

    while (send_n < ARRAYSIZE) {
        QSPI_SSMSwitch(QSPI2,ENABLE);
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI2, QSPI2_send_array_16B[send_n]);
        while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI3, QSPI3_send_array_16B[send_n++]);
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_EMPTY)) {}
        QSPI2_receive_array_16B[receive_n] = QSPI_ReceiveData(QSPI2);
        while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_RX_EMPTY)) {}
        QSPI3_receive_array_16B[receive_n++] = QSPI_ReceiveData(QSPI3);
        QSPI_SSMSwitch(QSPI2,DISABLE);
    }
    if (MemoryCompare16B(QSPI2_receive_array_16B, QSPI3_send_array_16B, ARRAYSIZE) &
        MemoryCompare16B(QSPI3_receive_array_16B, QSPI2_send_array_16B, ARRAYSIZE)) {
        printf("3 SPI_master_slave_fullduplex_polling  #### PASS ####\n");
    } else {
        printf("3 SPI_master_slave_fullduplex_polling  #### FAIL ####\n");
    }
    for (int i = 0; i < ARRAYSIZE; i++) {
        QSPI3_receive_array_32B[i] = 0;
        QSPI2_receive_array_32B[i] = 0;
    }
    /* SPI0 parameter config */
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    
    spi_init_struct.SSM = QSPI_CR_SSM_SOFT;
    spi_init_struct.DataSize = QSPI_FMT_LEN_32B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_HIGH;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE2;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_LSB;
    QSPI_Init(QSPI3, &spi_init_struct);
    /* SPI1 parameter config */
    spi_init_struct.DevMode = QSPI_CR_MODE_SLAVE;
    QSPI_Init(QSPI2, &spi_init_struct);

    send_n = 0;
    receive_n = 0;

    while (send_n < ARRAYSIZE) {
        QSPI_SSMSwitch(QSPI2,ENABLE);
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI2, QSPI2_send_array_32B[send_n]);
        while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI3, QSPI3_send_array_32B[send_n++]);
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_EMPTY)) {}
        QSPI2_receive_array_32B[receive_n] = QSPI_ReceiveData(QSPI2);
        while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_RX_EMPTY)) {}
        QSPI3_receive_array_32B[receive_n++] = QSPI_ReceiveData(QSPI3);
        QSPI_SSMSwitch(QSPI2,DISABLE);
    }
    if (MemoryCompare32B(QSPI2_receive_array_32B, QSPI3_send_array_32B, ARRAYSIZE) &
        MemoryCompare32B(QSPI3_receive_array_32B, QSPI2_send_array_32B, ARRAYSIZE)) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

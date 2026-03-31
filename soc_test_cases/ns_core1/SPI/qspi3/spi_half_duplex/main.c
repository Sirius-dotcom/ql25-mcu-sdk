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
#define  ARRAYSIZE         (10)

volatile uint32_t send_n = 0, receive_n = 0;
uint8_t QSPI3_send_array[ARRAYSIZE] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};
uint8_t QSPI2_send_array[ARRAYSIZE] = {0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA};
uint8_t QSPI3_receive_array[ARRAYSIZE] = {0};
uint8_t QSPI2_receive_array[ARRAYSIZE] = {0};

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
  * \brief configure the SPI peripheral
  */
void SPI_Config(void)
{
    QSPI_InitTypeDef spi_init_struct;
    /* deinitilize SPI and the parameters */
    QSPI_StructInit(&spi_init_struct);
    /* SPI1 parameter config */ /*Adjust the init sequence to prevent the "x" state */
    spi_init_struct.DataSize = QSPI_FMT_LEN_8B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_LOW;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE2;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    spi_init_struct.ProtolMode = QSPI_FMT_PROTO_HDS;
    spi_init_struct.DevMode = QSPI_CR_MODE_SLAVE;
    QSPI_Init(QSPI2, &spi_init_struct);
    /* SPI0 parameter config */
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
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
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_2_IOF_IVAL, QSPI3_DQ_2_PAD_SEL, QSPI3_DQ_2_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_2_IOF_OVAL, QSPI3_DQ_2_PAD_SEL, QSPI3_DQ_2_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_3_IOF_IVAL, QSPI3_DQ_3_PAD_SEL, QSPI3_DQ_3_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_3_IOF_OVAL, QSPI3_DQ_3_PAD_SEL, QSPI3_DQ_3_HS_SEL, 0, 0);

    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_SCK_IOF_IVAL, QSPI2_SCK_PAD_SEL, QSPI2_SCK_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_CS_0_IOF_IVAL, QSPI2_CS_0_PAD_SEL, QSPI2_CS_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_0_IOF_IVAL, QSPI2_DQ_0_PAD_SEL, QSPI2_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_0_IOF_OVAL, QSPI2_DQ_0_PAD_SEL, QSPI2_DQ_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_1_IOF_IVAL, QSPI2_DQ_1_PAD_SEL, QSPI2_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_1_IOF_OVAL, QSPI2_DQ_1_PAD_SEL, QSPI2_DQ_1_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_2_IOF_IVAL, QSPI2_DQ_2_PAD_SEL, QSPI2_DQ_2_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_2_IOF_OVAL, QSPI2_DQ_2_PAD_SEL, QSPI2_DQ_2_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_3_IOF_IVAL, QSPI2_DQ_3_PAD_SEL, QSPI2_DQ_3_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_3_IOF_OVAL, QSPI2_DQ_3_PAD_SEL, QSPI2_DQ_3_HS_SEL, 0, 0);
}

void main()
{
    int state0 = 0;
    int state1 = 0;

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

    SPI_Config();
    QSPI_DirectionConfig(QSPI3, QSPI_FMT_DIR_TX);
    QSPI_DirectionConfig(QSPI2, QSPI_FMT_DIR_RX);
    /* wait for transmit complete */
    while (send_n < ARRAYSIZE) {
        while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI3, QSPI3_send_array[send_n++]);
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_EMPTY)) {}
        QSPI2_receive_array[receive_n++] = QSPI_ReceiveData(QSPI2);
    }
    if (MemoryCompare(QSPI2_receive_array, QSPI3_send_array, ARRAYSIZE)) {
        state0 = 1;
    }

    send_n = 0;
    receive_n = 0;

    QSPI_DirectionConfig(QSPI3, QSPI_FMT_DIR_RX);
    QSPI_DirectionConfig(QSPI2, QSPI_FMT_DIR_TX);
    /* wait for transmit complete */
    while (send_n < ARRAYSIZE) {
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI2, QSPI2_send_array[send_n]);
        while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI3, QSPI3_send_array[send_n++]);
        while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_RX_EMPTY)) {}
        QSPI3_receive_array[receive_n++] = QSPI_ReceiveData(QSPI3);
    }
    if (MemoryCompare(QSPI3_receive_array, QSPI2_send_array, ARRAYSIZE)) {
        state1 = 1;
    }

    if (state0 & state1) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

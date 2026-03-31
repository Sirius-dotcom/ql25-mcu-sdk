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

#define  ARRAYSIZE         30

volatile uint32_t send_n = 0, receive_n = 0;
uint8_t QSPI2_send_array[ARRAYSIZE] = {0};
uint8_t QSPI2_receive_array[ARRAYSIZE] = {0};

/**
  * \brief configure the SPI peripheral
  */
void SPI_Config(void)
{
    QSPI_InitTypeDef spi_init_struct;
    /* deinitilize SPI and the parameters */
    QSPI_StructInit(&spi_init_struct);
    /* QSPI2 parameter config */
    spi_init_struct.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    spi_init_struct.DataSize = QSPI_FMT_LEN_8B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_LOW;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE2;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    /* QSPI2 parameter config */
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    QSPI_Init(QSPI2, &spi_init_struct);
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
ErrStatus MemoryCompare(uint8_t* src, uint8_t* dst, uint16_t length)
{
    while (length--) {
        if (*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

void iomux_config(void)
{
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_SCK_IOF_OVAL, QSPI2_SCK_PAD_SEL, QSPI2_SCK_HS_SEL, 0, 0);
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_CS_0_IOF_OVAL, QSPI2_CS_0_PAD_SEL, QSPI2_CS_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_0_IOF_IVAL, QSPI2_DQ_0_PAD_SEL, QSPI2_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_0_IOF_OVAL, QSPI2_DQ_0_PAD_SEL, QSPI2_DQ_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_1_IOF_IVAL, QSPI2_DQ_1_PAD_SEL, QSPI2_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_1_IOF_OVAL, QSPI2_DQ_1_PAD_SEL, QSPI2_DQ_1_HS_SEL, 0, 0);

}

void main(void)
{
    int state0 = 0;
    int state1 = 0;

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
    printf("start spi_autotest\r\n");

    for (int i = 0; i < ARRAYSIZE; i++){
        QSPI2_send_array[i] = 0xa1 + i;
    }
    /* wait for transmit complete */
    while (send_n < ARRAYSIZE) {
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI2, QSPI2_send_array[send_n++]);
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_EMPTY)) {}
        QSPI2_receive_array[receive_n++] = QSPI_ReceiveData(QSPI2);
    }
    if (MemoryCompare(QSPI2_receive_array, QSPI2_send_array, ARRAYSIZE)) {
        state0 = 1;
    }

    if (state0) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

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
uint8_t QSPI1_send_array[ARRAYSIZE] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};
uint8_t QSPI2_send_array[ARRAYSIZE] = {0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA};
uint8_t QSPI1_receive_array[ARRAYSIZE] = {0};
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
 * \brief this function handles QSPI1 Handler exception
 */
void QSPI1_IRQHandler(void)
{
    if (RESET != QSPI_GetIPStatus(QSPI1, QSPI_IP_TX)) {
        /* send data */
        while (SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI1, QSPI1_send_array[send_n++]);
        if (ARRAYSIZE == send_n) {
            QSPI_ITConfig(QSPI1, QSPI_IE_TX,DISABLE);
        }
    } else if (SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_RX_OVR)) {
        QSPI_ClearFlag(QSPI1,QSPI_STATUS_RX_OVR);
        printf("this is QSPI_STATUS_RX_OVR\r\n");
    } else if (SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_UDR)) {
        QSPI_ClearFlag(QSPI1,QSPI_STATUS_TX_UDR);
        printf("this is QSPI_STATUS_TX_UDR\r\n");
    }
}

/**
 * \brief this function handles QSPI2 Handler exception
 */
void QSPI2_IRQHandler(void)
{
    /* receive data */
    if (RESET != QSPI_GetIPStatus(QSPI2, QSPI_IP_RX)) {
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_EMPTY)) {}
        QSPI2_send_array[receive_n++] = QSPI_ReceiveData(QSPI2);
    }
    if (ARRAYSIZE == receive_n) {
        QSPI_ITConfig(QSPI2, QSPI_IE_RX,DISABLE);
    }
}

/**
 * \brief configure the SPI peripheral
 */
void SPI_Config(void)
{
    QSPI_InitTypeDef spi_init_struct;
    /* deinitilize SPI and the parameters */
    QSPI_StructInit(&spi_init_struct);
    /* configure QSPI1 parameter */
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    spi_init_struct.DataSize = QSPI_FMT_LEN_8B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_LOW;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE2;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_4;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    QSPI_Init(QSPI1, &spi_init_struct);
    /* configure QSPI2 parameter */
    spi_init_struct.DevMode = QSPI_CR_MODE_SLAVE;
    QSPI_Init(QSPI2, &spi_init_struct);
}

void iomux_config(void)
{
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_SCK_IOF_OVAL, QSPI1_SCK_PAD_SEL, QSPI1_SCK_HS_SEL, 0, 0);
    
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_CS_0_IOF_OVAL, QSPI1_CS_0_PAD_SEL, QSPI1_CS_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_IVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_OVAL, QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_IVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_OVAL, QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);

    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_SCK_IOF_IVAL, QSPI2_SCK_PAD_SEL, QSPI2_SCK_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_CS_0_IOF_IVAL, QSPI2_CS_0_PAD_SEL, QSPI2_CS_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_0_IOF_IVAL, QSPI2_DQ_0_PAD_SEL, QSPI2_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_0_IOF_OVAL, QSPI2_DQ_0_PAD_SEL, QSPI2_DQ_0_HS_SEL, 0, 0);
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_1_IOF_IVAL, QSPI2_DQ_1_PAD_SEL, QSPI2_DQ_1_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_1_IOF_OVAL, QSPI2_DQ_1_PAD_SEL, QSPI2_DQ_1_HS_SEL, 0, 0);
}

void main(void)
{
    #ifdef MISC_HAS_QSPI1_HAS_CLK
    qspi1_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_QSPI1_RST
    qspi1_set_rst(DISABLE);
    qspi1_set_rst(ENABLE);
    #endif
    
    #ifdef MISC_HAS_QSPI2_HAS_CLK
    qspi2_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_QSPI2_RST
    qspi2_set_rst(DISABLE);
    qspi2_set_rst(ENABLE);
    #endif

    iomux_config();

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt QSPI1_IRQn */
    ECLIC_Register_IRQ(QSPI1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    QSPI1_IRQHandler);
    /*register interrupt QSPI2_IRQn */
    ECLIC_Register_IRQ(QSPI2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 2, 0,
                                    QSPI2_IRQHandler);
    /* SPI configure */
    SPI_Config();
    QSPI_TxmarkConfig(QSPI1, 1);
    QSPI_RxmarkConfig(QSPI2, 0);
    QSPI_TxmarkConfig(QSPI2, 1);
    QSPI_RxmarkConfig(QSPI1, 0);
    /* QSPI interrupt enable */
    QSPI_ITConfig(QSPI2, QSPI_IE_RX,ENABLE);
    QSPI_ITConfig(QSPI1, QSPI_IE_TX,ENABLE);
    /* wait transmit complete */
    while (receive_n < ARRAYSIZE) {}
    /* compare receive data with send data */
    if (MemoryCompare(QSPI2_send_array, QSPI1_send_array, ARRAYSIZE)) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

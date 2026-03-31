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

#define  ARRAYSIZE         (4)
#define OVERFLOW_COUNT     (5)

volatile uint8_t test_cs = 0;

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
    if ((SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_RX_UDR)) & 
        (SET == QSPI_GetITConfig(QSPI1, QSPI_IE_RX_UDR))) {
        QSPI_ClearFlag(QSPI1,QSPI_STATUS_RX_UDR);
        test_cs = 1;
        
        QSPI_ITConfig(QSPI1, QSPI_IE_RX_UDR,DISABLE);
    }
    if ((SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_UDR)) & 
        (SET == QSPI_GetITConfig(QSPI1, QSPI_IE_TX_UDR))) {
        QSPI_ClearFlag(QSPI1,QSPI_STATUS_TX_UDR);
        test_cs = 3;
        
        QSPI_ITConfig(QSPI1, QSPI_IE_TX_UDR,DISABLE);
    }
    if ((SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_OVR)) & 
        (SET == QSPI_GetITConfig(QSPI1, QSPI_IE_TX_OVR))) {
        QSPI_ClearFlag(QSPI1,QSPI_STATUS_TX_OVR);
        test_cs = 5;
        
        QSPI_ITConfig(QSPI1, QSPI_IE_TX_OVR,DISABLE);
    }
    if ((SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_RX_OVR)) & 
        (SET == QSPI_GetITConfig(QSPI1, QSPI_IE_RX_OVR))) {
        QSPI_ClearFlag(QSPI1,QSPI_STATUS_RX_OVR);
        test_cs = 7;
        
        QSPI_ITConfig(QSPI1, QSPI_IE_RX_OVR,DISABLE);
    }
}

/**
 * \brief this function handles QSPI2 Handler exception
 */
void QSPI2_IRQHandler(void)
{
    if ((SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_UDR)) & (SET == QSPI_GetITConfig(QSPI2, QSPI_IE_RX_UDR))) {
        QSPI_ClearFlag(QSPI2,QSPI_STATUS_RX_UDR);
        test_cs = 2;
        
        QSPI_ITConfig(QSPI2, QSPI_IE_RX_UDR,DISABLE);
    }
    if ((SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_TX_UDR)) & (SET == QSPI_GetITConfig(QSPI2, QSPI_IE_TX_UDR))) {
        QSPI_ClearFlag(QSPI2,QSPI_STATUS_TX_UDR);
        test_cs = 4;
        
        QSPI_ITConfig(QSPI2, QSPI_IE_TX_UDR,DISABLE);
    }
    if ((SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_TX_OVR)) & (SET == QSPI_GetITConfig(QSPI2, QSPI_IE_TX_OVR))) {
        QSPI_ClearFlag(QSPI2,QSPI_STATUS_TX_OVR);
        test_cs = 6;
        
        QSPI_ITConfig(QSPI2, QSPI_IE_TX_OVR,DISABLE);
    }
    if ((SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_OVR)) & (SET == QSPI_GetITConfig(QSPI2, QSPI_IE_RX_OVR))) {
        QSPI_ClearFlag(QSPI2,QSPI_STATUS_RX_OVR);
        test_cs = 8;
        
        QSPI_ITConfig(QSPI2, QSPI_IE_RX_OVR,DISABLE);
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
    /* configure QSPI2 parameter */ /*Adjust the init sequence to prevent the "x" state */
    spi_init_struct.SSM = QSPI_CR_SSM_HARD;
    spi_init_struct.DataSize = QSPI_FMT_LEN_8B;
    spi_init_struct.CPOL = QSPI_SCKMODE_CPOL_LOW;
    spi_init_struct.CPHA = QSPI_SCKMODE_CPHA_EDGE2;
    spi_init_struct.SCKDIV = QSPI_SCKDIV_PRESCALER_64;
    spi_init_struct.Endian = QSPI_FMT_ENDIAN_MSB;
    spi_init_struct.DevMode = QSPI_CR_MODE_SLAVE;
    QSPI_Init(QSPI2, &spi_init_struct);
    /* configure QSPI1 parameter */
    spi_init_struct.DevMode = QSPI_CR_MODE_MASTER;
    QSPI_Init(QSPI1, &spi_init_struct);
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
    QSPI_RxmarkConfig(QSPI2, ARRAYSIZE);
    QSPI_TxmarkConfig(QSPI2, 1);
    QSPI_RxmarkConfig(QSPI1, ARRAYSIZE);

    while(1) {
        switch (test_cs) {
            case 0:
                test_cs = 0xFF;
                QSPI_ITConfig(QSPI1, QSPI_IE_RX_UDR,ENABLE);
                QSPI_ReceiveData(QSPI1);
                break;
            case 1:
                test_cs = 0xFF;
                QSPI_ITConfig(QSPI2, QSPI_IE_RX_UDR,ENABLE);
                QSPI_ReceiveData(QSPI2);
                break;
            case 2:
                #if 0
                test_cs = 0xFF;
                QSPI_ITConfig(QSPI1, QSPI_IE_TX_UDR,ENABLE);
                for (int i = 0;i < ARRAYSIZE;i++) {
                    QSPI_SendData(QSPI2, QSPI2_send_array[i]);
                }
                QSPI_SendData(QSPI2, 0xAA);
                break;
                #endif
            case 3:
                test_cs = 0xFF;
                QSPI_ITConfig(QSPI2, QSPI_IE_TX_UDR,ENABLE);
                QSPI_DirectionConfig(QSPI1, QSPI_FMT_DIR_TX);
                QSPI_DirectionConfig(QSPI2, QSPI_FMT_DIR_RX);
                QSPI_SendData(QSPI1, 0xAA);
                while (QSPI1->STATUS & QSPI_STATUS_BUSY) {}
                break;
            case 4:
                test_cs = 0xFF;
                QSPI_ITConfig(QSPI1, QSPI_IE_TX_OVR,ENABLE);
                QSPI_DirectionConfig(QSPI1, QSPI_FMT_DIR_TX);
                QSPI_DirectionConfig(QSPI2, QSPI_FMT_DIR_TX);
                for (int i = 0;i < ARRAYSIZE * OVERFLOW_COUNT;i++) {
                    
                    QSPI1->TXDATA = 0xAA;
                }
                while (QSPI1->STATUS & QSPI_STATUS_BUSY) {}
                break;
            case 5:
                test_cs = 0xFF;
                QSPI_ITConfig(QSPI2, QSPI_IE_TX_OVR,ENABLE);
                QSPI_DirectionConfig(QSPI1, QSPI_FMT_DIR_TX);
                QSPI_DirectionConfig(QSPI2, QSPI_FMT_DIR_TX);
                for (int i = 0;i < ARRAYSIZE * OVERFLOW_COUNT;i++) {
                    
                    QSPI2->TXDATA = 0xBB;
                }
                while (QSPI2->STATUS & QSPI_STATUS_BUSY) {}
                break;
            case 6:
                test_cs = 0xFF;
                QSPI_ITConfig(QSPI1, QSPI_IE_RX_OVR,ENABLE);
                QSPI_DirectionConfig(QSPI1, QSPI_FMT_DIR_RX);
                QSPI_DirectionConfig(QSPI2, QSPI_FMT_DIR_RX);
                for (int i = 0;i < ARRAYSIZE * OVERFLOW_COUNT;i++) {
                    
                    QSPI1->TXDATA = 0xAA;
                    QSPI2->TXDATA = 0xBB;
                }
                while (QSPI1->STATUS & QSPI_STATUS_BUSY) {}
                while (QSPI2->STATUS & QSPI_STATUS_BUSY) {}
                break;
            case 7:
                test_cs = 0xFF;
                QSPI_ITConfig(QSPI2, QSPI_IE_RX_OVR,ENABLE);
                QSPI_DirectionConfig(QSPI1, QSPI_FMT_DIR_RX);
                QSPI_DirectionConfig(QSPI2, QSPI_FMT_DIR_RX);
                for (int i = 0;i < ARRAYSIZE * OVERFLOW_COUNT;i++) {
                    
                    QSPI1->TXDATA = 0xAA;
                    QSPI2->TXDATA = 0xBB;
                }
                while (QSPI1->STATUS & QSPI_STATUS_BUSY) {}
                while (QSPI2->STATUS & QSPI_STATUS_BUSY) {}
                break;
            case 8:
                simulation_pass();
                while (1) {}
                break;
            default:
                break;
        }
    }

    while (1) {}
}

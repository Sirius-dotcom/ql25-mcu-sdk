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
#include "eeprom.h"

#define PAGE_SIZE            (16)
#define EEP_FIRST_PAGE         (0x00)

uint8_t i2c_buffer_write[PAGE_SIZE] = {0};
uint8_t i2c_buffer_read[PAGE_SIZE] = {0};

volatile int send_index = 0;
volatile int recv_index = 0;
/**
 * @brief handles the I2C1 interrupt
 *        once tx_fifo_depth <  tx_watermark, it will trigger an interrupt
 */
void I2C1_IRQHandler(void)
{
    if(I2C_Get_IntStatus(I2C1, I2C_INT_IE_TXFIFO_WM_ENABLE) && I2C_Get_Status(I2C1, I2C_STATUS_TXFIFO_WM))
    {
        I2C1->TXDATA = i2c_buffer_write[send_index];
        I2C_Write_En(I2C1);
        send_index ++;
        if(send_index == PAGE_SIZE) {
            I2C_Interrupt_En(I2C1, I2C_INT_IE_TXFIFO_WM_ENABLE, DISABLE);
            I2C_Set_TxWaterMark(I2C1, 0);
        }
    }
    if(I2C_Get_IntStatus(I2C1, I2C_INT_IE_RXFIFO_WM_ENABLE) && I2C_Get_Status(I2C1, I2C_STATUS_RXFIFO_WM))
    {
        I2C_Read_En(I2C1);
        i2c_buffer_read[recv_index] = I2C1->RXDATA;
        recv_index ++;
        if(recv_index == PAGE_SIZE) {
            I2C_Interrupt_En(I2C1, I2C_INT_IE_RXFIFO_WM_ENABLE, DISABLE);
        }
    }
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
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE, I2C1_SCL_IOF_IVAL, I2C1_SCL_PAD_SEL, I2C1_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, I2C1_SCL_IOF_OVAL, I2C1_SCL_PAD_SEL, I2C1_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, I2C1_SDA_IOF_IVAL, I2C1_SDA_PAD_SEL, I2C1_SDA_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, I2C1_SDA_IOF_OVAL, I2C1_SDA_PAD_SEL, I2C1_SDA_HS_SEL, 0, 0);

    iomux_ls_iof_pullup_cfg(IOMUX_BASE, I2C1_SCL_PAD_SEL, I2C1_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_pullup_cfg(IOMUX_BASE, I2C1_SDA_PAD_SEL, I2C1_SDA_HS_SEL, 0, 0);
}

/**
  * \brief cofigure the I2C1 interfaces
  */
void I2C_Config(void)
{
    I2C_InitTypeDef init = {0};
    init.speed = I2C_SPEED_100K;
    init.time_cfg = DISABLE;
    init.auto_end = DISABLE;
    init.mode = I2C_MASTER_MODE;
    init.scl_pull = ENABLE;
    init.sda_pull = ENABLE;
    I2C_Init(I2C1, &init);
}

/**
  * \brief I2C read and write functions
  *
  * \retval None
  */
void i2c_eeprom_int_test()
{
    /* EEPROM data write */
    Eeprom_ByteWriteInt(I2C1, EEP_FIRST_PAGE, PAGE_SIZE);

    Eeprom_WaitStandbyState(I2C1);

    /* EEPROM data read */
    Eeprom_ByteReadInt(I2C1, EEP_FIRST_PAGE, PAGE_SIZE);
}

/**
  * \brief main
  *
  * \return int
  */
int main(void)
{
    
    #ifdef MISC_HAS_I2C1_HAS_CLK
    i2c1_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_I2C1_RST
    i2c1_set_rst(DISABLE);
    i2c1_set_rst(ENABLE);
    #endif
    
    #ifdef CFG_SIMULATION
    #ifdef MISC_HAS_I2C1_CLK_DIV
    
    #endif
    #endif

    /* initialize i2c_buffer_write */
    for (int i = 0;i < PAGE_SIZE; i++) {
        i2c_buffer_write[i] = i;
    }

    iomux_config();

    /* configure I2C */
    I2C_Config();

      /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt I2C1_IRQn */
    ECLIC_Register_IRQ(I2C1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    I2C1_IRQHandler);

    /* I2C1-eeprom read-write polling test*/
    i2c_eeprom_int_test();

    /* compare the read buffer and write buffer */
    if(SUCCESS != MemoryCompare(i2c_buffer_write, i2c_buffer_read , PAGE_SIZE)) {
        simulation_fail();
    } else {
        simulation_pass();
    }

    while (1) {}
}

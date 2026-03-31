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

#if CFG_SIMULATION
#define SOC_CLK                (100000000)
#else
#define SOC_CLK                (SystemCoreClock)
#endif
#define EEP_FIRST_PAGE         (0x00)

#define EEPROM_BLOCK0_ADDRESS  (0x50)
#define BUFFER_SIZE            (8)
#define I2C0_SPEED             (100000)
#define I3_SLAVE_ADDRESS1 (0x62)
#define I3_SLAVE_ADDRESS2 (0x72)
#define I2C_PAGE_SIZE          (16)

uint16_t eeprom_address = 0;
uint8_t i2c_buffer_write[BUFFER_SIZE] = {0};
uint8_t i2c_buffer_read[BUFFER_SIZE] = {0};

uint8_t i2c_transmitter[BUFFER_SIZE] = {0};
uint8_t i2c_receiver[BUFFER_SIZE] = {0};
volatile ErrStatus state = ERROR;

/**
  * \brief memory compare function
  *
  * \param src: source data pointer
  * \param dst: destination data pointer
  * \param length: the compare data length
  *
  * \retval ErrStatus: ERROR or SUCCESS
  */
static ErrStatus MemoryCompare(uint8_t* src, uint8_t* dst, uint16_t length)
{
    while (length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

/**
  * \brief cofigure the I2C0 interfaces
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
    I2C_Init(I2C0, &init);

    init.mode = I2C_SLAVE_MODE;
    init.slave_info1.slave_addr = I3_SLAVE_ADDRESS1;
    init.slave_info1.slave_addr_mask = 0x3FF;
    init.slave_info1.slave_addr_en = ENABLE;
    I2C_Init(I3, &init);
}

/**
  * \brief I2C read and write functions
  *
  * \retval None
  */
void i2c_eeprom_test()
{
    
    /* EEPROM data write */
    Eeprom_BufferWrite(I2C0, i2c_buffer_write, EEP_FIRST_PAGE, BUFFER_SIZE);

    /* EEPROM data read */
    Eeprom_BufferRead(I2C0, i2c_buffer_read, EEP_FIRST_PAGE, BUFFER_SIZE);
}

void main(void)
{
    #ifdef MISC_HAS_I2C0_HAS_CLK
    i2c0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_I2C0_RST
    i2c0_set_rst(DISABLE);
    i2c0_set_rst(ENABLE);
    #endif
    #ifdef CFG_SIMULATION
    #ifdef MISC_HAS_I2C0_CLK_DIV
    
    #endif
    #endif
    
    /* configure I2C */
    I2C_Config();

    for (int i = 0; i < BUFFER_SIZE; i++) {
        i2c_transmitter[i] = i + 0x80;
        i2c_buffer_write[i] = i;
    }
    /* read-write eeprom */
    i2c_eeprom_test();
    state = MemoryCompare(i2c_buffer_write, i2c_buffer_read , BUFFER_SIZE);
    delay_1ms(1);
    /* send data to slave i2c */
    I2C_MasterRecvSlaveSend_Onebyone(I2C0, I3, I3_SLAVE_ADDRESS1, i2c_transmitter, i2c_receiver, BUFFER_SIZE);

    state &= MemoryCompare(i2c_transmitter, i2c_receiver, BUFFER_SIZE);

    if (SUCCESS == state) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

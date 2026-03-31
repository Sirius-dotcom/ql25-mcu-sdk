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

#define I3_SLAVE_ADDRESS1 (0x62)
#define I3_SLAVE_ADDRESS2 (0x72)
#define BUFSIZE 8

uint8_t i2c_mst_send[BUFSIZE] = {0};
uint8_t i2c_slv_recv[BUFSIZE] = {0};
uint8_t i2c_mst_recv[BUFSIZE] = {0};
uint8_t i2c_slv_send[BUFSIZE] = {0};
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
ErrStatus MemoryCompare(uint8_t* src, uint8_t* dst, uint16_t length)
{
    while (length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

/**
  * \brief cofigure the I2C0 and I3 interfaces
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

void main(void)
{
    
    uint32_t timeout = 0xFFFFFFFF;

    #ifdef MISC_HAS_I2C0_HAS_CLK
    i2c0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_I2C0_RST
    i2c0_set_rst(DISABLE);
    i2c0_set_rst(ENABLE);
    #endif
    
    /* I2C config */
    I2C_Config();
    /* fill the tx buffer */
    for (int i = 0; i < BUFSIZE; i++) {
        i2c_mst_send[i] = i + 0x10;
        i2c_slv_send[i] = 0xff - i;
    }

    I2C_MasterRecvSlaveSend_Onebyone(I2C0, I3, I3_SLAVE_ADDRESS1, i2c_slv_send, i2c_mst_recv, BUFSIZE);

    I2C_MasterSendSlaveRecv_Onebyone(I2C0, I3, I3_SLAVE_ADDRESS1, i2c_mst_send, i2c_slv_recv, BUFSIZE);

    if((SUCCESS == MemoryCompare(i2c_mst_send, i2c_slv_recv, BUFSIZE)) & (SUCCESS == MemoryCompare(i2c_slv_send, i2c_mst_recv, BUFSIZE))){
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

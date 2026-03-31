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

#define MOD5(a) ((a) % (5))
#define I3_SLAVE_ADDRESS1 (0x62)
#define I3_SLAVE_ADDRESS2 (0x72)

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
  *         1.cfg mst tx_watermark = I2C0_FIFO_DEPTH tx_size = BUFSIZE
  *         2.cfg slv rx_watermark = 0 rx_size = BUFSIZE
  */
void I2C_Config(void)
{
    I2C_InitTypeDef init = {0};
    init.speed = I2C_SPEED_100K;
    init.time_cfg = DISABLE;
    init.auto_end = ENABLE;
    init.mode = I2C_MASTER_MODE;
    init.scl_pull = ENABLE;
    init.sda_pull = ENABLE;
    init.smbus_cfg.pec_en = ENABLE;
    I2C_Init(I2C0, &init);

    init.mode = I2C_SLAVE_MODE;
    init.slave_info1.slave_addr = I3_SLAVE_ADDRESS1;
    init.slave_info1.slave_addr_mask = 0x3FF;
    init.slave_info1.slave_addr_en = ENABLE;
    init.slave_info2.slave_addr = I3_SLAVE_ADDRESS2;
    init.slave_info2.slave_addr_mask = 0x3FF;
    init.slave_info2.slave_addr_en = ENABLE;

    init.smbus_cfg.smbd_en = ENABLE;
    init.smbus_cfg.smbh_en = ENABLE;
    init.smbus_cfg.alert_en = ENABLE;

    I2C_Init(I3, &init);
}

void i2c_sys_cfg(void)
{
    #ifdef MISC_HAS_I2C0_HAS_CLK
    i2c0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_I2C0_RST
    i2c0_set_rst(DISABLE);
    i2c0_set_rst(ENABLE);
    #endif

}

I2C_StateTypedef i2c_spec_addr_test(void)
{
    uint8_t slave_addr = SMBUS_ADDR_DEFAULT;
    I2C_StateTypedef state;
    uint32_t timeout = 0xFFFFFF;

    /* send slave address to I2C bus */
    state = I2C_Send_SlaveAddr(I2C0, slave_addr, I2C_DIR_WRITE, 0xFFFFFF);
    if(I2C_OK != state) {
        return state;
    }

    /* wait for addr match */
    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status (I3, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status(I3,I2C_STATUS_ADDR_MATCH );

    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status(I2C0, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status (I2C0, I2C_STATUS_ADDR_MATCH);

    I2C_Restart_Cmd();
    slave_addr = SMBUS_ADDR_HOST;
    timeout = 0xFFFFFF;
    I2C_Send_SlaveAddr(I2C0, slave_addr, I2C_DIR_WRITE, 0xFFFFFF);
    /* wait for addr match */
    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status (I3, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status(I3,I2C_STATUS_ADDR_MATCH );

    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status(I2C0, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status (I2C0, I2C_STATUS_ADDR_MATCH);

    I2C_Restart_Cmd();
    slave_addr = SMBUS_ADDR_ARA;
    timeout = 0xFFFFFF;
    I2C_Send_SlaveAddr(I2C0, slave_addr, I2C_DIR_WRITE, 0xFFFFFF);
    /* wait for addr match */
    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status (I3, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status(I3,I2C_STATUS_ADDR_MATCH );

    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status(I2C0, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status (I2C0, I2C_STATUS_ADDR_MATCH);

    return I2C_OK;
}

void main(void)
{
    
    i2c_sys_cfg();

    /* I2C config */
    I2C_Config();

    if(I2C_OK == i2c_spec_addr_test()){
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

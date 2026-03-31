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
#define I2C0_FIFO_DEPTH  (8)

volatile int alert_flag = 0;
volatile int addr_match = 0;
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
    I2C_InitTypeDef init1 = {0};
    I2C_InitTypeDef init2 = {0};
    init1.speed = I2C_SPEED_100K;
    init1.time_cfg = DISABLE;
    init1.auto_end = ENABLE;
    init1.mode = I2C_MASTER_MODE;
    init1.scl_pull = ENABLE;
    init1.sda_pull = ENABLE;
    init1.smbus_cfg.smbh_en = ENABLE;
    init1.smbus_cfg.alert_en = ENABLE;
    I2C_Init(I2C0, &init1);

    init2.speed = I2C_SPEED_100K;
    init2.time_cfg = DISABLE;
    init2.auto_end = ENABLE;
    init2.mode = I2C_SLAVE_MODE;
    init2.slave_info1.slave_addr = I3_SLAVE_ADDRESS1;
    init2.slave_info1.slave_addr_mask = 0x3FF;
    init2.slave_info1.slave_addr_en = ENABLE;
    I2C_Init(I3, &init2);
}

I2C_StateTypedef i2c_alert_test(void)
{
    uint8_t slave_addr;
    uint32_t timeout = 0xFFFFFF;
    I2C_StateTypedef state;
    /* alert request */
    I2C_Slave_Send_Alert(I3);

    do {
        if(timeout-- == 0){
            return I2C_TIMEOUT;
        }
    } while(alert_flag == 0);

    I2C_ACK(I3, ENABLE);

    /* send ara address to I2C bus */
    state = I2C_Send_SlaveAddr(I2C0, SMBUS_ADDR_ARA, I2C_DIR_READ, 0xFFFFFFFF);
    if(state != I2C_OK) {
        return I2C_TIMEOUT;
    }
    /* enable I3 addr match */
    I2C_Interrupt_En(I3, I2C_INT_IE_ADDR_MATCH, ENABLE);
    /* wait for addr match */
    do {
        if(timeout-- == 0){
            return I2C_TIMEOUT;
        }
    } while(addr_match == 0);

    do {
        if(timeout-- == 0) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status(I2C0, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status(I2C0, I2C_STATUS_ADDR_MATCH );

    I2C_ACK(I2C0, DISABLE);
    I2C_Read_En(I2C0);
    I2C_Set_RxSize(I2C0, 1);
    I3->TXDATA = I3_SLAVE_ADDRESS1;
    /* wait slave send over and master recv complete */
    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status(I3, I2C_STATUS_TXFIFO_EMPTY));

    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (I2C_Get_Status(I2C0, I2C_STATUS_RXFIFO_EMPTY));

    /* read a data from I2C_DATA */
    slave_addr = I2C0->RXDATA;

    if(slave_addr != I3_SLAVE_ADDRESS1) {
        return I2C_ERROR;
    }

    return I2C_OK;
}

/**
 * @brief handles the I2C0 interrupt
 */
void I3_IRQHandler(void)
{
    if(I2C_Get_IntStatus(I3, I2C_INT_IE_ADDR_MATCH) && I2C_Get_Status(I3, I2C_STATUS_ADDR_MATCH))
    {
        I2C_Interrupt_En(I3, I2C_INT_IE_ADDR_MATCH, DISABLE);
        I2C_Clear_Status(I3, I2C_STATUS_ADDR_MATCH);
        addr_match = 1;
    }
}

/**
 * @brief handles the I2C0 interrupt
 */
void I2C0_IRQHandler(void)
{
    if(I2C_Get_IntStatus(I2C0, I2C_INT_IE_ERROR_ENABLE) && I2C_Get_AdvStatus(I2C0, I2C_ADVANCED_STATUS_ALERT_STUS))
    {
        I2C_Interrupt_En(I2C0, I2C_INT_IE_ERROR, DISABLE);
        I2C_Clear_AdvStatus(I2C0, I2C_ADVANCED_STATUS_ALERT_STUS);
        alert_flag = 1;
    }
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

void main(void)
{
    
    uint32_t timeout = 0xFFFFFFFF;
    uint8_t mod = 0;
    ErrStatus status = ERROR;

    i2c_sys_cfg();

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt I2C0_IRQn */
    ECLIC_Register_IRQ(I2C0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    I2C0_IRQHandler);

    /*register interrupt I2C0_IRQn */
    ECLIC_Register_IRQ(I3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    I3_IRQHandler);

    I2C_Interrupt_En(I2C0, I2C_INT_IE_ERROR, ENABLE);

    /* I2C config */
    I2C_Config();

    if(I2C_OK == i2c_alert_test()){
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

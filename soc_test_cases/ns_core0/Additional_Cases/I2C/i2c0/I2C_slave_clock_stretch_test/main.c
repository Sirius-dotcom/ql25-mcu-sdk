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

#define I3_FIFO_DEPTH  (8)

#define BUFSIZE (I3_FIFO_DEPTH + 1)

#define MOD5(a) ((a) % (5))

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

void MstRecvSlvSend_test(void)
{
    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(I2C0, I3_SLAVE_ADDRESS1, I2C_DIR_READ, 0xFFFFFFFF);
    /* wait for addr match */
    while (!I2C_Get_Status(I3, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I3,I2C_STATUS_ADDR_MATCH );
    while (!I2C_Get_Status(I2C0, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I2C0,I2C_STATUS_ADDR_MATCH );
    /* enable ack and read_en */
    I2C_ACK(I2C0, ENABLE);
    I2C_Read_En(I2C0);
    for (int i = 0; i < BUFSIZE; i++) {
        /* send a data byte */
        for(int j = 0; j < ((i%0xf)*0xf); j++) {
            __NOP();
        }
        if (i == (BUFSIZE - 1)) {
            /* wait for controller exit from ack stage*/
            while(I2C_CHECK_MST_STATE(I2C0, I2C_STATUS_MASTER_STATE_ACK));
            /* disable acknowledge */
            I2C_ACK(I2C0, DISABLE);
            I2C_Stop(I2C0);
        }
        I3->TXDATA = i2c_slv_send[i];
        /* wait slave send over and master recv complete */
        while (!I2C_Get_Status(I3, I2C_STATUS_TXFIFO_EMPTY)) {}
        while (I2C_Get_Status(I2C0, I2C_STATUS_RXFIFO_EMPTY)) {}

        /* read a data from I2C_DATA */
        i2c_mst_recv[i] = I2C0->RXDATA;
    }
    /* send a stop condition to I2C bus */
    I2C_WaitStopOver(I2C0);
}

void MstSendSlvRecv_test(void)
{
    uint32_t addr;
    while (I2C_Get_Status(I2C0, I2C_STATUS_BUSY)) {}

    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(I2C0, I3_SLAVE_ADDRESS1, I2C_DIR_WRITE, 0xFFFFFFFF);
    /* wait for addr match */
    while (!I2C_Get_Status(I3, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I3, I2C_STATUS_ADDR_MATCH);
    while (!I2C_Get_Status(I2C0, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I2C0,I2C_STATUS_ADDR_MATCH );
    
    /* master enable write */
    I2C_Write_En(I2C0);

    for (int i = 0; i < BUFSIZE; i++) {
        /* send a data byte */
        I2C0->TXDATA = i2c_mst_send[i];
        while (I2C_Get_Status(I2C0, I2C_STATUS_TXFIFO_FULL)) {}
    }

    while (!I2C_Get_Status(I3, I2C_STATUS_RXFIFO_FULL)) {}

    for(int j = 0; j < 0xFF; j++) {
        __NOP();
    }

    for (int i = 0; i < BUFSIZE; i++) {
        while (I2C_Get_Status(I3, I2C_STATUS_RXFIFO_EMPTY)) {}
        /* read a data from I2C_DATA */
        i2c_slv_recv[i] = I3->RXDATA;
    }

    /* send a stop condition to I2C bus */
    I2C_Stop(I2C0);
    I2C_WaitStopOver(I2C0);
}

void main(void)
{
    
    uint32_t timeout = 0xFFFFFFFF;
    uint32_t mod = 0;

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
        mod = MOD5(i);
        if(mod == 0){
            i2c_mst_send[i] = 0x11 + i;
            i2c_slv_send[i] = 0x55 + i;
        } else if(mod == 1) {
            i2c_mst_send[i] = 0xff - i;
            i2c_slv_send[i] = 0xa1 + i;
        } else if (mod == 2) {
            i2c_mst_send[i] = 0x22 + i;
            i2c_slv_send[i] = 0x66 + i;
        } else if (mod == 3) {
            i2c_mst_send[i] = 0xee - i;
            i2c_slv_send[i] = 0xb1 + i;
        } else if (mod == 4) {
            i2c_mst_send[i] = 0x33 + i;
            i2c_slv_send[i] = 0x77 + i;
        } else {
            i2c_mst_send[i] = 0xff - i;
            i2c_slv_send[i] = 0xaa + i;
        }
    }
    
    MstRecvSlvSend_test();

    MstSendSlvRecv_test();

    if((SUCCESS == MemoryCompare(i2c_mst_send, i2c_slv_recv, BUFSIZE)) & (SUCCESS == MemoryCompare(i2c_slv_send, i2c_mst_recv, BUFSIZE))){
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

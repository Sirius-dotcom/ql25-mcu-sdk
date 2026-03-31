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

#define I3_FIFO_DEPTH  (8)
#define I3_SLAVE_ADDRESS1 (0x62)
#define I3_SLAVE_ADDRESS2 (0x72)

#define RXSIZE I3_FIFO_DEPTH

#define BUFSIZE (2 * RXSIZE)

uint8_t i2c_mst_send[BUFSIZE] = {0};
uint8_t i2c_slv_recv[BUFSIZE] = {0};

ErrStatus state = ERROR;

volatile int send_index = 0;
volatile int recv_index = 0;
volatile int ind = 0;
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
  *         1.cfg mst tx_watermark = I3_FIFO_DEPTH tx_size = BUFSIZE
  *         2.cfg slv rx_watermark = 0 rx_size = BUFSIZE
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
    init.smbus_cfg.slv_byte_ctrl = ENABLE;
    init.slave_info1.slave_addr = I3_SLAVE_ADDRESS1;
    init.slave_info1.slave_addr_mask = 0x3FF;
    init.slave_info1.slave_addr_en = ENABLE;
    I2C_Init(I3, &init);
}

/**
 * @brief handles the I3 interrupt
 *        once rx_fifo_depth > rx_watermark, it will trigger an interrupt
 */
void I3_IRQHandler(void)
{
    if(I2C_Get_IntStatus(I3, I2C_INT_IE_SBC_ENABLE) && I2C_Get_AdvStatus(I3, I2C_ADVANCED_STATUS_SBC_TCR))
    {
        /* read a data from I2C_DATA */
        for(int i = 0; i < RXSIZE; i++){
            i2c_slv_recv[ind++] = I3->RXDATA;
        }

        for(int j = 0; j < 100; j++){
            __NOP();
        }

        I2C_ACK(I3, ENABLE);
        I2C_Clear_AdvStatus(I3, I2C_ADVANCED_STATUS_SBC_TCR);

        if(ind == BUFSIZE)   I2C_Interrupt_En(I3, I2C_INT_IE_SBC_ENABLE, DISABLE);
    }
}

void i2c_transceive_int_test(void)
{
    /* config slave rxsize and ack cmd */
    I2C_Set_RxSize(I3, RXSIZE);
    I2C_ACK(I3, ENABLE);

    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(I2C0, I3_SLAVE_ADDRESS1, I2C_DIR_WRITE, 0xFFFFFFFF);
    /* wait for addr match */
    while (!I2C_Get_Status(I2C0, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I2C0, I2C_STATUS_ADDR_MATCH );
    
    while (!I2C_Get_Status(I3, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I3, I2C_STATUS_ADDR_MATCH );

    I2C_Set_TxSize(I2C0, BUFSIZE);
    /* master enable write */
    I2C_Write_En(I2C0);
    for (int i = 0; i < BUFSIZE; i++) {
        /* send a data byte to i2c bus */
        I2C0->TXDATA = i2c_mst_send[i];
        while (I2C_Get_Status(I2C0, I2C_STATUS_TXFIFO_FULL)) {}
    }
    while (!I2C_Get_Status(I2C0, I2C_STATUS_EOT)) {}
    /* send a stop condition to I2C bus */
    I2C_Stop(I2C0);
    I2C_WaitStopOver(I2C0);
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
    I2C_RxDma_ContinuousEn(I3, ENABLE);

    /* Global interrupt enable*/
    __enable_irq();

    /*register interrupt I3_IRQn */
    ECLIC_Register_IRQ(I3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    I3_IRQHandler);

    I2C_Interrupt_En(I3, I2C_INT_IE_SBC_ENABLE, ENABLE);

    /* fill the tx buffer */
    for (int i = 0; i < BUFSIZE; i++) {
        i2c_mst_send[i] = i + 0x10;
    }

    i2c_transceive_int_test();

    if(SUCCESS == MemoryCompare(i2c_mst_send, i2c_slv_recv, BUFSIZE)){
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

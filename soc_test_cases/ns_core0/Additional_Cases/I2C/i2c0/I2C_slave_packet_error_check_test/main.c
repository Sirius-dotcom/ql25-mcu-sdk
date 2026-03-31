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
#define BUFSIZE 12

#define RXSIZE 3

uint8_t i2c_slv_send1[BUFSIZE] = {0};
uint8_t i2c_mst_recv1[BUFSIZE] = {0};
uint8_t i2c_slv_send2[BUFSIZE] = {0};
uint8_t i2c_mst_recv2[BUFSIZE] = {0};
uint8_t i2c_slv_send3[BUFSIZE] = {0};
uint8_t i2c_mst_recv3[BUFSIZE] = {0};

uint8_t pec_byte_expt[RXSIZE] = {0x17, 0xf5 ,0x29};
uint8_t pec_byte_send[RXSIZE] = {0};
uint8_t pec_byte_recv[RXSIZE] = {0};

volatile ErrStatus state = ERROR;

volatile uint8_t end_flag = 0;
volatile uint8_t tx_size = 0;

#define I2C0_FIFO_DEPTH  (8)

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
  *         1.cfg mst tx_watermark = I2C0_FIFO_DEPTH tx_size = BUFSIZE
  *         2.cfg slv rx_watermark = 0 rx_size = BUFSIZE
  */
void I2C_Config_Autoend(void)
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
    I2C_Set_RxSize(I2C0, BUFSIZE + 1);

    init.mode = I2C_SLAVE_MODE;
    init.smbus_cfg.slv_byte_ctrl = ENABLE;
    init.slave_info1.slave_addr = I3_SLAVE_ADDRESS1;
    init.slave_info1.slave_addr_mask = 0x3FF;
    init.slave_info1.slave_addr_en = ENABLE;
    I2C_Init(I3, &init);
    I2C_Set_TxSize(I3, BUFSIZE + 1);
}

void I2C_Config_Unautoend(void)
{
    I2C_InitTypeDef init = {0};
    init.speed = I2C_SPEED_100K;
    init.time_cfg = DISABLE;
    init.auto_end = DISABLE;
    init.mode = I2C_MASTER_MODE;
    init.scl_pull = ENABLE;
    init.sda_pull = ENABLE;
    init.smbus_cfg.pec_en = ENABLE;
    I2C_Init(I2C0, &init);
    I2C_Set_RxSize(I2C0, BUFSIZE + 1);

    init.mode = I2C_SLAVE_MODE;
    init.smbus_cfg.slv_byte_ctrl = ENABLE;
    init.slave_info1.slave_addr = I3_SLAVE_ADDRESS1;
    init.slave_info1.slave_addr_mask = 0x3FF;
    init.slave_info1.slave_addr_en = ENABLE;
    I2C_Init(I3, &init);
    I2C_Set_TxSize(I3, BUFSIZE + 1);
}

void i2c_pec_transceive_test_autoend(void)
{
    /* pec request */
    I2C_PEC_Request(I3);
    I2C_PEC_Request(I2C0);

    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(I2C0, I3_SLAVE_ADDRESS1, I2C_DIR_READ, 0xFFFFFFFF);
    /* wait for addr match */
    while (!I2C_Get_Status(I3, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I3, I2C_STATUS_ADDR_MATCH );
    while (!I2C_Get_Status(I2C0, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I2C0, I2C_STATUS_ADDR_MATCH );
    /* enable ack and read_en */
    I2C_ACK(I2C0, ENABLE);
    I2C_Read_En(I2C0);
    for (int i = 0; i < BUFSIZE; i++) {
        /* send a data byte */
        I3->TXDATA = i2c_slv_send1[i];
        /* wait slave send over and master recv complete */
        while (!I2C_Get_Status(I3, I2C_STATUS_TXFIFO_EMPTY)) {}
        while (I2C_Get_Status(I2C0, I2C_STATUS_RXFIFO_EMPTY)) {}
        /* read a data from I2C_DATA */
        i2c_mst_recv1[i] = I2C0->RXDATA;
    }

    /* wait for controller exit from ack stage*/
    while(I2C_CHECK_MST_STATE(I2C0, I2C_STATUS_MASTER_STATE_ACK));

    I2C_ACK(I2C0, DISABLE);
    while(!I2C_Get_Status(I3, I2C_STATUS_EOT));
    I2C_Clear_Status(I3, I2C_STATUS_EOT );
    pec_byte_send[0] = I2C_Get_PECode(I3);
    pec_byte_recv[0] = I2C_Get_PECode(I2C0);
    while(!I2C_Get_Status(I2C0, I2C_STATUS_EOT));
    I2C_Clear_Status(I2C0, I2C_STATUS_EOT );

    /* wait for controller exit from stop stage*/
    while(I2C_CHECK_MST_STATE(I2C0, I2C_STATUS_MASTER_STATE_STOP));
}

/**
 * @brief handles the I2C0 interrupt
 */
void I2C0_IRQHandler(void){}

/**
 * @brief handles the I3 interrupt
 */
void I3_IRQHandler(void)
{
    if(I2C_Get_IntStatus(I3, I2C_INT_IE_EOT_ENABLE) && I2C_Get_Status(I3, I2C_STATUS_EOT))
    {
        pec_byte_send[1] = I2C_Get_PECode(I3);
        pec_byte_recv[1] = I2C_Get_PECode(I2C0);
        I2C_IntStatus_Clear(I3, I2C_STATUS_EOT);
        I2C_Interrupt_En(I3, I2C_INT_IE_EOT, DISABLE);
        end_flag = 1;
    }
}

void i2c_pec_transceive_test_unautoend(void)
{
    /* pec request */
    I2C_PEC_Request(I3);
    I2C_PEC_Request(I2C0);
    /* clear rxffio(optional) */
    I2C_RxChannel_Clear(I2C0);

    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(I2C0, I3_SLAVE_ADDRESS1, I2C_DIR_READ, 0xFFFFFFFF);
    /* wait for addr match */
    while (!I2C_Get_Status(I2C0, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I2C0, I2C_STATUS_ADDR_MATCH );
    while (!I2C_Get_Status(I3, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I3, I2C_STATUS_ADDR_MATCH );

    /* enable ack and read_en */
    I2C_ACK(I2C0, ENABLE);
    I2C_Read_En(I2C0);
    for (int i = 0; i < BUFSIZE; i++) {
        
        /* send a data byte to i2c bus */
        I3->TXDATA = (end_flag ? i2c_slv_send3[i] : i2c_slv_send2[i]);
        while (!I2C_Get_Status(I3, I2C_STATUS_TXFIFO_EMPTY)) {}
        while (I2C_Get_Status(I2C0, I2C_STATUS_RXFIFO_EMPTY)) {}
        if (end_flag) {
            i2c_mst_recv3[i] = I2C0->RXDATA;
        } else {
            i2c_mst_recv2[i] = I2C0->RXDATA;
        }
    }
    if(end_flag == 1) {
        while (!I2C_Get_Status(I2C0, I2C_STATUS_EOT)) {}
        pec_byte_send[2] = I2C_Get_PECode(I3);
        pec_byte_recv[2] = I2C_Get_PECode(I2C0);
        I2C_Clear_Status(I2C0, I2C_STATUS_EOT);
    }
    do {
        tx_size = I2C_Get_TxSize(I3);
        if(tx_size == 1) {
            /* wait for controller exit from ack stage*/
            while(I2C_CHECK_MST_STATE(I2C0, I2C_STATUS_MASTER_STATE_ACK));
            I2C_ACK(I2C0, DISABLE);
            I2C_Stop(I2C0);
            break;
        } 
    } while (tx_size > 0);
    
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

    /* fill the tx buffer */
    for (int i = 0; i < BUFSIZE; i++) {
        mod = MOD5(i);
        if(mod == 0){
            i2c_slv_send1[i] = 0x11 + i;
            i2c_slv_send2[i] = 0x22 + i;
            i2c_slv_send3[i] = 0x33 + i;
        } else if(mod == 1) {
            i2c_slv_send1[i] = 0xff - i;
            i2c_slv_send2[i] = 0xee - i;
            i2c_slv_send3[i] = 0xdd - i;
        } else if (mod == 2) {
            i2c_slv_send1[i] = 0x22 + i;
            i2c_slv_send2[i] = 0x33 + i;
            i2c_slv_send3[i] = 0x44 + i;
        } else if (mod == 3) {
            i2c_slv_send1[i] = 0xee - i;
            i2c_slv_send2[i] = 0xdd - i;
            i2c_slv_send3[i] = 0xcc + i;
        } else if (mod == 4) {
            i2c_slv_send1[i] = 0x33 + i;
            i2c_slv_send2[i] = 0x44 + i;
            i2c_slv_send3[i] = 0x55 + i;
        } else {
            i2c_slv_send1[i] = 0xff - i;
            i2c_slv_send2[i] = 0xff - i;
            i2c_slv_send3[i] = 0xff - i;
        }
    }

    i2c_sys_cfg();

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt I2C0_IRQn */
    ECLIC_Register_IRQ(I2C0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    I2C0_IRQHandler);
    /*register interrupt I3_IRQn */
    ECLIC_Register_IRQ(I3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 2, 0,
                                    I3_IRQHandler);

    /* I2C config */
    I2C_Config_Autoend();

    i2c_pec_transceive_test_autoend();

    /* wait for autoend send over */
    while(I2C_Get_Status(I2C0, I2C_STATUS_BUSY)){}

    I2C_Config_Unautoend();
    I2C_Interrupt_En(I3, I2C_INT_IE_EOT_ENABLE, ENABLE);

    i2c_pec_transceive_test_unautoend();

    while(end_flag != 1) {}
    i2c_sys_cfg();
    I2C_Config_Unautoend();
    i2c_pec_transceive_test_unautoend();

    if(SUCCESS == MemoryCompare(i2c_slv_send1, i2c_mst_recv1, BUFSIZE) &&   \
        SUCCESS == MemoryCompare(i2c_slv_send2, i2c_mst_recv2, BUFSIZE) &&  \
        SUCCESS == MemoryCompare(i2c_slv_send3, i2c_mst_recv3, BUFSIZE) &&  \
        SUCCESS == MemoryCompare(pec_byte_recv, pec_byte_send, RXSIZE)){
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

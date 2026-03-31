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
#define BUFSIZE 12

uint8_t i2c_mst_send[BUFSIZE] = {0};
uint8_t i2c_slv_recv[BUFSIZE] = {0};

volatile ErrStatus state = ERROR;

#define I2C1_FIFO_DEPTH  (8)

volatile int send_index = 0;
volatile int recv_index = 0;

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
    iomux_ls_iof_ival_cfg(IOMUX_BASE,I2C1_SCL_IOF_IVAL, I2C1_SCL_PAD_SEL, I2C1_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I2C1_SCL_IOF_OVAL, I2C1_SCL_PAD_SEL,  I2C1_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,I2C1_SDA_IOF_IVAL, I2C1_SDA_PAD_SEL, I2C1_SDA_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I2C1_SDA_IOF_OVAL, I2C1_SDA_PAD_SEL,  I2C1_SDA_HS_SEL, 0, 0);

    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I2C1_SCL_PAD_SEL, I2C1_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I2C1_SDA_PAD_SEL, I2C1_SDA_HS_SEL, 0, 0);

    iomux_ls_iof_ival_cfg(IOMUX_BASE,I3_SCL_IOF_IVAL, I3_SCL_PAD_SEL,I3_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I3_SCL_IOF_OVAL, I3_SCL_PAD_SEL,I3_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,I3_SDA_IOF_IVAL, I3_SDA_PAD_SEL,I3_SDA_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I3_SDA_IOF_OVAL, I3_SDA_PAD_SEL,I3_SDA_HS_SEL, 0, 0);

    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I3_SCL_PAD_SEL, I3_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I3_SDA_PAD_SEL, I3_SDA_HS_SEL, 0, 0);
}

/**
  * \brief cofigure the I2C1 and I3 interfaces
  *         1.cfg mst tx_watermark = I2C1_FIFO_DEPTH tx_size = BUFSIZE
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
    
    I2C_Init(I2C1, &init);
    I2C_Set_TxWaterMark(I2C1, I2C1_FIFO_DEPTH);
    I2C_Set_TxSize(I2C1, BUFSIZE);

    init.mode = I2C_SLAVE_MODE;
    init.slave_info1.slave_addr = I3_SLAVE_ADDRESS1;
    init.slave_info1.slave_addr_mask = 0x3FF;
    init.slave_info1.slave_addr_en = ENABLE;
    I2C_Init(I3, &init);
    I2C_Set_RxWaterMark(I3, 0);
    I2C_Set_RxSize(I3, BUFSIZE);
}

/**
 * @brief handles the I2C1 interrupt
 *        once tx_fifo_depth <  tx_watermark, it will trigger an interrupt
 */
void I2C1_IRQHandler(void)
{
    if(I2C_Get_IntStatus(I2C1, I2C_INT_IE_TXFIFO_WM_ENABLE) && I2C_Get_Status(I2C1, I2C_STATUS_TXFIFO_WM))
    {
        I2C1->TXDATA = i2c_mst_send[send_index];
        send_index ++;
        if(send_index == BUFSIZE) {
            I2C_Interrupt_En(I2C1, I2C_INT_IE_TXFIFO_WM_ENABLE, DISABLE);
        }
    }
}

/**
 * @brief handles the I3 interrupt
 *        once rx_fifo_depth > rx_watermark, it will trigger an interrupt
 */
void I3_IRQHandler(void)
{
    if(I2C_Get_IntStatus(I3, I2C_INT_IE_RXFIFO_WM_ENABLE) && I2C_Get_Status(I3, I2C_STATUS_RXFIFO_WM))
    {   
        i2c_slv_recv[recv_index] = I3->RXDATA;
        recv_index ++;
        if(recv_index == BUFSIZE) {
            I2C_Interrupt_En(I3, I2C_INT_IE_RXFIFO_WM_ENABLE, DISABLE);
        }
    }
}

void i2c_transceive_int_test(void)
{
    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(I2C1, I3_SLAVE_ADDRESS1, I2C_DIR_WRITE, 0xFFFFFFFF);
    /* wait for addr match */
    while (!I2C_Get_Status(I3, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I3, I2C_STATUS_ADDR_MATCH);
    while (!I2C_Get_Status(I2C1, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I2C1, I2C_STATUS_ADDR_MATCH );
    
    /* master enable write */
    I2C_Write_En(I2C1);
    /* enable int and transceive data via int*/
    I2C_Interrupt_En(I2C1, I2C_INT_IE_TXFIFO_WM_ENABLE, ENABLE);
    I2C_Interrupt_En(I3, I2C_INT_IE_RXFIFO_WM_ENABLE, ENABLE);
    /* wait for mst write over */
    while(!I2C_Get_Status(I2C1, I2C_STATUS_EOT));
    I2C_Stop(I2C1);
    I2C_WaitStopOver(I2C1);
    /* wait for slv read over */
    while(!I2C_Get_Status(I3, I2C_STATUS_EOT));
}

void main(void)
{
    
    uint32_t timeout = 0xFFFFFFFF;

    #ifdef MISC_HAS_I2C1_HAS_CLK
    i2c1_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_I2C1_RST
    i2c1_set_rst(DISABLE);
    i2c1_set_rst(ENABLE);
    #endif
    
    iomux_config();
    /* I2C config */
    I2C_Config();

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt I2C1_IRQn */
    ECLIC_Register_IRQ(I2C1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    I2C1_IRQHandler);
    /*register interrupt I3_IRQn */
    ECLIC_Register_IRQ(I3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 2, 0,
                                    I3_IRQHandler);

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

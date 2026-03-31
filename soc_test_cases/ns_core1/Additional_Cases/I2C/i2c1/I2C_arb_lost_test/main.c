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

#define ARB_DATA    0x57
#define PRIOR_DATA  0x00

#define I2C1_SLAVE_ADDRESS1 (0x62)
#define I2C1_SLAVE_ADDRESS2 (0x72)
#define BUFSIZE 12

uint8_t i2c_mst_send[BUFSIZE] = {0};
uint8_t i2c_slv_recv[BUFSIZE] = {0};

volatile ErrStatus state = ERROR;
volatile ErrStatus arb_flag = ERROR;

#define I2C1_FIFO_DEPTH  (8)

int send_index = 0;
int recv_index = 0;

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
    
    I2C_Init(I3, &init);

    init.mode = I2C_SLAVE_MODE;
    init.slave_info1.slave_addr = I2C1_SLAVE_ADDRESS1;
    init.slave_info1.slave_addr_mask = 0x3FF;
    init.slave_info1.slave_addr_en = ENABLE;
    I2C_Init(I2C1, &init);
}

/**
 * @brief handles the I2C1 interrupt
 *        once tx_fifo_depth <  tx_watermark, it will trigger an interrupt
 */
void I2C1_IRQHandler(void)
{
    if(I2C_Get_IntStatus(I2C1, I2C_INT_IE_ARBITRATION_LOST_ENABLE) && I2C_Get_Status(I2C1, I2C_STATUS_ARB_LOST))
    {
        I2C_Interrupt_En(I2C1, I2C_INT_IE_ARBITRATION_LOST_ENABLE, DISABLE);
        I2C_Clear_Status(I2C1,I2C_STATUS_ARB_LOST);
        if ((I2C1->SETUP & I2C_SETUP_ROLE_MODE) == I2C_SETUP_ROLE_MODE_SLAVE) arb_flag = SUCCESS;
    }
}

static inline void i2c_arb_lost_test(void)
{
    I2C_InitTypeDef init = {0};
    init.speed = I2C_SPEED_100K;
    init.time_cfg = DISABLE;
    init.auto_end = DISABLE;
    init.mode = I2C_MASTER_MODE;
    init.scl_pull = ENABLE;
    init.sda_pull = ENABLE;
    I2C_Init(I2C1, &init);
    I2C_Init(I3, &init);

    I2C1->TXDATA = ARB_DATA;
    I3->TXDATA = PRIOR_DATA;
    I2C1->SETUP |= I2C_SETUP_START;
    I3->SETUP |= I2C_SETUP_START;
    while (I2C1->SETUP & I2C_SETUP_START){}
    while (I3->SETUP & I2C_SETUP_START){}
    I2C_Write_En(I2C1);
    I2C_Write_En(I3);

    while(ERROR == arb_flag);
    I2C_Stop(I3);
    I2C_WaitStopOver(I3);
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

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt I2C1_IRQn */
    ECLIC_Register_IRQ(I2C1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    I2C1_IRQHandler);

    I2C_Interrupt_En(I2C1, I2C_INT_IE_ARBITRATION_LOST_ENABLE, ENABLE);

    /* fill the tx buffer */
    for (int i = 0; i < BUFSIZE; i++) {
        i2c_mst_send[i] = i + 0x10;
    }

    i2c_arb_lost_test();

    /* I2C config */
    I2C_Config();

    I2C_MasterSendSlaveRecv_Onebyone(I3, I2C1, I2C1_SLAVE_ADDRESS1, i2c_mst_send, i2c_slv_recv, BUFSIZE);

    if(SUCCESS == arb_flag && SUCCESS == MemoryCompare(i2c_mst_send, i2c_slv_recv, BUFSIZE)){
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

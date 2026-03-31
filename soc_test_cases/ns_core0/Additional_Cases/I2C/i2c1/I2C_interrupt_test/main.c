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

#define ARB_DATA    0x57
#define PRIOR_DATA  0x00

#define I2C1_FIFO_DEPTH  (8)

#define BUFFER_SIZE 3

uint8_t i2c_mst_send[BUFFER_SIZE] = {0};
uint8_t i2c_slv_recv[BUFFER_SIZE] = {0};
uint8_t i2c_mst_recv[BUFFER_SIZE] = {0};
uint8_t i2c_slv_send[BUFFER_SIZE] = {0};
volatile uint8_t tx_wm_int_flag1 = 0;
volatile uint8_t rx_wm_int_flag1 = 0;
volatile uint8_t eot_int_flag1= 0;
volatile uint8_t addr_match1 = 0;
volatile uint8_t tx_wm_int_flag2 = 0;
volatile uint8_t rx_wm_int_flag2 = 0;
volatile uint8_t eot_int_flag2= 0;
volatile uint8_t addr_match2 = 0;
volatile ErrStatus state = ERROR;
volatile ErrStatus int_state = ERROR;
volatile ErrStatus arb_flag = ERROR;

I2C_InitTypeDef init = {0};

typedef enum {
    TEST_COMPLETE = 0,
    MST_EOT = 1,
    MST_ARBT_LOST,
    MST_TX_WM,
    MST_RX_WM,
    MST_TIME_OUT,
    MST_ADDR_MATCH,
    MST_NACK_RECP,
    TEST_MAX
} TestProj_TypedefEnum;

volatile TestProj_TypedefEnum test_cs = MST_ARBT_LOST;

volatile uint8_t test_flag = 1;

/**
 * @brief handles the I2C1 interrupt
 */
void I2C1_IRQHandler(void)
{
    test_flag = 0;
    if(I2C_Get_IntStatus(I2C1, I2C_INT_IE_ARBITRATION_LOST_ENABLE) && I2C_Get_Status(I2C1, I2C_STATUS_ARB_LOST))
    {
        
        I2C_IntStatus_Clear(I2C1, I2C_STATUS_ARB_LOST);
        I2C_Interrupt_En(I2C1, I2C_INT_IE_ARBITRATION_LOST, DISABLE);
        arb_flag = SUCCESS;
    }
    if(I2C_Get_IntStatus(I2C1, I2C_INT_IE_NACK_RECEPTION_ENABLE) && I2C_Get_Status(I2C1, I2C_STATUS_NACK))
    {
        
        I2C_IntStatus_Clear(I2C1, I2C_STATUS_NACK);
        I2C_Interrupt_En(I2C1, I2C_INT_IE_NACK_RECEPTION, DISABLE);
    }
    if(I2C_Get_IntStatus(I2C1, I2C_INT_IE_ADDR_MATCH_ENABLE) && I2C_Get_Status(I2C1, I2C_STATUS_ADDR_MATCH))
    {
        
        I2C_IntStatus_Clear(I2C1, I2C_STATUS_ADDR_MATCH);
        I2C_Interrupt_En(I2C1, I2C_INT_IE_ADDR_MATCH, DISABLE);
        addr_match1 = 1;
    }
    if(I2C_Get_IntStatus(I2C1, I2C_INT_IE_TXFIFO_WM_ENABLE) && I2C_Get_Status(I2C1, I2C_STATUS_TXFIFO_WM))
    {
        
        I2C_Interrupt_En(I2C1, I2C_INT_IE_TXFIFO_WM, DISABLE);
        test_cs = MST_RX_WM;
    }
    if(I2C_Get_IntStatus(I2C1, I2C_INT_IE_RXFIFO_WM_ENABLE) && I2C_Get_Status(I2C1, I2C_STATUS_RXFIFO_WM))
    {
        
        I2C_Interrupt_En(I2C1, I2C_INT_IE_RXFIFO_WM, DISABLE);
        test_cs = TEST_COMPLETE;
    }
    if(I2C_Get_IntStatus(I2C1, I2C_INT_IE_EOT_ENABLE) && I2C_Get_Status(I2C1, I2C_STATUS_EOT))
    {
        
        I2C_IntStatus_Clear(I2C1, I2C_STATUS_EOT);
        I2C_Interrupt_En(I2C1, I2C_INT_IE_EOT, DISABLE);
        test_cs = MST_TIME_OUT;
    }
    if(I2C_Get_IntStatus(I2C1, I2C_INT_IE_TIME_OUT_ENABLE) && I2C_Get_Status(I2C1, I2C_STATUS_TIME_OUT))
    {
        
        I2C_TimeOut_En(I2C1, DISABLE);
        I2C_IntStatus_Clear(I2C1, I2C_STATUS_TIME_OUT);
        I2C_Interrupt_En(I2C1, I2C_INT_IE_TIME_OUT, DISABLE);
        test_cs = MST_TX_WM;
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

/*!
    \brief      cofigure the iomux ports.
    \param[in]  none
    \param[out] none
    \retval     none
*/

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,I2C1_SCL_IOF_IVAL, I2C1_SCL_PAD_SEL, I2C1_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I2C1_SCL_IOF_OVAL, I2C1_SCL_PAD_SEL,  I2C1_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,I2C1_SDA_IOF_IVAL, I2C1_SDA_PAD_SEL, I2C1_SDA_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I2C1_SDA_IOF_OVAL, I2C1_SDA_PAD_SEL,  I2C1_SDA_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,I2C1_SMBALERT_IOF_IVAL, I2C1_SMBALERT_PAD_SEL, I2C1_SMBALERT_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I2C1_SMBALERT_IOF_OVAL, I2C1_SMBALERT_PAD_SEL, I2C1_SMBALERT_HS_SEL, 0, 0);

    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I2C1_SCL_PAD_SEL, I2C1_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I2C1_SDA_PAD_SEL, I2C1_SDA_HS_SEL, 0, 0);
    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I2C1_SMBALERT_PAD_SEL, I2C1_SMBALERT_HS_SEL, 0, 0);

    iomux_ls_iof_ival_cfg(IOMUX_BASE,I3_SCL_IOF_IVAL, I3_SCL_PAD_SEL,I3_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I3_SCL_IOF_OVAL, I3_SCL_PAD_SEL,I3_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,I3_SDA_IOF_IVAL, I3_SDA_PAD_SEL,I3_SDA_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I3_SDA_IOF_OVAL, I3_SDA_PAD_SEL,I3_SDA_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,I3_SMBALERT_IOF_IVAL, I3_SMBALERT_PAD_SEL, I3_SMBALERT_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,I3_SMBALERT_IOF_OVAL, I3_SMBALERT_PAD_SEL, I3_SMBALERT_HS_SEL, 0, 0);

    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I3_SCL_PAD_SEL, I3_SCL_HS_SEL, 0, 0);
    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I3_SDA_PAD_SEL, I3_SDA_HS_SEL, 0, 0);
    iomux_ls_iof_pullup_cfg(IOMUX_BASE,I3_SMBALERT_PAD_SEL, I3_SMBALERT_HS_SEL, 0, 0);
}

/**
  * \brief cofigure the I2C1 and I3 interfaces
  */
void I2C_Config(void)
{
    init.speed = I2C_SPEED_100K;
    init.time_cfg = DISABLE;
    init.auto_end = DISABLE;
    init.mode = I2C_MASTER_MODE;
    init.scl_pull = ENABLE;
    init.sda_pull = ENABLE;
    I2C_Init(I2C1, &init);

    init.mode = I2C_SLAVE_MODE;
    init.slave_info1.slave_addr = I3_SLAVE_ADDRESS1;
    init.slave_info1.slave_addr_mask = 0x3FF;
    init.slave_info1.slave_addr_en = ENABLE;
    I2C_Init(I3, &init);
}

static inline void addr_match_test(void)
{
    
    I2C1->TXDATA = (I3_SLAVE_ADDRESS1 << 1 | I2C_DIR_READ);
    /* send a start condition to I2C bus */
    I2C_Start(I2C1);

    while(addr_match1 == 0);

    I2C_Read_En(I2C1);
    I2C_ACK(I2C1, DISABLE);
    I3->TXDATA = i2c_slv_send[0];
    while (!I2C_Get_Status(I3, I2C_STATUS_TXFIFO_EMPTY)) {}
    while (!I2C_Get_Status(I2C1,I2C_STATUS_BYTE_DONE)) {}
    i2c_mst_recv[0] = I2C1->RXDATA;
    I2C_Stop(I2C1);
    I2C_WaitStopOver(I2C1);
    test_cs = MST_EOT;
}

static inline void i2c_arb_lost_test(void)
{
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
    test_cs = MST_NACK_RECP;
}

static inline void i2c_nack_recept_test(void)
{
    uint32_t data;
    while (I2C_Get_Status(I2C1, I2C_STATUS_BUSY)) {}

    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(I2C1, I3_SLAVE_ADDRESS1, I2C_DIR_WRITE, 0xFFFFFFFF);
    /* wait for addr match */
    while (!I2C_Get_Status(I3, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I3, I2C_STATUS_ADDR_MATCH);
    while (!I2C_Get_Status(I2C1, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I2C1,I2C_STATUS_ADDR_MATCH );
    
    /* master enable write */
    I2C_Write_En(I2C1);
    I2C_ACK(I3, DISABLE);
    /* send a data byte */
    I2C1->TXDATA = 0x13;
    while (I2C_Get_Status(I3, I2C_STATUS_RXFIFO_EMPTY)) {}
        /* read a data from I2C_DATA */
    data = I3->RXDATA;

    /* send a stop condition to I2C bus */
    I2C_Stop(I2C1);
    I2C_WaitStopOver(I2C1);
    test_cs = MST_ADDR_MATCH;
}

static inline void i2c_rx_watermark_test(I2C_TypeDef* I2Cx_m, I2C_TypeDef* I2Cx_s, uint8_t slave_addr, uint8_t* send, uint8_t* recv, uint32_t len)
{
    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(I2Cx_m, slave_addr, I2C_DIR_READ, I2C_TIMEOUT_VAL);

    /* wait for addr match */
    while (!I2C_Get_Status(I2Cx_s, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I2Cx_s, I2C_STATUS_ADDR_MATCH);
    while (!I2C_Get_Status(I2Cx_m, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(I2Cx_m,I2C_STATUS_ADDR_MATCH );

    /* enable ack and read_en */
    I2C_ACK(I2Cx_m, ENABLE);
    I2C_Read_En(I2Cx_m);
    for (int i = 0; i < len; i++) {
        if (i == len-1) {
            /* wait for controller exit from ack stage*/
            while(I2C_CHECK_MST_STATE(I2Cx_m, I2C_STATUS_MASTER_STATE_ACK));
            I2C_ACK(I2Cx_m, DISABLE);
            I2C_Stop(I2Cx_m);
        }
        /* send a data byte */
        I2Cx_s->TXDATA = send[i];
        /* wait slave send over and master recv complete */
        while (!I2C_Get_Status(I2Cx_s, I2C_STATUS_TXFIFO_EMPTY));

        /* read a data from I2C_DATA */
        recv[i] = I2Cx_m->RXDATA;
    }

    I2C_WaitStopOver(I2Cx_m);
}

void main(void)
{
    
    int i;
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
    
    /* fill the tx buffer */
    for (int i = 0; i < BUFFER_SIZE; i++) {
        i2c_mst_send[i] = i + 0x10;
        i2c_slv_send[i] = i + 0x20;
    }

    while(1) {
        switch(test_cs) {
            case MST_ARBT_LOST:
                test_cs = TEST_MAX;
                I2C_Interrupt_En(I2C1, I2C_INT_IE_ARBITRATION_LOST, ENABLE);
                i2c_arb_lost_test();
                while(test_flag);
                test_flag = 1;
                break;
            case MST_TIME_OUT:
                test_cs = TEST_MAX;
                I2C_TimeOut_En(I2C1, ENABLE);
                I2C_TimeOut_Cfg(I2C1, 0x10);
                I2C1->TXDATA = 0x00;
                I2C_Start(I2C1);
                I2C_Stop(I2C1);
                I2C_Interrupt_En(I2C1, I2C_INT_IE_TIME_OUT, ENABLE);
                I2C_WaitStopOver(I2C1);
                while(test_flag);
                test_flag = 1;
                break;
            case MST_NACK_RECP:
                test_cs = TEST_MAX;
                I2C_Config();
                I2C_Interrupt_En(I2C1, I2C_INT_IE_NACK_RECEPTION, ENABLE);
                
                i2c_nack_recept_test();
                while(test_flag);
                test_flag = 1;
                break;
            case MST_ADDR_MATCH:
                test_cs = TEST_MAX;
                I2C_Interrupt_En(I2C1, I2C_INT_IE_ADDR_MATCH, ENABLE);
                addr_match_test();

                while(test_flag);
                test_flag = 1;
                break;
            case MST_TX_WM:
                test_cs = TEST_MAX;
                I2C_Interrupt_En(I2C1, I2C_INT_IE_TXFIFO_WM, ENABLE);
                I2C_Set_TxWaterMark(I2C1, I2C1_FIFO_DEPTH);
                while(test_flag);
                test_flag = 1;
                break;
            case MST_RX_WM:
                test_cs = TEST_MAX;
                #ifdef MISC_HAS_I2C1_RST
                i2c1_set_rst(DISABLE);
                i2c1_set_rst(ENABLE);
                #endif
                I2C_Config();
                I2C_Interrupt_En(I2C1, I2C_INT_IE_RXFIFO_WM, ENABLE);
                I2C_Set_RxWaterMark(I2C1, 0);
                i2c_rx_watermark_test(I2C1, I3, I3_SLAVE_ADDRESS1, i2c_slv_send, i2c_mst_recv, 1);
                while(test_flag);
                test_flag = 1;
                break;
            case MST_EOT:
                test_cs = TEST_MAX;
                I2C_Interrupt_En(I2C1, I2C_INT_IE_EOT_ENABLE, ENABLE);
                I2C_Set_RxSize(I2C1, 1);
                I2C_MasterRecvSlaveSend_Onebyone(I2C1, I3, I3_SLAVE_ADDRESS1, i2c_slv_send, i2c_mst_recv, 1);
                I2C_Config();
                while(test_flag);
                test_flag = 1;
                break;
            case TEST_COMPLETE:
                simulation_pass();
                while (1);
                break;
            case TEST_MAX:
                break;
            default:
                test_cs = TEST_MAX;
                simulation_fail();
                while (1) {}
                break;
        }
    }
}

/*
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier Apache-2.0
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

#include "ns_i2c.h"
#include "ns_conf.h"

typedef enum {
    I2C_START = 0,
    I2C_RESTART = 1,
} I2C_StartTypedef;

static I2C_StartTypedef restart_flag = I2C_START;

#define TIMECFG 9
/*PRESC	START_SETUP_TIME START_HOLD_TIME SDA_SETUP_TIME	SDA_HOLD_TIME SCL_HIGH_PERIOD SCL_LOW_PERIOD STOP_SETUP_TIME BUS_FREE_TIME */

static uint16_t timing_cfg[I2C_SYS_CLK_MAX][I2C_SPEED_MAX][TIMECFG] = {
    {/* sys_clk = 8M*/
        {3, 9, 8, 1, 1, 9, 9, 8, 9},    /* i2c_speed = 100K */
        {1, 2, 2, 1, 1, 4, 4, 2, 3},    /* i2c_speed = 400K */
        {0, 2, 2, 1, 1, 3, 3, 2, 2},    /* i2c_speed = 1M */
        {}/* i2c_speed = 3.4M */
    },
    {/* sys_clk = 16M*/
        {4, 15, 13, 1, 1, 15, 15, 13, 16},
        {1,  4,  4, 1, 1,  9,  9,  4,  5},
        {0,  4,  4, 1, 1,  7,  7,  4,  5},
        {}
    },
    {/* sys_clk = 48M*/
        {9, 22, 19, 1, 1, 23, 23, 19, 23},
        {5,  4,  4, 1, 1,  9,  9,  4,  5},
        {3,  3,  3, 1, 1,  5,  5,  3,  4},
        {0,  8,  8, 1, 1,  4,  9,  8,  9}
    },
    {/* sys_clk = 100M*/
        {9, 47, 40, 2, 2, 49, 49, 40, 49},
        {5, 10, 10, 1, 1, 20, 20, 10, 11},
        {4,  5,  5, 1, 1,  9,  9,  5,  6},
        {1,  8,  8, 1, 1,  4,  8,  8,  9}
    }
};

static inline void I2C_Role_Cfg(I2C_TypeDef* I2Cx, I2C_ModeTypedef mode);
static inline void I2C_10BitAddr_En(I2C_TypeDef* I2Cx, ControlStatus status);
static inline void I2C_SDAPull_En(I2C_TypeDef* I2Cx, ControlStatus status);
static inline void I2C_SCLPull_En(I2C_TypeDef* I2Cx, ControlStatus status);
static inline void I2C_Control_En(I2C_TypeDef* I2Cx, ControlStatus status);
static inline void I2C_SlaveAddr_Cfg(I2C_TypeDef* I2Cx, I2C_InitTypeDef* init);

void I2C_Restart_Cmd(void)
{
    restart_flag = I2C_RESTART;
}

/**
  * \brief I2C set rx size
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param rx_size i2c rx size
  */
void I2C_Set_RxSize(I2C_TypeDef* I2Cx, uint32_t rx_size)
{
    
    I2Cx->RX_SIZE = rx_size;
}

/**
  * \brief I2C set rx size
  * \param I2Cx where x can be to select the I2C peripheral.
  * \retval size i2c rx size
  */
uint32_t I2C_Get_RxSize(I2C_TypeDef* I2Cx)
{
    uint32_t rx_size = 0;
    rx_size = I2Cx->RX_SIZE;
    return rx_size;
}

/**
  * \brief I2C set tx size
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param tx_size i2c tx size
  */
void I2C_Set_TxSize(I2C_TypeDef* I2Cx,uint32_t tx_size)
{
    I2Cx->TX_SIZE = tx_size;
}

/**
  * \brief I2C set tx size
  * \param I2Cx where x can be to select the I2C peripheral.
  * \retval size i2c tx size
  */
uint32_t I2C_Get_TxSize(I2C_TypeDef* I2Cx)
{
    uint32_t tx_size = 0;
    tx_size = I2Cx->TX_SIZE;
    return tx_size;
}

/**
  * \brief I2C tx dma mode config
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I2C_TxDma_ContinuousEn(I2C_TypeDef* I2Cx, ControlStatus status)
{
    
    if (ENABLE == status) {
        I2Cx->TX_CFG |= I2C_TX_CFG_CONTINUOUS_ENABLE;
    } else {
        I2Cx->TX_CFG &= ~(I2C_TX_CFG_CONTINUOUS_ENABLE);
    }
}

/**
  * \brief I2C rx dma mode config
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I2C_RxDma_ContinuousEn(I2C_TypeDef* I2Cx, ControlStatus status)
{
    
    if (ENABLE == status) {
        I2Cx->RX_CFG |= I2C_RX_CFG_CONTINUOUS_ENABLE;
    } else {
        I2Cx->RX_CFG &= ~(I2C_RX_CFG_CONTINUOUS_ENABLE);
    }
}

/**
  * \brief I2C tx dma start enable
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I2C_TxDma_Start(I2C_TypeDef* I2Cx, ControlStatus status)
{
    
    if (ENABLE == status) {
        I2Cx->TX_CFG |= I2C_TX_CFG_EN_ENABLE;
    } else {
        I2Cx->TX_CFG &= ~(I2C_TX_CFG_EN_ENABLE);
    }
}

/**
  * \brief I2C rx dma start enable
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I2C_RxDma_Start(I2C_TypeDef* I2Cx, ControlStatus status)
{
    
    if (ENABLE == status) {
        I2Cx->RX_CFG |= I2C_RX_CFG_EN_ENABLE;
    } else {
        I2Cx->RX_CFG &= ~(I2C_RX_CFG_EN_ENABLE);
    }
}

/**
  * \brief I2C tx fifo channel clear
  * \param I2Cx where x can be to select the I2C peripheral.
  * \note Only used in continuous mode(this function causes a soft_reset)
  */
void I2C_TxChannel_Clear(I2C_TypeDef* I2Cx)
{
    I2Cx->TX_CFG |= I2C_TX_CFG_CLR_ENABLE;
}

/**
  * \brief I2C rx fifo channel clear
  * \param I2Cx where x can be to select the I2C peripheral.
  * \note Only used in continuous mode(this function causes a soft_reset)
  */
void I2C_RxChannel_Clear(I2C_TypeDef* I2Cx)
{
    I2Cx->RX_CFG |= I2C_RX_CFG_CLR_ENABLE;
}

/**
  * \brief I2C write enable
  * \param I2Cx where x can be to select the I2C peripheral.
  */
void I2C_Write_En(I2C_TypeDef* I2Cx)
{
    I2Cx->SETUP &= (~I2C_SETUP_DIR_READ);
}

/**
  * \brief I2C read enable
  * \param I2Cx where x can be to select the I2C peripheral.
  */
void I2C_Read_En(I2C_TypeDef* I2Cx)
{
    I2Cx->SETUP |= I2C_SETUP_DIR_READ;
}

/**
  * \brief I2C timeout enable
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I2C_TimeOut_En(I2C_TypeDef* I2Cx, ControlStatus status)
{
    
    if (ENABLE == status) {
        I2Cx->SETUP |= I2C_SETUP_TIME_OUT_ENABLE;
    } else {
        I2Cx->SETUP &= ~(I2C_SETUP_TIME_OUT_ENABLE);
    }
}

/**
  * \brief I2C interrupt enable config
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param interrupt
  *     \arg I2C_EOT_INT_IE_ENABLE
  *     \arg I2C_ARBITRATION_LOST_INT_IE_ENABLE
  *     \arg I2C_TXFIFO_WM_INT_IE_ENABLE
  *     \arg I2C_RXFIFO_WM_INT_IE_ENABLE
  *     \arg I2C_TIME_OUT_INT_IE_ENABLE
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I2C_Interrupt_En(I2C_TypeDef* I2Cx, uint32_t interrupt, ControlStatus status)
{
    
    if (ENABLE == status) {
        I2Cx->INT_IE |= interrupt;
    } else {
        I2Cx->INT_IE &= ~(interrupt);
    }
}

/**
  * \brief I2C interrupt clear
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param starus
  *     \arg I2C_INT_CLR_EOT
  *     \arg I2C_INT_CLR_ARBITRATION_LOST
  *     \arg I2C_INT_CLR_TIME_OUT
  */
void I2C_IntStatus_Clear(I2C_TypeDef* I2Cx, uint32_t starus)
{
    I2Cx->STATUS |= starus;
}

/**
  * \brief I2C Interrupt enable
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param interrupt
  *     \arg I2C_INT_IE_EOT_ENABLE
  *     \arg I2C_INT_IE_ARBITRATION_LOST_INT_IE_ENABLE
  *     \arg I2C_INT_IE_TXFIFO_WM_ENABLE
  *     \arg I2C_INT_IE_RXFIFO_WM_ENABLE
  *     \arg I2C_INT_IE_TIME_OUT_INT_IE_ENABLE
  * \return status
  */
FlagStatus I2C_Get_IntStatus(I2C_TypeDef* I2Cx, uint32_t interrupt)
{
    if (interrupt == ((I2Cx->INT_IE) & interrupt)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief I2C time out num config
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param num time out number
  */
void I2C_TimeOut_Cfg(I2C_TypeDef* I2Cx, uint16_t num)
{
    I2Cx->TIME_OUT = num;
}

/**
  * \brief I2C software reset
  * \param I2Cx where x can be to select the I2C peripheral.
  */
void I2C_SoftReset_En(I2C_TypeDef* I2Cx)
{
    I2Cx->SETUP |= I2C_SETUP_SOFT_RESET;
    I2Cx->SETUP &= ~(I2C_SETUP_SOFT_RESET);
}

/**
  * \brief I2C dma mode enable/disable
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I2C_Dma_Cfg(I2C_TypeDef* I2Cx, ControlStatus status)
{
    if (ENABLE == status) {
        I2Cx->SETUP |= I2C_SETUP_WORK_MODE_DMA;
    } else {
        I2Cx->SETUP &= ~(I2C_SETUP_WORK_MODE_DMA);
    }
}

/**
  * \brief I2C get status
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg I2C_STATUS_BUSY
  *     \arg I2C_STATUS_ARB_LOST
  *     \arg I2C_STATUS_NACK
  *     \arg I2C_STATUS_EOT
  *     \arg I2C_STATUS_BYTE_ON_GOING
  *     \arg I2C_STATUS_TIME_OUT
  *     \arg I2C_STATUS_I2C_SLAVE_RECEIVE
  *     \arg I2C_STATUS_SLAVE_TRANSMIT
  *     \arg I2C_STATUS_TXFIFO_EMPTY
  *     \arg I2C_STATUS_TXFIFO_FULL
  *     \arg I2C_STATUS_RXFIFO_EMPTY
  *     \arg I2C_STATUS_RXFIFO_FULL
  *     \arg I2C_STATUS_TXFIFO_WM
  *     \arg I2C_STATUS_RXFIFO_WM
  *     \arg I2C_STATUS_ADDR_MATCH
  * \return FlagStatus
  */
FlagStatus I2C_Get_Status(I2C_TypeDef* I2Cx, uint32_t status)
{
    if (status == ((I2Cx->STATUS) & status)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief I2C clear status
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg I2C_STATUS_ARB_LOST
  *     \arg I2C_STATUS_EOT
  *     \arg I2C_STATUS_TIME_OUT
  *     \arg I2C_STATUS_ADDR_MATCH
  */
void I2C_Clear_Status(I2C_TypeDef* I2Cx, uint32_t status)
{
    I2Cx->STATUS |= status;
}

/**
  * \brief I2C send start commond
  * \param I2Cx where x can be to select the I2C peripheral.
  * \retval i2c status
  */
I2C_StateTypedef I2C_Start(I2C_TypeDef* I2Cx)
{
    uint32_t timeout = I2C_TIMEOUT_VAL;

    do {
        if (timeout -- == 0U) {
            return I2C_TIMEOUT;
        } 
    } while (I2C_Get_Status(I2Cx, I2C_STATUS_BUSY));

    I2Cx->SETUP |= I2C_SETUP_START;
    do {
        if (timeout -- == 0U) {
            return I2C_TIMEOUT;
        } 
    } while (I2Cx->SETUP & I2C_SETUP_START);
    return I2C_OK;
}

/**
  * \brief I2C send restart commond
  * \param I2Cx where x can be to select the I2C peripheral.
  */
void I2C_Restart(I2C_TypeDef* I2Cx)
{
    I2Cx->SETUP |= I2C_SETUP_START;
}

/**
  * \brief I2C send stop commond
  * \param I2Cx where x can be to select the I2C peripheral.
  */
void I2C_Stop(I2C_TypeDef* I2Cx)
{
    I2Cx->SETUP |= I2C_SETUP_STOP;
}

/**
  * \brief I2C get stop send status
  * \param I2Cx where x can be to select the I2C peripheral.
  * \retval i2c status
  */
I2C_StateTypedef I2C_WaitStopOver(I2C_TypeDef* I2Cx)
{
    uint32_t timeout = I2C_TIMEOUT_VAL;

    do {
        if (timeout -- == 0U) {
            return I2C_TIMEOUT;
        } 
    } while (I2Cx->SETUP & I2C_SETUP_STOP);

    return I2C_OK;
}

/**
  * \brief I2C ACK ENABLE
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I2C_ACK(I2C_TypeDef* I2Cx, ControlStatus status)
{
    if (ENABLE == status) {
        I2Cx->SETUP &= ~(I2C_SETUP_ACK);
    } else {
        I2Cx->SETUP |= I2C_SETUP_ACK;
    }
}

/**
  * \brief DMA tx data size config
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param size tx data size
  */
void I2C_DmaTxDateSize_Cfg(I2C_TypeDef* I2Cx, I2C_DmaDataSizeTypedef size)
{
    I2Cx->DMA_TX_DATASIZE = size;
}

/**
  * \brief DMA rx data size config
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param size rx data size
  */
void I2C_DmaRxDateSize_Cfg(I2C_TypeDef* I2Cx, I2C_DmaDataSizeTypedef size)
{
    I2Cx->DMA_RX_DATASIZE = size;
}

void I2C_Calcu_Time(I2C_TypeDef* I2Cx, I2C_InitTypeDef* init)
{
    assert_param(IS_I2C_SPEED(init->speed));

    uint16_t sys_clk = 0;

    init->timecfg.presc = timing_cfg[sys_clk][init->speed][0];
    init->timecfg.start_setup_time = timing_cfg[sys_clk][init->speed][1];
    init->timecfg.start_hold_time = timing_cfg[sys_clk][init->speed][2];
    init->timecfg.sda_setup_time = timing_cfg[sys_clk][init->speed][3];
    init->timecfg.sda_hold_time = timing_cfg[sys_clk][init->speed][4];
    init->timecfg.scl_high_period = timing_cfg[sys_clk][init->speed][5];
    init->timecfg.scl_low_period = timing_cfg[sys_clk][init->speed][6];
    init->timecfg.stop_setup_time = timing_cfg[sys_clk][init->speed][7];
    init->timecfg.bus_free_time = timing_cfg[sys_clk][init->speed][8];
}

void I2C_Set_Time(I2C_TypeDef* I2Cx, I2C_InitTypeDef* init)
{
    I2Cx->TIMING = init->timecfg.presc;
    I2Cx->START_SETUP_TIME = init->timecfg.start_setup_time;
    I2Cx->START_HOLD_TIME = init->timecfg.start_hold_time;
    I2Cx->SDA_SETUP_TIME = init->timecfg.sda_setup_time;
    I2Cx->SDA_HOLD_TIME = init->timecfg.sda_hold_time;
    I2Cx->SCL_HIGH_PERIOD = init->timecfg.scl_high_period;
    I2Cx->SCL_LOW_PERIOD = init->timecfg.scl_low_period;
    I2Cx->STOP_SETUP_TIME = init->timecfg.stop_setup_time;
    I2Cx->BUS_FREE_TIME = init->timecfg.bus_free_time;
}
/**
  * \brief  Initializes the I2Cx peripheral according to the specified
  *         parameters in the I2C_InitStruct.
  * \param  I2Cx where x can be to select the I2C peripheral.
  * \param  init pointer to a I2C_InitTypeDef structure that
  *         contains the configuration information for the specified I2C peripheral.
  */
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* init)
{
    uint32_t presc = 0;
    I2C_SoftReset_En(I2Cx);         
    I2C_Control_En(I2Cx, ENABLE);   
    I2C_Role_Cfg(I2Cx, init->mode); 

    I2C_AutoEnd_En(I2Cx, init->auto_end);

    if(ENABLE == init->time_cfg) { 
        I2C_Calcu_Time(I2Cx, init);
        I2C_Set_Time(I2Cx, init);
    } else {
        I2Cx->TIMING = 0x01;
    }

    I2C_SlaveAddr_Cfg(I2Cx, init);

    I2C_10BitAddr_En(I2Cx, init->addr_10bit);

    if(ENABLE == init->slave_info1.slave_addr_en){ 
        I2Cx->SLAVE_ADDRESS1 = ((init->slave_info1.slave_addr_en << I2C_SLAVE_ADDRESS1_EN_OFS) | 
                                (init->slave_info1.slave_addr_mask << I2C_SLAVE_ADDRESS1_MASK_OFS) | 
                                (init->slave_info1.slave_addr << I2C_SLAVE_ADDRESS1_NUM_OFS));
    }
    if(ENABLE == init->slave_info2.slave_addr_en){ 
        I2Cx->SLAVE_ADDRESS2 = ((init->slave_info2.slave_addr_en << I2C_SLAVE_ADDRESS2_EN_OFS) | 
                                (init->slave_info2.slave_addr_mask << I2C_SLAVE_ADDRESS2_MASK_OFS) | 
                                (init->slave_info2.slave_addr << I2C_SLAVE_ADDRESS2_NUM_OFS));
    }
    if(ENABLE == init->smbus_cfg.slv_byte_ctrl) {
        I2Cx->ADVANCED_SETUP |= I2C_ADVANCED_SETUP_SBCEN_ENABLE;
    } else {
        I2Cx->ADVANCED_SETUP &= ~I2C_ADVANCED_SETUP_SBCEN_ENABLE;
    }    
    if(ENABLE == init->smbus_cfg.pec_en) {
        I2Cx->ADVANCED_SETUP |= I2C_ADVANCED_SETUP_PECEN_ENABLE;
    } else {
        I2Cx->ADVANCED_SETUP &= ~I2C_ADVANCED_SETUP_PECEN_ENABLE;
    }
    if(ENABLE == init->smbus_cfg.smbd_en) {
        I2Cx->ADVANCED_SETUP |= I2C_ADVANCED_SETUP_SMDEN_ENABLE;
    } else {
        I2Cx->ADVANCED_SETUP &= ~I2C_ADVANCED_SETUP_SMDEN_ENABLE;
    }
    if(ENABLE == init->smbus_cfg.smbh_en) {
        I2Cx->ADVANCED_SETUP |= I2C_ADVANCED_SETUP_SMHEN_ENABLE;
    } else {
        I2Cx->ADVANCED_SETUP &= ~I2C_ADVANCED_SETUP_SMHEN_ENABLE;
    }
    if(ENABLE == init->smbus_cfg.alert_en) {
        I2Cx->ADVANCED_SETUP |= I2C_ADVANCED_SETUP_ALERTEN_ENABLE;
    } else {
        I2Cx->ADVANCED_SETUP &= ~I2C_ADVANCED_SETUP_ALERTEN_ENABLE;
    }
    if(ENABLE == init->smbus_cfg.tidle_en) {
        I2Cx->ADVANCED_SETUP |= I2C_ADVANCED_SETUP_TIDLEN_IDLE;
    } else {
        I2Cx->ADVANCED_SETUP &= ~I2C_ADVANCED_SETUP_TIDLEN_IDLE;
    }
    if(ENABLE == init->smbus_cfg.text_en) {
        I2Cx->ADVANCED_SETUP |= (I2C_ADVANCED_SETUP_TEXTEN_ENABLE | I2C_ADVANCED_SETUP_TEXT_NUM(init->smbus_cfg.text_num));
    } else {
        I2Cx->ADVANCED_SETUP &= ~I2C_ADVANCED_SETUP_TEXTEN_ENABLE;
    }

    I2C_SCLPull_En(I2Cx, init->scl_pull);
    I2C_SDAPull_En(I2Cx, init->sda_pull);

    if(ENABLE == init->dma_mode){
        I2C_Dma_Cfg(I2Cx, ENABLE);   
    }
    I2C_DmaTxDateSize_Cfg(I2Cx, init->dma_txdata_size);
    I2C_DmaRxDateSize_Cfg(I2Cx, init->dma_rxdata_size);
}

/**
  * \brief I2C set rx fifo watermark count
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param rx_watermark i2c rx fifo watermark count
  */
void I2C_Set_RxWaterMark(I2C_TypeDef* I2Cx, uint8_t rx_watermark)
{
    I2Cx->RXFIFO_WM = rx_watermark;
}

/**
  * \brief I2C get rx fifo watermark count
  * \param I2Cx where x can be to select the I2C peripheral.
  * \retval rx_watermark
  */
uint8_t I2C_Get_RxWaterMark(I2C_TypeDef* I2Cx)
{
    uint8_t rx_watermark = 0;
    rx_watermark = I2Cx->RXFIFO_WM;
    return rx_watermark;
}

/**
  * \brief I2C set tx fifo watermark count
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param tx_watermark i2c tx fifo watermark count
  */
void I2C_Set_TxWaterMark(I2C_TypeDef* I2Cx,uint8_t tx_watermark)
{
    I2Cx->TXFIFO_WM = tx_watermark;
}

/**
  * \brief I2C get tx fifo watermark count
  * \param I2Cx where x can be to select the I2C peripheral.
  * \retval tx_watermark
  */
uint8_t I2C_Get_TxWaterMark(I2C_TypeDef* I2Cx)
{
    uint8_t tx_watermark = 0;
    tx_watermark = I2Cx->TXFIFO_WM;
    return tx_watermark;
}

/**
  * \brief I2C send slave addr
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param addr slave addr without r/w bit
  * \param dir
  *     \arg I2C_DIR_WRITE
  *     \arg I2C_DIR_READ
  * \param timeout wait status times
  * \retval i2c status
  */
I2C_StateTypedef I2C_Send_SlaveAddr(I2C_TypeDef* I2Cx, uint8_t addr, I2C_DIRTypedef dir, uint32_t timeout)
{
    uint8_t send_data = (addr << 1) | dir;
    I2C_Write_En(I2Cx);
    switch (restart_flag) {
        case I2C_START:
            /* send dev_addr before start_cmd */
            I2Cx->TXDATA = send_data;
            /* send a start condition to I2C bus */
            I2C_Start(I2Cx);
            break;
        case I2C_RESTART:
            /* send a start condition to I2C bus */
            I2C_Restart(I2Cx);
            /* send dev_addr after start_cmd */
            I2Cx->TXDATA = send_data;
            restart_flag = I2C_START;
            break;
        default:
            break;
    }
    
    return I2C_OK;
}

/**
  * \brief I2C write data to slave
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param data write data
  * \param timeout wait status times
  * \retval i2c status
  */
I2C_StateTypedef I2C_Write(I2C_TypeDef* I2Cx, uint8_t data, uint32_t timeout)
{
    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (I2C_Get_Status(I2Cx, I2C_STATUS_TXFIFO_FULL));

    I2Cx->TXDATA = data;
    return I2C_OK;
}

/**
  * \brief I2C write data to slave one by one
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param data write data
  * \param timeout wait status times
  * \retval i2c status
  */
I2C_StateTypedef I2C_Write_Done(I2C_TypeDef* I2Cx, uint8_t data, uint32_t timeout)
{
    /* wait for controller exit from ack stage*/
    while(I2C_CHECK_MST_STATE(I2Cx, I2C_STATUS_MASTER_STATE_ACK));

    I2C_Set_TxSize(I2Cx, 1);
    I2Cx->TXDATA = data;
    I2C_Write_En(I2Cx);
    do { /* wait for dev send over */
        if (timeout -- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status(I2Cx, I2C_STATUS_EOT));
    I2C_Clear_Status(I2Cx, I2C_STATUS_EOT);

    return I2C_OK;
}

/**
  * \brief I2C read data from slave
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param data read data
  * \param timeout wait status times
  * \retval i2c status
  */
I2C_StateTypedef I2C_Read(I2C_TypeDef* I2Cx, uint8_t* data, uint32_t timeout)
{
    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (I2C_Get_Status(I2Cx, I2C_STATUS_RXFIFO_EMPTY));

    *data = I2Cx->RXDATA;
    return I2C_OK;
}

/**
  * \brief I2C auto end mode cfg
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
void I2C_AutoEnd_En(I2C_TypeDef* I2Cx, ControlStatus status)
{
    
    if (ENABLE == status) {
        I2Cx->SETUP |= I2C_SETUP_AUTO_END;
    } else {
        I2Cx->SETUP &= ~(I2C_SETUP_AUTO_END);
    }
}

/* ------------------------------------------------ TEST API ------------------------------------------------ */
/**
  * \brief this api for master receive slave send test case
  * \param I2Cx_m master i2c
  * \param I2Cx_s slave i2c
  * \param slave_addr slave i2c slave addr
  * \param send slave i2c need to send data
  * \param recv master i2c receive data
  * \param len data length, len(send) == len(recv)
  * \retval i2c status
  */
I2C_StateTypedef I2C_MasterRecvSlaveSend_Onebyone(I2C_TypeDef* I2Cx_m, I2C_TypeDef* I2Cx_s, uint8_t slave_addr, uint8_t* send, uint8_t* recv, uint32_t len)
{
    uint32_t timeout = I2C_TIMEOUT_VAL;
    I2C_StateTypedef state;
    /* send slave address to I2C bus */
    state = I2C_Send_SlaveAddr(I2Cx_m, slave_addr, I2C_DIR_READ, I2C_TIMEOUT_VAL);
    if(I2C_OK != state) {
        return state;
    }
    /* wait for addr match */

    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status (I2Cx_s, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status(I2Cx_s,I2C_STATUS_ADDR_MATCH );

    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status(I2Cx_m, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status (I2Cx_m, I2C_STATUS_ADDR_MATCH);

    timeout = I2C_TIMEOUT_VAL;
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
        do {
            if (timeout-- == 0U) {
                return I2C_TIMEOUT;
            }
        } while (!I2C_Get_Status(I2Cx_s, I2C_STATUS_TXFIFO_EMPTY));

        do {
            if (timeout-- == 0U) {
                return I2C_TIMEOUT;
            }
        } while (I2C_Get_Status(I2Cx_m, I2C_STATUS_RXFIFO_EMPTY));

        /* read a data from I2C_DATA */
        recv[i] = I2Cx_m->RXDATA;
    }
    /* send a stop condition to I2C bus */

    state = I2C_WaitStopOver(I2Cx_m);
    if(I2C_OK != state) {
        return state;
    }

    return I2C_OK;
}

/**
  * \brief this api for master send slave receive test case
  * \param I2Cx_m master i2c
  * \param I2Cx_s slave i2c
  * \param slave_addr slave i2c slave addr
  * \param send master i2c need to send data
  * \param recv slave i2c receive data
  * \param len data length, len(send) == len(recv)
  * \retval i2c status
  */
I2C_StateTypedef I2C_MasterSendSlaveRecv_Onebyone(I2C_TypeDef* I2Cx_m, I2C_TypeDef* I2Cx_s, uint8_t slave_addr, uint8_t* send, uint8_t* recv, uint32_t len)
{
    uint32_t timeout = I2C_TIMEOUT_VAL;
    I2C_StateTypedef state;

    do {
        if (timeout -- == 0U) {
            return I2C_TIMEOUT;
        } 
    } while (I2C_Get_Status(I2Cx_m, I2C_STATUS_BUSY));

    /* send slave address to I2C bus */
    state = I2C_Send_SlaveAddr(I2Cx_m, slave_addr, I2C_DIR_WRITE, I2C_TIMEOUT_VAL);
    if(I2C_OK != state) {
        return state;
    }

    timeout = I2C_TIMEOUT_VAL;
    /* wait for addr match */
    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status (I2Cx_s, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status(I2Cx_s,I2C_STATUS_ADDR_MATCH );

    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status(I2Cx_m, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status (I2Cx_m, I2C_STATUS_ADDR_MATCH);

    timeout = I2C_TIMEOUT_VAL;
    /* master enable write */
    I2C_Write_En(I2Cx_m);
    for (int i = 0; i < len; i++) {
        /* send a data byte */
        I2Cx_m->TXDATA = send[i];
        
        do {
            if (timeout-- == 0U) {
                return I2C_TIMEOUT;
            }
        } while (I2C_Get_Status(I2Cx_s, I2C_STATUS_RXFIFO_EMPTY));
        /* read a data from I2C_DATA */
        recv[i] = I2Cx_s->RXDATA;
    }
    /* send a stop condition to I2C bus */
    I2C_Stop(I2Cx_m);
    state = I2C_WaitStopOver(I2Cx_m);
    if(I2C_OK != state) {
        return state;
    }

    return I2C_OK;
}

/**
  * \brief this api for master receive slave send test in 10bit mode
  * \param I2Cx_m master i2c
  * \param I2Cx_s slave i2c
  * \param slave_addr slave i2c slave addr
  * \param send master i2c need to send data
  * \param recv slave i2c receive data
  * \param len data length, len(send) == len(recv)
  * \retval i2c status
  */
I2C_StateTypedef I2C_MasterRecvSlaveSend_10bit(I2C_TypeDef* I2Cx_m, I2C_TypeDef* I2Cx_s, uint16_t slave_addr, uint8_t* send, uint8_t* recv, uint32_t len)
{
    uint32_t timeout = I2C_TIMEOUT_VAL;
    I2C_StateTypedef state;

    I2C_ACK(I2Cx_m, DISABLE);

    /* send first slave address to I2C bus */
    state = I2C_Send_SlaveAddr(I2Cx_m, ADDR_10BIT_1ST(slave_addr), I2C_DIR_WRITE, I2C_TIMEOUT_VAL);
    if(I2C_OK != state) {
        return state;
    }

    /* wait for addr match */
    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status (I2Cx_s, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status(I2Cx_s,I2C_STATUS_ADDR_MATCH );

    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status(I2Cx_m, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status (I2Cx_m, I2C_STATUS_ADDR_MATCH);

    /* send second slave address to I2C bus */
    state = I2C_Write_Done(I2Cx_m, ADDR_10BIT_2ND(slave_addr), I2C_TIMEOUT_VAL);
    if(I2C_OK != state) {
        return state;
    }

    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (I2C_Get_Status(I2Cx_m, I2C_STATUS_NACK));

    /* send first slave address to I2C bus */
    I2C_Restart_Cmd();
    state = I2C_Send_SlaveAddr(I2Cx_m, ADDR_10BIT_1ST(slave_addr), I2C_DIR_READ, I2C_TIMEOUT_VAL);
    if(I2C_OK != state) {
        return state;
    }

    timeout = I2C_TIMEOUT_VAL;
    /* wait until I2C recv ack */
    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status (I2Cx_s, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status(I2Cx_s,I2C_STATUS_ADDR_MATCH );

    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status(I2Cx_m, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status (I2Cx_m, I2C_STATUS_ADDR_MATCH);

    timeout = I2C_TIMEOUT_VAL;
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
        do {
            if (timeout-- == 0U) {
                return I2C_TIMEOUT;
            }
        } while (!I2C_Get_Status(I2Cx_s, I2C_STATUS_TXFIFO_EMPTY));

        do {
            if (timeout-- == 0U) {
                return I2C_TIMEOUT;
            }
        } while (I2C_Get_Status(I2Cx_m, I2C_STATUS_RXFIFO_EMPTY));
        /* read a data from I2C_DATA */
        recv[i] = I2Cx_m->RXDATA;
    }
    state = I2C_WaitStopOver(I2Cx_m);
    if(I2C_OK != state) {
        return state;
    }
}

/**
  * \brief this api for master send slave receive test in 10bit mode
  * \param I2Cx_m master i2c
  * \param I2Cx_s slave i2c
  * \param slave_addr slave i2c slave addr
  * \param send master i2c need to send data
  * \param recv slave i2c receive data
  * \param len data length, len(send) == len(recv)
  * \retval i2c status
  */
I2C_StateTypedef I2C_MasterSendSlaveRecv_10bit(I2C_TypeDef* I2Cx_m, I2C_TypeDef* I2Cx_s, uint16_t slave_addr, uint8_t* send, uint8_t* recv, uint32_t len)
{
    uint32_t timeout = I2C_TIMEOUT_VAL*4;
    I2C_StateTypedef state;

    do {
        if (timeout -- == 0U) {
            return I2C_TIMEOUT;
        } 
    } while (I2C_Get_Status(I2Cx_m, I2C_STATUS_BUSY));

    /* send first slave address to I2C bus */
    state = I2C_Send_SlaveAddr(I2Cx_m, ADDR_10BIT_1ST(slave_addr), I2C_DIR_WRITE, I2C_TIMEOUT_VAL);
    if(I2C_OK != state) {
        return state;
    }

    timeout = I2C_TIMEOUT_VAL;
    /* wait for addr match */
    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status (I2Cx_s, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status(I2Cx_s,I2C_STATUS_ADDR_MATCH );

    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (!I2C_Get_Status(I2Cx_m, I2C_STATUS_ADDR_MATCH));
    I2C_Clear_Status (I2Cx_m, I2C_STATUS_ADDR_MATCH);

    timeout = I2C_TIMEOUT_VAL;
    /* send second slave address to I2C bus */
    state = I2C_Write_Done(I2Cx_m, ADDR_10BIT_2ND(slave_addr), I2C_TIMEOUT_VAL);
    if(I2C_OK != state) {
        return state;
    }

    do {
        if (timeout-- == 0U) {
            return I2C_TIMEOUT;
        }
    } while (I2C_Get_Status(I2Cx_m, I2C_STATUS_NACK));

    timeout = I2C_TIMEOUT_VAL;
    /* master enable write */
    I2C_Write_En(I2Cx_m);
    for (int i = 0; i < len; i++) {
        /* send a data byte */
        I2Cx_m->TXDATA = send[i];
        do {
            if (timeout-- == 0U) {
                return I2C_TIMEOUT;
            }
        } while (!I2C_Get_Status(I2Cx_m, I2C_STATUS_TXFIFO_EMPTY));

        do {
            if (timeout-- == 0U) {
                return I2C_TIMEOUT;
            }
        } while (I2C_Get_Status(I2Cx_s, I2C_STATUS_RXFIFO_EMPTY));

        /* read a data from I2C_DATA */
        recv[i] = I2Cx_s->RXDATA;
    }
    /* send a stop condition to I2C bus */
    I2C_Stop(I2Cx_m);
    state = I2C_WaitStopOver(I2Cx_m);
    if(I2C_OK != state) {
        return state;
    }

    return I2C_OK;
}

/* ------------------------------------------------ Adv feature API ------------------------------------------------ */
/**
  * \brief I2C get adv status
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg I2C_ADVANCED_STATUS_PECERR
  *     \arg I2C_ADVANCED_STATUS_PECBYTE
  *     \arg I2C_ADVANCED_STATUS_SBC_TCR
  * \return FlagStatus
  */
FlagStatus I2C_Get_AdvStatus(I2C_TypeDef* I2Cx, uint32_t status)
{
    if (status == ((I2Cx->ADVANCED_STATUS) & status)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief I2C clear adv status
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg I2C_ADVANCED_STATUS_SBC_TCR
  *     \arg I2C_ADVANCED_STATUS_PECBYTE
  *     \arg I2C_ADVANCED_STATUS_PECERR
  */
void I2C_Clear_AdvStatus(I2C_TypeDef* I2Cx, uint32_t status)
{
    I2Cx->ADVANCED_STATUS |= status;
}

/**
  * \brief I2C pec enable
  * \param I2Cx where x can be to select the I2C peripheral.
  */
void I2C_PEC_Request(I2C_TypeDef* I2Cx)
{
    I2Cx->ADVANCED_SETUP |= I2C_ADVANCED_SETUP_PECIRQ_REQUEST;
}

/**
  * \brief get I2C pec byte
  * \param I2Cx where x can be to select the I2C peripheral.
  * \retval pec_byte
  */
uint8_t I2C_Get_PECode(I2C_TypeDef* I2Cx)
{
    uint8_t pec_byte;
    pec_byte = ((I2Cx->ADVANCED_STATUS & I2C_ADVANCED_STATUS_PECBYTE) >> 1);
    return pec_byte;
}

/**
  * \brief I2C send alert signal to bus
  * \param I2Cx where x can be to select the I2C peripheral.
  * \retval none
  */
void I2C_Slave_Send_Alert(I2C_TypeDef* I2Cx)
{
    I2Cx->ADVANCED_SETUP |= I2C_ADVANCED_SETUP_ALERTEN_ENABLE;
}

/* ------------------------------------------------ PRIVATE API ------------------------------------------------ */
/**
  * \brief I2C slave addr config
  *
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param init pointer to a I2C_InitTypeDef structure that
  *         contains the configuration information for the specified I2C peripheral.
  */
static inline void I2C_SlaveAddr_Cfg(I2C_TypeDef* I2Cx, I2C_InitTypeDef* init)
{
    if(ENABLE == init->slave_info1.slave_addr_en){
        I2Cx->SLAVE_ADDRESS1 = ((init->slave_info1.slave_addr_en << I2C_SLAVE_ADDRESS1_EN_OFS) | 
                                (init->slave_info1.slave_addr_mask << I2C_SLAVE_ADDRESS1_MASK_OFS) | 
                                (init->slave_info1.slave_addr << I2C_SLAVE_ADDRESS1_NUM_OFS));
    }
    if(ENABLE == init->slave_info2.slave_addr_en){
        I2Cx->SLAVE_ADDRESS2 = ((init->slave_info2.slave_addr_en << I2C_SLAVE_ADDRESS2_EN_OFS) | 
                                (init->slave_info2.slave_addr_mask << I2C_SLAVE_ADDRESS2_MASK_OFS) | 
                                (init->slave_info2.slave_addr << I2C_SLAVE_ADDRESS2_NUM_OFS));
    }
}

/**
  * \brief I2C core mode enable/disable
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
static inline void I2C_Control_En(I2C_TypeDef* I2Cx, ControlStatus status)
{
    
    if (ENABLE == status) {
        I2Cx->SETUP |= I2C_SETUP_ENABLE;
    } else {
        I2Cx->SETUP &= ~(I2C_SETUP_ENABLE);
    }
}

/**
  * \brief I2C SCL io pull enable/disable
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
static inline void I2C_SCLPull_En(I2C_TypeDef* I2Cx, ControlStatus status)
{
    
    if (ENABLE == status) {
        I2Cx->SETUP |= I2C_SETUP_SCL_PULLUP_ENABLE;
    } else {
        I2Cx->SETUP &= ~(I2C_SETUP_SCL_PULLUP_ENABLE);
    }
}

/**
  * \brief I2C SDA io pull enable/disable
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
static inline void I2C_SDAPull_En(I2C_TypeDef* I2Cx, ControlStatus status)
{
    
    if (ENABLE == status) {
        I2Cx->SETUP |= I2C_SETUP_SDA_PULLUP_ENABLE;
    } else {
        I2Cx->SETUP &= ~(I2C_SETUP_SDA_PULLUP_ENABLE);
    }
}

/**
  * \brief I2C 10bit addr mode enable
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param status
  *     \arg ENABLE
  *     \arg DISABLE
  */
static inline void I2C_10BitAddr_En(I2C_TypeDef* I2Cx, ControlStatus status)
{
    
    if (ENABLE == status) {
        I2Cx->SETUP |= I2C_SETUP_10BIT_ADDR_ENABLE;
    } else {
        I2Cx->SETUP &= ~(I2C_SETUP_10BIT_ADDR_ENABLE);
    }
}

/**
  * \brief I2C master mode
  * \param I2Cx where x can be to select the I2C peripheral.
  * \param mode I2C role mode
  */
static inline void I2C_Role_Cfg(I2C_TypeDef* I2Cx, I2C_ModeTypedef mode)
{
    switch (mode) {
        case I2C_MASTER_MODE:
            I2Cx->SETUP &= ((~I2C_SETUP_ROLE_MODE) | I2C_SETUP_ROLE_MODE_MASTER);
            break;
        case I2C_SLAVE_MODE:
            I2Cx->SETUP |= I2C_SETUP_ROLE_MODE_SLAVE;
            break;
        default:
            
            break;
    }
}

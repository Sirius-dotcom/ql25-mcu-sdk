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

#define BUFSIZE 64

xKan_InitTypeDef xkan_parameter_init = {0};
xKan_FilterTypeDef xKan_RXFilter = {0};
xKan_TxHeaderTypeDef xKan_TxHeader = {0};
xKan_RxHeaderTypeDef xKan_RxHeader = {0};

uint8_t tx_msg[BUFSIZE];
uint8_t g_KanRxData[BUFSIZE];

volatile uint32_t interrupt_staus = 0;

volatile ErrStatus status = ERROR;

void xkan_config(void)
{
    /* step0: clk config */

    /* step1: function config */
    xKan_StructInit(&xkan_parameter_init);
    xkan_parameter_init.working_mode = XKAN_NORMAL_MODE;
    xkan_parameter_init.frame_format = XKAN_FRAME_FD_BRS;
    xkan_parameter_init.time_stamp = ENABLE;
    xkan_parameter_init.auto_retransmit = DISABLE;
    xkan_parameter_init.tx_delay_compensation = DISABLE;
    xkan_parameter_init.transmit_pause = DISABLE;

    /* step2.1: nominal bit timing config(sample-point:75%) */
    xkan_parameter_init.normal_time.NominalPrescaler = 0x2;
    xkan_parameter_init.normal_time.NominalSyncJumpWidth = 0x1;
    xkan_parameter_init.normal_time.NominalTimeSeg1 = 0x3;
    xkan_parameter_init.normal_time.NominalTimeSeg2 = 0x2;

    /* step2.2: data bit timing config(sample-point:75%) */
    xkan_parameter_init.data_time.DataPrescaler = 0x1;
    xkan_parameter_init.data_time.DataSyncJumpWidth = 0x1;
    xkan_parameter_init.data_time.DataTimeSeg1 = 0x3;
    xkan_parameter_init.data_time.DataTimeSeg2 = 0x2;
    
    /* step3: ram config */
    xkan_parameter_init.ram_info.MessageRAMOffset = 0;
    xkan_parameter_init.ram_info.StdFiltersNbr = 3;
    xkan_parameter_init.ram_info.ExtFiltersNbr = 0;
    xkan_parameter_init.ram_info.RxFifo0ElmtsNbr = 3;
    xkan_parameter_init.ram_info.RxFifo0ElmtSize = XKAN_DATA_BYTES_64;
    xkan_parameter_init.ram_info.RxFifo1ElmtsNbr = 3;
    xkan_parameter_init.ram_info.RxFifo1ElmtSize = XKAN_DATA_BYTES_64;
    xkan_parameter_init.ram_info.RxBuffersNbr = 2;
    xkan_parameter_init.ram_info.RxBufferSize = XKAN_DATA_BYTES_64;

    xkan_parameter_init.ram_info.TxBuffersNbr = 10;
    xkan_parameter_init.ram_info.TxFifoElmtsNbr = 4;
    
    xkan_parameter_init.ram_info.TxElmtSize = XKAN_DATA_BYTES_64;

    xKan_Init(XKAN0, &xkan_parameter_init);
    xKan_Init(XKAN1, &xkan_parameter_init);

    /* step4.1: standard filter config */ 
    xKan_RXFilter.IdType = XKAN_STANDARD_ID; 
    xKan_RXFilter.FilterIndex = 0;
    xKan_RXFilter.FilterType = XKAN_FILTER_DUAL;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXFIFO0;
    xKan_RXFilter.FilterID1 = 0x013;
    xKan_RXFilter.FilterID2 = 0x014;
    
    xKan_ConfigFilter(XKAN0, &xkan_parameter_init, &xKan_RXFilter, XKAN0_RAM_BASE);
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_RXFilter.IdType = XKAN_STANDARD_ID; 
    xKan_RXFilter.FilterIndex = 1;
    xKan_RXFilter.FilterType = XKAN_FILTER_RANGE;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXFIFO1;
    xKan_RXFilter.FilterID1 = 0x015;
    xKan_RXFilter.FilterID2 = 0x020;
    
    xKan_ConfigFilter(XKAN0, &xkan_parameter_init, &xKan_RXFilter, XKAN0_RAM_BASE);
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_RXFilter.IdType = XKAN_STANDARD_ID; 
    xKan_RXFilter.FilterIndex = 2;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x021;
    xKan_RXFilter.FilterID2 = 0x7FF;
    xKan_RXFilter.RxBufferIndex = 0;
    xKan_ConfigFilter(XKAN0, &xkan_parameter_init, &xKan_RXFilter, XKAN0_RAM_BASE);
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);

    /* step4.2: extend filter config */

    /* 1.reject frame which has non-matching stand_id or filter extend_id 
     * 2.filter remote frame either standard or extended frame
    */
    xKan_ConfigGlobalFilter(XKAN0, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);
    xKan_ConfigGlobalFilter(XKAN1, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);

    /* step5: rx water mark config */
    xKan_ConfigRxFifoWatermark(XKAN1, XKAN_CFG_RX_FIFO0, 1);
    xKan_ConfigRxFifoWatermark(XKAN1, XKAN_CFG_RX_FIFO1, 1);
}

/**
  * @brief  xKan_SendPacket send xkan frame to bus
  * @param  _DataBuf: send data buffer
  * @param  _Len: data length
  *             XKAN_DLC_BYTES_0  
  *             XKAN_DLC_BYTES_1  
  *             XKAN_DLC_BYTES_2   
  *             XKAN_DLC_BYTES_3   
  *             XKAN_DLC_BYTES_4 
  *             XKAN_DLC_BYTES_5 
  *             XKAN_DLC_BYTES_6 
  *             XKAN_DLC_BYTES_7 
  *             XKAN_DLC_BYTES_64 
  *             XKAN_DLC_BYTES_12
  *             XKAN_DLC_BYTES_16 
  *             XKAN_DLC_BYTES_20 
  *             XKAN_DLC_BYTES_24
  *             XKAN_DLC_BYTES_32 
  *             XKAN_DLC_BYTES_48
  *             XKAN_DLC_BYTES_64 
  */
void xKan_SendPacket(uint8_t *_DataBuf, uint32_t _Len)
{
    uint32_t tmpreg;
    xKan_TxHeader.Identifier = 0x21;
    xKan_TxHeader.IdType = XKAN_STANDARD_ID;
    xKan_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xKan_TxHeader.DataLength = _Len;
    xKan_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    xKan_TxHeader.BitRateSwitch = XKAN_BRS_ON;
    xKan_TxHeader.FDFormat = XKAN_FD_CAN;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER0, XKAN0_RAM_BASE);

    xKan_TxHeader.Identifier = 0x13;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER1, XKAN0_RAM_BASE);
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER2, XKAN0_RAM_BASE);
    xKan_TxHeader.Identifier = 0x14;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER3, XKAN0_RAM_BASE);
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER4, XKAN0_RAM_BASE);

    xKan_TxHeader.Identifier = 0x15;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER5, XKAN0_RAM_BASE);
    xKan_TxHeader.Identifier = 0x16;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER6, XKAN0_RAM_BASE);
    xKan_TxHeader.Identifier = 0x17;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER7, XKAN0_RAM_BASE);
    xKan_TxHeader.Identifier = 0x18;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER8, XKAN0_RAM_BASE);

    while (1)
    {
        xKan_EnableTxBufferRequest(XKAN0, 0x1FF);
        do {
            tmpreg = XKAN0->CFG_TXB_TRANS;
        } while (tmpreg != 0x1FF);
        XKAN0->CFG_TXB_TRANS = tmpreg;

        if(xKan_GetInterruptStatus(XKAN0, XKAN_CFG_INT_STATUS_TIME_STAMP_WRAP_INT)) {
            xKan_ClearInterruptStatus(XKAN0, XKAN_CFG_INT_STATUS_TIME_STAMP_WRAP_INT);
            break;
        }
    }
}

void XKAN1_IRQHandler(void)
{
    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF0_WM_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXF0_WM_EN))
    {
        
        interrupt_staus |= XKAN_CFG_INT_STATUS_RXF0_WM_INT;
        xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF0_WM_EN, DISABLE);
        xKan_ConfigRxFifoWatermark(XKAN1, XKAN_CFG_RX_FIFO0, 2);
    }

    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF0_FILL_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXF0_FILL_EN))
    {
        interrupt_staus |= XKAN_CFG_INT_STATUS_RXF0_FILL_INT;
        xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF0_FILL_EN, DISABLE);
        xKan_ClearInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF0_FILL_INT);
    }

    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF0_FULL_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXF0_FULL_EN))
    {
        interrupt_staus |= XKAN_CFG_INT_STATUS_RXF0_FULL_INT;
        xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF0_FULL_EN, DISABLE);
    }

    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF0_LOST_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXF0_LOST_EN))
    {
        interrupt_staus |= XKAN_CFG_INT_STATUS_RXF0_LOST_INT;
        xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF0_LOST_EN, DISABLE);
    }

    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF1_WM_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXF1_WM_EN))
    {
        
        interrupt_staus |= XKAN_CFG_INT_STATUS_RXF1_WM_INT;
        xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF1_WM_EN, DISABLE);
        xKan_ConfigRxFifoWatermark(XKAN1, XKAN_CFG_RX_FIFO1, 2);
    }
    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF1_FILL_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXF1_FILL_EN))
    {
        interrupt_staus |= XKAN_CFG_INT_STATUS_RXF1_FILL_INT;
        xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF1_FILL_EN, DISABLE);
        xKan_ClearInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF1_FILL_INT);
    }

    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF1_FULL_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXF1_FULL_EN))
    {
        interrupt_staus |= XKAN_CFG_INT_STATUS_RXF1_FULL_INT;
        xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF1_FULL_EN, DISABLE);
    }

    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF1_LOST_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXF1_LOST_EN))
    {
        interrupt_staus |= XKAN_CFG_INT_STATUS_RXF1_LOST_INT;
        xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF1_LOST_EN, DISABLE);
    }

    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXB_UPD_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXB_UPD_EN))
    {
        
        interrupt_staus |= XKAN_CFG_INT_STATUS_RXB_UPD_INT;
        xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXB_UPD_EN, DISABLE);
    }
}

void XKAN0_IRQHandler(void)
{
    if(xKan_GetInterruptStatus(XKAN0, XKAN_CFG_INT_STATUS_ARBITRATION_LOST_INT) && xKan_GetInterruptEnable(XKAN0, XKAN_CFG_INT_EN_ARBITRATION_LOST_INT_EN))
    {
        interrupt_staus |= XKAN_CFG_INT_STATUS_ARBITRATION_LOST_INT;
        xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_ARBITRATION_LOST_INT_EN, DISABLE);
        xKan_ClearInterruptStatus(XKAN0, XKAN_CFG_INT_STATUS_ARBITRATION_LOST_INT);
    }

    if(xKan_GetInterruptStatus(XKAN0, XKAN_CFG_INT_STATUS_TX_TRANS_COMPLETE_INT) && xKan_GetInterruptEnable(XKAN0, XKAN_CFG_INT_EN_TX_TRANS_COMPLETE_EN))
    {
        interrupt_staus |= XKAN_CFG_INT_STATUS_TX_TRANS_COMPLETE_INT;
        xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_TX_TRANS_COMPLETE_EN, DISABLE);

    }

    if(xKan_GetInterruptStatus(XKAN0, XKAN_CFG_INT_STATUS_TIME_STAMP_WRAP_INT) && xKan_GetInterruptEnable(XKAN0, XKAN_CFG_INT_EN_TIME_STAMP_WRAP_EN))
    {
        interrupt_staus |= XKAN_CFG_INT_STATUS_TIME_STAMP_WRAP_INT;
        xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_TIME_STAMP_WRAP_EN, DISABLE);
    }
}

ErrStatus interrupt_check(uint32_t actual_value, uint32_t expected_value)
{
    ErrStatus state = actual_value ^ expected_value;
    if (state) {
        return ERROR;
    } else {
        return SUCCESS;
    }
}

void main(void)
{
    #ifdef MISC_HAS_XKAN0_HAS_CLK
    xkan0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_XKAN0_RST
    xkan0_set_rst(DISABLE);
    xkan0_set_rst(ENABLE);
    #endif

    #ifdef CFG_SIMULATION
    #ifdef MISC_HAS_XKAN0_CLK_DIV
    xkan0_clk_div(0);
    #endif

    #ifdef MISC_HAS_XKAN0_INTF_CLK_DIV
    xkan0_intf_clk_div(1);
    #endif
    #endif

    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif

    printf("xkan normal_interrupt_test \n\r");
    /* xkan cfg */ 

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt XKAN0_IRQn */
    ECLIC_Register_IRQ(XKAN0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    XKAN0_IRQHandler);

    /*register interrupt XKAN1_IRQn */
    ECLIC_Register_IRQ(XKAN1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    XKAN1_IRQHandler);
    /* this case only test normal transmit interrupt */
    /* enable XKAN1 receive corresponding interrupt */
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF0_FILL_EN, ENABLE);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF0_WM_EN, ENABLE);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF0_FULL_EN, ENABLE);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF0_LOST_EN, ENABLE);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF1_FILL_EN, ENABLE);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF1_WM_EN, ENABLE);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF1_FULL_EN, ENABLE);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF1_LOST_EN, ENABLE);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXB_UPD_EN, ENABLE);
    /* enable XKAN0 interrupt */
    
    xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_TX_TRANS_COMPLETE_EN, ENABLE);
    xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_TIME_STAMP_WRAP_EN, ENABLE);
    
    uint32_t test_interrupt_mask = (XKAN_CFG_INT_STATUS_RXB_UPD_INT | XKAN_CFG_INT_STATUS_TIME_STAMP_WRAP_INT | \
                                    XKAN_CFG_INT_STATUS_ARBITRATION_LOST_INT | XKAN_CFG_INT_STATUS_TX_TRANS_COMPLETE_INT | \
                                    XKAN_CFG_INT_STATUS_RXF0_FILL_INT | XKAN_CFG_INT_STATUS_RXF0_WM_INT | \
                                    XKAN_CFG_INT_STATUS_RXF0_FULL_INT | XKAN_CFG_INT_STATUS_RXF0_LOST_INT | \
                                    XKAN_CFG_INT_STATUS_RXF1_FILL_INT | XKAN_CFG_INT_STATUS_RXF1_WM_INT | \
                                    XKAN_CFG_INT_STATUS_RXF1_FULL_INT | XKAN_CFG_INT_STATUS_RXF1_LOST_INT
                                );
    xkan_config();

    /* config int handle */

    /* exit init mode */
    xKan_Start(XKAN0);
    xKan_Start(XKAN1);

    /* fill tx_msg buffer */
    for(int i = 0; i < BUFSIZE; i++){
        tx_msg[i] = i + 0x10;
    }

    /*send tx_msg */
    xKan_SendPacket(tx_msg, XKAN_DLC_BYTES_64);

    if(interrupt_check(interrupt_staus, test_interrupt_mask)) {
        simulation_fail();
    } else {
        simulation_pass();
    }

    while(1);
}

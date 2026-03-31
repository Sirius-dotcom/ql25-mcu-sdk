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

#define BUFSIZE 8

xKan_InitTypeDef xkan0_parameter_init = {0};
xKan_InitTypeDef xkan1_parameter_init = {0};
xKan_FilterTypeDef xkan0_RXFilter = {0};
xKan_FilterTypeDef xkan1_RXFilter = {0};

xKan_TxHeaderTypeDef xkan0_TxHeader = {0};
xKan_RxHeaderTypeDef xkan0_RxHeader = {0};
xKan_TxHeaderTypeDef xkan1_TxHeader = {0};
xKan_RxHeaderTypeDef xkan1_RxHeader = {0};

uint8_t tx_msg[BUFSIZE];
uint8_t g_xkan0RxData[BUFSIZE];
uint8_t g_xkan1RxData[BUFSIZE];

volatile uint8_t xkan0_rx_done = 0;
volatile uint8_t xkan1_rx_done = 0;

void xkan0_config(void)
{
    /* step0: clk config */

    /* step1: function config */
    xKan_StructInit(&xkan0_parameter_init);
    xkan0_parameter_init.working_mode = XKAN_NORMAL_MODE;
    xkan0_parameter_init.frame_format = XKAN_FRAME_FD_BRS;
    xkan0_parameter_init.time_stamp = DISABLE;
    xkan0_parameter_init.auto_retransmit = ENABLE;
    xkan0_parameter_init.tx_delay_compensation = DISABLE;
    xkan0_parameter_init.transmit_pause = DISABLE;
    
    /* step2.1: nominal bit timing config(sample-point:75%) */
    xkan0_parameter_init.normal_time.NominalPrescaler = 0x4;
    xkan0_parameter_init.normal_time.NominalSyncJumpWidth = 0x3;
    xkan0_parameter_init.normal_time.NominalTimeSeg1 = 0xb;
    xkan0_parameter_init.normal_time.NominalTimeSeg2 = 0x4;

    /* step2.2: data bit timing config(sample-point:75%) */
    xkan0_parameter_init.data_time.DataPrescaler = 0x2;
    xkan0_parameter_init.data_time.DataSyncJumpWidth = 0x3;
    xkan0_parameter_init.data_time.DataTimeSeg1 = 0xb;
    xkan0_parameter_init.data_time.DataTimeSeg2 = 0x4;

    /* step3: ram config */
    xkan0_parameter_init.ram_info.MessageRAMOffset = 0;
    xkan0_parameter_init.ram_info.StdFiltersNbr = 10;
    xkan0_parameter_init.ram_info.ExtFiltersNbr = 0;
    xkan0_parameter_init.ram_info.RxFifo0ElmtsNbr = 1;
    xkan0_parameter_init.ram_info.RxFifo0ElmtSize = XKAN_DATA_BYTES_8;

    xkan0_parameter_init.ram_info.TxBuffersNbr = 1;
    xkan0_parameter_init.ram_info.TxElmtSize = XKAN_DATA_BYTES_8;

    xKan_Init(XKAN0, &xkan0_parameter_init);

    /* step4.1: standard filter config */
    xkan0_RXFilter.IdType = XKAN_STANDARD_ID; 
    xkan0_RXFilter.FilterIndex = 0;
    xkan0_RXFilter.FilterType = XKAN_FILTER_DUAL;
    xkan0_RXFilter.FilterConfig = XKAN_FILTER_TO_RXFIFO0;
    xkan0_RXFilter.FilterID1 = 0x2aa;
    xkan0_RXFilter.FilterID2 = 0x3bb;
    /* step4.2: extend filter config */

    xKan_ConfigFilter(XKAN0, &xkan0_parameter_init, &xkan0_RXFilter, XKAN0_RAM_BASE);

    /* 1.reject frame which has non-matching stand_id or filter extend_id 
     * 2.filter remote frame either standard or extended frame
    */
    xKan_ConfigGlobalFilter(XKAN0, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);

    /* step5: rx water mark config */
    xKan_ConfigRxFifoWatermark(XKAN0, XKAN_CFG_RX_FIFO0, 0);
}

void xkan1_config(void)
{
    /* step0: clk config */

    /* step1: function config */
    xKan_StructInit(&xkan1_parameter_init);
    xkan1_parameter_init.working_mode = XKAN_NORMAL_MODE;
    xkan1_parameter_init.frame_format = XKAN_FRAME_FD_BRS;
    xkan1_parameter_init.time_stamp = DISABLE;
    xkan1_parameter_init.auto_retransmit = DISABLE;
    xkan1_parameter_init.tx_delay_compensation = DISABLE;
    xkan1_parameter_init.transmit_pause = DISABLE;
    
    /* step2.1: nominal bit timing config(sample-point:75%) */
    xkan1_parameter_init.normal_time.NominalPrescaler = 0x4;
    xkan1_parameter_init.normal_time.NominalSyncJumpWidth = 0x3;
    xkan1_parameter_init.normal_time.NominalTimeSeg1 = 0xb;
    xkan1_parameter_init.normal_time.NominalTimeSeg2 = 0x4;

    /* step2.2: data bit timing config(sample-point:75%) */
    xkan1_parameter_init.data_time.DataPrescaler = 0x2;
    xkan1_parameter_init.data_time.DataSyncJumpWidth = 0x3;
    xkan1_parameter_init.data_time.DataTimeSeg1 = 0xb;
    xkan1_parameter_init.data_time.DataTimeSeg2 = 0x4;
    
    /* step3: ram config */
    xkan1_parameter_init.ram_info.MessageRAMOffset = 0;
    xkan1_parameter_init.ram_info.StdFiltersNbr = 1;
    xkan1_parameter_init.ram_info.ExtFiltersNbr = 0;
    xkan1_parameter_init.ram_info.RxFifo0ElmtsNbr = 1;
    xkan1_parameter_init.ram_info.RxFifo0ElmtSize = XKAN_DATA_BYTES_8;

    xkan1_parameter_init.ram_info.TxBuffersNbr = 1;
    xkan1_parameter_init.ram_info.TxElmtSize = XKAN_DATA_BYTES_8;
    xKan_Init(XKAN1, &xkan1_parameter_init);

    /* step4.1: standard filter config */

    /* step4.2: extend filter config */
    xkan1_RXFilter.IdType = XKAN_STANDARD_ID; 
    xkan1_RXFilter.FilterIndex = 0;
    xkan1_RXFilter.FilterType = XKAN_FILTER_DUAL;
    xkan1_RXFilter.FilterConfig = XKAN_FILTER_TO_RXFIFO0;
    xkan1_RXFilter.FilterID1 = 0x555;
    xkan1_RXFilter.FilterID2 = 0x777;
    
    xkan1_RXFilter.RxBufferIndex = 0;

    xKan_ConfigFilter(XKAN1, &xkan1_parameter_init, &xkan1_RXFilter, XKAN1_RAM_BASE);

    /* 1.reject frame which has non-matching stand_id or filter extend_id 
     * 2.filter remote frame either standard or extended frame
    */
    xKan_ConfigGlobalFilter(XKAN1, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);

    /* step5: rx water mark config */
    xKan_ConfigRxFifoWatermark(XKAN1, XKAN_CFG_RX_FIFO0, 0);
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
  *             XKAN_DLC_BYTES_8 
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
    xkan0_TxHeader.Identifier = 0x555;
    xkan0_TxHeader.IdType = XKAN_STANDARD_ID;
    xkan0_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xkan0_TxHeader.DataLength = _Len;
    xkan0_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    xkan0_TxHeader.BitRateSwitch = XKAN_BRS_ON;
    xkan0_TxHeader.FDFormat = XKAN_FD_CAN;

    xkan1_TxHeader.Identifier = 0x2aa;
    xkan1_TxHeader.IdType = XKAN_STANDARD_ID;
    xkan1_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xkan1_TxHeader.DataLength = _Len;
    xkan1_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    xkan1_TxHeader.BitRateSwitch = XKAN_BRS_ON;
    xkan1_TxHeader.FDFormat = XKAN_FD_CAN;

    XKAN_AddMessageToTxBuffer(&xkan0_parameter_init, XKAN0, &xkan0_TxHeader, _DataBuf, XKAN_TX_BUFFER0, XKAN0_RAM_BASE);
    XKAN_AddMessageToTxBuffer(&xkan1_parameter_init, XKAN1, &xkan1_TxHeader, _DataBuf, XKAN_TX_BUFFER0, XKAN1_RAM_BASE);

    xKan_EnableTxBufferRequest(XKAN0, XKAN_TX_BUFFER0);
    xKan_EnableTxBufferRequest(XKAN1, XKAN_TX_BUFFER0);
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
static ErrStatus MemoryCompare(uint8_t* src, uint8_t* dst, uint16_t length)
{
    while (length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

void XKAN0_IRQHandler(void)
{
    if(xKan_GetInterruptStatus(XKAN0, XKAN_CFG_INT_STATUS_ARBITRATION_LOST_INT) && xKan_GetInterruptEnable(XKAN0, XKAN_CFG_INT_EN_ARBITRATION_LOST_INT_EN)){
        xKan_ClearInterruptStatus(XKAN0, XKAN_CFG_INT_STATUS_ARBITRATION_LOST_INT);
        xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_ARBITRATION_LOST_INT_EN, DISABLE);
    }
    if(xKan_GetInterruptStatus(XKAN0, XKAN_CFG_INT_STATUS_RXF0_WM_INT) && xKan_GetInterruptEnable(XKAN0, XKAN_CFG_INT_EN_RXF0_WM_EN))
    {
        xKan_RxFifo0Callback(XKAN0, &xkan0_parameter_init, &xkan0_RxHeader, g_xkan0RxData, XKAN0_RAM_BASE);
        xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_RXF0_WM_EN, DISABLE);
        xkan0_rx_done = 1;
    }
}

void XKAN1_IRQHandler(void)
{
    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF0_WM_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXF0_WM_EN))
    {
        xKan_RxFifo0Callback(XKAN1, &xkan1_parameter_init, &xkan1_RxHeader, g_xkan1RxData, XKAN1_RAM_BASE);
        xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF0_WM_EN, DISABLE);
        xkan1_rx_done = 1;
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

    printf("xkan normal_send_retransmit_test \n\r");
    /* xkan cfg */ 

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt XKAN0_IRQn */
    ECLIC_Register_IRQ(XKAN0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 2, 0,
                                    XKAN0_IRQHandler);

    /*register interrupt XKAN1_IRQn */
    ECLIC_Register_IRQ(XKAN1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    XKAN1_IRQHandler);

    xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_ARBITRATION_LOST_INT_EN, ENABLE);
    xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_RXF0_WM_EN, ENABLE);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF0_WM_EN, ENABLE);

    xkan0_config();
    xkan1_config();

    /* config int handle */

    /* exit init mode */
    xKan_Start(XKAN0);
    xKan_Start(XKAN1);

    /* fill tx_msg buffer */
    for(int i = 0; i < BUFSIZE; i++){
        tx_msg[i] = i + 0x10;
    }

    /*send tx_msg */
    xKan_SendPacket(tx_msg, XKAN_DLC_BYTES_8);

    /* wait for read over for rx_buffer */
    while(xkan1_rx_done != 1){}

    /* compare the read buffer and write buffer */
    if(SUCCESS != MemoryCompare(tx_msg, g_xkan1RxData, BUFSIZE)) {
        simulation_fail();
    } else {
        simulation_pass();
    }

    while(1);
}

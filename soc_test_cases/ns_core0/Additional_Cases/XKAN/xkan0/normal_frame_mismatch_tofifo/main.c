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
#define FRAMENUM 4

xKan_InitTypeDef xkan_parameter_init = {0};
xKan_FilterTypeDef xKan_RXFilter = {0};
xKan_TxHeaderTypeDef xKan_TxHeader = {0};
xKan_RxHeaderTypeDef xKan_RxHeader = {0};

uint8_t g_KanRxData[BUFSIZE*FRAMENUM];
uint8_t g_KanTxData[BUFSIZE*FRAMENUM];
uint8_t *pRxData = g_KanRxData;
uint8_t *pTxData = g_KanTxData;
volatile uint32_t rxbuffer_done = 0;
volatile uint32_t rxfifo0_miss_data_done = 0;
volatile uint32_t rxfifo1_miss_data_done = 0;

volatile uint32_t is_miss_frame = 1;

uint8_t tx_msg[BUFSIZE];
uint8_t tmp_msg0[BUFSIZE], tmp_msg1[BUFSIZE], tmp_msg2[BUFSIZE], tmp_msg3[BUFSIZE];

void xkan_config(void)
{
    /* step0: clk config */

    /* step1: function config */
    xKan_StructInit(&xkan_parameter_init);
    xkan_parameter_init.working_mode = XKAN_NORMAL_MODE;
    xkan_parameter_init.frame_format = XKAN_FRAME_FD_BRS;
    xkan_parameter_init.time_stamp = DISABLE;
    xkan_parameter_init.auto_retransmit = DISABLE;
    xkan_parameter_init.tx_delay_compensation = DISABLE;
    xkan_parameter_init.transmit_pause = DISABLE;

    /* step2.1: nominal bit timing config(sample-point:75%) */
    xkan_parameter_init.normal_time.NominalPrescaler = 0x4;
    xkan_parameter_init.normal_time.NominalSyncJumpWidth = 0x3;
    xkan_parameter_init.normal_time.NominalTimeSeg1 = 0xb;
    xkan_parameter_init.normal_time.NominalTimeSeg2 = 0x4;

    /* step2.2: data bit timing config(sample-point:75%) */
    xkan_parameter_init.data_time.DataPrescaler = 0x2;
    xkan_parameter_init.data_time.DataSyncJumpWidth = 0x3;
    xkan_parameter_init.data_time.DataTimeSeg1 = 0xb;
    xkan_parameter_init.data_time.DataTimeSeg2 = 0x4;
    
    /* step3: ram config */
    xkan_parameter_init.ram_info.MessageRAMOffset = 0;
    xkan_parameter_init.ram_info.StdFiltersNbr = 2;
    xkan_parameter_init.ram_info.ExtFiltersNbr = 2;
    xkan_parameter_init.ram_info.RxFifo0ElmtsNbr = 2;
    xkan_parameter_init.ram_info.RxFifo0ElmtSize = XKAN_DATA_BYTES_8;
    xkan_parameter_init.ram_info.RxFifo1ElmtsNbr = 2;
    xkan_parameter_init.ram_info.RxFifo1ElmtSize = XKAN_DATA_BYTES_8;
    xkan_parameter_init.ram_info.RxBuffersNbr = 2;
    xkan_parameter_init.ram_info.RxBufferSize = XKAN_DATA_BYTES_8;

    xkan_parameter_init.ram_info.TxBuffersNbr = 4;
    xkan_parameter_init.ram_info.TxFifoElmtsNbr = 0;
    
    xkan_parameter_init.ram_info.TxElmtSize = XKAN_DATA_BYTES_8;

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
    xKan_RXFilter.IdType = XKAN_EXTENDED_ID; 
    xKan_RXFilter.FilterIndex = 0;
    xKan_RXFilter.FilterType = XKAN_FILTER_RANGE;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXFIFO1;
    xKan_RXFilter.FilterID1 = 0x12345678;
    xKan_RXFilter.FilterID2 = 0x12345680;
    
    xKan_ConfigFilter(XKAN0, &xkan_parameter_init, &xKan_RXFilter, XKAN0_RAM_BASE);
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);

    /* step4.2: extend filter config */
    xKan_RXFilter.IdType = XKAN_STANDARD_ID;
    xKan_RXFilter.FilterIndex = 1;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x12;
    xKan_RXFilter.FilterID2 = 0x7FF;
    xKan_RXFilter.RxBufferIndex = 0;
    xKan_ConfigFilter(XKAN0, &xkan_parameter_init, &xKan_RXFilter, XKAN0_RAM_BASE);
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    
    xKan_RXFilter.IdType = XKAN_EXTENDED_ID;
    xKan_RXFilter.FilterIndex = 1;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x12345690;
    xKan_RXFilter.FilterID2 = 0x1FFFFFFF;
    xKan_RXFilter.RxBufferIndex = 1;
    xKan_ConfigFilter(XKAN0, &xkan_parameter_init, &xKan_RXFilter, XKAN0_RAM_BASE);
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);

    /* step4.2: extend filter config */

    /* 1.reject frame which has non-matching stand_id or filter extend_id 
     * 2.filter remote frame either standard or extended frame
    */
    xKan_ConfigGlobalFilter(XKAN0, XKAN_ACCEPT_IN_RX_FIFO0, XKAN_ACCEPT_IN_RX_FIFO1, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);
    xKan_ConfigGlobalFilter(XKAN1, XKAN_ACCEPT_IN_RX_FIFO0, XKAN_ACCEPT_IN_RX_FIFO1, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);

    /* step5: rx water mark config */
    xKan_ConfigRxFifoWatermark(XKAN1, XKAN_CFG_RX_FIFO0, 0);
    xKan_ConfigRxFifoWatermark(XKAN1, XKAN_CFG_RX_FIFO1, 0);
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
    /* step1: cfg send frame */
    xKan_TxHeader.Identifier = 0x011;
    xKan_TxHeader.IdType = XKAN_STANDARD_ID;
    xKan_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xKan_TxHeader.DataLength = _Len;
    xKan_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    xKan_TxHeader.BitRateSwitch = XKAN_BRS_ON;
    xKan_TxHeader.FDFormat = XKAN_FD_CAN;
    
    _DataBuf = tmp_msg0;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER0, XKAN0_RAM_BASE);

    _DataBuf = tmp_msg1;
    xKan_TxHeader.Identifier = 0x012;
    xKan_TxHeader.IdType = XKAN_STANDARD_ID;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER1, XKAN0_RAM_BASE);

    _DataBuf = tmp_msg2;
    xKan_TxHeader.Identifier = 0x12345670;
    xKan_TxHeader.IdType = XKAN_EXTENDED_ID;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER2, XKAN0_RAM_BASE);

    _DataBuf = tmp_msg3;
    xKan_TxHeader.Identifier = 0x12345690;
    xKan_TxHeader.IdType = XKAN_EXTENDED_ID;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER3, XKAN0_RAM_BASE);

    xKan_EnableTxBufferRequest(XKAN0, XKAN_TX_BUFFER0|XKAN_TX_BUFFER1|XKAN_TX_BUFFER2|XKAN_TX_BUFFER3);
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

void XKAN1_IRQHandler(void)
{
    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF0_WM_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXF0_WM_EN))
    {
        xKan_RxFifo0Callback(XKAN1, &xkan_parameter_init, &xKan_RxHeader, pRxData, XKAN1_RAM_BASE);
        if(xKan_RxHeader.IsFilterMatchingFrame != 1) {
            is_miss_frame = 0;
        }
        rxfifo0_miss_data_done ++;
        pRxData += BUFSIZE;
        if (rxfifo0_miss_data_done == 2) xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF0_WM_EN, DISABLE);
    }

    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXF1_WM_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXF1_WM_EN))
    {
        xKan_RxFifo1Callback(XKAN1, &xkan_parameter_init, &xKan_RxHeader, pRxData, XKAN1_RAM_BASE);
        if(xKan_RxHeader.IsFilterMatchingFrame != 1) {
            is_miss_frame = 0;
        }
        rxfifo1_miss_data_done ++;
        pRxData += BUFSIZE;
        if (rxfifo1_miss_data_done == 4) xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF1_WM_EN, DISABLE);
    }

    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_RXB_UPD_INT) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_EN_RXB_UPD_EN))
    {
        xKan_RxBufferNewMessageCallback(XKAN1, &xkan_parameter_init, &xKan_RxHeader, pRxData, XKAN1_RAM_BASE);
        rxbuffer_done ++;
        pRxData += BUFSIZE;
        if (rxbuffer_done == 2 ) xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXB_UPD_EN, DISABLE);
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

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt XKAN1_IRQn */
    ECLIC_Register_IRQ(XKAN1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    XKAN1_IRQHandler);

    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF0_WM_EN, ENABLE);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXF1_WM_EN, ENABLE);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_RXB_UPD_EN, ENABLE);

    /* xkan cfg */ 
    xkan_config();

    /* config int handle */

    /* exit init mode */
    xKan_Start(XKAN0);
    xKan_Start(XKAN1);

    /* fill tx_msg buffer */
    /* fill tx_msg buffer */
    for(int i = 0; i < BUFSIZE; i++){
        tmp_msg0[i] = 0x11;
        tmp_msg1[i] = 0x22;
        tmp_msg2[i] = 0x33;
        tmp_msg3[i] = 0x44;

        *(pTxData+BUFSIZE*(FRAMENUM-1)) = tmp_msg3[i];
        *(pTxData+BUFSIZE*(FRAMENUM-2)) = tmp_msg2[i];
        *(pTxData+BUFSIZE*(FRAMENUM-3)) = tmp_msg1[i];
        *pTxData ++ = tmp_msg0[i];
    }

    /*send tx_msg */
    xKan_SendPacket(tx_msg, XKAN_DLC_BYTES_8);

    /* wait for read over for rx_buffer */
    while(rxbuffer_done != 2 || rxfifo0_miss_data_done != 1 || rxfifo1_miss_data_done != 1){}

    /* compare the read buffer and write buffer */
    if(SUCCESS != MemoryCompare(g_KanTxData, g_KanRxData, BUFSIZE * FRAMENUM) || (is_miss_frame == 0)) {
        simulation_fail();
    } else {
        simulation_pass();
    }

    while(1);
}

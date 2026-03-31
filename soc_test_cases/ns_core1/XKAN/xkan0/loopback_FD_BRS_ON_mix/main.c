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

xKan_InitTypeDef xkan_parameter_init = {0};
xKan_FilterTypeDef xKan_RXFilter = {0};
xKan_TxHeaderTypeDef xKan_TxHeader = {0};
xKan_RxHeaderTypeDef xKan_RxHeader = {0};

uint8_t tx_msg[BUFSIZE];
uint8_t tx_msg1[BUFSIZE];
uint8_t g_KanRxData[BUFSIZE];
uint8_t g_KanRxData1[BUFSIZE];

volatile uint8_t rxfifo_done = 0;
volatile uint8_t rxbuf_done = 0;

void xkan_config(uint8_t tx_fifo, uint8_t tx_buffer, uint8_t rx_fifo, uint8_t rx_buffer)
{
    /* step0: clk config */

    /* step1: function config */
    xKan_StructInit(&xkan_parameter_init);
    xkan_parameter_init.working_mode = XKAN_LOOPBACK_MODE;
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
    xkan_parameter_init.ram_info.StdFiltersNbr = 1;
    xkan_parameter_init.ram_info.ExtFiltersNbr = 0;
    xkan_parameter_init.ram_info.RxFifo0ElmtsNbr = rx_fifo;
    xkan_parameter_init.ram_info.RxFifo0ElmtSize = XKAN_DATA_BYTES_8;
    xkan_parameter_init.ram_info.RxFifo1ElmtsNbr = 0;
    xkan_parameter_init.ram_info.RxFifo1ElmtSize = XKAN_DATA_BYTES_8;
    xkan_parameter_init.ram_info.RxBuffersNbr = rx_buffer;
    xkan_parameter_init.ram_info.RxBufferSize = XKAN_DATA_BYTES_8;

    xkan_parameter_init.ram_info.TxBuffersNbr = tx_buffer;
    xkan_parameter_init.ram_info.TxFifoElmtsNbr = tx_fifo;
    
    xkan_parameter_init.ram_info.TxElmtSize = XKAN_DATA_BYTES_8;

    xKan_Init(XKAN0, &xkan_parameter_init);

    /* step4.1: standard filter config */ 
    xKan_RXFilter.IdType = XKAN_STANDARD_ID; 
    xKan_RXFilter.FilterIndex = 0;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    if(rx_buffer == 1) {
        xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    } else if(rx_fifo == 1) {
        xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXFIFO0;
    }
    xKan_RXFilter.FilterID1 = 0x014;
    xKan_RXFilter.FilterID2 = 0x7FF;
    
    xKan_RXFilter.RxBufferIndex = 0;
    xKan_ConfigFilter(XKAN0, &xkan_parameter_init, &xKan_RXFilter, XKAN0_RAM_BASE);

    /* step4.2: extend filter config */

    /* 1.reject frame which has non-matching stand_id or filter extend_id 
     * 2.filter remote frame either standard or extended frame
    */
    xKan_ConfigGlobalFilter(XKAN0, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);
    xKan_ConfigRxFifoWatermark(XKAN0, XKAN_CFG_RX_FIFO0, 0);
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
void xKan_SendPacket(uint8_t *_DataBuf, uint32_t _Len, BOOL fifo)
{
    /* step1: cfg send frame */
    xKan_TxHeader.Identifier = 0x014;
    xKan_TxHeader.IdType = XKAN_STANDARD_ID;
    xKan_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xKan_TxHeader.DataLength = _Len;
    xKan_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    xKan_TxHeader.BitRateSwitch = XKAN_BRS_ON;
    xKan_TxHeader.FDFormat = XKAN_FD_CAN;
    
    if(fifo) {
        xKan_AddMessageToTxFifo(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN0_RAM_BASE);
    } else {
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER0, XKAN0_RAM_BASE);
        xKan_EnableTxBufferRequest(XKAN0, XKAN_TX_BUFFER0);
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
    if(xKan_GetInterruptStatus(XKAN0, XKAN_CFG_INT_STATUS_RXB_UPD_INT) && xKan_GetInterruptEnable(XKAN0, XKAN_CFG_INT_EN_RXB_UPD_EN))
    {
        xKan_RxBufferNewMessageCallback(XKAN0, &xkan_parameter_init, &xKan_RxHeader, g_KanRxData, XKAN0_RAM_BASE);
        xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_RXB_UPD_EN, DISABLE);
        rxbuf_done = 1;
    }
    if(xKan_GetInterruptStatus(XKAN0, XKAN_CFG_INT_STATUS_RXF0_WM_INT) && xKan_GetInterruptEnable(XKAN0, XKAN_CFG_INT_EN_RXF0_WM_EN))
    {
        xKan_RxFifo0Callback(XKAN0, &xkan_parameter_init, &xKan_RxHeader, g_KanRxData1, XKAN0_RAM_BASE);
        xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_RXF0_WM_EN, DISABLE);
        rxfifo_done = 1;
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
    /*register interrupt XKAN0_IRQn */
    ECLIC_Register_IRQ(XKAN0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    XKAN0_IRQHandler);

    xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_RXF0_WM_EN, ENABLE);

    /* fill tx_msg buffer */
    for(int i = 0; i < BUFSIZE; i++){
        tx_msg[i] = i + 0x10;
        tx_msg1[i] = 0xff - i;
    }

    printf("xkan loopback_FD_BRS_ON_mix test\n\r");
    /* xkan cfg */ 
    
    xkan_config(0,1,1,0);
    /* exit init mode */
    xKan_Start(XKAN0);
    /*send tx_msg */
    xKan_SendPacket(tx_msg, XKAN_DLC_BYTES_8, 0);
    while (!rxfifo_done);

    #ifdef MISC_HAS_XKAN0_RST
    xkan0_set_rst(DISABLE);
    xkan0_set_rst(ENABLE);
    #endif

    xKan_EnableInterrupt(XKAN0, XKAN_CFG_INT_EN_RXB_UPD_EN, ENABLE);
    xkan_config(1,0,0,1);
    /* exit init mode */
    xKan_Start(XKAN0);
    /*send tx_msg */
    xKan_SendPacket(tx_msg1, XKAN_DLC_BYTES_8, 1);
    while (!rxbuf_done);
    
    /* compare the read buffer and write buffer */
    if(SUCCESS != MemoryCompare(tx_msg, g_KanRxData1, BUFSIZE) || SUCCESS != MemoryCompare(tx_msg1, g_KanRxData, BUFSIZE)) {
        simulation_fail();
    }
    
    simulation_pass();
    
    while(1);
}

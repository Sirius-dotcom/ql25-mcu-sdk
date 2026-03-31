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

#define FRAMENUM 2
#define MAX_FRAME 64

xKan_InitTypeDef xkan_parameter_init = {0};
xKan_FilterTypeDef xKan_RXFilter = {0};
xKan_TxHeaderTypeDef xKan_TxHeader = {0};
xKan_RxHeaderTypeDef xKan_RxHeader = {0};

uint8_t g_KanRxData[MAX_FRAME*FRAMENUM] = {0};
uint8_t g_KanTxData[MAX_FRAME*FRAMENUM] = {0};
uint8_t *pRxData = g_KanRxData;
uint8_t *pTxData = g_KanTxData;
volatile uint8_t bufsize;
volatile uint32_t rx_done = 0;
uint8_t *tx_msg;
uint8_t tmp_msg0[MAX_FRAME]  = {0};

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
void xKan_SendPacket(uint8_t *_DataBuf, uint32_t _Len, uint32_t _RamIndex, uint32_t id)
{
    /* step1: cfg send frame */
    xKan_TxHeader.Identifier = id;
    xKan_TxHeader.IdType = XKAN_STANDARD_ID;
    xKan_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xKan_TxHeader.DataLength = _Len;
    xKan_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    xKan_TxHeader.BitRateSwitch = XKAN_BRS_ON;
    xKan_TxHeader.FDFormat = XKAN_FD_CAN;
    
    _DataBuf = tmp_msg0;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, _RamIndex, XKAN0_RAM_BASE);

    xKan_EnableTxBufferRequest(XKAN0, _RamIndex);
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

    printf("xkan loopback_dlc test\n\r");

    /* xkan cfg */ 
    /* just support 8byte dlc at most */
    int loop_num = 16;
    uint32_t rxbufer_num = 0;
    uint32_t data_bytes = 0;
    for(int i = loop_num -1 ; i >= 0; i--){
        rxbufer_num++;
        if(i <= 8) {
            data_bytes = XKAN_DATA_BYTES_8;
            bufsize = i;
        } else if (i == 9) {
            bufsize = 12;
            data_bytes = 0x5;
        } else if(i == 10) {
            bufsize = 16;
            data_bytes = 0x6;
        } else if(i == 11) {
            bufsize = 20;
            data_bytes = 0x7;
        } else if(i == 12) {
            bufsize = 24;
            data_bytes = 0x8;
        } else if(i == 13) {
            bufsize = 32;
            data_bytes = 0xA;
        } else if(i == 14) {
            bufsize = 48;
            data_bytes = 0xE;
        } else if(i == 15) {
            bufsize = 64;
            data_bytes = 0x12;
        } else {
            printf("wrong cfg\n");
        }
    
        #ifdef MISC_HAS_XKAN0_RST
        xkan0_set_rst(DISABLE);
        xkan0_set_rst(ENABLE);
        #endif

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
        xkan_parameter_init.normal_time.NominalPrescaler = 0x8;
        xkan_parameter_init.normal_time.NominalSyncJumpWidth = 0x3;
        xkan_parameter_init.normal_time.NominalTimeSeg1 = 0xb;
        xkan_parameter_init.normal_time.NominalTimeSeg2 = 0x4;

        /* step2.2: data bit timing config(sample-point:75%) */
        xkan_parameter_init.data_time.DataPrescaler = 0x4;
        xkan_parameter_init.data_time.DataSyncJumpWidth = 0x3;
        xkan_parameter_init.data_time.DataTimeSeg1 = 0xb;
        xkan_parameter_init.data_time.DataTimeSeg2 = 0x4;

        /* step3: ram config */
        xkan_parameter_init.ram_info.MessageRAMOffset = 0;
        xkan_parameter_init.ram_info.StdFiltersNbr = 128;
        xkan_parameter_init.ram_info.ExtFiltersNbr = 0;

        xkan_parameter_init.ram_info.RxBuffersNbr = i + 1;
        xkan_parameter_init.ram_info.RxBufferSize = data_bytes;
        xkan_parameter_init.ram_info.TxBuffersNbr = i + 1;
        xkan_parameter_init.ram_info.TxElmtSize = data_bytes;

        xKan_Init(XKAN0, &xkan_parameter_init);

        /* init filter for simmulation */
        xKan_RXFilter.IdType = XKAN_STANDARD_ID; 
        xKan_RXFilter.FilterType = 0x3; /* Reserved */
        xKan_RXFilter.FilterConfig = 0x3; /* Reserved */
        xKan_RXFilter.FilterID1 = 0x00;
        xKan_RXFilter.FilterID2 = 0x7FF;
        xKan_RXFilter.RxBufferIndex = i;
        for(int j = 0; j < 127; j++) {
            xKan_RXFilter.FilterIndex = j;
            xKan_ConfigFilter(XKAN0, &xkan_parameter_init, &xKan_RXFilter, XKAN0_RAM_BASE);
        }

        /* step4.1: standard filter config */ 
        xKan_RXFilter.IdType = XKAN_STANDARD_ID; 
        xKan_RXFilter.FilterIndex = 127;
        xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
        xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
        xKan_RXFilter.FilterID1 = i + 1;
        xKan_RXFilter.FilterID2 = 0x7FF;
        xKan_RXFilter.RxBufferIndex = i;
        xKan_ConfigFilter(XKAN0, &xkan_parameter_init, &xKan_RXFilter, XKAN0_RAM_BASE);

        /* 1.reject frame which has non-matching stand_id or filter extend_id 
        * 2.filter remote frame either standard or extended frame
        */
        xKan_ConfigGlobalFilter(XKAN0, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);

        /* config int handle */
        /* exit init mode */
        xKan_Start(XKAN0);

        /* fill tx_msg buffer */
        
        for(int j = 0; j < bufsize; j++){
            tmp_msg0[j] = 10 * i + i;
            *pTxData ++ = tmp_msg0[j];
        }

        /*send tx_msg */
        xKan_SendPacket(tx_msg, i<<16, 1 << i, i + 1);

        /* wait for read over for rx_buffer */
        while(XKAN0->CFG_RXDATL_RSP == 0 && XKAN0->CFG_RXDATH_RSP == 0);
        xKan_RxBufferNewMessageCallback(XKAN0, &xkan_parameter_init, &xKan_RxHeader, g_KanRxData, XKAN0_RAM_BASE);

        /* compare the read buffer and write buffer */
        if(SUCCESS != MemoryCompare(g_KanTxData, g_KanRxData, bufsize)) {
            simulation_fail();
        }
        /* point return to header */
        rx_done = 0;
        pTxData -= bufsize;
        pRxData -= bufsize;
    }

    simulation_pass();
    while(1);
}

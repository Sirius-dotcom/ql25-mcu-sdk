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
volatile uint32_t rx_done = 0;

uint8_t tx_msg[8];
uint8_t tmp_msg0[8], tmp_msg1[8], tmp_msg2[8], tmp_msg3[8];

void xkan_config(void)
{
    /* step0: clk config */

    /* step1: function config */
    xKan_StructInit(&xkan_parameter_init);
    xkan_parameter_init.working_mode = XKAN_NORMAL_MODE;
    xkan_parameter_init.frame_format = XKAN_FRAME_CLASSIC;
    xkan_parameter_init.time_stamp = DISABLE;
    xkan_parameter_init.auto_retransmit = DISABLE;
    xkan_parameter_init.tx_delay_compensation = DISABLE;
    xkan_parameter_init.transmit_pause = DISABLE;

    /* step2.1: nominal bit timing config(sample-point:75%) */
    xkan_parameter_init.normal_time.NominalPrescaler = 0x4;
    xkan_parameter_init.normal_time.NominalSyncJumpWidth = 0x3;
    xkan_parameter_init.normal_time.NominalTimeSeg1 = 0xb;
    xkan_parameter_init.normal_time.NominalTimeSeg2 = 0x4;

    /* step2.2: data bit timing config */
    xkan_parameter_init.data_time.DataPrescaler = 0;
    xkan_parameter_init.data_time.DataSyncJumpWidth = 0;
    xkan_parameter_init.data_time.DataTimeSeg1 = 0;
    xkan_parameter_init.data_time.DataTimeSeg2 = 0;

    /* step3: ram config */
    xkan_parameter_init.ram_info.MessageRAMOffset = 0;
    xkan_parameter_init.ram_info.StdFiltersNbr = 2;
    xkan_parameter_init.ram_info.ExtFiltersNbr = 2;
    xkan_parameter_init.ram_info.RxFifo0ElmtsNbr = 0;
    xkan_parameter_init.ram_info.RxFifo0ElmtSize = XKAN_DATA_BYTES_8;
    xkan_parameter_init.ram_info.RxFifo1ElmtsNbr = 0;
    xkan_parameter_init.ram_info.RxFifo1ElmtSize = XKAN_DATA_BYTES_8;
    xkan_parameter_init.ram_info.RxBuffersNbr = 4;
    xkan_parameter_init.ram_info.RxBufferSize = XKAN_DATA_BYTES_8;

    xkan_parameter_init.ram_info.TxBuffersNbr = 4;
    xkan_parameter_init.ram_info.TxFifoElmtsNbr = 0;
    
    xkan_parameter_init.ram_info.TxElmtSize = XKAN_DATA_BYTES_8;

    xKan_Init(XKAN1, &xkan_parameter_init);
    xKan_Init(XKAN2, &xkan_parameter_init);
    /* step4.1: standard filter config */ 
    xKan_RXFilter.IdType = XKAN_STANDARD_ID; 
    xKan_RXFilter.FilterIndex = 0;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x013;
    xKan_RXFilter.FilterID2 = 0x7FF;
    xKan_RXFilter.RxBufferIndex = 0;
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);
    xKan_RXFilter.IdType = XKAN_STANDARD_ID; 
    xKan_RXFilter.FilterIndex = 1;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x014;
    xKan_RXFilter.FilterID2 = 0x7FF;
    xKan_RXFilter.RxBufferIndex = 1;
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);

    /* step4.2: extend filter config */
    xKan_RXFilter.IdType = XKAN_EXTENDED_ID;
    xKan_RXFilter.FilterIndex = 0;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x12345678;
    xKan_RXFilter.FilterID2 = 0x1FFFFFFF;
    xKan_RXFilter.RxBufferIndex = 2;
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);
    
    xKan_RXFilter.IdType = XKAN_EXTENDED_ID;
    xKan_RXFilter.FilterIndex = 1;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x12345679;
    xKan_RXFilter.FilterID2 = 0x1FFFFFFF;
    xKan_RXFilter.RxBufferIndex = 3;
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);

    /* 1.reject frame which has non-matching stand_id or filter extend_id 
     * 2.filter remote frame either standard or extended frame
    */
    xKan_ConfigGlobalFilter(XKAN1, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);
    xKan_ConfigGlobalFilter(XKAN2, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);
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
    xKan_TxHeader.Identifier = 0x013;
    xKan_TxHeader.IdType = XKAN_STANDARD_ID;
    xKan_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xKan_TxHeader.DataLength = _Len;
    xKan_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    xKan_TxHeader.BitRateSwitch = XKAN_BRS_OFF; 
    xKan_TxHeader.FDFormat = XKAN_CLASSIC_CAN;
    
    _DataBuf = tmp_msg0;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER0, XKAN1_RAM_BASE);

    xKan_TxHeader.Identifier = 0x014;
    xKan_TxHeader.IdType = XKAN_STANDARD_ID;
    _DataBuf = tmp_msg1;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER1, XKAN1_RAM_BASE);

    xKan_TxHeader.Identifier = 0x12345678;
    xKan_TxHeader.IdType = XKAN_EXTENDED_ID;
    _DataBuf = tmp_msg2;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER2, XKAN1_RAM_BASE);

    xKan_TxHeader.Identifier = 0x12345679;
    xKan_TxHeader.IdType = XKAN_EXTENDED_ID;
    _DataBuf = tmp_msg3;
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER3, XKAN1_RAM_BASE);

    xKan_EnableTxBufferRequest(XKAN1, XKAN_TX_BUFFER0|XKAN_TX_BUFFER1|XKAN_TX_BUFFER2|XKAN_TX_BUFFER3);
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

void XKAN2_IRQHandler(void)
{
    if(xKan_GetInterruptStatus(XKAN2, XKAN_CFG_INT_STATUS_RXB_UPD_INT) && xKan_GetInterruptEnable(XKAN2, XKAN_CFG_INT_EN_RXB_UPD_EN))
    {
        xKan_RxBufferNewMessageCallback(XKAN2, &xkan_parameter_init, &xKan_RxHeader, pRxData, XKAN2_RAM_BASE);
        rx_done ++;
        pRxData += BUFSIZE;
        if (rx_done == 4 ) xKan_EnableInterrupt(XKAN2, XKAN_CFG_INT_EN_RXB_UPD_EN, DISABLE);
    }
}

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE, XKAN1_XKAN_RX_IOF_IVAL, XKAN1_XKAN_RX_PAD_SEL, XKAN1_XKAN_RX_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, XKAN1_XKAN_TX_IOF_OVAL, XKAN1_XKAN_TX_PAD_SEL, XKAN1_XKAN_TX_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, XKAN2_XKAN_RX_IOF_IVAL, XKAN2_XKAN_RX_PAD_SEL, XKAN2_XKAN_RX_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, XKAN2_XKAN_TX_IOF_OVAL, XKAN2_XKAN_TX_PAD_SEL, XKAN2_XKAN_TX_HS_SEL, 0, 0);
}

void main(void)
{

    #ifdef MISC_HAS_XKAN1_HAS_CLK
    xkan1_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_XKAN1_RST
    xkan1_set_rst(DISABLE);
    xkan1_set_rst(ENABLE);
    #endif

    #ifdef CFG_SIMULATION
    #ifdef MISC_HAS_XKAN1_CLK_DIV
    xkan1_clk_div(0);
    #endif

    #ifdef MISC_HAS_XKAN1_INTF_CLK_DIV
    xkan1_intf_clk_div(1);
    #endif
    #endif

    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif
    iomux_config();
    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt XKAN2_IRQn */
    ECLIC_Register_IRQ(XKAN2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    XKAN2_IRQHandler);

    xKan_EnableInterrupt(XKAN2, XKAN_CFG_INT_EN_RXB_UPD_EN, ENABLE);

    printf("xkan normal_2.0AB_multi_frame test\n\r");
    /* xkan cfg */ 
    xkan_config();

    /* config int handle */

    /* exit init mode */
    xKan_Start(XKAN1);
    xKan_Start(XKAN2);

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
    while(rx_done != 4){}

    /* compare the read buffer and write buffer */
    if(SUCCESS != MemoryCompare(g_KanTxData, g_KanRxData, BUFSIZE * FRAMENUM)) {
        simulation_fail();
    } else {
        simulation_pass();
    }

    while(1);
}

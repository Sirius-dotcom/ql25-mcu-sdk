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

uint8_t tx_msg[BUFSIZE] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
uint8_t g_KanRxData[BUFSIZE];

volatile uint8_t XKAN2_rxbuf = 0;
volatile uint8_t XKAN2_rxfifo0 = 0;
volatile uint8_t XKAN2_rxfifo1 = 0;

void xkan_config(XKAN_TypeDef *xKanx, xKan_TestModeTypeDef test_mode)
{
    /* step0: clk config */

    /* step1: function config */
    xKan_StructInit(&xkan_parameter_init);
    xkan_parameter_init.working_mode = test_mode;
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
    xkan_parameter_init.ram_info.ExtFiltersNbr = 1;
    xkan_parameter_init.ram_info.RxFifo0ElmtsNbr = 1;
    xkan_parameter_init.ram_info.RxFifo0ElmtSize = XKAN_DATA_BYTES_8;
    xkan_parameter_init.ram_info.RxFifo1ElmtsNbr = 1;
    xkan_parameter_init.ram_info.RxFifo1ElmtSize = XKAN_DATA_BYTES_8;
    xkan_parameter_init.ram_info.RxBuffersNbr = 1;
    xkan_parameter_init.ram_info.RxBufferSize = XKAN_DATA_BYTES_8;

    xkan_parameter_init.ram_info.TxBuffersNbr = 1;
    xkan_parameter_init.ram_info.TxFifoElmtsNbr = 1;
    
    xkan_parameter_init.ram_info.TxElmtSize = XKAN_DATA_BYTES_8;

    xKan_Init(xKanx, &xkan_parameter_init);

    /* step4.1: standard filter config */ 
    xKan_RXFilter.IdType = XKAN_STANDARD_ID; 
    xKan_RXFilter.FilterIndex = 0;
    xKan_RXFilter.FilterType = XKAN_FILTER_DUAL;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXFIFO0;
    xKan_RXFilter.FilterID1 = 0x013;
    xKan_RXFilter.FilterID2 = 0x014;
    
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);
    xKan_RXFilter.IdType = XKAN_STANDARD_ID; 
    xKan_RXFilter.FilterIndex = 1;
    xKan_RXFilter.FilterType = XKAN_FILTER_RANGE;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXFIFO1;
    xKan_RXFilter.FilterID1 = 0x015;
    xKan_RXFilter.FilterID2 = 0x020;
    
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);
    /* step4.2: extend filter config */
    xKan_RXFilter.IdType = XKAN_EXTENDED_ID;
    xKan_RXFilter.FilterIndex = 0;   
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x12345678;
    xKan_RXFilter.FilterID2 = 0x1FFFFFFF;
    
    xKan_RXFilter.RxBufferIndex = 0;
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);

    /* 1.reject frame which has non-matching stand_id or filter extend_id 
     * 2.filter remote frame either standard or extended frame
    */
    xKan_ConfigGlobalFilter(xKanx, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);
    /* step5: rx water mark config */
    xKan_ConfigRxFifoWatermark(xKanx, XKAN_CFG_RX_FIFO0, 0);
    xKan_ConfigRxFifoWatermark(xKanx, XKAN_CFG_RX_FIFO1, 0);
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
void xKan_SendPacket(uint8_t *_DataBuf, uint32_t id)
{
    /* step1: cfg send frame */
    xKan_TxHeader.Identifier = id;
    xKan_TxHeader.IdType = XKAN_EXTENDED_ID;
    xKan_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xKan_TxHeader.DataLength = XKAN_DLC_BYTES_8;
    xKan_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    xKan_TxHeader.BitRateSwitch = XKAN_BRS_ON;
    xKan_TxHeader.FDFormat = XKAN_FD_CAN;
    
    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER0, XKAN1_RAM_BASE);
    xKan_EnableTxBufferRequest(XKAN1, XKAN_TX_BUFFER0);
}

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE, XKAN1_XKAN_RX_IOF_IVAL, XKAN1_XKAN_RX_PAD_SEL, XKAN1_XKAN_RX_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, XKAN1_XKAN_TX_IOF_OVAL, XKAN1_XKAN_TX_PAD_SEL, XKAN1_XKAN_TX_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, XKAN2_XKAN_RX_IOF_IVAL, XKAN2_XKAN_RX_PAD_SEL, XKAN2_XKAN_RX_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, XKAN2_XKAN_TX_IOF_OVAL, XKAN2_XKAN_TX_PAD_SEL, XKAN2_XKAN_TX_HS_SEL, 0, 0);
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
    if(xKan_GetInterruptStatus(XKAN2, XKAN_CFG_INT_STATUS_RXF0_WM_INT) && xKan_GetInterruptEnable(XKAN2, XKAN_CFG_INT_EN_RXF0_WM_EN))
    {
        xKan_RxFifo0Callback(XKAN2, &xkan_parameter_init, &xKan_RxHeader, g_KanRxData, XKAN2_RAM_BASE);
        XKAN2_rxfifo0 += 1;
    }

    if(xKan_GetInterruptStatus(XKAN2, XKAN_CFG_INT_STATUS_RXF1_WM_INT) && xKan_GetInterruptEnable(XKAN2, XKAN_CFG_INT_EN_RXF1_WM_EN))
    {
        xKan_RxFifo1Callback(XKAN2, &xkan_parameter_init, &xKan_RxHeader, g_KanRxData, XKAN2_RAM_BASE);
        XKAN2_rxfifo1 += 1;
    }

    if(xKan_GetInterruptStatus(XKAN2, XKAN_CFG_INT_STATUS_RXB_UPD_INT) && xKan_GetInterruptEnable(XKAN2, XKAN_CFG_INT_EN_RXB_UPD_EN))
    {
        xKan_RxBufferNewMessageCallback(XKAN2, &xkan_parameter_init, &xKan_RxHeader, g_KanRxData, XKAN2_RAM_BASE);
        if(xKan_RxHeader.Identifier == 0x12345678) {
            XKAN2_rxbuf += 1;
        } else {
            XKAN2_rxbuf += 2;
        }
    }
}

void loopback_silent_test(void)
{
    uint32_t tmpreg;
    xkan_config(XKAN1, XKAN_LOOPBACK_MODE);
    xkan_config(XKAN2, XKAN_SILENT_MODE);

    /* exit init mode */
    xKan_Start(XKAN1);
    xKan_Start(XKAN2);
    /*send tx_msg */
    xKan_SendPacket(tx_msg, 0x12345678);
    do {
        tmpreg = XKAN1->CFG_TXB_TRANS;
    } while (!tmpreg);
    XKAN1->CFG_TXB_TRANS = tmpreg;
}

void loopback_loopback_test(void)
{
    uint32_t tmpreg;
    xkan_config(XKAN1, XKAN_LOOPBACK_MODE);
    xkan_config(XKAN2, XKAN_LOOPBACK_MODE);

    /* exit init mode */
    xKan_Start(XKAN1);
    xKan_Start(XKAN2);
    /*send tx_msg */
    xKan_SendPacket(tx_msg, 0x13);
    do {
        tmpreg = XKAN1->CFG_TXB_TRANS;
    } while (!tmpreg);
    XKAN1->CFG_TXB_TRANS = tmpreg;
}

void silent_normal_test(void)
{
    uint32_t tmpreg;
    xkan_config(XKAN1, XKAN_SILENT_MODE);
    xkan_config(XKAN2, XKAN_NORMAL_MODE);

    /* exit init mode */
    xKan_Start(XKAN1);
    xKan_Start(XKAN2);
    /*send tx_msg */
    xKan_SendPacket(tx_msg, 0x14);
    do {
        tmpreg = XKAN1->CFG_TXB_TRANS;
    } while (!tmpreg);
    XKAN1->CFG_TXB_TRANS = tmpreg;
}

void loopback_loopbackSilent_test(void)
{
    uint32_t tmpreg;
    xkan_config(XKAN1, XKAN_LOOPBACK_MODE);
    xkan_config(XKAN2, XKAN_SILENT_LOOPBACK_MODE);

    /* exit init mode */
    xKan_Start(XKAN1);
    xKan_Start(XKAN2);
    /*send tx_msg */
    xKan_SendPacket(tx_msg, 0x15);
    do {
        tmpreg = XKAN1->CFG_TXB_TRANS;
    } while (!tmpreg);
    XKAN1->CFG_TXB_TRANS = tmpreg;
}

void loopbackSilent_normal_test(void)
{
    uint32_t tmpreg;
    xkan_config(XKAN1, XKAN_SILENT_LOOPBACK_MODE);
    xkan_config(XKAN2, XKAN_NORMAL_MODE);

    /* exit init mode */
    xKan_Start(XKAN1);
    xKan_Start(XKAN2);
    /*send tx_msg */
    xKan_SendPacket(tx_msg, 0x15);
    do {
        tmpreg = XKAN1->CFG_TXB_TRANS;
    } while (!tmpreg);
    XKAN1->CFG_TXB_TRANS = tmpreg;
}

void interface_rst(void)
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

}

void main(void)
{
    interface_rst();
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif

    printf("xkan test mode\n\r");
    /* xkan cfg */ 
    iomux_config();
    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt XKAN2_IRQn */
    ECLIC_Register_IRQ(XKAN2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    XKAN2_IRQHandler);

    xKan_EnableInterrupt(XKAN2, XKAN_CFG_INT_EN_RXF0_WM_EN, ENABLE);
    xKan_EnableInterrupt(XKAN2, XKAN_CFG_INT_EN_RXF1_WM_EN, ENABLE);
    xKan_EnableInterrupt(XKAN2, XKAN_CFG_INT_EN_RXB_UPD_EN, ENABLE);

    loopback_silent_test();

    interface_rst();
    loopback_loopback_test();

    interface_rst();
    silent_normal_test();

    interface_rst();
    loopback_loopbackSilent_test();

    interface_rst();
    loopbackSilent_normal_test();

    if((XKAN2_rxbuf == 1) && (XKAN2_rxfifo0 == 0) && (XKAN2_rxfifo1 == 0)) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while(1);
}

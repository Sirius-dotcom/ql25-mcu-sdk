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
uint8_t g_KanRxData[BUFSIZE];

volatile ErrStatus bus_off_int_sta = ERROR;
volatile uint32_t rec_cnt[2] = {0};
volatile uint32_t err_tec = 0;

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
    xkan_parameter_init.normal_time.NominalPrescaler = 0x2;
    xkan_parameter_init.normal_time.NominalSyncJumpWidth = 0x3;
    xkan_parameter_init.normal_time.NominalTimeSeg1 = 0xb;
    xkan_parameter_init.normal_time.NominalTimeSeg2 = 0x4;

    /* step2.2: data bit timing config(sample-point:75%) */
    xkan_parameter_init.data_time.DataPrescaler = 0x1;
    xkan_parameter_init.data_time.DataSyncJumpWidth = 0x3;
    xkan_parameter_init.data_time.DataTimeSeg1 = 0xb;
    xkan_parameter_init.data_time.DataTimeSeg2 = 0x4;
    
    /* step3: ram config */
    xkan_parameter_init.ram_info.MessageRAMOffset = 0;
    xkan_parameter_init.ram_info.StdFiltersNbr = 1;
    xkan_parameter_init.ram_info.ExtFiltersNbr = 0;
    xkan_parameter_init.ram_info.RxFifo0ElmtsNbr = 0;
    xkan_parameter_init.ram_info.RxFifo0ElmtSize = XKAN_DATA_BYTES_8;
    xkan_parameter_init.ram_info.RxFifo1ElmtsNbr = 0;
    xkan_parameter_init.ram_info.RxFifo1ElmtSize = XKAN_DATA_BYTES_8;
    xkan_parameter_init.ram_info.RxBuffersNbr = 1;
    xkan_parameter_init.ram_info.RxBufferSize = XKAN_DATA_BYTES_8;

    xkan_parameter_init.ram_info.TxBuffersNbr = 32;
    xkan_parameter_init.ram_info.TxFifoElmtsNbr = 0;
    
    xkan_parameter_init.ram_info.TxElmtSize = XKAN_DATA_BYTES_8;

    xKan_Init(XKAN0, &xkan_parameter_init);
    xKan_Init(XKAN1, &xkan_parameter_init);

    /* step4.1: standard filter config */
    xKan_RXFilter.IdType = XKAN_STANDARD_ID;
    xKan_RXFilter.FilterIndex = 0;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x123;
    xKan_RXFilter.FilterID2 = 0x7FF;
    xKan_RXFilter.RxBufferIndex = 0;
    xKan_ConfigFilter(XKAN0, &xkan_parameter_init, &xKan_RXFilter, XKAN0_RAM_BASE);
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);

    /* 1.reject frame which has non-matching stand_id or filter extend_id 
     * 2.filter remote frame either standard or extended frame
    */
    xKan_ConfigGlobalFilter(XKAN0, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);
    xKan_ConfigGlobalFilter(XKAN1, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);
}

void xkan1_SendClassicPacket(uint8_t *_DataBuf, uint32_t _Len)
{
    /* step1: cfg send frame */
    xKan_TxHeader.Identifier = 0x123;
    xKan_TxHeader.IdType = XKAN_STANDARD_ID;
    xKan_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xKan_TxHeader.DataLength = _Len;
    xKan_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    xKan_TxHeader.BitRateSwitch = XKAN_BRS_OFF;
    xKan_TxHeader.FDFormat = XKAN_CLASSIC_CAN;

    if(_Len == XKAN_DLC_BYTES_8) {
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER17, XKAN1_RAM_BASE);
        xKan_EnableTxBufferRequest(XKAN1, XKAN_TX_BUFFER17);
        while ((XKAN1->CFG_ESR & XKAN_CFG_ESR_TEC) < 128);

        do{
            __NOP();
        } while ((xKan_TxHeader.Identifier--));
    } else {
        for(int i = 0; i <= 15; i++) {
            XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, (1<<i), XKAN1_RAM_BASE);
        }
        xKan_EnableTxBufferRequest(XKAN1, 0xFFFF);
    }
}

void xkan1_SendCanFdPacket(uint8_t *_DataBuf, uint32_t _Len)
{
    /* step1: cfg send frame */
    xKan_TxHeader.Identifier = 0x124;
    xKan_TxHeader.IdType = XKAN_STANDARD_ID;
    xKan_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xKan_TxHeader.DataLength = _Len;
    xKan_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    xKan_TxHeader.BitRateSwitch = XKAN_BRS_ON;
    xKan_TxHeader.FDFormat = XKAN_FD_CAN;

    XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, XKAN_TX_BUFFER16, XKAN1_RAM_BASE);

    xKan_EnableTxBufferRequest(XKAN1, XKAN_TX_BUFFER16);
}

void xkan0_SendClassicPacket(uint8_t *_DataBuf, uint32_t _Len)
{
    /* step1: cfg send frame */
    xKan_TxHeader.Identifier = 0x123;
    xKan_TxHeader.IdType = XKAN_STANDARD_ID;
    xKan_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xKan_TxHeader.DataLength = _Len;
    xKan_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    xKan_TxHeader.BitRateSwitch = XKAN_BRS_OFF;
    xKan_TxHeader.FDFormat = XKAN_CLASSIC_CAN;
    for(int k = 0; k < 4; k++) {
        for(int i = 0; i < 32; i++) {
            XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN0, &xKan_TxHeader, _DataBuf, (1<<i), XKAN0_RAM_BASE);
        }
        xKan_EnableTxBufferRequest(XKAN0, 0xFFFFFFFF);
        while (!(XKAN0->CFG_ESR & XKAN_CFG_ESR_BUS_OFF));

        #ifdef MISC_HAS_XKAN0_RST
        xkan0_set_rst(DISABLE);
        xkan0_set_rst(ENABLE);
        #endif
        xkan_config();

        /* config int handle */

        /* exit init mode */
        xKan_Start(XKAN0);
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

void XKAN1_IRQHandler(void)
{
    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_EN_BUS_OFF_INT_EN) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_STATUS_BUS_OFF_INT))
    {
        xKan_ClearInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_BUS_OFF_INT);
        xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_BUS_OFF_INT_EN, DISABLE);
        err_tec = (XKAN1->CFG_ESR & XKAN_CFG_ESR_TEC);
        if (err_tec == 0) {
            bus_off_int_sta = SUCCESS;
        }
        rec_cnt[1] = (XKAN1->CFG_ESR & XKAN_CFG_ESR_REC >> 8);
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

    printf("xkan normal_err_rec_unaffect_tec\n\r");
    /* xkan cfg */ 

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt XKAN0_IRQn */
    ECLIC_Register_IRQ(XKAN1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    XKAN1_IRQHandler);

    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_BUS_OFF_INT_EN, ENABLE);

    xkan_config();

    /* config int handle */

    /* exit init mode */
    xKan_Start(XKAN0);
    xKan_Start(XKAN1);

    /* fill tx_msg buffer */
    for(int i = 0; i < BUFSIZE; i++){
        tx_msg[i] = i + 0x10;
    }

    /* send msg tx_msg for tec:128 -1 = 127 */
    xkan1_SendClassicPacket(tx_msg, XKAN_DLC_BYTES_1);
    xkan1_SendCanFdPacket(tx_msg, XKAN_DLC_BYTES_1);
    while ((XKAN1->CFG_ESR & XKAN_CFG_ESR_TEC) != 127);

    /* xkan1 rec up to 128 */
    xkan0_SendClassicPacket(tx_msg, XKAN_DLC_BYTES_1);

    /* xkan1 seng a frame with passive err */
    xkan1_SendClassicPacket(tx_msg, XKAN_DLC_BYTES_8);

    simulation_pass();

    while(1);
}

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
#define FRAME_TYPE 3
#define TEST_CLYCLE 5

xKan_InitTypeDef xkan_parameter_init = {0};
xKan_FilterTypeDef xKan_RXFilter = {0};
xKan_TxHeaderTypeDef xKan_TxHeader = {0};
xKan_RxHeaderTypeDef xKan_RxHeader = {0};

uint8_t tx_msg[BUFSIZE];
uint8_t g_KanRxData[BUFSIZE];

volatile ErrStatus status = ERROR;
volatile ErrStatus arb_lost_sta = ERROR;
volatile ErrStatus err_det_int_sta = ERROR;

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

    /* step2.1: nominal bit timing config */
    xkan_parameter_init.normal_time.NominalPrescaler = 0x1;
    xkan_parameter_init.normal_time.NominalSyncJumpWidth = 0x1;
    xkan_parameter_init.normal_time.NominalTimeSeg1 = 0x3;
    xkan_parameter_init.normal_time.NominalTimeSeg2 = 0x2;

    /* step2.2: data bit timing config */
    xkan_parameter_init.data_time.DataPrescaler = 0x1;
    xkan_parameter_init.data_time.DataSyncJumpWidth = 0x0;
    xkan_parameter_init.data_time.DataTimeSeg1 = 0x1;
    xkan_parameter_init.data_time.DataTimeSeg2 = 0x1;
    
    /* step3: ram config */
    xkan_parameter_init.ram_info.MessageRAMOffset = 0;
    xkan_parameter_init.ram_info.StdFiltersNbr = 2;
    xkan_parameter_init.ram_info.ExtFiltersNbr = 3;
    xkan_parameter_init.ram_info.RxFifo0ElmtsNbr = 0;
    xkan_parameter_init.ram_info.RxFifo0ElmtSize = XKAN_DATA_BYTES_64;
    xkan_parameter_init.ram_info.RxFifo1ElmtsNbr = 0;
    xkan_parameter_init.ram_info.RxFifo1ElmtSize = XKAN_DATA_BYTES_64;
    xkan_parameter_init.ram_info.RxBuffersNbr = 5;
    xkan_parameter_init.ram_info.RxBufferSize = XKAN_DATA_BYTES_64;

    xkan_parameter_init.ram_info.TxBuffersNbr = 5;
    xkan_parameter_init.ram_info.TxFifoElmtsNbr = 0;
    
    xkan_parameter_init.ram_info.TxElmtSize = XKAN_DATA_BYTES_64;

    xKan_Init(XKAN1, &xkan_parameter_init);
    xKan_Init(XKAN2, &xkan_parameter_init);

    /* for bit timing gen */
    XKAN1->CFG_DBTR = ((((uint32_t)0x3) - 1 << 14)  | (((uint32_t)0x4 - 1) << 10) | (((uint32_t)0xb - 1) << 5)  | ((uint32_t)0x2 - 1));
    XKAN2->CFG_DBTR = ((((uint32_t)0x3) - 1 << 14)  | (((uint32_t)0x4 - 1) << 10) | (((uint32_t)0xb - 1) << 5)  | ((uint32_t)0x2 - 1));
    XKAN1->CFG_NBTR = ((((uint32_t)0x3) - 1 << 24)  | (((uint32_t)0x9 - 1) << 17) | (((uint32_t)0x16 - 1) << 9)  | ((uint32_t)0x8 - 1));
    XKAN2->CFG_NBTR = ((((uint32_t)0x3) - 1 << 24)  | (((uint32_t)0x9 - 1) << 17) | (((uint32_t)0x16 - 1) << 9)  | ((uint32_t)0x8 - 1));

    /* step4.1: standard filter config */
    xKan_RXFilter.IdType = XKAN_STANDARD_ID; 
    xKan_RXFilter.FilterIndex = 0;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x11;
    xKan_RXFilter.FilterID2 = 0x7FF;
    xKan_RXFilter.RxBufferIndex = 0;
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);
    xKan_RXFilter.IdType = XKAN_STANDARD_ID; 
    xKan_RXFilter.FilterIndex = 1;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x110;
    xKan_RXFilter.FilterID2 = 0x7FF;
    xKan_RXFilter.RxBufferIndex = 1;
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);
    xKan_RXFilter.IdType = XKAN_EXTENDED_ID; 
    xKan_RXFilter.FilterIndex = 0;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x00123456;
    xKan_RXFilter.FilterID2 = 0x1FFFFFFF;
    xKan_RXFilter.RxBufferIndex = 2;
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);
    xKan_RXFilter.IdType = XKAN_EXTENDED_ID; 
    xKan_RXFilter.FilterIndex = 1;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x05543210;
    xKan_RXFilter.FilterID2 = 0x1FFFFFFF;
    xKan_RXFilter.RxBufferIndex = 3;
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);
    xKan_RXFilter.IdType = XKAN_EXTENDED_ID; 
    xKan_RXFilter.FilterIndex = 2;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x17654321;
    xKan_RXFilter.FilterID2 = 0x1FFFFFFF;
    xKan_RXFilter.RxBufferIndex = 4;
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);

    /* step4.2: extend filter config */

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
void xKan_SendPacket(uint8_t *_DataBuf, uint32_t _Len, uint32_t rval)
{
    xKan_TxHeader.IdType = XKAN_EXTENDED_ID;
    xKan_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xKan_TxHeader.DataLength = _Len;
    xKan_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    if(_Len == XKAN_DLC_BYTES_8) {
        xKan_TxHeader.BitRateSwitch = XKAN_BRS_OFF;
        xKan_TxHeader.FDFormat = XKAN_CLASSIC_CAN;
    } else if (_Len == XKAN_DLC_BYTES_16) {
        xKan_TxHeader.BitRateSwitch = XKAN_BRS_OFF;
        xKan_TxHeader.FDFormat = XKAN_FD_CAN;
    } else if (_Len == XKAN_DLC_BYTES_64) {
        xKan_TxHeader.BitRateSwitch = XKAN_BRS_ON;
        xKan_TxHeader.FDFormat = XKAN_FD_CAN;
    } else {
        xKan_TxHeader.BitRateSwitch = XKAN_BRS_OFF;
        xKan_TxHeader.FDFormat = XKAN_CLASSIC_CAN;
    }

    switch (rval) {
    case 0:
        xKan_TxHeader.IdType = XKAN_STANDARD_ID;
        xKan_TxHeader.Identifier = 0x11;
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, (XKAN_TX_BUFFER0 << rval), XKAN1_RAM_BASE);
        xKan_TxHeader.Identifier = 0x12;
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN2, &xKan_TxHeader, _DataBuf, (XKAN_TX_BUFFER0 << rval), XKAN2_RAM_BASE);

        break;
    case 1:
        xKan_TxHeader.IdType = XKAN_STANDARD_ID;
        xKan_TxHeader.Identifier = 0x110;
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, (XKAN_TX_BUFFER0 << rval), XKAN1_RAM_BASE);
        xKan_TxHeader.Identifier = 0x111;
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN2, &xKan_TxHeader, _DataBuf, (XKAN_TX_BUFFER0 << rval), XKAN2_RAM_BASE);
        break;
    case 2:
        xKan_TxHeader.Identifier = 0x00123456;
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, (XKAN_TX_BUFFER0 << rval), XKAN1_RAM_BASE);
        xKan_TxHeader.Identifier = 0x00143456;
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN2, &xKan_TxHeader, _DataBuf, (XKAN_TX_BUFFER0 << rval), XKAN2_RAM_BASE);
        break;
    case 3:
        xKan_TxHeader.Identifier = 0x05543210;
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, (XKAN_TX_BUFFER0 << rval), XKAN1_RAM_BASE);
        xKan_TxHeader.Identifier = 0x05543211;
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN2, &xKan_TxHeader, _DataBuf, (XKAN_TX_BUFFER0 << rval), XKAN2_RAM_BASE);
        break;
    case 4:
        xKan_TxHeader.Identifier = 0x17654321;
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, (XKAN_TX_BUFFER0 << rval), XKAN1_RAM_BASE);
        xKan_TxHeader.Identifier = 0x17654331;
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN2, &xKan_TxHeader, _DataBuf, (XKAN_TX_BUFFER0 << rval), XKAN2_RAM_BASE);
        break;
    default:
        xKan_TxHeader.Identifier = 0x1d;
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, (XKAN_TX_BUFFER0 << rval), XKAN1_RAM_BASE);
        xKan_TxHeader.Identifier = 0x12;
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN2, &xKan_TxHeader, _DataBuf, (XKAN_TX_BUFFER0 << rval), XKAN2_RAM_BASE);
        break;
    }

    xKan_EnableTxBufferRequest(XKAN1, (XKAN_TX_BUFFER0 << rval));
    xKan_EnableTxBufferRequest(XKAN2, (XKAN_TX_BUFFER0 << rval));

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
    if(xKan_GetInterruptStatus(XKAN2, XKAN_CFG_INT_STATUS_ARBITRATION_LOST_INT) && xKan_GetInterruptEnable(XKAN2, XKAN_CFG_INT_EN_ARBITRATION_LOST_INT_EN))
    {
        xKan_ClearInterruptStatus(XKAN2, XKAN_CFG_INT_STATUS_ARBITRATION_LOST_INT);
        arb_lost_sta = SUCCESS;
    }
    if(xKan_GetInterruptStatus(XKAN2, XKAN_CFG_INT_EN_ERROR_DET_INT_EN) && xKan_GetInterruptEnable(XKAN2, XKAN_CFG_INT_STATUS_ERROR_DET_INT))
    {
        xKan_ClearInterruptStatus(XKAN2, XKAN_CFG_INT_STATUS_ERROR_DET_INT);
        err_det_int_sta = SUCCESS;
    }
}

void main(void)
{
    uint32_t dlc;
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

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt XKAN2_IRQn */
    ECLIC_Register_IRQ(XKAN2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    XKAN2_IRQHandler);

    xKan_EnableInterrupt(XKAN2, XKAN_CFG_INT_EN_ARBITRATION_LOST_INT_EN, ENABLE);
    xKan_EnableInterrupt(XKAN2, XKAN_CFG_INT_EN_ERROR_DET_INT_EN, ENABLE);

    printf("normal_arb_lost_test\n\r");

    /* xkan cfg */ 
    iomux_config();
    xkan_config();

    /* config int handle */

    /* exit init mode */
    xKan_Start(XKAN1);
    xKan_Start(XKAN2);
    xKan_EnableKernel(XKAN1,DISABLE);
    xKan_EnableKernel(XKAN2,DISABLE);
    /* fill tx_msg buffer */
    for(int i = 0; i < BUFSIZE; i++){
        tx_msg[i] = BUFSIZE - i;
    }

    for(int k = 0; k < FRAME_TYPE; k++) {
        for(int i = 0; i < TEST_CLYCLE; i++) {
            /*send tx_msg */
            switch(k) {
                case 0:
                    dlc = XKAN_DLC_BYTES_8;
                    break;
                case 1:
                    dlc = XKAN_DLC_BYTES_64;
                    break;
                case 2:
                    dlc = XKAN_DLC_BYTES_16;
                    break;
                default:
                    dlc = XKAN_DLC_BYTES_8;
                    break;
            }
            xKan_SendPacket(tx_msg, dlc, i);
            XKAN1->CFG_MCR = 0x36181;
            XKAN2->CFG_MCR = 0x36181;
            while(XKAN2->CFG_RXDATL_RSP == 0 && XKAN2->CFG_RXDATH_RSP == 0);

            xKan_RxBufferNewMessageCallback(XKAN2, &xkan_parameter_init, &xKan_RxHeader, g_KanRxData, XKAN2_RAM_BASE);

            if((SUCCESS == err_det_int_sta)) {
                simulation_fail();
            }

        }
    }

    if((SUCCESS == arb_lost_sta)) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while(1);
}

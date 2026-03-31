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

volatile ErrStatus status = ERROR;
volatile ErrStatus err_det_int_sta = ERROR;
volatile ErrStatus bus_off_int_sta = ERROR;
volatile ErrStatus passive_sta = ERROR;
volatile uint32_t err_cnt = 0;
volatile uint32_t err_passive = 0;
volatile uint32_t err_tec = 0;
volatile uint32_t err_rec = 0;
volatile uint32_t err_code = 0;

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

    xKan_Init(XKAN1, &xkan_parameter_init);
    xKan_Init(XKAN2, &xkan_parameter_init);

    /* step4.1: standard filter config */
    xKan_RXFilter.IdType = XKAN_STANDARD_ID;
    xKan_RXFilter.FilterIndex = 0;
    xKan_RXFilter.FilterType = XKAN_FILTER_MASK;
    xKan_RXFilter.FilterConfig = XKAN_FILTER_TO_RXBUFFER;
    xKan_RXFilter.FilterID1 = 0x123;
    xKan_RXFilter.FilterID2 = 0x7FF;
    xKan_RXFilter.RxBufferIndex = 0;
    xKan_ConfigFilter(XKAN1, &xkan_parameter_init, &xKan_RXFilter, XKAN1_RAM_BASE);
    xKan_ConfigFilter(XKAN2, &xkan_parameter_init, &xKan_RXFilter, XKAN2_RAM_BASE);

    /* 1.reject frame which has non-matching stand_id or filter extend_id 
     * 2.filter remote frame either standard or extended frame
    */
    xKan_ConfigGlobalFilter(XKAN1, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);
    xKan_ConfigGlobalFilter(XKAN2, XKAN_REJECT, XKAN_REJECT, XKAN_FILTER_REMOTE, XKAN_FILTER_REMOTE);
}

void xKan_SendClassicPacket(uint8_t *_DataBuf, uint32_t _Len)
{
    /* step1: cfg send frame */
    xKan_TxHeader.Identifier = 0x123;
    xKan_TxHeader.IdType = XKAN_STANDARD_ID;
    xKan_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xKan_TxHeader.DataLength = _Len;
    xKan_TxHeader.ErrorStateIndicator = XKAN_ESI_ACTIVE;
    xKan_TxHeader.BitRateSwitch = XKAN_BRS_OFF;
    xKan_TxHeader.FDFormat = XKAN_CLASSIC_CAN;
    for(int i = 0; i < 32; i++) {
        XKAN_AddMessageToTxBuffer(&xkan_parameter_init, XKAN1, &xKan_TxHeader, _DataBuf, (1<<i), XKAN1_RAM_BASE);
    }

    xKan_EnableTxBufferRequest(XKAN1, 0xFFFFFFFF);
}

void xKan_SendCanFdPacket(uint8_t *_DataBuf, uint32_t _Len)
{
    /* step1: cfg send frame */
    xKan_TxHeader.Identifier = 0x123;
    xKan_TxHeader.IdType = XKAN_STANDARD_ID;
    xKan_TxHeader.TxFrameType = XKAN_DATA_FRAME;
    xKan_TxHeader.DataLength = _Len;
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

void XKAN1_IRQHandler(void)
{
    __disable_irq();
    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_EN_ERROR_DET_INT_EN) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_STATUS_ERROR_DET_INT))
    {

        xKan_ClearInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_ERROR_DET_INT);
        err_det_int_sta = SUCCESS;
        err_cnt ++;
        /* clear pending bit */
        xKan_ClearTxReqPending(XKAN1, (1<<(32-err_cnt)));
        if(err_cnt == 16) {
            err_tec = (XKAN1->CFG_ESR & XKAN_CFG_ESR_TEC);
            err_passive = (XKAN1->CFG_ESR & XKAN_CFG_ESR_ERR_PASS_FLAG);
            if (err_tec == 128 && err_passive){
                passive_sta = SUCCESS;
            }
        }
        if(err_cnt == 32) {
            err_code = (XKAN1->CFG_ESR & XKAN_CFG_ESR_LEC_MASK) >> XKAN_CFG_ESR_LEC_OFS;
            xKan_ClearErrCode(XKAN1);
            xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_ERROR_DET_INT_EN, DISABLE);
        }
    }

    if(xKan_GetInterruptStatus(XKAN1, XKAN_CFG_INT_EN_BUS_OFF_INT_EN) && xKan_GetInterruptEnable(XKAN1, XKAN_CFG_INT_STATUS_BUS_OFF_INT))
    {
        xKan_ClearInterruptStatus(XKAN1, XKAN_CFG_INT_STATUS_BUS_OFF_INT);
        xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_BUS_OFF_INT_EN, DISABLE);
        err_tec = (XKAN1->CFG_ESR & XKAN_CFG_ESR_TEC);
        if (err_tec == 0) {
            bus_off_int_sta = SUCCESS;
        }
    }
    __enable_irq();
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

    printf("xkan normal_err_basic_test\n\r");
    /* xkan cfg */ 
    iomux_config();

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt XKAN1_IRQn */
    ECLIC_Register_IRQ(XKAN1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 0,
                                    XKAN1_IRQHandler);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_ERROR_DET_INT_EN, ENABLE);
    xKan_EnableInterrupt(XKAN1, XKAN_CFG_INT_EN_BUS_OFF_INT_EN, ENABLE);

    xkan_config();

    /* config int handle */

    /* exit init mode */
    xKan_Start(XKAN1);
    xKan_Start(XKAN2);

    /* fill tx_msg buffer */
    for(int i = 0; i < BUFSIZE; i++){
        tx_msg[i] = i + 0x10;
    }

    /* send classic tx_msg for gen bus-off */
    xKan_SendClassicPacket(tx_msg, XKAN_DLC_BYTES_8);

    while (!bus_off_int_sta) {}

    /* wait 128 * 11bit passive */
    while ((XKAN1->CFG_ESR & XKAN_CFG_ESR_BUS_OFF)) {}
    
    xKan_SendCanFdPacket(tx_msg, XKAN_DLC_BYTES_8);

    /* wait for rx data */
    while(XKAN2->CFG_RXDATL_RSP == 0 && XKAN2->CFG_RXDATH_RSP == 0);

    xKan_RxBufferNewMessageCallback(XKAN2, &xkan_parameter_init, &xKan_RxHeader, g_KanRxData, XKAN2_RAM_BASE);

    /* compare the read buffer and write buffer */
    if(SUCCESS != MemoryCompare(tx_msg, g_KanRxData, BUFSIZE) && (SUCCESS != bus_off_int_sta) && (SUCCESS != err_det_int_sta) && (SUCCESS != passive_sta) && (SUCCESS !=  err_code)) {
        simulation_fail();
    } else {
        simulation_pass();
    }

    while(1);
}

/*
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier Apache-2.0
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

#include <string.h>
#include "ns.h"
#include "ns_xkan.h"
#include "ns_conf.h"

/* ------------------------------------------------------------- Private macro ------------------------------------------------------------- */
#define XKAN_ELEMENT_MASK_STDID ((uint32_t)0x1FFC0000U) /* Standard Identifier Mask    */
#define XKAN_ELEMENT_MASK_EXTID ((uint32_t)0x1FFFFFFFU) /* Extended Identifier Mask    */
#define XKAN_ELEMENT_MASK_XTD   ((uint32_t)0x20000000U) /* Extended Identifier         */
#define XKAN_ELEMENT_MASK_RTR   ((uint32_t)0x40000000U) /* Remote Transmission Request */
#define XKAN_ELEMENT_MASK_TS    ((uint32_t)0x0000FFFFU) /* Timestamp                   */
#define XKAN_ELEMENT_MASK_DLC   ((uint32_t)0x000F0000U) /* Data Length Code            */
#define XKAN_ELEMENT_MASK_BRS   ((uint32_t)0x00100000U) /* Bit Rate Switch             */
#define XKAN_ELEMENT_MASK_FDF   ((uint32_t)0x00200000U) /* FD Format                   */
#define XKAN_ELEMENT_MASK_ESI   ((uint32_t)0x00400000U) /* Error State Indicator       */
#define XKAN_ELEMENT_MASK_FIDX  ((uint32_t)0x7F000000U) /* Filter Index                */
#define XKAN_ELEMENT_MASK_ANMF  ((uint32_t)0x80000000U) /* Accepted Non-matching Frame */

/* ------------------------------------------------------------- Private variables ---------------------------------------------------------*/
DECLARE_PRIVATE_ARRAY(uint8_t, DLCtoBytes, 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64);
DECLARE_PRIVATE_ARRAY(uint8_t, CvtEltSize, 0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 5, 0, 0, 0, 6, 0, 0, 0, 7);

static ErrStatus xKan_CalcultateRamBlockAddresses(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init);
static ErrStatus xKanx_CopyMessageToRAM(xKan_InitTypeDef *xkan_parameter_init, xKan_TxHeaderTypeDef *pTxHeader, uint8_t *pTxData, uint32_t BufferIndex, ramtype_t xkanRamAddr);
static void xKan_ConfigStandardIdMask(XKAN_TypeDef *xKanx, uint32_t Mask);
static void xKan_ConfigExtendedIdMask(XKAN_TypeDef *xKanx, uint32_t Mask);
/**
  * \brief  xKan info struct initial.
  * \param  xkan_parameter_init pointer to an xKan_InitTypeDef structure that contains
  *         the configuration information for the specified xKan.
  * \retval none
  */
void xKan_StructInit(xKan_InitTypeDef *xkan_parameter_init)
{
    xkan_parameter_init->working_mode = XKAN_LOOPBACK_MODE;
    xkan_parameter_init->frame_format = XKAN_FRAME_CLASSIC;
    xkan_parameter_init->protocol_except_handle = XKAN_EXCEPT_NACK;
    xkan_parameter_init->time_stamp = DISABLE;
    
    xkan_parameter_init->auto_retransmit = DISABLE;
    xkan_parameter_init->tx_delay_compensation = DISABLE;
    xkan_parameter_init->transmit_pause = DISABLE;
    xkan_parameter_init->receive_in_arb_loss = ENABLE;

    xkan_parameter_init->normal_time.NominalPrescaler = 0;
    xkan_parameter_init->normal_time.NominalSyncJumpWidth = 0;
    xkan_parameter_init->normal_time.NominalTimeSeg1 = 0;
    xkan_parameter_init->normal_time.NominalTimeSeg2 = 0;

    xkan_parameter_init->data_time.DataPrescaler = 0;
    xkan_parameter_init->data_time.DataSyncJumpWidth = 0;
    xkan_parameter_init->data_time.DataTimeSeg1 = 0;
    xkan_parameter_init->data_time.DataTimeSeg2 = 0;

    memset(&(xkan_parameter_init->ram_info), 0, sizeof(xKan_RamInfoTypeDef));
}

/**
  * \brief  Initialize flexan.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  xkan_parameter_init pointer to an xKan_InitTypeDef structure that contains
  *         the configuration information for the specified xKan.
  * \retval none
  */
ErrStatus xKan_Init(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init)
{
    /* disable xkan */
    xKan_EnableKernel(xKanx, DISABLE);

    /* exit sleep mode */
    xKan_Sleep(xKanx, DISABLE);

    /* exit clock stop mode */
    xKan_ClkStop(xKanx, DISABLE);

    /* enter initialisation state */
    xKan_EnterInit(xKanx);

    /* Configure xkan feature */
    /* time trigger communication mode */
    if (ENABLE == xkan_parameter_init->time_stamp) {
        xKanx->CFG_MCR |= XKAN_CFG_MCR_TIMESTAMP_EN;
    } else {
        xKanx->CFG_MCR &= ~XKAN_CFG_MCR_TIMESTAMP_EN;
    }

    /* Configure automatic retransmit mode */
    if (ENABLE == xkan_parameter_init->auto_retransmit) {
        xKanx->CFG_MCR |= XKAN_CFG_MCR_AUTO_RETRANS_EN;
    } else {
        xKanx->CFG_MCR &= ~XKAN_CFG_MCR_AUTO_RETRANS_EN;
    }

    /* Configure transmit compensation */
    if (ENABLE == xkan_parameter_init->tx_delay_compensation) {
        xKanx->CFG_MCR |= XKAN_CFG_MCR_TDC_EN;
    } else {
        xKanx->CFG_MCR &= ~XKAN_CFG_MCR_TDC_EN;
    }

    /* Configure tx pause */
    if (ENABLE == xkan_parameter_init->transmit_pause) {
        xKanx->CFG_MCR |= XKAN_CFG_MCR_TXPAUSE;
    } else {
        xKanx->CFG_MCR &= ~XKAN_CFG_MCR_TXPAUSE;
    }

    /* Configure receive mode in case of loss arbitration */
    if (ENABLE == xkan_parameter_init->receive_in_arb_loss) {
        xKanx->CFG_MCR |= XKAN_CFG_MCR_BE_RECEIVER_IN_LOSS;
    } else {
        xKanx->CFG_MCR &= ~XKAN_CFG_MCR_BE_RECEIVER_IN_LOSS;
    }

    /* Configure protocol exception handling */
    xKanx->CFG_MCR &= ~XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING_MASK;
    if (XKAN_EXCEPT_NACK == xkan_parameter_init->protocol_except_handle) {
        xKanx->CFG_MCR |= XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING_PRO_EXE_AS_PXE;
    } else if (XKAN_EXCEPT_RSP_FMT_ERR == xkan_parameter_init->protocol_except_handle){
        xKanx->CFG_MCR |= XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING_PRO_EXE_AS_FORM_ERR;
    } else if (XKAN_EXCEPT_ACK == xkan_parameter_init->protocol_except_handle){
        xKanx->CFG_MCR |= XKAN_CFG_MCR_PROTOCOL_EXCEPTION_HANDLING_PRO_EXE_AS_VALID;
    } else {
        return ERROR;
    }

    /* Set XKAN Frame Format */
    if(XKAN_FRAME_CLASSIC == xkan_parameter_init->frame_format) {
        xKanx->CFG_MCR &= (~(XKAN_CFG_MCR_FD_EN | XKAN_CFG_MCR_BRS_EN));
    } else if (XKAN_FRAME_FD_NO_BRS == xkan_parameter_init->frame_format) {
        xKanx->CFG_MCR |= (XKAN_CFG_MCR_FD_EN & (~XKAN_CFG_MCR_BRS_EN));
    } else if (XKAN_FRAME_FD_BRS == xkan_parameter_init->frame_format){
        xKanx->CFG_MCR |= (XKAN_CFG_MCR_FD_EN | XKAN_CFG_MCR_BRS_EN);
    } else {
        return ERROR;
    }

    /* Set xKan Operating Mode */
    if(XKAN_NORMAL_MODE == xkan_parameter_init->working_mode) {
        xKanx->CFG_TEST &= ~(XKAN_CFG_TEST_LBKM|XKAN_CFG_TEST_SILM);
    } else {
        if (XKAN_LOOPBACK_MODE == xkan_parameter_init->working_mode) {
            xKanx->CFG_TEST |= (XKAN_CFG_TEST_LBKM);
        } else if (XKAN_SILENT_MODE == xkan_parameter_init->working_mode) {
            xKanx->CFG_TEST |= (XKAN_CFG_TEST_SILM);
        } else if (XKAN_SILENT_LOOPBACK_MODE == xkan_parameter_init->working_mode) {
            xKanx->CFG_TEST |= (XKAN_CFG_TEST_LBKM | XKAN_CFG_TEST_SILM);
        } else if (XKAN_RESTRICTED_OPERATION_MODE == xkan_parameter_init->working_mode) {
            xKanx->CFG_MCR |= (XKAN_CFG_MCR_RESTRICTED_OPERATION);
        } else {
            return ERROR;
        }
    }

    /* Check the value of the normal_time */
    if(IS_XKAN_GREATER(xkan_parameter_init->normal_time.NominalTimeSeg2, xkan_parameter_init->normal_time.NominalSyncJumpWidth)) {
        /* Set the nominal bit timing register */
        xKanx->CFG_NBTR = (((uint32_t)xkan_parameter_init->normal_time.NominalSyncJumpWidth) << 24)  | \
                                (((uint32_t)xkan_parameter_init->normal_time.NominalTimeSeg2 - 1) << 17) | \
                                (((uint32_t)xkan_parameter_init->normal_time.NominalTimeSeg1 - 1) << 9)  | \
                                ((uint32_t)xkan_parameter_init->normal_time.NominalPrescaler - 1);
    } else {
        return ERROR;
    }

    /* If FD operation with BRS is selected, set the data bit timing register */
    if(XKAN_FRAME_FD_BRS == xkan_parameter_init->frame_format) {
        /* Check the value of the data_time */
        if(IS_XKAN_GREATER(xkan_parameter_init->data_time.DataTimeSeg2, xkan_parameter_init->data_time.DataSyncJumpWidth)) {
            xKanx->CFG_DBTR = (((uint32_t)xkan_parameter_init->data_time.DataSyncJumpWidth) << 14  | \
                                (((uint32_t)xkan_parameter_init->data_time.DataTimeSeg2 - 1) << 10) | \
                                (((uint32_t)xkan_parameter_init->data_time.DataTimeSeg1 - 1) << 5)  | \
                                ((uint32_t)xkan_parameter_init->data_time.DataPrescaler - 1));
        } else {
            return ERROR;
        }
    }

    /* Configure Tx element size */
    if(xkan_parameter_init->ram_info.TxBuffersNbr + xkan_parameter_init->ram_info.TxFifoElmtsNbr > 0) {
        xKanx->CFG_TX_DSIZE = CvtEltSize[xkan_parameter_init->ram_info.TxElmtSize];
    }

    /* Configure Rx FIFO 0 element size */
    if(xkan_parameter_init->ram_info.RxFifo0ElmtsNbr > 0) {
        xKanx->CFG_RX_DSIZE |= CvtEltSize[xkan_parameter_init->ram_info.RxFifo0ElmtSize] << XKAN_CFG_RX_DSIZE_F0_DSIZE_OFS;
    }

    /* Configure Rx FIFO 1 element size */
    if(xkan_parameter_init->ram_info.RxFifo1ElmtsNbr > 0) {
        xKanx->CFG_RX_DSIZE |= CvtEltSize[xkan_parameter_init->ram_info.RxFifo1ElmtSize] << XKAN_CFG_RX_DSIZE_F1_DSIZE_OFS;
    }

    /* Configure Rx buffer element size */
    if(xkan_parameter_init->ram_info.RxBuffersNbr > 0) {
        xKanx->CFG_RX_DSIZE |= CvtEltSize[xkan_parameter_init->ram_info.RxBufferSize] << XKAN_CFG_RX_DSIZE_RB_DSIZE_OFS;
    }

    /* Calculate each RAM block address */
    xKan_CalcultateRamBlockAddresses(xKanx, xkan_parameter_init);

    /* enable xkan */
    xKan_EnableKernel(xKanx, ENABLE);

    /* Return function status */
    return SUCCESS;
}

/**
  * \brief  Abort transmission request
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  BufferIndex buffer index.
  *                      This parameter can be any combination of \arg XKAN_Tx_location.
  * \retval none
  */
ErrStatus xKan_AbortTxRequest(XKAN_TypeDef *xKanx, uint32_t BufferIndex)
{
    uint32_t tx_abort_req = ((xKanx->CFG_TXBAR) & (~xKanx->CFG_TXREQ_PENDING) & BufferIndex);

    if (tx_abort_req) {
        xKanx->CFG_TXABORT_REQ = BufferIndex;
    } else {
        return ERROR;
    }

    return SUCCESS;
}

/**
  * \brief  stop can icb & ker clk
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  ena 
  *     \arg ENABLE
  *     \arg DISABLE
  * \retval err status
  */
ErrStatus xKan_ClkStop(XKAN_TypeDef *xKanx, ControlStatus ena)
{
    uint32_t timeout = XKAN_TIMEOUT;
    if (ENABLE == ena) {
        xKanx->CFG_MCR |= XKAN_CFG_MCR_CLK_STOP_REQ;
        while (timeout--) {
            if(XKAN_CFG_MCR_CLK_STOP_ACK == (xKanx->CFG_MCR & XKAN_CFG_MCR_CLK_STOP_ACK)) {
                return SUCCESS;
            }
        }
        return ERROR;
    } else {
        xKanx->CFG_MCR &= (~XKAN_CFG_MCR_CLK_STOP_REQ);
    }
    return SUCCESS;
}

/**
  * \brief  Configure ISO 11898-1 protocol mode or not.
  *         CAN FD frame format is according to ISO 11898-1 standard.
  * \param  xKanx pointer to an FDCAN_HandleTypeDef structure that contains
  *         the configuration information for the specified FDCAN.
  * \param  ena 
  *     \arg ENABLE
  *     \arg DISABLE
  * \retval None
  */
void xKan_ConfigISOMode(XKAN_TypeDef *xKanx, ControlStatus ena)
{
    if (ENABLE == ena) {
        xKanx->CFG_ISO_MODE |= XKAN_CFG_ISO_MODE_ENABLE;
    } else {
        xKanx->CFG_ISO_MODE &= (~XKAN_CFG_ISO_MODE_ENABLE);
    }
}

/**
  * \brief  Enable the xKan kernel.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  ena
  *     \arg ENABLE
  *     \arg DISABLE
  * \retval none
  */
void xKan_EnableKernel(XKAN_TypeDef *xKanx, ControlStatus ena)
{
    if (ENABLE == ena) {
        xKanx->CFG_MCR |= XKAN_CFG_MCR_ENABLE;
    } else {
        xKanx->CFG_MCR &= (~XKAN_CFG_MCR_ENABLE);
    }
}

/**
  * \brief  Init the xKan module.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \retval err status
  */
ErrStatus xKan_EnterInit(XKAN_TypeDef *xKanx)
{
    uint32_t timeout = XKAN_TIMEOUT;
    /* enter initialisation mode */
    xKanx->CFG_MCR |= XKAN_CFG_MCR_INITIAL_REQ;
    /* init mode check */
    while(!xKan_GetMsrStatus(xKanx, XKAN_CFG_MSR_INITIAL_ACK)) {
        if(!timeout --) {
            return ERROR;
        }
    }

    return SUCCESS;
}

/**
  * \brief  Start the xKan module.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \retval err status
  */
ErrStatus xKan_Start(XKAN_TypeDef *xKanx)
{
    uint32_t timeout = XKAN_TIMEOUT;
    /* exit initialisation mode */
    xKanx->CFG_MCR &= (~XKAN_CFG_MCR_INITIAL_REQ);

    /* normal mode check */
    while(xKan_GetMsrStatus(xKanx, XKAN_CFG_MSR_INITIAL_ACK)) {
        if(!timeout --) {
            return ERROR;
        }
    }

    timeout = XKAN_TIMEOUT;
    while(!xKan_GetMsrStatus(xKanx, XKAN_CFG_MSR_NORMAL_ACK)) {
        if(!timeout --) {
            return ERROR;
        }
    }

    return SUCCESS;
}

/**
  * \brief  Configure xKan sleep function.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  ena
  *     \arg ENABLE
  *     \arg DISABLE
  * \retval err status
  */
ErrStatus xKan_Sleep(XKAN_TypeDef *xKanx, ControlStatus ena)
{
    uint32_t timeout = XKAN_TIMEOUT;

    if(ENABLE == ena){
        xKanx->CFG_MCR |= XKAN_CFG_MCR_SLEEP_REQ;
        /* check sleep state */
        while(!xKan_GetMsrStatus(xKanx, XKAN_CFG_MSR_SLEEP_ACK)){
            if(!timeout --) {
                return ERROR;
            }
        }
    } else {
        xKanx->CFG_MCR &= (~XKAN_CFG_MCR_SLEEP_REQ);
    }

    return SUCCESS;
}

/**
  * \brief XKAN get XKAN_MSR status
  * \param xKanx where x can be to select the XKAN peripheral.
  * \param status xkan CFG_MSR status bit
  * \return FlagStatus
  */
FlagStatus xKan_GetMsrStatus(XKAN_TypeDef *xKanx, uint32_t status)
{
    if (status == ((xKanx->CFG_MSR) & status)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief XKAN get current timestamp
  * \param xKanx where x can be to select the XKAN peripheral.
  * \return Current timestamp
  */
uint16_t xKan_GetTimeStamp(XKAN_TypeDef *xKanx)
{
    uint16_t timestamp;
    timestamp = xKanx->CFG_TIME_STAMP;
    return timestamp;
}

/**
  * \brief XKAN clear transmission complete
  * \param xKanx where x can be to select the XKAN peripheral.
  * \return None
  */
void xKan_ClearTxComplete(XKAN_TypeDef *xKanx)
{
    uint32_t tmpreg = xKanx->CFG_TXB_TRANS;
    xKanx->CFG_TXB_TRANS = XKAN_CFG_TXB_TRANS_TX_COMPLETE(tmpreg);
}

/**
  * \brief XKAN clear tx req pending
  * \param xKanx where x can be to select the XKAN peripheral.
  * \param  BufferIndex buffer index.
  * \return None
  */
void xKan_ClearTxReqPending(XKAN_TypeDef *xKanx, uint32_t BufferIndex)
{
    xKanx->CFG_TXREQ_PENDING = BufferIndex;
}

/**
  * \brief XKAN clear error code
  * \param xKanx where x can be to select the XKAN peripheral.
  * \return None
  */
void xKan_ClearErrCode(XKAN_TypeDef *xKanx)
{
    xKanx->CFG_ESR |= XKAN_CFG_ESR_LEC_MASK;
}

/**
  * \brief  Configure the xKan interrupt enable.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  interrupt
  *     \arg XKAN_CFG_INT_STATUS_RXB_UPD_INT
  *     \arg XKAN_CFG_INT_EN_RXF0_WM_EN
  *     \arg XKAN_CFG_INT_EN_RXF0_FILL_EN
  *     \arg XKAN_CFG_INT_EN_RXF0_LOST_EN
  *     \arg XKAN_CFG_INT_EN_RXF0_FULL_EN
  *     \arg XKAN_CFG_INT_EN_RXF1_WM_EN 
  *     \arg XKAN_CFG_INT_EN_RXF1_FILL_EN 
  *     \arg XKAN_CFG_INT_EN_RXF1_FULL_EN
  *     \arg XKAN_CFG_INT_EN_RXF1_LOST_EN
  *     \arg XKAN_CFG_INT_EN_RXFF_OF_EN
  *     \arg XKAN_CFG_INT_EN_TXFF_UF_EN
  * \param  status
  *     \arg ENABLE
  *     \arg DISABLE
  * \retval none
  */
void xKan_EnableInterrupt(XKAN_TypeDef *xKanx, uint32_t interrupt, ControlStatus status)
{
    if (ENABLE == status) {
        xKanx->CFG_INT_EN |= interrupt;
    } else {
        xKanx->CFG_INT_EN &= ~(interrupt);
    }
}

/**
  * \brief  Check whether the specified XKAN flag is set or not.
  * \param  xKanx XKAN handle.
  * \param  interrupt XKAN flag.
  *          This parameter can be one of \arg XKAN_flags
  * \retval int en status
  */
FlagStatus xKan_GetInterruptEnable(XKAN_TypeDef *xKanx, uint32_t interrupt)
{
    if (interrupt == ((xKanx->CFG_INT_EN) & interrupt)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief  Check whether the specified XKAN flag is set or not.
  * \param  xKanx XKAN handle.
  * \param  status XKAN flag.
  *          This parameter can be one of \arg XKAN_flags
  * \retval int stasus
  */
FlagStatus xKan_GetInterruptStatus(XKAN_TypeDef *xKanx, uint32_t status)
{
    if (status == ((xKanx->CFG_INT_STATUS) & status)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * \brief  Clear the specified XKAN flag.
  * \param  xKanx XKAN handle.
  * \param  status XKAN flag.
  *          This parameter can be one of \arg XKAN_flags
  * \retval int stasus
  */
void xKan_ClearInterruptStatus(XKAN_TypeDef *xKanx, uint32_t status)
{
    xKanx->CFG_INT_STATUS = status;
}

/**
  * \brief  Add a message to a dedicated Tx buffer
  * \param  xkan_parameter_init pointer to an xKan_InitTypeDef structure that contains
  *         the configuration information for the specified xKan. 
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  pTxHeader pointer to a XKAN_TxHeaderTypeDef structure.
  * \param  pTxData pointer to a buffer containing the payload of the Tx frame.
  * \param  BufferIndex index of the buffer to be configured.
  *                      This parameter can be a value of \arg XKAN_Tx_location.
  * \param  xkanRamAddr pointer to xKan RAM base address.
  * \retval err status
  */
ErrStatus XKAN_AddMessageToTxBuffer(xKan_InitTypeDef *xkan_parameter_init, XKAN_TypeDef *xKanx, xKan_TxHeaderTypeDef *pTxHeader, uint8_t *pTxData, uint32_t BufferIndex, ramtype_t xkanRamAddr)
{
    /* Check that there is no transmittion request pending for the selected buffer */
    if((xKanx->CFG_TXREQ_PENDING & BufferIndex) != 0 || (xKanx->CFG_TXBAR & BufferIndex) != 0) {
        return ERROR;
    } else {
        /* Add the message to the Tx buffer */
        xKanx_CopyMessageToRAM(xkan_parameter_init, pTxHeader, pTxData, __HBIT_POSITION(BufferIndex), xkanRamAddr);
    }

    /* Return function status */
    return SUCCESS;
}

/**
  * \brief  Add a message to the Tx FIFO and activate the corresponding transmission request
  * \param  xkan_parameter_init pointer to an xKan_InitTypeDef structure that contains
  *         the configuration information for the specified xKan. 
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  pTxHeader pointer to a xKan_TxHeaderTypeDef structure.
  * \param  pTxData pointer to a buffer containing the payload of the Tx frame.
  * \param  xkanRamAddr pointer to xKan RAM base address.
  * \retval err status
  */
ErrStatus xKan_AddMessageToTxFifo(xKan_InitTypeDef *xkan_parameter_init, XKAN_TypeDef *xKanx, xKan_TxHeaderTypeDef *pTxHeader, uint8_t* pTxData, ramtype_t xkanRamAddr)
{
    uint32_t PutIndex;

    /* Check that the Tx FIFO has an allocated area into the RAM */
    if((xKanx->CFG_TXBUF & XKAN_CFG_TXBUF_TXFIFO_NUM_MASK) == 0) {
        return ERROR;
    }

    /* Check that the Tx FIFO is not full */
    if(xKanx->CFG_TXFIFO_STATUS & XKAN_CFG_TXFIFO_STATUS_TXFF_FULL) {
        return ERROR;
    } else {
        /* Retrieve the Tx FIFO PutIndex */
        PutIndex = ((xKanx->CFG_TXFIFO_STATUS & XKAN_CFG_TXFIFO_STATUS_TXFF_WPTR) >> 16) + xkan_parameter_init->ram_info.TxBuffersNbr;
        /* Add the message to the Tx FIFO/Queue */
        xKanx_CopyMessageToRAM(xkan_parameter_init, pTxHeader, pTxData, PutIndex, xkanRamAddr);
        /* Activate the corresponding transmission request */
        xKanx->CFG_TXBAR |= (1 << PutIndex);
    }

    return SUCCESS;
}

/**
  * \brief  Configure the xKan reception filter according to the specified
  *         parameters in the xKan_FilterTypeDef structure.
  * \param  xkan_parameter_init pointer to an xKan_InitTypeDef structure that contains
  *         the configuration information for the specified xKan.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  sFilterConfig pointer to an xKan_FilterTypeDef structure that
  *         contains the filter configuration information
  * \param  xkanRamAddr pointer to xKan RAM base address.
  * \retval err status
  */
ErrStatus xKan_ConfigFilter(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init, xKan_FilterTypeDef* sFilterConfig, ramtype_t xkanRamAddr)
{
    uint32_t FilterElementW1;
    uint32_t FilterElementW2;
    uint32_t *FilterAddress;

    if(sFilterConfig->IdType == XKAN_STANDARD_ID) {
        /* Build filter element */
        if(sFilterConfig->FilterConfig == XKAN_FILTER_TO_RXBUFFER) {
            FilterElementW1 = ((sFilterConfig->FilterType << 30)   | \
                            (sFilterConfig->FilterConfig << 28)    | \
                            (sFilterConfig->FilterID1 << 16)       | \
                            sFilterConfig->RxBufferIndex);
            xKan_ConfigStandardIdMask(xKanx, sFilterConfig->FilterID2);
        } else {
            FilterElementW1 = ((sFilterConfig->FilterType << 30)   | \
                            (sFilterConfig->FilterConfig << 28)    | \
                            (sFilterConfig->FilterID1 << 16)       | \
                            sFilterConfig->FilterID2);
        }

        /* Calculate filter address */
        FilterAddress = ADDR32P(xkan_parameter_init->ram_info.ram_addr.StandardFilterSA * 4 + (sFilterConfig->FilterIndex * 4) + xkanRamAddr);

        /* Write filter element to the message RAM */
        *FilterAddress = FilterElementW1;
    } else {/* sFilterConfig->IdType == xKan_EXTENDED_ID */
        /* Build first word of filter element */
        FilterElementW1 = ((sFilterConfig->FilterType << 30) | sFilterConfig->FilterID1);

        /* Build second word of filter element */
        if(sFilterConfig->FilterConfig == XKAN_FILTER_TO_RXBUFFER) {
            FilterElementW2 = ((sFilterConfig->FilterConfig << 30) | sFilterConfig->RxBufferIndex);
            xKan_ConfigExtendedIdMask(xKanx, sFilterConfig->FilterID2);
        } else {
            FilterElementW2 = ((sFilterConfig->FilterConfig << 30) | sFilterConfig->FilterID2);
        }

        /* Calculate filter address */
        FilterAddress = ADDR32P(xkan_parameter_init->ram_info.ram_addr.ExtendedFilterSA * 4 + (sFilterConfig->FilterIndex * 4 * 2) + xkanRamAddr);

        /* Write filter element to the message RAM */
        *FilterAddress++ = FilterElementW1;
        *FilterAddress = FilterElementW2;
    }

    /* Return function status */
    return SUCCESS;
}

/**
  * \brief  Configure the xKanx global filter.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  NonMatchingStd Defines how received messages with 11-bit IDs that
                            do not match any element of the filter list are treated.
                            This parameter can be a value of \arg xKanx_Non_Matching_Frames.
  * \param  NonMatchingExt Defines how received messages with 29-bit IDs that
                            do not match any element of the filter list are treated.
                            This parameter can be a value of \arg xKanx_Non_Matching_Frames.
  * \param  RejectRemoteStd Enable or disable the remote standard frames rejection.
                             This parameter can be set to ENABLE or DISABLE.
  * \param  RejectRemoteExt Enable or disable the remote extended frames rejection.
                             This parameter can be set to ENABLE or DISABLE.
  * \retval err status
  */
void xKan_ConfigGlobalFilter(XKAN_TypeDef *xKanx, uint32_t NonMatchingStd, uint32_t NonMatchingExt, uint32_t RejectRemoteStd, uint32_t RejectRemoteExt)
{
    /* Configure global filter */
    xKanx->CFG_GLOBAL_FILTER = ((NonMatchingStd << 4U) | (NonMatchingExt << 2U) | (RejectRemoteStd << 1U) | RejectRemoteExt);
}

/**
  * \brief  Enable transmission request.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  BufferIndex buffer index.
  *                      This parameter can be any combination of \arg XKAN_Tx_location.
  * \retval none
  */
void xKan_EnableTxBufferRequest(XKAN_TypeDef *xKanx, uint32_t BufferIndex)
{
    /* Add transmission request */
    xKanx->CFG_TXBAR = BufferIndex;
}

/**
  * \brief  Get an XKAN frame from the Rx Buffer/FIFO zone into the message RAM.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  xkan_parameter_init pointer to an xKan_InitTypeDef structure that contains
  *         the configuration information for the specified XKAN.
  * \param  RxLocation Location of the received message to be read.
                        This parameter can be a value of \arg XKAN_Rx_location.
  * \param  pRxHeader pointer to a xKan_RxHeaderTypeDef structure.
  * \param  pRxData pointer to a buffer where the payload of the Rx frame will be stored.
  * \param  xkanRamAddr pointer to xkan ram address.
  * \retval err status
  */
ErrStatus xKan_GetRxMessage(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init, uint32_t RxLocation, xKan_RxHeaderTypeDef *pRxHeader, uint8_t *pRxData, ramtype_t xkanRamAddr)
{
    uint32_t *RxAddress;
    uint8_t  *pData;
    uint32_t ByteCounter;
    uint32_t GetIndex = 0;

    uint32_t left;
    uint32_t ptr = 0;

    if(RxLocation == XKAN_RX_FIFO0) {/* Rx element is assigned to the Rx FIFO 0 */
        /* Check that the Rx FIFO 0 has an allocated area into the RAM */
        if((xKanx->CFG_RXFIFO0 & XKAN_CFG_RXFIFO0_RXFF0_NUM_MASK) == 0) {
            return ERROR;
        }
        /* Check that the Rx FIFO 0 is not empty */
        if((xKanx->CFG_RXFIFO0_STATUS & XKAN_CFG_RXFIFO0_STATUS_RXFF0_ENTRY_NUM) == 0){
            return ERROR;
        } else {
            /* Calculate Rx FIFO 0 element address */
            GetIndex = ((xKanx->CFG_RXFIFO0_STATUS & XKAN_CFG_RXFIFO0_STATUS_RXFF0_RPTR) >> 8);
            RxAddress = ADDR32P(xkan_parameter_init->ram_info.ram_addr.RxFIFO0SA * 4 + (GetIndex * xkan_parameter_init->ram_info.RxFifo0ElmtSize * 4 + xkanRamAddr));
        }
    } else if (RxLocation == XKAN_RX_FIFO1) {/* Rx element is assigned to the Rx FIFO 1 */
        /* Check that the Rx FIFO 1 has an allocated area into the RAM */
        if((xKanx->CFG_RXFIFO1 & XKAN_CFG_RXFIFO1_RXFF1_NUM_MASK) == 0) {
            return ERROR;
        }

        /* Check that the Rx FIFO 0 is not empty */
        if((xKanx->CFG_RXFIFO1_STATUS & XKAN_CFG_RXFIFO1_STATUS_RXFF1_ENTRY_NUM) == 0) {
            return ERROR;
        } else {
            /* Calculate Rx FIFO 1 element address */
            GetIndex = ((xKanx->CFG_RXFIFO1_STATUS & XKAN_CFG_RXFIFO1_STATUS_RXFF1_RPTR) >> 8);
            RxAddress = ADDR32P(xkan_parameter_init->ram_info.ram_addr.RxFIFO1SA * 4 + (GetIndex * xkan_parameter_init->ram_info.RxFifo1ElmtSize * 4 + xkanRamAddr));
        }
    } else {/* Rx element is assigned to a dedicated Rx buffer */
        /* Check that the selected buffer has an allocated area into the RAM */
        if(RxLocation >= xkan_parameter_init->ram_info.RxBuffersNbr) {
            return ERROR;
        } else {
        /* Calculate Rx buffer address */
            RxAddress = ADDR32P(xkan_parameter_init->ram_info.ram_addr.RxBufferSA * 4 + (RxLocation * xkan_parameter_init->ram_info.RxBufferSize * 4 + xkanRamAddr));
        }
    }

    /* Retrieve RxFrameType */
    pRxHeader->RxFrameType = (*RxAddress & XKAN_ELEMENT_MASK_RTR);

    /* Retrieve IdType */
    pRxHeader->IdType = *RxAddress & XKAN_ELEMENT_MASK_XTD;

    /* Retrieve Identifier */
    if(pRxHeader->IdType == XKAN_STANDARD_ID) {/* Standard ID element */
        pRxHeader->Identifier = ((*RxAddress & XKAN_ELEMENT_MASK_STDID) >> 18);
    } else {/* Extended ID element */
        pRxHeader->Identifier = (*RxAddress & XKAN_ELEMENT_MASK_EXTID);
    }

    /* Next rx addr point */
    RxAddress += 1;

    /* Retrieve NonMatchingFrame */
    pRxHeader->IsFilterMatchingFrame = ((*RxAddress & XKAN_ELEMENT_MASK_ANMF) >> 31);

    /* Retrieve FilterIndex */
    if (pRxHeader->FDFormat & XKAN_ELEMENT_MASK_FDF ) { /* Receive frame is extend, filter_id is last addr(hardware defined) */
        pRxHeader->FilterIndex = ((*RxAddress & XKAN_ELEMENT_MASK_FIDX) >> 24) / 2;
    } else {
        pRxHeader->FilterIndex = ((*RxAddress & XKAN_ELEMENT_MASK_FIDX) >> 24);
    }

    /* Retrieve ErrorStateIndicator */
    pRxHeader->ErrorStateIndicator = (*RxAddress & XKAN_ELEMENT_MASK_ESI);

    /* Retrieve FDFormat */
    pRxHeader->FDFormat = (*RxAddress & XKAN_ELEMENT_MASK_FDF);

    /* Retrieve BitRateSwitch */
    pRxHeader->BitRateSwitch = (*RxAddress & XKAN_ELEMENT_MASK_BRS);

    /* Retrieve DataLength */
    pRxHeader->DataLength = (*RxAddress & XKAN_ELEMENT_MASK_DLC);

    /* Retrieve RxTimestamp */
    pRxHeader->RxTimestamp = (*RxAddress & XKAN_ELEMENT_MASK_TS);

    /* Next rx addr point */
    RxAddress += 1;

    /* Retrieve Rx payload */
    pData = (uint8_t *)RxAddress;
    left = DLCtoBytes[pRxHeader->DataLength >> 16];

    /* Read according to the amount of remaining data */
    while (left >= 0x4) {
        *(pRxData + ptr) = *(pData + ptr);
        *(pRxData + 1 + ptr) = *(pData + 1 + ptr);
        *(pRxData + 2 + ptr) = *(pData + 2 + ptr);
        *(pRxData + 3 + ptr) = *(pData + 3 + ptr);

        ptr  = ptr  + 4;
        left = left - 4;
    }
    while (left >= 0x3) {
        *(pRxData + ptr) = *(pData + ptr);
        *(pRxData + 1 + ptr) = *(pData + 1 + ptr);
        *(pRxData + 2 + ptr) = *(pData + 2 + ptr);

        ptr  = ptr  + 3;
        left = left - 3;
    }
    while (left >= 0x2) {
        *(pRxData + ptr) = *(pData + ptr);
        *(pRxData + 1 + ptr) = *(pData + 1 + ptr);

        ptr  = ptr  + 2;
        left = left - 2;
    }
    while (left >= 0x1) {
        *(pRxData + ptr) = *(pData + ptr);

        ptr  = ptr  + 1 ;
        left = left - 1 ;
    }

    /* Rx element is assigned to the Rx FIFO 0 */
    if(RxLocation == XKAN_RX_FIFO0) {
        /* Acknowledge the Rx FIFO 0 that the oldest element is read so that it increments the GetIndex */
        xKanx->CFG_RXFIFO0_ACK = XKAN_CFG_RXFIFO0_ACK_RXF0_ACK;
    }
    else if(RxLocation == XKAN_RX_FIFO1) {/* Rx element is assigned to the Rx FIFO 1 */
        /* Acknowledge the Rx FIFO 1 that the oldest element is read so that it increments the GetIndex */
        xKanx->CFG_RXFIFO1_ACK = XKAN_CFG_RXFIFO1_ACK_RXF1_ACK;
    } else {/* Rx element is assigned to a dedicated Rx buffer */
        /* Clear the New Data flag of the current Rx buffer */
        if(RxLocation < XKAN_RX_BUFFER32) {
            xKanx->CFG_RXDATL_RSP = (1 << RxLocation);
        } else {/* XKAN_RX_BUFFER32 <= RxLocation <= XKAN_RX_BUFFER63 */
        
            xKanx->CFG_RXDATH_RSP = (1 << (RxLocation - XKAN_RX_LOW_RSP_SIZE));
        }
    }

    /* Return function status */
    return SUCCESS;
}

/**
  * \brief  RxFifo0 callback.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  xkan_parameter_init pointer to an xKan_InitTypeDef structure that contains
  *         the configuration information for the specified XKAN.
  * \param  xKan_RxHeader pointer to a xKan_RxHeaderTypeDef structure.
  * \param  pRxData pointer to a buffer where the payload of the Rx frame will be stored.
  * \param  xkanRamAddr pointer to xkan ram address.
  * \retval none
  */
void xKan_RxFifo0Callback(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init, xKan_RxHeaderTypeDef *xKan_RxHeader, uint8_t *pRxData, ramtype_t xkanRamAddr)
{
    xKan_GetRxMessage(xKanx, xkan_parameter_init, XKAN_RX_FIFO0, xKan_RxHeader, pRxData, xkanRamAddr);
}

/**
  * \brief  RxFifo1 callback.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  xkan_parameter_init pointer to an xKan_InitTypeDef structure that contains
  *         the configuration information for the specified XKAN.
  * \param  xKan_RxHeader pointer to a xKan_RxHeaderTypeDef structure.
  * \param  pRxData pointer to a buffer where the payload of the Rx frame will be stored.
  * \param  xkanRamAddr pointer to xkan ram address.
  * \retval none
  */
void xKan_RxFifo1Callback(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init, xKan_RxHeaderTypeDef *xKan_RxHeader, uint8_t *pRxData, ramtype_t xkanRamAddr)
{
    xKan_GetRxMessage(xKanx, xkan_parameter_init, XKAN_RX_FIFO1, xKan_RxHeader, pRxData, xkanRamAddr);
}

/**
  * \brief  Rx Buffer New Message callback.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  xkan_parameter_init pointer to an xKan_InitTypeDef structure that contains
  *         the configuration information for the specified XKAN.
  * \param  xKan_RxHeader pointer to a xKan_RxHeaderTypeDef structure.
  * \param  pRxData pointer to a buffer where the payload of the Rx frame will be stored.
  * \param  xkanRamAddr pointer to xkan ram address.
  * \retval none
  */
void xKan_RxBufferNewMessageCallback(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init, xKan_RxHeaderTypeDef *xKan_RxHeader, uint8_t *pRxData, ramtype_t xkanRamAddr)
{
    uint8_t i, off;
    uint8_t arrsize = 32;
    ErrStatus state;

    for(i = 0; i < arrsize; i++) {
        off = (xKanx->CFG_RXDATL_RSP & (1 << i)) >> i;
        if(off == 1) {
            /* get data from rx_buffer, i is equal to XKAN_RX_BUFFER */
            state = xKan_GetRxMessage(xKanx, xkan_parameter_init, i, xKan_RxHeader, pRxData, xkanRamAddr);
            goto over;
        }
    }

    for(i = 0; i < arrsize; i++) {
        off = (xKanx->CFG_RXDATH_RSP & (1 << i)) >> i;
        if(off == 1) {
            /* get data from rx_buffer, i is equal to XKAN_RX_BUFFER */
            state = xKan_GetRxMessage(xKanx, xkan_parameter_init, i + XKAN_RX_LOW_RSP_SIZE, xKan_RxHeader, pRxData, xkanRamAddr);
            goto over;
        }
    }
over:
    return;
}

/**
  * \brief  Configure the FIFO watermark.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  FIFO select the FIFO to be configured.
  * \param  watermark level for FIFO watermark interrupt.
  *                    This parameter must be a number between
  *                     - 0 and 64, if FIFO is XKAN_CFG_RX_FIFO0 or XKAN_CFG_RX_FIFO1
  * \retval none
  */
void xKan_ConfigRxFifoWatermark(XKAN_TypeDef *xKanx, uint32_t FIFO, uint32_t watermark)
{

    assert_param(IS_XKAN_MAX_VALUE(Watermark, 64));

    /* Set the level for FIFO watermark interrupt */
    if(FIFO == XKAN_CFG_RX_FIFO0) {  /* FIFO == XKAN_CFG_RX_FIFO0 */
        xKanx->CFG_RXFIFO0 |= XKAN_CFG_RXFIFO0_RXFF0_WM(watermark);
    } else {  /* FIFO == XKAN_CFG_RX_FIFO1 */
        xKanx->CFG_RXFIFO1 |= XKAN_CFG_RXFIFO1_RXFF1_WM(watermark);
    }
}

/**
  * \brief  Configure the FIFO operation mode.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  FIFO select the FIFO to be configured.
  * \param  operation_mode level for FIFO watermark interrupt.
  *     \arg XKAN_RXFIFO_OP_BLOCK
  *     \arg XKAN_RXFIFO_OP_OVERWRITE
  * \retval none
  */
void xKan_ConfigRxFifoOperationMode(XKAN_TypeDef *xKanx, uint32_t FIFO, uint32_t operation_mode)
{
    /* Set the operation mode for FIFO */
    if(FIFO == XKAN_CFG_RX_FIFO0) {  /* FIFO == XKAN_CFG_RX_FIFO0 */
        xKanx->CFG_RXFIFO0 &= (~XKAN_CFG_RXFIFO0_RXFF0_OPM);
        xKanx->CFG_RXFIFO0 |= operation_mode;
    } else {  /* FIFO == XKAN_CFG_RX_FIFO1 */
        xKanx->CFG_RXFIFO1 &= (~XKAN_CFG_RXFIFO1_RXFF1_OPM);
        xKanx->CFG_RXFIFO1 |= operation_mode;
    }
}

/**
  * \brief  Configure the transmitter delay compensation offset value.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  offset Offset value defining the distance between the measured delay from XKAN_TX to XKAN_RX and the secondary sample point.
  * \retval none
  */
void xKan_ConfigTdcOffset(XKAN_TypeDef *xKanx, uint8_t offset)
{
    assert_param(IS_XKAN_MAX_VALUE(offset, 0x7F));

    /* Set the transmitter delay compensation offset value */
    xKanx->CFG_TRANS_DELAY_COMP = XKAN_CFG_TRANS_DELAY_COMP_OFFSET_VALUE(offset);
}

/* -------------------------------------------------- private functions -------------------------------------------------- */

/**
  * \brief  Configure the standard ID mask.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  Mask standard ID Mask.
                  This parameter must be a number between 0 and 0x1FFFFFFF
  * \retval none
  */
static void xKan_ConfigStandardIdMask(XKAN_TypeDef *xKanx, uint32_t Mask)
{
    /* Check function parameters */
    assert_param(IS_XKAN_MAX_VALUE(Mask, 0x3FF));
    /* Configure the standard ID mask */
    xKanx->CFG_STIDM = Mask;
}

/**
  * \brief  Configure the extended ID mask.
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  Mask Extended ID Mask.
                  This parameter must be a number between 0 and 0x1FFFFFFF
  * \retval none
  */
static void xKan_ConfigExtendedIdMask(XKAN_TypeDef *xKanx, uint32_t Mask)
{
    /* Check function parameters */
    assert_param(IS_XKAN_MAX_VALUE(Mask, 0x1FFFFFFF));
    /* Configure the extended ID mask */
    xKanx->CFG_EXIDM = Mask;
}

/**
  * \brief  Calculate each RAM block start address and size
  * \param  xKanx where x can be to select the XKAN peripheral.
  * \param  xkan_parameter_init pointer to an xKan_InitTypeDef structure that contains
  *         the configuration information for the specified XKAN.
  * \retval err status
 */
static ErrStatus xKan_CalcultateRamBlockAddresses(XKAN_TypeDef *xKanx, xKan_InitTypeDef *xkan_parameter_init)
{
    /* Standard filter list start address */
    xkan_parameter_init->ram_info.ram_addr.StandardFilterSA = xkan_parameter_init->ram_info.MessageRAMOffset;
    xKanx->CFG_ST_FILTER |= (xkan_parameter_init->ram_info.ram_addr.StandardFilterSA << XKAN_CFG_ST_FILTER_STFLT_ADDR_OFS) & XKAN_CFG_ST_FILTER_STFLT_ADDR_MASK;
    /* Standard filter elements number */
    xKanx->CFG_ST_FILTER |= (xkan_parameter_init->ram_info.StdFiltersNbr << XKAN_CFG_ST_FILTER_STFLT_NUM_OFS) & XKAN_CFG_ST_FILTER_STFLT_NUM_MASK;

    /* Extended filter list start address */
    xkan_parameter_init->ram_info.ram_addr.ExtendedFilterSA = xkan_parameter_init->ram_info.ram_addr.StandardFilterSA + xkan_parameter_init->ram_info.StdFiltersNbr;
    xKanx->CFG_EX_FILTER |= (xkan_parameter_init->ram_info.ram_addr.ExtendedFilterSA << XKAN_CFG_EX_FILTER_EXFLT_ADDR_OFS) & XKAN_CFG_EX_FILTER_EXFLT_ADDR_MASK;
    /* Extended filter elements number(unit: word) */
    xKanx->CFG_EX_FILTER |= ((xkan_parameter_init->ram_info.ExtFiltersNbr * 2) << XKAN_CFG_EX_FILTER_EXFLT_NUM_OFS) & XKAN_CFG_EX_FILTER_EXFLT_NUM_MASK;

    /* Rx FIFO 0 start address */
    xkan_parameter_init->ram_info.ram_addr.RxFIFO0SA = xkan_parameter_init->ram_info.ram_addr.ExtendedFilterSA + (xkan_parameter_init->ram_info.ExtFiltersNbr * 2);
    xKanx->CFG_RXFIFO0 |= (xkan_parameter_init->ram_info.ram_addr.RxFIFO0SA << XKAN_CFG_RXFIFO0_RXFF0_ADDR_OFS) & XKAN_CFG_RXFIFO0_RXFF0_ADDR_MASK;
    /* Rx FIFO 0 elements number */
    xKanx->CFG_RXFIFO0 |= (xkan_parameter_init->ram_info.RxFifo0ElmtsNbr << XKAN_CFG_RXFIFO0_RXFF0_NUM_OFS) & XKAN_CFG_RXFIFO0_RXFF0_NUM_MASK;

    /* Rx FIFO 1 start address */
    xkan_parameter_init->ram_info.ram_addr.RxFIFO1SA = xkan_parameter_init->ram_info.ram_addr.RxFIFO0SA + (xkan_parameter_init->ram_info.RxFifo0ElmtsNbr * xkan_parameter_init->ram_info.RxFifo0ElmtSize);
    xKanx->CFG_RXFIFO1 |= (xkan_parameter_init->ram_info.ram_addr.RxFIFO1SA << XKAN_CFG_RXFIFO1_RXFF1_ADDR_OFS) & XKAN_CFG_RXFIFO1_RXFF1_ADDR_MASK;
    /* Rx FIFO 1 elements number */
    xKanx->CFG_RXFIFO1 |= (xkan_parameter_init->ram_info.RxFifo1ElmtsNbr << XKAN_CFG_RXFIFO1_RXFF1_NUM_OFS) & XKAN_CFG_RXFIFO1_RXFF1_NUM_MASK;

    /* Rx buffer list start address */
    xkan_parameter_init->ram_info.ram_addr.RxBufferSA = xkan_parameter_init->ram_info.ram_addr.RxFIFO1SA + (xkan_parameter_init->ram_info.RxFifo1ElmtsNbr * xkan_parameter_init->ram_info.RxFifo1ElmtSize);
    xKanx->CFG_RXBUF |= (xkan_parameter_init->ram_info.ram_addr.RxBufferSA << XKAN_CFG_RXBUF_RXBUF_ADDR_OFS) & XKAN_CFG_RXBUF_RXBUF_ADDR_MASK;

    /* Tx buffer list start address */
    xkan_parameter_init->ram_info.ram_addr.TxBufferSA = xkan_parameter_init->ram_info.ram_addr.RxBufferSA + (xkan_parameter_init->ram_info.RxBuffersNbr * xkan_parameter_init->ram_info.RxBufferSize);
    xKanx->CFG_TXBUF |= (xkan_parameter_init->ram_info.ram_addr.TxBufferSA << XKAN_CFG_TXBUF_TXBUF_ADDR_OFS) & XKAN_CFG_TXBUF_TXBUF_ADDR_MASK;
    /* Dedicated Tx buffers number */
    xKanx->CFG_TXBUF |= (xkan_parameter_init->ram_info.TxBuffersNbr << XKAN_CFG_TXBUF_TXBUF_NUM_OFS) & XKAN_CFG_TXBUF_TXBUF_NUM_MASK;

    /* Tx FIFO start address */
    xkan_parameter_init->ram_info.ram_addr.TxFIFOSA = xkan_parameter_init->ram_info.ram_addr.TxBufferSA + (xkan_parameter_init->ram_info.TxBuffersNbr * xkan_parameter_init->ram_info.TxElmtSize);
    /* Tx FIFO elements number */
    xKanx->CFG_TXBUF |= (xkan_parameter_init->ram_info.TxFifoElmtsNbr << XKAN_CFG_TXBUF_TXFIFO_NUM_OFS) & XKAN_CFG_TXBUF_TXFIFO_NUM_MASK;

    xkan_parameter_init->ram_info.ram_addr.EndAddress = xkan_parameter_init->ram_info.ram_addr.TxFIFOSA + (xkan_parameter_init->ram_info.TxFifoElmtsNbr * xkan_parameter_init->ram_info.TxElmtSize * 4);

    /* if beyond xkan ram range */
    if(xkan_parameter_init->ram_info.ram_addr.EndAddress > xkan_parameter_init->ram_info.MessageRAMOffset + XKAN_RAM_SIZE) {/* Last address of the Message RAM */
        return ERROR;
    }

    /* Return function status */
    return SUCCESS;
}

 /**
  * \brief  Copy Tx message to the message RAM.
  * \param  xkan_parameter_init pointer to an xKan_InitTypeDef structure that contains
  *                 the configuration information for the specified XKAN.
  * \param  pTxHeader pointer to a XKAN_TxHeaderTypeDef structure.
  * \param  pTxData pointer to a buffer containing the payload of the Tx frame.
  * \param  BufferIndex index of the buffer to be configured.
  * \param  xkanRamAddr pointer to xkan ram address.
  * \retval err status
 */
static ErrStatus xKanx_CopyMessageToRAM(xKan_InitTypeDef *xkan_parameter_init, xKan_TxHeaderTypeDef *pTxHeader, uint8_t *pTxData, uint32_t BufferIndex, ramtype_t xkanRamAddr)
{
    uint32_t TxElementW1;
    uint32_t TxElementW2;
    uint32_t *TxAddress;
    uint32_t ByteCounter;
    
    /* Send remote frame only in classic mode */
    if((pTxHeader->FDFormat != XKAN_CLASSIC_CAN) && (pTxHeader->TxFrameType == XKAN_REMOTE_FRAME)) {
        return ERROR;
    }

    /* Build first word of Tx header element */
    if(pTxHeader->IdType == XKAN_STANDARD_ID) {
        TxElementW1 = (pTxHeader->FDFormat | \
                    pTxHeader->IdType | \
                    pTxHeader->TxFrameType | \
                    (pTxHeader->Identifier << 18));
    } else {/* pTxHeader->IdType == XKAN_EXTENDED_ID */
        TxElementW1 = (pTxHeader->FDFormat | \
                    pTxHeader->IdType | \
                    pTxHeader->TxFrameType | \
                    pTxHeader->Identifier);
    }

    /* Build second word of Tx header element */
    TxElementW2 = (pTxHeader->ErrorStateIndicator  | \
                    pTxHeader->BitRateSwitch | \
                    pTxHeader->DataLength);

    /* Calculate Tx element address */
    TxAddress = ADDR32P(xkan_parameter_init->ram_info.ram_addr.TxBufferSA * 4 + (BufferIndex * xkan_parameter_init->ram_info.TxElmtSize * 4) + xkanRamAddr);

    /* Write Tx element header to the message RAM */
    *TxAddress++ = TxElementW1;
    *TxAddress++ = TxElementW2;

    /* Write Tx payload to the message RAM */
    for(ByteCounter = 0; ByteCounter < DLCtoBytes[pTxHeader->DataLength >> 16]; ByteCounter += 4) {
        *TxAddress++ = ((pTxData[ByteCounter+3] << 24) | \
                        (pTxData[ByteCounter+2] << 16) | \
                        (pTxData[ByteCounter+1] << 8) | \
                        pTxData[ByteCounter]);
    }

    /* Return function status */
    return SUCCESS;
}

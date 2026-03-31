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

/* Includes ------------------------------------------------------------------*/
#include "ns.h"
#include "ns_qspi_xip.h"
#include "ns_conf.h"

/**
 * @file     ns_qspi_xip.c
 * @brief    This file contains all the functions prototypes for the QSPI_XIP firmware
  */

/* QSPI_XIP registers Masks */
#define QSPI_XIP_CR_CLEAR_MASK                       (BITS(0,6))
#define QSPI_XIP_CSMODE_CLEAR_MASK                   (BITS(0,1))
#define QSPI_XIP_FMT_CLEAR_MASK                      (BITS(0,21))
#define QSPI_XIP_FORCE_CLEAR_MASK                    (BITS(0,4))
/* QSPI_XIP_TX_MARK */
#define QSPI_XIP_TX_MASK                             BITS(0,2)
/* QSPI_XIP_RX_MARK */
#define QSPI_XIP_RX_MASK                             BITS(0,2)
/* QSPI_XIP_DELAY0 Mask*/
#define QSPI_XIP_DELAY0_CSSCK_MASK                   BITS(0,7)
#define QSPI_XIP_DELAY0_SCKCS_MASK                   BITS(16,23)
/* QSPI_XIP_DELAY1 Mask*/
#define QSPI_XIP_DELAY1_INTERCS_MASK                 BITS(0,7)
#define QSPI_XIP_DELAY1_INTERXFR_MASK                BITS(16,23)
/* QSPI_XIP rxedge Mask*/
#define QSPI_XIP_RXEDGE_MASK                    BIT(0)

/**
  * \brief      Initialize QSPI_XIP parameter.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  QSPI_XIP_Struct QSPI_XIP parameter initialization stuct members of the structure.
  */
void QSPI_XIP_Init(QSPI_XIP_TypeDef* QSPI_XIPx, QSPI_XIP_InitTypeDef* QSPI_XIP_Struct)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_SPI_SCKDIV_PRESCALER(QSPI_XIP_Struct->SCKDIV));
    assert_param(IS_QSPI_XIP_SPI_SCKMODE_CPOL(QSPI_XIP_Struct->CPOL));
    assert_param(IS_QSPI_XIP_SPI_SCKMODE_CPHA(QSPI_XIP_Struct->CPHA));
    assert_param(IS_QSPI_XIP_SPI_CSMODE_MODE(QSPI_XIP_Struct->CSMode));
    assert_param(IS_QSPI_XIP_SPI_FMT_ENDIAN(QSPI_XIP_Struct->Endian));
    assert_param(IS_QSPI_XIP_SPI_FMT_LEN(QSPI_XIP_Struct->DataSize));
    assert_param(IS_QSPI_XIP_SPI_FMT_PROTO(QSPI_XIP_Struct->ProtolMode));
    assert_param(IS_QSPI_XIP_SPI_CR_MODE(QSPI_XIP_Struct->DevMode));
    assert_param(IS_QSPI_XIP_SPI_CR_SSM(QSPI_XIP_Struct->SSM));

    uint32_t reg_tmp = 0U;
    uint32_t reg_cr = 0U;
    /* Set  SCKDIV register according to QSPI_XIP BaudRatePrescaler value */
    QSPI_XIPx->SCKDIV = QSPI_XIP_Struct->SCKDIV;
    /* Configure QSPI_XIPx  CPOL bit */
    QSPI_XIPx->SCKMODE |= QSPI_XIP_Struct->CPOL;
    /* Configure QSPI_XIPx  CPHA bit */
    QSPI_XIPx->SCKMODE |= QSPI_XIP_Struct->CPHA;

    /* Configure QSPI_XIPx CS mode */
    reg_tmp = QSPI_XIPx->CSMODE;
    reg_tmp &= ~QSPI_XIP_CSMODE_CLEAR_MASK;
    reg_tmp |= (QSPI_XIP_Struct->CSMode);
    QSPI_XIPx->CSMODE = reg_tmp;

    /* Configure QSPI_XIPx force*/
    reg_tmp = 0;
    reg_tmp = QSPI_XIPx->FORCE;
    reg_tmp &= ~QSPI_XIP_FORCE_CLEAR_MASK;
    reg_tmp |= QSPI_XIP_Struct->Force;
    QSPI_XIPx->FORCE = reg_tmp;

    /* Configure QSPI_XIPx direction, first transmitted ,datasize ,Protol*/
    reg_tmp = 0;
    reg_tmp = QSPI_XIPx->FMT;
    reg_tmp &= ~QSPI_XIP_FMT_CLEAR_MASK;
    reg_tmp |= (QSPI_XIP_Struct->Endian | QSPI_XIP_Struct->DataSize | QSPI_XIP_Struct->ProtolMode);
    QSPI_XIPx->FMT = reg_tmp;

    /* Configure QSPI_XIPxMSTR bits and SSM bits according to  DevMode and  SSM value*/
    reg_cr = QSPI_XIPx->CR;
    reg_cr &= ~ QSPI_XIP_CR_CLEAR_MASK;
    reg_cr |= (QSPI_XIP_Struct->DevMode | QSPI_XIP_Struct->SSM | QSPI_XIP_CR_CSOE_ENABLE | QSPI_XIP_CR_CSI_OFF );
    QSPI_XIPx->CR = reg_cr;

    QSPI_XIPx->FCTRL &=QSPI_XIP_FCTRL_FLASH_DISABLE;
}

/**
  * \brief  Fills each QSPI_XIP_InitTypeDef member with its default value.
  * \param  QSPI_XIP_Struct pointer to a QSPI_XIP_InitTypeDef structure which will be initialized.
  */
void QSPI_XIP_StructInit(QSPI_XIP_InitTypeDef* QSPI_XIP_Struct)
{
    /*--------------- Reset QSPI_XIP init structure parameters values -----------------*/
    /* Initialize the ProtolMode member */
    QSPI_XIP_Struct->ProtolMode =QSPI_XIP_FMT_PROTO_SINGLE;

    /* initialize the DevMode member */
    QSPI_XIP_Struct->DevMode =QSPI_XIP_CR_MODE_MASTER;

    /* initialize the DataSize member */
    QSPI_XIP_Struct->DataSize =QSPI_XIP_FMT_LEN_8B;
    /* Initialize the CPOL member */
    QSPI_XIP_Struct->CPOL =QSPI_XIP_SCKMODE_CPOL_LOW ;
    /* Initialize the CPHA member */
    QSPI_XIP_Struct->CPHA =QSPI_XIP_SCKMODE_CPHA_EDGE1;
    /* Initialize the SSM member */
    QSPI_XIP_Struct->SSM =QSPI_XIP_CR_SSM_HARD;
    /* Initialize the SCKDIV member */
    QSPI_XIP_Struct->SCKDIV =QSPI_XIP_SCKDIV_PRESCALER_4;
    /* Initialize the Endian member */
    QSPI_XIP_Struct->Endian =QSPI_XIP_FMT_ENDIAN_MSB;
    /* Initialize the CSModet member */
    QSPI_XIP_Struct->CSMode =QSPI_XIP_CSMODE_MODE_AUTO;
    /* Initialize the Force member */
    QSPI_XIP_Struct->Force = 0;
}

/**
  * \brief      Configures QSPI_XIP CS in slave mode.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Status new state of the QSPI_XIPx CS signal.
  *                     This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_SSMSwitch(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status == ENABLE) {
        /* Set cs enabled */
        QSPI_XIPx->CR &= (~QSPI_XIP_CR_CSI_OFF);
    } else {
        /* Set cs disabled */
        QSPI_XIPx->CR |=QSPI_XIP_CR_CSI_OFF;
    }
}

/**
  * \brief      Configures QSPI_XIP frame size .
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  FrameSize QSPI_XIP transmit one frame size.
  */
void QSPI_XIP_FrameSizeConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t FrameSize)
{
    uint32_t reg_fmt = QSPI_XIPx->FMT;
    reg_fmt &= ~QSPI_XIP_FMT_LEN_MASK;

    QSPI_XIPx->FMT = (reg_fmt | (FrameSize << QSPI_XIP_FMT_LEN_OFS));
}

/**
  * \brief      just QSPI_XIP bus width.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  BusWidth spi bus width.
  */
void QSPI_XIP_BusWidthSwitch(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t BusWidth)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    
    uint32_t reg_fmt = QSPI_XIPx->FMT;
    reg_fmt &= ~(QSPI_XIP_FMT_PROTO_MASK | QSPI_XIP_FMT_PROTO_HI);
    
    QSPI_XIPx->FMT = (reg_fmt | BusWidth);
}

/**
  * \brief      just QSPI_XIP force enable.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  HrStatus spi hold/reset Status.
  * \param[in]  WpStatus spi write protect Status.
  */
void QSPI_XIP_ForceEnable(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus HrStatus, ControlStatus WpStatus)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(HrStatus));
    assert_param(IS_FUNCTIONAL_STATE(WpStatus));

    if (HrStatus == DISABLE) {
        QSPI_XIPx->FORCE &= ~QSPI_XIP_FORCE_EN;
    } else {
        QSPI_XIPx->FORCE |= QSPI_XIP_FORCE_EN;
    }

    if (WpStatus == DISABLE) {
        QSPI_XIPx->FORCE &= ~QSPI_XIP_FORCE_WP;
    } else {
        QSPI_XIPx->FORCE |= QSPI_XIP_FORCE_WP;
    }
}

/**
  * \brief      Configures QSPI_XIP ddr mode.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Status new state of the QSPI_XIPx CS signal.
  *                     This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_DdrModeEnable(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status == ENABLE) {
        QSPI_XIPx->CR |=QSPI_XIP_CR_DDR_ENABLE;
    } else {
        QSPI_XIPx->CR &= (~QSPI_XIP_CR_DDR_ENABLE);
    }
}

/**
  * \brief      Configures QSPI_XIP Sample Point.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  SamplePoint Usage scenario:  default value of 0, indicating a delay of 3 time units on top of the standard sampling point in SDR mode, with a delay unit of the bus clock
  */
void QSPI_XIP_RxSdrSampleConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t SamplePoint)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    QSPI_XIPx->SDR_SCKSAMPLE &= ~QSPI_XIP_SDR_SCKSAMPLE_SDR_SCKSAMPLE(SamplePoint);
}

/**
  * \brief      Configures QSPI_XIP  TX water mark.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Mark Interruption thresholds
  */
void QSPI_XIP_TxmarkConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Mark)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_TX_RX_WaterMark(Mark));

    /* Set TX Interruption thresholds bit value */
    QSPI_XIPx->TX_MARK = Mark & QSPI_XIP_TX_MASK;
}

/**
  * \brief      Configures QSPI_XIP RX water mark.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Mark Interruption thresholds.
  */
void QSPI_XIP_RxmarkConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Mark)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_TX_RX_WaterMark(Mark));

    /* Set RX Interruption thresholds bit value */
    QSPI_XIPx->RX_MARK = Mark & QSPI_XIP_RX_MASK;
}

/**
  * \brief      Configures QSPI_XIP  TX water mark.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Size tx size
  */
void QSPI_XIP_TxsizeConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Size)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    /* Set TX Interruption thresholds bit value */
    QSPI_XIPx->TSIZE = Size & QSPI_XIP_TSIZE_NUM_MASK;
}

/**
  * \brief      Configures QSPI_XIP RX water mark.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Size rx size
  */
void QSPI_XIP_RxsizeConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Size)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    /* Set RX Interruption thresholds bit value */
    QSPI_XIPx->RSIZE = Size & QSPI_XIP_RSIZE_NUM_MASK;
}

/**
  * \brief      Configures QSPI_XIP Direction
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Direction QSPI_XIP_FMT_DIR_TX,QSPI_XIP_FMT_DIR_RX.
  *  This parameter can be one of the following values
  *     \arg QSPI_XIP_FMT_DIR_TX Selects Tx transmission direction.
  *     \arg QSPI_XIP_FMT_DIR_RX Selects Rx receive direction.
  */
void QSPI_XIP_DirectionConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Direction)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_SPI_FMT_DIR(Direction));

    if (Direction ==QSPI_XIP_FMT_DIR_TX) {
        /* Set the Tx/Rx mode */
        QSPI_XIPx->FMT |=QSPI_XIP_FMT_DIR_TX;
    } else {
        /* Set the Rx only mode */
        QSPI_XIPx->FMT &= (~QSPI_XIP_FMT_DIR_TX);
    }
}

/**
  * \brief      Enables or disables QSPI_XIP interrupts.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  QSPI_XIP_IE specifies the QSPI_XIP interrupt source to be enabled or disabled.
  *  This parameter can be one of the following values
  *     \arg QSPI_XIP_IE_TX
  *     \arg QSPI_XIP_IE_RX
  *     \arg QSPI_XIP_IE_TX_UDR
  *     \arg QSPI_XIP_IE_RX_OVR
  *     \arg QSPI_XIP_IE_RX_UDR
  *     \arg QSPI_XIP_IE_TX_OVR
  *     \arg QSPI_XIP_IE_DONE
  *     \arg QSPI_XIP_IE_TX_DONE
  *     \arg QSPI_XIP_IE_RX_DONE
  * \param[in]  Status new state of the QSPI_XIPx peripheral.
  *                    This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_ITConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_IE, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        /* Enable the selected QSPI_XIP interrupt */
        QSPI_XIPx->IE |= QSPI_XIP_IE;
    } else {
        /* Disable the selected QSPI_XIP interrupt */
        QSPI_XIPx->IE &= ~QSPI_XIP_IE;
    }
}

/**
  * \brief      Enables or disables QSPI_XIP interrupts.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  QSPI_XIP_IE specifies the QSPI_XIP interrupt source to be enabled or disabled.
  *     \arg QSPI_XIP_IE_TX
  *     \arg QSPI_XIP_IE_RX
  *     \arg QSPI_XIP_IE_TX_UDR
  *     \arg QSPI_XIP_IE_RX_OVR
  *     \arg QSPI_XIP_IE_RX_UDR
  *     \arg QSPI_XIP_IE_TX_OVR
  *     \arg QSPI_XIP_IE_DONE
  *     \arg QSPI_XIP_IE_TX_DONE
  *     \arg QSPI_XIP_IE_RX_DONE
  * \retval  The specified QSPI_XIP interrupt Status, 
  *          the value can be SET or RESET.
  */
FlagStatus QSPI_XIP_GetITConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_IE)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_CONFIG_IE(QSPI_XIP_IE));

    if (QSPI_XIPx->IE & QSPI_XIP_IE) {
        /* QSPI_XIP_IT is set */
        return SET;
    } else {
        /* QSPI_XIP_IT is reset */
        return RESET;
    }
}

/**
  * \brief     Checks whether the specified QSPI_XIP interrupt has occurred or not.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  QSPI_XIP_IP specifies the QSPI_XIP interrupt source to check.
  *     \arg QSPI_XIP_IP_TX
  *     \arg QSPI_XIP_IP_RX
  * \retval  The specified QSPI_XIP interrupt Status, 
  *          the value can be SET or RESET.
  */
FlagStatus QSPI_XIP_GetIPStatus(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_IP)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    /* Check the Status of the specified QSPI_XIP interrupt */
    if (QSPI_XIPx->IP & QSPI_XIP_IP) {
        /* QSPI_XIP_IP is set */
        return SET;
    } else {
        /* QSPI_XIP_IP is reset */
        return RESET;
    }
}

/**
  * \brief      Checks whether the specified QSPI_XIP flag is set or not.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  QSPI_XIP_FLAG specifies the QSPI_XIP flag to check.
  *    \arg QSPI_XIP_STATUS_BUSY
  *    \arg QSPI_XIP_STATUS_RX_OVR
  *    \arg QSPI_XIP_STATUS_TX_UDR
  *    \arg QSPI_XIP_STATUS_TX_FULL
  *    \arg QSPI_XIP_STATUS_RX_EMPTY
  *    \arg QSPI_XIP_STATUS_RX_UDR
  *    \arg QSPI_XIP_STATUS_TX_OVR
  *    \arg QSPI_XIP_STATUS_TX_EMPTY
  *    \arg QSPI_XIP_STATUS_RX_FULL
  *    \arg QSPI_XIP_STATUS_DONE
  *    \arg QSPI_XIP_STATUS_TX_DONE
  *    \arg QSPI_XIP_STATUS_RX_DONE
  * \retval  The specified QSPI_XIP flag Status, 
  *          the value can be SET or RESET.
  */
FlagStatus QSPI_XIP_GetFlag(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_FLAG)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    /* Check the Status of the specified QSPI_XIP flag */
    if ((QSPI_XIPx->STATUS & QSPI_XIP_FLAG) != RESET) {
        /* QSPI_XIP_FLAG is set */
        return SET;
    } else {
        /* QSPI_XIP_FLAG is reset */
        return RESET;
    }
}

/**
  * \brief      Clears the specified QSPI_XIP flag.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  QSPI_XIP_FLAG specifies the QSPI_XIP flag to clears.
  *    \arg QSPI_XIP_STATUS_RX_OVR
  *    \arg QSPI_XIP_STATUS_TX_UDR
  *    \arg QSPI_XIP_STATUS_RX_UDR
  *    \arg QSPI_XIP_STATUS_TX_OVR
  *    \arg QSPI_XIP_STATUS_DONE
  *    \arg QSPI_XIP_STATUS_TX_DONE
  *    \arg QSPI_XIP_STATUS_RX_DONE
  */
void QSPI_XIP_ClearFlag(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t QSPI_XIP_FLAG)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    
    /* Clear the selected QSPI_XIP flag */
    QSPI_XIPx->STATUS = QSPI_XIP_FLAG;
}

/**
  * \brief      Configures QSPI_XIP CS mode for the selected QSPI_XIP.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  CsMode specifies the chip select signal mode.
  *     \arg AUTO CS is automatically controlled by the hardware.
  *     \arg HOLD CS is controlled by CSID¡¢CSDef¡¢CSMode and QSPI_XIP_FCTRL.
  *     \arg OFF CS is controlled by CSID¡¢CSDef and CSMode.
  */
void QSPI_XIP_CSModeConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t CsMode)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_SPI_CSMODE_MODE(CsMode));

    /* Set CSmode */
    QSPI_XIPx->CSMODE = CsMode;
}

/**
  * \brief      Configures QSPI_XIP CSID value.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  CsID configures the value of CSID.
  */
void QSPI_XIP_CSIDConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t CsID)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_SPI_CSID_NUM(CsID));

    /* Set new CSID bit value */
    QSPI_XIPx->CSID = CsID;
}

/**
  * \brief      Configures QSPI_XIP delay0
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Sckcs Specifies how many cycles are still valid at least after the last SCK clock edge, after the end of sending data.
  * \param[in]  Cssck Specifies how many cycles are still valid at least after the last SCK clock edge, before the end of sending data.
*/
void QSPI_XIP_Delay0Config(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Sckcs, uint32_t Cssck)
{
    uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    tmp |= ((Sckcs << 16) & QSPI_XIP_DELAY0_SCKCS_MASK);
    tmp |= (Cssck & QSPI_XIP_DELAY0_CSSCK_MASK);
    QSPI_XIPx->DELAY0 = tmp;
}

/**
  * \brief      Configures QSPI_XIP delay1
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Intercs Specifies Minimum CS inactive time.
  * \param[in]  Interxfr Specifies Maximum interframe delay.
  * \note       This only happen if QSPI_XIP_SCKMODE is configured to HOLD or OFF modes.
  */
void QSPI_XIP_Delay1Config(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Intercs, uint32_t Interxfr)
{
    uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    tmp |= (Interxfr << 16);
    tmp |= Intercs;
    QSPI_XIPx->DELAY1 = tmp;
}

/**
  * \brief      Transmits a Data through the QSPI_XIPx peripheral.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Data Data to be transmitted.
  */
void QSPI_XIP_SendData(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Data)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_QSPI_XIP_DATA(Data));

    /* Write in the TXDATA register the data to be sent */
    QSPI_XIPx->TXDATA = Data;
}

/**
  * \brief      QSPI_XIP receive data function.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \retval     The value of the received data.
  */
uint32_t QSPI_XIP_ReceiveData(QSPI_XIP_TypeDef* QSPI_XIPx)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    /* Return the data in the RXDATA register */
    return (uint32_t)(QSPI_XIPx->RXDATA);
}

/**
  * \brief      Enables/Disables flashxip ddr enable.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  XipDdrMode QSPI_XIP ddr enable structure definition.
  */
void QSPI_XIP_DdrEnable(QSPI_XIP_TypeDef* QSPI_XIPx, QSPI_XIP_DdrEnableTypedef XipDdrMode)
{
    uint32_t xip_ddr_en = 0;
    if (ENABLE == XipDdrMode.CMD_DDR_EN) {
        xip_ddr_en |= QSPI_XIP_FFMT1_DDR_EN(1);
    }
    if (ENABLE == XipDdrMode.ADDR_DDR_EN) {
        xip_ddr_en |= QSPI_XIP_FFMT1_DDR_EN(2);
    }
    if (ENABLE == XipDdrMode.MODE_DDR_EN) {
        xip_ddr_en |= QSPI_XIP_FFMT1_DDR_EN(4);
    }
    if (ENABLE == XipDdrMode.DATA_DDR_EN) {
        xip_ddr_en |= QSPI_XIP_FFMT1_DDR_EN(8);
    }
    QSPI_XIPx->FFMT1 |= xip_ddr_en;
}

/**
  * \brief      Enables/Disables flashxip mode.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Status new state of the selected QSPI_XIP XIP mode.
  *                    This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_Enable(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        /* Enable the selected QSPI_XIP XIP mode */
        QSPI_XIPx->FCTRL |= (uint32_t)QSPI_XIP_FCTRL_FLASH_ENABLE;
    } else {
        /* Disable the selected QSPI_XIP mode */
        QSPI_XIPx->FCTRL &= (~(uint32_t)QSPI_XIP_FCTRL_FLASH_ENABLE);
    }
}

/**
  * \brief      Enables/Disables handle mask mode.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Status new state of the selected QSPI_XIP XIP mode.
  *                    This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_EnableDataMask(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        /* Enable the selected QSPI_XIP XIP mode */
        QSPI_XIPx->FCTRL |= (uint32_t)QSPI_XIP_FCTRL_FLASH_WMASK_ENABLE;
    } else {
        /* Disable the selected QSPI_XIP mode */
        QSPI_XIPx->FCTRL &= (~(uint32_t)QSPI_XIP_FCTRL_FLASH_WMASK_ENABLE);
    }
}

/**
  * \brief      Enables/Disables handle burst mode.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Status new state of the selected QSPI_XIP XIP mode.
  *                    This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_EnableBurstMode(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        /* Enable the selected QSPI_XIP XIP mode */
        QSPI_XIPx->FCTRL |= (uint32_t)QSPI_XIP_FCTRL_FLASH_BURST_ENABLE;
    } else {
        /* Disable the selected QSPI_XIP mode */
        QSPI_XIPx->FCTRL &= (~(uint32_t)QSPI_XIP_FCTRL_FLASH_BURST_ENABLE);
    }
}

/**
  * \brief      Enables/Disables handle wrap mode.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Status new state of the selected QSPI_XIP XIP mode.
  *                    This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_EnableWrapMode(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        /* Enable the selected QSPI_XIP XIP mode */
        QSPI_XIPx->FCTRL |= (uint32_t)QSPI_XIP_FCTRL_FLASH_WRAP_EN_ENABLE;
    } else {
        /* Disable the selected QSPI_XIP mode */
        QSPI_XIPx->FCTRL &= (~(uint32_t)QSPI_XIP_FCTRL_FLASH_WRAP_EN_ENABLE);
    }
}

/**
  * \brief      Enables/Disables xip write en.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Status new state of the selected QSPI_XIP XIP mode.
  *                    This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_EnableXipWrite(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        /* Enable the selected QSPI_XIP XIP mode */
        QSPI_XIPx->FCTRL |= (uint32_t)QSPI_XIP_FCTRL_FLASHW_ENABLE;
    } else {
        /* Disable the selected QSPI_XIP mode */
        QSPI_XIPx->FCTRL &= (~(uint32_t)QSPI_XIP_FCTRL_FLASHW_ENABLE);
    }
}

/**
  * \brief      Enables/Disables xip boundary en.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  Status new state of the selected QSPI_XIP XIP mode.
  *                    This parameter can be ENABLE or DISABLE.
  */
void QSPI_XIP_EnableBoundaryMode(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    assert_param(IS_FUNCTIONAL_STATE(Status));

    if (Status) {
        /* Enable the selected QSPI_XIP XIP mode */
        QSPI_XIPx->FCTRL |= (uint32_t)QSPI_XIP_FCTRL_BOUNDARY_EN_ENABLE;
    } else {
        /* Disable the selected QSPI_XIP mode */
        QSPI_XIPx->FCTRL &= (~(uint32_t)QSPI_XIP_FCTRL_BOUNDARY_EN_ENABLE);
    }
}

/**
  * \brief      the format of date to send in flashxip write mode.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \param[in]  QSPI_XIP_Timing QSPI_XIP_Timing pointer to a QSPI_XIP_TimingTypedef structure that
  *             contains the configuration information for the specified QSPI_XIP peripheral in xip timing.
  */
void QSPI_XIP_TimingConfig(QSPI_XIP_TypeDef* QSPI_XIPx, QSPI_XIP_TimingTypedef* QSPI_XIP_Timing)
{
    uint32_t reg_ffmt_tmp = 0U;
    uint32_t reg_ffmt1_tmp = 0U;
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));

    reg_ffmt_tmp = QSPI_XIPx->FFMT;
    reg_ffmt1_tmp = QSPI_XIPx->FFMT1;

    /* Clear config mask */
    reg_ffmt_tmp &= ~(QSPI_XIP_FFMT_CMD_CODE_MASK | QSPI_XIP_FFMT_CMD | QSPI_XIP_FFMT_ADDR_LEN_MASK | \
                  QSPI_XIP_FFMT_PAD_CNT_MASK | QSPI_XIP_FFMT_CMD_PROTO_MASK |QSPI_XIP_FFMT_ADDR_PROTO_MASK | \
                  QSPI_XIP_FFMT_DATA_PROTO_MASK | QSPI_XIP_FFMT_ENDINA_F | QSPI_XIP_FFMT_CMD_CODE_MASK);
    reg_ffmt1_tmp &= ~(QSPI_XIP_FFMT1_WCMD_CODE_MASK | QSPI_XIP_FFMT1_WPAD_CNT_MASK | QSPI_XIP_FFMT1_PAD_CNT_H | \
                  QSPI_XIP_FFMT1_DDR_EN_MASK | QSPI_XIP_FFMT1_MODE_PROTO_MASK |QSPI_XIP_FFMT1_MODE_CODE_MASK | \
                  QSPI_XIP_FFMT1_MODE_CNT_MASK);

    reg_ffmt_tmp |= QSPI_XIP_Timing->QSPI_XIP_FFMT_u.reg_ffmt;
    reg_ffmt1_tmp |= QSPI_XIP_Timing->QSPI_XIP_FFMT1_u.reg_ffmt1;

    QSPI_XIPx->FFMT = reg_ffmt_tmp;
    QSPI_XIPx->FFMT1 = reg_ffmt1_tmp;
}

/**
  * \brief      Read Tx FIFO Entry.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \retval     the num of fifo entry.
  */
uint32_t QSPI_XIP_TxReadEntry(QSPI_XIP_TypeDef* QSPI_XIPx)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    return QSPI_XIPx->FIFO_NUM & BITS(0, 15);
}

/**
  * \brief      Read Rx FIFO Entry.
  * \param[in]  QSPI_XIPx where x can be to select the QSPI_XIP peripheral number.
  * \retval     the num of fifo entry.
  */
uint32_t QSPI_XIP_RxReadEntry(QSPI_XIP_TypeDef* QSPI_XIPx)
{
    /* Check the parameters */
    assert_param(IS_QSPI_XIP_ALL_PERIPH(QSPI_XIPx));
    return (QSPI_XIPx->FIFO_NUM & BITS(16, 31)) >> 16;
}

/**
  * \brief  Transmit and Receive an amount of data in blocking mode(just support single line mode).
  * \param  QSPI_XIPx pointer to a QSPI_XIP_HandleTypeDef structure that contains
  *                the configuration information for QSPI_XIP module.
  * \param  pTxData pointer to transmission data buffer
  * \param  pRxData pointer to reception data buffer
  * \retval FlagStatus
  */
void QSPI_XIP_TransmitReceive(QSPI_XIP_TypeDef* QSPI_XIPx, uint8_t *pTxData, uint8_t *pRxData)
{
    while(SET == QSPI_XIP_GetFlag(QSPI_XIPx, QSPI_XIP_STATUS_TX_FULL)){}
    QSPI_XIPx->TXDATA = *pTxData;
    while (SET == QSPI_XIP_GetFlag(QSPI_XIPx, QSPI_XIP_STATUS_BUSY)){}

    while(SET == QSPI_XIP_GetFlag(QSPI_XIPx, QSPI_XIP_STATUS_RX_EMPTY)){}
    *pRxData = (uint32_t)(QSPI_XIPx->RXDATA);
}

/**
  * \brief  Configure Chip Select
  * \param  QSPI_XIPx pointer to a QSPI_XIP_HandleTypeDef structure that contains
  *                the configuration information for QSPI_XIP module.
  * \param  csid Chip select data line
  * \param  Status new state of the QSPI_XIPx peripheral.
  *            This parameter can be ENABLE or DISABLE.
  * \retval None
  */
void QSPI_XIP_CSEnable(QSPI_XIP_TypeDef* QSPI_XIPx, uint8_t csid, ControlStatus Status)
{
    if(ENABLE == Status){
        QSPI_XIPx->CSID |= csid;
    } else {
        QSPI_XIPx->CSID &= (~csid);
    }
}

void QSPI_XIP_ClearFifo(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t Channel)
{
    if(QSPI_XIP_TX_FIFO == Channel) {
        QSPI_XIPx->CR |= QSPI_XIP_CR_TX_FIFO_CLR_CLEAR;
    } else if (QSPI_XIP_RX_FIFO == Channel) {
        QSPI_XIPx->CR |= QSPI_XIP_CR_RX_FIFO_CLR_CLEAR;
    }
}

/**
  * \brief  Configure rx fifo receive data enable
  * \param  QSPI_XIPx pointer to a QSPI_XIP_HandleTypeDef structure that contains
  *                the configuration information for QSPI_XIP module.
  * \param  Status new state of the QSPI_XIPx peripheral.
  *            This parameter can be ENABLE or DISABLE.
  * \retval None
  */
void QSPI_XIP_RxFifoEnable(QSPI_XIP_TypeDef* QSPI_XIPx, ControlStatus Status)
{
    if(ENABLE == Status){
        QSPI_XIPx->CR |= QSPI_XIP_CR_RECEIVE_DATA_ENABLE;
    } else {
        QSPI_XIPx->CR &= (~QSPI_XIP_CR_RECEIVE_DATA_ENABLE);
    }
}

/**
  * \brief QSPI_XIP data width config
  * \param QSPI_XIPx where x can be to select the QSPI_XIP peripheral.
  * \param DateWidth data width.
  */
void QSPI_XIP_DataWidthConfig(QSPI_XIP_TypeDef* QSPI_XIPx, uint32_t DateWidth)
{
    uint32_t tmp_reg;
    tmp_reg = QSPI_XIPx->FMT;
    tmp_reg &= (~QSPI_XIP_FMT_LEN_MASK);
    tmp_reg |= DateWidth;
    QSPI_XIPx->FMT = tmp_reg;
}

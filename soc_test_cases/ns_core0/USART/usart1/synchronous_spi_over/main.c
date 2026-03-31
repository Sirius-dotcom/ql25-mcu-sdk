#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE (16+2)

uint8_t txbuffer1[ARR_SIZE] = {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,0xa9, 0xaA,0xa4, 0xa5, 0xa6, 0xa7, 0xa8,0xa9};

uint8_t state0 = 1;

/**
  * \brief configure SPI
  */
void SPI_Config(void)
{
    QSPI_InitTypeDef spi_init_parameter = {0};
    /* deinitilize SPI and the parameters */
    QSPI_StructInit(&spi_init_parameter);
    /* configure QSPI3 */
    spi_init_parameter.DevMode = QSPI_CR_MODE_MASTER;
    spi_init_parameter.SSM = QSPI_CR_SSM_SOFT;
    spi_init_parameter.DataSize = QSPI_FMT_LEN_8B;
    spi_init_parameter.Endian = QSPI_FMT_ENDIAN_LSB;
    spi_init_parameter.CPOL = QSPI_SCKMODE_CPOL_LOW;
    spi_init_parameter.CPHA = QSPI_SCKMODE_CPHA_EDGE1;
    spi_init_parameter.SCKDIV = QSPI_SCKDIV_PRESCALER_8;
    QSPI_Init(QSPI3, &spi_init_parameter);
}

/*USART and QSPI3 COMUNICATION */
#ifdef USART1_SCLK_PAD_SEL

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART1_SCLK_IOF_IVAL,USART1_SCLK_PAD_SEL,USART1_SCLK_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART1_TX_IOF_OVAL,USART1_TX_PAD_SEL,USART1_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART1_RX_IOF_IVAL,USART1_RX_PAD_SEL,USART1_RX_HS_SEL,0,0);
    #ifdef QSPI3_SCK_IOF_IVAL
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_SCK_IOF_OVAL, QSPI3_SCK_PAD_SEL, QSPI3_SCK_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_0_IOF_OVAL,QSPI3_DQ_0_PAD_SEL, QSPI3_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_1_IOF_IVAL,QSPI3_DQ_1_PAD_SEL, QSPI3_DQ_1_HS_SEL, 0, 0);
    #endif
}

#else

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART1_TX_CLK_IOF_IVAL,USART1_TX_CLK_PAD_SEL,USART1_TX_CLK_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART1_TX_IOF_OVAL,USART1_TX_PAD_SEL,USART1_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART1_RX_IOF_IVAL,USART1_RX_PAD_SEL,USART1_RX_HS_SEL,0,0);
    #ifdef QSPI3_SCK_IOF_IVAL
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_SCK_IOF_OVAL, QSPI3_SCK_PAD_SEL, QSPI3_SCK_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_0_IOF_OVAL, QSPI3_DQ_0_PAD_SEL, QSPI3_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_1_IOF_IVAL,QSPI3_DQ_1_PAD_SEL, QSPI3_DQ_1_HS_SEL, 0, 0);
    #endif

}

#endif
/**
  * \brief memory compare function
  *
  * \param src: source data pointer
  * \param dst: destination data pointer
  * \param length: the compare data length
  *
  * \retval ErrStatus: ERROR or SUCCESS
  */
ErrStatus MemoryCompare(uint8_t* src, uint8_t* dst, uint16_t length)
{
    while (length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

void USART1_IRQHandler(void)
{
   
    if(USART_GetITStatus(USART1,USART_STATUS_SPI_SLV_OVER_RUN) == SET)
    {
        USART_ClearITStatus(USART1,USART_STATUS_SPI_SLV_OVER_RUN);
        USART_ITConfig(USART1,USART_INT_EN_SPI_SLV_OVER_RUN_IRQ_EN,DISABLE);
        state0 =0;
    }

}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif
    #ifdef MISC_HAS_USART1_HAS_CLK
    usart1_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_USART1_RST
    usart1_set_rst(DISABLE);
    usart1_set_rst(ENABLE);
    #endif
    
    #ifdef MISC_HAS_QSPI3_HAS_CLK
    qspi3_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_QSPI3_RST
    qspi3_set_rst(DISABLE);
    qspi3_set_rst(ENABLE);
    #endif

    iomux_config();
    __enable_irq();
    ECLIC_Register_IRQ(USART1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    USART1_IRQHandler);

    /* configure SPI */
    SPI_Config();
    /* configure USART */
    USART_ClockInitTypeDef USART_ClockInitStruct = {0};
    USART_ClockInitStruct.USART_Clock = ENABLE;
    USART_ClockInitStruct.USART_CPOL = 0;
    USART_ClockInitStruct.USART_CPHA = 0;
    USART_ClockInitStruct.USART_LastBit = USART_SETUP_LBCL;
    USART_ClockInit(USART1, &USART_ClockInitStruct);

    USART_InitTypeDef usart_init_t = {0};
    USART_StructInit(&usart_init_t);
    usart_init_t.USART_BaudRate = 115200;
    usart_init_t.USART_WordLength = USART_SETUP_BIT_LENGTH_8;
    usart_init_t.USART_StopBits = USART_SETUP_CFG_STOP_BIT_1BIT;
    usart_init_t.USART_TX_CTL = USART_TXCTRL_TXEN_ENABLE;
    usart_init_t.USART_RX_CTL = USART_RXCTRL_RXEN_ENABLE;
    usart_init_t.USART_Parity = USART_SETUP_PARITY_EN_DISABLE;
    usart_init_t.USART_HardwareFlowControl = USART_SETUP_RTSE_DISABLE | USART_SETUP_CTSE_DISABLE;
    usart_init_t.USART_RX_IDLE_Timeout = 0xFF;
    usart_init_t.USART_RX_WM_Timeout = 0xFF;
    usart_init_t.USART_TX_Data_Size = USART_TX_DATASIZE_NUM_BYTE;
    usart_init_t.USART_RX_Data_Size = USART_RX_DATASIZE_NUM_BYTE;
    usart_init_t.USART_DMA_EN = 0;
    usart_init_t.USART_RX_DMA_EN = 0;
    USART_Init(USART1, &usart_init_t);

    USART_SynClockConfig(USART1, ENABLE);
    USART_SLV_EN(USART1,ENABLE);
    USART_DIS_NSS_EN(USART1,ENABLE);
    USART_CleanFifo_RX(USART1);
    USART_CleanFifo_TX(USART1);
    USART_ITConfig(USART1,USART_INT_EN_SPI_SLV_OVER_RUN_IRQ_EN,ENABLE);

    for(uint8_t i=0;i<ARR_SIZE;i++){
        while (SET == QSPI_GetFlag(QSPI3,QSPI_STATUS_TX_FULL)) {}
        QSPI_SendData(QSPI3, txbuffer1[i]);    
    }

    while(state0){}
    if (state0) {
        simulation_fail();
    } else {
        simulation_pass();
    }

    while (1) {}
}

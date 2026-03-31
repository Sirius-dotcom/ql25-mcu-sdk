#include <stdio.h>
#include "ns_sdk_hal.h"

#define txbuffer_size1   (countof(txbuffer1) )
#define txbuffer_size2   (countof(txbuffer2) )
#define DYMMY_BYTE       0x00000000
#define countof(a)       (sizeof(a) / sizeof(*(a)))

#if 0
uint8_t txbuffer1[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                       0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
                       0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
                       0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
                       0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
                       0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
                       0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
                       0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F};
uint8_t txbuffer2[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                       0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
                       0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
                       0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
                       0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
                       0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
                       0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
                       0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F};
#endif

uint8_t txbuffer1[] = {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,0xa9, 0xaA};
uint8_t txbuffer2[] = {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xbA};

uint8_t rxbuffer1[txbuffer_size2] = {0};
uint8_t rxbuffer2[txbuffer_size1] = {0};
__IO uint8_t data_read1 = txbuffer_size2;
__IO uint8_t data_read2 = txbuffer_size1;
__IO uint8_t tx_counter1 = 0, rx_counter1 = 0;
__IO uint8_t tx_counter2 = 0, rx_counter2 = 0;
__IO ErrStatus state1 = ERROR;
__IO ErrStatus state2 = ERROR;

/**
  * \brief configure SPI
  */
void SPI_Config(void)
{
    QSPI_InitTypeDef spi_init_parameter = {0};
    /* deinitilize SPI and the parameters */
    QSPI_StructInit(&spi_init_parameter);
    /* configure QSPI3 */
    spi_init_parameter.DevMode = QSPI_CR_MODE_SLAVE;
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
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART1_SCLK_IOF_OVAL,USART1_SCLK_PAD_SEL,USART1_SCLK_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART1_TX_IOF_OVAL,USART1_TX_PAD_SEL,USART1_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART1_RX_IOF_IVAL,USART1_RX_PAD_SEL,USART1_RX_HS_SEL,0,0);
    #ifdef QSPI3_SCK_IOF_IVAL
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_SCK_IOF_IVAL, QSPI3_SCK_PAD_SEL, QSPI3_SCK_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_0_IOF_IVAL,QSPI3_DQ_0_PAD_SEL, QSPI3_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_1_IOF_OVAL,QSPI3_DQ_1_PAD_SEL, QSPI3_DQ_1_HS_SEL, 0, 0);
    #endif
}

#else

void iomux_config(void)
{
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART1_TX_CLK_IOF_OVAL,USART1_TX_CLK_PAD_SEL,USART1_TX_CLK_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART1_TX_IOF_OVAL,USART1_TX_PAD_SEL,USART1_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART1_RX_IOF_IVAL,USART1_RX_PAD_SEL,USART1_RX_HS_SEL,0,0);
    #ifdef QSPI3_SCK_IOF_IVAL
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_SCK_IOF_IVAL, QSPI3_SCK_PAD_SEL, QSPI3_SCK_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI3_DQ_0_IOF_IVAL, QSPI3_DQ_0_PAD_SEL, QSPI3_DQ_0_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI3_DQ_1_IOF_OVAL,QSPI3_DQ_1_PAD_SEL, QSPI3_DQ_1_HS_SEL, 0, 0);
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
    USART_CleanFifo_RX(USART1);
    USART_CleanFifo_TX(USART1);

    while (data_read2--) {
        QSPI_SSMSwitch(QSPI3, ENABLE);
        /* write one byte in the USART1 data register */
        USART_SendData(USART1, txbuffer1[tx_counter1++]);
        /* wait until end of transmit */
        while (SET == USART_GetFlag(USART1, USART_STATUS_TX_BUSY)) {}
        /* wait the byte is entirely received by QSPI3 */
        while (SET == QSPI_GetFlag(QSPI3, QSPI_STATUS_RX_EMPTY)) {}
        /* store the received byte in the rxbuffer2 */
        rxbuffer2[rx_counter2++] = QSPI_ReceiveData(QSPI3);
        QSPI_SSMSwitch(QSPI3, DISABLE);
    }
    /* configure USART */
    usart_init_t.USART_RX_CTL = USART_RXCTRL_RXEN_ENABLE;
    USART_Init(USART1, &usart_init_t);
    /* clear the USART1 data register */
    USART_CleanFifo_RX(USART1);
    USART_CleanFifo_TX(USART1);

    while (data_read1--) {
        QSPI_SSMSwitch(QSPI3, ENABLE);
        /* wait until end of transmit */
        while (SET == QSPI_GetFlag(QSPI3,QSPI_STATUS_TX_FULL)) {}
        /* write one byte in the QSPI3 transmit data register */
        QSPI_SendData(QSPI3, txbuffer2[tx_counter2++]);
        /* send a dummy byte to generate clock to slave */
        USART_SendData(USART1, 0xff);
        /* wait until end of transmit */
        while (SET == USART_GetFlag(USART1, USART_STATUS_TX_BUSY)) {}
        /* wait the byte is entirely received by USART1 */
        while (SET == USART_GetFlag(USART1, USART_STATUS_RX_BUSY)) {}
        /* store the received byte in the rxbuffer1 */
        rxbuffer1[rx_counter1++] = USART_ReceiveData(USART1);
        QSPI_SSMSwitch(QSPI3, DISABLE);
    }
    /* check the received data with the send ones */
    state1 = MemoryCompare(txbuffer1, rxbuffer2, txbuffer_size1);
    state2 = MemoryCompare(txbuffer2, rxbuffer1, txbuffer_size2);
    if (state1 & state2) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 6

#define TX_RX_SIZE 6

uint8_t USART1_send_buffer[ARR_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
uint8_t USART2_receive_buffer[ARR_SIZE] = {0};
uint8_t USART1_receive_buffer[ARR_SIZE] = {};
uint8_t USART2_send_buffer[ARR_SIZE] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};

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
    while (length--){
        if (*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

/* USART PAD CONFIG */
void iomux_config(void)
{
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART1_TX_IOF_OVAL,USART1_TX_PAD_SEL,USART1_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART1_RX_IOF_IVAL,USART1_RX_PAD_SEL,USART1_RX_HS_SEL,0,0); 
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART2_TX_IOF_OVAL,USART2_TX_PAD_SEL,USART2_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART2_RX_IOF_IVAL,USART2_RX_PAD_SEL,USART2_RX_HS_SEL,0,0);
}

void main(void)
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
    
    #ifdef MISC_HAS_USART2_HAS_CLK
    usart2_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_USART2_RST
    usart2_set_rst(DISABLE);
    usart2_set_rst(ENABLE);
    #endif

    iomux_config();

    USART_InitTypeDef usart_init_t = {0};
    USART_StructInit(&usart_init_t);
    /* USART_InitStruct members default value */
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

    USART_Init(USART1, &usart_init_t);
    USART_Init(USART2, &usart_init_t);

    USART_Set_RXSize(USART1,TX_RX_SIZE);
    USART_Set_TXSize(USART1,TX_RX_SIZE);
    USART_Set_RXSize(USART2,TX_RX_SIZE);
    USART_Set_TXSize(USART2,TX_RX_SIZE);

    int i = 0, j = 0, state0 = 0, state1 = 0;
    /* clean USART2 FIFO */
    USART_CleanFifo_RX(USART2);
    USART_CleanFifo_TX(USART2);

    do {
        USART_SendData(USART1, USART1_send_buffer[i]);
    } while ((++i) < ARR_SIZE);

    while (!USART_GetTransferFlag(USART1, USART_STATUS_TX_EOT)) {}

    for (int i = 0; i < ARR_SIZE; i++) {
        USART2_receive_buffer[i] = USART_ReceiveData(USART2);
    }
    
    /* clean USART1 FIFO */
    USART_CleanFifo_RX(USART1);
    USART_CleanFifo_TX(USART1);

    do {
        USART_SendData(USART2, USART2_send_buffer[j]);
        USART1_receive_buffer[j] = USART_ReceiveData(USART1);
    } while ((++j) < ARR_SIZE);

    while (!USART_GetTransferFlag(USART1, USART_STATUS_RX_EOT)) {}

    state0 = MemoryCompare(USART1_send_buffer, USART2_receive_buffer, ARR_SIZE);
    state1 = MemoryCompare(USART1_receive_buffer, USART2_send_buffer, ARR_SIZE);

    if (state0 & state1) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 10

uint8_t USART1_send_buffer[ARR_SIZE]={ 0x01 ,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
uint8_t USART2_receive_buffer[ARR_SIZE]={0};
uint8_t USART1_receive_buffer[ARR_SIZE]={};
uint8_t USART2_send_buffer[ARR_SIZE]={ 0x11 ,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a};

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
/* USART PAD CONFIG */
void iomux_config(void)
{
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART1_TX_IOF_IVAL,USART1_TX_PAD_SEL,USART1_TX_HS_SEL,0,0); 
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART1_RX_IOF_IVAL,USART1_RX_PAD_SEL,USART1_RX_HS_SEL,0,0); 
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART1_TX_IOF_OVAL,USART1_TX_PAD_SEL,USART1_TX_HS_SEL,0,0); 
    
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART2_TX_IOF_IVAL,USART2_TX_PAD_SEL,USART2_TX_HS_SEL,0,0); 
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART2_RX_IOF_IVAL,USART2_RX_PAD_SEL,USART2_RX_HS_SEL,0,0); 
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART2_TX_IOF_OVAL,USART2_TX_PAD_SEL,USART2_TX_HS_SEL,0,0); 
}
/* USART IOF LOOPBACK */
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

    USART_InitTypeDef usart_init_t ={0};
    USART_StructInit(&usart_init_t);
    USART_SetPrescaler(USART1, 10);
    USART_SetGuardTime(USART1, 16);
    USART_SetupTxpue(USART1, 1);

    USART_SetPrescaler(USART2, 10);
    USART_SetGuardTime(USART2, 16);
    USART_SetupTxpue(USART2, 1);
    USART_StructInit(&usart_init_t);
#if defined(CFG_SIMULATION)
    usart_init_t.USART_BaudRate = 115200;
#else
    usart_init_t.USART_BaudRate = 9600;
#endif
    usart_init_t.USART_WordLength = USART_SETUP_BIT_LENGTH_8;
    usart_init_t.USART_StopBits = USART_SETUP_CFG_STOP_BIT_1_5BIT;
    usart_init_t.USART_TX_CTL = USART_TXCTRL_TXEN_ENABLE;
    usart_init_t.USART_RX_CTL = USART_RXCTRL_RXEN_ENABLE;
    usart_init_t.USART_Parity = USART_SETUP_PARITY_EN_ENABLE;
    usart_init_t.USART_SC_EN = USART_SETUP_SMARTCARD_EN;
    USART_Init(USART1, &usart_init_t);
    USART_Init(USART2, &usart_init_t);

    int i = 0, j = 0, state0 = 0, state1 = 0;
    USART_CleanFifo_RX(USART2);
    USART_CleanFifo_TX(USART2);

    do {
        USART_SendData(USART1, USART1_send_buffer[i]);
        USART2_receive_buffer[i] = USART_ReceiveData(USART2);
    } while ((++i) < ARR_SIZE);
    /* cleanUSART1 FIFO */
    delay_1ms(2);
    USART_CleanFifo_RX(USART1);
    USART_CleanFifo_TX(USART1);

    do {
        USART_SendData(USART2, USART2_send_buffer[j]);
        USART1_receive_buffer[j] = USART_ReceiveData(USART1);
    } while ((++j) < ARR_SIZE);
    state0 = MemoryCompare(USART1_send_buffer, USART2_receive_buffer, ARR_SIZE);
    state1 = MemoryCompare(USART1_receive_buffer, USART2_send_buffer, ARR_SIZE);
    if (state0 & state1) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while(1){}

}

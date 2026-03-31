#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 18
#define ARR_SIZE1 27

uint8_t USART1_send_buffer[ARR_SIZE]={ 0x3A ,0X30,0X31,0X30,0X33,0X30,0X30,0X30,0X30,0X30,0X30,0X30,0X34,0X46,0X38,0x0D,0x0A,0x00};
uint8_t USART2_send_buffer[ARR_SIZE1]={ 0x3A ,0X30,0X31,0X30,0X33,0X30,0X38,0X30,0X30,0X30,0X31,0X30,0X30,0X30,0X32,0X30,0X30,0X30,0X33,0X30,0X30,0X30,0X34,0x45,0x41,0x0D,0x0A};
uint8_t USART2_receive_buffer[ARR_SIZE]={0};
uint8_t USART1_receive_buffer[ARR_SIZE1]={0};

uint8_t state2 = 1;

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
   
    if (USART_GetABRStatus(USART1, USART_ADVANCED_STATUS_LFCMF) == SET) {
        USART_ITConfig(USART1, USART_INT_EN_MODBUS_ASCII_MATCH_IRQ_EN, DISABLE);
         
       state2=0;
    }

}

void iomux_config(void)
{
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART1_TX_IOF_OVAL,USART1_TX_PAD_SEL,USART1_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART1_RX_IOF_IVAL,USART1_RX_PAD_SEL,USART1_RX_HS_SEL,0,0); 
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART2_TX_IOF_OVAL,USART2_TX_PAD_SEL,USART2_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART2_RX_IOF_IVAL,USART2_RX_PAD_SEL,USART2_RX_HS_SEL,0,0);
}

void main(void )
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

    __enable_irq();
    ECLIC_Register_IRQ(USART1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    USART1_IRQHandler);

    iomux_config();

    USART_Set_RTU(USART1, DISABLE);
    USART_InitTypeDef usart_init_t = {0};
    USART_StructInit(&usart_init_t);
    /* USART_InitStruct members default value */
    usart_init_t.USART_BaudRate = 115200;
    usart_init_t.USART_WordLength = USART_SETUP_BIT_LENGTH_7;
    usart_init_t.USART_StopBits = USART_SETUP_CFG_STOP_BIT_1BIT;
    usart_init_t.USART_TX_CTL = USART_TXCTRL_TXEN_ENABLE;
    usart_init_t.USART_RX_CTL = USART_RXCTRL_RXEN_ENABLE;
    usart_init_t.USART_Parity = USART_SETUP_PARITY_EN_DISABLE;
    usart_init_t.USART_HardwareFlowControl = USART_SETUP_RTSE_DISABLE | USART_SETUP_CTSE_DISABLE;
    usart_init_t.USART_RX_IDLE_Timeout = 0xFF;
    usart_init_t.USART_RX_WM_Timeout = 0xFF;
    usart_init_t.USART_TX_Data_Size = USART_TX_DATASIZE_NUM_BYTE;
    usart_init_t.USART_RX_Data_Size = USART_RX_DATASIZE_NUM_BYTE;
    /* USART_StructInit(&usart_init_t) */
    USART_Init(USART1, &usart_init_t);
    USART_Init(USART2, &usart_init_t);

    USART_ITConfig(USART1, USART_INT_EN_MODBUS_ASCII_MATCH_IRQ_EN, ENABLE);
    USART_Set_RXSize(USART1,27);
    USART_SetRXAddress(USART1,ENABLE, 0x0a);
    int i = 0, j = 0, state0 = 0, state1 = 0;
    /* clean USART2 FIFO */
    USART_CleanFifo_RX(USART2);
    USART_CleanFifo_TX(USART2);

    do {
        USART_SendData(USART1, USART1_send_buffer[i]);
        USART2_receive_buffer[i] = USART_ReceiveData(USART2);
    } while ((++i) < ARR_SIZE);
    /* cleanUSART1 FIFO */
    USART_CleanFifo_RX(USART1);
    USART_CleanFifo_TX(USART1);

    do {
        USART_SendData(USART2, USART2_send_buffer[j]);
        USART1_receive_buffer[j] = USART_ReceiveData(USART1);
    } while ((++j) < ARR_SIZE1);
    state0 = MemoryCompare(USART1_send_buffer, USART2_receive_buffer, ARR_SIZE);
    state1 = MemoryCompare(USART1_receive_buffer, USART2_send_buffer, ARR_SIZE);
    while (state2){}

    if (state0 & state1) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

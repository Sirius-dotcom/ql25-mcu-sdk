#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 8

uint8_t USART1_send_buffer[ARR_SIZE]={ 0x01 ,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
uint8_t USART2_receive_buffer[ARR_SIZE]={0};
uint8_t USART1_receive_buffer[ARR_SIZE]={0};
uint8_t USART2_send_buffer[ARR_SIZE]={ 0x15 ,0x12,0x13,0x14,0x15,0x16,0x17,0x18};
uint8_t USART1_send_n=0,USART1_receive_n=0,USART2_send_n=0,USART2_receive_n=0,state0=0,state1=0;

/*!
    \brief      memory compare function
    \param[in]  src: source data pointer
    \param[in]  dst: destination data pointer
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/
ErrStatus MemoryCompare(uint8_t* src, uint8_t* dst, uint8_t length)
{
    while(length--){
        if(*src++ != *dst++)
            return ERROR;
    }
    return SUCCESS;
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_STATUS_TXIP) == SET) {
        USART_SendData(USART1, USART1_send_buffer[USART1_send_n++]);
    }
    if (USART_GetITStatus(USART1, USART_STATUS_RXIP) == SET) {
        USART1_receive_buffer[USART1_receive_n++] = USART_ReceiveData(USART1);
    }
    if (ARR_SIZE == USART1_send_n) {
        USART_ITConfig(USART1, USART_INT_EN_TXIE, DISABLE);
    }
    if (ARR_SIZE == USART1_receive_n) {
        USART_ITConfig(USART1, USART_INT_EN_RXIE, DISABLE);
    }
}

void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_STATUS_TXIP) == SET) {
        USART_SendData(USART2,USART2_send_buffer[USART2_send_n++]);
    }
    if (USART_GetITStatus(USART2, USART_STATUS_RXIP) == SET) {
        USART2_receive_buffer[USART2_receive_n++] = USART_ReceiveData(USART2);
    }
    if (ARR_SIZE == USART2_send_n) {
        USART_ITConfig(USART2, USART_INT_EN_TXIE, DISABLE);
    }
    if (ARR_SIZE == USART2_receive_n) {
        USART_ITConfig(USART2, USART_INT_EN_RXIE, DISABLE);
    }
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

    __enable_irq();
    ECLIC_Register_IRQ(USART1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    USART1_IRQHandler);
    ECLIC_Register_IRQ(USART2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    USART2_IRQHandler);
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
    USART_Init(USART1, &usart_init_t);
    USART_Init(USART2, &usart_init_t);

    /* USART set watermark and config tx/rx IE */
    USART_Set_RxWaterMark(USART2, 0);
    USART_ITConfig(USART2, USART_INT_EN_RXIE, ENABLE);
    USART_Set_TxWaterMark(USART1, 1);
    USART_ITConfig(USART1, USART_INT_EN_TXIE, ENABLE);
    USART_Set_RxWaterMark(USART1, 0);
    USART_ITConfig(USART1, USART_INT_EN_RXIE, ENABLE);
    USART_Set_TxWaterMark(USART2, 1);
    USART_ITConfig(USART2, USART_INT_EN_TXIE, ENABLE);

    while (USART1_receive_n < ARR_SIZE ) {}
    while (USART1_send_n < ARR_SIZE ) {}
    while (USART2_receive_n < ARR_SIZE ) {}
    while (USART2_send_n < ARR_SIZE ) {}

    state0 = MemoryCompare(USART1_send_buffer, USART2_receive_buffer, ARR_SIZE);
    state1 = MemoryCompare(USART1_receive_buffer, USART2_send_buffer, ARR_SIZE);
    if (state0 & state1) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

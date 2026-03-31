#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 8

uint8_t USART3_send_buffer[ARR_SIZE]={ 0x01 ,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
uint8_t USART4_receive_buffer[ARR_SIZE]={0};
uint8_t USART3_receive_buffer[ARR_SIZE]={0};
uint8_t USART4_send_buffer[ARR_SIZE]={ 0x15 ,0x12,0x13,0x14,0x15,0x16,0x17,0x18};
uint8_t USART3_send_n=0,USART3_receive_n=0,USART4_send_n=0,USART4_receive_n=0,state0=0,state1=0;

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

void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_STATUS_TXIP) == SET) {
        USART_SendData(USART3, USART3_send_buffer[USART3_send_n++]);
    }
    if (USART_GetITStatus(USART3, USART_STATUS_RXIP) == SET) {
        USART3_receive_buffer[USART3_receive_n++] = USART_ReceiveData(USART3);
    }
    if (ARR_SIZE == USART3_send_n) {
        USART_ITConfig(USART3, USART_INT_EN_TXIE, DISABLE);
    }
    if (ARR_SIZE == USART3_receive_n) {
        USART_ITConfig(USART3, USART_INT_EN_RXIE, DISABLE);
    }
}

void USART4_IRQHandler(void)
{
    if (USART_GetITStatus(USART4, USART_STATUS_TXIP) == SET) {
        USART_SendData(USART4,USART4_send_buffer[USART4_send_n++]);
    }
    if (USART_GetITStatus(USART4, USART_STATUS_RXIP) == SET) {
        USART4_receive_buffer[USART4_receive_n++] = USART_ReceiveData(USART4);
    }
    if (ARR_SIZE == USART4_send_n) {
        USART_ITConfig(USART4, USART_INT_EN_TXIE, DISABLE);
    }
    if (ARR_SIZE == USART4_receive_n) {
        USART_ITConfig(USART4, USART_INT_EN_RXIE, DISABLE);
    }
}

/* USART PAD CONFIG */
void iomux_config(void)
{
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART3_TX_IOF_OVAL,USART3_TX_PAD_SEL,USART3_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART3_RX_IOF_IVAL,USART3_RX_PAD_SEL,USART3_RX_HS_SEL,0,0); 
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART4_TX_IOF_OVAL,USART4_TX_PAD_SEL,USART4_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART4_RX_IOF_IVAL,USART4_RX_PAD_SEL,USART4_RX_HS_SEL,0,0);
}

void main(void)
{
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif
    #ifdef MISC_HAS_USART3_HAS_CLK
    usart3_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_USART3_RST
    usart3_set_rst(DISABLE);
    usart3_set_rst(ENABLE);
    #endif
    
    #ifdef MISC_HAS_USART4_HAS_CLK
    usart4_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_USART4_RST
    usart4_set_rst(DISABLE);
    usart4_set_rst(ENABLE);
    #endif

    iomux_config();

    __enable_irq();
    ECLIC_Register_IRQ(USART3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    USART3_IRQHandler);
    ECLIC_Register_IRQ(USART4_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    USART4_IRQHandler);
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
    USART_Init(USART3, &usart_init_t);
    USART_Init(USART4, &usart_init_t);

    /* USART set watermark and config tx/rx IE */
    USART_Set_RxWaterMark(USART4, 0);
    USART_ITConfig(USART4, USART_INT_EN_RXIE, ENABLE);
    USART_Set_TxWaterMark(USART3, 1);
    USART_ITConfig(USART3, USART_INT_EN_TXIE, ENABLE);
    USART_Set_RxWaterMark(USART3, 0);
    USART_ITConfig(USART3, USART_INT_EN_RXIE, ENABLE);
    USART_Set_TxWaterMark(USART4, 1);
    USART_ITConfig(USART4, USART_INT_EN_TXIE, ENABLE);

    while (USART3_receive_n < ARR_SIZE ) {}
    while (USART3_send_n < ARR_SIZE ) {}
    while (USART4_receive_n < ARR_SIZE ) {}
    while (USART4_send_n < ARR_SIZE ) {}

    state0 = MemoryCompare(USART3_send_buffer, USART4_receive_buffer, ARR_SIZE);
    state1 = MemoryCompare(USART3_receive_buffer, USART4_send_buffer, ARR_SIZE);
    if (state0 & state1) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

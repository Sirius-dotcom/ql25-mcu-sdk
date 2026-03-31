#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 8     
uint8_t USART3_send_buffer[ARR_SIZE]={0x01,0x02,0x13,0x14,0x05,0x08,0x06,0x1a};
uint8_t temp1[ARR_SIZE] = {};
uint8_t USART3_receive_buffer[ARR_SIZE] = {};
uint8_t state0 = 0,state1=1;
/*!
    \brief      memory compare function
    \param[in]  src: source data pointer
    \param[in]  dst: destination data pointer
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/

ErrStatus MemoryCompare(uint8_t* src, uint8_t* dst, int length)
{
    while(length--){
        if(*src++ != *dst++)
            return ERROR;
    }
    return SUCCESS;
}

void USART_SendArray(USART_TypeDef *USARTx,uint8_t* array,int length)
{      
    for (int i = 0; i < length; i++){
        while (USART_GetFlag(USARTx, USART_STATUS_TX_BUSY) == SET) {}
        USART_SendData(USARTx, array[i]);
    }
}

void USART_ReceiveArray(USART_TypeDef *USARTx,int length)
{
    
    for(int j=0;j<length;j++){
        while (USART_GetFlag(USARTx, USART_STATUS_RX_BUSY) == SET) {}
        temp1[j] = USART_ReceiveData(USARTx);
    }
}

void USART_ReceiveArray1(USART_TypeDef *USARTx,int length)
{
    
    for(int k=0;k<length;k++){
        while (USART_GetFlag(USARTx, USART_STATUS_RX_BUSY) == SET) {}
        USART3_receive_buffer[k] = USART_ReceiveData(USARTx);
    }
}

void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3,USART_STATUS_TX_FIFO_EMPTY) == SET)
    {
        USART_CleanFifo_RX(USART4);
        USART_CleanFifo_TX(USART4);
        USART_SendArray(USART3,USART3_send_buffer,ARR_SIZE);
        USART_ITConfig(USART3,USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN,DISABLE);
        USART_ITConfig(USART4,USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN,ENABLE);
    }
}

void USART4_IRQHandler(void)
{     

    if(USART_GetITStatus(USART3,USART_STATUS_TX_FIFO_EMPTY) == SET)
    {
        USART_ReceiveArray(USART4,ARR_SIZE);
        USART_CleanFifo_RX(USART3);
        USART_CleanFifo_TX(USART3);
        USART_SendArray(USART4,temp1,ARR_SIZE);
        USART_ITConfig(USART4,USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN,DISABLE);
        state1 = 0;
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

    USART_ITConfig(USART3,USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN,ENABLE);
    
    while(state1){}
    USART_ReceiveArray1(USART3,ARR_SIZE);
    state0 = MemoryCompare(USART3_send_buffer,USART3_receive_buffer,ARR_SIZE);
    if (state0) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

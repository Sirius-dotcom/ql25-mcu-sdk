#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 8     
uint8_t USART1_send_buffer[ARR_SIZE]={0x01,0x02,0x13,0x14,0x05,0x08,0x06,0x1a};
uint8_t temp1[ARR_SIZE] = {};
uint8_t USART1_receive_buffer[ARR_SIZE] = {};
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
        USART1_receive_buffer[k] = USART_ReceiveData(USARTx);
    }
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1,USART_STATUS_TX_FIFO_EMPTY) == SET)
    {
        USART_CleanFifo_RX(USART2);
        USART_CleanFifo_TX(USART2);
        USART_SendArray(USART1,USART1_send_buffer,ARR_SIZE);
        USART_ITConfig(USART1,USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN,DISABLE);
        USART_ITConfig(USART2,USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN,ENABLE);
    }
}

void USART2_IRQHandler(void)
{     

    if(USART_GetITStatus(USART1,USART_STATUS_TX_FIFO_EMPTY) == SET)
    {
        USART_ReceiveArray(USART2,ARR_SIZE);
        USART_CleanFifo_RX(USART1);
        USART_CleanFifo_TX(USART1);
        USART_SendArray(USART2,temp1,ARR_SIZE);
        USART_ITConfig(USART2,USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN,DISABLE);
        state1 = 0;
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

    USART_ITConfig(USART1,USART_INT_EN_TX_FIFO_EMPTY_IRQ_EN,ENABLE);
    
    while(state1){}
    USART_ReceiveArray1(USART1,ARR_SIZE);
    state0 = MemoryCompare(USART1_send_buffer,USART1_receive_buffer,ARR_SIZE);
    if (state0) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

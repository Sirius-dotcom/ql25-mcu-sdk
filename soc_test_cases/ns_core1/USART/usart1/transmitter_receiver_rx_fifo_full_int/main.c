#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE (16+2)
   
uint8_t USART1_send_buffer[ARR_SIZE]={0x01,0x02,0x03,0x01,0x02,0x03,0x01,0x02,0x03,0x01,0x02,0x03,0x01,0x02,0x03,0x01};
uint8_t temp1[ARR_SIZE] = {};
uint8_t temp[ARR_SIZE] = {};
uint8_t state0 = 1,state1=1;
/*!
    \brief      memory compare function
    \param[in]  src: source data pointer
    \param[in]  dst: destination data pointer
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/

void USART_SendArray(USART_TypeDef *USARTx,uint8_t* array,int length)
{      
    for (int i = 0; i < length; i++) {
        while (USART_GetFlag(USARTx, USART_STATUS_TX_BUSY) == SET) {}
        USART_SendData(USARTx, array[i]);
    }
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1,USART_STATUS_RX_FIFO_FULL) == SET)
    {
        state0 = 0;
        USART_ITConfig(USART1,USART_INT_EN_RX_FIFO_FULL_IRQ_EN,DISABLE);
    }
}

void USART2_IRQHandler(void)
{       
    if(USART_GetITStatus(USART2,USART_STATUS_RX_FIFO_FULL) == SET)
    {
        state1 = 0;
        USART_ITConfig(USART2,USART_INT_EN_RX_FIFO_FULL_IRQ_EN,DISABLE);
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

    USART_ITConfig(USART2,USART_INT_EN_RX_FIFO_FULL_IRQ_EN,ENABLE);
    USART_ITConfig(USART1,USART_INT_EN_RX_FIFO_FULL_IRQ_EN,ENABLE);

    USART_CleanFifo_RX(USART2);
    USART_CleanFifo_TX(USART1);

    USART_SendArray(USART1,USART1_send_buffer,ARR_SIZE);
    USART_SendArray(USART2,USART1_send_buffer,ARR_SIZE);
    
    while(state0|state1){}

    if (state0|state1) {
        simulation_fail();
    } else {
        simulation_pass();
    }

    while (1) {}
}

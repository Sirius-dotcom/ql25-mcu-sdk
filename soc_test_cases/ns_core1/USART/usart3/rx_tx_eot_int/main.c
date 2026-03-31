#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE (3)

uint8_t USART3_send_buffer[ARR_SIZE]={ 0x11 ,0x12,0x13};
uint8_t USART3_receive_buffer[ARR_SIZE]={0};
uint8_t state0=1;
void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_STATUS_RX_EOT|USART_STATUS_TX_EOT)==SET)
    {
        USART_ClearITStatus(USART3, USART_STATUS_RX_EOT|USART_STATUS_TX_EOT);
        state0=0;
    }
}

/* USART PAD CONFIG */
void iomux_config(void)
{
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART3_TX_IOF_OVAL,USART3_TX_PAD_SEL,USART3_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART3_RX_IOF_IVAL,USART3_RX_PAD_SEL,USART3_RX_HS_SEL,0,0); 

}

void main(void )
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
    
    iomux_config();

    __enable_irq();
    ECLIC_Register_IRQ(USART3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    USART3_IRQHandler);

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

    USART_Set_RXSize(USART3,3);
    USART_Set_TXSize(USART3,3);
    /* USART3 overflow irq enable */
    USART_ITConfig(USART3, USART_INT_EN_RX_EOT_IRQ_EN |USART_INT_EN_TX_EOT_IRQ_EN, ENABLE);
    int i = 0;
    do {
        USART_SendData(USART3, USART3_send_buffer[i]);
        USART3_receive_buffer[i] = USART_ReceiveData(USART3);
    } while ((++i) < ARR_SIZE);

    while(state0){}

    if (state0) {
        simulation_fail();
    } else {
         simulation_pass();
    }

    while (1) {}
}

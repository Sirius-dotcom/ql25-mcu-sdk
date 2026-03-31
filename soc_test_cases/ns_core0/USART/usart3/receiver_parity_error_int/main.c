#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 3

uint8_t USART3_send_buffer[ARR_SIZE]={ 0x11 ,0x12,0x13};
uint8_t err_count=0;
uint8_t state=0;

/**
  * \brief USART4 IRQ Handler
  */

void USART4_IRQHandler(void)
{
    if (USART_GetITStatus(USART4, USART_STATUS_RX_ERROR_PARITY) != 0) {
        err_count++;
        USART_ClearITStatus(USART4, USART_STATUS_RX_ERROR_PARITY);
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
    
    #ifdef MISC_HAS_USART4_HAS_CLK
    usart4_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_USART4_RST
    usart4_set_rst(DISABLE);
    usart4_set_rst(ENABLE);
    #endif

    iomux_config();

    __enable_irq();
    ECLIC_Register_IRQ(USART4_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_POSTIVE_EDGE_TRIGGER, 1, 1,
                                    USART4_IRQHandler);

    USART_InitTypeDef usart_init_t = {0};
    USART_StructInit(&usart_init_t);
    usart_init_t.USART_BaudRate = 115200;
    usart_init_t.USART_WordLength = USART_SETUP_BIT_LENGTH_8;
    usart_init_t.USART_StopBits = USART_SETUP_CFG_STOP_BIT_1BIT;
    usart_init_t.USART_TX_CTL = USART_TXCTRL_TXEN_ENABLE;
    usart_init_t.USART_RX_CTL = USART_RXCTRL_RXEN_ENABLE;
    /* USART3 parity even config */
    usart_init_t.USART_Parity = (USART_SETUP_PARITY_EN | USART_SETUP_PARITY_SEL_DISABLE);
    usart_init_t.USART_HardwareFlowControl = USART_SETUP_RTSE_DISABLE | USART_SETUP_CTSE_DISABLE;
    usart_init_t.USART_RX_IDLE_Timeout = 0xFF;
    usart_init_t.USART_RX_WM_Timeout = 0xFF;
    usart_init_t.USART_TX_Data_Size = USART_TX_DATASIZE_NUM_BYTE;
    usart_init_t.USART_RX_Data_Size = USART_RX_DATASIZE_NUM_BYTE;
    USART_Init(USART3, &usart_init_t);
    /* USART4 parity odd config */
    usart_init_t.USART_Parity = (USART_SETUP_PARITY_EN | USART_SETUP_PARITY_SEL);
    USART_Init(USART4, &usart_init_t);
    /* USART4 parity irq enable */
    USART_ITConfig(USART4, USART_INT_EN_PARITY_IRQ_EN,ENABLE);

    for (uint8_t i = 0; i < ARR_SIZE; i++) {
        USART_SendData(USART3, USART3_send_buffer[i]);
        delay_1ms(1);
    }
    while (err_count < ARR_SIZE) {}
    if (err_count == ARR_SIZE) {
        state = 1;
    }
    /* USART4 parity irq disable */
    USART_ITConfig(USART4, USART_INT_EN_PARITY_IRQ_EN,DISABLE);
    if (state) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

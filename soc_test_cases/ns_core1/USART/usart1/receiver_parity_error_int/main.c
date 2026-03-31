#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 3

uint8_t USART1_send_buffer[ARR_SIZE]={ 0x11 ,0x12,0x13};
uint8_t err_count=0;
uint8_t state=0;

/**
  * \brief USART2 IRQ Handler
  */

void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_STATUS_RX_ERROR_PARITY) != 0) {
        err_count++;
        USART_ClearITStatus(USART2, USART_STATUS_RX_ERROR_PARITY);
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

    iomux_config();

    __enable_irq();
    ECLIC_Register_IRQ(USART2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_POSTIVE_EDGE_TRIGGER, 1, 1,
                                    USART2_IRQHandler);

    USART_InitTypeDef usart_init_t = {0};
    USART_StructInit(&usart_init_t);
    usart_init_t.USART_BaudRate = 115200;
    usart_init_t.USART_WordLength = USART_SETUP_BIT_LENGTH_8;
    usart_init_t.USART_StopBits = USART_SETUP_CFG_STOP_BIT_1BIT;
    usart_init_t.USART_TX_CTL = USART_TXCTRL_TXEN_ENABLE;
    usart_init_t.USART_RX_CTL = USART_RXCTRL_RXEN_ENABLE;
    /* USART1 parity even config */
    usart_init_t.USART_Parity = (USART_SETUP_PARITY_EN | USART_SETUP_PARITY_SEL_DISABLE);
    usart_init_t.USART_HardwareFlowControl = USART_SETUP_RTSE_DISABLE | USART_SETUP_CTSE_DISABLE;
    usart_init_t.USART_RX_IDLE_Timeout = 0xFF;
    usart_init_t.USART_RX_WM_Timeout = 0xFF;
    usart_init_t.USART_TX_Data_Size = USART_TX_DATASIZE_NUM_BYTE;
    usart_init_t.USART_RX_Data_Size = USART_RX_DATASIZE_NUM_BYTE;
    USART_Init(USART1, &usart_init_t);
    /* USART2 parity odd config */
    usart_init_t.USART_Parity = (USART_SETUP_PARITY_EN | USART_SETUP_PARITY_SEL);
    USART_Init(USART2, &usart_init_t);
    /* USART2 parity irq enable */
    USART_ITConfig(USART2, USART_INT_EN_PARITY_IRQ_EN,ENABLE);

    for (uint8_t i = 0; i < ARR_SIZE; i++) {
        USART_SendData(USART1, USART1_send_buffer[i]);
        delay_1ms(1);
    }
    while (err_count < ARR_SIZE) {}
    if (err_count == ARR_SIZE) {
        state = 1;
    }
    /* USART2 parity irq disable */
    USART_ITConfig(USART2, USART_INT_EN_PARITY_IRQ_EN,DISABLE);
    if (state) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

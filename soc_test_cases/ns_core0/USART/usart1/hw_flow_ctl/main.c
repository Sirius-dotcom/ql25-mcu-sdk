#include <stdio.h>
#include <stdlib.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE (16+2)

uint8_t USART1_send_buffer[ARR_SIZE]={ 0xa1 ,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa};
uint8_t USART1_receive_buffer[ARR_SIZE]={0};
uint8_t USART2_receive_buffer[ARR_SIZE]={};
uint8_t USART2_send_buffer[ARR_SIZE]={ 0x11 ,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a};
int send_i=0,send_j=0,state0=0,state1=0, rece_i=0,rece_j=0,flag=1;

/**
  * \brief USART2 IRQ Handler
  */
void USART1_IRQHandler(void)
{
    if ((USART_GetITStatus(USART1, USART_STATUS_CTS_FALL_FLAG) == SET)|(USART_GetITStatus(USART1, USART_STATUS_CTS_RISE_FLAG) == SET)) {
        
        USART_ClearITStatus(USART1, USART_STATUS_CTS_RISE_FLAG);
        USART_ClearITStatus(USART1, USART_STATUS_CTS_FALL_FLAG);
        flag=0;
    }
}

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
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART1_TX_IOF_OVAL,USART1_TX_PAD_SEL,USART1_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART1_RX_IOF_IVAL,USART1_RX_PAD_SEL,USART1_RX_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART1_RTS_IOF_OVAL,USART1_RTS_PAD_SEL,USART1_RTS_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART1_CTS_IOF_IVAL,USART1_CTS_PAD_SEL,USART1_CTS_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART2_TX_IOF_OVAL,USART2_TX_PAD_SEL,USART2_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART2_RX_IOF_IVAL,USART2_RX_PAD_SEL,USART2_RX_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART2_RTS_IOF_OVAL,USART2_RTS_PAD_SEL,USART2_RTS_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART2_CTS_IOF_IVAL,USART2_CTS_PAD_SEL,USART2_CTS_HS_SEL,0,0);
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
                                        /* USART cts rise enable */

    USART_InitTypeDef usart_init_t = {0};
    USART_StructInit(&usart_init_t);
    usart_init_t.USART_BaudRate = 115200;
    usart_init_t.USART_WordLength = USART_SETUP_BIT_LENGTH_8;
    usart_init_t.USART_StopBits = USART_SETUP_CFG_STOP_BIT_1BIT;
    usart_init_t.USART_TX_CTL = USART_TXCTRL_TXEN_ENABLE;
    usart_init_t.USART_RX_CTL = USART_RXCTRL_RXEN_ENABLE;
    usart_init_t.USART_Parity = USART_SETUP_PARITY_EN_DISABLE;
    /* USART hardwareflowcontrol rts cts config */
    usart_init_t.USART_HardwareFlowControl = USART_SETUP_RTSE_ENABLE | USART_SETUP_CTSE_ENABLE;
    usart_init_t.USART_RX_IDLE_Timeout = 0xFF;
    usart_init_t.USART_RX_WM_Timeout = 0xFF;
    usart_init_t.USART_TX_Data_Size = USART_TX_DATASIZE_NUM_BYTE;
    usart_init_t.USART_RX_Data_Size = USART_RX_DATASIZE_NUM_BYTE;
    USART_Init(USART1, &usart_init_t);
    USART_Init(USART2, &usart_init_t);

    USART_ITConfig(USART1, (USART_INT_EN_CTSIE_RISE_EN | USART_INT_EN_CTSIE_FALL_EN), ENABLE);

    USART_CleanFifo_TX(USART1);
    USART_CleanFifo_RX(USART2);
    do {
        USART_SendData(USART1, USART1_send_buffer[send_i]);
        send_i++;
    } while((send_i) < ARR_SIZE);

    while (flag) {}
    do {
        USART2_receive_buffer[rece_i] = USART_ReceiveData(USART2);
        rece_i++;
    } while(rece_i < ARR_SIZE) ;
    state0 = MemoryCompare(USART1_send_buffer, USART2_receive_buffer, ARR_SIZE);

    /* USART cts rise disable */
    /* USART cts fall disable */
    USART_ITConfig(USART1, (USART_INT_EN_CTSIE_RISE_EN | USART_INT_EN_CTSIE_FALL_EN), DISABLE);
    USART_CleanFifo_RX(USART2);
    USART_CleanFifo_TX(USART1);
    while(USART_GetTXCTRL_Status(USART1, USART_TXCTRL_CLR)){}
    while(USART_GetRXCTRL_Status(USART2, USART_RXCTRL_CLR)){}
    usart_init_t.USART_HardwareFlowControl = USART_SETUP_RTSE_DISABLE | USART_SETUP_CTSE_DISABLE;
    USART_Init(USART1, &usart_init_t);
    USART_Init(USART2, &usart_init_t);

    if (state0 ) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

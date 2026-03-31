#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE (10)

uint8_t USART3_send_buffer[ARR_SIZE]={ 0x01 ,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
uint8_t USART4_receive_buffer[ARR_SIZE]={0};
uint8_t USART3_receive_buffer[ARR_SIZE]={};
uint8_t USART4_send_buffer[ARR_SIZE]={ 0x11 ,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a};

uint8_t flag=1;

/**
  * \brief USART4 IRQ Handler
  */
void USART3_IRQHandler(void)
{
    if ((USART_GetITStatus(USART3, USART_STATUS_CTS_FALL_FLAG) == SET)|(USART_GetITStatus(USART3, USART_STATUS_CTS_RISE_FLAG) == SET)) {
        
        USART_ClearITStatus(USART3, USART_STATUS_CTS_RISE_FLAG);
        USART_ClearITStatus(USART3, USART_STATUS_CTS_FALL_FLAG);
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
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART3_TX_IOF_OVAL,USART3_TX_PAD_SEL,USART3_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART3_RX_IOF_IVAL,USART3_RX_PAD_SEL,USART3_RX_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART3_RTS_IOF_OVAL,USART3_RTS_PAD_SEL,USART3_RTS_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART3_CTS_IOF_IVAL,USART3_CTS_PAD_SEL,USART3_CTS_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART4_TX_IOF_OVAL,USART4_TX_PAD_SEL,USART4_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART4_RX_IOF_IVAL,USART4_RX_PAD_SEL,USART4_RX_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART4_RTS_IOF_OVAL,USART4_RTS_PAD_SEL,USART4_RTS_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART4_CTS_IOF_IVAL,USART4_CTS_PAD_SEL,USART4_CTS_HS_SEL,0,0);
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
    ECLIC_Register_IRQ(USART3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    USART3_IRQHandler);
                                        /* USART cts rise enable */
    USART_InitTypeDef usart_init_t = {0};
    USART_485InitTypeDef usart_485init_t = {0};
    USART_StructInit(&usart_init_t);
    /* USART_InitStruct members default value */
    usart_init_t.USART_BaudRate = 115200;
    usart_init_t.USART_WordLength = USART_SETUP_BIT_LENGTH_8;
    usart_init_t.USART_StopBits = USART_SETUP_CFG_STOP_BIT_1BIT;
    usart_init_t.USART_TX_CTL = USART_TXCTRL_TXEN_ENABLE;
    usart_init_t.USART_RX_CTL = USART_RXCTRL_RXEN_ENABLE;
    usart_init_t.USART_Parity = USART_SETUP_PARITY_EN_DISABLE;
    usart_init_t.USART_HardwareFlowControl = USART_SETUP_RTSE_DISABLE | USART_SETUP_CTSE_ENABLE;
    usart_init_t.USART_RX_IDLE_Timeout = 0xFF;
    usart_init_t.USART_RX_WM_Timeout = 0xFF;
    usart_init_t.USART_TX_Data_Size = USART_TX_DATASIZE_NUM_BYTE;
    usart_init_t.USART_RX_Data_Size = USART_RX_DATASIZE_NUM_BYTE;
    /* USART_StructInit(&usart_init_t) */
    USART_Init(USART3, &usart_init_t);
    USART_Init(USART4, &usart_init_t);
    usart_485init_t.USART_485EN=USART_SETUP_RS485_EN;
    usart_485init_t.USART_485Start_Early=USART_SETUP_DE_ASSERTION_EARLY_EN;
    usart_485init_t.USART_485Stop_Delay=USART_SETUP_DE_DEASSERTION_DELAY_EN;
    usart_485init_t.USART_485DE=USART_SETUP_DE_INV_DISABLE;
    USART_485Init(USART3, &usart_485init_t);
    USART_485Init(USART4, &usart_485init_t);
    int i = 0, j = 0, state0 = 0, state1 = 0;
    USART_ITConfig(USART3, (USART_INT_EN_CTSIE_RISE_EN | USART_INT_EN_CTSIE_FALL_EN), ENABLE);
    /* cleanUSART4 FIFO */
    /* cleanUSART4 FIFO */
    USART_CleanFifo_RX(USART4);
    USART_CleanFifo_TX(USART3);

    do {
        USART_SendData(USART3, USART3_send_buffer[i]);
        USART4_receive_buffer[i] = USART_ReceiveData(USART4);
    } while ((++i) < ARR_SIZE);

    USART_SendData(USART4, USART4_send_buffer[i]);
    USART_SendData(USART3, USART3_send_buffer[i]);
    /* cleanUSART3 FIFO */
    while(flag){}

    if (flag ) {
        simulation_fail();
    } else {
        simulation_pass();
    }

    while (1) {}
}

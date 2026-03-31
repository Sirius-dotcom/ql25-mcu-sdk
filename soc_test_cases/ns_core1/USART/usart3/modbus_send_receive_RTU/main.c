#include <stdio.h>
#include "ns_sdk_hal.h"

#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 8
#define ARR_SIZE1 25

uint8_t USART3_send_buffer[ARR_SIZE]={ 0x01 ,0x03,0x00,0x00,0x00,0x0a,0xc5,0xcd};
uint8_t USART4_send_buffer[ARR_SIZE1]={ 0x01 ,0X03,0X14,0X00,0X01,0X00,0X02,0X00,0X03,0X00,0X04,0X00,0X05,0X00,0X06,0X00,0X07,0X00,0X08,0X00,0X09,0X00,0X0a,0x8F,0x16};

uint8_t USART4_receive_buffer[ARR_SIZE]={0};
uint8_t USART3_receive_buffer[ARR_SIZE1]={0};

uint8_t state2=1;

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

void USART3_IRQHandler(void)
{

    if (USART_GetITStatus(USART3, USART_STATUS_RX_IDLE_TOUT_STATUS) == SET) {

        USART_ClearITStatus(USART3, USART_STATUS_RX_IDLE_TOUT_STATUS);

        USART_ITConfig(USART3, USART_INT_EN_RX_IDLE_TOUT_IRQ_EN|USART_INT_EN_MODBUS_RTU_TOUT_IRQ_EN, DISABLE);
         
       state2=0;
    }

}

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

    __enable_irq();
    ECLIC_Register_IRQ(USART3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    USART3_IRQHandler);

    iomux_config();
    USART_Set_RTU(USART3, ENABLE);
    USART_InitTypeDef usart_init_t = {0};
    USART_StructInit(&usart_init_t);
    /* USART_InitStruct members default value */
    usart_init_t.USART_BaudRate = 115200;
    usart_init_t.USART_WordLength = USART_SETUP_BIT_LENGTH_8;
    usart_init_t.USART_StopBits = USART_SETUP_CFG_STOP_BIT_1BIT;
    usart_init_t.USART_TX_CTL = USART_TXCTRL_TXEN_ENABLE;
    usart_init_t.USART_RX_CTL = USART_RXCTRL_RXEN_ENABLE;
    usart_init_t.USART_Parity = USART_SETUP_PARITY_EN_DISABLE;
    usart_init_t.USART_HardwareFlowControl = USART_SETUP_RTSE_DISABLE | USART_SETUP_CTSE_DISABLE;
    usart_init_t.USART_RX_IDLE_Timeout = 0x02;
    usart_init_t.USART_RX_WM_Timeout = 0xFF;
    usart_init_t.USART_TX_Data_Size = USART_TX_DATASIZE_NUM_BYTE;
    usart_init_t.USART_RX_Data_Size = USART_RX_DATASIZE_NUM_BYTE;
    /* USART_StructInit(&usart_init_t) */
    USART_Init(USART3, &usart_init_t);
    USART_Init(USART4, &usart_init_t);

    USART_Set_RXSize(USART3,25);
    USART_ITConfig(USART3, USART_INT_EN_RX_IDLE_TOUT_IRQ_EN, ENABLE);
    USART_ITConfig(USART3, USART_INT_EN_MODBUS_RTU_TOUT_IRQ_EN, ENABLE);

    int i = 0, j = 0, state0 = 0, state1 = 0;
    /* cleanUSART4 FIFO */
    USART_CleanFifo_RX(USART4);
    USART_CleanFifo_TX(USART4);

    do {
        USART_SendData(USART3, USART3_send_buffer[i]);
        USART4_receive_buffer[i] = USART_ReceiveData(USART4);
    } while ((++i) < ARR_SIZE);
    /* cleanUSART3 FIFO */
    USART_CleanFifo_RX(USART3);
    USART_CleanFifo_TX(USART3);

    do {
        USART_SendData(USART4, USART4_send_buffer[j]);
        USART3_receive_buffer[j] = USART_ReceiveData(USART3);
    } while ((++j) < ARR_SIZE1);
    state0 = MemoryCompare(USART3_send_buffer, USART4_receive_buffer, ARR_SIZE);
    state1 = MemoryCompare(USART3_receive_buffer, USART4_send_buffer, ARR_SIZE);

    while(state2){}
    if (state0 & state1) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 10

uint8_t USART3_send_buffer[ARR_SIZE]={ 0x01 ,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
uint8_t USART4_receive_buffer[ARR_SIZE]={0};
uint8_t USART3_receive_buffer[ARR_SIZE]={};
uint8_t USART4_send_buffer[ARR_SIZE]={ 0x55 ,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a};
uint32_t baudrateval = 0;

void iomux_config(void)
{
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART3_TX_IOF_OVAL,USART3_TX_PAD_SEL,USART3_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART3_RX_IOF_IVAL,USART3_RX_PAD_SEL,USART3_RX_HS_SEL,0,0); 
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART4_TX_IOF_OVAL,USART4_TX_PAD_SEL,USART4_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART4_RX_IOF_IVAL,USART4_RX_PAD_SEL,USART4_RX_HS_SEL,0,0);
}

static void AutoBauRate_StartBitMethod(void)
{
	uint8_t  tmp[25];
  /* Loop until the end of Autobaudrate phase */
  while(USART_GetABRStatus(USART3, USART_ADVANCED_STATUS_ABRF) == RESET)
  {}
 
  /* If AutoBaudBate error occurred */
  if (USART_GetABRStatus(USART3, USART_ADVANCED_STATUS_ABRE) != RESET)
  {

  }
  else
  {

    #ifdef USE_USART_V3_0_0  
            USART_InitTypeDef usart_init_t ={0};
            USART_StructInit(&usart_init_t);
            baudrateval= usart_init_t.USART_CLK_Feq /((USART3->DIV) +1) ;
    #else
            baudrateval= SystemCoreClock /((USART3->DIV) +1) ;
    #endif

  }
 
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
    usart_init_t.USART_RX_IDLE_Timeout = 0xFF;
    usart_init_t.USART_RX_WM_Timeout = 0xFF;
    usart_init_t.USART_TX_Data_Size = USART_TX_DATASIZE_NUM_BYTE;
    usart_init_t.USART_RX_Data_Size = USART_RX_DATASIZE_NUM_BYTE;
    usart_init_t.USART_Fra_Baud = 0;
    USART_Init(USART4, &usart_init_t);
    usart_init_t.USART_BaudRate = 200000;
	usart_init_t.AutoBaudRateEnable = USART_ADVANCED_SETUP_ABR_EN_ENABLE;
    /*Uncomment your appropriate mode */
    usart_init_t.AutoBaudRateMode = USART_ADVANCED_SETUP_ABR_MOD_0X55; 
    usart_init_t.USART_Fra_Baud = 0;
    /* USART_StructInit(&usart_init_t) */
    USART_Init(USART3, &usart_init_t);

    int i = 0, j = 0, state0 = 0, state1 = 0;
    /* cleanUSART4 FIFO */
    USART_CleanFifo_RX(USART4);
    USART_CleanFifo_TX(USART4);
    for(uint8_t i;i<10;i++){
        USART_SendData(USART4, USART4_send_buffer[i]);
    }

    AutoBauRate_StartBitMethod();
        
    if (abs(baudrateval-115200)<115200*0.05) {
        simulation_pass();
    } else {
        simulation_fail();
    }
   
    while (1) {}
}

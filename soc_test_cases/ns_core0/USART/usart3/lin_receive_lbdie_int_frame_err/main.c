#include <stdio.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 8

uint8_t ReceivePID= 0,state0=1;

uint8_t FrameReceiveOverFlag=0;
uint8_t DataReceiveflag=0;
uint8_t ReceiveCheckSum=0;
uint8_t LinReceiveData[8]={0};
uint8_t DataProcess=0;
 uint8_t DtRProcess=0;
uint8_t Test_Data[8]={0x01,0x02,0x03,0x04,0x11,0x12,0x13,0x14};
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

void Lin_SendBreak(void)
{
	USART_SendBreak(USART3);
}

void Lin_SendSyncSegment(void)
{
	USART_SendData(USART3,0x55);
	while(USART_GetFlag(USART3,USART_STATUS_TX_EOT) == SET );
}

uint8_t Lin_CheckPID(uint8_t id)
{
	uint8_t returnpid =0;
	uint8_t P0 =0;
	uint8_t P1 =0;
	
	P0 = (((id)^(id>>1)^(id>>2)^(id>>4))&0x01)<<6 ;
	P1 = ((~((id>>1)^(id>>3)^(id>>4)^(id>>5)))&0x01)<<7 ;
	
	returnpid = id|P0|P1 ;
	
	return returnpid ;
	
}

void Lin_SendHead(uint8_t id)
{
	Lin_SendBreak();
	Lin_SendSyncSegment();
	USART_SendData(USART3,Lin_CheckPID(id));
	while(USART_GetFlag(USART3,USART_STATUS_TX_EOT) == SET );
}

void USART4_IRQHandler(void)                	
{
    if((USART_LIN_GET_INT(USART4) == USART_ADVANCED_STATUS_LIN_LBDF)&(USART_GetITStatus(USART4, USART_STATUS_RX_IDLE_TOUT_STATUS)==SET))
    {
        USART_ClearAdvancedITStatus(USART4, USART_ADVANCED_STATUS_LIN_LBDF);
        USART_ITConfig(USART4, USART_INT_EN_LIN_LBDIE_IRQ_EN, DISABLE);
	  USART_ITConfig(USART4, USART_INT_EN_FRAME_ERR_IRQ_EN, DISABLE);
        state0=0;
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
	ECLIC_Register_IRQ(USART4_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
	ECLIC_LEVEL_TRIGGER, 1, 2,
	USART4_IRQHandler);

	iomux_config();

	USART_InitTypeDef usart_init_t = {0};
	USART_StructInit(&usart_init_t);
	/* USART_InitStruct members default value */
	usart_init_t.USART_BaudRate = 115200;
	usart_init_t.USART_WordLength = USART_SETUP_BIT_LENGTH_8;
	usart_init_t.USART_StopBits = USART_SETUP_CFG_STOP_BIT_2BIT;
	usart_init_t.USART_TX_CTL = USART_TXCTRL_TXEN_ENABLE;
	usart_init_t.USART_RX_CTL = USART_RXCTRL_RXEN_ENABLE;
	usart_init_t.USART_Parity = USART_SETUP_PARITY_EN_DISABLE;
	usart_init_t.USART_HardwareFlowControl = USART_SETUP_RTSE_DISABLE | USART_SETUP_CTSE_DISABLE;
	usart_init_t.USART_RX_IDLE_Timeout = 0xFF;
	usart_init_t.USART_RX_WM_Timeout = 0xFF;
	usart_init_t.USART_TX_Data_Size = USART_TX_DATASIZE_NUM_BYTE;
	usart_init_t.USART_RX_Data_Size = USART_RX_DATASIZE_NUM_BYTE;
	/* USART_StructInit(&usart_init_t) */
	USART_Init(USART3, &usart_init_t);
	USART_Init(USART4, &usart_init_t);

	USART_LINBreakDetectLengthConfig(USART4,USART_ADVANCED_SETUP_LIN_LBDL_11);          
	USART_LIN_EN(USART3,ENABLE);
	USART_LIN_EN(USART4,ENABLE);
	USART_ITConfig(USART4, USART_INT_EN_FRAME_ERR_IRQ_EN, ENABLE);
	USART_ITConfig(USART4, USART_INT_EN_LIN_LBDIE_IRQ_EN, ENABLE);

	Lin_SendHead(22);

	while (state0) {}

	if (state0) {
		simulation_fail();
	} else {
		simulation_pass();
	}

	while (1) {}
}

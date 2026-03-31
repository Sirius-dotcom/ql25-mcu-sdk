#include <stdio.h>
#include "ns_sdk_hal.h"
#define ARR_SIZE 8

uint8_t ReceivePID= 0;

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

uint8_t Lin_Checksum(uint8_t id , uint8_t data[])
{
	uint8_t t =0;
	uint16_t sum =0;

	sum = data[0];
	if(id == 0x3c)			
	{
		for(t=1;t<8;t++)
		{
			sum += data[t];
			if(sum&0xff00)
			{
				sum&=0x00ff;
				sum+=1;
			}
		}
		sum = ~sum;	
		
		return (uint8_t)sum ;
	}
	
	for(t=1;t<8;t++){
		sum+= data[t];
		if(sum & 0xff00)
		{
			sum&= 0x00ff;
			sum+= 1;
		}
	}
	sum+=Lin_CheckPID(id);

	if(sum&0xff00){
		sum&=0x00ff;
		sum+=1;
	}
	sum = ~sum;	
	return (uint8_t)sum ;
}

void Lin_SentData(uint8_t data[])
{
	uint8_t t =0;
	for(t=0;t<8;t++){
		USART_SendData(USART3,data[t]);
		while(USART_GetFlag(USART3,USART_STATUS_TX_EOT) == SET );
	}
}

void Lin_SendAnswer(uint8_t id ,uint8_t data[])
{
	Lin_SentData(data);
	USART_SendData(USART3,Lin_Checksum(id,data));
	while(USART_GetFlag(USART3,USART_STATUS_TX_EOT) == SET );
}

void Lin_DataProcess(void)
{
	uint8_t ReceiveID =0;
	uint8_t PIDChecksum =0;
	uint8_t SumCheck =0;
	uint8_t state0=0;
	if(DataReceiveflag == 1){
		ReceiveID = ReceivePID&0x3f ;
		PIDChecksum = Lin_CheckPID(ReceiveID);
		if (PIDChecksum != ReceivePID)
		{
			
		}
		else
		{}
		if(FrameReceiveOverFlag == 1)							
		{
			SumCheck = Lin_Checksum(ReceiveID,LinReceiveData);
			if(ReceiveCheckSum != SumCheck)
			{
				simulation_fail();
				while (1) {}
				return ;
			}
			else
			{
				state0=MemoryCompare((uint8_t*)Test_Data, (uint8_t*)LinReceiveData, ARR_SIZE);
				if(state0)
				{
					simulation_pass();
					while (1) {}
				}
				else{
					simulation_fail();
					while (1) {}
				}
				
			}
					
			FrameReceiveOverFlag = 0 ;
			DataReceiveflag = 0 ;
		}
	}
}

void USART4_IRQHandler(void)                	
{
	uint8_t ReceiveData=0;
	uint8_t ReceiveID=0;
      uint8_t testdata[8]={0x01,0x02,0x03,0x04,0x11,0x22,0x33,0x44};
	 if(USART_GetFlag(USART4, USART_STATUS_RXIP) == SET)  
	 {
		ReceiveData =USART_ReceiveData(USART4);	
		if(DataProcess == 0)
		{
			if(ReceiveData != 0x55)
			{
				return ;
			}
			if(ReceiveData == 0x55)
			{
				
				DataProcess = 1 ;
				return ;
			}
		}
		else if(DataProcess == 1)
		{
			ReceivePID = ReceiveData ;
			ReceiveID = ReceivePID&0x3f ;
			if(ReceiveID == 0x33)							
			{
				Lin_SentData(testdata);
				USART_SendData(USART4,Lin_Checksum(ReceiveID,testdata));
				while(USART_GetFlag(USART4,USART_STATUS_TX_EOT) == SET );
				
				DataProcess = 0 ;
				return ;
			}
			DataReceiveflag = 1 ;
			DataProcess = 2 ;
			return ;
		}
		else if(DataProcess == 2)
		{
			if(DtRProcess<8)
			{
				LinReceiveData[DtRProcess] = ReceiveData ;
				DtRProcess ++ ;
				if(DtRProcess == 8)
				{
					DtRProcess = 0 ;
					DataProcess = 3 ;
					return ;
				}
			}
		
		}
		else if(DataProcess == 3)
		{
			ReceiveCheckSum = ReceiveData ;
			FrameReceiveOverFlag = 1 ;
			DataProcess = 0 ;
		}
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
	
	USART_ITConfig(USART4, USART_INT_EN_RXIE, ENABLE);
	
	/***************master*********************/
	Lin_SendHead(22);
	Lin_SendAnswer(22,Test_Data);

	while(FrameReceiveOverFlag==0){}
	Lin_DataProcess();

	while (1) {}
}

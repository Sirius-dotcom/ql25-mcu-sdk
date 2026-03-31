#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ns_sdk_hal.h"

#define ARR_SIZE 10
#define SRC_DATA_ACC_ADDR_BASE           (0x8000000 + 32768 - 0x200)
#define DES_DATA_ACC_ADDR_BASE           (0x8000000 + 32768 - 0x100)

uint32_t USART3_send_buffer[ARR_SIZE]={ 0x11 ,0x12,0x13,0x14,0x15,0x16,0x17,0x8,0x19,0x1a};
uint32_t USART4_receive_buffer[ARR_SIZE]={ 0};
uint8_t USART3_receive_buffer[ARR_SIZE]={};
uint8_t USART4_send_buffer[ARR_SIZE]={ 0xB1 ,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBa};
uint8_t err_count=0;
uint8_t state0=0,state1=0;

volatile int  send_flag = 0;
volatile  int receive_flag = 0;

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
    iomux_ls_iof_oval_cfg(IOMUX_BASE,USART4_TX_IOF_OVAL,USART4_TX_PAD_SEL,USART4_TX_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,USART4_RX_IOF_IVAL,USART4_RX_PAD_SEL,USART4_RX_HS_SEL,0,0);
}

/**
 * \brief UDMA IRQ Handler
 */
void UDMA0_IRQHandler(void)
{
    if (UDMA_PA2M_GetITStatus(USART4_USART_DMA_RX_DMA_IRQ, PA2M_FTRANS_IRQ_STAT)) {
        UDMA_PA2M_ClearITStatus(USART4_USART_DMA_RX_DMA_IRQ, PA2M_FTRANS_IRQ_CLEAR_STAT);
        receive_flag=1;
    } 
}

int main(void)
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
    #ifdef MISC_HAS_UDMA0_HAS_CLK
    udma0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_UDMA0_RST
    udma0_set_rst(DISABLE);
    udma0_set_rst(ENABLE);
    #endif
    
    #ifdef MISC_HAS_USART4_HAS_CLK
    usart4_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_USART4_RST
    usart4_set_rst(DISABLE);
    usart4_set_rst(ENABLE);
    #endif
    #ifdef MISC_HAS_UDMA0_HAS_CLK
    udma0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_UDMA0_RST
    udma0_set_rst(DISABLE);
    udma0_set_rst(ENABLE);
    #endif
    iomux_config();
    uint8_t *dst_base_addr = (uint8_t *)(DES_DATA_ACC_ADDR_BASE);
    uint8_t *src_base_addr = (uint8_t *)(SRC_DATA_ACC_ADDR_BASE);
    memset((uint8_t *)dst_base_addr, 0, ARR_SIZE*4 );
    memcpy((uint8_t *)src_base_addr, (uint8_t *)USART3_send_buffer, ARR_SIZE*4);

    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt UDMA0 */
    ECLIC_Register_IRQ(UDMA0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    UDMA0_IRQHandler);
    
    /*Enable UMDA transmission interrupt in PA2M mode*/
    UDMA_PA2M_ITConfig(USART4_USART_DMA_RX_DMA_IRQ, PA2M_FTRANS_IRQ_EN, ENABLE);
    
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
    usart_init_t.USART_TX_Data_Size = USART_TX_DATASIZE_NUM_WORD;
    usart_init_t.USART_RX_Data_Size = USART_RX_DATASIZE_NUM_WORD;
    usart_init_t.USART_DMA_EN = USART_SETUP_DMA_EN_ENABLE;
    USART_Init(USART3, &usart_init_t);
    usart_init_t.USART_RX_DMA_EN = USART_SETUP_RX_DMA_EN_ENABLE;
    USART_Init(USART4, &usart_init_t);

    UDMA_PAM2MTypeDef UDMA_PAM2MStruct = {0};
    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(&(USART3->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(src_base_addr);

#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(&(USART3->TXDATA));
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(src_base_addr);
#endif

    UDMA_PAM2MStruct.UDMA_BufferSize = ARR_SIZE*4;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_32BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_USART3_USART_DMA_TX;  
    UDMA_PAM2M_Init(USART3_USART_DMA_TX_DMA_CH, &UDMA_PAM2MStruct);

    UDMA_PAM2MStruct.UDMA_TransEn = PA2M_TRANS_ENABLE;
    UDMA_PAM2MStruct.UDMA_DstBaseAddr = CAL_ADDR_LO(dst_base_addr);
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr = CAL_ADDR_LO(&(USART4->RXDATA));

#if defined(SOC_ADDRESS_HIGH_VALID) && SOC_ADDRESS_HIGH_VALID == 1
    UDMA_PAM2MStruct.UDMA_DstBaseAddr_H = CAL_ADDR_HI(dst_base_addr) ;
    UDMA_PAM2MStruct.UDMA_SrcBaseAddr_H = CAL_ADDR_HI(&(USART4->RXDATA)) ;
#endif

    UDMA_PAM2MStruct.UDMA_BufferSize = ARR_SIZE*4;
    UDMA_PAM2MStruct.UDMA_DstInc = PA2M_MDNA_ENABLE;
    UDMA_PAM2MStruct.UDMA_SrcInc = PA2M_MSNA_DISABLE;
    UDMA_PAM2MStruct.UDMA_Width = PA2M_MDWIDTH_32BIT;
    UDMA_PAM2MStruct.UDMA_Mode = PA2M_MODE_NORMAL;
    UDMA_PAM2MStruct.UDMA_PER_SEL = UDMA_SEL_USART4_USART_DMA_RX;
    UDMA_PAM2M_Init(USART4_USART_DMA_RX_DMA_CH, &UDMA_PAM2MStruct);

    while(USART_GetFlag(USART3,USART_STATUS_TX_BUSY)==1){};
    while(receive_flag==0){};
    
    memcpy((uint8_t *)USART4_receive_buffer, (uint8_t *)dst_base_addr, ARR_SIZE );
    state0 = MemoryCompare((uint8_t*)USART3_send_buffer, (uint8_t*)USART4_receive_buffer, ARR_SIZE);
    
    if (state0 ) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while (1) {}
}

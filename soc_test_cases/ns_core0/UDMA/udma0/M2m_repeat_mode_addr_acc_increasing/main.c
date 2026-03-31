#include <stdio.h>
#include "nuclei_sdk_hal.h"

#define UDMA0_REPEAT_TRAN_NUMS    4
#define UDMA0_TRANSFER_BYTE_NUMS  64    

#define SRC_DATA_ACC_ADDR_BASE           (0x8000000 + 32768 - 0x200)
#define DES_DATA_ACC_ADDR_BASE           (0x8000000 + 32768 - 0x100)

static uint8_t __attribute__ ((aligned (64))) src_buffer_8[256] = {0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,
                            0x12,0x5a,0xa5,0xaa,0x55,0xa5,0x12,0x56,0x34,0x78,0x12,0x78,0x12,0x78,0x34,0x78,};
static uint8_t __attribute__ ((aligned (64))) dst_buffer_8[256] = {0};

volatile uint8_t state1 = 0;
volatile uint8_t state2 = 0;
void UDMA0_IRQHandler(void)
{
    static uint32_t intCnt = 0;
    
    if (UDMA_GetITStatus(UDMA0_CH0_M2M_IRQ, UDMA_FTRANS_IRQ) == SET) {
        UDMA_ClearITStatus(UDMA0_CH0_M2M_IRQ, UDMA_FTRANS_IRQ);
        for (int i = 0; i < UDMA0_TRANSFER_BYTE_NUMS; i++) {
            if (((uint8_t*)(CAL_ADDR(SRC_DATA_ACC_ADDR_BASE)))[i + intCnt * UDMA0_TRANSFER_BYTE_NUMS] != ((uint8_t*)(CAL_ADDR(DES_DATA_ACC_ADDR_BASE)))[i + intCnt * UDMA0_TRANSFER_BYTE_NUMS]) {
                printf("des[%d]: %x\r\n", i, ((uint8_t*)(CAL_ADDR(DES_DATA_ACC_ADDR_BASE)))[i + intCnt * UDMA0_TRANSFER_BYTE_NUMS]);
                state1 = 1;
                state2 = 1;
                UDMA_ITConfig(UDMA0_CH0_M2M_IRQ, UDMA_FTRANS_IRQ, DISABLE);
                M2M_DMA_Cmd(UDMA0_M2M_CH0, DISABLE);
            }
        }
        intCnt++;

        if (intCnt >= UDMA0_REPEAT_TRAN_NUMS) {
            UDMA_ITConfig(UDMA0_CH0_M2M_IRQ, UDMA_FTRANS_IRQ, DISABLE);
            M2M_DMA_Cmd(UDMA0_M2M_CH0, DISABLE); 
            state1 = 1;
        } else {
            M2M_DMA_Cmd(UDMA0_M2M_CH0, ENABLE);   
        }
    } else if (UDMA_GetITStatus(UDMA0_CH0_M2M_IRQ, UDMA_HTRANS_IRQ) == SET) {
        UDMA_ClearITStatus(UDMA0_CH0_M2M_IRQ, UDMA_HTRANS_IRQ);
    } else if (UDMA_GetITStatus(UDMA0_CH0_M2M_IRQ, UDMA_ERR_IRQ) == SET) {
        UDMA_ClearITStatus(UDMA0_CH0_M2M_IRQ, UDMA_ERR_IRQ);
    }
}

void memory_init()
{
    int i = 0;
    for (; i < 256; i++) {
        REG8(CAL_ADDR(SRC_DATA_ACC_ADDR_BASE + i)) = src_buffer_8[i];  
    }
}

void main(void)
{
    #ifdef MISC_HAS_UDMA0_HAS_CLK
    udma0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_UDMA0_RST
    udma0_set_rst(DISABLE);
    udma0_set_rst(ENABLE);
    #endif
#ifdef CFG_SIMULATION
    #ifdef MISC_HAS_UDMA0_CLK_DIV
    udma0_clk_div(10);
    #endif
#endif
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    MFlushInvalDCache();
    #endif
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif
    int32_t retVal = 0;

    memory_init();
    UDMA_InitTypeDef  UDMA_InitStruct;
    UDMA_StructInit(&UDMA_InitStruct);
    UDMA_InitStruct.UDMA_DstBaseAddr = (uint32_t)(CAL_ADDR(DES_DATA_ACC_ADDR_BASE));
    UDMA_InitStruct.UDMA_SrcBaseAddr = (uint32_t)(CAL_ADDR(SRC_DATA_ACC_ADDR_BASE));
    UDMA_InitStruct.UDMA_DstBaseAddr_h = 0x0;
    UDMA_InitStruct.UDMA_SrcBaseAddr_h = 0x0;
    UDMA_InitStruct.UDMA_BufferSize = UDMA0_TRANSFER_BYTE_NUMS;
    UDMA_InitStruct.UDMA_DstInc = UDMA_MDNA_ENABLE;
    UDMA_InitStruct.UDMA_SrcInc = UDMA_MSNA_ENABLE;
    UDMA_InitStruct.UDMA_SrcWidth = UDMA_MSWIDTH_32BIT;
    UDMA_InitStruct.UDMA_DstWidth = UDMA_MDWIDTH_8BIT;
    UDMA_InitStruct.UDMA_Mode = UDMA_MODE_REPEAT;
    UDMA_InitStruct.UDMA_Priority = UDMA_PRIORITY_HIGH;
    /* Global interrupt enable*/
    __enable_irq();
    /*register interrupt UDMA_IRQn */
    retVal = ECLIC_Register_IRQ(UDMA0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                                  ECLIC_LEVEL_TRIGGER, 1, 0,
                                                  UDMA0_IRQHandler);
    if (retVal == -1) {
        simulation_fail();
        while (1);
    }
    UDMA_Init(UDMA0_M2M_CH0, &UDMA_InitStruct);
    /* Every transmission, moving the data from the same source address to same destination address */
    UDMA_RptConfig(UDMA0_M2M_CH0, UDMA0_REPEAT_TRAN_NUMS, UDMA_RPT_SAUM_ADD_MODE, 0, UDMA_RPT_DAUM_ADD_MODE, 0);
    UDMA_ITConfig(UDMA0_CH0_M2M_IRQ, UDMA_FTRANS_IRQ, ENABLE);
    M2M_DMA_Cmd(UDMA0_M2M_CH0, ENABLE);
    
    while (!state1) {};

    if (state2) {
        simulation_fail();
    } else {
        simulation_pass();
    }
    
    while (1);
}

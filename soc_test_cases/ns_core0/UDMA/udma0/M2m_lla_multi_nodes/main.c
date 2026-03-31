#include <stdio.h>
#include "nuclei_sdk_hal.h"

#define SRC_DATA_ACC_ADDR_BASE           (0x8000000 + 32768 - 0x200)
#define DES_DATA_ACC_ADDR_BASE           (0x8000000 + 32768 - 0x100)

#define LLA_SRC_DATA_ACC_ADDR_BASE       (0x8000000 + 32768 - 0x400)
#define LLA_DES_DATA_ACC_ADDR_BASE       (0x8000000 + 32768 - 0x300)
#define LLA_CONFIG_ADDR_BASE             (0x8000000 + 32768 - 0x500)
#define LLA2_DES_DATA_ACC_ADDR_BASE      (0x8000000 + 32768 - 0x600)
#define LLA2_CONFIG_ADDR_BASE            (0x8000000 + 32768 - 0x700)
#define LLA3_DES_DATA_ACC_ADDR_BASE      (0x8000000 + 32768 - 0x800)
#define LLA3_CONFIG_ADDR_BASE            (0x8000000 + 32768 - 0x900)
UDMA_CHx_llaCfg_TypeDef __attribute__ ((aligned (4))) *s_lla_cfg = ((UDMA_CHx_llaCfg_TypeDef*)(LLA_CONFIG_ADDR_BASE));
UDMA_CHx_llaCfg_TypeDef __attribute__ ((aligned (4))) *s_lla2_cfg = ((UDMA_CHx_llaCfg_TypeDef*)(LLA2_CONFIG_ADDR_BASE));
UDMA_CHx_llaCfg_TypeDef __attribute__ ((aligned (4))) *s_lla3_cfg = ((UDMA_CHx_llaCfg_TypeDef*)(LLA3_CONFIG_ADDR_BASE));

#define UDMA0_TRANSFER_BYTE_NUMS  256
#define UDMA0_REPEAT_TRAN_NUMS    10

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

static uint8_t __attribute__ ((aligned (64))) src_buffer_lla_8[256] = {0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,
                            0x22,0x7a,0xa1,0xa4,0x55,0xa2,0x02,0x86,0x94,0x48,0x32,0x58,0x22,0x58,0x64,0xc8,};
static uint8_t __attribute__ ((aligned (64))) dst_buffer_lla_8[256] = {0};
static uint8_t __attribute__ ((aligned (64))) dst_buffer_lla2_8[256] = {0};
static uint8_t __attribute__ ((aligned (64))) dst_buffer_lla3_8[256] = {0};
volatile uint8_t ftrans_flg = 0;
volatile uint8_t lla_ftrans_flg = 0;
void UDMA0_IRQHandler(void)
{
    if (UDMA_GetITStatus(UDMA0_CH0_M2M_IRQ, UDMA_FTRANS_IRQ) == SET) {
        UDMA_ClearITStatus(UDMA0_CH0_M2M_IRQ, UDMA_FTRANS_IRQ);
        UDMA_ClearITStatus(UDMA0_CH0_M2M_IRQ, UDMA_HTRANS_IRQ);
        
        ftrans_flg++;
    } else if (UDMA_GetITStatus(UDMA0_CH0_M2M_IRQ, UDMA_LLA_FTRANS_IRQ) == SET) {
        UDMA_ClearITStatus(UDMA0_CH0_M2M_IRQ, UDMA_LLA_FTRANS_IRQ);
        UDMA_ITConfig(UDMA0_CH0_M2M_IRQ, UDMA_LLA_FTRANS_IRQ, DISABLE);
        UDMA_LinkListConfig(UDMA0_M2M_CH0, 0, 0);
        
        lla_ftrans_flg++;
    } else if (UDMA_GetITStatus(UDMA0_CH0_M2M_IRQ, UDMA_ERR_IRQ) == SET) {
        UDMA_ClearITStatus(UDMA0_CH0_M2M_IRQ, UDMA_ERR_IRQ);
    }
}

void memory_init()
{
    int i = 0;
    for (; i < UDMA0_TRANSFER_BYTE_NUMS; i++) {
        REG8(CAL_ADDR(SRC_DATA_ACC_ADDR_BASE + i)) = src_buffer_8[i];
        REG8(CAL_ADDR(LLA_SRC_DATA_ACC_ADDR_BASE + i)) = src_buffer_lla_8[i];  
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

    UDMA_InitStruct.UDMA_BufferSize = UDMA0_TRANSFER_BYTE_NUMS;
    UDMA_InitStruct.UDMA_DstInc = UDMA_MDNA_ENABLE;
    UDMA_InitStruct.UDMA_SrcInc = UDMA_MSNA_ENABLE;
    UDMA_InitStruct.UDMA_SrcWidth = UDMA_MSWIDTH_32BIT;
    UDMA_InitStruct.UDMA_DstWidth = UDMA_MDWIDTH_8BIT;
    UDMA_InitStruct.UDMA_Mode = UDMA_MODE_REPEAT;
    UDMA_InitStruct.UDMA_Priority = UDMA_PRIORITY_HIGH;
    UDMA_InitStruct.UDMA_DstBaseAddr = (uint32_t)(CAL_ADDR(DES_DATA_ACC_ADDR_BASE));
    UDMA_InitStruct.UDMA_SrcBaseAddr = (uint32_t)(CAL_ADDR(SRC_DATA_ACC_ADDR_BASE));
    UDMA_InitStruct.UDMA_DstBaseAddr_h = 0x0;
    UDMA_InitStruct.UDMA_SrcBaseAddr_h = 0x0;
    UDMA_LinkListConfig(UDMA0_M2M_CH0, ADDR32(CAL_ADDR(LLA_CONFIG_ADDR_BASE)), (uint64_t)(LLA_CONFIG_ADDR_BASE) >> 32);

    s_lla_cfg->MSRCADDR = (uint32_t)(CAL_ADDR(LLA_SRC_DATA_ACC_ADDR_BASE));
    s_lla_cfg->MDSTADDR = (uint32_t)(CAL_ADDR(LLA_DES_DATA_ACC_ADDR_BASE));
    s_lla_cfg->MCTRL = (UDMA_TRANS_ENABLE | UDMA_MODE_REPEAT | UDMA_PRIORITY_HIGH | UDMA_MDNA_ENABLE | UDMA_MSNA_ENABLE | UDMA_MSWIDTH_32BIT | UDMA_MDWIDTH_32BIT);
    s_lla_cfg->RPT = UDMA0_REPEAT_TRAN_NUMS | UDMA_RPT_SAUM_SUB_MODE | UDMA_RPT_DAUM_SUB_MODE;
    s_lla_cfg->MSIZE = UDMA0_TRANSFER_BYTE_NUMS;
    s_lla_cfg->MRAU = UDMA_CFG_MRAU_RDAU_VAL(UDMA0_TRANSFER_BYTE_NUMS) | UDMA_CFG_MRAU_RSAU_VAL(UDMA0_TRANSFER_BYTE_NUMS);
    s_lla_cfg->MLLA = (uint32_t)(CAL_ADDR(LLA2_CONFIG_ADDR_BASE));
    
    s_lla2_cfg->MSRCADDR = (uint32_t)(CAL_ADDR(LLA_SRC_DATA_ACC_ADDR_BASE));
    s_lla2_cfg->MDSTADDR = (uint32_t)(CAL_ADDR(LLA2_DES_DATA_ACC_ADDR_BASE));
    s_lla2_cfg->MCTRL = (UDMA_TRANS_ENABLE | UDMA_MODE_REPEAT | UDMA_PRIORITY_HIGH | UDMA_MDNA_ENABLE | UDMA_MSNA_ENABLE | UDMA_MSWIDTH_32BIT | UDMA_MDWIDTH_32BIT);
    s_lla2_cfg->RPT = UDMA0_REPEAT_TRAN_NUMS | UDMA_RPT_SAUM_SUB_MODE | UDMA_RPT_DAUM_SUB_MODE;
    s_lla2_cfg->MSIZE = UDMA0_TRANSFER_BYTE_NUMS;
    s_lla2_cfg->MRAU = UDMA_CFG_MRAU_RDAU_VAL(UDMA0_TRANSFER_BYTE_NUMS) | UDMA_CFG_MRAU_RSAU_VAL(UDMA0_TRANSFER_BYTE_NUMS);
    s_lla2_cfg->MLLA = (uint32_t)(CAL_ADDR(LLA3_CONFIG_ADDR_BASE));

    s_lla3_cfg->MSRCADDR = (uint32_t)(CAL_ADDR(LLA_SRC_DATA_ACC_ADDR_BASE));
    s_lla3_cfg->MDSTADDR = (uint32_t)(CAL_ADDR(LLA3_DES_DATA_ACC_ADDR_BASE));
    s_lla3_cfg->MCTRL = (UDMA_TRANS_ENABLE | UDMA_MODE_REPEAT | UDMA_PRIORITY_HIGH | UDMA_MDNA_ENABLE | UDMA_MSNA_ENABLE | UDMA_MSWIDTH_32BIT | UDMA_MDWIDTH_32BIT);
    s_lla3_cfg->RPT = UDMA0_REPEAT_TRAN_NUMS | UDMA_RPT_SAUM_SUB_MODE | UDMA_RPT_DAUM_SUB_MODE;
    s_lla3_cfg->MSIZE = UDMA0_TRANSFER_BYTE_NUMS;
    s_lla3_cfg->MRAU = UDMA_CFG_MRAU_RDAU_VAL(UDMA0_TRANSFER_BYTE_NUMS) | UDMA_CFG_MRAU_RSAU_VAL(UDMA0_TRANSFER_BYTE_NUMS);
    s_lla3_cfg->MLLA = 0;
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
    UDMA_RptConfig(UDMA0_M2M_CH0, UDMA0_REPEAT_TRAN_NUMS, UDMA_RPT_SAUM_SUB_MODE, UDMA0_TRANSFER_BYTE_NUMS, UDMA_RPT_DAUM_SUB_MODE, UDMA0_TRANSFER_BYTE_NUMS);
    UDMA_RptITConfig(UDMA0_M2M_CH0, ENABLE);
    UDMA_ITConfig(UDMA0_CH0_M2M_IRQ, UDMA_FTRANS_IRQ | UDMA_LLA_FTRANS_IRQ, ENABLE);
    M2M_DMA_Cmd(UDMA0_M2M_CH0, ENABLE);
    
    while (!ftrans_flg) {};
    ftrans_flg = 0;
    for (int i = 0; i < UDMA0_TRANSFER_BYTE_NUMS; i++) {
        if (((uint8_t*)(CAL_ADDR(SRC_DATA_ACC_ADDR_BASE)))[i] != ((uint8_t*)(CAL_ADDR(DES_DATA_ACC_ADDR_BASE)))[i]) {
            printf("des[%d]: %x\r\n", i, ((uint8_t*)(CAL_ADDR(DES_DATA_ACC_ADDR_BASE)))[i]);
            simulation_fail();
            while (1);
        }
    }
#ifndef CFG_SIMULATION
    printf("repeat done!\r\n");
#endif

    while ((!lla_ftrans_flg) || (M2M_DMA_Get_Mctrl_Field_Val(UDMA0_M2M_CH0, UDMA_TRANS_ENABLE))) {};
    for (int i = 0; i < UDMA0_TRANSFER_BYTE_NUMS; i++) {
        if (((uint8_t*)(CAL_ADDR(LLA_SRC_DATA_ACC_ADDR_BASE)))[i] != ((uint8_t*)(CAL_ADDR(LLA_DES_DATA_ACC_ADDR_BASE)))[i]) {
            printf("lla des[%d]: %x\r\n", i, ((uint8_t*)(CAL_ADDR(LLA_DES_DATA_ACC_ADDR_BASE)))[i]);
            simulation_fail();
            while (1);
        }
    }

    for (int i = 0; i < UDMA0_TRANSFER_BYTE_NUMS; i++) {
        if (((uint8_t*)(CAL_ADDR(LLA_SRC_DATA_ACC_ADDR_BASE)))[i] != ((uint8_t*)(CAL_ADDR(LLA2_DES_DATA_ACC_ADDR_BASE)))[i]) {
            printf("lla2 des[%d]: %x\r\n", i, ((uint8_t*)(CAL_ADDR(LLA2_DES_DATA_ACC_ADDR_BASE)))[i]);
            simulation_fail();
            while (1);
        }
    }

    for (int i = 0; i < UDMA0_TRANSFER_BYTE_NUMS; i++) {
        if (((uint8_t*)(CAL_ADDR(LLA_SRC_DATA_ACC_ADDR_BASE)))[i] != ((uint8_t*)(CAL_ADDR(LLA3_DES_DATA_ACC_ADDR_BASE)))[i]) {
            printf("lla3 des[%d]: %x\r\n", i, ((uint8_t*)(CAL_ADDR(LLA3_DES_DATA_ACC_ADDR_BASE)))[i]);
            simulation_fail();
            while (1);
        }
    }

    if ((ftrans_flg > UDMA0_REPEAT_TRAN_NUMS * 3) || (lla_ftrans_flg > 1)) {
        simulation_fail();
    } else {
        simulation_pass();
    }
    while (1);
}

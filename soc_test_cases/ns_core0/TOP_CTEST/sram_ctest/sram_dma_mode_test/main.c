#include "ns.h"
#include "ns_sdk_hal.h"
#include <stdio.h>
#include "nuclei_sdk_hal.h"
#include "sram_dma_info.h"
#include <stdarg.h>
#include <string.h>
int level = 0;

void debug_printf(int flags,const char *fmt,...)
{
    if(!flags){
        return; 
    }
    va_list args;
    va_start(args,fmt);
    printf(fmt,args);
    va_end(args);
}
#if defined(SRAM_NUM) && defined(UDMA_NUM)
uint32_t src_buffer_32[2] = {0x12345678,0x5a5a5a5a};
uint32_t dst_base_addr;
uint32_t src_base_addr;
int tran_flag=0;
UDMA_InitTypeDef  UDMA_InitStruct = {0};
#define CSR_CCM_MBEGINADDR        0x7CB
#define CSR_CCM_MCOMMAND          0x7CC
void SRAM_MFlushInvalDCache(uint32_t addr)
{
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    __RV_CSR_WRITE(CSR_CCM_MBEGINADDR, addr);
    __RV_CSR_WRITE(CSR_CCM_MCOMMAND, 0x02);
#endif
}

void UDMA0_IRQHandler(void)
{
    int count;
    if(UDMA_GetITStatus(UDMA0_CH0_M2M_IRQ,UDMA_FTRANS_IRQ)==SET)
    {
        UDMA_ClearITStatus(UDMA0_CH0_M2M_IRQ,UDMA_FTRANS_IRQ);
        for(count=0; count<2; count++)
        {
            if(*( volatile uint32_t* )(dst_base_addr+count*4) != *( volatile uint32_t* )(src_base_addr+count*4))
            {
                debug_printf(level,"0x%x:%x 0x%x:%x\n",src_base_addr, *(volatile uint32_t*)(src_base_addr+ count*4),dst_base_addr,*( volatile uint32_t*)(dst_base_addr+ count*4));
                while(1);
            }
        }
        tran_flag=1;
    }
    else if(UDMA_GetITStatus(UDMA0_CH0_M2M_IRQ,UDMA_HTRANS_IRQ)==SET)
    {
        UDMA_ClearITStatus(UDMA0_CH0_M2M_IRQ,UDMA_HTRANS_IRQ); 
        for(count=0; count<2; count++)
        {
            if(*( volatile uint32_t* )(dst_base_addr+count*4) != *( volatile uint32_t* )(src_base_addr+count*4))
            {
                debug_printf(level,"0x%x:%x 0x%x:%x\n",src_base_addr, *(volatile uint32_t*)(src_base_addr+ count*4),dst_base_addr,*(   volatile uint32_t*)(dst_base_addr+ count*4));
                while(1);
            }
        }
        tran_flag=1;
    }
    else
    {
        UDMA_ClearITStatus(UDMA0_CH0_M2M_IRQ,UDMA_ERR_IRQ);
        debug_printf(level,"UDMA ERROR\n");
        tran_flag=1;   
    }
}

int buf_sram_tran(int flag)
{
    int i,j,count;
    
    debug_printf(level,"buf and sram tran\n");
    src_base_addr = (uint32_t)src_buffer_32;
    
    if(flag == 0)
        debug_printf(level,"start\n");
    if(flag == 1)
        debug_printf(level,"end\n");
    for(i=0; i<SRAM_NUM; i++ )
    {
        if(flag == 0)
            dst_base_addr = (uint32_t)(sram_base[i]);
        if(flag == 1)
            dst_base_addr = (uint32_t)(sram_base[i] + sram_size[i] - 63);
        UDMA_InitStruct.UDMA_DstBaseAddr = dst_base_addr;
        UDMA_InitStruct.UDMA_SrcBaseAddr = src_base_addr;
        for(j=0; j<UDMA_NUM; j++)
        {
            tran_flag = 0;
            memset((void*)dst_base_addr, 0, 8);
            
            UDMA_Init((UDMA_CHxCfg_TypeDef *)(udma_base[j] + 0x8),&UDMA_InitStruct);
            UDMA_ITConfig((UDMA_CHx_IRQ_TypeDef *)(udma_base[j] + 0x800),UDMA_HTRANS_IRQ ,ENABLE);
            M2M_DMA_Cmd((UDMA_CHxCfg_TypeDef *)(udma_base[j] + 0x8),ENABLE);
            while(!tran_flag);
        }
    }

    return 0;
}

int sram_sram_tran(int flag)
{
    int i,j,count;
    debug_printf(level,"sram to sram\n");

    if(flag == 0)
        debug_printf(level,"start\n");
    if(flag == 1)
        debug_printf(level,"end\n");
    for(i=0; i<SRAM_NUM-1; i++ )
        {
        if(flag == 0)
        {
            src_base_addr = (uint32_t)(sram_base[i]);
            dst_base_addr = (uint32_t)(sram_base[i+1]);
        }
        if(flag == 1)
        {
            src_base_addr = (uint32_t)(sram_base[i] + sram_size[i] - 63);
            dst_base_addr = (uint32_t)(sram_base[i+1] +sram_size[i+1] - 63);
        }
        
        UDMA_InitStruct.UDMA_SrcBaseAddr = src_base_addr;
        UDMA_InitStruct.UDMA_DstBaseAddr = dst_base_addr;
        *( volatile uint32_t* )(src_base_addr)=0x5a5a5a5a;
        *( volatile uint32_t* )(src_base_addr + 4)=0x12345678;
        
        for(j=0; j<UDMA_NUM; j++)
        {
            tran_flag=0;
            memset((void*)dst_base_addr, 0, 8);
            
            UDMA_Init((UDMA_CHxCfg_TypeDef *)(udma_base[j] + 0x8),&UDMA_InitStruct);
            UDMA_ITConfig((UDMA_CHx_IRQ_TypeDef *)(udma_base[j] + 0x800),UDMA_HTRANS_IRQ ,ENABLE);
            M2M_DMA_Cmd((UDMA_CHxCfg_TypeDef *)(udma_base[j] + 0x8),ENABLE);
            while(!tran_flag);
        }
    }

    return 0;
}

int sram_inter_tran()
{
    int i,j,count;

    debug_printf(level,"sram internal test\n");
    for(i=0; i<SRAM_NUM; i++ )
    {
        src_base_addr = (uint32_t)(sram_base[i]);
        dst_base_addr = (uint32_t)(sram_base[i] + sram_size[i] - 63);
        UDMA_InitStruct.UDMA_DstBaseAddr = dst_base_addr;
        UDMA_InitStruct.UDMA_SrcBaseAddr = src_base_addr;
        *( volatile uint32_t* )(src_base_addr)=0x5a5a5a5a;
        *( volatile uint32_t* )(src_base_addr + 4)=0x12345678;
        
        for(j=0; j<UDMA_NUM; j++)
        {
            tran_flag=0;
            memset((void*)dst_base_addr, 0, 8);
            
            UDMA_Init((UDMA_CHxCfg_TypeDef *)(udma_base[j] + 0x8),&UDMA_InitStruct);
            UDMA_ITConfig((UDMA_CHx_IRQ_TypeDef *)(udma_base[j] + 0x800),UDMA_HTRANS_IRQ ,ENABLE);
            M2M_DMA_Cmd((UDMA_CHxCfg_TypeDef *)(udma_base[j] + 0x8),ENABLE);
            while(!tran_flag);           
        }
    }
    return 0;
}

void cache_select(int flag)
{
    switch(flag)
    {
        case 0:
            debug_printf(level,"cache off off \n");
            #if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT == 1
                DisableICache();
            #endif
            #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
                DisableDCache();
            #endif
            break;
        
        case 1:
            debug_printf(level,"cache on off \n");
            #if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT == 1
                EnableICache();
            #endif
            #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
                DisableDCache();
            #endif
            break;

        case 2:
            debug_printf(level,"cache off on \n");
            #if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT == 1
                DisableICache();
            #endif
            #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
                EnableDCache();
            #endif
            break;

        case 3:
            debug_printf(level,"cache on on \n");
            #if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT == 1
                EnableICache();
            #endif
            #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
                EnableDCache();
            #endif
                    break;

        default:
            debug_printf(level,"parameter error\n");
    }
}
#endif
int main(void)
{
    #if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT == 1
        DisableICache();
    #endif
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
        DisableDCache();
    #endif
    int n;
    int i,j,count;
    int errors = 0;
#if defined(SRAM_NUM) && defined(UDMA_NUM)
    UDMA_StructInit(&UDMA_InitStruct);
    UDMA_InitStruct.UDMA_BufferSize = 8;
    UDMA_InitStruct.UDMA_DstInc = UDMA_MDNA_ENABLE;
    UDMA_InitStruct.UDMA_SrcInc = UDMA_MSNA_ENABLE;
    UDMA_InitStruct.UDMA_SrcWidth = UDMA_MSWIDTH_32BIT;
    UDMA_InitStruct.UDMA_DstWidth = UDMA_MDWIDTH_32BIT;
    UDMA_InitStruct.UDMA_Mode = UDMA_MODE_NORMAL;
    UDMA_InitStruct.UDMA_Priority = UDMA_PRIORITY_HIGH;
    /* Global interrupt enable*/
    __enable_irq();

    /*register interrupt UDMA_IRQn */
    ECLIC_Register_IRQ(UDMA0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                        ECLIC_LEVEL_TRIGGER, 1, 0,
                        UDMA0_IRQHandler);

    #if defined(SRAM_NUM) && defined(UDMA_NUM)
    for(i=0; i<2; i++)
    {
        errors = sram_sram_tran(i);
        if(errors != 0)
        {
            debug_printf(level,"sram to sram failed!\n");
            return -1;
        }
    }
    errors = sram_inter_tran();
    #endif
    if(errors != 0)
    {
        debug_printf(level,"sram inter tran failed!\n");
        return -1;
    }
    if(errors == 0)
        debug_printf(level,"DMA MODE SRAM TEST PASS \r\n");
    else
        debug_printf(level,"DMA MODE SRAM TEST FAILED! \r\n");
#else
    #warning "UDMA & SRAM_NUM Marco is not defined"
#endif
    if (errors == 0)
    {
        simulation_pass();
    }
    else
    {
        simulation_fail();
    }
    while(1){};
    return 0 ;

}

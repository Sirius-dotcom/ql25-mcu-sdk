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
#if defined(SRAM_NUM)
uint32_t src_buffer_32[2] = {0x12345678,0x5a5a5a5a};
#define CSR_CCM_MBEGINADDR        0x7CB
#define CSR_CCM_MCOMMAND          0x7CC
void SRAM_MFlushInvalDCache(uint32_t addr)
{
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    __RV_CSR_WRITE(CSR_CCM_MBEGINADDR, addr);
    __RV_CSR_WRITE(CSR_CCM_MCOMMAND, 0x02);
#endif
}

int buf_sram_tran(int flag)
{
	int i,j,count,errors;
    uint32_t dst_base_addr;
    uint32_t src_base_addr;
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
		memset((void*)dst_base_addr,0,8);
        
        memcpy((void*)dst_base_addr, (void*)src_base_addr, 8);
	    for(count=0; count<2; count++)
        {
            if(*( volatile uint32_t* )(dst_base_addr+count*4) != *( volatile uint32_t* )(src_base_addr+count*4))
            {
                debug_printf(level,"0x%x:%x 0x%x:%x\n",src_base_addr, *(volatile uint32_t*)(src_base_addr+ count*4),dst_base_addr,*(   volatile uint32_t*)(dst_base_addr+ count*4));
                return -1;
            }
        }
	}
	
	return 0;
}
int sram_sram_tran(int flag)
{
    int i,j,count;
    uint32_t dst_base_addr;
    uint32_t src_base_addr;

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
		*( volatile uint32_t* )(src_base_addr)=0x5a5a5a5a;
        *( volatile uint32_t* )(src_base_addr + 4)=0x12345678;
        
		memset((void*)dst_base_addr, 0, 8);
        
		memcpy((void*)dst_base_addr, (void*)src_base_addr, 8);
		for(count=0; count<2; count++)
		{
			if((*( volatile uint32_t* )(src_base_addr+count*4)) != (*( volatile uint32_t* )(dst_base_addr+count*4)))
			{
				debug_printf(level,"sram%d to sram%d:0x%x 0x%x\n",i,i+1,*( volatile uint32_t* )(src_base_addr+count*4),*( volatile uint32_t* )(dst_base_addr+count*4));
				return -1;
			}
		}
	}

	return 0;
}

int sram_inter_tran()
{
        int i,j,count;
        uint32_t dst_base_addr;
        uint32_t src_base_addr;

        debug_printf(level,"sram internal test\n");
        for(i=0; i<SRAM_NUM; i++)
        {
		    dst_base_addr = (uint32_t)(sram_base[i] + sram_size[i] - 63 );
            src_base_addr = (uint32_t)(sram_base[i]);
            *( volatile uint32_t* )(sram_base[i] )=0x5a5a5a5a;
            *( volatile uint32_t* )(sram_base[i] + 4)=0x12345678;
            
		    memset((void*)src_base_addr, 0, 8);
            
		    memcpy((void*)dst_base_addr, (void*)src_base_addr, 8);
            for(count=0; count<2; count++)
            {
                if((*( volatile uint32_t* )(src_base_addr+count*4)) != (*( volatile uint32_t* )(dst_base_addr+count*4)))
                {
                    debug_printf(level,"sram%d to sram%d:0x%x 0x%x\n",i,i+1,*( volatile uint32_t* )(src_base_addr+count*4),*( volatile uint32_t* )(dst_base_addr+count*4));
                    return -1;
                }
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

int main()
{	
	uint32_t errors = 0;
	int i,count,n;
    int time_srand;
    int cache_sort;
	debug_printf(level,"test sram cpu mode\r\n");

#if defined(SRAM_NUM)
	
	#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT == 1                  
        DisableICache();
    #endif
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
        DisableDCache();
    #endif
		
    for(i=0; i<2; i++)
	    errors=sram_sram_tran(i);
    if(errors != 0)
    {
        debug_printf(level,"sram to sram failed! cache: %d sram location:%d\n",n, i);
        return -1;
    }
	
    errors=sram_inter_tran();
    if(errors != 0)
    {
        debug_printf(level,"sram inter tran failed!\n");
        return -1;
    }
	
    if(errors == 0)
        debug_printf(level,"CPU MODE SRAM TEST PASS \r\n");
    else
        debug_printf(level,"CPU MODE SRAM TEST FAILED! \r\n");

#endif
#ifdef CFG_SIMULATION
        if (errors == 0)
        {
                simulation_pass();
        }
        else
        {
                simulation_fail();
        }
#endif
    while(1){};
	return 0;
}

#include "ns.h"
#include "ns_sdk_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include "sram_dma_info.h"
#include <stdarg.h>
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
int main()
{
    int i,j,count;
    uint32_t addr;

    debug_printf(level,"BIG SRAM INTERNAL TEST!\n");
    #if defined(SRAM_NUM)
	
	#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT == 1                  
        DisableICache();
    #endif
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
        DisableDCache();
    #endif

    for(j=0;j<sizeof(sram_base)/sizeof(sram_base[0]);j++)
    {
        
            for(i=0;i<16;i++)
            {
                if(sram_size[j]<10240)
                {
                    addr = sram_base[j]+i*((sram_size[j]+1)/16);
                    for(count=0;count<8;count++)
                    {
                        *( volatile uint32_t* )(addr + 4*count)=addr+4*count;
                    }

                    addr = sram_base[j]+i*((sram_size[j]+1)/16) + ((sram_size[j]+1)/16)/2;
                    for(count=0;count<8;count++)
                    {
                        *( volatile uint32_t* )(addr + 4*count)=addr+4*count;
                    }

                    addr = sram_base[j]+i*((sram_size[j]+1)/16) + ((sram_size[j]+1)/16);
                    for(count=0;count<8;count++)
                    {
                        *( volatile uint32_t* )(addr - 4*(count+1))=addr-4*(count+1);
                    }
                }
                else
                {
                
                    addr = sram_base[j]+i*(10240/16);
                    for(count=0;count<8;count++)
                    {
                        *( volatile uint32_t* )(addr + 4*count)=addr+4*count;
                    }

                    addr = sram_base[j]+i*(10240/16) + (10240/16)/2;
                    for(count=0;count<8;count++)
                    {
                        *( volatile uint32_t* )(addr + 4*count)=addr+4*count;
                    }

                    addr = sram_base[j]+i*(10240/16) + (10240/16);
                    for(count=0;count<8;count++)
                    {
                        *( volatile uint32_t* )(addr - 4*(count+1))=addr-4*(count+1);
                    }
                }
            }
        
        for(i=0;i<16;i++)
        {
            
                if(sram_size[j]<10240)
                {
                    addr = sram_base[j]+i*((sram_size[j]+1)/16);
                    for(count=0;count<8;count++)
                    {
                        if(*( volatile uint32_t* )(addr + 4*count) != (addr+4*count))
                        {
                            debug_printf(level,"sram%d: r:0x%x w:0x%x\n",j,*( volatile uint32_t* )(addr + 4*count),addr+4*count);
                            simulation_fail();
                            return -1;
                        }
                    }

                    addr = sram_base[j]+i*((sram_size[j]+1)/16) + ((sram_size[j]+1)/16)/2;
                    for(count=0;count<8;count++)
                    {
                        if(*( volatile uint32_t* )(addr + 4*count) != (addr+4*count))
                        {
                            debug_printf(level,"sram%d: r:0x%x w:0x%x\n",j,*( volatile uint32_t* )(addr + 4*count),addr+4*count);
                            simulation_fail();
                            return -1;
                        }
                    }

                    addr = sram_base[j]+i*((sram_size[j]+1)/16) + ((sram_size[j]+1)/16);
                    for(count=0;count<8;count++)
                    {
                        if(*( volatile uint32_t* )(addr - 4*(count+1)) != (addr-4*(count+1)))
                        {
                            debug_printf(level,"sram%d: r:0x%x w:0x%x\n",j,*( volatile uint32_t* )(addr - 4*(count+1)),addr-4*(count+1));
                            simulation_fail();
                            return -1;
                        }
                    }
                }
                else
                {
                    addr = sram_base[j]+i*(10240/16);
                    for(count=0;count<8;count++)
                    {
                        if(*( volatile uint32_t* )(addr + 4*count) != (addr+4*count))
                        {
                            debug_printf(level,"sram%d: r:0x%x w:0x%x\n",j,*( volatile uint32_t* )(addr + 4*count),addr+4*count);
                            simulation_fail();
                            return -1;
                        }
                    }

                    addr = sram_base[j]+i*(10240/16) + (10240/16)/2;
                    for(count=0;count<8;count++)
                    {
                        if(*( volatile uint32_t* )(addr + 4*count) != (addr+4*count))
                        {
                            debug_printf(level,"sram%d: r:0x%x w:0x%x\n",j,*( volatile uint32_t* )(addr + 4*count),addr+4*count);
                            simulation_fail();
                            return -1;
                        }
                    }

                    addr = sram_base[j]+i*(10240/16) + (10240/16);
                    for(count=0;count<8;count++)
                    {
                        if(*( volatile uint32_t* )(addr - 4*(count+1)) != (addr-4*(count+1)))
                        {
                            debug_printf(level,"sram%d: r:0x%x w:0x%x\n",j,*( volatile uint32_t* )(addr - 4*(count+1)),addr-4*(count+1));
                            simulation_fail();
                            return -1;
                        }
                    }
                }
        }

    }
    #endif
    simulation_pass();
    while(1){};
    return 0;
}

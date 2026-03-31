#include "ns.h"
#include "ns_sdk_hal.h"
#include <stdio.h>
#include "nuclei_sdk_hal.h"
#include "sram_dma_info.h"
#include "stdlib.h"
#include "stdint.h"
#include <stdarg.h>

#define TEST_NUM    (10)
#define TEST_DATA   (0xa5a5a5a5)

uint32_t i, j;
long test_base_addr;
long test_offset;
uint8_t state0 = 1;
int time_srand;
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

void  buserror_handler(unsigned long cause, unsigned long sp)
{
    simulation_fail();
}

int main()
{
    debug_printf(level,"full add scan test\n");
    #if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT == 1			
    DisableICache();
    #endif
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif

    for(i=0; i<12;i++) {
        Exception_Register_EXC(i, (unsigned long)buserror_handler);
    }

    time_srand = ((int)(__TIME__[0])-48)*100000 +
                 ((int)(__TIME__[1])-48)*10000 +
                 ((int)(__TIME__[3])-48)*1000 +
                 ((int)(__TIME__[4])-48)*100 +
                 ((int)(__TIME__[6])-48)*10 +
                 ((int)( __TIME__[7])-48);
    srand(time_srand);

#ifdef DO_NOTHING_NUM
    
    for (i = 0;i < DO_NOTHING_NUM;i++) {
        test_offset = random() % (do_nothing_end[i] - do_nothing_start[i]);
        test_offset = (test_offset / 4) * 4;
        test_base_addr = do_nothing_start[i] + test_offset;
        
    }
#endif
#ifdef WRITE_ONLY_NUM
    
    for (i = 0;i < WRITE_ONLY_NUM;i++) {
        for (j = 0; j < TEST_NUM;j++) {
            test_offset = random() % (sram_size[i]);
            test_offset = (test_offset / 4) * 4;
            test_base_addr = write_only_start[i] + test_offset;
            __SW((void*)test_base_addr, TEST_DATA); 
            __RWMB();
        }
    }
#endif
#ifdef READ_ONLY_NUM
    
    for (i = 0;i < READ_ONLY_NUM;i++) {
        for (j = 0; j < TEST_NUM;j++) {
            test_offset = random() % (read_only_end[i] - read_only_start[i]);
            test_offset = (test_offset / 4) * 4;
            test_base_addr = read_only_start[i] + test_offset;
            __LW((void*)test_base_addr); 
            __RWMB();
        }
    }
#endif

#ifdef SLV_PORT_NUM
    
        for (j = 0; j < TEST_NUM;j++) {
            test_offset = random() % (slv_port_end[0] - slv_port_start[0]);
            test_offset = (test_offset / 4) * 4;
            test_base_addr = slv_port_start[0] + test_offset;
            __LW((void*)test_base_addr); 
            __RWMB();
        }
    
#endif

#ifdef SRAM_NUM
    
    for (i = 0;i < SRAM_NUM;i++) {
        for (j = 0; j < TEST_NUM;j++) {
            test_offset = random() % sram_size[i];
            test_offset = (test_offset / 4) * 4;
            test_base_addr = sram_base[i] + test_offset;
            __SW((void*)test_base_addr, TEST_DATA); 
            __RWMB();
            if (TEST_DATA != __LW((void*)test_base_addr)) {
                state0 = 0;
            }
        }
    }
#endif

#ifdef CFG_SIMULATION
    if (state0) {
        simulation_pass();
    } else {
        simulation_fail();
    }
#endif
    while(1){};
    return 0;
}

#include <stdio.h>
#include "ns_sdk_hal.h"

#define LEDn  2
static uint32_t LGPIO_CLORK[] = {0,1};

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,LGPIO0_GPIO0_IOF_IVAL,LGPIO0_GPIO0_PAD_SEL,LGPIO0_GPIO0_HS_SEL,0,0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE,LGPIO0_GPIO1_IOF_IVAL,LGPIO0_GPIO1_PAD_SEL,LGPIO0_GPIO1_HS_SEL,0,0);
}

int main(void)
{
    uint32_t status0 = 0;
    uint32_t status1 = 0;
    
    #ifdef MISC_HAS_LGPIO0_HAS_CLK
    lgpio0_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_LGPIO0_RST
    lgpio0_set_rst(DISABLE);
    lgpio0_set_rst(ENABLE);
    #endif

    iomux_config();

    printf("test start \r\n");
    for(int i = 0; i < 100;i++) {
        if(0 != LGPIO_ReadInputDataBit(LGPIO0,1 << LGPIO_CLORK[0])){
            status0 = 1;
        } else {
            status0 = 0;
        }
        if(0 != LGPIO_ReadInputDataBit(LGPIO0,1 << LGPIO_CLORK[1])){
            status1 = 1;
        } else {
            status1 = 0;
        }
        if (status0 & status1) {
            simulation_pass();
            while (1) {}
        }
    }
     while (1) {}
}

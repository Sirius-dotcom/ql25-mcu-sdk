#include <stdio.h>
#include "ns_sdk_hal.h"

static uint32_t LGPIO_CLORK[] = {0,1};

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,LGPIO3_GPIO0_IOF_IVAL,LGPIO3_GPIO0_PAD_SEL,LGPIO3_GPIO0_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,LGPIO3_GPIO1_IOF_OVAL,LGPIO3_GPIO1_PAD_SEL,LGPIO3_GPIO1_HS_SEL,0,0);
}

int main(void)
{
    uint8_t status_inout = 0;
    
    #ifdef MISC_HAS_LGPIO3_HAS_CLK
    lgpio3_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_LGPIO3_RST
    lgpio3_set_rst(DISABLE);
    lgpio3_set_rst(ENABLE);
    #endif

    iomux_config();

    LGPIO_Mode(LGPIO3,1 << LGPIO_CLORK[1],PP);
    LGPIO_WriteBit(LGPIO3, 1 << LGPIO_CLORK[1], 1);
    if(0 != LGPIO_ReadInputDataBit(LGPIO3, 1 << LGPIO_CLORK[0])) {
        status_inout += 1;
    } else {
        status_inout += 2;
    }

    LGPIO_WriteBit(LGPIO3, 1 << LGPIO_CLORK[1], 0);
    if(0 == LGPIO_ReadInputDataBit(LGPIO3, 1 << LGPIO_CLORK[0])) {
        status_inout += 3;
    } else {
        status_inout += 4;
    }

    if (4 == status_inout) {
        simulation_pass();
    } else {
        simulation_fail();
    }

    while(1) {};
}

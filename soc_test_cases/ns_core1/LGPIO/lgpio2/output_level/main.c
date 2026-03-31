#include <stdio.h>
#include "ns_sdk_hal.h"

#define LEDn  2

static uint32_t LED_CLORK[] = {LGPIO_PinSource0, LGPIO_PinSource1};    

void led_config(void)
{
   for (int i=0; i<LEDn; i++) {
        LGPIO_WriteBit(LGPIO2,LED_CLORK[i],1);
        LGPIO_Mode(LGPIO2,LED_CLORK[i],PP);
    }
}

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,LGPIO2_GPIO0_IOF_IVAL,LGPIO2_GPIO0_PAD_SEL,LGPIO2_GPIO0_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,LGPIO2_GPIO1_IOF_OVAL,LGPIO2_GPIO1_PAD_SEL,LGPIO2_GPIO1_HS_SEL,0,0);
}

int main(void)
{
    #ifdef MISC_HAS_LGPIO2_HAS_CLK
    lgpio2_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_LGPIO2_RST
    lgpio2_set_rst(DISABLE);
    lgpio2_set_rst(ENABLE);
    #endif

    iomux_config();

    led_config();

    #ifdef CFG_SIMULATION
    for (int c = 0; c < 10; c++) {
    #else
    while (1) {
    #endif
        for(int i=0; i<LEDn; i++)   
        {
            LGPIO_Toggle(LGPIO2,LED_CLORK[i]);
        }
        #ifdef CFG_SIMULATION
        delay_1ms(0);
        #else
        delay_1ms(1000);
        #endif
    }
    simulation_check();
}

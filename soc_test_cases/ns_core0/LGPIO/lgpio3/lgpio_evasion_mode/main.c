#include <stdio.h>
#include "ns_sdk_hal.h"

static uint32_t pin_source[3] = {LGPIO_PinSource0, LGPIO_PinSource1};
uint32_t IRQ_status = 0;

void LGPIO3_IRQHandler(void)
{
    if ((LGPIO_GetITStatus(LGPIO3, pin_source[0], LGPIO_IT_RISE) == pin_source[0]) && ((LGPIO3->PAD_GRP0_RISE_IE & pin_source[0]) == pin_source[0])) {
        LGPIO_ITClear(LGPIO3, pin_source[0], LGPIO_IT_RISE);
        IRQ_status++;
    }
}
void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,LGPIO3_GPIO0_IOF_IVAL,LGPIO3_GPIO0_PAD_SEL,LGPIO3_GPIO0_HS_SEL,0,0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE,LGPIO3_GPIO1_IOF_OVAL,LGPIO3_GPIO1_PAD_SEL,LGPIO3_GPIO1_HS_SEL,0,0);
}

int main(void)
{
    #ifdef MISC_HAS_LGPIO3_HAS_CLK
    lgpio3_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_LGPIO3_RST
    lgpio3_set_rst(DISABLE);
    lgpio3_set_rst(ENABLE);
    #endif

    iomux_config();

    LGPIO_ITClear(LGPIO3, pin_source[0], LGPIO_IT_RISE);

    LGPIO_WriteBit(LGPIO3,pin_source[1],0);
    LGPIO_Mode(LGPIO3,pin_source[1],PP);
    LGPIO_PULLMode(LGPIO3,pin_source[1],PDE);

    delay_1ms(1);

    __enable_irq();

    ECLIC_Register_IRQ(LGPIO3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    LGPIO3_IRQHandler);

    LGPIO_ITConfig(LGPIO3, pin_source[0], LGPIO_IT_RISE, ENABLE);

    delay_1ms(1);

    LGPIO_WriteBit(LGPIO3,pin_source[1],1);
    LGPIO_Mode(LGPIO3,pin_source[1],PP);
    LGPIO_PULLMode(LGPIO3,pin_source[1],PDE);

    delay_1ms(10);

    if(IRQ_status == 2) {
        simulation_fail();
    } else {
        simulation_pass();
    }
    while (1) {};
}

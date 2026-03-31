#include <stdio.h>
#include "ns_sdk_hal.h"
uint8_t state = 1;

void LGPIO1_IRQHandler(void)
{
    if(LGPIO_GetIRQStatus(LGPIO1,LGPIO_PinSource0) == LGPIO_PinSource0){
        printf("Enter gpio0_irqhandler\r\n");
        LGPIO_ITClear(LGPIO1,LGPIO_PinSource0,LGPIO_IT_HIGH);
        LGPIO_ITConfig(LGPIO1,LGPIO_PinSource0,LGPIO_IT_HIGH,DISABLE);
        state = 0;

    }
}

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,LGPIO1_GPIO0_IOF_IVAL,LGPIO1_GPIO0_PAD_SEL,LGPIO1_GPIO0_HS_SEL,0,0);
}

int main(void)
{
    #ifdef MISC_HAS_LGPIO1_HAS_CLK
    lgpio1_clk_en(ENABLE);
    #endif
    #ifdef MISC_HAS_LGPIO1_RST
    lgpio1_set_rst(DISABLE);
    lgpio1_set_rst(ENABLE);
    #endif

     __enable_irq();

	ECLIC_Register_IRQ(LGPIO1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    LGPIO1_IRQHandler);

    iomux_config();

    LGPIO_ITConfig(LGPIO1,LGPIO_PinSource0,LGPIO_IT_HIGH,ENABLE);
    printf("config finish \r\n");

    delay_1ms(1);
    while (state) {}
    simulation_pass();
    while (1) {}
}

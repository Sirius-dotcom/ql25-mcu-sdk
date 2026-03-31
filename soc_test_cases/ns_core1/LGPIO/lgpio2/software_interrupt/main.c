#include <stdio.h>
#include "ns_sdk_hal.h"
uint8_t state = 1;

void LGPIO2_IRQHandler(void)
{
    if(LGPIO_GetITStatus(LGPIO2,LGPIO_PinSource0,LGPIO_IT_HIGH) == LGPIO_PinSource0){
        printf("Enter gpio0_irqhandler\r\n");
        LGPIO_ITClear(LGPIO2,LGPIO_PinSource0,LGPIO_IT_HIGH);
        LGPIO_ITConfig(LGPIO2,LGPIO_PinSource0,LGPIO_IT_HIGH,DISABLE);
        state = 0;
    }
}

void gpio_config(void)
{
   LGPIO_ITConfig(LGPIO2,LGPIO_PinSource0,LGPIO_IT_HIGH,ENABLE);
   LGPIO_SetSoftIT(LGPIO2,LGPIO_PinSource0,LGPIO_IT_HIGH);
}

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,LGPIO2_GPIO0_IOF_IVAL,LGPIO2_GPIO0_PAD_SEL,LGPIO2_GPIO0_HS_SEL,0,0);
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

    gpio_config();  
    printf("config finish \r\n");

    __enable_irq();

	ECLIC_Register_IRQ(LGPIO2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    LGPIO2_IRQHandler);
    while (state) {}
    simulation_pass();
    while (1) {}
}

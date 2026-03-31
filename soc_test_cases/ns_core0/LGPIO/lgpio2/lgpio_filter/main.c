#include <stdio.h>
#include "ns_sdk_hal.h"
uint8_t state = 0;

void LGPIO2_IRQHandler(void)
{
    if(LGPIO_GetITStatus(LGPIO2,LGPIO_PinSource0,LGPIO_IT_RISE) == LGPIO_PinSource0){
        
        LGPIO_ITClear(LGPIO2,LGPIO_PinSource0,LGPIO_IT_RISE);
        state++;
    }
}

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,LGPIO2_GPIO0_IOF_IVAL,LGPIO2_GPIO0_PAD_SEL, LGPIO2_GPIO0_HS_SEL,0,0);
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
    
    uint32_t id = __RV_CSR_READ(CSR_MHARTID);

    if(id==0){

    }else{
        simulation_pass();
        while(1) {}
    }

    __enable_irq();

	ECLIC_Register_IRQ(LGPIO2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    LGPIO2_IRQHandler);

    iomux_config();
    LGPIO_DN_Filter_Num(LGPIO2, LGPIO_DIGITAL_NOISE_FILTER_NUM(3));
    LGPIO_Digital_Noise_Filter(LGPIO2, LGPIO_PinSource0, ENABLE);
    LGPIO_ITClear(LGPIO2,LGPIO_PinSource0,LGPIO_IT_RISE);
    LGPIO_ITConfig(LGPIO2,LGPIO_PinSource0,LGPIO_IT_RISE,ENABLE);

    while (state<2) {}
    if(state==2){
        simulation_pass();
    }else{
        simulation_fail();
    }
    
    while (1) {}
}

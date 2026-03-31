#include <stdio.h>
#include "ns_sdk_hal.h"
uint8_t state = 0;

void LGPIO3_IRQHandler(void)
{
    if(LGPIO_GetITStatus(LGPIO3,LGPIO_PinSource0,LGPIO_IT_RISE) == LGPIO_PinSource0){
        
        LGPIO_ITClear(LGPIO3,LGPIO_PinSource0,LGPIO_IT_RISE);
        state++;
    }
}

void iomux_config(void)
{
    iomux_ls_iof_ival_cfg(IOMUX_BASE,LGPIO3_GPIO0_IOF_IVAL,LGPIO3_GPIO0_PAD_SEL, LGPIO3_GPIO0_HS_SEL,0,0);
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
    
    uint32_t id = __RV_CSR_READ(CSR_MHARTID);

    if(id==0){

    }else{
        simulation_pass();
        while(1) {}
    }

    __enable_irq();

	ECLIC_Register_IRQ(LGPIO3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    LGPIO3_IRQHandler);

    iomux_config();
    LGPIO_DN_Filter_Num(LGPIO3, LGPIO_DIGITAL_NOISE_FILTER_NUM(3));
    LGPIO_Digital_Noise_Filter(LGPIO3, LGPIO_PinSource0, ENABLE);
    LGPIO_ITClear(LGPIO3,LGPIO_PinSource0,LGPIO_IT_RISE);
    LGPIO_ITConfig(LGPIO3,LGPIO_PinSource0,LGPIO_IT_RISE,ENABLE);

    while (state<2) {}
    if(state==2){
        simulation_pass();
    }else{
        simulation_fail();
    }
    
    while (1) {}
}

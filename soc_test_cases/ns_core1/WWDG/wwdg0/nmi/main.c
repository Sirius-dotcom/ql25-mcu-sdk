/*!
    \file  main.c
    \brief WWDG delay feed demo

    \version 2019-6-5, V1.0.0, firmware
*/

/*
    Copyright (c) 2020, Nucleisys Technology  Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include <stdio.h>
#include "ns_sdk_hal.h"

#define WWDG0_NMI BIT(0+1)
#define WWDG1_NMI BIT(1+1)
#define WWDG2_NMI BIT(2+1)
#define WWDG3_NMI BIT(3+1)
#define WWDG4_NMI BIT(4+1)
#define WWDG5_NMI BIT(5+1)
#define WWDG6_NMI BIT(6+1)
#define WWDG7_NMI BIT(7+1)
#define WWDG8_NMI BIT(8+1)
#define WWDG9_NMI BIT(9+1)
#define WWDG10_NMI BIT(10+1)
#define WWDG11_NMI BIT(11+1)
#define WWDG12_NMI BIT(12+1)
#define WWDG13_NMI BIT(13+1)
#define WWDG14_NMI BIT(14+1)
#define WWDG15_NMI BIT(15+1)
#define WWDG16_NMI BIT(16+1)
#define WWDG17_NMI BIT(17+1)
#define WWDG18_NMI BIT(18+1)
#define WWDG19_NMI BIT(19+1)
#define WWDG20_NMI BIT(20+1)
#define WWDG21_NMI BIT(21+1)
#define WWDG22_NMI BIT(22+1)
#define WWDG23_NMI BIT(23+1)
#define WWDG24_NMI BIT(24+1)
#define WWDG25_NMI BIT(25+1)
#define WWDG26_NMI BIT(26+1)
#define WWDG27_NMI BIT(27+1)
#define WWDG28_NMI BIT(28+1)
#define WWDG29_NMI BIT(29+1)
#define WWDG30_NMI BIT(30+1)
#define IDU_HART_NUM_OFFSET 0XC088
#define core_nmi_sel_ofs 0XC000
#define CORE_LOCK_KEY 0XC080
#define POR_FLAG ~por_flag_i

void NMI_Handler(void)
{
    simulation_pass();
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    if (hart_id.b.processor_id == 0) {
        /* Let core1 run */  
        #if defined(CORE1_STOP_ON_RESET)   
        core1_stop_on_reset(DISABLE);
        #endif
        #if defined(CORE2_STOP_ON_RESET)   
        core2_stop_on_reset(DISABLE);
        #endif
        #if defined(CORE3_STOP_ON_RESET)   
        core3_stop_on_reset(DISABLE);
        #endif
        #if defined(CORE4_STOP_ON_RESET)   
        core4_stop_on_reset(DISABLE);
        #endif
        #if defined(CORE5_STOP_ON_RESET)   
        core5_stop_on_reset(DISABLE);
        #endif
        #if defined(CORE6_STOP_ON_RESET)   
        core6_stop_on_reset(DISABLE);
        #endif

        #ifdef MISC_HAS_WWDG0_HAS_CLK
        wwdg0_clk_en(ENABLE);
        #endif
        #ifdef MISC_HAS_WWDG0_RST
        wwdg0_set_rst(DISABLE);
        wwdg0_set_rst(ENABLE);
        #endif

        REG32((IDU_BASE )+CORE_LOCK_KEY)=0xFFFFFFFF;
        REG32((IDU_BASE )+CORE_LOCK_KEY)=0xFFFFFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs)=0xFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs+0x04)=0xFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs+0x08)=0xFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs+0x0c)=0xFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs+0x10)=0xFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs+0x14)=0xFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs+0x18)=0xFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs+0x1c)=0xFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs+0x20)=0xFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs+0x24)=0xFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs+0x28)=0xFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs+0x2c)=0xFFFF;
        REG32((IDU_BASE )+core_nmi_sel_ofs+0x30)=0xFFFF;

        wwdg_interrupt_enable(WWDG0);

        /*
        *  set WWDG clock = (CLK1/4096)/8 = 1648Hz (~606.8 us)
        *  set counter value to 127
        *  set window value to 80
        *  refresh window is: WWDG clock * (127-80)= 28.5ms < refresh window < WWDG clock * (127-63) =38.8ms.
        */
        #ifdef CFG_SIMULATION
        wwdg_config(WWDG0,0x43, 80, WWDG_CFR_WDGTB_DIV_1);
        #else
        wwdg_config(WWDG0,127, 80, WWDG_CFR_WDGTB_DIV_8);
        #endif
        
    }

    /* CFG NMI_handler */
    Exception_Register_EXC(NMI_EXCn, (unsigned long) NMI_Handler);
    
    /*get the hartid*/
    uint32_t hartid=__RV_CSR_READ(CSR_MHARTID);

    printf("WWDG0_NMI\n");
    printf("id=%d\n",__RV_CSR_READ(CSR_MHARTID));

    while(1){
    }
}

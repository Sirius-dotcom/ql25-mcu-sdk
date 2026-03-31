/*
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ns.h"
#include "ns_misc.h"

#ifdef RESET_CTRL0_OFS
#define  RESET_CTRL0_BASE   RESET_CTRL0_OFS
#else
#define  RESET_CTRL0_BASE   SUBM_RESET_CTRL0_OFS
#endif

/**
  * \brief       Enables or disables the clock.
  * \param[in]   reg_base: mis control based adress.
  * \param[in]   offset  : mis control register offset .
  * \param[in]   bit     : mis control register bit offset.
  * \param[in]   status  : new state of the the specified peripheral clock.
  *              This parameter can be: ENABLE or DISABLE.
  */
static inline void misc_clk_cfg1(addr_xlen reg_base, uint32_t offset, uint32_t bit, ControlStatus status) 
{
    if(status == ENABLE)
        REG32(reg_base + offset) |= BIT(bit);
    else
        REG32(reg_base + offset) &= ~BIT(bit);
}

/**
  * \brief     Configurates the frequency division of clock.
  * \param[in]  misc_base: mis control based adress.
  * \param[in]  clk_ctrlx_ofs: reg offset addr.
  * \param[in]  div_val: frequency division coefficient.
  */
void  misc_clk_div(addr_xlen misc_base, uint32_t clk_ctrlx_ofs ,uint8_t div_val)
{
    REG32(misc_base + clk_ctrlx_ofs  ) = div_val;
}

/**
  * \brief       Configurates the clock divider.
  * \param[in]   reg_base: mis control based adress.
  * \param[in]   offset: mis control clock control offset adress.
  * \param[in]   div_val: division coefficient.
  * \param[in]   start: control register start bit.
  * \param[in]   end: control register end bit.
  */
 static inline  void misc_clk_div1(addr_xlen reg_base, uint32_t offset ,uint32_t div_val ,uint32_t start,uint32_t end) 
{
    uint32_t tmp;
    tmp = REG32(reg_base + offset );
    tmp &= ~BITS(start,end);
    REG32(reg_base + offset  ) = (tmp | div_val<<start);
}

/**
  * \brief      Configurates the specified peripheral Reset.
  * \param[in]  reg_base: mis control based adress.
  * \param[in]  offset: reg offset addr.
  * \param[in]  bit: mis control register bit offset.   
  * \param[in]  Status: the rst status of function.
  *             This parameter can be: ENABLE or DISABLE.
  */
 static inline  void misc_reset_cfg(addr_xlen reg_base, uint32_t offset, uint32_t bit, ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(reg_base + offset) |= BIT(bit);
    else
        REG32(reg_base + offset) &= ~BIT(bit);
}

uint32_t get_status_reset_ctrl0(uint32_t flag)
{
    return REG32(0x12000000+0x0) & flag;
}

uint32_t get_status_reset_ctrl1(uint32_t flag)
{
    return REG32(0x12000000+0x4) & flag;
}

uint32_t get_status_reset_ctrl2(uint32_t flag)
{
    return REG32(0x12000000+0x8) & flag;
}

uint32_t get_status_reset_ctrl3(uint32_t flag)
{
    return REG32(0x12000000+0xc) & flag;
}

/**
  * \brief      Get the status_ of soc_c1_sysrstreq.
  * \retval     The status_ of soc_c1_sysrstreq.
  */
uint32_t get_status_soc_c1_sysrstreq(void)
{
    return REG32(0x12000000+0x0) & BIT(7);
}

/**
  * \brief      Get the status_ of soc_c1_sysrstreq.
  * \retval     The status_ of soc_c1_sysrstreq.
  */
uint32_t Get_status_soc_c1_sysrstreq(void)
{
    return REG32(0x12000000+0x0) & BIT(7);
}

/**
  * \brief      Get the status_ of soc_c0_sysrstreq.
  * \retval     The status_ of soc_c0_sysrstreq.
  */
uint32_t get_status_soc_c0_sysrstreq(void)
{
    return REG32(0x12000000+0x0) & BIT(6);
}

/**
  * \brief      Get the status_ of soc_c0_sysrstreq.
  * \retval     The status_ of soc_c0_sysrstreq.
  */
uint32_t Get_status_soc_c0_sysrstreq(void)
{
    return REG32(0x12000000+0x0) & BIT(6);
}

/**
  * \brief      Get the status_ of soc_misc_cpu1_rst_n.
  * \retval     The status_ of soc_misc_cpu1_rst_n.
  */
uint32_t get_status_soc_misc_cpu1_rst_n(void)
{
    return REG32(0x12000000+0x0) & BIT(5);
}

/**
  * \brief      Get the status_ of soc_misc_cpu1_rst_n.
  * \retval     The status_ of soc_misc_cpu1_rst_n.
  */
uint32_t Get_status_soc_misc_cpu1_rst_n(void)
{
    return REG32(0x12000000+0x0) & BIT(5);
}

/**
  * \brief      Get the status_ of soc_misc_cpu0_rst_n.
  * \retval     The status_ of soc_misc_cpu0_rst_n.
  */
uint32_t get_status_soc_misc_cpu0_rst_n(void)
{
    return REG32(0x12000000+0x0) & BIT(4);
}

/**
  * \brief      Get the status_ of soc_misc_cpu0_rst_n.
  * \retval     The status_ of soc_misc_cpu0_rst_n.
  */
uint32_t Get_status_soc_misc_cpu0_rst_n(void)
{
    return REG32(0x12000000+0x0) & BIT(4);
}

/**
  * \brief      Get the status_ of wwdg1_WDOGRES.
  * \retval     The status_ of wwdg1_WDOGRES.
  */
uint32_t get_status_wwdg1_wdogres(void)
{
    return REG32(0x12000000+0x0) & BIT(3);
}

/**
  * \brief      Get the status_ of wwdg1_WDOGRES.
  * \retval     The status_ of wwdg1_WDOGRES.
  */
uint32_t Get_status_wwdg1_WDOGRES(void)
{
    return REG32(0x12000000+0x0) & BIT(3);
}

/**
  * \brief      Get the status_ of wwdg0_WDOGRES.
  * \retval     The status_ of wwdg0_WDOGRES.
  */
uint32_t get_status_wwdg0_wdogres(void)
{
    return REG32(0x12000000+0x0) & BIT(2);
}

/**
  * \brief      Get the status_ of wwdg0_WDOGRES.
  * \retval     The status_ of wwdg0_WDOGRES.
  */
uint32_t Get_status_wwdg0_WDOGRES(void)
{
    return REG32(0x12000000+0x0) & BIT(2);
}

/**
  * \brief      Get the status_ of por_rst_n.
  * \retval     The status_ of por_rst_n.
  */
uint32_t get_status_por_rst_n(void)
{
    return REG32(0x12000000+0x0) & BIT(1);
}

/**
  * \brief      Get the status_ of por_rst_n.
  * \retval     The status_ of por_rst_n.
  */
uint32_t Get_status_por_rst_n(void)
{
    return REG32(0x12000000+0x0) & BIT(1);
}

/**
  * \brief      Get the status_ of sys_rst_n_in.
  * \retval     The status_ of sys_rst_n_in.
  */
uint32_t get_status_sys_rst_n_in(void)
{
    return REG32(0x12000000+0x0) & BIT(0);
}

/**
  * \brief      Get the status_ of sys_rst_n_in.
  * \retval     The status_ of sys_rst_n_in.
  */
uint32_t Get_status_sys_rst_n_in(void)
{
    return REG32(0x12000000+0x0) & BIT(0);
}

/**
  * \brief      clear_reset_status.
  */
void clear_reset_status(void)
{
    REG32(0x12000000+0xc) |=BIT(31);
}

/*******************************subm_reset_ctrl0 rst ctrl******************************************/

/**
  * \brief      Enables or disables sram1 reset.
  * \param[in]  Status: new state of the sram1 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sram1_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,31,Status);
        
}

/**
  * \brief      Enables or disables sram0 reset.
  * \param[in]  Status: new state of the sram0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sram0_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,30,Status);
        
}

/**
  * \brief      Enables or disables xkan1 reset.
  * \param[in]  Status: new state of the xkan1 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void xkan1_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,29,Status);
        
}

/**
  * \brief      Enables or disables xkan0 reset.
  * \param[in]  Status: new state of the xkan0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void xkan0_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,28,Status);
        
}

/**
  * \brief      Enables or disables lgpio3 reset.
  * \param[in]  Status: new state of the lgpio3 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void lgpio3_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,27,Status);
        
}

/**
  * \brief      Enables or disables lgpio2 reset.
  * \param[in]  Status: new state of the lgpio2 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void lgpio2_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,26,Status);
        
}

/**
  * \brief      Enables or disables lgpio1 reset.
  * \param[in]  Status: new state of the lgpio1 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void lgpio1_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,25,Status);
        
}

/**
  * \brief      Enables or disables lgpio0 reset.
  * \param[in]  Status: new state of the lgpio0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void lgpio0_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,24,Status);
        
}

/**
  * \brief      Enables or disables basic_timer7 reset.
  * \param[in]  Status: new state of the basic_timer7 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer7_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,23,Status);
        
}

/**
  * \brief      Enables or disables basic_timer6 reset.
  * \param[in]  Status: new state of the basic_timer6 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer6_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,22,Status);
        
}

/**
  * \brief      Enables or disables basic_timer5 reset.
  * \param[in]  Status: new state of the basic_timer5 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer5_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,21,Status);
        
}

/**
  * \brief      Enables or disables basic_timer4 reset.
  * \param[in]  Status: new state of the basic_timer4 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer4_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,20,Status);
        
}

/**
  * \brief      Enables or disables basic_timer3 reset.
  * \param[in]  Status: new state of the basic_timer3 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer3_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,19,Status);
        
}

/**
  * \brief      Enables or disables basic_timer2 reset.
  * \param[in]  Status: new state of the basic_timer2 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer2_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,18,Status);
        
}

/**
  * \brief      Enables or disables basic_timer1 reset.
  * \param[in]  Status: new state of the basic_timer1 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer1_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,17,Status);
        
}

/**
  * \brief      Enables or disables basic_timer0 reset.
  * \param[in]  Status: new state of the basic_timer0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer0_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,16,Status);
        
}

/**
  * \brief      Enables or disables advanced_timer3 reset.
  * \param[in]  Status: new state of the advanced_timer3 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void advanced_timer3_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,15,Status);
        
}

/**
  * \brief      Enables or disables advanced_timer2 reset.
  * \param[in]  Status: new state of the advanced_timer2 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void advanced_timer2_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,14,Status);
        
}

/**
  * \brief      Enables or disables advanced_timer1 reset.
  * \param[in]  Status: new state of the advanced_timer1 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void advanced_timer1_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,13,Status);
        
}

/**
  * \brief      Enables or disables advanced_timer0 reset.
  * \param[in]  Status: new state of the advanced_timer0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void advanced_timer0_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,12,Status);
        
}

/**
  * \brief      Enables or disables qspi3 reset.
  * \param[in]  Status: new state of the qspi3 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void qspi3_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,11,Status);
        
}

/**
  * \brief      Enables or disables qspi2 reset.
  * \param[in]  Status: new state of the qspi2 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void qspi2_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,10,Status);
        
}

/**
  * \brief      Enables or disables qspi1 reset.
  * \param[in]  Status: new state of the qspi1 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void qspi1_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,9,Status);
        
}

/**
  * \brief      Enables or disables qspi_xip0 reset.
  * \param[in]  Status: new state of the qspi_xip0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void qspi_xip0_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,8,Status);
        
}

/**
  * \brief      Enables or disables i2c1 reset.
  * \param[in]  Status: new state of the i2c1 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void i2c1_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,7,Status);
        
}

/**
  * \brief      Enables or disables i2c0 reset.
  * \param[in]  Status: new state of the i2c0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void i2c0_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,6,Status);
        
}

/**
  * \brief      Enables or disables usart4 reset.
  * \param[in]  Status: new state of the usart4 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void usart4_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,5,Status);
        
}

/**
  * \brief      Enables or disables usart3 reset.
  * \param[in]  Status: new state of the usart3 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void usart3_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,4,Status);
        
}

/**
  * \brief      Enables or disables usart2 reset.
  * \param[in]  Status: new state of the usart2 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void usart2_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,3,Status);
        
}

/**
  * \brief      Enables or disables usart1 reset.
  * \param[in]  Status: new state of the usart1 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void usart1_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,2,Status);
        
}

/**
  * \brief      Enables or disables usart0 reset.
  * \param[in]  Status: new state of the usart0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void usart0_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,1,Status);
        
}

/**
  * \brief      Enables or disables udma0 reset.
  * \param[in]  Status: new state of the udma0 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void udma0_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x20,0,Status);
        
}

/*******************************subm_reset_ctrl1 rst ctrl******************************************/

/**
  * \brief      Enables or disables iomux reset.
  * \param[in]  Status: new state of the iomux reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void iomux_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,12,Status);
        
}

/**
  * \brief      Enables or disables idu reset.
  * \param[in]  Status: new state of the idu reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void idu_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,11,Status);
        
}

/**
  * \brief      Enables or disables apb_slv2_ratio reset.
  * \param[in]  Status: new state of the apb_slv2_ratio reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void apb_slv2_ratio_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,10,Status);
        
}

/**
  * \brief      Enables or disables apb_slv1_ratio reset.
  * \param[in]  Status: new state of the apb_slv1_ratio reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void apb_slv1_ratio_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,9,Status);
        
}

/**
  * \brief      Enables or disables apb_slv0_ratio reset.
  * \param[in]  Status: new state of the apb_slv0_ratio reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void apb_slv0_ratio_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,8,Status);
        
}

/**
  * \brief      Enables or disables ahb_slv3_ratio reset.
  * \param[in]  Status: new state of the ahb_slv3_ratio reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_slv3_ratio_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,7,Status);
        
}

/**
  * \brief      Enables or disables ahb_slv2_ratio reset.
  * \param[in]  Status: new state of the ahb_slv2_ratio reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_slv2_ratio_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,6,Status);
        
}

/**
  * \brief      Enables or disables ahb_slv1_ratio reset.
  * \param[in]  Status: new state of the ahb_slv1_ratio reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_slv1_ratio_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,5,Status);
        
}

/**
  * \brief      Enables or disables ahb_slv0_ratio reset.
  * \param[in]  Status: new state of the ahb_slv0_ratio reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_slv0_ratio_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,4,Status);
        
}

/**
  * \brief      Enables or disables ahb_mst1_sync reset.
  * \param[in]  Status: new state of the ahb_mst1_sync reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_mst1_sync_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,3,Status);
        
}

/**
  * \brief      Enables or disables ahb_mst0_sync reset.
  * \param[in]  Status: new state of the ahb_mst0_sync reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_mst0_sync_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,2,Status);
        
}

/**
  * \brief      Enables or disables sram3 reset.
  * \param[in]  Status: new state of the sram3 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sram3_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,1,Status);
        
}

/**
  * \brief      Enables or disables sram2 reset.
  * \param[in]  Status: new state of the sram2 reset.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sram2_set_rst(ControlStatus Status)
{
    misc_reset_cfg(0x12000000,0x24,0,Status);
        
}

/*******************************subm_reset_ctrl2 rst ctrl******************************************/

/*******************************subm_reset_ctrl3 rst ctrl******************************************/

/*******************************subm_reset_ctrl4 rst ctrl******************************************/

/*******************************subm_reset_ctrl5 rst ctrl******************************************/

/*******************************subm_reset_ctrl6 rst ctrl******************************************/

/*******************************subm_reset_ctrl7 rst ctrl******************************************/

/*******************************subm_clk_ctrl0 clk ctrl******************************************/

/**
  * \brief      Enables or disables sram1 clock.
  * \param[in]  Status: new state of the sram1 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sram1_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,31,Status);
        
}

/**
  * \brief      Enables or disables sram0 clock.
  * \param[in]  Status: new state of the sram0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sram0_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,30,Status);
        
}

/**
  * \brief      Enables or disables xkan1 clock.
  * \param[in]  Status: new state of the xkan1 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void xkan1_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,29,Status);
        
}

/**
  * \brief      Enables or disables xkan0 clock.
  * \param[in]  Status: new state of the xkan0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void xkan0_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,28,Status);
        
}

/**
  * \brief      Enables or disables lgpio3 clock.
  * \param[in]  Status: new state of the lgpio3 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void lgpio3_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,27,Status);
        
}

/**
  * \brief      Enables or disables lgpio2 clock.
  * \param[in]  Status: new state of the lgpio2 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void lgpio2_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,26,Status);
        
}

/**
  * \brief      Enables or disables lgpio1 clock.
  * \param[in]  Status: new state of the lgpio1 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void lgpio1_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,25,Status);
        
}

/**
  * \brief      Enables or disables lgpio0 clock.
  * \param[in]  Status: new state of the lgpio0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void lgpio0_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,24,Status);
        
}

/**
  * \brief      Enables or disables basic_timer7 clock.
  * \param[in]  Status: new state of the basic_timer7 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer7_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,23,Status);
        
}

/**
  * \brief      Enables or disables basic_timer6 clock.
  * \param[in]  Status: new state of the basic_timer6 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer6_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,22,Status);
        
}

/**
  * \brief      Enables or disables basic_timer5 clock.
  * \param[in]  Status: new state of the basic_timer5 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer5_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,21,Status);
        
}

/**
  * \brief      Enables or disables basic_timer4 clock.
  * \param[in]  Status: new state of the basic_timer4 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer4_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,20,Status);
        
}

/**
  * \brief      Enables or disables basic_timer3 clock.
  * \param[in]  Status: new state of the basic_timer3 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer3_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,19,Status);
        
}

/**
  * \brief      Enables or disables basic_timer2 clock.
  * \param[in]  Status: new state of the basic_timer2 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer2_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,18,Status);
        
}

/**
  * \brief      Enables or disables basic_timer1 clock.
  * \param[in]  Status: new state of the basic_timer1 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer1_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,17,Status);
        
}

/**
  * \brief      Enables or disables basic_timer0 clock.
  * \param[in]  Status: new state of the basic_timer0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void basic_timer0_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,16,Status);
        
}

/**
  * \brief      Enables or disables advanced_timer3 clock.
  * \param[in]  Status: new state of the advanced_timer3 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void advanced_timer3_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,15,Status);
        
}

/**
  * \brief      Enables or disables advanced_timer2 clock.
  * \param[in]  Status: new state of the advanced_timer2 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void advanced_timer2_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,14,Status);
        
}

/**
  * \brief      Enables or disables advanced_timer1 clock.
  * \param[in]  Status: new state of the advanced_timer1 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void advanced_timer1_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,13,Status);
        
}

/**
  * \brief      Enables or disables advanced_timer0 clock.
  * \param[in]  Status: new state of the advanced_timer0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void advanced_timer0_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,12,Status);
        
}

/**
  * \brief      Enables or disables qspi3 clock.
  * \param[in]  Status: new state of the qspi3 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void qspi3_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,11,Status);
        
}

/**
  * \brief      Enables or disables qspi2 clock.
  * \param[in]  Status: new state of the qspi2 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void qspi2_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,10,Status);
        
}

/**
  * \brief      Enables or disables qspi1 clock.
  * \param[in]  Status: new state of the qspi1 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void qspi1_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,9,Status);
        
}

/**
  * \brief      Enables or disables qspi_xip0 clock.
  * \param[in]  Status: new state of the qspi_xip0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void qspi_xip0_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,8,Status);
        
}

/**
  * \brief      Enables or disables i2c1 clock.
  * \param[in]  Status: new state of the i2c1 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void i2c1_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,7,Status);
        
}

/**
  * \brief      Enables or disables i2c0 clock.
  * \param[in]  Status: new state of the i2c0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void i2c0_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,6,Status);
        
}

/**
  * \brief      Enables or disables usart4 clock.
  * \param[in]  Status: new state of the usart4 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void usart4_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,5,Status);
        
}

/**
  * \brief      Enables or disables usart3 clock.
  * \param[in]  Status: new state of the usart3 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void usart3_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,4,Status);
        
}

/**
  * \brief      Enables or disables usart2 clock.
  * \param[in]  Status: new state of the usart2 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void usart2_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,3,Status);
        
}

/**
  * \brief      Enables or disables usart1 clock.
  * \param[in]  Status: new state of the usart1 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void usart1_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,2,Status);
        
}

/**
  * \brief      Enables or disables usart0 clock.
  * \param[in]  Status: new state of the usart0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void usart0_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,1,Status);
        
}

/**
  * \brief      Enables or disables udma0 clock.
  * \param[in]  Status: new state of the udma0 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void udma0_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x40,0,Status);
        
}

/*******************************subm_clk_ctrl1 clk ctrl******************************************/

/**
  * \brief      Enables or disables iomux clock.
  * \param[in]  Status: new state of the iomux clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void iomux_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,12,Status);
        
}

/**
  * \brief      Enables or disables idu clock.
  * \param[in]  Status: new state of the idu clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void idu_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,11,Status);
        
}

/**
  * \brief      Enables or disables apb_slv2_ratio clock.
  * \param[in]  Status: new state of the apb_slv2_ratio clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void apb_slv2_ratio_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,10,Status);
        
}

/**
  * \brief      Enables or disables apb_slv1_ratio clock.
  * \param[in]  Status: new state of the apb_slv1_ratio clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void apb_slv1_ratio_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,9,Status);
        
}

/**
  * \brief      Enables or disables apb_slv0_ratio clock.
  * \param[in]  Status: new state of the apb_slv0_ratio clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void apb_slv0_ratio_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,8,Status);
        
}

/**
  * \brief      Enables or disables ahb_slv3_ratio clock.
  * \param[in]  Status: new state of the ahb_slv3_ratio clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_slv3_ratio_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,7,Status);
        
}

/**
  * \brief      Enables or disables ahb_slv2_ratio clock.
  * \param[in]  Status: new state of the ahb_slv2_ratio clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_slv2_ratio_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,6,Status);
        
}

/**
  * \brief      Enables or disables ahb_slv1_ratio clock.
  * \param[in]  Status: new state of the ahb_slv1_ratio clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_slv1_ratio_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,5,Status);
        
}

/**
  * \brief      Enables or disables ahb_slv0_ratio clock.
  * \param[in]  Status: new state of the ahb_slv0_ratio clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_slv0_ratio_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,4,Status);
        
}

/**
  * \brief      Enables or disables ahb_mst1_sync clock.
  * \param[in]  Status: new state of the ahb_mst1_sync clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_mst1_sync_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,3,Status);
        
}

/**
  * \brief      Enables or disables ahb_mst0_sync clock.
  * \param[in]  Status: new state of the ahb_mst0_sync clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void ahb_mst0_sync_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,2,Status);
        
}

/**
  * \brief      Enables or disables sram3 clock.
  * \param[in]  Status: new state of the sram3 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sram3_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,1,Status);
        
}

/**
  * \brief      Enables or disables sram2 clock.
  * \param[in]  Status: new state of the sram2 clock.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void sram2_clk_en(ControlStatus Status)
{
    misc_clk_cfg1(0x12000000,0x44,0,Status);
        
}

/*******************************subm_clk_ctrl2 clk ctrl******************************************/

/*******************************subm_clk_ctrl3 clk ctrl******************************************/

/*******************************subm_clk_ctrl4 clk ctrl******************************************/

/*******************************subm_clk_ctrl5 clk ctrl******************************************/

/*******************************subm_clk_ctrl6 clk ctrl******************************************/

/*******************************subm_clk_ctrl7 clk ctrl******************************************/

/**
  * \brief main fab clk i div configuration
  * \param div_val: clk div number
  */
void main_fab_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x100,div_val,0,19);
}

/**
  * \brief test fab clk i div configuration
  * \param div_val: clk div number
  */
void test_fab_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x104,div_val,0,19);
}

/**
  * \brief ahb slv0 clk i div configuration
  * \param div_val: clk div number
  */
void ahb_slv0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x108,div_val,0,19);
}

/**
  * \brief ahb slv1 clk i div configuration
  * \param div_val: clk div number
  */
void ahb_slv1_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x10c,div_val,0,19);
}

/**
  * \brief ahb slv2 clk i div configuration
  * \param div_val: clk div number
  */
void ahb_slv2_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x110,div_val,0,19);
}

/**
  * \brief ahb slv3 clk i div configuration
  * \param div_val: clk div number
  */
void ahb_slv3_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x114,div_val,0,19);
}

/**
  * \brief apb slv0 clk i div configuration
  * \param div_val: clk div number
  */
void apb_slv0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x118,div_val,0,19);
}

/**
  * \brief apb slv1 clk i div configuration
  * \param div_val: clk div number
  */
void apb_slv1_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x11c,div_val,0,19);
}

/**
  * \brief apb slv2 clk i div configuration
  * \param div_val: clk div number
  */
void apb_slv2_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x120,div_val,0,19);
}

/**
  * \brief usart0 clk i div configuration
  * \param div_val: clk div number
  */
void usart0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x124,div_val,0,19);
}

/**
  * \brief usart1 clk i div configuration
  * \param div_val: clk div number
  */
void usart1_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x128,div_val,0,19);
}

/**
  * \brief usart2 clk i div configuration
  * \param div_val: clk div number
  */
void usart2_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x12c,div_val,0,19);
}

/**
  * \brief usart3 clk i div configuration
  * \param div_val: clk div number
  */
void usart3_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x130,div_val,0,19);
}

/**
  * \brief usart4 clk i div configuration
  * \param div_val: clk div number
  */
void usart4_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x134,div_val,0,19);
}

/**
  * \brief i2c0 clk i div configuration
  * \param div_val: clk div number
  */
void i2c0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x138,div_val,0,19);
}

/**
  * \brief i2c1 clk i div configuration
  * \param div_val: clk div number
  */
void i2c1_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x13c,div_val,0,19);
}

/**
  * \brief qspi1 clk i div configuration
  * \param div_val: clk div number
  */
void qspi1_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x140,div_val,0,19);
}

/**
  * \brief qspi2 clk i div configuration
  * \param div_val: clk div number
  */
void qspi2_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x144,div_val,0,19);
}

/**
  * \brief qspi3 clk i div configuration
  * \param div_val: clk div number
  */
void qspi3_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x148,div_val,0,19);
}

/**
  * \brief advanced timer0 clk i div configuration
  * \param div_val: clk div number
  */
void advanced_timer0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x14c,div_val,0,19);
}

/**
  * \brief advanced timer1 clk i div configuration
  * \param div_val: clk div number
  */
void advanced_timer1_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x150,div_val,0,19);
}

/**
  * \brief advanced timer2 clk i div configuration
  * \param div_val: clk div number
  */
void advanced_timer2_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x154,div_val,0,19);
}

/**
  * \brief advanced timer3 clk i div configuration
  * \param div_val: clk div number
  */
void advanced_timer3_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x158,div_val,0,19);
}

/**
  * \brief basic timer0 clk i div configuration
  * \param div_val: clk div number
  */
void basic_timer0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x15c,div_val,0,19);
}

/**
  * \brief basic timer1 clk i div configuration
  * \param div_val: clk div number
  */
void basic_timer1_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x160,div_val,0,19);
}

/**
  * \brief basic timer2 clk i div configuration
  * \param div_val: clk div number
  */
void basic_timer2_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x164,div_val,0,19);
}

/**
  * \brief basic timer3 clk i div configuration
  * \param div_val: clk div number
  */
void basic_timer3_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x168,div_val,0,19);
}

/**
  * \brief basic timer4 clk i div configuration
  * \param div_val: clk div number
  */
void basic_timer4_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x16c,div_val,0,19);
}

/**
  * \brief basic timer5 clk i div configuration
  * \param div_val: clk div number
  */
void basic_timer5_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x170,div_val,0,19);
}

/**
  * \brief basic timer6 clk i div configuration
  * \param div_val: clk div number
  */
void basic_timer6_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x174,div_val,0,19);
}

/**
  * \brief basic timer7 clk i div configuration
  * \param div_val: clk div number
  */
void basic_timer7_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x178,div_val,0,19);
}

/**
  * \brief lgpio0 clk i div configuration
  * \param div_val: clk div number
  */
void lgpio0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x17c,div_val,0,19);
}

/**
  * \brief lgpio1 clk i div configuration
  * \param div_val: clk div number
  */
void lgpio1_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x180,div_val,0,19);
}

/**
  * \brief lgpio2 clk i div configuration
  * \param div_val: clk div number
  */
void lgpio2_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x184,div_val,0,19);
}

/**
  * \brief lgpio3 clk i div configuration
  * \param div_val: clk div number
  */
void lgpio3_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x188,div_val,0,19);
}

/**
  * \brief wwdg0 clk i div configuration
  * \param div_val: clk div number
  */
void wwdg0_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x18c,div_val,0,19);
}

/**
  * \brief wwdg1 clk i div configuration
  * \param div_val: clk div number
  */
void wwdg1_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x190,div_val,0,19);
}

/**
  * \brief rtc0 cfg clk i div configuration
  * \param div_val: clk div number
  */
void rtc0_cfg_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x194,div_val,0,19);
}

/**
  * \brief rtc1 cfg clk i div configuration
  * \param div_val: clk div number
  */
void rtc1_cfg_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x198,div_val,0,19);
}

/**
  * \brief rtc2 cfg clk i div configuration
  * \param div_val: clk div number
  */
void rtc2_cfg_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x19c,div_val,0,19);
}

/**
  * \brief xkan0 cfg clk i div configuration
  * \param div_val: clk div number
  */
void xkan0_cfg_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x1a0,div_val,0,19);
}

/**
  * \brief xkan intf clk div configuration
  * \param div_val: clk div number
  */
void xkan_intf_clk_div(uint32_t div_val) 
{
    misc_clk_div1(0x12000000, 0x1a4,div_val,0,19);
}

void all_clk_div(uint32_t div_val)
{

#ifdef MISC_HAS_AHB_SLV0_CLK_DIV
 ahb_slv0_clk_div(div_val);
#endif

#ifdef MISC_HAS_AHB_SLV1_CLK_DIV
 ahb_slv1_clk_div(div_val);
#endif

#ifdef MISC_HAS_AHB_SLV2_CLK_DIV
 ahb_slv2_clk_div(div_val);
#endif

#ifdef MISC_HAS_AHB_SLV3_CLK_DIV
 ahb_slv3_clk_div(div_val);
#endif

#ifdef MISC_HAS_APB_SLV0_CLK_DIV
 apb_slv0_clk_div(div_val);
#endif

#ifdef MISC_HAS_APB_SLV1_CLK_DIV
 apb_slv1_clk_div(div_val);
#endif

#ifdef MISC_HAS_APB_SLV2_CLK_DIV
 apb_slv2_clk_div(div_val);
#endif

#ifdef MISC_HAS_USART1_CLK_DIV
 usart1_clk_div(div_val);
#endif

#ifdef MISC_HAS_USART2_CLK_DIV
 usart2_clk_div(div_val);
#endif

#ifdef MISC_HAS_USART3_CLK_DIV
 usart3_clk_div(div_val);
#endif

#ifdef MISC_HAS_USART4_CLK_DIV
 usart4_clk_div(div_val);
#endif

#ifdef MISC_HAS_I2C0_CLK_DIV
 i2c0_clk_div(div_val);
#endif

#ifdef MISC_HAS_I2C1_CLK_DIV
 i2c1_clk_div(div_val);
#endif

#ifdef MISC_HAS_QSPI1_CLK_DIV
 qspi1_clk_div(div_val);
#endif

#ifdef MISC_HAS_QSPI2_CLK_DIV
 qspi2_clk_div(div_val);
#endif

#ifdef MISC_HAS_QSPI3_CLK_DIV
 qspi3_clk_div(div_val);
#endif

#ifdef MISC_HAS_ADVANCED_TIMER0_CLK_DIV
 advanced_timer0_clk_div(div_val);
#endif

#ifdef MISC_HAS_ADVANCED_TIMER1_CLK_DIV
 advanced_timer1_clk_div(div_val);
#endif

#ifdef MISC_HAS_ADVANCED_TIMER2_CLK_DIV
 advanced_timer2_clk_div(div_val);
#endif

#ifdef MISC_HAS_ADVANCED_TIMER3_CLK_DIV
 advanced_timer3_clk_div(div_val);
#endif

#ifdef MISC_HAS_BASIC_TIMER0_CLK_DIV
 basic_timer0_clk_div(div_val);
#endif

#ifdef MISC_HAS_BASIC_TIMER1_CLK_DIV
 basic_timer1_clk_div(div_val);
#endif

#ifdef MISC_HAS_BASIC_TIMER2_CLK_DIV
 basic_timer2_clk_div(div_val);
#endif

#ifdef MISC_HAS_BASIC_TIMER3_CLK_DIV
 basic_timer3_clk_div(div_val);
#endif

#ifdef MISC_HAS_BASIC_TIMER4_CLK_DIV
 basic_timer4_clk_div(div_val);
#endif

#ifdef MISC_HAS_BASIC_TIMER5_CLK_DIV
 basic_timer5_clk_div(div_val);
#endif

#ifdef MISC_HAS_BASIC_TIMER6_CLK_DIV
 basic_timer6_clk_div(div_val);
#endif

#ifdef MISC_HAS_BASIC_TIMER7_CLK_DIV
 basic_timer7_clk_div(div_val);
#endif

#ifdef MISC_HAS_LGPIO0_CLK_DIV
 lgpio0_clk_div(div_val);
#endif

#ifdef MISC_HAS_LGPIO1_CLK_DIV
 lgpio1_clk_div(div_val);
#endif

#ifdef MISC_HAS_LGPIO2_CLK_DIV
 lgpio2_clk_div(div_val);
#endif

#ifdef MISC_HAS_LGPIO3_CLK_DIV
 lgpio3_clk_div(div_val);
#endif

#ifdef MISC_HAS_WWDG0_CLK_DIV
 wwdg0_clk_div(div_val);
#endif

#ifdef MISC_HAS_WWDG1_CLK_DIV
 wwdg1_clk_div(div_val);
#endif

#ifdef MISC_HAS_RTC0_CFG_CLK_DIV
 rtc0_cfg_clk_div(div_val);
#endif

#ifdef MISC_HAS_RTC1_CFG_CLK_DIV
 rtc1_cfg_clk_div(div_val);
#endif

#ifdef MISC_HAS_RTC2_CFG_CLK_DIV
 rtc2_cfg_clk_div(div_val);
#endif

#ifdef MISC_HAS_XKAN0_CFG_CLK_DIV
 xkan0_cfg_clk_div(div_val);
#endif

#ifdef MISC_HAS_XKAN_INTF_CLK_DIV
 xkan_intf_clk_div(div_val);
#endif

}

/**
  * \brief      Enables or disables pll_ctrl0_sys_clk_pll.
  * \param[in]  Status: new state of the pll_ctrl0_sys_clk_pll.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void pll_ctrl0_sys_clk_pll_bp(ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(0x12000000 + 0x78) |= (1<<24);
    else
        REG32(0x12000000 + 0x78) &= ~(1<<24);
}

/**
  * \brief    Get pll_ctrl0_sys_clk_pll lock status.
  * \retval SET: the specified PLL is LOCK.
  * \retval RESET: the specified PLL is UNLOCK.
  */

uint32_t pll_ctrl0_sys_clk_pll_lock(void)
{
    return REG32(0x12000000 + 0x78) & (1<<25);
}

/**
  * \brief      Enables or disables pll_ctrl1_sys_aclk_pll.
  * \param[in]  Status: new state of the pll_ctrl1_sys_aclk_pll.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void pll_ctrl1_sys_aclk_pll_bp(ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(0x12000000 + 0x7c) |= (1<<24);
    else
        REG32(0x12000000 + 0x7c) &= ~(1<<24);
}

/**
  * \brief    Get pll_ctrl1_sys_aclk_pll lock status.
  * \retval SET: the specified PLL is LOCK.
  * \retval RESET: the specified PLL is UNLOCK.
  */

uint32_t pll_ctrl1_sys_aclk_pll_lock(void)
{
    return REG32(0x12000000 + 0x7c) & (1<<25);
}

/**
  * \brief      Enables or disables pll_ctrl2_sys_bclk_pll.
  * \param[in]  Status: new state of the pll_ctrl2_sys_bclk_pll.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void pll_ctrl2_sys_bclk_pll_bp(ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(0x12000000 + 0x80) |= (1<<24);
    else
        REG32(0x12000000 + 0x80) &= ~(1<<24);
}

/**
  * \brief    Get pll_ctrl2_sys_bclk_pll lock status.
  * \retval SET: the specified PLL is LOCK.
  * \retval RESET: the specified PLL is UNLOCK.
  */

uint32_t pll_ctrl2_sys_bclk_pll_lock(void)
{
    return REG32(0x12000000 + 0x80) & (1<<25);
}

/**
  * \brief  dbg_jtag_dwbypass configuration.
  * \param[in]  Status: new state of the misc_ctrl1.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void dbg_jtag_dwbypass(ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(0x12000000 + 0xc84) |= (1<<1);
    else
        REG32(0x12000000 + 0xc84) &= ~(1<<1);
}

/**
  * \brief Get flag bit of dbg_jtag_dwen.
  */
uint32_t dbg_jtag_dwen(void)
{
    return REG32(0x12000000 + 0xc84) & BIT(0);
}

/**
  * \brief  core1_dbg_pd_d configuration.
  * \param[in]  Status: new state of the misc_ctrl2.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void core1_dbg_pd_d(ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(0x12000000 + 0xc88) |= (1<<1);
    else
        REG32(0x12000000 + 0xc88) &= ~(1<<1);
}

/**
  * \brief  core0_dbg_pd_d configuration.
  * \param[in]  Status: new state of the misc_ctrl2.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void core0_dbg_pd_d(ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(0x12000000 + 0xc88) |= (1<<0);
    else
        REG32(0x12000000 + 0xc88) &= ~(1<<0);
}

/**
  * \brief  soc_misc_cpu1_rst_n configuration.
  * \param[in]  Status: new state of the misc_ctrl3.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void soc_misc_cpu1_rst_n(ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(0x12000000 + 0xc8c) |= (1<<1);
    else
        REG32(0x12000000 + 0xc8c) &= ~(1<<1);
}

/**
  * \brief  soc_misc_cpu0_rst_n configuration.
  * \param[in]  Status: new state of the misc_ctrl3.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void soc_misc_cpu0_rst_n(ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(0x12000000 + 0xc8c) |= (1<<0);
    else
        REG32(0x12000000 + 0xc8c) &= ~(1<<0);
}

/**
  * \brief  core1_sysrstreq_en configuration.
  * \param[in]  Status: new state of the misc_ctrl4.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void core1_sysrstreq_en(ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(0x12000000 + 0xc90) |= (1<<1);
    else
        REG32(0x12000000 + 0xc90) &= ~(1<<1);
}

/**
  * \brief  core0_sysrstreq_en configuration.
  * \param[in]  Status: new state of the misc_ctrl4.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void core0_sysrstreq_en(ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(0x12000000 + 0xc90) |= (1<<0);
    else
        REG32(0x12000000 + 0xc90) &= ~(1<<0);
}

/**
  * \brief  soc_clk_core1_stop_on_reset configuration.
  * \param[in]  Status: new state of the misc_ctrl5.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void soc_clk_core1_stop_on_reset(ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(0x12000000 + 0xc94) |= (1<<1);
    else
        REG32(0x12000000 + 0xc94) &= ~(1<<1);
}

/**
  * \brief  soc_clk_core0_stop_on_reset configuration.
  * \param[in]  Status: new state of the misc_ctrl5.
  *                     This parameter can be: ENABLE or DISABLE.
  */
void soc_clk_core0_stop_on_reset(ControlStatus Status)
{
    if(Status == ENABLE)
        REG32(0x12000000 + 0xc94) |= (1<<0);
    else
        REG32(0x12000000 + 0xc94) &= ~(1<<0);
}

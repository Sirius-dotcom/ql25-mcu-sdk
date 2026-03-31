#ifndef _NS_MISC_H
#define _NS_MISC_H
/*!
 * \file     ns_misc.h
 * \brief    This file contains all the functions prototypes for the MISC firmware
 */

#ifdef __cplusplus
 extern "C" {
#endif

#include "ns.h"
        
/* ========== Register List ========== */
#define RESET_CTRL0_OFS                        0x0UL 
#define RESET_CTRL1_OFS                        0x4UL 
#define RESET_CTRL2_OFS                        0x8UL 
#define RESET_CTRL3_OFS                        0xcUL 
#define SUBM_RESET_CTRL0_OFS                   0x20UL 
#define SUBM_RESET_CTRL1_OFS                   0x24UL 
#define SUBM_RESET_CTRL2_OFS                   0x28UL 
#define SUBM_RESET_CTRL3_OFS                   0x2cUL 
#define SUBM_RESET_CTRL4_OFS                   0x30UL 
#define SUBM_RESET_CTRL5_OFS                   0x34UL 
#define SUBM_RESET_CTRL6_OFS                   0x38UL 
#define SUBM_RESET_CTRL7_OFS                   0x3cUL 
#define SUBM_CLK_CTRL0_OFS                     0x40UL 
#define SUBM_CLK_CTRL1_OFS                     0x44UL 
#define SUBM_CLK_CTRL2_OFS                     0x48UL 
#define SUBM_CLK_CTRL3_OFS                     0x4cUL 
#define SUBM_CLK_CTRL4_OFS                     0x50UL 
#define SUBM_CLK_CTRL5_OFS                     0x54UL 
#define SUBM_CLK_CTRL6_OFS                     0x58UL 
#define SUBM_CLK_CTRL7_OFS                     0x5cUL 
#define CLK_CTRL0_MAIN_FAB_CLK_OFS           0x100UL 
#define CLK_CTRL1_TEST_FAB_CLK_OFS           0x104UL 
#define CLK_CTRL2_AHB_SLV0_CLK_OFS           0x108UL 
#define CLK_CTRL3_AHB_SLV1_CLK_OFS           0x10cUL 
#define CLK_CTRL4_AHB_SLV2_CLK_OFS           0x110UL 
#define CLK_CTRL5_AHB_SLV3_CLK_OFS           0x114UL 
#define CLK_CTRL6_APB_SLV0_CLK_OFS           0x118UL 
#define CLK_CTRL7_APB_SLV1_CLK_OFS           0x11cUL 
#define CLK_CTRL8_APB_SLV2_CLK_OFS           0x120UL 
#define CLK_CTRL9_USART0_CLK_OFS             0x124UL 
#define CLK_CTRL10_USART1_CLK_OFS            0x128UL 
#define CLK_CTRL11_USART2_CLK_OFS            0x12cUL 
#define CLK_CTRL12_USART3_CLK_OFS            0x130UL 
#define CLK_CTRL13_USART4_CLK_OFS            0x134UL 
#define CLK_CTRL142C0_CLK_OFS              0x138UL 
#define CLK_CTRL152C1_CLK_OFS              0x13cUL 
#define CLK_CTRL16_QSPI1_CLK_OFS             0x140UL 
#define CLK_CTRL17_QSPI2_CLK_OFS             0x144UL 
#define CLK_CTRL18_QSPI3_CLK_OFS             0x148UL 
#define CLK_CTRL19_ADVANCED_TIMER0_CLK_OFS   0x14cUL 
#define CLK_CTRL20_ADVANCED_TIMER1_CLK_OFS   0x150UL 
#define CLK_CTRL21_ADVANCED_TIMER2_CLK_OFS   0x154UL 
#define CLK_CTRL22_ADVANCED_TIMER3_CLK_OFS   0x158UL 
#define CLK_CTRL23_BASIC_TIMER0_CLK_OFS      0x15cUL 
#define CLK_CTRL24_BASIC_TIMER1_CLK_OFS      0x160UL 
#define CLK_CTRL25_BASIC_TIMER2_CLK_OFS      0x164UL 
#define CLK_CTRL26_BASIC_TIMER3_CLK_OFS      0x168UL 
#define CLK_CTRL27_BASIC_TIMER4_CLK_OFS      0x16cUL 
#define CLK_CTRL28_BASIC_TIMER5_CLK_OFS      0x170UL 
#define CLK_CTRL29_BASIC_TIMER6_CLK_OFS      0x174UL 
#define CLK_CTRL30_BASIC_TIMER7_CLK_OFS      0x178UL 
#define CLK_CTRL31_LGPIO0_CLK_OFS            0x17cUL 
#define CLK_CTRL32_LGPIO1_CLK_OFS            0x180UL 
#define CLK_CTRL33_LGPIO2_CLK_OFS            0x184UL 
#define CLK_CTRL34_LGPIO3_CLK_OFS            0x188UL 
#define CLK_CTRL35_WWDG0_CLK_OFS             0x18cUL 
#define CLK_CTRL36_WWDG1_CLK_OFS             0x190UL 
#define CLK_CTRL37_RTC0_CFG_CLK_OFS          0x194UL 
#define CLK_CTRL38_RTC1_CFG_CLK_OFS          0x198UL 
#define CLK_CTRL39_RTC2_CFG_CLK_OFS          0x19cUL 
#define CLK_CTRL40_XKAN0_CFG_CLK_OFS         0x1a0UL 
#define CLK_CTRL41_XKANTF_CLK_OFS           0x1a4UL        
#define XTL_CTRL0_SYS_CLK_OFS               0xa0UL        
#define XTL_CTRL1_RTC_CLK_OFS                  0xa4UL 
#define PLL_CTRL0_SYS_CLK_PLL_OFS              0x78UL 
#define PLL_CTRL1_SYS_ACLK_PLL_OFS             0x7cUL 
#define PLL_CTRL2_SYS_BCLK_PLL_OFS             0x80UL 
#define MISC_CTRL1_OFS                         0xc84UL 
#define MISC_CTRL2_OFS                         0xc88UL 
#define MISC_CTRL3_OFS                         0xc8cUL 
#define MISC_CTRL4_OFS                         0xc90UL 
#define MISC_CTRL5_OFS                         0xc94UL 
            
/* ==========reset_ctrl0========== */
                
#define RESET_CTRL0_SOC_C1_SYSRSTREQ                    BIT(7)                   /*!< soc_c1 */ 
                
#define RESET_CTRL0_SOC_C0_SYSRSTREQ                    BIT(6)                   /*!< soc_c0 */ 
                
#define RESET_CTRL0_SOC_MISC_CPU1_RST_N                 BIT(5)                /*!< soc_misc_cpu1_rst */ 
                
#define RESET_CTRL0_SOC_MISC_CPU0_RST_N                 BIT(4)                /*!< soc_misc_cpu0_rst */ 
                
#define RESET_CTRL0_WWDG1_WDOGRES                       BIT(3)                      /*!< wwdg1 */ 
                
#define RESET_CTRL0_WWDG0_WDOGRES                       BIT(2)                      /*!< wwdg0 */ 
                
#define RESET_CTRL0_POR_RST_N                           BIT(1)                          /*!< por_rst */ 
                
#define RESET_CTRL0_SYS_RST_N_IN                        BIT(0)                       /*!< sys_rst_n */ 
            
/* ==========reset_ctrl1========== */
            
/* ==========reset_ctrl2========== */
            
/* ==========reset_ctrl3========== */
                
#define RESET_CTRL3_CLEAR_RESET_STATUS                  BIT(31)                 /*!< clear_reset */ 
            
/* ==========subm_reset_ctrl0========== */
                
#define SUBM_RESET_CTRL0_SRAM1_31                            BIT(31)                           /*!< sram1 */ 
                
#define SUBM_RESET_CTRL0_SRAM0_30                            BIT(30)                           /*!< sram0 */ 
                
#define SUBM_RESET_CTRL0_XKAN1_29                            BIT(29)                           /*!< xkan1 */ 
                
#define SUBM_RESET_CTRL0_XKAN0_28                            BIT(28)                           /*!< xkan0 */ 
                
#define SUBM_RESET_CTRL0_LGPIO3_27                           BIT(27)                          /*!< lgpio3 */ 
                
#define SUBM_RESET_CTRL0_LGPIO2_26                           BIT(26)                          /*!< lgpio2 */ 
                
#define SUBM_RESET_CTRL0_LGPIO1_25                           BIT(25)                          /*!< lgpio1 */ 
                
#define SUBM_RESET_CTRL0_LGPIO0_24                           BIT(24)                          /*!< lgpio0 */ 
                
#define SUBM_RESET_CTRL0_BASIC_TIMER7_23                     BIT(23)                    /*!< basic_timer7 */ 
                
#define SUBM_RESET_CTRL0_BASIC_TIMER6_22                     BIT(22)                    /*!< basic_timer6 */ 
                
#define SUBM_RESET_CTRL0_BASIC_TIMER5_21                     BIT(21)                    /*!< basic_timer5 */ 
                
#define SUBM_RESET_CTRL0_BASIC_TIMER4_20                     BIT(20)                    /*!< basic_timer4 */ 
                
#define SUBM_RESET_CTRL0_BASIC_TIMER3_19                     BIT(19)                    /*!< basic_timer3 */ 
                
#define SUBM_RESET_CTRL0_BASIC_TIMER2_18                     BIT(18)                    /*!< basic_timer2 */ 
                
#define SUBM_RESET_CTRL0_BASIC_TIMER1_17                     BIT(17)                    /*!< basic_timer1 */ 
                
#define SUBM_RESET_CTRL0_BASIC_TIMER0_16                     BIT(16)                    /*!< basic_timer0 */ 
                
#define SUBM_RESET_CTRL0_ADVANCED_TIMER3_15                  BIT(15)                 /*!< advanced_timer3 */ 
                
#define SUBM_RESET_CTRL0_ADVANCED_TIMER2_14                  BIT(14)                 /*!< advanced_timer2 */ 
                
#define SUBM_RESET_CTRL0_ADVANCED_TIMER1_13                  BIT(13)                 /*!< advanced_timer1 */ 
                
#define SUBM_RESET_CTRL0_ADVANCED_TIMER0_12                  BIT(12)                 /*!< advanced_timer0 */ 
                
#define SUBM_RESET_CTRL0_QSPI3_11                            BIT(11)                           /*!< qspi3 */ 
                
#define SUBM_RESET_CTRL0_QSPI2_10                            BIT(10)                           /*!< qspi2 */ 
                
#define SUBM_RESET_CTRL0_QSPI1_9                             BIT(9)                            /*!< qspi1 */ 
                
#define SUBM_RESET_CTRL0_QSPI_XIP0_8                         BIT(8)                        /*!< qspi_xip0 */ 
                
#define SUBM_RESET_CTRL0_I2C1_7                              BIT(7)                             /*!< i2c1 */ 
                
#define SUBM_RESET_CTRL0_I2C0_6                              BIT(6)                             /*!< i2c0 */ 
                
#define SUBM_RESET_CTRL0_USART4_5                            BIT(5)                           /*!< usart4 */ 
                
#define SUBM_RESET_CTRL0_USART3_4                            BIT(4)                           /*!< usart3 */ 
                
#define SUBM_RESET_CTRL0_USART2_3                            BIT(3)                           /*!< usart2 */ 
                
#define SUBM_RESET_CTRL0_USART1_2                            BIT(2)                           /*!< usart1 */ 
                
#define SUBM_RESET_CTRL0_USART0_1                            BIT(1)                           /*!< usart0 */ 
                
#define SUBM_RESET_CTRL0_UDMA0_0                             BIT(0)                            /*!< udma0 */ 
            
/* ==========subm_reset_ctrl1========== */
                
#define SUBM_RESET_CTRL1_IOMUX_44                            BIT(12)                           /*!< iomux */ 
                
#define SUBM_RESET_CTRL1_IDU_43                              BIT(11)                             /*!< idu */ 
                
#define SUBM_RESET_CTRL1_APB_SLV2_RATIO_42                   BIT(10)                  /*!< apb_slv2_ratio */ 
                
#define SUBM_RESET_CTRL1_APB_SLV1_RATIO_41                   BIT(9)                  /*!< apb_slv1_ratio */ 
                
#define SUBM_RESET_CTRL1_APB_SLV0_RATIO_40                   BIT(8)                  /*!< apb_slv0_ratio */ 
                
#define SUBM_RESET_CTRL1_AHB_SLV3_RATIO_39                   BIT(7)                  /*!< ahb_slv3_ratio */ 
                
#define SUBM_RESET_CTRL1_AHB_SLV2_RATIO_38                   BIT(6)                  /*!< ahb_slv2_ratio */ 
                
#define SUBM_RESET_CTRL1_AHB_SLV1_RATIO_37                   BIT(5)                  /*!< ahb_slv1_ratio */ 
                
#define SUBM_RESET_CTRL1_AHB_SLV0_RATIO_36                   BIT(4)                  /*!< ahb_slv0_ratio */ 
                
#define SUBM_RESET_CTRL1_AHB_MST1_SYNC_35                    BIT(3)                   /*!< ahb_mst1_sync */ 
                
#define SUBM_RESET_CTRL1_AHB_MST0_SYNC_34                    BIT(2)                   /*!< ahb_mst0_sync */ 
                
#define SUBM_RESET_CTRL1_SRAM3_33                            BIT(1)                           /*!< sram3 */ 
                
#define SUBM_RESET_CTRL1_SRAM2_32                            BIT(0)                           /*!< sram2 */ 
            
/* ==========subm_reset_ctrl2========== */
            
/* ==========subm_reset_ctrl3========== */
            
/* ==========subm_reset_ctrl4========== */
            
/* ==========subm_reset_ctrl5========== */
            
/* ==========subm_reset_ctrl6========== */
            
/* ==========subm_reset_ctrl7========== */
            
/* ==========subm_clk_ctrl0========== */
                
#define SUBM_CLK_CTRL0_SRAM1_31                            BIT(31)                           /*!< sram1 */ 
                
#define SUBM_CLK_CTRL0_SRAM0_30                            BIT(30)                           /*!< sram0 */ 
                
#define SUBM_CLK_CTRL0_XKAN1_29                            BIT(29)                           /*!< xkan1 */ 
                
#define SUBM_CLK_CTRL0_XKAN0_28                            BIT(28)                           /*!< xkan0 */ 
                
#define SUBM_CLK_CTRL0_LGPIO3_27                           BIT(27)                          /*!< lgpio3 */ 
                
#define SUBM_CLK_CTRL0_LGPIO2_26                           BIT(26)                          /*!< lgpio2 */ 
                
#define SUBM_CLK_CTRL0_LGPIO1_25                           BIT(25)                          /*!< lgpio1 */ 
                
#define SUBM_CLK_CTRL0_LGPIO0_24                           BIT(24)                          /*!< lgpio0 */ 
                
#define SUBM_CLK_CTRL0_BASIC_TIMER7_23                     BIT(23)                    /*!< basic_timer7 */ 
                
#define SUBM_CLK_CTRL0_BASIC_TIMER6_22                     BIT(22)                    /*!< basic_timer6 */ 
                
#define SUBM_CLK_CTRL0_BASIC_TIMER5_21                     BIT(21)                    /*!< basic_timer5 */ 
                
#define SUBM_CLK_CTRL0_BASIC_TIMER4_20                     BIT(20)                    /*!< basic_timer4 */ 
                
#define SUBM_CLK_CTRL0_BASIC_TIMER3_19                     BIT(19)                    /*!< basic_timer3 */ 
                
#define SUBM_CLK_CTRL0_BASIC_TIMER2_18                     BIT(18)                    /*!< basic_timer2 */ 
                
#define SUBM_CLK_CTRL0_BASIC_TIMER1_17                     BIT(17)                    /*!< basic_timer1 */ 
                
#define SUBM_CLK_CTRL0_BASIC_TIMER0_16                     BIT(16)                    /*!< basic_timer0 */ 
                
#define SUBM_CLK_CTRL0_ADVANCED_TIMER3_15                  BIT(15)                 /*!< advanced_timer3 */ 
                
#define SUBM_CLK_CTRL0_ADVANCED_TIMER2_14                  BIT(14)                 /*!< advanced_timer2 */ 
                
#define SUBM_CLK_CTRL0_ADVANCED_TIMER1_13                  BIT(13)                 /*!< advanced_timer1 */ 
                
#define SUBM_CLK_CTRL0_ADVANCED_TIMER0_12                  BIT(12)                 /*!< advanced_timer0 */ 
                
#define SUBM_CLK_CTRL0_QSPI3_11                            BIT(11)                           /*!< qspi3 */ 
                
#define SUBM_CLK_CTRL0_QSPI2_10                            BIT(10)                           /*!< qspi2 */ 
                
#define SUBM_CLK_CTRL0_QSPI1_9                             BIT(9)                            /*!< qspi1 */ 
                
#define SUBM_CLK_CTRL0_QSPI_XIP0_8                         BIT(8)                        /*!< qspi_xip0 */ 
                
#define SUBM_CLK_CTRL0_I2C1_7                              BIT(7)                             /*!< i2c1 */ 
                
#define SUBM_CLK_CTRL0_I2C0_6                              BIT(6)                             /*!< i2c0 */ 
                
#define SUBM_CLK_CTRL0_USART4_5                            BIT(5)                           /*!< usart4 */ 
                
#define SUBM_CLK_CTRL0_USART3_4                            BIT(4)                           /*!< usart3 */ 
                
#define SUBM_CLK_CTRL0_USART2_3                            BIT(3)                           /*!< usart2 */ 
                
#define SUBM_CLK_CTRL0_USART1_2                            BIT(2)                           /*!< usart1 */ 
                
#define SUBM_CLK_CTRL0_USART0_1                            BIT(1)                           /*!< usart0 */ 
                
#define SUBM_CLK_CTRL0_UDMA0_0                             BIT(0)                            /*!< udma0 */ 
            
/* ==========subm_clk_ctrl1========== */
                
#define SUBM_CLK_CTRL1_IOMUX_44                            BIT(12)                           /*!< iomux */ 
                
#define SUBM_CLK_CTRL1_IDU_43                              BIT(11)                             /*!< idu */ 
                
#define SUBM_CLK_CTRL1_APB_SLV2_RATIO_42                   BIT(10)                  /*!< apb_slv2_ratio */ 
                
#define SUBM_CLK_CTRL1_APB_SLV1_RATIO_41                   BIT(9)                  /*!< apb_slv1_ratio */ 
                
#define SUBM_CLK_CTRL1_APB_SLV0_RATIO_40                   BIT(8)                  /*!< apb_slv0_ratio */ 
                
#define SUBM_CLK_CTRL1_AHB_SLV3_RATIO_39                   BIT(7)                  /*!< ahb_slv3_ratio */ 
                
#define SUBM_CLK_CTRL1_AHB_SLV2_RATIO_38                   BIT(6)                  /*!< ahb_slv2_ratio */ 
                
#define SUBM_CLK_CTRL1_AHB_SLV1_RATIO_37                   BIT(5)                  /*!< ahb_slv1_ratio */ 
                
#define SUBM_CLK_CTRL1_AHB_SLV0_RATIO_36                   BIT(4)                  /*!< ahb_slv0_ratio */ 
                
#define SUBM_CLK_CTRL1_AHB_MST1_SYNC_35                    BIT(3)                   /*!< ahb_mst1_sync */ 
                
#define SUBM_CLK_CTRL1_AHB_MST0_SYNC_34                    BIT(2)                   /*!< ahb_mst0_sync */ 
                
#define SUBM_CLK_CTRL1_SRAM3_33                            BIT(1)                           /*!< sram3 */ 
                
#define SUBM_CLK_CTRL1_SRAM2_32                            BIT(0)                           /*!< sram2 */ 
            
/* ==========subm_clk_ctrl2========== */
            
/* ==========subm_clk_ctrl3========== */
            
/* ==========subm_clk_ctrl4========== */
            
/* ==========subm_clk_ctrl5========== */
            
/* ==========subm_clk_ctrl6========== */
            
/* ==========subm_clk_ctrl7========== */
            
/* ==========clk_ctrl_main_fab_clk_i========== */
                
#define CLK_CTRL_MAIN_FAB_CLK_I_MAIN_FAB_CLK_I_DIV                  BITS(0,19)                 /*!< main_fab_clk_i */
            
/* ==========clk_ctrl_test_fab_clk_i========== */
                
#define CLK_CTRL_TEST_FAB_CLK_I_TEST_FAB_CLK_I_DIV                  BITS(0,19)                 /*!< test_fab_clk_i */
            
/* ==========clk_ctrl_ahb_slv0_clk_i========== */
                
#define CLK_CTRL_AHB_SLV0_CLK_I_AHB_SLV0_CLK_I_DIV                  BITS(0,19)                 /*!< ahb_slv0_clk_i */
            
/* ==========clk_ctrl_ahb_slv1_clk_i========== */
                
#define CLK_CTRL_AHB_SLV1_CLK_I_AHB_SLV1_CLK_I_DIV                  BITS(0,19)                 /*!< ahb_slv1_clk_i */
            
/* ==========clk_ctrl_ahb_slv2_clk_i========== */
                
#define CLK_CTRL_AHB_SLV2_CLK_I_AHB_SLV2_CLK_I_DIV                  BITS(0,19)                 /*!< ahb_slv2_clk_i */
            
/* ==========clk_ctrl_ahb_slv3_clk_i========== */
                
#define CLK_CTRL_AHB_SLV3_CLK_I_AHB_SLV3_CLK_I_DIV                  BITS(0,19)                 /*!< ahb_slv3_clk_i */
            
/* ==========clk_ctrl_apb_slv0_clk_i========== */
                
#define CLK_CTRL_APB_SLV0_CLK_I_APB_SLV0_CLK_I_DIV                  BITS(0,19)                 /*!< apb_slv0_clk_i */
            
/* ==========clk_ctrl_apb_slv1_clk_i========== */
                
#define CLK_CTRL_APB_SLV1_CLK_I_APB_SLV1_CLK_I_DIV                  BITS(0,19)                 /*!< apb_slv1_clk_i */
            
/* ==========clk_ctrl_apb_slv2_clk_i========== */
                
#define CLK_CTRL_APB_SLV2_CLK_I_APB_SLV2_CLK_I_DIV                  BITS(0,19)                 /*!< apb_slv2_clk_i */
            
/* ==========clk_ctrl_usart0_clk_i========== */
                
#define CLK_CTRL_USART0_CLK_I_USART0_CLK_I_DIV                    BITS(0,19)                   /*!< usart0_clk_i */
            
/* ==========clk_ctrl_usart1_clk_i========== */
                
#define CLK_CTRL_USART1_CLK_I_USART1_CLK_I_DIV                    BITS(0,19)                   /*!< usart1_clk_i */
            
/* ==========clk_ctrl_usart2_clk_i========== */
                
#define CLK_CTRL_USART2_CLK_I_USART2_CLK_I_DIV                    BITS(0,19)                   /*!< usart2_clk_i */
            
/* ==========clk_ctrl_usart3_clk_i========== */
                
#define CLK_CTRL_USART3_CLK_I_USART3_CLK_I_DIV                    BITS(0,19)                   /*!< usart3_clk_i */
            
/* ==========clk_ctrl_usart4_clk_i========== */
                
#define CLK_CTRL_USART4_CLK_I_USART4_CLK_I_DIV                    BITS(0,19)                   /*!< usart4_clk_i */
            
/* ==========clk_ctrl_i2c0_clk_i========== */
                
#define CLK_CTRL_I2C0_CLK_I_I2C0_CLK_I_DIV                      BITS(0,19)                     /*!< i2c0_clk_i */
            
/* ==========clk_ctrl_i2c1_clk_i========== */
                
#define CLK_CTRL_I2C1_CLK_I_I2C1_CLK_I_DIV                      BITS(0,19)                     /*!< i2c1_clk_i */
            
/* ==========clk_ctrl_qspi1_clk_i========== */
                
#define CLK_CTRL_QSPI1_CLK_I_QSPI1_CLK_I_DIV                     BITS(0,19)                    /*!< qspi1_clk_i */
            
/* ==========clk_ctrl_qspi2_clk_i========== */
                
#define CLK_CTRL_QSPI2_CLK_I_QSPI2_CLK_I_DIV                     BITS(0,19)                    /*!< qspi2_clk_i */
            
/* ==========clk_ctrl_qspi3_clk_i========== */
                
#define CLK_CTRL_QSPI3_CLK_I_QSPI3_CLK_I_DIV                     BITS(0,19)                    /*!< qspi3_clk_i */
            
/* ==========clk_ctrl_advanced_timer0_clk_i========== */
                
#define CLK_CTRL_ADVANCED_TIMER0_CLK_I_ADVANCED_TIMER0_CLK_I_DIV           BITS(0,19)          /*!< advanced_timer0_clk_i */
            
/* ==========clk_ctrl_advanced_timer1_clk_i========== */
                
#define CLK_CTRL_ADVANCED_TIMER1_CLK_I_ADVANCED_TIMER1_CLK_I_DIV           BITS(0,19)          /*!< advanced_timer1_clk_i */
            
/* ==========clk_ctrl_advanced_timer2_clk_i========== */
                
#define CLK_CTRL_ADVANCED_TIMER2_CLK_I_ADVANCED_TIMER2_CLK_I_DIV           BITS(0,19)          /*!< advanced_timer2_clk_i */
            
/* ==========clk_ctrl_advanced_timer3_clk_i========== */
                
#define CLK_CTRL_ADVANCED_TIMER3_CLK_I_ADVANCED_TIMER3_CLK_I_DIV           BITS(0,19)          /*!< advanced_timer3_clk_i */
            
/* ==========clk_ctrl_basic_timer0_clk_i========== */
                
#define CLK_CTRL_BASIC_TIMER0_CLK_I_BASIC_TIMER0_CLK_I_DIV              BITS(0,19)             /*!< basic_timer0_clk_i */
            
/* ==========clk_ctrl_basic_timer1_clk_i========== */
                
#define CLK_CTRL_BASIC_TIMER1_CLK_I_BASIC_TIMER1_CLK_I_DIV              BITS(0,19)             /*!< basic_timer1_clk_i */
            
/* ==========clk_ctrl_basic_timer2_clk_i========== */
                
#define CLK_CTRL_BASIC_TIMER2_CLK_I_BASIC_TIMER2_CLK_I_DIV              BITS(0,19)             /*!< basic_timer2_clk_i */
            
/* ==========clk_ctrl_basic_timer3_clk_i========== */
                
#define CLK_CTRL_BASIC_TIMER3_CLK_I_BASIC_TIMER3_CLK_I_DIV              BITS(0,19)             /*!< basic_timer3_clk_i */
            
/* ==========clk_ctrl_basic_timer4_clk_i========== */
                
#define CLK_CTRL_BASIC_TIMER4_CLK_I_BASIC_TIMER4_CLK_I_DIV              BITS(0,19)             /*!< basic_timer4_clk_i */
            
/* ==========clk_ctrl_basic_timer5_clk_i========== */
                
#define CLK_CTRL_BASIC_TIMER5_CLK_I_BASIC_TIMER5_CLK_I_DIV              BITS(0,19)             /*!< basic_timer5_clk_i */
            
/* ==========clk_ctrl_basic_timer6_clk_i========== */
                
#define CLK_CTRL_BASIC_TIMER6_CLK_I_BASIC_TIMER6_CLK_I_DIV              BITS(0,19)             /*!< basic_timer6_clk_i */
            
/* ==========clk_ctrl_basic_timer7_clk_i========== */
                
#define CLK_CTRL_BASIC_TIMER7_CLK_I_BASIC_TIMER7_CLK_I_DIV              BITS(0,19)             /*!< basic_timer7_clk_i */
            
/* ==========clk_ctrl_lgpio0_clk_i========== */
                
#define CLK_CTRL_LGPIO0_CLK_I_LGPIO0_CLK_I_DIV                    BITS(0,19)                   /*!< lgpio0_clk_i */
            
/* ==========clk_ctrl_lgpio1_clk_i========== */
                
#define CLK_CTRL_LGPIO1_CLK_I_LGPIO1_CLK_I_DIV                    BITS(0,19)                   /*!< lgpio1_clk_i */
            
/* ==========clk_ctrl_lgpio2_clk_i========== */
                
#define CLK_CTRL_LGPIO2_CLK_I_LGPIO2_CLK_I_DIV                    BITS(0,19)                   /*!< lgpio2_clk_i */
            
/* ==========clk_ctrl_lgpio3_clk_i========== */
                
#define CLK_CTRL_LGPIO3_CLK_I_LGPIO3_CLK_I_DIV                    BITS(0,19)                   /*!< lgpio3_clk_i */
            
/* ==========clk_ctrl_wwdg0_clk_i========== */
                
#define CLK_CTRL_WWDG0_CLK_I_WWDG0_CLK_I_DIV                     BITS(0,19)                    /*!< wwdg0_clk_i */
            
/* ==========clk_ctrl_wwdg1_clk_i========== */
                
#define CLK_CTRL_WWDG1_CLK_I_WWDG1_CLK_I_DIV                     BITS(0,19)                    /*!< wwdg1_clk_i */
            
/* ==========clk_ctrl_rtc0_cfg_clk_i========== */
                
#define CLK_CTRL_RTC0_CFG_CLK_I_RTC0_CFG_CLK_I_DIV                  BITS(0,19)                 /*!< rtc0_cfg_clk_i */
            
/* ==========clk_ctrl_rtc1_cfg_clk_i========== */
                
#define CLK_CTRL_RTC1_CFG_CLK_I_RTC1_CFG_CLK_I_DIV                  BITS(0,19)                 /*!< rtc1_cfg_clk_i */
            
/* ==========clk_ctrl_rtc2_cfg_clk_i========== */
                
#define CLK_CTRL_RTC2_CFG_CLK_I_RTC2_CFG_CLK_I_DIV                  BITS(0,19)                 /*!< rtc2_cfg_clk_i */
            
/* ==========clk_ctrl_xkan0_cfg_clk_i========== */
                
#define CLK_CTRL_XKAN0_CFG_CLK_I_XKAN0_CFG_CLK_I_DIV                 BITS(0,19)                /*!< xkan0_cfg_clk_i */
            
/* ==========clk_ctrl_xkan_intf_clk========== */
                
#define CLK_CTRL_XKAN_INTF_CLK_XKAN_INTF_CLK_DIV                   BITS(0,19)                  /*!< xkan_intf_clk */
            
/* ==========xtl_ctrl_sys_clk_in========== */
                
#define XTL_CTRL_SYS_CLK_IN_RESTART                             BIT(0)                            /*!< restart */ 
            
/* ==========xtl_ctrl_rtc_clk========== */
                
#define XTL_CTRL_RTC_CLK_RESTART                             BIT(0)                            /*!< restart */ 
            
/* ==========pll_ctrl_sys_clk_pll========== */
                
#define PLL_CTRL_SYS_CLK_PLL_N                                   BITS(0,7)                                  /*!< N */
                
#define PLL_CTRL_SYS_CLK_PLL_M                                   BITS(8,17)                                  /*!< M */
                
#define PLL_CTRL_SYS_CLK_PLL_OD                                  BITS(18,23)                                 /*!< od */
                
#define PLL_CTRL_SYS_CLK_PLL_BP                                  BIT(24)                                 /*!< bp */ 
                
#define PLL_CTRL_SYS_CLK_PLL_LOCK                                BIT(25)                               /*!< lock */ 
            
/* ==========pll_ctrl_sys_aclk_pll========== */
                
#define PLL_CTRL_SYS_ACLK_PLL_N                                   BITS(0,7)                                  /*!< N */
                
#define PLL_CTRL_SYS_ACLK_PLL_M                                   BITS(8,17)                                  /*!< M */
                
#define PLL_CTRL_SYS_ACLK_PLL_OD                                  BITS(18,23)                                 /*!< od */
                
#define PLL_CTRL_SYS_ACLK_PLL_BP                                  BIT(24)                                 /*!< bp */ 
                
#define PLL_CTRL_SYS_ACLK_PLL_LOCK                                BIT(25)                               /*!< lock */ 
            
/* ==========pll_ctrl_sys_bclk_pll========== */
                
#define PLL_CTRL_SYS_BCLK_PLL_N                                   BITS(0,7)                                  /*!< N */
                
#define PLL_CTRL_SYS_BCLK_PLL_M                                   BITS(8,17)                                  /*!< M */
                
#define PLL_CTRL_SYS_BCLK_PLL_OD                                  BITS(18,23)                                 /*!< od */
                
#define PLL_CTRL_SYS_BCLK_PLL_BP                                  BIT(24)                                 /*!< bp */ 
                
#define PLL_CTRL_SYS_BCLK_PLL_LOCK                                BIT(25)                               /*!< lock */ 
            
/* ==========misc_ctrl1========== */
                
#define MISC_CTRL1_DBG_JTAG_DWBYPASS                   BIT(1)                  /*!< dbg_jtag */ 
                
#define MISC_CTRL1_DBG_JTAG_DWEN                       BIT(0)                      /*!< dbg_jtag */ 
            
/* ==========misc_ctrl2========== */
                
#define MISC_CTRL2_CORE1_DBG_PD_D                      BIT(1)                     /*!< core1bg_pd */ 
                
#define MISC_CTRL2_CORE0_DBG_PD_D                      BIT(0)                     /*!< core0bg_pd */ 
            
/* ==========misc_ctrl3========== */
                
#define MISC_CTRL3_SOC_MISC_CPU1_RST_N                 BIT(1)                /*!< soc_misc_cpu1_rst */ 
                
#define MISC_CTRL3_SOC_MISC_CPU0_RST_N                 BIT(0)                /*!< soc_misc_cpu0_rst */ 
            
/* ==========misc_ctrl4========== */
                
#define MISC_CTRL4_CORE1_SYSRSTREQ_EN                  BIT(1)                 /*!< core1_sysrstreq */ 
                
#define MISC_CTRL4_CORE0_SYSRSTREQ_EN                  BIT(0)                 /*!< core0_sysrstreq */ 
            
/* ==========misc_ctrl5========== */
                
#define MISC_CTRL5_SOC_CLK_CORE1_STOP_ON_RESET         BIT(1)        /*!< soc_clk_core1_stop_on */ 
                
#define MISC_CTRL5_SOC_CLK_CORE0_STOP_ON_RESET         BIT(0)        /*!< soc_clk_core0_stop_on */ 

/* ==========reset_ctrl0========== */
                
/* ==========reset_ctrl1========== */
            
/* ==========reset_ctrl2========== */
            
/* ==========reset_ctrl3========== */
                
/* ==========subm_reset_ctrl0========== */
                
#define MISC_HAS_SRAM1_RST
                
#define MISC_HAS_SRAM0_RST
                
#define MISC_HAS_XKAN1_RST
                
#define MISC_HAS_XKAN0_RST
                
#define MISC_HAS_LGPIO3_RST
                
#define MISC_HAS_LGPIO2_RST
                
#define MISC_HAS_LGPIO1_RST
                
#define MISC_HAS_LGPIO0_RST
                
#define MISC_HAS_BASIC_TIMER7_RST
                
#define MISC_HAS_BASIC_TIMER6_RST
                
#define MISC_HAS_BASIC_TIMER5_RST
                
#define MISC_HAS_BASIC_TIMER4_RST
                
#define MISC_HAS_BASIC_TIMER3_RST
                
#define MISC_HAS_BASIC_TIMER2_RST
                
#define MISC_HAS_BASIC_TIMER1_RST
                
#define MISC_HAS_BASIC_TIMER0_RST
                
#define MISC_HAS_ADVANCED_TIMER3_RST
                
#define MISC_HAS_ADVANCED_TIMER2_RST
                
#define MISC_HAS_ADVANCED_TIMER1_RST
                
#define MISC_HAS_ADVANCED_TIMER0_RST
                
#define MISC_HAS_QSPI3_RST
                
#define MISC_HAS_QSPI2_RST
                
#define MISC_HAS_QSPI1_RST
                
#define MISC_HAS_QSPI_XIP0_RST
                
#define MISC_HAS_I2C1_RST
                
#define MISC_HAS_I2C0_RST
                
#define MISC_HAS_USART4_RST
                
#define MISC_HAS_USART3_RST
                
#define MISC_HAS_USART2_RST
                
#define MISC_HAS_USART1_RST
                
#define MISC_HAS_USART0_RST
                
#define MISC_HAS_UDMA0_RST
            
/* ==========subm_reset_ctrl1========== */
                
#define MISC_HAS_IOMUX_RST
                
#define MISC_HAS_IDU_RST
                
#define MISC_HAS_APB_SLV2_RATIO_RST
                
#define MISC_HAS_APB_SLV1_RATIO_RST
                
#define MISC_HAS_APB_SLV0_RATIO_RST
                
#define MISC_HAS_AHB_SLV3_RATIO_RST
                
#define MISC_HAS_AHB_SLV2_RATIO_RST
                
#define MISC_HAS_AHB_SLV1_RATIO_RST
                
#define MISC_HAS_AHB_SLV0_RATIO_RST
                
#define MISC_HAS_AHB_MST1_SYNC_RST
                
#define MISC_HAS_AHB_MST0_SYNC_RST
                
#define MISC_HAS_SRAM3_RST
                
#define MISC_HAS_SRAM2_RST
            
/* ==========subm_reset_ctrl2========== */
            
/* ==========subm_reset_ctrl3========== */
            
/* ==========subm_reset_ctrl4========== */
            
/* ==========subm_reset_ctrl5========== */
            
/* ==========subm_reset_ctrl6========== */
            
/* ==========subm_reset_ctrl7========== */
            
/* ==========subm_clk_ctrl0========== */
                
#define MISC_HAS_SRAM1_HAS_CLK
                
#define MISC_HAS_SRAM0_HAS_CLK
                
#define MISC_HAS_XKAN1_HAS_CLK
                
#define MISC_HAS_XKAN0_HAS_CLK
                
#define MISC_HAS_LGPIO3_HAS_CLK
                
#define MISC_HAS_LGPIO2_HAS_CLK
                
#define MISC_HAS_LGPIO1_HAS_CLK
                
#define MISC_HAS_LGPIO0_HAS_CLK
                
#define MISC_HAS_BASIC_TIMER7_HAS_CLK
                
#define MISC_HAS_BASIC_TIMER6_HAS_CLK
                
#define MISC_HAS_BASIC_TIMER5_HAS_CLK
                
#define MISC_HAS_BASIC_TIMER4_HAS_CLK
                
#define MISC_HAS_BASIC_TIMER3_HAS_CLK
                
#define MISC_HAS_BASIC_TIMER2_HAS_CLK
                
#define MISC_HAS_BASIC_TIMER1_HAS_CLK
                
#define MISC_HAS_BASIC_TIMER0_HAS_CLK
                
#define MISC_HAS_ADVANCED_TIMER3_HAS_CLK
                
#define MISC_HAS_ADVANCED_TIMER2_HAS_CLK
                
#define MISC_HAS_ADVANCED_TIMER1_HAS_CLK
                
#define MISC_HAS_ADVANCED_TIMER0_HAS_CLK
                
#define MISC_HAS_QSPI3_HAS_CLK
                
#define MISC_HAS_QSPI2_HAS_CLK
                
#define MISC_HAS_QSPI1_HAS_CLK
                
#define MISC_HAS_QSPI_XIP0_HAS_CLK
                
#define MISC_HAS_I2C1_HAS_CLK
                
#define MISC_HAS_I2C0_HAS_CLK
                
#define MISC_HAS_USART4_HAS_CLK
                
#define MISC_HAS_USART3_HAS_CLK
                
#define MISC_HAS_USART2_HAS_CLK
                
#define MISC_HAS_USART1_HAS_CLK
                
#define MISC_HAS_USART0_HAS_CLK
                
#define MISC_HAS_UDMA0_HAS_CLK
            
/* ==========subm_clk_ctrl1========== */
                
#define MISC_HAS_IOMUX_HAS_CLK
                
#define MISC_HAS_IDU_HAS_CLK
                
#define MISC_HAS_APB_SLV2_RATIO_HAS_CLK
                
#define MISC_HAS_APB_SLV1_RATIO_HAS_CLK
                
#define MISC_HAS_APB_SLV0_RATIO_HAS_CLK
                
#define MISC_HAS_AHB_SLV3_RATIO_HAS_CLK
                
#define MISC_HAS_AHB_SLV2_RATIO_HAS_CLK
                
#define MISC_HAS_AHB_SLV1_RATIO_HAS_CLK
                
#define MISC_HAS_AHB_SLV0_RATIO_HAS_CLK
                
#define MISC_HAS_AHB_MST1_SYNC_HAS_CLK
                
#define MISC_HAS_AHB_MST0_SYNC_HAS_CLK
                
#define MISC_HAS_SRAM3_HAS_CLK
                
#define MISC_HAS_SRAM2_HAS_CLK
            
/* ==========subm_clk_ctrl2========== */
            
/* ==========subm_clk_ctrl3========== */
            
/* ==========subm_clk_ctrl4========== */
            
/* ==========subm_clk_ctrl5========== */
            
/* ==========subm_clk_ctrl6========== */
            
/* ==========subm_clk_ctrl7========== */
            
/* ==========clk_ctrl_main_fab_clk_i========== */
                
#define MISC_HAS_MAIN_FAB_CLK_DIV
            
/* ==========clk_ctrl_test_fab_clk_i========== */
                
#define MISC_HAS_TEST_FAB_CLK_DIV
            
/* ==========clk_ctrl_ahb_slv0_clk_i========== */
                
#define MISC_HAS_AHB_SLV0_CLK_DIV
            
/* ==========clk_ctrl_ahb_slv1_clk_i========== */
                
#define MISC_HAS_AHB_SLV1_CLK_DIV
            
/* ==========clk_ctrl_ahb_slv2_clk_i========== */
                
#define MISC_HAS_AHB_SLV2_CLK_DIV
            
/* ==========clk_ctrl_ahb_slv3_clk_i========== */
                
#define MISC_HAS_AHB_SLV3_CLK_DIV
            
/* ==========clk_ctrl_apb_slv0_clk_i========== */
                
#define MISC_HAS_APB_SLV0_CLK_DIV
            
/* ==========clk_ctrl_apb_slv1_clk_i========== */
                
#define MISC_HAS_APB_SLV1_CLK_DIV
            
/* ==========clk_ctrl_apb_slv2_clk_i========== */
                
#define MISC_HAS_APB_SLV2_CLK_DIV
            
/* ==========clk_ctrl_usart0_clk_i========== */
                
#define MISC_HAS_USART0_CLK_DIV
            
/* ==========clk_ctrl_usart1_clk_i========== */
                
#define MISC_HAS_USART1_CLK_DIV
            
/* ==========clk_ctrl_usart2_clk_i========== */
                
#define MISC_HAS_USART2_CLK_DIV
            
/* ==========clk_ctrl_usart3_clk_i========== */
                
#define MISC_HAS_USART3_CLK_DIV
            
/* ==========clk_ctrl_usart4_clk_i========== */
                
#define MISC_HAS_USART4_CLK_DIV
            
/* ==========clk_ctrl_i2c0_clk_i========== */
                
#define MISC_HAS_I2C0_CLK_DIV
            
/* ==========clk_ctrl_i2c1_clk_i========== */
                
#define MISC_HAS_I2C1_CLK_DIV
            
/* ==========clk_ctrl_qspi1_clk_i========== */
                
#define MISC_HAS_QSPI1_CLK_DIV
            
/* ==========clk_ctrl_qspi2_clk_i========== */
                
#define MISC_HAS_QSPI2_CLK_DIV
            
/* ==========clk_ctrl_qspi3_clk_i========== */
                
#define MISC_HAS_QSPI3_CLK_DIV
            
/* ==========clk_ctrl_advanced_timer0_clk_i========== */
                
#define MISC_HAS_ADVANCED_TIMER0_CLK_DIV
            
/* ==========clk_ctrl_advanced_timer1_clk_i========== */
                
#define MISC_HAS_ADVANCED_TIMER1_CLK_DIV
            
/* ==========clk_ctrl_advanced_timer2_clk_i========== */
                
#define MISC_HAS_ADVANCED_TIMER2_CLK_DIV
            
/* ==========clk_ctrl_advanced_timer3_clk_i========== */
                
#define MISC_HAS_ADVANCED_TIMER3_CLK_DIV
            
/* ==========clk_ctrl_basic_timer0_clk_i========== */
                
#define MISC_HAS_BASIC_TIMER0_CLK_DIV
            
/* ==========clk_ctrl_basic_timer1_clk_i========== */
                
#define MISC_HAS_BASIC_TIMER1_CLK_DIV
            
/* ==========clk_ctrl_basic_timer2_clk_i========== */
                
#define MISC_HAS_BASIC_TIMER2_CLK_DIV
            
/* ==========clk_ctrl_basic_timer3_clk_i========== */
                
#define MISC_HAS_BASIC_TIMER3_CLK_DIV
            
/* ==========clk_ctrl_basic_timer4_clk_i========== */
                
#define MISC_HAS_BASIC_TIMER4_CLK_DIV
            
/* ==========clk_ctrl_basic_timer5_clk_i========== */
                
#define MISC_HAS_BASIC_TIMER5_CLK_DIV
            
/* ==========clk_ctrl_basic_timer6_clk_i========== */
                
#define MISC_HAS_BASIC_TIMER6_CLK_DIV
            
/* ==========clk_ctrl_basic_timer7_clk_i========== */
                
#define MISC_HAS_BASIC_TIMER7_CLK_DIV
            
/* ==========clk_ctrl_lgpio0_clk_i========== */
                
#define MISC_HAS_LGPIO0_CLK_DIV
            
/* ==========clk_ctrl_lgpio1_clk_i========== */
                
#define MISC_HAS_LGPIO1_CLK_DIV
            
/* ==========clk_ctrl_lgpio2_clk_i========== */
                
#define MISC_HAS_LGPIO2_CLK_DIV
            
/* ==========clk_ctrl_lgpio3_clk_i========== */
                
#define MISC_HAS_LGPIO3_CLK_DIV
            
/* ==========clk_ctrl_wwdg0_clk_i========== */
                
#define MISC_HAS_WWDG0_CLK_DIV
            
/* ==========clk_ctrl_wwdg1_clk_i========== */
                
#define MISC_HAS_WWDG1_CLK_DIV
            
/* ==========clk_ctrl_rtc0_cfg_clk_i========== */
                
#define MISC_HAS_RTC0_CFG_CLK_DIV
            
/* ==========clk_ctrl_rtc1_cfg_clk_i========== */
                
#define MISC_HAS_RTC1_CFG_CLK_DIV
            
/* ==========clk_ctrl_rtc2_cfg_clk_i========== */
                
#define MISC_HAS_RTC2_CFG_CLK_DIV
            
/* ==========clk_ctrl_xkan0_cfg_clk_i========== */
                
#define MISC_HAS_XKAN0_CFG_CLK_DIV
            
/* ==========clk_ctrl_xkan_intf_clk========== */
                
#define MISC_HAS_XKAN_INTF_CLK_DIV
            
/* ==========xtl_ctrl_sys_clk_in========== */
                
/* ==========xtl_ctrl_rtc_clk========== */
                
/* ==========pll_ctrl_sys_clk_pll========== */
                
/* ==========pll_ctrl_sys_aclk_pll========== */
                
/* ==========pll_ctrl_sys_bclk_pll========== */
                
/* ==========misc_ctrl1========== */
                
/* ==========misc_ctrl2========== */
                
/* ==========misc_ctrl3========== */
                
/* ==========misc_ctrl4========== */
                
/* ==========misc_ctrl5========== */
                
void sram1_set_rst(ControlStatus status);
                
void sram0_set_rst(ControlStatus status);
                
void xkan1_set_rst(ControlStatus status);
                
void xkan0_set_rst(ControlStatus status);
                
void lgpio3_set_rst(ControlStatus status);
                
void lgpio2_set_rst(ControlStatus status);
                
void lgpio1_set_rst(ControlStatus status);
                
void lgpio0_set_rst(ControlStatus status);
                
void basic_timer7_set_rst(ControlStatus status);
                
void basic_timer6_set_rst(ControlStatus status);
                
void basic_timer5_set_rst(ControlStatus status);
                
void basic_timer4_set_rst(ControlStatus status);
                
void basic_timer3_set_rst(ControlStatus status);
                
void basic_timer2_set_rst(ControlStatus status);
                
void basic_timer1_set_rst(ControlStatus status);
                
void basic_timer0_set_rst(ControlStatus status);
                
void advanced_timer3_set_rst(ControlStatus status);
                
void advanced_timer2_set_rst(ControlStatus status);
                
void advanced_timer1_set_rst(ControlStatus status);
                
void advanced_timer0_set_rst(ControlStatus status);
                
void qspi3_set_rst(ControlStatus status);
                
void qspi2_set_rst(ControlStatus status);
                
void qspi1_set_rst(ControlStatus status);
                
void qspi_xip0_set_rst(ControlStatus status);
                
void i2c1_set_rst(ControlStatus status);
                
void i2c0_set_rst(ControlStatus status);
                
void usart4_set_rst(ControlStatus status);
                
void usart3_set_rst(ControlStatus status);
                
void usart2_set_rst(ControlStatus status);
                
void usart1_set_rst(ControlStatus status);
                
void usart0_set_rst(ControlStatus status);
                
void udma0_set_rst(ControlStatus status);
            
void iomux_set_rst(ControlStatus status);
                
void idu_set_rst(ControlStatus status);
                
void apb_slv2_ratio_set_rst(ControlStatus status);
                
void apb_slv1_ratio_set_rst(ControlStatus status);
                
void apb_slv0_ratio_set_rst(ControlStatus status);
                
void ahb_slv3_ratio_set_rst(ControlStatus status);
                
void ahb_slv2_ratio_set_rst(ControlStatus status);
                
void ahb_slv1_ratio_set_rst(ControlStatus status);
                
void ahb_slv0_ratio_set_rst(ControlStatus status);
                
void ahb_mst1_sync_set_rst(ControlStatus status);
                
void ahb_mst0_sync_set_rst(ControlStatus status);
                
void sram3_set_rst(ControlStatus status);
                
void sram2_set_rst(ControlStatus status);
            
void sram1_clk_en(ControlStatus status);
                
void sram0_clk_en(ControlStatus status);
                
void xkan1_clk_en(ControlStatus status);
                
void xkan0_clk_en(ControlStatus status);
                
void lgpio3_clk_en(ControlStatus status);
                
void lgpio2_clk_en(ControlStatus status);
                
void lgpio1_clk_en(ControlStatus status);
                
void lgpio0_clk_en(ControlStatus status);
                
void basic_timer7_clk_en(ControlStatus status);
                
void basic_timer6_clk_en(ControlStatus status);
                
void basic_timer5_clk_en(ControlStatus status);
                
void basic_timer4_clk_en(ControlStatus status);
                
void basic_timer3_clk_en(ControlStatus status);
                
void basic_timer2_clk_en(ControlStatus status);
                
void basic_timer1_clk_en(ControlStatus status);
                
void basic_timer0_clk_en(ControlStatus status);
                
void advanced_timer3_clk_en(ControlStatus status);
                
void advanced_timer2_clk_en(ControlStatus status);
                
void advanced_timer1_clk_en(ControlStatus status);
                
void advanced_timer0_clk_en(ControlStatus status);
                
void qspi3_clk_en(ControlStatus status);
                
void qspi2_clk_en(ControlStatus status);
                
void qspi1_clk_en(ControlStatus status);
                
void qspi_xip0_clk_en(ControlStatus status);
                
void i2c1_clk_en(ControlStatus status);
                
void i2c0_clk_en(ControlStatus status);
                
void usart4_clk_en(ControlStatus status);
                
void usart3_clk_en(ControlStatus status);
                
void usart2_clk_en(ControlStatus status);
                
void usart1_clk_en(ControlStatus status);
                
void usart0_clk_en(ControlStatus status);
                
void udma0_clk_en(ControlStatus status);
            
void iomux_clk_en(ControlStatus status);
                
void idu_clk_en(ControlStatus status);
                
void apb_slv2_ratio_clk_en(ControlStatus status);
                
void apb_slv1_ratio_clk_en(ControlStatus status);
                
void apb_slv0_ratio_clk_en(ControlStatus status);
                
void ahb_slv3_ratio_clk_en(ControlStatus status);
                
void ahb_slv2_ratio_clk_en(ControlStatus status);
                
void ahb_slv1_ratio_clk_en(ControlStatus status);
                
void ahb_slv0_ratio_clk_en(ControlStatus status);
                
void ahb_mst1_sync_clk_en(ControlStatus status);
                
void ahb_mst0_sync_clk_en(ControlStatus status);
                
void sram3_clk_en(ControlStatus status);
                
void sram2_clk_en(ControlStatus status);
            
void main_fab_clk_div(uint32_t div_val) ;
            
void test_fab_clk_div(uint32_t div_val) ;
            
void ahb_slv0_clk_div(uint32_t div_val) ;
            
void ahb_slv1_clk_div(uint32_t div_val) ;
            
void ahb_slv2_clk_div(uint32_t div_val) ;
            
void ahb_slv3_clk_div(uint32_t div_val) ;
            
void apb_slv0_clk_div(uint32_t div_val) ;
            
void apb_slv1_clk_div(uint32_t div_val) ;
            
void apb_slv2_clk_div(uint32_t div_val) ;
            
void usart0_clk_div(uint32_t div_val) ;
            
void usart1_clk_div(uint32_t div_val) ;
            
void usart2_clk_div(uint32_t div_val) ;
            
void usart3_clk_div(uint32_t div_val) ;
            
void usart4_clk_div(uint32_t div_val) ;
            
void i2c0_clk_div(uint32_t div_val) ;
            
void i2c1_clk_div(uint32_t div_val) ;
            
void qspi1_clk_div(uint32_t div_val) ;
            
void qspi2_clk_div(uint32_t div_val) ;
            
void qspi3_clk_div(uint32_t div_val) ;
            
void advanced_timer0_clk_div(uint32_t div_val) ;
            
void advanced_timer1_clk_div(uint32_t div_val) ;
            
void advanced_timer2_clk_div(uint32_t div_val) ;
            
void advanced_timer3_clk_div(uint32_t div_val) ;
            
void basic_timer0_clk_div(uint32_t div_val) ;
            
void basic_timer1_clk_div(uint32_t div_val) ;
            
void basic_timer2_clk_div(uint32_t div_val) ;
            
void basic_timer3_clk_div(uint32_t div_val) ;
            
void basic_timer4_clk_div(uint32_t div_val) ;
            
void basic_timer5_clk_div(uint32_t div_val) ;
            
void basic_timer6_clk_div(uint32_t div_val) ;
            
void basic_timer7_clk_div(uint32_t div_val) ;
            
void lgpio0_clk_div(uint32_t div_val) ;
            
void lgpio1_clk_div(uint32_t div_val) ;
            
void lgpio2_clk_div(uint32_t div_val) ;
            
void lgpio3_clk_div(uint32_t div_val) ;
            
void wwdg0_clk_div(uint32_t div_val) ;
            
void wwdg1_clk_div(uint32_t div_val) ;
            
void rtc0_cfg_clk_div(uint32_t div_val) ;
            
void rtc1_cfg_clk_div(uint32_t div_val) ;
            
void rtc2_cfg_clk_div(uint32_t div_val) ;
            
void xkan0_cfg_clk_div(uint32_t div_val) ;
            
void xkan_intf_clk_div(uint32_t div_val) ;
            
#define GET_STATUS_SOC_C1_SYSRSTREQ
uint32_t get_status_soc_c1_sysrstreq(void);
uint32_t Get_status_soc_c1_sysrstreq(void);
#define GET_STATUS_SOC_C0_SYSRSTREQ
uint32_t get_status_soc_c0_sysrstreq(void);
uint32_t Get_status_soc_c0_sysrstreq(void);
#define GET_STATUS_SOC_MISC_CPU1_RST_N
uint32_t get_status_soc_misc_cpu1_rst_n(void);
uint32_t Get_status_soc_misc_cpu1_rst_n(void);
#define GET_STATUS_SOC_MISC_CPU0_RST_N
uint32_t get_status_soc_misc_cpu0_rst_n(void);
uint32_t Get_status_soc_misc_cpu0_rst_n(void);
#define GET_STATUS_WWDG1_WDOGRES
uint32_t get_status_wwdg1_wdogres(void);
uint32_t Get_status_wwdg1_WDOGRES(void);
#define GET_STATUS_WWDG0_WDOGRES
uint32_t get_status_wwdg0_wdogres(void);
uint32_t Get_status_wwdg0_WDOGRES(void);
#define GET_STATUS_POR_RST_N
uint32_t get_status_por_rst_n(void);
uint32_t Get_status_por_rst_n(void);
#define GET_STATUS_SYS_RST_N_IN
uint32_t get_status_sys_rst_n_in(void);
uint32_t Get_status_sys_rst_n_in(void);
            
#define CLEAR_RESET_STATUS
void clear_reset_status(void);
            
void pll_ctrl0_sys_clk_pll(ControlStatus status);
uint32_t pll_ctrl0_sys_clk_pll_lock(void);
void pll_ctrl0_sys_clk_pll_bp(ControlStatus Status);
            
void pll_ctrl1_sys_aclk_pll(ControlStatus status);
uint32_t pll_ctrl1_sys_aclk_pll_lock(void);
void pll_ctrl1_sys_aclk_pll_bp(ControlStatus Status);
            
void pll_ctrl2_sys_bclk_pll(ControlStatus status);
uint32_t pll_ctrl2_sys_bclk_pll_lock(void);
void pll_ctrl2_sys_bclk_pll_bp(ControlStatus Status);
            
#define DBG_JTAG_DWBYPASS
void dbg_jtag_dwbypass(ControlStatus status);
                    
#define DBG_JTAG_DWEN
uint32_t dbg_jtag_dwen(void);                        
            
#define CORE1_DBG_PD_D
void core1_dbg_pd_d(ControlStatus status);
                    
#define CORE0_DBG_PD_D
void core0_dbg_pd_d(ControlStatus status);
            
#define SOC_MISC_CPU1_RST_N
void soc_misc_cpu1_rst_n(ControlStatus status);
                    
#define SOC_MISC_CPU0_RST_N
void soc_misc_cpu0_rst_n(ControlStatus status);
            
#define CORE1_SYSRSTREQ_EN
void core1_sysrstreq_en(ControlStatus status);
                    
#define CORE0_SYSRSTREQ_EN
void core0_sysrstreq_en(ControlStatus status);
            
#define SOC_CLK_CORE1_STOP_ON_RESET
void soc_clk_core1_stop_on_reset(ControlStatus status);
                    
#define SOC_CLK_CORE0_STOP_ON_RESET
void soc_clk_core0_stop_on_reset(ControlStatus status);

void all_clk_div(uint32_t div_val);

#ifdef __cplusplus
}
#endif

#endif

/* _NS_MISC_H */

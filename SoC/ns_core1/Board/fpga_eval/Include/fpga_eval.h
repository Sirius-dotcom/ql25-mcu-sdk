
#ifndef _BOARD_FPGA_EVAL_H_
#define _BOARD_FPGA_EVAL_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "ns_sdk_soc.h"

/* exported types */
typedef enum 
{
    LED1 = 0,
    LED2 = 1,
    LED3 = 2,
} led_typedef_enum;

typedef enum 
{
    KEY_A = 0,
    KEY_B = 1,
    KEY_C = 2,
    KEY_D = 3,
    KEY_CET = 4
} key_typedef_enum;

typedef enum 
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
} keymode_typedef_enum;

#define SOC_BUTTON_1_IRQn           SOC_INT49_IRQn
#define SOC_BUTTON_2_IRQn           SOC_INT50_IRQn

#define SOC_BUTTON_1_HANDLER        eclic_irq49_handler
#define SOC_BUTTON_2_HANDLER        eclic_irq50_handler

#define SOC_LED_RED_GPIO_OFS        19
#define SOC_LED_GREEN_GPIO_OFS      21
#define SOC_LED_BLUE_GPIO_OFS       22
#define SOC_BUTTON_1_GPIO_OFS       30
#define SOC_BUTTON_2_GPIO_OFS       31

#define SOC_LED_RED_GPIO_MASK       (1<<SOC_LED_RED_GPIO_OFS)
#define SOC_LED_GREEN_GPIO_MASK     (1<<SOC_LED_GREEN_GPIO_OFS)
#define SOC_LED_BLUE_GPIO_MASK      (1<<SOC_LED_BLUE_GPIO_OFS)
#define SOC_BUTTON_1_GPIO_MASK      (1<<SOC_BUTTON_1_GPIO_OFS)
#define SOC_BUTTON_2_GPIO_MASK      (1<<SOC_BUTTON_2_GPIO_OFS)
#define SOC_BUTTON_GPIO_MASK        (SOC_BUTTON_1_GPIO_MASK | SOC_BUTTON_2_GPIO_MASK)
#define SOC_LED_GPIO_MASK           (SOC_LED_RED_GPIO_MASK | SOC_LED_GREEN_GPIO_MASK | SOC_LED_BLUE_GPIO_MASK)

#define ADVANCED_TIMER0_TIM_ETR_0_PAD_SEL      PAD15
                
#define ADVANCED_TIMER0_TIM_CH1_PAD_SEL      PAD17
                
#define ADVANCED_TIMER0_TIM_CH2_PAD_SEL      PAD18
                
#define ADVANCED_TIMER0_TIM_CH3_PAD_SEL      PAD19
                
#define ADVANCED_TIMER0_TIM_CH4_PAD_SEL      PAD20
                
#define ADVANCED_TIMER0_TIM_BKIN_0_PAD_SEL      PAD5
                
#define ADVANCED_TIMER0_TIM_BKIN2_0_PAD_SEL      PAD6
                
#define ADVANCED_TIMER0_TIM_CH1N_PAD_SEL      PAD21
                
#define ADVANCED_TIMER0_TIM_CH2N_PAD_SEL      PAD22
                
#define ADVANCED_TIMER0_TIM_CH3N_PAD_SEL      PAD23
                
#define ADVANCED_TIMER1_TIM_ETR_0_PAD_SEL      PAD15
                
#define ADVANCED_TIMER1_TIM_CH1_PAD_SEL      PAD17
                
#define ADVANCED_TIMER1_TIM_CH2_PAD_SEL      PAD18
                
#define ADVANCED_TIMER1_TIM_CH3_PAD_SEL      PAD19
                
#define ADVANCED_TIMER1_TIM_CH4_PAD_SEL      PAD20
                
#define ADVANCED_TIMER1_TIM_BKIN_0_PAD_SEL      PAD15
                
#define ADVANCED_TIMER1_TIM_BKIN2_0_PAD_SEL      PAD16
                
#define ADVANCED_TIMER1_TIM_CH1N_PAD_SEL      PAD21
                
#define ADVANCED_TIMER1_TIM_CH2N_PAD_SEL      PAD22
                
#define ADVANCED_TIMER1_TIM_CH3N_PAD_SEL      PAD23
                
#define ADVANCED_TIMER2_TIM_ETR_0_PAD_SEL      PAD15
                
#define ADVANCED_TIMER2_TIM_CH1_PAD_SEL      PAD17
                
#define ADVANCED_TIMER2_TIM_CH2_PAD_SEL      PAD18
                
#define ADVANCED_TIMER2_TIM_CH3_PAD_SEL      PAD19
                
#define ADVANCED_TIMER2_TIM_CH4_PAD_SEL      PAD20
                
#define ADVANCED_TIMER2_TIM_BKIN_0_PAD_SEL      PAD25
                
#define ADVANCED_TIMER2_TIM_BKIN2_0_PAD_SEL      PAD26
                
#define ADVANCED_TIMER2_TIM_CH1N_PAD_SEL      PAD21
                
#define ADVANCED_TIMER2_TIM_CH2N_PAD_SEL      PAD22
                
#define ADVANCED_TIMER2_TIM_CH3N_PAD_SEL      PAD23
                
#define ADVANCED_TIMER3_TIM_ETR_0_PAD_SEL      PAD15
                
#define ADVANCED_TIMER3_TIM_CH1_PAD_SEL      PAD17
                
#define ADVANCED_TIMER3_TIM_CH2_PAD_SEL      PAD18
                
#define ADVANCED_TIMER3_TIM_CH3_PAD_SEL      PAD19
                
#define ADVANCED_TIMER3_TIM_CH4_PAD_SEL      PAD20
                
#define ADVANCED_TIMER3_TIM_BKIN_0_PAD_SEL      PAD35
                
#define ADVANCED_TIMER3_TIM_BKIN2_0_PAD_SEL      PAD36
                
#define ADVANCED_TIMER3_TIM_CH1N_PAD_SEL      PAD21
                
#define ADVANCED_TIMER3_TIM_CH2N_PAD_SEL      PAD22
                
#define ADVANCED_TIMER3_TIM_CH3N_PAD_SEL      PAD23
                
#define LGPIO0_GPIO0_PAD_SEL      PAD13
                
#define LGPIO0_GPIO1_PAD_SEL      PAD14
                
#define LGPIO0_GPIO2_PAD_SEL      PAD15
                
#define LGPIO0_GPIO3_PAD_SEL      PAD16
                
#define LGPIO0_GPIO4_PAD_SEL      PAD44
                
#define LGPIO0_GPIO5_PAD_SEL      PAD45
                
#define LGPIO0_GPIO6_PAD_SEL      PAD46
                
#define LGPIO0_GPIO7_PAD_SEL      PAD47
                
#define LGPIO1_GPIO0_PAD_SEL      PAD13
                
#define LGPIO1_GPIO1_PAD_SEL      PAD14
                
#define LGPIO1_GPIO2_PAD_SEL      PAD15
                
#define LGPIO1_GPIO3_PAD_SEL      PAD16
                
#define LGPIO1_GPIO4_PAD_SEL      PAD52
                
#define LGPIO1_GPIO5_PAD_SEL      PAD53
                
#define LGPIO1_GPIO6_PAD_SEL      PAD54
                
#define LGPIO1_GPIO7_PAD_SEL      PAD55
                
#define LGPIO2_GPIO0_PAD_SEL      PAD13
                
#define LGPIO2_GPIO1_PAD_SEL      PAD14
                
#define LGPIO2_GPIO2_PAD_SEL      PAD15
                
#define LGPIO2_GPIO3_PAD_SEL      PAD16
                
#define LGPIO2_GPIO4_PAD_SEL      PAD60
                
#define LGPIO2_GPIO5_PAD_SEL      PAD61
                
#define LGPIO2_GPIO6_PAD_SEL      PAD62
                
#define LGPIO2_GPIO7_PAD_SEL      PAD63
                
#define LGPIO3_GPIO0_PAD_SEL      PAD13
                
#define LGPIO3_GPIO1_PAD_SEL      PAD14
                
#define LGPIO3_GPIO2_PAD_SEL      PAD15
                
#define LGPIO3_GPIO3_PAD_SEL      PAD16
                
#define LGPIO3_GPIO4_PAD_SEL      PAD68
                
#define LGPIO3_GPIO5_PAD_SEL      PAD69
                
#define LGPIO3_GPIO6_PAD_SEL      PAD70
                
#define LGPIO3_GPIO7_PAD_SEL      PAD71
                
#define XKAN1_XKAN_TX_PAD_SEL      PAD17
                
#define XKAN1_XKAN_RX_PAD_SEL      PAD18
                
#define USART1_RX_PAD_SEL      PAD6
                
#define USART1_TX_PAD_SEL      PAD7
                
#define USART1_SCLK_PAD_SEL      PAD10
                
#define USART1_CTS_PAD_SEL      PAD8
                
#define USART1_RTS_PAD_SEL      PAD9
                
#define USART2_RX_PAD_SEL      PAD27
                
#define USART2_TX_PAD_SEL      PAD28
                
#define USART2_SCLK_PAD_SEL      PAD31
                
#define USART2_CTS_PAD_SEL      PAD29
                
#define USART2_RTS_PAD_SEL      PAD30
                
#define USART3_RX_PAD_SEL      PAD6
                
#define USART3_TX_PAD_SEL      PAD7
                
#define USART3_SCLK_PAD_SEL      PAD10
                
#define USART3_CTS_PAD_SEL      PAD8
                
#define USART3_RTS_PAD_SEL      PAD9
                
#define USART4_RX_PAD_SEL      PAD27
                
#define USART4_TX_PAD_SEL      PAD28
                
#define USART4_SCLK_PAD_SEL      PAD31
                
#define USART4_CTS_PAD_SEL      PAD29
                
#define USART4_RTS_PAD_SEL      PAD30
                
#define I2C1_SCL_PAD_SEL      PAD11
                
#define I2C1_SDA_PAD_SEL      PAD12
                
#define I2C1_SMBALERT_PAD_SEL      PAD13
                
#define QSPI1_SCK_PAD_SEL      PAD0
                
#define QSPI1_CS_0_PAD_SEL      PAD1
                
#define QSPI1_DQ_0_PAD_SEL      PAD2
                
#define QSPI1_DQ_1_PAD_SEL      PAD3
                
#define QSPI1_DQ_2_PAD_SEL      PAD4
                
#define QSPI1_DQ_3_PAD_SEL      PAD5
                
#define QSPI2_SCK_PAD_SEL      PAD21
                
#define QSPI2_CS_0_PAD_SEL      PAD22
                
#define QSPI2_DQ_0_PAD_SEL      PAD23
                
#define QSPI2_DQ_1_PAD_SEL      PAD24
                
#define QSPI2_DQ_2_PAD_SEL      PAD25
                
#define QSPI2_DQ_3_PAD_SEL      PAD26
                
#define QSPI3_SCK_PAD_SEL      PAD0
                
#define QSPI3_CS_0_PAD_SEL      PAD1
                
#define QSPI3_DQ_0_PAD_SEL      PAD2
                
#define QSPI3_DQ_1_PAD_SEL      PAD3
                
#define QSPI3_DQ_2_PAD_SEL      PAD4
                
#define QSPI3_DQ_3_PAD_SEL      PAD5
                
#define ADVANCED_TIMER0_TIM_ETR_0_PAD_SEL      PAD15
                
#define ADVANCED_TIMER0_TIM_CH1_PAD_SEL      PAD17
                
#define ADVANCED_TIMER0_TIM_CH2_PAD_SEL      PAD18
                
#define ADVANCED_TIMER0_TIM_CH3_PAD_SEL      PAD19
                
#define ADVANCED_TIMER0_TIM_CH4_PAD_SEL      PAD20
                
#define ADVANCED_TIMER0_TIM_BKIN_0_PAD_SEL      PAD5
                
#define ADVANCED_TIMER0_TIM_BKIN2_0_PAD_SEL      PAD6
                
#define ADVANCED_TIMER0_TIM_CH1N_PAD_SEL      PAD21
                
#define ADVANCED_TIMER0_TIM_CH2N_PAD_SEL      PAD22
                
#define ADVANCED_TIMER0_TIM_CH3N_PAD_SEL      PAD23
                
#define ADVANCED_TIMER1_TIM_ETR_0_PAD_SEL      PAD15
                
#define ADVANCED_TIMER1_TIM_CH1_PAD_SEL      PAD17
                
#define ADVANCED_TIMER1_TIM_CH2_PAD_SEL      PAD18
                
#define ADVANCED_TIMER1_TIM_CH3_PAD_SEL      PAD19
                
#define ADVANCED_TIMER1_TIM_CH4_PAD_SEL      PAD20
                
#define ADVANCED_TIMER1_TIM_BKIN_0_PAD_SEL      PAD15
                
#define ADVANCED_TIMER1_TIM_BKIN2_0_PAD_SEL      PAD16
                
#define ADVANCED_TIMER1_TIM_CH1N_PAD_SEL      PAD21
                
#define ADVANCED_TIMER1_TIM_CH2N_PAD_SEL      PAD22
                
#define ADVANCED_TIMER1_TIM_CH3N_PAD_SEL      PAD23
                
#define ADVANCED_TIMER2_TIM_ETR_0_PAD_SEL      PAD15
                
#define ADVANCED_TIMER2_TIM_CH1_PAD_SEL      PAD17
                
#define ADVANCED_TIMER2_TIM_CH2_PAD_SEL      PAD18
                
#define ADVANCED_TIMER2_TIM_CH3_PAD_SEL      PAD19
                
#define ADVANCED_TIMER2_TIM_CH4_PAD_SEL      PAD20
                
#define ADVANCED_TIMER2_TIM_BKIN_0_PAD_SEL      PAD25
                
#define ADVANCED_TIMER2_TIM_BKIN2_0_PAD_SEL      PAD26
                
#define ADVANCED_TIMER2_TIM_CH1N_PAD_SEL      PAD21
                
#define ADVANCED_TIMER2_TIM_CH2N_PAD_SEL      PAD22
                
#define ADVANCED_TIMER2_TIM_CH3N_PAD_SEL      PAD23
                
#define ADVANCED_TIMER3_TIM_ETR_0_PAD_SEL      PAD15
                
#define ADVANCED_TIMER3_TIM_CH1_PAD_SEL      PAD17
                
#define ADVANCED_TIMER3_TIM_CH2_PAD_SEL      PAD18
                
#define ADVANCED_TIMER3_TIM_CH3_PAD_SEL      PAD19
                
#define ADVANCED_TIMER3_TIM_CH4_PAD_SEL      PAD20
                
#define ADVANCED_TIMER3_TIM_BKIN_0_PAD_SEL      PAD35
                
#define ADVANCED_TIMER3_TIM_BKIN2_0_PAD_SEL      PAD36
                
#define ADVANCED_TIMER3_TIM_CH1N_PAD_SEL      PAD21
                
#define ADVANCED_TIMER3_TIM_CH2N_PAD_SEL      PAD22
                
#define ADVANCED_TIMER3_TIM_CH3N_PAD_SEL      PAD23
                
#define LGPIO0_GPIO0_PAD_SEL      PAD13
                
#define LGPIO0_GPIO1_PAD_SEL      PAD14
                
#define LGPIO0_GPIO2_PAD_SEL      PAD15
                
#define LGPIO0_GPIO3_PAD_SEL      PAD16
                
#define LGPIO0_GPIO4_PAD_SEL      PAD44
                
#define LGPIO0_GPIO5_PAD_SEL      PAD45
                
#define LGPIO0_GPIO6_PAD_SEL      PAD46
                
#define LGPIO0_GPIO7_PAD_SEL      PAD47
                
#define LGPIO1_GPIO0_PAD_SEL      PAD13
                
#define LGPIO1_GPIO1_PAD_SEL      PAD14
                
#define LGPIO1_GPIO2_PAD_SEL      PAD15
                
#define LGPIO1_GPIO3_PAD_SEL      PAD16
                
#define LGPIO1_GPIO4_PAD_SEL      PAD52
                
#define LGPIO1_GPIO5_PAD_SEL      PAD53
                
#define LGPIO1_GPIO6_PAD_SEL      PAD54
                
#define LGPIO1_GPIO7_PAD_SEL      PAD55
                
#define LGPIO2_GPIO0_PAD_SEL      PAD13
                
#define LGPIO2_GPIO1_PAD_SEL      PAD14
                
#define LGPIO2_GPIO2_PAD_SEL      PAD15
                
#define LGPIO2_GPIO3_PAD_SEL      PAD16
                
#define LGPIO2_GPIO4_PAD_SEL      PAD60
                
#define LGPIO2_GPIO5_PAD_SEL      PAD61
                
#define LGPIO2_GPIO6_PAD_SEL      PAD62
                
#define LGPIO2_GPIO7_PAD_SEL      PAD63
                
#define LGPIO3_GPIO0_PAD_SEL      PAD13
                
#define LGPIO3_GPIO1_PAD_SEL      PAD14
                
#define LGPIO3_GPIO2_PAD_SEL      PAD15
                
#define LGPIO3_GPIO3_PAD_SEL      PAD16
                
#define LGPIO3_GPIO4_PAD_SEL      PAD68
                
#define LGPIO3_GPIO5_PAD_SEL      PAD69
                
#define LGPIO3_GPIO6_PAD_SEL      PAD70
                
#define LGPIO3_GPIO7_PAD_SEL      PAD71
                
#define XKAN1_XKAN_TX_PAD_SEL      PAD17
                
#define XKAN1_XKAN_RX_PAD_SEL      PAD18
                
#define USART1_RX_PAD_SEL      PAD6
                
#define USART1_TX_PAD_SEL      PAD7
                
#define USART1_SCLK_PAD_SEL      PAD10
                
#define USART1_CTS_PAD_SEL      PAD8
                
#define USART1_RTS_PAD_SEL      PAD9
                
#define USART2_RX_PAD_SEL      PAD27
                
#define USART2_TX_PAD_SEL      PAD28
                
#define USART2_SCLK_PAD_SEL      PAD31
                
#define USART2_CTS_PAD_SEL      PAD29
                
#define USART2_RTS_PAD_SEL      PAD30
                
#define USART3_RX_PAD_SEL      PAD6
                
#define USART3_TX_PAD_SEL      PAD7
                
#define USART3_SCLK_PAD_SEL      PAD10
                
#define USART3_CTS_PAD_SEL      PAD8
                
#define USART3_RTS_PAD_SEL      PAD9
                
#define USART4_RX_PAD_SEL      PAD27
                
#define USART4_TX_PAD_SEL      PAD28
                
#define USART4_SCLK_PAD_SEL      PAD31
                
#define USART4_CTS_PAD_SEL      PAD29
                
#define USART4_RTS_PAD_SEL      PAD30
                
#define I2C1_SCL_PAD_SEL      PAD11
                
#define I2C1_SDA_PAD_SEL      PAD12
                
#define I2C1_SMBALERT_PAD_SEL      PAD13
                
#define QSPI1_SCK_PAD_SEL      PAD0
                
#define QSPI1_CS_0_PAD_SEL      PAD1
                
#define QSPI1_DQ_0_PAD_SEL      PAD2
                
#define QSPI1_DQ_1_PAD_SEL      PAD3
                
#define QSPI1_DQ_2_PAD_SEL      PAD4
                
#define QSPI1_DQ_3_PAD_SEL      PAD5
                
#define QSPI2_SCK_PAD_SEL      PAD21
                
#define QSPI2_CS_0_PAD_SEL      PAD22
                
#define QSPI2_DQ_0_PAD_SEL      PAD23
                
#define QSPI2_DQ_1_PAD_SEL      PAD24
                
#define QSPI2_DQ_2_PAD_SEL      PAD25
                
#define QSPI2_DQ_3_PAD_SEL      PAD26
                
#define QSPI3_SCK_PAD_SEL      PAD0
                
#define QSPI3_CS_0_PAD_SEL      PAD1
                
#define QSPI3_DQ_0_PAD_SEL      PAD2
                
#define QSPI3_DQ_1_PAD_SEL      PAD3
                
#define QSPI3_DQ_2_PAD_SEL      PAD4
                
#define QSPI3_DQ_3_PAD_SEL      PAD5

void Nuclei_EvalLedInit(led_typedef_enum lednum);
void Nuclei_EvalLedOn(led_typedef_enum lednum);
void Nuclei_EvalLedOff(led_typedef_enum lednum);
void Nuclei_EvalLedToggle(led_typedef_enum lednum);

#ifdef __cplusplus
}
#endif
#endif

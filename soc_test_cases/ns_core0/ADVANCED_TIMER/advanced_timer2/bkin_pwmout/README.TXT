/*!
    \file  readme.txt
    \brief description of the advanced timer brake input PWM test

    \version 2019-6-5, V1.0.0
*/
    Function description:
    This demo demonstrates how to configure the ADVANCED_TIMER2 peripheral with brake input (BKIN) functionality.
    The timer generates 4 PWM signals with different duty cycles and monitors brake input pins.
    
    Timer Configuration:
    - Timer period: 100 counts
    - CH1 duty cycle: 10% (10/100)
    - CH2 duty cycle: 20% (20/100)
    - CH3 duty cycle: 30% (30/100)
    - CH4 duty cycle: 40% (40/100)
    - Output mode: PWM mode 1 for all channels
    - Break inputs: BKIN and BKIN2 enabled
    
    Test Logic:
    The program initializes brake input interrupts for BKIN and BKIN2 pins.
    It waits for both brake interrupts to be triggered (bkin == 0 && bkin2 == 0).
    When both interrupts occur, the test passes. Otherwise, it fails.
    
    IO Connection information:
    - ADVANCED_TIMER2_TIM_BKIN_0: Brake input 0
    - ADVANCED_TIMER2_TIM_BKIN2_0: Brake input 2
    - ADVANCED_TIMER2_TIM_CH1: PWM output (10% duty)
    - ADVANCED_TIMER2_TIM_CH2: PWM output (20% duty)
    - ADVANCED_TIMER2_TIM_CH3: PWM output (30% duty)
    - ADVANCED_TIMER2_TIM_CH4: PWM output (40% duty)
    
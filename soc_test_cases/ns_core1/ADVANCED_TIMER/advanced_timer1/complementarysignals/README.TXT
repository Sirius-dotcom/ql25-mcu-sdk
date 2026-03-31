/*!
    \file  readme.txt
    \brief description of the advanced timer complementary signals test

    \version 2019-6-5, V1.0.0
*/
    Function description:
    This demo shows how to configure the ADVANCED_TIMER1 peripheral to generate three complementary PWM signals.
    The timer generates complementary signal pairs with different duty cycles.
    
    Timer Configuration:
    - Timer period: 15999 counts
    - Output mode: PWM mode 1 for all channels
    
    Complementary Signal Configuration:
    - CH1 duty cycle: 25% (3999/15999), CH1N complementary signal: 75%
    - CH2 duty cycle: 50% (7999/15999), CH2N complementary signal: 50%
    - CH3 duty cycle: 75% (11999/15999), CH3N complementary signal: 25%
    
    IO Connection information:
    - ADVANCED_TIMER1_TIM_CH1: PWM output (25% duty)
    - ADVANCED_TIMER1_TIM_CH1N: Complementary output (75% duty)
    - ADVANCED_TIMER1_TIM_CH2: PWM output (50% duty)
    - ADVANCED_TIMER1_TIM_CH2N: Complementary output (50% duty)
    - ADVANCED_TIMER1_TIM_CH3: PWM output (75% duty)
    - ADVANCED_TIMER1_TIM_CH3N: Complementary output (25% duty)
    
    Test Procedure:
    Verify that each channel and its complementary signal have the correct duty cycle relationships.
    
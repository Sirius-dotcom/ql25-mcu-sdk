/*!
    \file  readme.txt
    \brief description of the advanced timer ctest
    \version 2025-12-03, V2.0.0
*/
    Function description:
    This demo shows how to configure the ADVANCED_TIMER2 peripheral to auto test.
    ADVANCED_TIMER2 CH1N outputs a PWM signal with pulse width of 500 counts.
    ADVANCED_TIMER2 CH2 and CH4 perform input capture sampling of the PWM signal.
    
    Timer Configuration:
    - Timer period: 1500 counts
    - CH1 pulse value: 500 counts
    - Output mode: PWM mode 2
    - Input capture: CH2 and CH4 with no prescaler and no filter
    
    Test Logic:
    The program reads the capture values from TIMER_CH_2 and TIMER_CH_4.
    It verifies that TIMER_CH_4 - TIMER_CH_2 equals CH1_COUNT (500).
    If the difference matches 500 counts within 20 attempts, test passes.
    Otherwise, test fails.

    Test result:
    - "pass": When TIMER_CH_4 - TIMER_CH_2 == 500 (CH1_COUNT)
    - "fail": When the difference doesn't match after 20 iterations

    IO Connection information:
    - ADVANCED_TIMER2_TIM_CH1N: PWM output signal
    - ADVANCED_TIMER2_TIM_CH2: Input capture channel
    - ADVANCED_TIMER2_TIM_CH4: Input capture channel
    
    Note: Both CH2 and CH4 capture the same PWM signal from CH1N output.

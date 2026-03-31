/*!
    \file  README.TXT
    \brief Advanced Timer ETR External Clock Mode Test

    \version 2025-12-03, V2.0.0
*/
    Function Description:
    This example demonstrates how to configure ADVANCED_TIMER2 to use external trigger input (ETR) as clock source.
    The timer operates in external clock mode with PWM output generation.
    
    Timer Configuration:

    - Timer Clock: External clock from ETR pin (SystemCoreClock/8 in simulation)
    - External Trigger: ETR signal as clock source
    - Slave Mode: External Clock Mode 1 (SMS = 111)
    - Trigger Source: ETR filtered input (TS = ETRF)
    - PWM Period: 16000 counts
    - Clock Division: CKD1 (no division)

    PWM Output Configuration:
    - Channel 1: PWM mode 1, 25% duty cycle (4000/16000)
    - Channel 2: PWM mode 1, 50% duty cycle (8000/16000)
    - Complementary outputs: Disabled for all channels
    - Shadow registers: Disabled (immediate update)
    - Primary output: Enabled

    External Clock Settings:
    - ETR Input: ADV_ETR pin configured as external clock source
    - External Filter: Configured through ETRF signal path
    - Clock Source: External edges drive timer counting
    - Direction: Up-counting based on external clock edges

    IO Connection Information:
    - ADVANCED_TIMER2_TIM_ETR: External clock input (source of timer clock)
    - ADVANCED_TIMER2_TIM_CH1: PWM output (25% duty cycle)
    - ADVANCED_TIMER2_TIM_CH2: PWM output (50% duty cycle)

    Test Procedure:
    1. Configure timer with external clock mode settings
    2. Set ETR as trigger source and slave mode selection
    3. Configure PWM channels with desired duty cycles
    4. Apply external clock signal to ETR pin
    5. Observe PWM outputs synchronized to external clock
    6. Verify PWM frequencies and duty cycles

    Expected Results:
    - Timer counts driven by external ETR clock edges
    - Stable PWM generation without internal clock dependency

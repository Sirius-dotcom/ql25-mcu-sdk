/*!
    \file  README.TXT
    \brief Advanced Timer Center-Aligned PWM Mode 1 Test

    \version 2025-12-03, V2.0.0
*/

Function Description:
This example demonstrates how to configure ADVANCED_TIMER0 in center-aligned PWM mode 1.
The test validates the timer's ability to generate center-aligned PWM signals with different duty cycles on all four channels.

Timer Configuration:
- Timer Mode: Center-aligned mode 3 (up/down counting)
- Period: 1000 counts
- Prescaler: 799 (800-1) for hardware, 0 for simulation
- Clock Division: CKD1 (no division)
- Counter Direction: Center-aligned (alternating up/down counting)
- Auto-reload Preload: Disabled

PWM Output Configuration:
- Channel 1: PWM mode 1, duty cycle 20% (CCR1=200, ARR=1000)
- Channel 2: PWM mode 1, duty cycle 40% (CCR2=400, ARR=1000)
- Channel 3: PWM mode 1, duty cycle 60% (CCR3=600, ARR=1000)
- Channel 4: PWM mode 1, duty cycle 80% (CCR4=800, ARR=1000)
- Output Mode: PWM mode 1 (output active when counter < CCR)
- Output State: Enabled for all channels
- Output Polarity: Normal polarity for CH1/CH2, inverted for CH3/CH4

Interrupt Generation:
- Interrupt Sources: Channel 1-4 compare match interrupts
- Interrupt Priority: Level 1, non-vector mode
- Interrupt Handling: Each interrupt sets corresponding state flag
- Test Logic: Pass when all 4 channel interrupts are received
- Interrupt Timing: Occurs during both up and down counting when counter matches CCR

PWM Characteristics:
- Channel 1: Compare value 200 (20% duty), interrupts at counter = 200
- Channel 2: Compare value 400 (40% duty), interrupts at counter = 400
- Channel 3: Compare value 600 (60% duty), interrupts at counter = 600
- Channel 4: Compare value 800 (80% duty), interrupts at counter = 800
- Center-aligned: Symmetrical PWM waveforms around counter midpoint (1000)

IO Connection Information:
- ADVANCED_TIMER0_TIM_CH1: Center-aligned PWM output (20% duty)
- ADVANCED_TIMER0_TIM_CH2: Center-aligned PWM output (40% duty)
- ADVANCED_TIMER0_TIM_CH3: Center-aligned PWM output (60% duty)
- ADVANCED_TIMER0_TIM_CH4: Center-aligned PWM output (80% duty)
- Primary Output: Enabled for all channels

Test Procedure:
1. Configure timer with prescaler (799 for hardware, 0 for simulation)
2. Set period to 1000 counts
3. Configure center-aligned mode 3 (up/down counting)
4. Set compare values: CH1=200, CH2=400, CH3=600, CH4=800
5. Configure all channels in PWM mode 1
6. Enable compare match interrupts for all channels
7. Start timer - counter counts 0¡ú1000¡ú0 repeatedly
8. Monitor state flags in interrupt handler
9. Verify test passes when all 4 state flags are set

Expected Results:
- Timer counts up from 0 to 1000, then down from 1000 to 0 repeatedly
- PWM outputs show symmetrical waveforms centered around count value 1000
- CH1 interrupt occurs when counter = 200 (during both up and down counting)
- CH2 interrupt occurs when counter = 400 (during both up and down counting)
- CH3 interrupt occurs when counter = 600 (during both up and down counting)
- CH4 interrupt occurs when counter = 800 (during both up and down counting)
- All 4 PWM channels generate correct duty cycles (20%, 40%, 60%, 80%)
- Test passes when all 4 state flags (state0, state1, state2, state3) are set

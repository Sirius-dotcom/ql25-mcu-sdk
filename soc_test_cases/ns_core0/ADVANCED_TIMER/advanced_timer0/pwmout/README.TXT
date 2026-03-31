/*!
    \file  README.TXT
    \brief Advanced Timer PWM Output with Fast Delay Test

    \version 2025-12-03, V2.0.0
*/

Function Description:
This example demonstrates how to configure ADVANCED_TIMER0 in PWM mode with fast output delay functionality.
The test validates the timer's ability to generate PWM signals with different duty cycles and fast output delay features for improved response time.

Timer Configuration:
- Timer Mode: Edge-aligned PWM generation
- Period: 16000 counts
- Prescaler: 7 (8-1) for hardware, 0 for simulation
- Clock Division: CKD1 (tDTS = TimerClock/8 for delay calculation)
- Counter Direction: Up-counting
- Auto-reload Preload: Enabled
- Primary Output: Enabled
- Repetition Counter: Disabled (RCR = 0)

PWM Output Configuration:
- Channel 1: PWM mode 1, duty cycle 25% (CCR1=4000, ARR=16000)
- Channel 2: PWM mode 1, duty cycle 25% (CCR2=4000, ARR=16000)
- Channel 3: PWM mode 1, duty cycle 50% (CCR3=8000, ARR=16000)
- Channel 4: PWM mode 1, duty cycle 50% (CCR4=8000, ARR=16000)
- Output Mode: PWM mode 1 (output active when counter < CCR)
- Output State: Enabled for all channels
- Output Polarity: Normal polarity for CH1/CH2, inverted for CH3/CH4

PWM Characteristics:
- Channel 1: Compare value 4000 (25% duty = 4000/16000) with fast delay
- Channel 2: Compare value 4000 (25% duty = 4000/16000)
- Channel 3: Compare value 8000 (50% duty = 8000/16000) with fast delay
- Channel 4: Compare value 8000 (50% duty = 8000/16000)
- Edge-aligned: Standard PWM generation with precise timing control

IO Connection Information:
- ADVANCED_TIMER0_TIM_CH1: PWM output with fast delay (25% duty)
- ADVANCED_TIMER0_TIM_CH2: Standard PWM output (25% duty)
- ADVANCED_TIMER0_TIM_CH3: PWM output with fast delay (50% duty)
- ADVANCED_TIMER0_TIM_CH4: Standard PWM output (50% duty)
- Primary Output: Enabled for all channels

Test Procedure:
1. Configure timer with prescaler (7 for hardware, 0 for simulation)
2. Set period to 16000 counts
3. Configure all channels in PWM mode 1 with respective duty cycles
4. Enable fast output delay for channels 1 and 3 (500 cycles delay)
5. Enable auto-reload preload for smooth operation
6. Start timer and generate PWM signals
7. Use oscilloscope to verify PWM waveforms and duty cycles
8. Measure fast output delay on channels 1 and 3 (500 delay cycles)
9. Verify stable PWM output on all channels

Expected Results:
- All PWM channels output with correct duty cycles (25% and 50%)
- Channels 1 and 3 show fast output delay functionality (500 cycles)
- Channels 2 and 4 operate without fast delay
- Stable PWM output with no jitter on all channels
- Fast output delay improves response time for critical applications

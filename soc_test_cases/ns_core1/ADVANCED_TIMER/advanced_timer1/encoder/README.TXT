/*!
    \file  README.TXT
    \brief Advanced Timer Encoder Interface Test Description

    \version 2025-12-03, V2.0.0
*/
    Function Description:
    This example demonstrates how to configure ADVANCED_TIMER1 peripheral for encoder interface testing.
    The timer operates in encoder mode 3 for motor control and position detection applications.
    
    Timer Configuration:
    - Timer Mode: Encoder Mode 3 (SMS = 011, TI1FP2 + TI2FP1 counting on both edges)
    - Auto-reload Period: 32 counts
    - Clock Division: CKD1 
    - Counter Direction: Bidirectional encoding

    Encoder Input Configuration:
    - Channel 1: TI1FP1 input capture, no prescaler, 8-sample filter
    - Channel 2: TI2FP2 input capture, no prescaler, 8-sample filter
    - Trigger Source: TI1F_ED (TI1 edge detection)
    - Filter Setting: FL1_N8 (8 sampling clocks filter)

    Test Logic:
    - Count forward (cnt_f) and reverse (cnt_z) pulses via update interrupt
    - Timer increments/decrements based on encoder input sequence
    - Test passes when both cnt_f and cnt_z reach 13 pulses
    - Validates bidirectional counting and direction detection

    IO Connection Information:
    - ADVANCED_TIMER1_TIM_CH1: Encoder Phase A input (TI1) - configured as input capture
    - ADVANCED_TIMER1_TIM_CH2: Encoder Phase B input (TI2) - configured as input capture

    Test Procedure:
    1. Configure timer for encoder mode 3 operation
    2. Set up input capture channels with appropriate filtering
    3. Enable update interrupt for direction monitoring
    4. Apply encoder signals to CH1 and CH2 inputs
    5. Monitor forward (cnt_f) and reverse (cnt_z) pulse counts
    6. Verify test passes when both counts reach 13

    Expected Results:
    - Correct bidirectional counting based on encoder signal sequence
    - Accurate direction detection via DIR flag monitoring
    - Interrupt handler properly counts direction changes
    - Test completion when both forward and reverse counts reach 13

/*!
    \file  README.TXT
    \brief Advanced Timer ITR1 Internal Trigger Test

    \version 2025-12-03, V2.0.0
*/
    Function Description:
    This example demonstrates how to configure ADVANCED_TIMER1 using TI1FP1 as internal trigger source.
    The timer operates in trigger mode with master-slave synchronization and repetition counter.
    
    Timer Configuration:
    - Timer Mode: Edge-aligned PWM generation
    - Period: 1000 counts
    - Clock Division: CKD1 (no division)
    - Counter Direction: Up-counting

    Trigger Configuration:
    - Trigger Source: TI1FP1 (filtered TI1 input)
    - Slave Mode: Trigger mode (SMS = 110)
    - Master-Slave Mode: Enabled (MSM = 1)
    - Master Output Trigger: TRGO output (MMS = 001)
    - Repetition Counter: 5 repetitions before update event

    PWM Output Configuration:

    Synchronization Features:
    - TI1FP1 serves as trigger input for slave mode operation
    - Master-slave mode ensures proper synchronization between timers
    - Repetition counter delays update events by 5 cycles
    - TRGO output provides trigger signal to other peripherals

    IO Connection Information:
    - ADVANCED_TIMER1_TIM_CH1: Trigger input (TI1FP1 source)
    - TRGO: Master trigger output to other peripherals

    Test Procedure:
    1. Configure timer with TI1FP1 as trigger source
    2. Set up slave mode for trigger-driven operation
    3. Enable master-slave mode and configure TRGO output
    4. Set repetition counter to 5 for delayed update events
    5. Apply trigger signal to CH1 input
    6. Observe TRGO output

    Expected Results:
    - Update events occur every 5 trigger pulses
    - TRGO output provides proper timing signals

/*!
    \file  README.TXT
    \brief Advanced Timer External Interrupt Test

    \version 2025-12-03, V2.0.0
*/
    Function Description:
    This example demonstrates how to configure ADVANCED_TIMER0 for external interrupt testing.
    The test validates capture interrupt functionality and overflow detection on multiple channels.
    **Hardware Setup: Connect CH1/CH2 channels to CH3/CH4 channels respectively (CH1?CH3, CH2?CH4).**
    
    Timer Configuration:
    - Timer Mode: Edge-aligned PWM generation
    - Period: 16000 counts
    - Clock Division: CKD1 (no division)
    - Counter Direction: Up-counting

    Input Capture Configuration:
    - Channel 1: TRC input source, 8-sample filter, no prescaler
    - Channel 2: TRC input source, 8-sample filter, no prescaler, inverted polarity
    - Trigger Source: TI1F_ED (TI1 edge detection)
    - Interrupts: CC1IE and CC2IE enabled
    - Compare Values: Both channels set to 12000

    PWM Output Configuration:
    - Channel 3: PWM mode 1, high polarity, 75% duty cycle (12000/16000)
    - Channel 4: PWM mode 1, high polarity, 75% duty cycle (12000/16000)
    - Complementary Outputs: Disabled for both channels
    - Shadow Registers: Disabled (immediate update)

    Interrupt Handling:
    - CC1 Interrupt: Detects first capture, then checks for overflow (CC1OF)
    - CC2 Interrupt: Detects first capture, then checks for overflow (CC2OF)
    - Test Logic: Requires both normal capture and overflow events
    - Status Tracking: cnt_buf, cnt_buf1, cnt_buf2, cnt_buf3 track interrupt progression

    IO Connection Information:
    **IMPORTANT: Hardware Loopback Connections Required**
    - Connect CH1 to CH3: ADVANCED_TIMER0_TIM_CH1 (input) ? ADVANCED_TIMER0_TIM_CH3 (PWM output)
    - Connect CH2 to CH4: ADVANCED_TIMER0_TIM_CH2 (input) ? ADVANCED_TIMER0_TIM_CH4 (PWM output)
    
    Channel Details:
    - ADVANCED_TIMER0_TIM_CH1: Input capture (TRC source, normal polarity, connects to CH3)
    - ADVANCED_TIMER0_TIM_CH2: Input capture (TRC source, inverted polarity, connects to CH4)
    - ADVANCED_TIMER0_TIM_CH3: PWM output (75% duty cycle, feeds CH1 via external wire)
    - ADVANCED_TIMER0_TIM_CH4: PWM output (75% duty cycle, feeds CH2 via external wire)

    Test Procedure:
    1. **Hardware Connection**: Wire CH1 to CH3 and CH2 to CH4 externally
    2. Configure timer with PWM generation on channels 3 and 4
    3. Set up input capture on channels 1 and 2 with interrupt enable
    4. Enable capture interrupts and overflow detection
    5. PWM signals from CH3/CH4 loop back to CH1/CH2, triggering capture interrupts
    6. Monitor interrupt handler progression through all 4 status flags
    7. Verify test completion when all capture and overflow events occur

    Expected Results:
    - CC1 interrupt triggers first capture event (cnt_buf = 1)
    - CC1 overflow interrupt triggers (cnt_buf2 = 1)
    - CC2 interrupt triggers first capture event (cnt_buf1 = 1)
    - CC2 overflow interrupt triggers (cnt_buf3 = 1)
    - Test passes when all 4 conditions are met
    - Successful interrupt handling and overflow detection

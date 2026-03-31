/*!
    \file  README.TXT
    \brief Advanced Timer Output Clear Enable (OC1CE) Test

    \version 2025-12-03, V2.0.0
*/

Function Description:
This example demonstrates how to configure ADVANCED_TIMER3 with Output Clear Enable (OC1CE) functionality.
The test validates the capability to clear OC1REF signal on an external event (ETRF), which is only applicable in output compare and PWM modes.

OC1CE Feature Overview:
The Output Clear Enable (OC1CE) feature allows immediate clearing of the OC1REF output signal when an external trigger event (ETRF) occurs.
This provides a mechanism for external event-triggered signal clearing without software intervention.

Hardware Configuration:
- Timer Mode: Edge-aligned PWM generation
- Period: 100 counts
- Clock Division: CKD1 (no division)
- Counter Direction: Up-counting
- Auto-reload preload: Disabled (immediate update)

PWM Output Configuration:
- Channel 1: PWM mode 1, 40% duty cycle (CCR1=40, ARR=100)
- Channel 2: PWM mode 1, 80% duty cycle (CCR2=80, ARR=100)
- Output Polarity: Normal polarity (CCxP=0, OCxREF active high)
- Complementary Outputs: Disabled for all channels
- Preload Register: Disabled (immediate update)

Output Clear Enable Feature (OC1CE):
- OC1CE Bit: Output Clear Enable for Channel 1
- Clear Source: ETRF signal (filtered external trigger)
- Clear Effect: OC1REF is forced to low when ETRF is high
- Clear Duration: Until the next update event (UEV) occurs
- Applicable Modes: Only output compare and PWM modes, not forced mode

External Trigger Configuration (ETR):
- External Trigger Prescaler: Off (ETPS=00)
- External Clock Mode 2: Disabled (ECE=0)
- External Trigger Polarity: Configurable (ETP bit)
- External Trigger Filter: Configurable (ETF bit)

IO Connection Information:
- ADVANCED_TIMER3_TIM_ETR: External trigger input (ETR signal source)
- ADVANCED_TIMER3_TIM_CH1: PWM output (affected by OC1CE clear function)
- ADVANCED_TIMER3_TIM_CH2: PWM output (normal PWM operation)

Test Procedure:
1. Configure timer with PWM outputs on channels 1 and 2
2. Enable OC1CE feature to allow external clearing of OC1REF
3. Configure ETR input as the clear trigger source
4. Start PWM generation (40% and 80% duty cycles)
5. Apply ETR high signal at appropriate timing
6. Observe Channel 1 output immediately cleared to inactive state
7. Verify Channel 2 continues normal PWM operation

Expected Results:
- Initial state: Channel 1 outputs 40% duty cycle PWM, Channel 2 outputs 80% duty cycle PWM
- When ETR is high: Channel 1 output is immediately cleared to inactive state (low level)
- Clear duration: Until the next update event occurs
- Channel 2: Unaffected, continues normal PWM operation
- OC1CE feature: Effectively implements response clearing to external events

Key Technical Points:
1. OC1CE functionality is enabled only on Channel 1
2. ETRF signal serves as the clear trigger source
3. Clear operation persists until the next update event
4. Other channels continue normal operation unaffected

Running Instructions:
1. Compile and download the program
2. Connect external trigger source to ETR input pin
3. Connect oscilloscope to monitor Channel 1 and Channel 2 outputs
4. Apply ETR high signal to observe the clear function
5. Verify Channel 1 output is cleared while Channel 2 continues normal PWM

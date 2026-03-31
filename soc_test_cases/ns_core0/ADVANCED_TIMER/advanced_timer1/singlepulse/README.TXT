/*!
    \file  README.TXT
    \brief Advanced Timer Single Pulse Mode Test

    \version 2025-12-03, V2.0.0
*/

Function Description:
This example demonstrates how to configure ADVANCED_TIMER0 in single pulse mode.
The timer generates a single output pulse triggered by an external signal on CH1 input, providing precise one-shot timing control.

Timer Configuration:
- Timer Mode: Single pulse mode (OPM enabled)
- Period: 1500 counts
- Prescaler: 7 (8-1) for hardware, 0 for simulation
- Clock Division: CKD1 (no division)
- Counter Direction: Up-counting
- Auto-reload Preload: Disabled (immediate update)
- Primary Output: Enabled

Single Pulse Mode Configuration:
- Single Pulse Mode: Enabled (OPM = 1)
- Trigger Source: TI1FP1 (filtered CH1 input)
- Trigger Edge: Falling edge active (CC1P enabled)
- Slave Mode: Trigger mode (SMS = 110)
- Trigger Action: Starts counter, generates one pulse, then stops automatically
- Pulse Generation: One complete timer cycle per trigger event

Output Pulse Generation:
- Output Channel: Channel 2 configured for PWM mode 2
- Output Mode: PWM mode 2 (output active when counter > CCR)

Input Capture Configuration:
- Input Channel: Channel 1 configured as input capture
- Capture Source: CC1S_OUTPUT (for trigger input)
- Edge Detection: Falling edge active (CC1P enabled)
- Input Filter: No filtering (direct edge detection)
- Prescaler: No prescaling
- Trigger Signal: External falling edge on CH1 input

IO Connection Information:
- ADVANCED_TIMER0_TIM_CH1: Trigger input (falling edge triggers pulse generation)
- ADVANCED_TIMER0_TIM_CH2: Single pulse output

Test Procedure:
1. Configure timer with prescaler (7 for hardware, 0 for simulation)
2. Set period to 1500 counts and enable single pulse mode (OPM)
3. Configure Channel 2 in PWM mode 2 with compare value = 500 counts
4. Configure Channel 1 as trigger input with falling edge detection
5. Set slave mode to trigger mode (SMS = 110)
6. Select TI1FP1 as trigger source
7. Enable timer and wait for trigger signal
8. Apply falling edge trigger signal to CH1 input
9. Observe single pulse on CH2 output (active for 1000 counts)
10. Verify timer stops automatically after pulse generation (at 1500 counts)

Expected results:
-Channel 1 will enable CH1 after capturing the pulse.
-After counting CRR1, CH2 generates a rising edge pulse.
-After counting the ARR number, the CH2 level is pulled down

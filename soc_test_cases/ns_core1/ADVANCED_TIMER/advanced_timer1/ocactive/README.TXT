/*!
    \file  README.TXT
    \brief Advanced Timer OC Active Mode Test

    \version 2025-12-03, V2.0.0
*/

Function Description:
This example demonstrates how to configure ADVANCED_TIMER1 in Output Compare Active mode.
The test validates the timer's ability to generate three pulse signals with different delays using output compare active mode.

OC Active Mode Overview:
OC Active mode (output compare active mode) is one of the output comparison modes. When the timer counter value matches the comparison register value,
the output pin is set to high level (active state). This mode is used to generate precise timing pulses.

Hardware Configuration:
- Prescaler: 799 (800-1) for hardware, 0 for simulation
- Auto-reload Value: 65535 (maximum period)
- Timer Mode: Edge-aligned mode
- Clock Division: CKD1 (no division)
- Counter Direction: Up-counting

Signal Configuration:
By configuring three different compare values, three pulse signals with different delays are generated:
- ADVANCED_TIMER1_CH1: Compare value = 4000 counts
- ADVANCED_TIMER1_CH2: Compare value = 8000 counts
- ADVANCED_TIMER1_CH3: Compare value = 12000 counts

Output Compare Configuration:
- Channel 1: OC Active mode (OCxM = SET_ACTIVE)
- Channel 2: OC Active mode (OCxM = SET_ACTIVE)
- Channel 3: OC Active mode (OCxM = SET_ACTIVE)
- Output Polarity: Normal polarity (CCxP=0, OCxREF high level active)
- Preload Register: Disabled (immediate update)

IO Connection Information:
- ADVANCED_TIMER1_TIM_CH1: Output compare channel 1
- ADVANCED_TIMER1_TIM_CH2: Output compare channel 2
- ADVANCED_TIMER1_TIM_CH3: Output compare channel 3
- Primary Output: Enabled for all channels

Test Procedure:
1. Configure timer with prescaler (799 for hardware, 0 for simulation)
2. Set all three channels to OC Active mode
3. Set different compare values for each channel (4000, 8000, 12000)
4. Start timer operation, counter begins counting from 0
5. Observe high-level pulse generated when counter matches each compare value
6. Verify pulse timing corresponds to configured compare values

Expected Results:
- Channel 1: High-level pulse generated when counter reaches 4000 counts
- Channel 2: High-level pulse generated when counter reaches 8000 counts
- Channel 3: High-level pulse generated when counter reaches 12000 counts
- Each channel remains high after compare match until next timer cycle
- Pulse timing accuracy corresponds to the configured compare value
- All three channels generate pulses at their respective count values

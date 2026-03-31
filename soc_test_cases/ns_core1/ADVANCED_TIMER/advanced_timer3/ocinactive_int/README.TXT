/*!
    \file  README.TXT
    \brief Advanced Timer Output Compare Inactive Interrupt Test

    \version 2025-12-03, V2.0.0
*/

Function Description:
This example demonstrates how to configure ADVANCED_TIMER3 in Output Compare Inactive mode with interrupt generation.
The test validates the timer's ability to generate interrupts when the counter reaches compare values in inactive output mode.

Timer Configuration:
- Timer Mode: Edge-aligned timing mode
- Period: 65535 counts (maximum period)
- Prescaler: 799 (800-1) for hardware, 0 for simulation
- Clock Division: CKD1 (no division)
- Counter Direction: Up-counting
- Auto-reload Preload: Disabled

Output Compare Configuration:
- Channel 1: Output Compare Active mode, compare value = 4000 counts
- Channel 2: Output Compare Active mode, compare value = 8000 counts  
- Channel 3: Output Compare Active mode, compare value = 12000 counts
- Channel 4: Output Compare Active mode, compare value = 12000 counts
- Output Mode: SET_ACTIVE (output becomes active when counter matches compare value)
- Output State: Enabled for all channels
- Output Polarity: Normal polarity for CH1/CH2, inverted for CH3/CH4

Interrupt Generation:
- Interrupt Sources: Channel 1-4 compare match interrupts + Update interrupt
- Interrupt Handling: Each interrupt increments state variable
- Test Logic: Pass when all 5 interrupts (4 channels + update) are received

Interrupt Trigger Points:
- Channel 1 Interrupt: Triggers when counter reaches 4000 counts
- Channel 2 Interrupt: Triggers when counter reaches 8000 counts
- Channel 3 Interrupt: Triggers when counter reaches 12000 counts
- Channel 4 Interrupt: Triggers when counter reaches 12000 counts
- Update Interrupt: Triggers when counter overflows at 65535 counts

IO Connection Information:
- ADVANCED_TIMER3_TIM_CH1: Output compare channel 1
- ADVANCED_TIMER3_TIM_CH2: Output compare channel 2
- ADVANCED_TIMER3_TIM_CH3: Output compare channel 3
- ADVANCED_TIMER3_TIM_CH4: Output compare channel 4
- Primary Output: Enabled for all channels

Test Procedure:
1. Configure timer with prescaler (799 for hardware, 0 for simulation)
2. Set maximum period (65535 counts) for continuous operation
3. Configure all 4 channels in Output Compare Active mode
4. Set compare values: CH1=4000, CH2=8000, CH3=12000, CH4=12000
5. Enable compare match interrupts for all channels + update interrupt
6. Start timer and begin counting from 0
7. Monitor state variable increment as counter reaches each compare value
8. Verify test passes when state variable reaches 5 (all interrupts received)

Expected Results:
- Timer runs continuously, counting from 0 to 65535
- CH1 interrupt occurs when counter = 4000
- CH2 interrupt occurs when counter = 8000
- CH3 interrupt occurs when counter = 12000
- CH4 interrupt occurs when counter = 12000
- Update interrupt occurs when counter overflows at 65535
- State variable increments from 0 to 5 sequentially
- Test passes when state = 5 (all compare and update interrupts received)
- Output pins reflect compare match events

/*!
    \file  README.TXT
    \brief Advanced Timer Timebase Generation Test

    \version 2025-12-03, V2.0.0
*/

Function Description:
This example demonstrates how to configure ADVANCED_TIMER2 as a precise timebase generator.
The timer operates in timing mode with periodic interrupt generation for system timing and scheduling applications.

Timer Configuration:
- Timer Mode: Edge-aligned timing mode
- Period: 65535 counts (maximum period for continuous operation)
- Prescaler: 399 (400-1) for hardware, 0 for simulation
- Clock Division: CKD1 (no division)
- Counter Direction: Up-counting
- Auto-reload Preload: Disabled (immediate update)
- Primary Output: Enabled

Timebase Generation:
- Channel 1 Compare Value: 4000 counts
- Timer Operation: Continuous counting from 0 to 65535 repeatedly
- Interrupt Trigger: When counter reaches compare value (4000 counts)
- Timing Reference: Provides periodic interrupt at fixed count intervals

Output Compare Configuration:
- Output Channel: Channel 1 configured for output compare
- Output Mode: Frozen mode (OC1M_FROZEN) - maintains output level
- Output State: Enabled with normal polarity
- Compare Value: 4000 counts (200ms timing interval)
- Shadow Register: Disabled (immediate update of compare value)
- Idle State: Output low when timer is disabled
- Frozen Mode: Output remains unchanged between compare events

Interrupt Handling:
- Interrupt Source: Channel 1 compare match (CC1IE)
- Interrupt Priority: Level 1, non-vector mode
- Interrupt Action: Sets status flag to indicate timing event
- Test Logic: Pass when interrupt occurs at counter value 4000
- Interrupt Trigger Point: Counter = 4000 counts
- Interrupt Handler: Simple flag-based timing indication

IO Connection Information:
- ADVANCED_TIMER2_TIM_CH1: Output compare channel (timing reference)
- ADVANCED_TIMER2_TIM_CH2: Available output channel
- ADVANCED_TIMER2_TIM_CH3: Available output channel
- ADVANCED_TIMER2_TIM_CH4: Available output channel
- No external inputs required for basic timebase function

Test Procedure:
1. Configure timer with prescaler (399 for hardware, 0 for simulation)
2. Set maximum period (65535 counts) for continuous operation without overflow
3. Configure Channel 1 in frozen output compare mode with compare value = 4000
4. Enable Channel 1 compare interrupt
5. Start timer and begin continuous counting from 0
6. Monitor status flag in interrupt handler
7. Verify interrupt occurs when counter reaches 4000 counts
8. Confirm test passes when first timing interrupt is detected

Expected Results:
- Timer runs continuously, counting from 0 to 65535 repeatedly
- Channel 1 compare interrupt occurs when counter = 4000
- Interrupt handler correctly sets status flag (status = 1)
- Test passes when first timing interrupt is detected
- Stable timing intervals with minimal jitter
- No spurious interrupts or timing drift
- Continuous operation without overflow issues

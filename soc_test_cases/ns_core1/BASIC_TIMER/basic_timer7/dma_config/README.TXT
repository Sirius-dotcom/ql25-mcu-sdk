/*!
    \file  README.TXT
    \brief Basic Timer DMA Configuration Test

    \version 2024-12-02, V2.0.0
*/
    Function Description:
    This example demonstrates how to configure BASIC_TIMER7 with DMA functionality for automatic register updates.
    The timer uses DMA to dynamically update the ARR register with new period values.
    
    Timer Configuration:
    - Timer Mode: Basic timing mode
    - Period: 200 counts (initial)
    - DMA Enable: Update DMA request (UDE)
    - Interrupt Enable: Update interrupt (UIE)
    - Configuration Focus: DMA functionality

    DMA Configuration:
    - DMA Channel: BASIC_TIMER7_I_DMA_CH
    - Source Address: Memory buffer with period values {400, 800}
    - Destination Address: BASIC_TIMER7 ARR register (offset 0x2C)
    - Buffer Size: 8 bytes (2 x 32-bit values)
    - Transfer Width: 32-bit
    - Source Increment: Enabled
    - Destination Increment: Disabled
    - Transfer Mode: Continuous

    Test Logic:
    - Timer starts with initial period of 200 counts
    - First update triggers DMA transfer of 400 to ARR register
    - Second update triggers DMA transfer of 800 to ARR register
    - Pattern repeats for total of 4 update events
    - Interrupt handler counts events and disables timer
    - Test passes when 4 events are completed

    Test Procedure:
    1. Configure DMA channel for PA2M transfer
    2. Set up source buffer with {400, 800} period values
    3. Configure destination as BASIC_TIMER7 ARR register
    4. Initialize timer with DMA and interrupt enable
    5. Start timer operation
    6. Wait for 4 update events to complete
    7. Verify test completion state

    Expected Results:
    - Timer runs with dynamically changing periods
    - DMA successfully updates ARR register twice per cycle
    - Four update interrupts are generated
    - Timer stops after 4th update
    - Test passes with correct DMA and interrupt operation

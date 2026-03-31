/*!
    \file  readme.txt
    \brief description of the advanced timer DMA CCXDE configuration test

    \version 2025-12-03, V2.0.0
*/
    Function description:
    This demo tests the DMA CCXDE (Capture/Compare X DMA Enable) functionality for ADVANCED_TIMER0.
    It verifies that the CC1 DMA channel can correctly transfer data and ensure accuracy.
    
    Timer Configuration:
    - Timer period: 1000 counts
    - Repetition counter: 1
    - Initial CH1 pulse value: 200
    - Output mode: PWM mode 1
    - DMA request: CC1DE enabled (Compare Channel 1 DMA Enable)
    
    DMA Configuration:
    - DMA channel: ADVANCED_TIMER0_CC1_DMA_CH
    - Source: Memory buffer with PWM values [250, 500, 750]
    - Destination: ADVANCED_TIMER0_CH0CV register (compare capture register)
    - Transfer width: 32-bit
    - Data size: 4 bytes per transfer
    - Mode: Normal mode
    - Peripheral selection: UDMA_SEL_ADVANCED_TIMER0_CC1
    
    Test Logic:
    1. Initialize DMA to transfer PWM values from memory to timer CCR register
    2. Configure timer with initial pulse value and enable CC1 DMA requests
    3. Start timer and wait for DMA transfer completion
    4. Verify final CCR1 register value equals 250 (first buffer value)
    5. Test passes if value matches, fails otherwise
    
    Expected Result:
    Test passes when DMA successfully overwrites the initial CCR1 value (200) with 250.
    Test fails if DMA transfer doesn't complete or incorrect value is loaded.
    
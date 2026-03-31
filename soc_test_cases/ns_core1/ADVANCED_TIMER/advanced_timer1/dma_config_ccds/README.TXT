/*!
    \file  readme.txt
    \brief description of the advanced timer DMA CCDS configuration test

    \version 2025-12-03, V2.0.0
*/
    Function description:
    This demo tests the DMA CCDS (Capture/Compare DMA Selection) functionality for ADVANCED_TIMER1.
    It verifies that the CC1 DMA channel can correctly transfer data and ensure accuracy using CCDS trigger source.
    
    Timer Configuration:
    - Timer period: 1000 counts
    - Repetition counter: 1
    - Output mode: PWM mode 1
    - DMA request source: CCDS enabled (Capture/Compare DMA Selection)
    
    DMA Configuration:
    - DMA channel: ADVANCED_TIMER1_CC1_DMA_CH
    - Source: Memory buffer with PWM values [250, 500, 750]
    - Destination: ADVANCED_TIMER1_CH0CV register (compare capture register)
    - Transfer width: 32-bit
    - Data size: 4 bytes per transfer
    - Mode: Normal mode
    
    Test Logic:
    1. Initialize DMA to transfer PWM values from memory to timer CCR register
    2. Configure timer to generate DMA requests on compare events
    3. Enable CCDS to select appropriate DMA trigger source
    4. Start timer and wait for DMA transfer completion
    5. Verify final CCR1 register value equals 250 (first buffer value)
    
    Expected Result:
    Test passes when DMA successfully transfers the first value (250) to the CCR1 register.
    Test fails if DMA transfer doesn't complete or incorrect value is loaded.
    
/*!
    \file  readme.txt
    \brief description of the advanced timer DMA COM configuration test

    \version 2025-12-03, V2.0.0
*/
    Function description:
    This demo tests COM (Commutation) interrupt and DMA functionality for ADVANCED_TIMER1.
    It verifies both COMIF interrupt generation and COMDE DMA transfer capabilities.
    
    Timer Configuration:
    - Timer period: 1000 counts
    - Repetition counter: 1
    - Initial CH1 pulse value: 200
    - Output mode: PWM mode 1, then switches to PWM mode 2
    - Control shadow update: Enabled
    - COM interrupt and DMA: Both enabled
    
    DMA Configuration:
    - DMA channel: ADVANCED_TIMER1_COM_DMA_CH
    - Source: Memory buffer with PWM values [250, 500, 750]
    - Destination: ADVANCED_TIMER1_CH0CV register (compare capture register)
    - Transfer width: 32-bit
    - Data size: 4 bytes per transfer
    - Mode: Normal mode
    - Peripheral selection: UDMA_SEL_ADVANCED_TIMER1_COM
    
    Test Logic:
    1. Initialize DMA to transfer PWM values from memory to timer CCR register
    2. Configure timer with COM interrupt and DMA enabled
    3. Start timer and trigger COM event software generation
    4. Wait for both COM interrupt (state0) and DMA completion (state1)
    5. Verify final CCR1 register value equals 250 (first buffer value)
    6. Test passes only if both interrupt and DMA complete correctly

    Expected Result:
    Test passes when:
    - COM interrupt is triggered (state0 == 0)
    - DMA transfer completes (state1 == 0)
    - Final CCR1 value equals 250
    Test fails if any of these conditions are not met.

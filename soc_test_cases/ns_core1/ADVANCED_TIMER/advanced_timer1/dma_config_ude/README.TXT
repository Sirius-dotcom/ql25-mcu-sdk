/*!
    \file  readme.txt
    \brief description of the advanced timer DMA UDE configuration test

    \version 2025-12-03, V2.0.0
*/
    Function description:
    This demo tests the DMA UDE (Update DMA Enable) functionality for ADVANCED_TIMER1.
    It modifies CCR1-4 registers using UG (Update Generation) register bit in DMA burst mode.
    
    Timer Configuration:
    - Timer period: 1000 counts
    - Repetition counter: 1
    - DMA request: UDE enabled (Update DMA Enable)
    - DMA transfer config: Burst mode with 3 transfers
    - Update generation: Software trigger enabled
    
    DMA Configuration:
    - DMA channel: ADVANCED_TIMER1_UEV_DMA_CH
    - Source: Memory buffer with CCR values [250, 500, 750]
    - Destination: Timer CCR1, CCR2, CCR3 registers sequentially
    - Transfer width: 32-bit
    - Buffer size: 12 bytes (3 transfers * 4 bytes)
    - Mode: Normal mode
    - Peripheral selection: UDMA_SEL_ADVANCED_TIMER1_UEV
    - Destination increment: Disabled (fixed CCR register addresses)
    - Source increment: Enabled (sequential memory addresses)
    
    Test Logic:
    1. Initialize DMA to transfer CCR values from memory to timer registers
    2. Configure timer for update event DMA requests
    3. Trigger update generation to start DMA burst transfer
    4. Wait for DMA completion
    5. Verify CCR1=250, CCR2=500, CCR3=750
    6. Test passes if all values match expected
    
    IO Connection information:
    - ADVANCED_TIMER1_TIM_CH1: Output with updated CCR1 value (250)
    - ADVANCED_TIMER1_TIM_CH2: Output with updated CCR2 value (500)
    - ADVANCED_TIMER1_TIM_CH3: Output with updated CCR3 value (750)

    Expected Result:
    Test passes when CCR1=250, CCR2=500, and CCR3=750 after DMA transfer.
    Test fails if any register contains incorrect value.
    
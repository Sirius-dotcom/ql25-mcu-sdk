/*!
    \file  readme.txt
    \brief description of advanced timer external trigger DMA test

    \version 2025-12-03, V2.0.0
*/
    Function description:
    This demo shows how to configure ADVANCED_TIMER1 with external trigger control and DMA functionality.
    The timer operates as a slave device controlled by external trigger signals.
    
    Timer Configuration:
    - Timer period: 1000 counts
    - Slave mode: Trigger mode (SMS_TRIGGER)
    - Input trigger source: TI1FP1 (external trigger on CH1)
    - CH3 output: PWM mode 1 with 50% duty cycle (500/1000)
    - Trigger interrupt and DMA: Both enabled
    
    DMA Configuration:
    - DMA channel: ADVANCED_TIMER1_TRG_DMA_CH
    - Source: Memory buffer with values [250, 500, 750]
    - Destination: ADVANCED_TIMER1_CH0CV register
    - Transfer width: 32-bit
    - Buffer size: 4 bytes (single transfer)
    - Mode: Normal mode
    - Peripheral selection: UDMA_SEL_ADVANCED_TIMER1_TRG
    
    External Trigger Configuration:
    - Trigger input: CH1 pin configured as input
    - Trigger polarity: Rising edge
    - Filter: 2 cycles (FL1_N2)
    - Slave mode selection: Trigger mode
    
    Test Logic:
    1. Configure timer in slave mode with external trigger input
    2. Initialize DMA for trigger event transfers
    3. Enable trigger interrupt and DMA
    4. Apply external trigger signal to CH1 pin
    5. Wait for both trigger interrupt (state0) and DMA completion (state1)
    6. Verify CCR1 register value equals 250
    
    IO Connection information:
    - ADVANCED_TIMER1_TIM_CH1: External trigger input 
    - ADVANCED_TIMER1_TIM_CH3: PWM output (50% duty cycle)
    
    Expected Result:
    Test passes when:
    - Trigger interrupt is generated (state0 == 0)
    - DMA transfer completes (state1 == 0)
    - CCR1 value equals 250
    Test fails if any condition is not met.
    
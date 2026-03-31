/*!
    \file  README.TXT
    \brief description of the LGPIO software interrupt test
    \version 2021-3-18, V1.0.0
*/

Function Description:
    This test demonstrates LGPIO software-triggered interrupt functionality.
    The program configures GPIO0 for high-level interrupt,
    then uses software to trigger the interrupt without external signal.

LGPIO Configuration:
    - LGPIO1 GPIO0: Input mode with software interrupt
      * IOMUX configuration: LGPIO1_GPIO0_IOF_IVAL
      * Pad selection: LGPIO1_GPIO0_PAD_SEL
      * High-speed selection: LGPIO1_GPIO0_HS_SEL
      * Interrupt type: High-level trigger (LGPIO_IT_HIGH)
      * Interrupt source: LGPIO_PinSource0
      * Trigger method: Software (LGPIO_SetSoftIT)

Interrupt Configuration:
    - IRQ number: LGPIO1_IRQn
    - Interrupt mode: ECLIC_NON_VECTOR_INTERRUPT
    - Trigger type: ECLIC_LEVEL_TRIGGER
    - Priority level: 1
    - Sub-priority: 1
    - Handler function: LGPIO1_IRQHandler

Test Process:
    1. Enable LGPIO1 clock
    2. Perform LGPIO1 reset sequence (disable then enable)
    3. Configure IOMUX for GPIO0 as input
    4. Enable GPIO0 high-level interrupt
    5. Trigger software interrupt using LGPIO_SetSoftIT
    6. Print "config finish" message
    7. Enable global interrupts with __enable_irq()
    8. Register LGPIO1 interrupt handler with ECLIC
    9. Wait for interrupt to trigger (state remains 1)
    10. When software interrupt triggers, enter handler
    11. Clear interrupt flag for GPIO0 high-level
    12. Disable further high-level interrupts
    13. Set state = 0 to exit waiting loop
    14. Exit interrupt handler
    15. Main loop detects state = 0 and passes test

Expected Result:
    - Software interrupt triggers successfully
    - Interrupt handler executed: state changes to 0, test passes
    - No external signal required for interrupt
    - Interrupt flag cleared successfully
    - Validates software interrupt mechanism

IO Connection Information:
    No external IO connection required (software-triggered interrupt)

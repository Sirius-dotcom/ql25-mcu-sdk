/*!
    \file  README.TXT
    \brief description of the LGPIO interrupt test
    \version 2022-11-11, V2.1.1
*/

Function Description:
    This test demonstrates LGPIO high-level interrupt functionality.
    The program configures GPIO0 for high-level interrupt detection,
    and verifies interrupt triggering, handling, and clearing mechanisms.

LGPIO Configuration:
    - LGPIO3 GPIO0: Input mode with high-level interrupt
      * IOMUX configuration: LGPIO3_GPIO0_IOF_IVAL
      * Pad selection: LGPIO3_GPIO0_PAD_SEL
      * High-speed selection: LGPIO3_GPIO0_HS_SEL
      * Interrupt type: High-level trigger (LGPIO_IT_HIGH)
      * Interrupt source: LGPIO_PinSource0

Interrupt Configuration:
    - IRQ number: LGPIO3_IRQn
    - Interrupt mode: ECLIC_NON_VECTOR_INTERRUPT
    - Trigger type: ECLIC_LEVEL_TRIGGER
    - Priority level: 1
    - Sub-priority: 1
    - Handler function: LGPIO3_IRQHandler

Test Process:
    1. Enable LGPIO3 clock
    2. Perform LGPIO3 reset sequence (disable then enable)
    3. Enable global interrupts with __enable_irq()
    4. Register LGPIO3 interrupt handler with ECLIC
    5. Configure IOMUX for GPIO0 as input
    6. Enable GPIO0 high-level interrupt
    7. Print "config finish" message
    8. Delay 1ms for system stabilization
    9. Wait for interrupt to trigger (state remains 1)
    10. When high level detected, enter interrupt handler
    11. Clear interrupt flag for GPIO0 high-level
    12. Disable further high-level interrupts
    13. Set state = 0 to exit waiting loop
    14. Exit interrupt handler
    15. Main loop detects state = 0 and passes test

Expected Result:
    - Interrupt handler executed: state changes to 0, test passes
    - Interrupt triggered correctly by high-level input
    - Interrupt flag cleared successfully
    - Further interrupts disabled after first trigger
    - Validates interrupt configuration and handling

IO Connection Information:
    Apply high-level signal to LGPIO3_GPIO0 to trigger interrupt

/*!
    \file  README.TXT
    \brief description of the LGPIO evasion mode test
    \version 2022-11-11, V2.1.1
*/

Function Description:
    This test demonstrates LGPIO interrupt evasion mode functionality.
    The program configures GPIO0 with rising edge interrupt,
    and GPIO1 generates transitions to test if duplicate interrupts are avoided.

LGPIO Configuration:
    - LGPIO3 GPIO0: Input mode with rising edge interrupt
      * IOMUX configuration: LGPIO3_GPIO0_IOF_IVAL
      * Pad selection: LGPIO3_GPIO0_PAD_SEL
      * High-speed selection: LGPIO3_GPIO0_HS_SEL
      * Interrupt type: Rising edge trigger (LGPIO_IT_RISE)
      * Interrupt source: LGPIO_PinSource0
    - LGPIO3 GPIO1: Push-pull output mode
      * IOMUX configuration: LGPIO3_GPIO1_IOF_OVAL
      * Pad selection: LGPIO3_GPIO1_PAD_SEL
      * High-speed selection: LGPIO3_GPIO1_HS_SEL
      * Output mode: PP (Push-Pull)
      * Pull mode: Pull-down enabled (PDE)

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
    3. Configure IOMUX for GPIO0 and GPIO1
    4. Clear GPIO0 rising edge interrupt flag
    5. Set GPIO1 to low level (0)
    6. Configure GPIO1 as push-pull output with pull-down
    7. Delay 1ms for stabilization
    8. Enable global interrupts
    9. Register LGPIO3 interrupt handler with ECLIC
    10. Enable GPIO0 rising edge interrupt
    11. Delay 1ms
    12. Set GPIO1 to high level (1) - first rising edge
    13. Reconfigure GPIO1 as push-pull with pull-down
    14. Delay 10ms to allow interrupt processing
    15. Check IRQ_status counter value
    16. If IRQ_status == 2: test fails (duplicate interrupt)
    17. If IRQ_status != 2: test passes (evasion mode working)

Expected Result:
    - IRQ_status != 2: Test passes (evasion mode prevents duplicate)
    - IRQ_status == 2: Test fails (evasion mode not working)
    - Only one interrupt expected despite reconfiguration
    - Validates interrupt evasion mechanism
    - Prevents redundant interrupt triggers

IO Connection Information:
    Connect LGPIO3_GPIO0 to LGPIO3_GPIO1

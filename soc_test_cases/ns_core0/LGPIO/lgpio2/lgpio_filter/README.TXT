/*!
    \file  README.TXT
    \brief description of the LGPIO digital noise filter test
    \version 2022-11-11, V2.1.1
*/

Function Description:
    This test demonstrates LGPIO digital noise filter functionality.
    The program configures GPIO0 with rising edge interrupt and digital noise filter,
    requiring consecutive samples to confirm valid signal transitions.

LGPIO Configuration:
    - LGPIO2 GPIO0: Input mode with rising edge interrupt and filter
      * IOMUX configuration: LGPIO2_GPIO0_IOF_IVAL
      * Pad selection: LGPIO2_GPIO0_PAD_SEL
      * High-speed selection: LGPIO2_GPIO0_HS_SEL
      * Interrupt type: Rising edge trigger (LGPIO_IT_RISE)
      * Interrupt source: LGPIO_PinSource0
      * Digital noise filter: Enabled
      * Filter sample count: 3

Interrupt Configuration:
    - IRQ number: LGPIO2_IRQn
    - Interrupt mode: ECLIC_NON_VECTOR_INTERRUPT
    - Trigger type: ECLIC_LEVEL_TRIGGER
    - Priority level: 1
    - Sub-priority: 1
    - Handler function: LGPIO2_IRQHandler

Digital Noise Filter Configuration:
    - Filter number: LGPIO_DIGITAL_NOISE_FILTER_NUM(3)
    - Pin: LGPIO_PinSource0
    - Filter state: Enabled
    - Function: Requires 3 consecutive identical samples to validate signal

Test Process:
    1. Enable LGPIO2 clock
    2. Perform LGPIO2 reset sequence (disable then enable)
    3. Check CPU hart ID (multicore handling)
    4. If not core 0, pass test and exit
    5. Enable global interrupts
    6. Register LGPIO2 interrupt handler with ECLIC
    7. Configure IOMUX for GPIO0 as input
    8. Set digital noise filter count to 3
    9. Enable digital noise filter for GPIO0
    10. Clear GPIO0 rising edge interrupt flag
    11. Enable GPIO0 rising edge interrupt
    12. Wait for state counter to reach 2
    13. If state == 2: test passes (2 valid rising edges detected)
    14. If state != 2: test fails (filter not working correctly)

Expected Result:
    - state == 2: Test passes (filter validates 2 rising edges)
    - state != 2: Test fails (filter malfunction)
    - Filter requires 3 consecutive samples for validation
    - Prevents false triggers from noise or glitches
    - Validates digital filtering mechanism

IO Connection Information:
    Apply rising edge signals to LGPIO2_GPIO0 (must be stable for filter)

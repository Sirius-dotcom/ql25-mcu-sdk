/*!
    \file  README.TXT
    \brief description of the LGPIO input level test
    \version 2022-11-11, V2.1.1
*/

Function Description:
    This test demonstrates LGPIO input level detection functionality.
    The program monitors GPIO0 and GPIO1 input levels simultaneously,
    and passes when both pins detect high level signals.

LGPIO Configuration:
    - LGPIO2 GPIO0: Input mode
      * IOMUX configuration: LGPIO2_GPIO0_IOF_IVAL
      * Pad selection: LGPIO2_GPIO0_PAD_SEL
      * High-speed selection: LGPIO2_GPIO0_HS_SEL
    - LGPIO2 GPIO1: Input mode
      * IOMUX configuration: LGPIO2_GPIO1_IOF_IVAL
      * Pad selection: LGPIO2_GPIO1_PAD_SEL
      * High-speed selection: LGPIO2_GPIO1_HS_SEL

Test Process:
    1. Enable LGPIO2 clock
    2. Perform LGPIO2 reset sequence (disable then enable)
    3. Configure IOMUX for both GPIO0 and GPIO1 as inputs
    4. Print "test start" message
    5. Loop 100 times to poll input levels
    6. Read GPIO0 input level
    7. If GPIO0 is high, set status0 = 1, else status0 = 0
    8. Read GPIO1 input level
    9. If GPIO1 is high, set status1 = 1, else status1 = 0
    10. Check if both status0 and status1 are 1
    11. If both high, test passes and exits loop
    12. If not both high, continue next iteration
    13. Report test result after condition met or timeout

Expected Result:
    - Both GPIO0 and GPIO1 detect high level: Test passes
    - Either or both detect low level: Continue polling
    - Loop limit: 100 iterations maximum
    - Verifies simultaneous multi-pin input detection
    - Validates IOMUX input configuration

IO Connection Information:
    Apply high-level signals to both LGPIO2_GPIO0 and LGPIO2_GPIO1

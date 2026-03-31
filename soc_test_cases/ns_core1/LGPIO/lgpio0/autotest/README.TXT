/*!
    \file  README.TXT
    \brief description of the LGPIO auto test
    \version 2022-11-11, V2.1.1
*/

Function Description:
    This test demonstrates LGPIO automatic input/output loopback functionality.
    The program configures GPIO1 as push-pull output and GPIO0 as input,
    then verifies signal propagation by writing to GPIO1 and reading from GPIO0.

LGPIO Configuration:
    - LGPIO0 GPIO0: Input mode
      * IOMUX configuration: LGPIO0_GPIO0_IOF_IVAL
      * Pad selection: LGPIO0_GPIO0_PAD_SEL
      * High-speed selection: LGPIO0_GPIO0_HS_SEL
    - LGPIO0 GPIO1: Push-pull output mode
      * IOMUX configuration: LGPIO0_GPIO1_IOF_OVAL
      * Pad selection: LGPIO0_GPIO1_PAD_SEL
      * High-speed selection: LGPIO0_GPIO1_HS_SEL
      * Output mode: PP (Push-Pull)

Test Process:
    1. Enable LGPIO0 clock
    2. Perform LGPIO0 reset sequence (disable then enable)
    3. Configure IOMUX for GPIO0 as input and GPIO1 as output
    4. Set GPIO1 to push-pull output mode
    5. Write high level (1) to GPIO1
    6. Read input level from GPIO0
    7. If GPIO0 reads high, increment status_inout by 1
    8. If GPIO0 reads low, increment status_inout by 2
    9. Write low level (0) to GPIO1
    10. Read input level from GPIO0
    11. If GPIO0 reads low, increment status_inout by 3
    12. If GPIO0 reads high, increment status_inout by 4
    13. Check if status_inout equals 4 (1+3)
    14. Report test result based on status value

Expected Result:
    - status_inout = 4: Test passes
    - First operation adds 1 (GPIO0 high when GPIO1 high)
    - Second operation adds 3 (GPIO0 low when GPIO1 low)
    - Other values indicate loopback failure
    - Verifies basic GPIO write/read and signal propagation

IO Connection Information:
    Connect LGPIO0_GPIO0 to LGPIO0_GPIO1

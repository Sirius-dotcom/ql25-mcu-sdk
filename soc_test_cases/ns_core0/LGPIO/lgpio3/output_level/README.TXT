/*!
    \file  README.TXT
    \brief description of the LGPIO output level test
    \version 2021-5-26, V1.0.1
*/

Function Description:
    This test demonstrates LGPIO output level toggle functionality.
    The program configures GPIO0 and GPIO1 as push-pull outputs,
    and continuously toggles both pins to verify output operation.

LGPIO Configuration:
    - LGPIO3 GPIO0: Push-pull output mode
      * IOMUX configuration: LGPIO3_GPIO0_IOF_IVAL
      * Pad selection: LGPIO3_GPIO0_PAD_SEL
      * High-speed selection: LGPIO3_GPIO0_HS_SEL
      * Output mode: PP (Push-Pull)
      * Initial state: High (1)
    - LGPIO3 GPIO1: Push-pull output mode
      * IOMUX configuration: LGPIO3_GPIO1_IOF_OVAL
      * Pad selection: LGPIO3_GPIO1_PAD_SEL
      * High-speed selection: LGPIO3_GPIO1_HS_SEL
      * Output mode: PP (Push-Pull)
      * Initial state: High (1)

Test Process:
    1. Enable LGPIO3 clock
    2. Perform LGPIO3 reset sequence (disable then enable)
    3. Configure IOMUX for GPIO0 and GPIO1
    4. Set both GPIO0 and GPIO1 to high level
    5. Configure both pins as push-pull output mode
    6. Enter toggle loop (10 iterations in simulation mode)
    7. Toggle GPIO0 level (high to low or low to high)
    8. Toggle GPIO1 level (high to low or low to high)
    9. In simulation mode: no delay between toggles
    10. In normal mode: 1000ms delay between toggles
    11. Repeat steps 7-10 for all iterations
    12. Call simulation_check() to verify results

Expected Result:
    - Both GPIO0 and GPIO1 toggle successfully
    - In simulation: 10 toggles with no delay
    - In normal mode: continuous toggle with 1s interval
    - Verifies push-pull output functionality
    - Validates level control and toggle operations

IO Connection Information:
    Monitor LGPIO3_GPIO0 and LGPIO3_GPIO1 output levels

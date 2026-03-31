/*!
    \file  README.TXT
    \brief description of the RTC divider get test
    \version 2022-11-11, V2.1.1
*/

Function Description:
    This test verifies RTC divider reading functionality.

RTC Configuration:
    - Prescaler: 0xFF
    - Enabled interrupt: Second (SECIE)

Test Process:
    1. Enable RTC clock and reset
    2. Set prescaler to 0xFF
    3. Enable second interrupt
    4. Read divider value continuously
    5. Print divider value
    6. Test passes when second interrupt occurs

Expected Result:
    Divider value is read successfully and second interrupt triggers.

IO Connection:
    No external connection required

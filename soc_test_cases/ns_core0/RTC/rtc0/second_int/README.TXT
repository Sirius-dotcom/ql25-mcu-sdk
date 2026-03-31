/*!
    \file  README.TXT
    \brief description of the RTC second interrupt test
    \version 2022-11-11, V2.1.1
*/

Function Description:
    This test verifies RTC second interrupt functionality.

RTC Configuration:
    - Prescaler: 0xF
    - Enabled interrupt: Second (SECIE)

Test Process:
    1. Enable RTC clock and reset
    2. Set prescaler to 0xF
    3. Enable second interrupt
    4. Wait for second interrupt
    5. Test passes when interrupt occurs

Expected Result:
    Test passes when second interrupt triggers.

IO Connection:
    No external connection required

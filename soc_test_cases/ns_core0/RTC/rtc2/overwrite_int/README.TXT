/*!
    \file  README.TXT
    \brief description of the RTC overflow interrupt test
    \version 2022-11-11, V2.1.1
*/

Function Description:
    This test verifies RTC overflow interrupt functionality.

RTC Configuration:
    - Counter initial value: 0xFFFFFFFFFFFFFFF0
    - Prescaler: 0xF
    - Enabled interrupt: Overflow (OWIE)

Test Process:
    1. Enable RTC clock and reset
    2. Set prescaler to 0xF
    3. Set counter to 0xFFFFFFFFFFFFFFF0
    4. Enable overflow interrupt
    5. Wait for overflow interrupt
    6. Test passes when interrupt occurs

Expected Result:
    Test passes when overflow interrupt triggers.

IO Connection:
    No external connection required

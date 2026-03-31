/*!
    \file  README.TXT
    \brief description of the RTC alarm interrupt test
    \version 2022-11-11, V2.1.1
*/

Function Description:
    This test verifies RTC alarm interrupt functionality.

RTC Configuration:
    - Counter initial value: 0
    - Prescaler: 0xF
    - Alarm value: 0x3
    - Enabled interrupt: Alarm (ALRIE)

Test Process:
    1. Enable RTC clock and reset
    2. Set counter to 0
    3. Set prescaler to 0xF
    4. Set alarm value to 0x3
    5. Enable alarm interrupt
    6. Wait for alarm interrupt
    7. Test passes when alarm triggers

Expected Result:
    Test passes when alarm interrupt occurs at counter value 0x3.

IO Connection:
    No external connection required

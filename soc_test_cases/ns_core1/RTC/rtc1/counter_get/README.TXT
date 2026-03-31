/*!
    \file  README.TXT
    \brief description of the RTC counter get test
    \version 2022-11-11, V2.1.1
*/

Function Description:
    This test verifies RTC counter reading with overflow and second interrupts.

RTC Configuration:
    - Counter initial value: 0xFFFFFFFFFFFFFFF0
    - Prescaler: 0xF
    - Enabled interrupts: Overflow (OWIE), Second (SECIE)

Test Process:
    1. Enable RTC clock and reset
    2. Set counter to 0xFFFFFFFFFFFFFFF0
    3. Set prescaler to 0xF
    4. Enable overflow and second interrupts
    5. Wait for both interrupts
    6. Read counter value in interrupt handler
    7. Test passes when both interrupts triggered

Expected Result:
    Test passes when overflow and second interrupts both occur.

IO Connection:
    No external connection required

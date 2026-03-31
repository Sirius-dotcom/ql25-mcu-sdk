/*!
    \file  README.TXT
    \brief USART Start Bit Mode Auto Baud Rate Detection Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies the auto baud rate detection function of USART in start bit mode. In this mode, the receiver automatically calculates the baud rate by precisely measuring the width of the start bit, which is the most accurate auto baud rate detection method.

Test Procedure:
1. Configure USART1 to enable auto baud rate detection, mode set to start bit detection
2. Configure USART2 to fixed baud rate 115200 bps
3. USART2 sends data frame starting with 0x55 (convenient for start bit measurement)
4. USART1 precisely measures start bit width and automatically calculates baud rate
5. Wait for auto baud rate detection completion (ABRF flag)
6. Check for detection errors (ABRE flag)
7. Read the calculated baud rate value
8. Use auto-detected baud rate for normal communication test

Test Data:
USART2 sends data:
- Sync character: 0x55 (binary 01010101, convenient for start bit measurement)
- Data content: 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A
- Data length: 10 bytes

Configuration Parameters:
- Auto baud rate mode: Start bit detection mode
- Initial baud rate: 200000 bps (USART1)
- Target baud rate: 115200 bps (USART2)
- Data bits: 8 bits
- Stop bits: 1 bit
- Parity: None
- Detection method: Precise start bit width measurement

Start Bit Mode Features:
- Detection principle: Precisely measure the duration of start bit
- Highest accuracy: Most accurate among all auto baud rate methods
- Strong universality: Suitable for all standard asynchronous communications
- Fast response: Can complete with one start bit measurement
- Standard compatibility: Fully compliant with UART communication standard

Baud Rate Calculation:
``
Actual baud rate = Clock frequency / (DIV + 1)
``
Where DIV is the division value calculated by auto baud rate detection

Expected Results:
If USART1 can successfully detect USART2's baud rate (115200 bps), with error within allowable range (usually 5%),
and subsequent data transmission is correct, the test passes, output "pass";
otherwise the test fails, output "fail".

IO Connection Description:
- USART2 TX connects to USART1 RX
- USART2 acts as data sender, USART1 acts as baud rate detector

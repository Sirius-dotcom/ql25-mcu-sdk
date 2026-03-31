/*!
    \file  README.TXT
    \brief USART 0x55 Mode Auto Baud Rate Detection Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies the auto baud rate detection function of USART in 0x55 mode. In this mode, the receiver automatically calculates the baud rate by detecting the specific pattern of the 0x55 character (binary 01010101), which is the most commonly used auto baud rate detection method.

Test Procedure:
1. Configure USART3 to enable auto baud rate detection, mode set to 0x55
2. Configure USART4 to fixed baud rate 115200 bps
3. USART4 sends data frame starting with 0x55
4. USART3 detects 0x55 pattern and automatically calculates baud rate
5. Wait for auto baud rate detection completion (ABRF flag)
6. Check for detection errors (ABRE flag)
7. Read the calculated baud rate value
8. Use auto-detected baud rate for normal communication test

Test Data:
USART4 sends data:
- Sync character: 0x55 (binary 01010101, auto baud rate detection standard character)
- Data content: 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A
- Data length: 10 bytes

Configuration Parameters:
- Auto baud rate mode: 0x55 mode
- Initial baud rate: 200000 bps (USART3)
- Target baud rate: 115200 bps (USART4)
- Data bits: 8 bits
- Stop bits: 1 bit
- Parity: None
- Detection method: 0x55 character pattern recognition

0x55 Mode Features:
- Sync character: 0x55 (binary 01010101)
- Detection principle: Recognize alternating 0 and 1 pattern
- Application scenarios: General auto baud rate detection
- Detection accuracy: High accuracy, low false detection rate
- Sync requirement: Must start with 0x55 character
- Universality: Widely used in various serial communications

Baud Rate Calculation:
``
Actual baud rate = Clock frequency / (DIV + 1)
``
Where DIV is the division value calculated by auto baud rate detection

Expected Results:
If USART3 can successfully detect USART4's baud rate (115200 bps), with error within allowable range (usually 5%),
and subsequent data transmission is correct, the test passes, output "pass"; otherwise the test fails, output "fail".

IO Connection Description:
- USART4 TX connects to USART3 RX
- USART4 acts as data sender, USART3 acts as baud rate detector

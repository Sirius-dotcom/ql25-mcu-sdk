/*!
    \file  README.TXT
    \brief USART Multiprocessor Idle Wakeup Mode Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies the functionality of USART in multiprocessor communication systems using idle line wakeup mode. In this mode, the receiver automatically awakens when it detects that the bus has been idle for a period of time, suitable for multi-point networks that need to detect communication start.

Test Procedure:
1. Configure USART1 as transmit mode, USART2 as receive mode
2. Set USART2 to idle line wakeup mode
3. Enable USART2 receiver wakeup function
4. USART1 continuously sends multi-byte data
5. Insert idle time during transmission process
6. USART2 is awakened after detecting idle condition
7. Verify whether idle wakeup function works normally

Test Data:
USART1 Send Data (9-bit mode):
- Data Frames: 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
- Data Length: 9 bytes
- Send Order: Send 5 bytes first, insert idle time, then send 4 bytes

Configuration Parameters:
- Data Bit Length: 9 bits
- Wakeup Mode: Idle line mode
- Receive Address: 0x02 (backup)
- Baud Rate: 115200 bps
- Stop Bits: 1 bit
- Parity: None
- FIFO Timeout: 0xFF
- Idle Detection Time: Configurable

Expected Results:
If USART2 can correctly detect idle condition and be awakened, successfully receiving all data frames,
then the test passes, output "pass"; otherwise the test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX
- USART1 RX connects to USART2 TX

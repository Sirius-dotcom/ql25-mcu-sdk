/*!
    \file  README.TXT
    \brief USART Multiprocessor Address Wakeup Mode Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies the functionality of USART in multiprocessor communication systems using address matching wakeup mode. In this mode, the receiver is only awakened when it receives a matching address byte, entering normal reception mode, thereby reducing unnecessary power consumption and processing overhead.

Test Procedure:
1. Configure USART3 as transmit mode, USART4 as receive mode
2. Set USART4 to address matching wakeup mode
3. Configure USART4 receive address as 0x02
4. Enable USART4 receiver wakeup function
5. USART3 sends 9-bit data frames containing address information
6. USART4 is awakened after detecting matching address
7. Verify whether address matching wakeup function works normally

Test Data:
USART3 Send Data (9-bit mode):
- Address Frame: 0x102 (9th bit is 1 indicates address frame, address is 0x02)
- Data Frames: 0x11, 0x22, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
- Data Length: 9 frames (1 address frame + 8 data frames)

Configuration Parameters:
- Data Bit Length: 9 bits (9th bit used for address/data identification)
- Wakeup Mode: Address matching mode
- Receive Address: 0x02
- Baud Rate: 115200 bps
- Stop Bits: 1 bit
- Parity: None
- FIFO Timeout: 0xFF

Expected Results:
If USART4 can correctly identify address 0x02 and be awakened, successfully receiving subsequent data frames,
then the test passes, output "pass"; otherwise the test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 RX
- USART3 RX connects to USART4 TX

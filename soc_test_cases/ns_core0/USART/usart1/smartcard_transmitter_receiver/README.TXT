/*!
    \file  README.TXT
    \brief USART Smart Card Mode Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies communication functionality of USART1 and USART2 in smart card (ISO 7816) mode. Smart card mode is an asynchronous communication protocol specifically designed for smart card communication, supporting special features such as parity check, guard time, etc.

Test Procedure:
1. Configure smart card mode of USART1 and USART2
2. Set smart card related parameters: prescaler, guard time, pull-up enable
3. Configure communication parameters: baud rate, data bits, stop bits, parity check
4. USART1 sends 10 bytes data to USART2
5. USART2 receives and stores data
6. Switch direction: USART2 sends data, USART1 receives
7. Verify integrity of bidirectional data transfer

Test Data:
USART1 Send Data:
- Data content: 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A
- Data length: 10 bytes

USART2 Send Data:
- Data content: 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A
- Data length: 10 bytes

Configuration Parameters:
- Smart Card Mode: Enabled
- Prescaler: 10
- Guard Time: 16-bit clock
- Transmit Pull-up Enable: Enabled
- Baud Rate: 115200 during simulation, actual 9600 bps
- Data Bits: 8 bits
- Stop Bits: 1.5 bits (smart card standard)
- Parity Check: Enabled
- FIFO Timeout: 0xFF

Expected Results:
If data transmitted by USART1 completely matches data received by USART2, and data transmitted by USART2 completely matches data received by USART1,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX
- USART1 RX connects to USART2 TX

/*!
    \file  README.TXT
    \brief USART 9-bit Data Mode Communication Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies bidirectional communication functionality of USART3 and USART4 in 9-bit data mode. 9-bit data mode is commonly used for multiprocessor communication, where the 9th bit can be used for address/data identification or parity check and other special purposes.

Test Procedure:
1. Configure USART3 and USART4 for 9-bit data length mode
2. Disable parity check, use complete 9-bit data bits
3. USART4 sends 10 9-bit data to USART3
4. USART3 receives and stores 9-bit data
5. Switch direction: USART3 sends 9-bit data to USART4
6. USART4 receives and stores data
7. Verify integrity of bidirectional 9-bit data transfer

Test Data:
USART3 Send Data (9-bit):
- Data content: 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A
- Data length: 10 9-bit data
- Data range: 257-266 (decimal)

USART4 Send Data (9-bit):
- Data content: 0x1F6, 0x1F7, 0x1F8, 0x1F9, 0x1FA, 0x1FB, 0x1FC, 0x1FD, 0x1FE, 0x1FF
- Data length: 10 9-bit data
- Data range: 502-511 (decimal)

Configuration Parameters:
- Data Bit Length: 9 bits
- Baud Rate: 115200 bps
- Stop Bits: 1 bit
- Parity Check: Disabled
- Hardware Flow Control: Disabled
- FIFO Timeout: 0xFF
- Data Size: Byte mode

Expected Results:
If 9-bit data transmitted by USART3 completely matches data received by USART4,
and 9-bit data transmitted by USART4 completely matches data received by USART3,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 RX
- USART3 RX connects to USART4 TX

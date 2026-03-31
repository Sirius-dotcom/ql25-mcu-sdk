/*!
    \file  README.TXT
    \brief USART RS485 Communication Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies communication functionality of USART1 and USART2 in RS485 mode. RS485 is a differential signal communication standard that supports multi-point communication, featuring strong anti-interference capability and long transmission distance, widely used in industrial automation fields.

Test Procedure:
1. Configure basic communication parameters of USART1 and USART2
2. Enable RS485 mode and CTS hardware flow control
3. Configure RS485 specific parameters: enable, assertion advance, de-assertion delay
4. USART1 sends data to USART2
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
- RS485 Mode: Enabled
- Hardware Flow Control: CTS enabled, RTS disabled
- DE Assertion Advance: Enabled (drive enable before transmission)
- DE De-assertion Delay: Enabled (delay cancel drive enable after transmission)
- DE Polarity: Normal (not inverted)
- Baud Rate: 115200 bps
- Data Bits: 8 bits
- Stop Bits: 1 bit
- Parity: None
- FIFO Timeout: 0xFF

Expected Results:
If data transmitted by USART1 completely matches data received by USART2, and data transmitted by USART2 completely matches data received by USART1,
then test passes, output "pass"; otherwise test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX
- USART1 RX connects to USART2 TX

/*!
    \file  README.TXT
    \brief USART Hardware Flow Control Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies the communication function of USART3 and USART4 in hardware flow control mode. When CTSE=1, CTS input pin state changes automatically set status bits (CTS_RISE_FLAG and CTS_FALL_FLAG), used to indicate whether the receiver is ready for communication.

Test Procedure:
1. Configure hardware flow control functions of USART3 and USART4 (RTS and CTS)
2. Enable CTS rising edge and falling edge interrupts
3. USART3 sends data, USART4 receives data
4. Detect flow control signal changes through CTS interrupts
5. Verify data transmission integrity
6. Disable hardware flow control, compare test results

Test Data:
- Transmitted data: 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA
- Data length: 10 bytes
- Baud rate: 115200 bps
- Data bits: 8 bits
- Stop bits: 1 bit
- Parity: None

Configuration Parameters:
- Hardware flow control: Both RTS and CTS enabled
- Interrupt configuration: CTS rising edge and falling edge interrupts
- FIFO timeout: 0xFF
- Data size: Byte mode

Expected Results:
If data transmitted by USART3 completely matches data received by USART4, the test passes, output "pass";
otherwise the test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 RX
- USART3 RX connects to USART4 TX  
- USART3 CTS connects to USART4 RTS
- USART3 RTS connects to USART4 CTS
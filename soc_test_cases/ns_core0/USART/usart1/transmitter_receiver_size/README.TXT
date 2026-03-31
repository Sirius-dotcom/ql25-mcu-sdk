/*!
    \file  README.TXT
    \brief USART Transfer Size Count Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies USART transfer size counting functionality. When transmitted or received data reaches preset count value, USART will set RXTX_SIZE flag bit, used for precise control of transferred data volume, suitable for application scenarios requiring precise data volume management.

Test Procedure:
1. Configure RXTX_SIZE counters of USART1 and USART2
2. Set transfer size count value (e.g., 10 bytes)
3. First phase: USART1 sends data to USART2
4. When transmitted data volume reaches set value, RXTX_SIZE flag is set
5. USART2 receives data, RXTX_SIZE flag is also set when received volume reaches set value
6. Second phase: USART2 sends data to USART1
7. Similarly monitor RXTX_SIZE flag status
8. Compare consistency of transmitted and received data
9. Verify correctness of RXTX_SIZE functionality

Test Data:
USART1 Send Data:
- Data content: 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A
- Data length: 10 bytes
- Transfer size set value: 10 bytes

USART2 Send Data:
- Data content: 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A
- Data length: 10 bytes
- Transfer size set value: 10 bytes

Configuration Parameters:
- RXTX_SIZE Function: Enabled
- Transfer Size Set Value: 10 bytes
- Baud Rate: 115200 bps
- Data Bits: 8 bits
- Stop Bits: 1 bit
- Parity: None
- Counting Mode: Independent counting for transmit and receive
- Flag Check: Software polling check

Counting Mechanism:
- Transmit Count: Transmit counter decrements by 1 for each byte sent
- Receive Count: Receive counter decrements by 1 for each byte received
- Flag Setting: RXTX_SIZE flag is set when counter decrements to 0
- Flag Clear: Counting can restart after software clears flag
- Interrupt Support: RXTX_SIZE interrupt can be configured

Expected Results:
If data transmitted by USART1 completely matches data received by USART2,
and data transmitted by USART2 completely matches data received by USART1,
and RXTX_SIZE flag is set at correct timing,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX
- USART1 RX connects to USART2 TX

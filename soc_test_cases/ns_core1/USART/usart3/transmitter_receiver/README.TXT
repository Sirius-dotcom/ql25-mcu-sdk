/*!
    \file  README.TXT
    \brief USART Basic Transceiver Communication Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies basic data transmission function of USART in standard full-duplex mode. This is the most basic and commonly used communication mode of USART, supporting simultaneous transmit and receive operations, suitable for most serial communication application scenarios.

Test Procedure:
1. Configure USART3 and USART4 for standard full-duplex mode
2. Clear transmit and receive FIFO
3. First phase: USART3 sends 10 bytes data, USART4 receives and stores
4. Verify correctness of first phase data transmission
5. Clear FIFO buffers
6. Second phase: USART4 sends 10 bytes data, USART3 receives and stores
7. Verify correctness of second phase data transmission
8. Compare integrity of bidirectional data transmission

Test Data:
USART3 sends data:
- Data content: 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A
- Data length: 10 bytes
- Data pattern: Incremental sequence (1-10)

USART4 sends data:
- Data content: 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A
- Data length: 10 bytes
- Data pattern: Incremental sequence (17-26)

Configuration Parameters:
- Communication mode: Full-duplex
- Baud rate: 115200 bps
- Data bits: 8 bits
- Stop bits: 1 bit
- Parity: None
- Hardware flow control: Disabled
- FIFO timeout: 0xFF
- Data size: Byte mode

Expected Results:
If data transmission in both directions is correct:
- Data transmitted by USART3 completely matches data received by USART4
- Data transmitted by USART4 completely matches data received by USART3
Then test passes, output "pass";
Otherwise test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 RX
- USART3 RX connects to USART4 TX
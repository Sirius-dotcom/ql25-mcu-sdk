/*!
    \file  README.TXT
    \brief USART RS485 Interrupt Mode Communication Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies interrupt-driven communication functionality of USART3 and USART4 in RS485 mode. RS485 is a differential signal communication standard that supports multi-device bus communication, featuring strong anti-interference capability and long transmission distance, widely used in industrial automation and building control systems.

Test Procedure:
1. Configure USART3 and USART4 for RS485 mode
2. Enable RS485 transmit enable control function
3. Configure USART transmit and receive interrupts
4. First phase: USART3 transmits, USART4 receives
5. USART3 sends 8 bytes data through interrupts
6. USART4 receives data through interrupts and stores
7. Second phase: USART4 transmits, USART3 receives
8. USART4 sends 8 bytes data through interrupts
9. USART3 receives data through interrupts and stores
10. Compare integrity of bidirectional data transfer

Test Data:
USART3 Send Data:
- Data content: 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
- Data length: 8 bytes
- Data pattern: Incremental sequence

USART4 Send Data:
- Data content: 0x15, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
- Data length: 8 bytes
- Data pattern: Mixed sequence

Configuration Parameters:
- RS485 Mode: Enabled
- Transmit Enable: Hardware automatic control
- Baud Rate: 115200 bps
- Data Bits: 8 bits
- Stop Bits: 1 bit
- Parity: None
- Interrupt Mode: Transmit complete interrupt + receive data interrupt
- Interrupt Priority: ECLIC configuration

Expected Results:
If data transmitted by USART3 completely matches data received by USART4,
and data transmitted by USART4 completely matches data received by USART3,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 RX
- USART3 RX connects to USART4 TX

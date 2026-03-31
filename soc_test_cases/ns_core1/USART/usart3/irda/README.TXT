/*!
    \file  README.TXT
    \brief USART IrDA Infrared Communication Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies the bidirectional communication functionality of USART3 and USART4 in IrDA (Infrared Data Association) mode. IrDA mode transmits data through infrared light, suitable for short-distance wireless communication.

Test Procedure:
1. Configure IrDA functionality for USART3 and USART4
2. Set IrDA prescaler value to 8
3. Configure USART3 as transmit mode, USART4 as receive mode
4. USART3 sends 20 bytes of data, USART4 receives and stores
5. Switch configuration: USART4 as transmit mode, USART3 as receive mode
6. USART4 sends the same data, USART3 receives and stores
7. Compare data consistency between transmitted and received data

Test Data:
- Transmit Data: 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
                 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10
- Data Length: 20 bytes
- Baud Rate: 115200 bps
- Data Bits: 8 bits
- Stop Bits: 1 bit
- Parity: None
- IrDA Prescaler: 8

Configuration Parameters:
- IrDA Mode: Enabled
- Communication Direction: Half-duplex (time-division transmit/receive)
- FIFO Timeout: 0xFF
- Data Size: Byte mode

Expected Results:
If the data transmitted by USART3 matches exactly with the data received by USART4, and the data transmitted by USART4 also matches exactly with the data received by USART3,
then the test passes, output "pass"; otherwise the test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 RX
- USART3 RX connects to USART4 TX

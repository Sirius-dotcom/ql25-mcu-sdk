/*!
    \file  README.TXT
    \brief USART Interrupt Mode Transceiver Communication Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This is a USART interrupt mode transceiver communication test program used to verify USART data transmission functionality in interrupt-driven mode. The program uses interrupt mode to handle transmission and reception, triggering interrupts through watermark mechanism, improving system efficiency.

Main Features:
1. Watermark Interrupt Mechanism:
   - Set TX watermark depth of USART1 and USART2 to 1
   - Generate transmit interrupt when TX data is less than set depth
   - Generate receive watermark interrupt when there is data in RX
   - Automatically manage data transmission process

2. Interrupt-Driven Communication:
   - Configure TX/RX interrupts of USART1 and USART2
   - Use interrupt service routines to handle data transceiver
   - Dynamically track transmit and receive counts
   - Automatically disable corresponding interrupts after completion

3. Bidirectional Data Transfer:
   - USART1 and USART2 perform interrupt transceiver simultaneously
   - Verify data integrity in interrupt mode
   - Support non-blocking data transfer

Test Procedure:
1. Initialize USART1 and USART2, configure as standard full-duplex mode
2. Register ECLIC interrupt service routines
3. Configure TX/RX watermarks: TX watermark=1, RX watermark=0
4. Enable TX/RX interrupts of USART1 and USART2
5. Wait for all data transfer completion (8 bytes)
6. Compare transmitted and received data

IO Connection Information:
- USART1 TX connects to USART2 RX
- USART1 RX connects to USART2 TX
- Use two USARTs to implement interrupt-driven full-duplex communication

Test Data:
- USART1 Send Data: [0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08]
- USART2 Send Data: [0x15, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18]
- Data Length: 8 bits
- Data Count: 8 each

Configuration Parameters:
- Baud Rate: 115200 bps
- Data Bits: 8 bits
- Stop Bits: 1 bit
- Parity: None
- Communication Mode: Full-duplex
- Interrupt Mode: ECLIC non-vector interrupt

Interrupt Configuration:
- USART1 Interrupt: Priority 1, Level 1
- USART2 Interrupt: Priority 1, Level 1
- TX Watermark: 1 byte (triggered when TX FIFO data < 1)
- RX Watermark: 0 byte (triggered when RX FIFO has data)

Test Results:
If interrupt-driven data transfer in both directions is correct (transmitted data completely matches received data), program outputs "pass";
If data transfer in either direction has errors, program outputs "fail".
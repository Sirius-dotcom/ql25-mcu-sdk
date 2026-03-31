/*!
    \file  README.TXT
    \brief USART DMA Interrupt Single-Direction Transceiver Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies unidirectional DMA communication from USART3 (transmitter) to USART4 (receiver) with DMA interrupt handling. When DMA transfer completes, interrupts are generated, and the system handles transfer completion events in interrupts, improving real-time performance.

Test Procedure:
1. Configure DMA functionality of USART3 and USART4
2. Register DMA interrupt handler (UDMA0_IRQHandler)
3. Enable DMA receive completion interrupt for USART4
4. Configure USART3 for DMA transmit mode (TX enabled, RX disabled)
5. Configure USART4 for DMA receive mode (RX enabled, TX disabled)
6. Set up DMA channels: USART3 TX DMA and USART4 RX DMA
7. Start DMA transfer from USART3 to USART4
8. Wait for DMA transfer completion interrupt (receive_flag set in ISR)
9. Copy received data from DMA destination buffer
10. Compare transmitted data with received data
11. Determine test result based on comparison

Test Data:
Transmitted Data (USART3):
- Data content: 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x08, 0x19, 0x1A
- Data length: 10 bytes
- Data type: uint32_t array (but treated as bytes for comparison)

Received Data (USART4):
- Expected: Same as transmitted data
- Data length: 10 bytes

Configuration Parameters:
- Transfer Direction: USART3 <-> USART4 (unidirectional)
- DMA Mode: Normal mode (single transfer)
- Data Width: 32 bits for transmission, 8 bits for reception
- Buffer Size: 10 bytes
- Baud Rate: 115200 bps
- Data Bits: 8 bits
- Stop Bits: 1 bit
- Parity: None
- Hardware Flow Control: Disabled
- FIFO Timeout: 0xFF

DMA Configuration:
- USART3 TX DMA: Memory-to-Peripheral (M2P)
- USART4 RX DMA: Peripheral-to-Memory (P2M)
- Source Increment: Enabled (for TX DMA)
- Destination Increment: Disabled (for TX DMA)
- Source Increment: Disabled (for RX DMA)
- Destination Increment: Enabled (for RX DMA)
- Transfer Mode: PA2M_MODE_NORMAL (single transfer)

Interrupt Handling:
- Interrupt Source: USART4 DMA receive completion interrupt
- Interrupt Handler: UDMA0_IRQHandler
- Synchronization: receive_flag set in interrupt, main program waits for it
- Flag Management: receive_flag cleared before transfer, set after completion

Expected Results:
If data transmitted by USART3 (0x11-0x1A with 0x08 at position 8) completely matches data received by USART4,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 RX

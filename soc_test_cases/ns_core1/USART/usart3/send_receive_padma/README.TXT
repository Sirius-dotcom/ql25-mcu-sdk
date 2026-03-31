/*!
    \file  README.TXT
    \brief USART DMA Single-Direction Transceiver Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies unidirectional DMA communication from USART3 (transmitter) to USART4 (receiver). The test uses DMA mode to achieve high-speed data transmission without CPU intervention, demonstrating basic DMA transfer functionality.

Test Procedure:
1. Configure USART3 for DMA transmit mode (TX enabled, RX disabled)
2. Configure USART4 for DMA receive mode (RX enabled, TX disabled)
3. Set up DMA channels: USART3 TX DMA and USART4 RX DMA
4. Configure source address (memory) and destination address (USART TXDATA register)
5. Configure DMA destination (memory) and source (USART RXDATA register)
6. Start DMA transmission from USART3 to USART4
7. Wait for DMA transfer completion interrupt
8. Copy received data from DMA destination buffer
9. Compare transmitted data with received data
10. Determine test result based on comparison

Test Data:
Transmitted Data (USART3):
- Data content: 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A
- Data length: 10 bytes
- Data pattern: Sequential incremental pattern

Received Data (USART4):
- Expected: Same as transmitted data
- Data length: 10 bytes

Configuration Parameters:
- Transfer Direction: USART3 <->USART4 (unidirectional)
- DMA Mode: Normal mode (single transfer)
- Data Width: 8 bits
- Buffer Size: 10 bytes
- Baud Rate: 115200 bps
- Data Bits: 8 bits
- Stop Bits: 1 bit
- Parity: None
- Hardware Flow Control: Disabled
- FIFO Timeout: 0xFF

DMA Configuration:
- Source Increment: Enabled (for TX DMA)
- Destination Increment: Disabled (for TX DMA)
- Source Increment: Disabled (for RX DMA)
- Destination Increment: Enabled (for RX DMA)
- Transfer Mode: PA2M_MODE_NORMAL (single transfer)

Expected Results:
If data transmitted by USART3 (0x11-0x1A) completely matches data received by USART4,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 RX

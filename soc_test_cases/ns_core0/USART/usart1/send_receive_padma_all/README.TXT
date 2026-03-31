/*!
    \file  README.TXT
    \brief USART DMA Full-Duplex Communication Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies full-duplex communication functionality of USART1 and USART2 in DMA mode. Both USARTs simultaneously use DMA for transmission and reception, achieving the most efficient data transfer, suitable for applications with large data volumes and high throughput requirements.

Test Procedure:
1. Configure DMA functionality of USART1 and USART2
2. USART1 transmit DMA and receive DMA configured simultaneously
3. USART2 transmit DMA and receive DMA configured simultaneously
4. Start bidirectional DMA transmission
5. USART1 sends data through DMA, USART2 receives through DMA
6. USART2 sends data through DMA, USART1 receives through DMA
7. Wait for DMA transmission completion
8. Compare consistency of transmitted and received data

Test Data:
USART1 Send Data:
- Data content: 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09
- Data length: 10 bytes
- Data pattern: Incremental sequence

USART2 Send Data:
- Data content: 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19
- Data length: 10 bytes
- Data pattern: Incremental sequence

Configuration Parameters:
- DMA Mode: Full-duplex (transmit and receive enabled simultaneously)
- Data Width: 8 bits
- Transfer Direction: Memory to peripheral, peripheral to memory
- Transfer Mode: Normal mode (non-circular)
- Priority: High priority
- Baud rate: 115200 bps
- Data bits: 8 bits
- Stop bits: 1 bit
- Parity: None

DMA Full-Duplex Characteristics:
- Simultaneous Transfer: Transmit and receive DMA work simultaneously
- High Efficiency: Zero CPU intervention data transfer
- Full Bandwidth: Fully utilize USART bandwidth
- Low Latency: DMA direct memory access
- High Throughput: Support large data volume transfer
- Automation: Transfer process completely automated

DMA Configuration:
- Data Width: 8 bits (byte transfer)
- Address Mode: Memory address increment, peripheral address fixed
- Transfer Complete: DMA transfer completion interrupt

Expected Results:
If data transmitted by USART1 completely matches data received by USART2,
and data transmitted by USART2 completely matches data received by USART1,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART1 TX_DMA connects to USART2 RX_DMA
- USART1 RX_DMA connects to USART2 TX_DMA

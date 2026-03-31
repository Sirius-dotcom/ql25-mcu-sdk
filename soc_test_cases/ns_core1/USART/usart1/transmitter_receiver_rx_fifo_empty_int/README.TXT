/*!
    \file  README.TXT
    \brief USART Receive FIFO Empty Interrupt Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies communication functionality of USART1 and USART2 in receive FIFO empty interrupt mode. When data in the receive FIFO is read and becomes empty, a receive FIFO empty interrupt is generated. The transmitter detects the empty interrupt and sends data, achieving efficient data flow control.

Test Procedure:
1. Configure receive FIFO empty interrupt of USART1 and USART2
2. USART1 sends data to USART2
3. USART2 receives data and stores to FIFO
4. USART2 reads data from FIFO, triggers empty interrupt when FIFO becomes empty
5. USART2 detects empty interrupt and sends received data back to USART1
6. USART1 receives echo data and verifies
7. Compare consistency of original transmitted data and final received data

Test Data:
USART1 Send Data:
- Data content: 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
- Data length: 8 bytes
- Data pattern: Incremental sequence

Configuration Parameters:
- Interrupt Type: Receive FIFO empty interrupt
- FIFO Depth: According to hardware configuration (usually 8-16 bytes)
- Baud Rate: 115200 bps
- Data Bits: 8 bits
- Stop Bits: 1 bit
- Parity: None
- Interrupt Priority: ECLIC configuration
- Communication Mode: Full-duplex

Interrupt Handling Process:
USART2 Interrupt Handling:
1. Detect RX FIFO empty interrupt
2. Read all data from FIFO to buffer
3. Disable RX FIFO empty interrupt
4. Send received data back to USART1
5. Wait for transmission completion

USART1 Handling:
1. Receive data echoed from USART2
2. Compare transmitted and received data
3. Verify integrity of data transfer

FIFO Management:
- Empty Detection: Hardware automatically detects FIFO empty status
- Full Detection: Cooperate with FIFO full interrupt for complete flow control
- Threshold Control: Settable FIFO interrupt trigger threshold
- Status Monitoring: Real-time monitoring of FIFO status
- Buffer Management: Effective management of receive buffer

Expected Results:
If data transmitted by USART1 completely matches data finally received from USART2 echo,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX
- USART1 RX connects to USART2 TX

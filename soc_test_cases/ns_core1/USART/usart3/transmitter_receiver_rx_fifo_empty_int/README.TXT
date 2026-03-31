/*!
    \file  README.TXT
    \brief USART Receive FIFO Empty Interrupt Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies communication functionality of USART3 and USART4 in receive FIFO empty interrupt mode. When data in the receive FIFO is read and becomes empty, a receive FIFO empty interrupt is generated. The transmitter detects the empty interrupt and sends data, achieving efficient data flow control.

Test Procedure:
1. Configure receive FIFO empty interrupt of USART3 and USART4
2. USART3 sends data to USART4
3. USART4 receives data and stores to FIFO
4. USART4 reads data from FIFO, triggers empty interrupt when FIFO becomes empty
5. USART4 detects empty interrupt and sends received data back to USART3
6. USART3 receives echo data and verifies
7. Compare consistency of original transmitted data and final received data

Test Data:
USART3 Send Data:
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
USART4 Interrupt Handling:
1. Detect RX FIFO empty interrupt
2. Read all data from FIFO to buffer
3. Disable RX FIFO empty interrupt
4. Send received data back to USART3
5. Wait for transmission completion

USART3 Handling:
1. Receive data echoed from USART4
2. Compare transmitted and received data
3. Verify integrity of data transfer

FIFO Management:
- Empty Detection: Hardware automatically detects FIFO empty status
- Full Detection: Cooperate with FIFO full interrupt for complete flow control
- Threshold Control: Settable FIFO interrupt trigger threshold
- Status Monitoring: Real-time monitoring of FIFO status
- Buffer Management: Effective management of receive buffer

Expected Results:
If data transmitted by USART3 completely matches data finally received from USART4 echo,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 RX
- USART3 RX connects to USART4 TX

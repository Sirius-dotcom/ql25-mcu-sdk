/*!
    \file  README.TXT
    \brief USART Transmit FIFO Empty Interrupt Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies communication functionality of USART1 and USART2 in transmit FIFO empty interrupt mode. When transmit FIFO is empty, an interrupt is generated. The transmitter can fill new data in the interrupt, achieving continuous data transmission.

Test Procedure:
1. Configure transmit FIFO empty interrupt of USART1 and USART2
2. Initially USART1's TX FIFO is empty, triggering interrupt
3. USART1 sends 8 bytes data to USART2 in interrupt
4. USART2 receives data and waits for its TX FIFO empty interrupt
5. USART2 echoes data back to USART1 in TX FIFO empty interrupt
6. USART1 receives echo data and verifies integrity
7. Achieve bidirectional communication through interrupt switching

Test Data:
USART1 Send Data:
- Data content: 0x01, 0x02, 0x13, 0x14, 0x05, 0x08, 0x06, 0x1A
- Data length: 8 bytes
- Data pattern: Mixed pattern, containing various test values

Configuration Parameters:
- Interrupt Type: Transmit FIFO empty interrupt
- FIFO Depth: According to hardware configuration
- Baud Rate: 115200 bps
- Data Bits: 8 bits
- Stop Bits: 1 bit
- Parity: None
- Interrupt Priority: ECLIC configuration

Interrupt Handling Mechanism:
USART1 Interrupt Handling:
- Detect TX FIFO empty interrupt
- Clear USART2's RX and TX FIFO
- Send 8 bytes data to USART2
- Disable USART1's TX FIFO empty interrupt
- Enable USART2's TX FIFO empty interrupt

USART2 Interrupt Handling:
- Detect TX FIFO empty interrupt
- Receive data sent by USART1
- Clear USART1's RX and TX FIFO
- Echo received data back to USART1
- Disable USART2's TX FIFO empty interrupt
- Set completion flag

Expected Results:
If data transmitted by USART1 completely matches data received by USART2, and echo data is also correctly received by USART1,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX
- USART1 RX connects to USART2 TX
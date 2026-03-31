/*!
    \file  README.TXT
    \brief USART Transmit FIFO Empty Interrupt Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies communication functionality of USART3 and USART4 in transmit FIFO empty interrupt mode. When transmit FIFO is empty, an interrupt is generated. The transmitter can fill new data in the interrupt, achieving continuous data transmission.

Test Procedure:
1. Configure transmit FIFO empty interrupt of USART3 and USART4
2. Initially USART3's TX FIFO is empty, triggering interrupt
3. USART3 sends 8 bytes data to USART4 in interrupt
4. USART4 receives data and waits for its TX FIFO empty interrupt
5. USART4 echoes data back to USART3 in TX FIFO empty interrupt
6. USART3 receives echo data and verifies integrity
7. Achieve bidirectional communication through interrupt switching

Test Data:
USART3 Send Data:
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
USART3 Interrupt Handling:
- Detect TX FIFO empty interrupt
- Clear USART4's RX and TX FIFO
- Send 8 bytes data to USART4
- Disable USART3's TX FIFO empty interrupt
- Enable USART4's TX FIFO empty interrupt

USART4 Interrupt Handling:
- Detect TX FIFO empty interrupt
- Receive data sent by USART3
- Clear USART3's RX and TX FIFO
- Echo received data back to USART3
- Disable USART4's TX FIFO empty interrupt
- Set completion flag

Expected Results:
If data transmitted by USART3 completely matches data received by USART4, and echo data is also correctly received by USART3,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 RX
- USART3 RX connects to USART4 TX
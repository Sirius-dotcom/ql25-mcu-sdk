/*!
    \file  README.TXT
    \brief USART Receive FIFO Full Interrupt Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies communication functionality of USART1 and USART2 in receive FIFO full interrupt mode. When data in receive FIFO exceeds set threshold, a receive FIFO full interrupt is generated. The receiver reads data in the interrupt, improving reception efficiency.

Test Procedure:
1. Configure receive FIFO full interrupt of USART1 and USART2
2. USART1 sends 16 bytes data to USART2
3. When USART2's RX FIFO is full, trigger interrupt
4. USART2 reads all received data in interrupt
5. USART2 sends received data back to USART1
6. When USART1's RX FIFO is full, trigger interrupt
7. USART1 reads echo data in interrupt
8. Verify integrity of data transfer

Test Data:
USART1 Send Data:
- Data content: 0x01, 0x02, 0x03, 0x01, 0x02, 0x03, 0x01, 0x02, 0x03, 0x01, 0x02, 0x03, 0x01, 0x02, 0x03, 0x01
- Data length: 16 bytes
- Data pattern: Repeating pattern (0x01,0x02,0x03 cycle)

Configuration Parameters:
- Interrupt Type: Receive FIFO full interrupt
- FIFO Depth: According to hardware configuration
- Baud Rate: 115200 bps
- Data Bits: 8 bits
- Stop Bits: 1 bit
- Parity: None
- Interrupt Priority: ECLIC configuration

Interrupt Handling Mechanism:
USART2 Interrupt Handling:
- Detect RX FIFO full interrupt
- Read all data from FIFO to temp1 array
- Disable RX FIFO full interrupt
- Detect receive idle status
- Clear FIFO and echo data

USART1 Interrupt Handling:
- Detect RX FIFO full interrupt
- Read all data from FIFO to temp array
- Disable RX FIFO full interrupt
- Set completion flag

Expected Results:
If data transmitted by USART1 completely matches data received by USART2, and echo data is also correctly received,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX
- USART1 RX connects to USART2 TX

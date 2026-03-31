/*!
    \file  README.TXT
    \brief USART Transmit FIFO Full Interrupt Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies USART transmit FIFO full interrupt functionality. When writing data to transmit FIFO causes FIFO to become full, a transmit FIFO full interrupt is generated, used to monitor transmit buffer status and implement flow control.

Test Procedure:
1. Configure USART1's transmit FIFO full interrupt
2. Disable USART1's transmit function
3. Write data to USART1's TX FIFO until FIFO is full
4. When TX FIFO is full, trigger transmit FIFO full interrupt
5. Verify interrupt trigger in interrupt service routine
6. Check interrupt flag bit status
7. Verify correctness of transmit FIFO full interrupt

Test Data:
- Fill Data: 0x55, 0xAA, 0x33, 0xCC, etc. (used to fill FIFO)
- Data Length: Determined by FIFO depth (usually 8-16 bytes)
- Fill Method: Continuous write until FIFO is full

Configuration Parameters:
- Interrupt Type: Transmit FIFO full interrupt
- FIFO Depth: According to hardware configuration
- Transmit Status: Initially disabled
- Interrupt Priority: ECLIC configuration
- Data Width: 8 bits
- Baud Rate: 115200 bps

Expected Results:
If USART1's TX FIFO full interrupt can trigger correctly,
and interrupt service routine can execute correctly and detect FIFO full status,
then test passes, output "pass";
if transmit FIFO full interrupt does not trigger or handling is abnormal,
then test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX (for subsequent testing)
- USART1 RX connects to USART2 TX

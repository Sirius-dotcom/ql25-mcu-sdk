/*!
    \file  README.TXT
    \brief USART Stop Bit Interrupt Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies USART stop bit interrupt function. Stop bit interrupt is generated during the stop bit time of each data frame, which can be used for precise data reception timing control and data processing synchronization.

Test Procedure:
1. Configure USART2 to enable stop bit interrupt (SBKIE=1)
2. Configure USART1 as normal transmit mode
3. USART1 sends multiple data bytes to USART2
4. USART2 generates interrupt at stop bit time for each received byte
5. Count received bytes in interrupt service routine
6. Disable stop bit interrupt after receiving specified quantity
7. Verify whether interrupt trigger count is correct

Test Data:
- Send Data: Arbitrary byte data (test uses 0x55, 0xAA, etc.)
- Data Length: 2 bytes (used to test interrupt trigger 2 times)
- Data Content: 0x55, 0xAA
- Expected Interrupt Count: 2 times

Configuration Parameters:
- Stop Bit Interrupt: Enabled (SBKIE=1)
- Baud Rate: 115200 bps
- Data Bits: 8 bits
- Stop Bits: 1 bit
- Parity: None
- Interrupt Priority: Configure according to system requirements
- Receive Mode: Polling or interrupt mode

Interrupt Handling Process:
1. Reception Complete: Data frame reception complete, enter stop bit time
2. Interrupt Trigger: Hardware automatically triggers stop bit interrupt
3. Interrupt Response: CPU jumps to interrupt service routine
4. Data Processing: Process received data in interrupt
5. Count Update: Update reception counter
6. Interrupt Clear: Clear interrupt flag bit
7. Return to Main Program: Continue waiting for next interrupt

Expected Results:
If USART2's stop bit interrupt service routine executes correctly 2 times (corresponding to receiving 2 bytes of data),
then successfully disables interrupt, test passes, output "pass";
if interrupt execution count is incorrect or interrupt handling is abnormal, test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX
- USART1 RX connects to USART2 TX

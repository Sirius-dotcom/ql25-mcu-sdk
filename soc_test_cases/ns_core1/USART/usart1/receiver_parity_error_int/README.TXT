/*!
    \file  README.TXT
    \brief USART Parity Error Interrupt Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies USART parity error interrupt function. When parity configurations of transmitter and receiver do not match, the receiver generates parity error interrupts, used to detect data transmission errors.

Test Procedure:
1. Configure USART1 as even parity mode
2. Configure USART2 as odd parity mode (intentionally mismatched)
3. Enable USART2 parity error interrupt
4. USART1 sends 3 bytes data to USART2
5. USART2 detects parity errors while receiving data
6. Each parity error triggers interrupt and counts
7. Wait for all 3 bytes to generate parity error interrupts
8. Determine test result based on interrupt count

Test Data:
USART1 Send Data:
- Data content: 0x11, 0x12, 0x13
- Data length: 3 bytes
- Send parity: Even parity

Configuration Parameters:
- USART1 parity: Even parity (PARITY_EN | PARITY_SEL_DISABLE)
- USART2 parity: Odd parity (PARITY_EN | PARITY_SEL)
- Interrupt type: Parity error interrupt
- Baud rate: 115200 bps
- Data bits: 8 bits
- Stop bits: 1 bit
- Interrupt priority: ECLIC configuration

Parity Configuration:
- Even Parity: Ensure the number of 1s in the entire byte (including parity bit) is even
- Odd Parity: Ensure the number of 1s in the entire byte (including parity bit) is odd
- Parity Bit: 9th bit used for parity
- Error Detection: Receiver hardware automatically calculates and compares

Interrupt Handling Mechanism:
USART2 interrupt handling:
- Detect parity error status (RX_ERROR_PARITY)
- Increment err_count counter
- Clear parity error interrupt flag
- Continue receiving subsequent data

Expected Results:
If USART2's parity error interrupt handler executes correctly 3 times (corresponding to 3 bytes of parity errors),
then the test passes, output "pass"; otherwise the test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX
- USART1 acts as even parity transmitter, USART2 acts as odd parity receiver

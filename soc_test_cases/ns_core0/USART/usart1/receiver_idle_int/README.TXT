/*!
    \file  README.TXT
    \brief USART Receiver Idle Interrupt Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies the USART receiver idle interrupt functionality. When no data transmission occurs on the bus within a specified time, a receiver idle interrupt is generated to detect the end of communication frames or the idle state of the communication line.

Test Procedure:
1. Configure USART2 to enable receiver idle interrupt
2. USART1 sends single byte data 0xA5 to USART2
3. USART2 generates receiver idle interrupt after receiving data
4. Read data in interrupt and send back acknowledgment byte 0xA4
5. Wait for second idle interrupt (due to communication end)
6. Count interrupt occurrences and verify interrupt functionality
7. Determine test result based on interrupt count

Test Data:
USART1 sent data:
- Data content: 0xA5
- Data length: 1 byte

USART2 echoed data:
- Acknowledgment byte: 0xA4
- Trigger condition: During first idle interrupt

Configuration Parameters:
- Interrupt type: Receiver idle timeout interrupt
- Idle timeout: 0xFF (according to clock configuration)
- Baud rate: 115200 bps
- Data bits: 8 bits
- Stop bits: 1 bit
- Parity bits: None
- Interrupt priority: ECLIC configuration

Interrupt Handling Mechanism:
USART2 interrupt handling:
- Detect receiver idle timeout status
- First interrupt (state0=0): Read data, send acknowledgment, set state
- Second interrupt (state0=1): Clear interrupt flag, disable interrupt
- Interrupt count: err_count variable counting

Expected Results:
If USART2's idle interrupt handler executes correctly twice (first time receiving data, second time detecting communication end),
then test passes, output "pass"; otherwise test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX
- USART1 RX connects to USART2 TX

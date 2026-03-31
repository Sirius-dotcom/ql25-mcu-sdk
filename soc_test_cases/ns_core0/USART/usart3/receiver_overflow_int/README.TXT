/*!
    \file  README.TXT
    \brief USART Receive Overflow Interrupt Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies USART receive overflow interrupt function. When receive FIFO or buffer is full and data reception continues, overflow error interrupt will be generated, used to detect data loss situations.

Test Procedure:
1. Configure USART4 to enable receive overflow interrupt
2. USART3 continuously sends 18 bytes data (exceeding FIFO capacity)
3. Overflow error occurs during USART4 reception process
4. Each overflow error triggers interrupt and counts
5. Wait for overflow interrupt to trigger at least twice
6. Determine test result based on interrupt count
7. Disable overflow interrupt

Test Data:
USART3 sends data:
- Data content: 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A
- Data length: 18 bytes (designed to exceed receive FIFO capacity)
- Transmission method: Continuous transmission, without waiting for reception

Configuration Parameters:
- Interrupt type: Receive overflow interrupt
- Overflow detection: RX_ERROR_FLOW status
- Baud rate: 115200 bps
- Data bits: 8 bits
- Stop bits: 1 bit
- Parity: None
- Interrupt priority: ECLIC configuration

Interrupt Handling Mechanism:
USART4 interrupt handling:
- Detect receive overflow error status (RX_ERROR_FLOW)
- Clear receive overflow error interrupt flag
- Increment err_count counter
- Do not read data, maintain overflow state

Expected Results:
If USART4's overflow interrupt handler executes correctly at least twice (err_count >= 2),
indicating receive overflow detection function is normal, then test passes, output "pass"; otherwise test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 RX
- USART3 acts as data transmitter, USART4 acts as receiver

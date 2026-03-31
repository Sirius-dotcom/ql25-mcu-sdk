/*!
    \file  README.TXT
    \brief USART Receive/Transmit End Interrupt Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies USART receive/transmit end (EOT, End of Transmission) interrupt function. When transmit or receive counter decrements to 0, corresponding EOT interrupts are generated, used for precise control of data transmission completion timing.

Test Procedure:
1. Configure USART3 as loopback mode (TX connected to RX)
2. Set RX_SIZE and TX_SIZE to 3 bytes
3. Enable receive EOT and transmit EOT interrupts
4. Send 3 bytes data while receiving loopback data
5. Wait for both transmit counter and receive counter to decrement to 0
6. Trigger RX_EOT and TX_EOT interrupts
7. Clear interrupt flags and set completion status in interrupt
8. Verify whether interrupts trigger correctly

Test Data:
USART3 Send/Receive Data:
- Data content: 0x11, 0x12, 0x13
- Data length: 3 bytes
- Transmission mode: Loopback (TX->RX)

Configuration Parameters:
- Interrupt type: Receive EOT interrupt + Transmit EOT interrupt
- RX_SIZE: 3 bytes
- TX_SIZE: 3 bytes
- Baud rate: 115200 bps
- Data bits: 8 bits
- Stop bits: 1 bit
- Parity: None
- Interrupt priority: ECLIC configuration

Interrupt Handling Mechanism:
USART3 interrupt handling:
- Detect RX_EOT or TX_EOT interrupt status
- Clear RX_EOT and TX_EOT interrupt flags
- Set state0=0 indicating transmission complete
- Interrupt trigger condition: transmit or receive counter is 0

EOT Interrupt Characteristics:
- Transmit EOT: Triggered when TX counter decrements to 0
- Receive EOT: Triggered when RX counter decrements to 0
- Precise Counting: Based on actually transmitted bytes
- Auto Trigger: Hardware automatically detects counter status
- Completion Notification: Provides precise timing of transmission completion

Expected Results:
If USART3's EOT interrupt handler executes correctly, state0 is set to 0 (indicating transmission complete),
then test passes, output "pass"; otherwise test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART3 RX (loopback connection)

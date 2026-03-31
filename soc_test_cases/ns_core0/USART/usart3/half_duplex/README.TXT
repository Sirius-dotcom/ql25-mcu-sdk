/*!
    \file  README.TXT
    \brief USART Half-Duplex Communication Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies bidirectional communication function of USART in half-duplex mode. Half-duplex mode uses single data line for time-division transmit and receive, suitable for cost-sensitive or pin-limited application scenarios.

Test Procedure:
1. Configure USART3 and USART4 for half-duplex mode (HDSEL=1)
2. First phase: USART3 configured for transmit mode, USART4 configured for receive mode
3. USART3 sends 10 bytes data, USART4 receives and stores
4. Second phase: USART4 configured for transmit mode, USART3 configured for receive mode
5. USART4 sends 10 bytes data, USART3 receives and stores
6. Compare consistency of transmitted and received data
7. Verify correctness of half-duplex communication

Test Data:
USART3 sends data:
- Data content: 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A
- Data length: 10 bytes
- Data range: 1-10 (decimal)

USART4 sends data:
- Data content: 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A
- Data length: 10 bytes
- Data range: 17-26 (decimal)

Configuration Parameters:
- Communication mode: Half-duplex (HDSEL=1)
- Baud rate: 115200 bps
- Data bits: 8 bits
- Stop bits: 1 bit
- Parity: None
- Hardware flow control: Disabled
- FIFO timeout: 0xFF
- Data size: Byte mode

Half-Duplex Mode Features:
- Single-wire communication: TX and RX internally connected, sharing same pin
- Time-division operation: Transmit and receive cannot occur simultaneously
- Direction control: Control transmit/receive direction switching through software
- Pin saving: Saves one pin compared to full-duplex mode
- Cost advantage: Reduces hardware cost and PCB area
- Application scenarios: RS-485, single-wire communication, etc.

Technical Implementation:
- HDSEL control: HDSEL bit in USART_SETUP register controls half-duplex mode
- Direction switching: Configure for transmit mode before sending, configure for receive mode before receiving

Expected Results:
If data transmission in both directions is correct (data transmitted by USART3 completely matches data received by USART4,
and data transmitted by USART4 completely matches data received by USART3), test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 TX (special connection for half-duplex mode)
- Both USARTs use TX pin for half-duplex communication
- Note: In half-duplex mode, TX and RX are internally connected

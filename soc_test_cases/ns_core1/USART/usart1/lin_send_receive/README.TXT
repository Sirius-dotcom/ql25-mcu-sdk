/*!
    \file  README.TXT
    \brief LIN Bus Communication Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies the complete communication functionality of USART in LIN (Local Interconnect Network) mode, including frame header transmission, data transfer, checksum verification, and other core features of LIN protocol. LIN protocol is a low-cost serial communication protocol widely used in automotive electronics.

Test Procedure:
1. Configure USART1 as LIN master mode, USART2 as LIN slave mode
2. USART1 sends LIN frame header (Break + Sync field + PID)
3. USART2 as slave receives frame header and parses PID
4. Based on PID type, USART2 sends response data (if it's a transmit frame)
5. USART1 receives slave response data
6. Verify data integrity and checksum correctness
7. Test bidirectional LIN communication functionality

Test Data:
Master Send (USART1):
- Sync Field: 0x55 (LIN standard sync character)
- PID: 0x1D (ID=22, parity bits calculated)
- Data Field: 0x01, 0x02, 0x03, 0x04, 0x11, 0x12, 0x13, 0x14
- Checksum: Enhanced checksum (includes PID and data)

Slave Response (USART2):
- Respond corresponding data based on received ID
- When ID=0x22 responds: 0x01, 0x02, 0x03, 0x04, 0x11, 0x22, 0x33, 0x44
- Automatically calculate and append checksum

Configuration Parameters:
- LIN Mode: Enabled (LINEN=1)
- Baud Rate: 10400 bps (LIN standard baud rate)
- Data Bits: 8 bits
- Stop Bits: 1 bit (LIN format)
- Parity: None
- Break Detection: Enabled
- Checksum Mode: Enhanced checksum
- Master-Slave Mode: Dynamic switching

Checksum Calculation:
- Standard Checksum: Calculated only on data field
- Enhanced Checksum: Calculated on PID and data field
- Initial Value: Enhanced checksum initial value is PID

Expected Results:
If LIN communication is completely correct, including:
- Break field correctly generated and detected
- Sync field correctly received
- PID parity check correct
- Data transmission without error
- Checksum verification passed
then the test passes, output "pass";
otherwise the test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX 
- USART2 TX connects to USART1 RX 

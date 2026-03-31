/*!
    \file  README.TXT
    \brief USART Modbus ASCII Protocol Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies communication function of USART1 and USART2 in Modbus ASCII protocol mode. Modbus ASCII is an industrial communication protocol based on ASCII characters, using colon (:) as start character and carriage return line feed (CR LF) as end character.

Test Procedure:
1. Configure USART1 and USART2 to support Modbus ASCII protocol
2. Enable Modbus ASCII match interrupt
3. USART1 sends 18-byte Modbus ASCII request data
4. USART2 sends 27-byte Modbus ASCII response data
5. Detect Modbus ASCII frame match completion through interrupt
6. Verify integrity of transmitted and received data

Test Data:
USART1 sends data (18 bytes):
- 0x3A 0x30 0x31 0x30 0x33 0x30 0x30 0x30 0x30 0x30 0x30 0x30 0x34 0x46 0x38 0x0D 0x0A 0x00

USART2 sends data (27 bytes):
- 0x3A 0x30 0x31 0x30 0x33 0x30 0x38 0x30 0x30 0x30 0x31 0x30 0x30 0x30 0x32 0x30 0x30 0x30 0x33 0x30 0x30 0x30 0x34 0x45 0x41 0x0D 0x0A

Configuration Parameters:
- Protocol mode: Modbus ASCII
- Frame format: Start character (:) + Data + LRC checksum + End character (CR LF)
- Interrupt: Modbus ASCII match interrupt
- Baud rate: 115200 bps
- Data bits: 8 bits
- Stop bits: 1 bit
- Parity: None

Expected Results:
If data transmitted by USART1 completely matches data received by USART2, and data transmitted by USART2 completely matches data received by USART1,
then test passes, output "pass"; otherwise test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX
- USART1 RX connects to USART2 TX

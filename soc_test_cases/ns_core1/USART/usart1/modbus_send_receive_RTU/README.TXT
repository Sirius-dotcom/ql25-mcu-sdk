/*!
    \file  README.TXT
    \brief USART Modbus RTU Protocol Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies the communication functionality of USART1 and USART2 in Modbus RTU protocol mode. Modbus RTU is a binary format industrial communication protocol with high efficiency and compact characteristics, widely used in industrial automation fields.

Test Procedure:
1. Configure USART1 and USART2 to support Modbus RTU protocol
2. Enable receive idle timeout and Modbus RTU timeout interrupts
3. USART1 sends 8 bytes of Modbus RTU request data
4. USART2 sends 25 bytes of Modbus RTU response data
5. Detect frame reception completion through idle timeout interrupt
6. Verify integrity of transmitted and received data

Test Data:
USART1 Send Data (8 bytes):
- 0x01 0x03 0x00 0x00 0x00 0x0A 0xC5 0xCD
- Analysis: Device address 1 + Function code 03 + Start address 0x0000 + Register count 10 + CRC check 0xC5CD
- Meaning: Read 10 holding registers of device 1 (addresses 0x0000-0x0009)

USART2 Send Data (25 bytes):
- 0x01 0x03 0x14 0x00 0x01 0x00 0x02 0x00 0x03 0x00 0x04 0x00 0x05 0x00 0x06 0x00 0x07 0x00 0x08 0x00 0x09 0x00 0x0A 0x8F 0x16
- Analysis: Device address 1 + Function code 03 + Byte count 20 + 20 data bytes + CRC check 0x8F16
- Meaning: Return 20 register data (values 1-10)

Configuration Parameters:
- Protocol Mode: Modbus RTU
- Frame Format: Device address + Function code + Data + CRC check
- Interrupts: Receive idle timeout interrupt + Modbus RTU timeout interrupt
- Baud Rate: 115200 bps
- Data Bits: 8 bits
- Stop Bits: 1 bit
- Parity: None
- Idle Timeout: 3.5 character times

Expected Results:
If the data transmitted by USART1 matches exactly with the data received by USART2, and the data transmitted by USART2 also matches exactly with the data received by USART1,
then the test passes, output "pass"; otherwise the test fails, output "fail".

IO Connection Description:
- USART1 TX connects to USART2 RX
- USART1 RX connects to USART2 TX

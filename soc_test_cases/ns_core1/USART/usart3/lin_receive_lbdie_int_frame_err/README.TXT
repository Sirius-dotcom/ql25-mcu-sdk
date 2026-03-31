/*!
    \file  README.TXT
    \brief LIN Bus Break Detection Interrupt and Frame Error Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies the Break detection interrupt functionality and frame error detection capability of USART in LIN (Local Interconnect Network) mode. LIN protocol is widely used in automotive electronic systems for low-cost in-vehicle network communication.

Test Procedure:
1. Configure USART3 as LIN master mode, USART4 as LIN slave mode
2. USART4 enables LIN mode Break detection interrupt (LBDIE=1)
3. USART3 sends Break field (forces line low for more than 10 bit times)
4. USART4 detects Break field and triggers LIN Break detection interrupt
5. Verify whether interrupt service routine executes correctly
6. Test frame error detection functionality (optional)
7. Check interrupt flag bits and status registers

Test Data:
- Break Field: Low level for more than 10 bit times
- Sync Field: 0x55 (LIN protocol standard sync character)
- PID: Set according to test requirements (usually test-specific ID)
- Data Field: Optional, used for frame error testing
- Checksum: Optional, used for integrity verification

Configuration Parameters:
- LIN Mode: Enabled (LINEN=1)
- Break Detection Interrupt: Enabled (LBDIE=1)
- Baud Rate: 10400 bps (LIN standard baud rate)
- Data Bits: 8 bits
- Stop Bits: 1 bit (format in LIN mode)
- Parity: None
- Frame Error Detection: Enabled
- Interrupt Priority: Configure according to system requirements

Expected Results:
If USART4 can correctly detect the Break field sent by USART3,
and successfully trigger LIN Break detection interrupt, then the test passes, output "pass";
if Break detection fails or interrupt is not triggered, then the test fails, output "fail".

IO Connection Description:
- USART3 TX connects to USART4 RX
- USART3 RX connects to USART4 TX

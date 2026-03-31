/*!
    \file  README.TXT
    \brief USART DMA Continuous Transfer Test Description
    \version 2024-11-27, V2.0.0
*/

Function Description:
This test case verifies USART communication functionality in DMA continuous transfer mode. The test uses DMA continuous mode to perform multiple transfers automatically, verifying data integrity and correctness during the continuous transfer process.

Test Procedure:
1. Configure USART3 transmit DMA and USART4 receive DMA in continuous mode
2. Initialize DMA channels with 10-byte buffer size
3. Start DMA transfer (both TX and RX in continuous mode)
4. Wait for at least 3 complete transfer cycles (interrupt_time_tx/rx < 3)
5. Copy received data from DMA destination buffer
6. Compare transmitted and received data
7. Verify data integrity and pass/fail the test

Test Data:
- Send Data: 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A (10 bytes)
- Data Length: 10 bytes
- Buffer Size: 10 bytes (ARR_SIZE)
- Transfer Cycles: Minimum 3 complete cycles
- Total Data Volume: 30+ bytes (continuous transfers)
- Data Pattern: Sequential test pattern

Configuration Parameters:
- DMA Mode: Continuous transfer mode (PA2M_MODE_CONTINUOUS)
- Transfer Direction: USART3<->USART4
- Data Width: 8 bits (PA2M_MDWIDTH_8BIT)
- Buffer Size: 10 bytes (ARR_SIZE)
- Transfer Cycles: 3+ (monitored by interrupt counters)
- Baud rate: 115200 bps
- Data bits: 8 bits
- Stop bits: 1 bit
- Parity: None
- DMA Interrupt: Full transfer interrupt enabled

DMA Configuration Details:
- TX DMA: Memory-to-Peripheral (USART3_TX)
- RX DMA: Peripheral-to-Memory (USART4_RX)
- Source Increment: Enabled for TX, Disabled for RX
- Destination Increment: Disabled for TX, Enabled for RX
- Continuous Mode: Both channels in continuous mode
- Interrupt: Transfer completion interrupt enabled

Continuous Transfer Characteristics:
- Auto Reload: DMA automatically restarts after each transfer
- Circular Buffer: Continuous transfers use same buffer
- Interrupt Counting: Track transfer cycles via interrupt counters
- No Manual Delays: Transfer timing controlled by DMA hardware
- Hardware Control: No software delays required

Expected Results:
If data transmitted by USART3 completely matches data received by USART4 after at least 3 transfer cycles,
then test passes, output "pass";
otherwise test fails, output "fail".

IO Connection Description:
- USART3 TX_DMA connects to USART4 RX_DMA

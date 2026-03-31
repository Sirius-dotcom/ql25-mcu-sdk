/*!
    \file  README.TXT
    \brief USART 9-bit Data Mode Auto Test Program Description
    \version 2022-5-14, V1.0.1
*/
    Function Description:
    This is a USART 9-bit data mode auto test program, used to verify the functionality of USART in 9-bit data transmission mode.
    The program tests 9-bit data transmission and reception through loopback mode.

    Main Functions:
    1. 9-bit Data Transmission:
       - Configure USART for 9-bit data length mode
       - Send 16-bit data (actually use low 9 bits)
       - Receive and verify the correctness of 9-bit data
    
    2. Loopback Test:
       - Use single USART for loopback test
       - TX and RX pins are connected to form a loop
       - Verify the consistency of transmitted and received data
    
    3. Data Verification:
       - Send predefined test data array
       - Receive data and compare with original data
       - Verify the integrity of 9-bit data transmission

    Test Procedure:
    1. Initialize USART3, configure for 9-bit data mode
    2. Clear transmit and receive FIFO
    3. Send 10 9-bit data sequentially
    4. Receive transmitted data simultaneously
    5. Compare transmitted and received data

    IO Connection Information:
    - USART3 TX connects to USART3 RX (loopback connection)
    - Use single USART for testing

    Test Data:
    - Transmitted data: [0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10a]
    - Data length: 9 bits (actually transmit low 9 bits)
    - Data count: 10

    Configuration Parameters:
    - Baud rate: 115200 bps
    - Data bits: 9 bits
    - Stop bits: 1 bit
    - Parity: None

    Test Results:
    If all transmitted 9-bit data completely matches the received data, the program outputs "pass";
    If any data does not match, the program outputs "fail".
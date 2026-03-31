/*!
    \file  README.TXT
    \brief description of the USART ctest
    \version 2022-5-19, V1.0.0
*/
    Function description:
    This demo show USART1 (Synchronous mode) with QSPI3 transmitter.
    Connect SPI to USART through IO Connection information. First USART_TX sends data and spi_Rx receives data,
    which is stored in rxbuffer2 and SPI for the second USART_TX send data.write one byte in the QSPI3 transmit 
    data register,send a dummy byte to generate clock to slave,wait until end of transmit.wait the byte is entirely 
    received by USART1 ,store the received byte in the rxbuffer1.
    
    Test result:
    This dmo show '{per_inst}_TX send data,SPI_RX receive data,and
    SPI_TX send data,'{per_inst}_RX receive data,if '{per_inst}_TX send 
    data equals SPI_RX receive data,or SPI_TX send data equals '{per_inst}_RX 
    receive data.print "pass",if not, print "fail".

    IO Connection information:
    connect QSPI3 SCK   TO USART1_CK
    connect QSPI3 MISO  TO USART1_RX
    connect QSPI3 MOSI  TO USART1_TX 

/*!
    \file  README.TXT
    \brief description of the USART ctest
    \version 2022-5-19, V1.0.0
*/
    Function description:
    This demo show USART3 (Synchronous mode) with QSPI3 transmitter.
    Connect SPI to USART through IO Connection information. The QSPI TX
    send data to the USART RX, send a dummy byte to generate clock to slave,
    USART3 in slave receive mode,generate a over run interrupt when the
    first clock pluse for data transmission appears while RX FIFO is full already.

    Test result:
    If USART3 generate a over run interrupt when the first clock pluse for
    data transmission appears while RX FIFO is full already.print "pass",
    if not, print "fail".

    IO Connection information:
    connect SPI3 SCK   TO USART3_CK
    connect SPI3 MISO  TO USART3_RX
    connect SPI3 MOSI  TO USART3_TX
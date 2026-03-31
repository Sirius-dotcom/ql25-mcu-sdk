/*!
    \file  README.TXT
    \brief description of the USART ctest
    \version 2022-5-19, V1.0.0
*/
    Function description:
    This demo show USART3 (Synchronous mode) with SPI3 transmitter.
    Connect SPI to USART through IO Connection information. Send a
    dummy byte to generate clock to slave,USART3 in slave transmission
    mode,generate a under run interrupt when the first clock pluse for
    data transmission appears while the software has not yet loaded any
    value into TX FIFO.

    Test result:
    If USART3 generate a under run interrupt when the first clock pluse for
    data transmission appears while the software has not yet loaded any
    value into TX FIFO.print "pass",if not, print "fail".

    IO Connection information:
    connect QSPI3 SCK   TO USART3_CK
    connect QSPI3 MISO  TO USART3_RX
    connect QSPI3 MOSI  TO USART3_TX
Synchronous
============

Function description:
---------------------
    This demo show USARTn (Synchronous mode) with SPI0 transmitter.
    Connect SPI to USARTn through IO Connection information. First USART_TX sends data and spi_Rx receives data,
    which is stored in rxbuffer2 and SPI for the second USART_TX send data.write one byte in the SPI1 transmit 
    data register,send a dummy byte to generate clock to slave,wait until end of transmit.wait the byte is entirely 
    received by USARTn ,store the received byte in the rxbuffer1.

Test result:
------------
    This dmo show USARTn_TX send data,SPI_RX receive data,and
    SPI_TX send data,USARTn_RX receive data,if USARTn_TX send 
    data equals SPI_RX receive data,or SPI_TX send data equals USARTn_RX 
    receive data.print "pass",if not, print "fail".
    
IO connection:
--------------
+-------------------+--------------------+
| PAD               | PAD                |
+===================+====================+
| pad_USARTn_CK     | pad_SPI1_SCK       |
+-------------------+--------------------+
| pad_USARTn_RX     | pad_SPI1_MISO      |
+-------------------+--------------------+
| pad_USARTn_TX     | pad_SPI1_MOSI      |
+-------------------+--------------------+

.. Note::
    The 'm' and 'n' in "usartm" and "usartn" indicate the peripheral number.

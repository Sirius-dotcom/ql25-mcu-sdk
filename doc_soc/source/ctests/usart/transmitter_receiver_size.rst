Transmitter_receiver_size
====================================

Function description:
---------------------
    This demo show USARTn with USARTm in normal mode transmitter.
    Configure USART_RXTX_Size is a value. When the transmitted data reaches this value, 
    the USART_RXTX_Size flag will be generated, and the received and transmitted data will 
    be compared at the same time.

Test result:
------------
    If the data written to USARTn TX is same to data read from USARTm RX,
    and the data written to USARTm TX is same to data read from USARTn RX, 
    print "pass", if not, print "fail".
    
IO connection:
--------------
+-------------------+--------------------+
| PAD               | PAD                |
+===================+====================+
| pad_USARTn_TX     | pad_USART1_RX      |
+-------------------+--------------------+
| pad_USARTn_RX     | pad_USART1_TX      |
+-------------------+--------------------+

.. Note::
    The 'm' and 'n' in "usartm" and "usartn" indicate the peripheral number.
    
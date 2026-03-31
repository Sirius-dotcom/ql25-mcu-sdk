Transmitter_receiver_9bit
====================================

Function description:
---------------------
    Connect two groups of USARTs according to the connection mode,Configure 
    wordlength to 9 bits, send data through USART_TX, USART1_RX 
    receives data and stores it in USART1_buffer.Similarly, USART1_TX 
    sends data, USART_RX receives data and stores it in USART_buffer.

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

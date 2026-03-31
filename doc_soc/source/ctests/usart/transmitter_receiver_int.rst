Transmitter_receiver_int
====================================

Function description:
---------------------
    Set the watermark depth of TX of USARTn and USARTm to 1, that is, the reception interrupt will 
    be generated at the beginning (the interrupt generation condition is that the interrupt will be 
    generated when the data in TX is less than the set depth).If there is data in Rx, a watermark 
    interrupt is generated to save the data.

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

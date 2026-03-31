Receiver_overflow_int
=======================

Function description:
---------------------
    This demo show USARTn with USARTm in overflow interrupt mode transmitter.
    To receive overflow interrupt, you need to configure the interrupt as overflow mode. In addition 
    to sending busy waiting, you can send data continuously to make the received data overflow.

Test result:
------------
    If the USARTm overflow interrupt Handler executs twice(then disable interrupt),
    print "pass", if not, print "fail".
    
IO connection:
--------------
+-------------------+--------------------+
| PAD               | PAD                |
+===================+====================+
| pad_USART0_TX     | pad_USARTm_RX      |
+-------------------+--------------------+
| pad_USART0_RX     | pad_USARTm_TX      |
+-------------------+--------------------+

.. Note::
    The 'm' and 'n' in "usartm" and "usartn" indicate the peripheral number.
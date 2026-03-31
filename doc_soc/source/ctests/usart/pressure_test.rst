Rressure_test
===============

Function description:
---------------------
    This demo show USARTn with USARTm pressure test mode.Configure different 
    modes according to different random numbers, and then send and receive data.

Test result:
------------
    If the data written to USARTn TX is same to data read from USARTm RX,
    and the data written to USARTm TX is same to data read from USARTn RX 
    print "pass", if not, print "fail".
    
IO connection:
--------------
+-------------------+--------------------+
| PAD               | PAD                |
+===================+====================+
| pad_USARTn_TX     | pad_USART1_RX      |
+-------------------+--------------------+

.. Note::
    The 'm' and 'n' in "usartm" and "usartn" indicate the peripheral number.

Receiver_parity_error_int
=============================

Function description:
---------------------
    This demo show USARTn with USARTm in parity error interrupt mode transmitter.
    Configure the usart into two parity check modes, one for odd and one for even. The received 
    USARTn is configured as parity error mode. When USARTn sends data, USARTm receives data, 
    which will parse the error and generate parity interrupt error.

Test result:
------------
    If the USARTm parity error interrupt Handler executs twice(then disable interrupt), 
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
Receiver_wm_int
===================

Function description:
---------------------
    This demo show USARTn with USARTm in watermark timeout interrupt mode transmitter.
    Configure USARTm to receive as watermark timeout interrupt, send data through USARTn, and 
    generate watermark timeout interrupt for receiving data within the specified time.

Test result:
------------
    If the USARTm watermark timeout interrupt Handler executs twice(then disable interrupt), 
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
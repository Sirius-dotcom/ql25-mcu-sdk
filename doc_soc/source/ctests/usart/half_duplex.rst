Half_duplex
=============

Function description:
---------------------
    This demo show USARTn with USARTm in Half-Duplex mode.

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
| pad_USARTn_TX     | pad_USARTm_RX      |
+-------------------+--------------------+

.. NOTE::
    The USARTn can be configured to follow a single-wire half-duplex protocol. In single-wire half-duplex mode, the TX
    and RX pins are connected internally. The selection between half and full-duplex communication is made with a control
    bit HDSEL in USART_SETUP.

.. Note::
    The 'm' and 'n' in "usartm" and "usartn" indicate the peripheral number.

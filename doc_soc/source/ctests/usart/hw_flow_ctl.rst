Hw_flow_ctl
=============

Function description:
---------------------
    This demo show USARTn with USARTm in hardware flow control mode.
    When CTSE=1, the status bit (CTS_RISE_FLAG and CTS_FALL_FLAG) is automatically set by hardware as soon
    as the CTS input toggles. It indicates when the receiver becomes ready or not ready for communication.

Test result:
------------
    If the data written to USARTn TX is same to data read from USARTm RX,
    and the data written to USARTm TX is same to data read from USARTn RX print "pass", if not, 
    print "fail".
    
IO connection:
--------------
+-------------------+--------------------+
| PAD               | PAD                |
+===================+====================+
| pad_USARTn_TX     | pad_USARTm_RX      |
+-------------------+--------------------+
| pad_USARTn_RX     | pad_USARTm_TX      |
+-------------------+--------------------+
| pad_USARTn_CTS    | pad_USARTm_RTS     |
+-------------------+--------------------+
| pad_USARTn_RTS    | pad_USARTm_CTS     |
+-------------------+--------------------+

.. Note::
    The 'm' and 'n' in "usartm" and "usartn" indicate the peripheral number.

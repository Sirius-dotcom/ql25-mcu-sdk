Receiver_idle_int
==================

Function description:
---------------------
    This demo show USARTn with USARTm in idle interrupt mode transmitter.
    To receive an idle interrupt, you need to configure the interrupt to receive an 
    idle mode. When USARTn sends 2 bytes of data, it will generate 2 idle interrupts 
    (USARTn is configured as 8bit mode).Judge whether to pass by judging whether two 
    interrupts have been reached.

Test result:
------------
    If the USARTm idle interrupt Handler executs twice(then disable interrupt), 
    print "pass", if not, print "fail".
    
IO connection:
--------------
+-------------------+--------------------+
| PAD               | PAD                |
+===================+====================+
| pad_USARTn_TX     | pad_USARTm_RX      |
+-------------------+--------------------+
| pad_USARTn_RX     | pad_USARTm_TX      |
+-------------------+--------------------+

.. Note::
    The 'm' and 'n' in "usartm" and "usartn" indicate the peripheral number.

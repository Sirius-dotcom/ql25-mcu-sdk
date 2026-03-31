Autotest
=========

Function description:
---------------------
    This demo is cpu_send_cpu_receive,Use the same group of USARTn for testing,
    and configure and connect the RX and TX of this group.

Test result:
------------
    This demo show the USARTn in 9bit mode auto test.If the data written 
    to USARTn TX is same to data read from USARTn RX, print "pass", 
    if not, print "fail".

IO connection:
--------------
+-----------------+------------------+
| PAD             | PAD              |
+=================+==================+
| pad_USARTn_TX   | pad_USARTn_RX    |
+-----------------+------------------+

.. Note::
    The 'm' and 'n' in "usartm" and "usartn" indicate the peripheral number.

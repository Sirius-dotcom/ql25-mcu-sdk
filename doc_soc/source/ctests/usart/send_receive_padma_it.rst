Send_receive_padma_it
=================================

Function description:
---------------------
    This demo is send_receive_padma_it, test with two groups of USARTn.
    In this demo, test DMA RX_interrupt,the USARTn_TX is configured as DMA mode and the USARTn_RX
    is configured as DMA mode. Through IO connection, the data will be sent through USARTn_TX  and received
    by USART1_RX.When data is received, DMA reception interrupt will be generated.The program will wait for 
    the current receiving interrupt to complete the data comparison.

Test result:
------------
    This dmo show USARTn_TX send data, USART1_RX receive data,
    if USARTn_TX send data equals USART1_RX receive data,
    print "pass",if not, print "fail".
    
IO connection:
--------------
+-------------------+--------------------+
| PAD               | PAD                |
+===================+====================+
| pad_USARTn_TX_DMA | pad_USART1_RX_DMA  |
+-------------------+--------------------+

.. Note::
    The 'm' and 'n' in "usartm" and "usartn" indicate the peripheral number.
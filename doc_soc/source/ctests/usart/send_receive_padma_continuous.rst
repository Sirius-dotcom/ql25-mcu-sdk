Send_receive_padma_continuous
=================================

Function description:
---------------------
    This demo is send_receive_padma_continuous, test with two groups of USARTn.
    In this demo, the USARTn_TX is configured as DMA mode and the USART1_RX
    is configured as DMA mode. Through IO connection, the data will be sent through
    USARTn_TX  and received by USART1_RX.The data will be sent 4 times 
    in a row to turn off DMA_ Continuous interrupt, compare the data after a delay 
    of 1ms, and then continue the above operation to compare the data again.

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

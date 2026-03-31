Send_receive_padma_all
=======================

Function description:
---------------------
    This demo is send_receive_padma_all, test with two groups of USARTn.
    In this demo, the USARTn_TX_RX is configured as DMA mode and the 
    USARTn_TX_RX is configured as DMA mode. Through IO connection, the data will be sent through
    USARTn_TX and received by USART1_RX.And data through USART1_TX and received by USARTn_RX.

Test result:
------------
    This dmo show USARTn_TX send data,USART1_RX receive data,
    if USARTn_TX send data equals' {per1_inst}_RX receive data,or 
    USART1_TX send data equals USARTn_RX receive data.
    print "pass",if not, print "fail".
    
IO connection:
--------------
+-------------------+--------------------+
| PAD               | PAD                |
+===================+====================+
| pad_USARTn_TX_DMA | pad_USART1_RX_DMA  |
+-------------------+--------------------+
| pad_USARTn_RX_DMA | pad_USART1_TX_DMA  |
+-------------------+--------------------+

.. Note::
    The 'm' and 'n' in "usartm" and "usartn" indicate the peripheral number.

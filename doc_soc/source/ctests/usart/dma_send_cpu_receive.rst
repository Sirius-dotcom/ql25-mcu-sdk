Dma_send_cpu_receive
=====================

Function description:
---------------------
    This demo is dma_send_cpu_receive, test with two groups of USARTn
    , configure TX as dma_send, Rx is configured as cpu_receive.

Test result:
------------
    This dmo show  USARTn_TX and USARTm_TX send data, USARTn_RX and 
    USARTm_RX receive data,if USARTn_TX send data equals USARTn_RX
    receive data or USARTm_TX send data equals USARTn_RX receive data, 
    print "pass",if not, print "fail".
    
IO connection:
--------------
+-------------------+--------------------+
| PAD               | PAD                |
+===================+====================+
| pad_USARTn_TX_DMA | pad_USARTm_RX_CPU  |
+-------------------+--------------------+
| pad_USARTn_RX_CPU | pad_USARTm_TX_DMA  |
+-------------------+--------------------+

.. Note::
    The 'm' and 'n' in "usartm" and "usartn" indicate the peripheral number.
    
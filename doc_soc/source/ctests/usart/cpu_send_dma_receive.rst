Cpu_send_dma_receive
======================

Function description:
---------------------
    This demo is CPU_send_dma_Receive, test with two groups of USARTn, configure 
    TX as CPU_Send, Rx is configured as DMA_Receive, the configuration is as follows:

Test result:
------------
    This dmo show USARTn_TX and USARTm_TX send data,USARTn_RX and 
    USARTm_RX receive data,if USARTn_TX send data equals USARTn_RX 
    receive data or USARTm_TX send data equals USARTn_RX receive data, 
    print "pass",if not, print "fail".
    
IO connection:
--------------
+-------------------+--------------------+
| PAD               | PAD                |
+===================+====================+
| pad_USARTn_TX_CPU | pad_USARTm_RX_DMA  |
+-------------------+--------------------+
| pad_USARTn_RX_DMA | pad_USARTm_TX_CPU  |
+-------------------+--------------------+

.. Note::
    The 'm' and 'n' in "usartm" and "usartn" indicate the peripheral number.

M2m_burst
=========

Function description:
---------------------

This demo shows the function of **burst transfer** in memory to memory mode.

The source & destination address is increasing automatically, and dma working in normal mode with burst transfer:

* Initialize & configure the udma.
* Configure & enable burst transformation.
* Start dma data transfer.
* Compare the data in destination address with the data in source address after transformation done.

Main Configuration:
-------------------

Udma m2m configure
^^^^^^^^^^^^^^^^^^
==========  =============  ====================  ====================  =============  =============  =========== 
   Mode      Buffer size     Des address mode      Src address mode      Src width      Des width      Channel  
----------  -------------  --------------------  --------------------  -------------  -------------  -----------
  normal      256 bytes         increasing            increasing          16 bits        16 bits         CH0
==========  =============  ====================  ====================  =============  =============  ===========

.. note::
    The destination burst numbers need to less than the depth of FIFO_DP. 

Test result:
------------

If the data writted to buffer by DMA is same as the source data, then print pass, if not print fail.

IO connection:
--------------

None

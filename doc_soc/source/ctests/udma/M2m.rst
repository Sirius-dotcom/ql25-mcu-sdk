M2m
===

Function description:
---------------------

This demo shows **memory to memory** data transfer working in normal mode.

The source & destination address is increasing automatically:

* Initialize & configure the udma.
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

Test result:
------------

If the data writted to buffer by DMA is same as the source data, then print pass, if not print fail.

IO connection:
--------------

None

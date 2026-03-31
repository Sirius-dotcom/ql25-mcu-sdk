M2m_fixed_addr
==============

Function description:
---------------------

This demo shows **memory to memory** data transfer working in normal mode.

The source & destination address is fixed:

* Enable global interrupt & register **UDMA** interrupt.
* Enable full transfer interrupt.
* Initialize & configure the udma.
* Start dma data transfer.
* Compare the data in destination address with the data in source address after trigger full interrupt.
* Change the fixed address of source & destination to restart to verify the ccorrectness when working in fixed address mode. 

Main Configuration:
-------------------

Udma m2m configure
^^^^^^^^^^^^^^^^^^
==========  =============  ====================  ====================  =============  =============  =========== 
   Mode      Buffer size     Des address mode      Src address mode      Src width      Des width      Channel  
----------  -------------  --------------------  --------------------  -------------  -------------  -----------
  normal       1 bytes            fixed                 fixed            8 bits         8 bits          CH0
==========  =============  ====================  ====================  =============  =============  ===========

Test result:
------------

If the data writted to buffer by DMA is same as the source data, then print pass, if not print fail.

IO connection:
--------------

None

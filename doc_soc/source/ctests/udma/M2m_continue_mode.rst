M2m_continue_mode
=================

Function description:
---------------------

This demo shows **memory to memory** data transfer working in continuous mode.

The source & destination address is increasing automatically:

* Enable global interrupt & register **UDMA** interrupt.
* Enable full & half transfer interrupt.
* Initialize & configure the udma.
* Start dma data transfer.
* Compare the data in destination address with the data in source address after trigger full or half interrupt.
* Repeat 10 times to verify the ccorrectness when working in continuous mode. 

Main Configuration:
-------------------

Udma m2m configure
^^^^^^^^^^^^^^^^^^
==========  =============  ====================  ====================  =============  =============  =========== 
   Mode      Buffer size     Des address mode      Src address mode      Src width      Des width      Channel  
----------  -------------  --------------------  --------------------  -------------  -------------  -----------
continuous     64 bytes         increasing            increasing          32 bits         8 bits         CH0
==========  =============  ====================  ====================  =============  =============  ===========

Test result:
------------

If the data writted to buffer by DMA is same as the source data, then print pass, if not print fail.

IO connection:
--------------

None

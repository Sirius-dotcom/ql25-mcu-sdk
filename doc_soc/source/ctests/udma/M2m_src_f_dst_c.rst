M2m_src_f_dst_c
===============

Function description:
---------------------

This demo shows **memory to memory** data transfer working in normal mode.

The destination address is increasing automatically, but source address is fixed:

* Enable global interrupt & register **UDMA** interrupt.
* Enable full transfer interrupt.
* Initialize & configure the udma.
* Start dma data transfer.
* Compare the data in destination address with the data in source address after trigger full interrupt. 

Main Configuration:
-------------------

Udma m2m configure
^^^^^^^^^^^^^^^^^^
==========  =============  ====================  ====================  =============  =============  =========== 
   Mode      Buffer size     Des address mode      Src address mode      Src width      Des width      Channel  
----------  -------------  --------------------  --------------------  -------------  -------------  -----------
  normal      256 bytes          increasing              fixed            32 bits         8 bits          CH0
==========  =============  ====================  ====================  =============  =============  ===========

Test result:
------------

If the data writted to buffer by DMA is same as the source data, then print pass, if not print fail.

IO connection:
--------------

None

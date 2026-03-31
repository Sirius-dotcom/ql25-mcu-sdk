M2m_repeat_mode_addr_acc_increasing
===================================

Function description:
---------------------

This demo shows **memory to memory** data transfer working in repeat mode.

The source and destination address is increasing automatically:

* Enable global interrupt & register **UDMA** interrupt.
* Enable full transfer interrupt.
* Initialize & configure the udma.
* Set transfer repeat times equal to 4 and address substraction value for next transmission, then start dma data transfer.
* Counter the number of transmission when trigger and enter interrupt service routine.
* Compare the data in destination address with the data in source address after finish every transmission. 

Main Configuration:
-------------------

Udma m2m configure
^^^^^^^^^^^^^^^^^^
==========  =============  ====================  ====================  =============  =============  ===========  ===========  ===========  ===========  ===========
   Mode      Buffer size     Des address mode      Src address mode      Src width      Des width      Channel       saum         rsau          daum        rdau
----------  -------------  --------------------  --------------------  -------------  -------------  -----------  -----------  -----------  -----------  -----------
  repeat       64 bytes         increasing            increasing          32 bits         8 bits         CH0         SUB           0           SUB           0
==========  =============  ====================  ====================  =============  =============  ===========  ===========  ===========  ===========  ===========

Test result:
------------

If the data writted to buffer by DMA is same as the source data, then print pass, if not print fail.

IO connection:
--------------

None

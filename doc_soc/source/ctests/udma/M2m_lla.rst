M2m_lla
=======

Function description:
---------------------

This demo shows **linked-list function** after finished the last transmission in repeat mode.

linked-list configuration changes the source & destination start address:

* Enable global interrupt & register **UDMA** interrupt.
* Enable full transfer interrupt.
* Initialize & configure the udma.
* Configure the linked-list function.
* Set transfer repeat times equal to 10 and address substraction value for next transmission, then start dma data transfer.
* Waiting the last transmission in repeat mode, then compare the data in destination address with the data in source address.
* Waiting trigger the linked-list interrupt, then to compare the data in destination address with the data in source address. 

Main Configuration:
-------------------

Udma m2m configure
^^^^^^^^^^^^^^^^^^
==========  =============  ====================  ====================  =============  =============  ===========  ===========  ===========  ===========  ===========
   Mode      Buffer size     Des address mode      Src address mode      Src width      Des width      Channel       saum         rsau          daum        rdau
----------  -------------  --------------------  --------------------  -------------  -------------  -----------  -----------  -----------  -----------  -----------
  repeat      256 bytes         increasing            increasing          32 bits         8 bits         CH0         SUB           256           SUB         256
==========  =============  ====================  ====================  =============  =============  ===========  ===========  ===========  ===========  ===========

Test result:
------------

If the data writted to buffer by DMA is same as the source data, then print pass, if not print fail.

IO connection:
--------------

None

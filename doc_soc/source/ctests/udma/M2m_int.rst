M2m_int
=======

Function description:
---------------------

This demo shows how to trigger **full & half transfer interrupt**.

The source & destination address is increasing automatically, and dma working in normal mode:

* Enable global interrupt & register **UDMA** interrupt.
* Enable full & half transfer interrupt.
* Initialize & configure the udma.
* Start dma data transfer.
* Compare the data in destination address with the data in source address after triggering the full & half transfer interrupt and entering interrupt service routine.

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

If successfully trigger and enter interrupt service routine, then print pass, if not print fail.

IO connection:
--------------

None

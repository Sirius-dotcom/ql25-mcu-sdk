M2m_repeat_mode_trigger_last_transfer_irq
=========================================

Function description:
---------------------

This demo shows how to trigger **full & half transfer interrupt but enter the interrupt service routine only in last transmission**.

The source & destination address is increasing automatically, and dma working in repeat mode:

* Enable global interrupt & register **UDMA** interrupt.
* Enable full & half transfer interrupt, and enable IRQ for last iteration of data transfer during repeat mode.
* Initialize & configure the udma.
* Start dma data transfer.
* Compare the data in destination address with the data in source address after triggering the full & half transfer interrupt and entering interrupt service routine.

Main Configuration:
-------------------

Udma m2m configure
^^^^^^^^^^^^^^^^^^
==========  =============  ====================  ====================  =============  =============  ===========  ===========  ===========  ===========  ===========
   Mode      Buffer size     Des address mode      Src address mode      Src width      Des width      Channel       saum         rsau          daum        rdau
----------  -------------  --------------------  --------------------  -------------  -------------  -----------  -----------  -----------  -----------  -----------
  repeat       64 bytes         increasing            increasing          32 bits         8 bits         CH0         SUB           64           SUB           0
==========  =============  ====================  ====================  =============  =============  ===========  ===========  ===========  ===========  ===========

Test result:
------------

If successfully trigger and enter interrupt service routine, then print pass, if not print fail.

IO connection:
--------------

None

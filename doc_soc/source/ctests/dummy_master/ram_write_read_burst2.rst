Ram_write_read_burst2
=====================

Function description:
---------------------
This demo is used to test **DUMMY_MASTER** transceiving 2 data in a burst.

Test result:
------------
If the data of **DUMMY_MASTER** received is equal to it transmitted, print pass, if not print fail.

IO connection:
-----------------
None

.. Note::
    The dummy master is a master which only performs idle transfer. It can support the following protocols: 1.axi 2.ahb 3.icb(default)
Spi_autotest
============

Function description:
---------------------
This demo is used to test QSPI transport data in autotest mode.

Test result:
------------
If the data of **QSPIn** received is equal to **QSPIn** transmitted, print pass, if not print fail.

IO connection:
--------------
+-----------------+------------------+
| PAD             |    PAD           |
+=================+==================+
| pad_qspin_dq0   | pad_qspin_dq1    |
+-----------------+------------------+

.. Note::
    The 'n' in "qspin" indicate the peripheral number.
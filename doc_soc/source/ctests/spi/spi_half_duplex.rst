Spi_half_duplex
===============

Function description:
---------------------
This demo is used to test QSPI transport data in halfduplex mode.

Test result:
------------
If the data of **QSPIn** received is equal to **QSPIm** transmitted, print pass, if not print fail.

IO connection:
--------------
+-----------------+------------------+
| PAD             |    PAD           |
+=================+==================+
| pad_qspin_sck   | pad_qspim_sck    |
+-----------------+------------------+
| pad_qspin_cs    | pad_qspim_cs     |
+-----------------+------------------+
| pad_qspin_dq0   | pad_qspim_dq0    |
+-----------------+------------------+

.. Note::
    The 'm' and 'n' in "qspim" and "qspin" indicate the peripheral number.
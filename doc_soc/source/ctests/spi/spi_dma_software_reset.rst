Spi_dma_software_reset
======================

Function description:
---------------------
This demo is used to test QSPI UDMA transport data after software reset.

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
| pad_qspin_dq1   | pad_qspim_dq1    |
+-----------------+------------------+

.. Note::
    The 'm' and 'n' in "qspim" and "qspin" indicate the peripheral number.
Spi_2_line
==========

Function description:
---------------------
This demo is used to test QSPI transport data in spi 2 line mode.

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
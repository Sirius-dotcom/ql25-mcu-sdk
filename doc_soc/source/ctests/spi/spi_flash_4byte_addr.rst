Spi_flash_4byte_addr
====================

Function description:
---------------------
This demo is used to test QSPI erase/write/read flash model in flash 4byte addr mode.

Test result:
------------
If the data of **QSPIn** write to **FLASH** is equal to the data **QSPIn** read back from **FLASH**, print pass, if not print fail.

IO connection:
--------------
+-----------------+------------------+
| PAD             |    PAD           |
+=================+==================+
| pad_qspin_sck   | pad_flsah_sck    |
+-----------------+------------------+
| pad_qspin_cs    | pad_flsah_cs     |
+-----------------+------------------+
| pad_qspin_dq0   | pad_flsah_dq0    |
+-----------------+------------------+
| pad_qspin_dq1   | pad_flsah_dq1    |
+-----------------+------------------+
| pad_qspin_dq2   | pad_flsah_dq2    |
+-----------------+------------------+
| pad_qspin_dq3   | pad_flsah_dq3    |
+-----------------+------------------+

.. Note::
    The 'n' in "qspin" indicate the peripheral number.
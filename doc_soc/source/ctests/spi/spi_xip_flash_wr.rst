spi_xip_flash_wr
================

Function description:
---------------------
This demo is used to test QSPI_XIP erase/write/read flash model in flash xip mode.

Test result:
------------
If the data of **QSPI_XIPn** write to **FLASH** is equal to the data **QSPI_XIPn** read back from **FLASH**, print pass, if not print fail.

IO connection:
--------------
+-------------------+------------------+
| PAD               |    PAD           |
+===================+==================+
| pad_QSPI_XIPn_sck | pad_flsah_sck    |
+-------------------+------------------+
| pad_QSPI_XIPn_cs  | pad_flsah_cs     |
+-------------------+------------------+
| pad_QSPI_XIPn_dq0 | pad_flsah_dq0    |
+-------------------+------------------+
| pad_QSPI_XIPn_dq1 | pad_flsah_dq1    |
+-------------------+------------------+
| pad_QSPI_XIPn_dq2 | pad_flsah_dq2    |
+-------------------+------------------+
| pad_QSPI_XIPn_dq3 | pad_flsah_dq3    |
+-------------------+------------------+

.. Note::
    The 'n' in "QSPI_XIPn" indicate the peripheral number.
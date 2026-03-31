I2C_eeprom_padma
================

Function description:
---------------------
This demo shows the **I2Cn** write to **EEPROM**, and then read from **EEPROM** at the same address in dma mode.
**I2Cn** writes a faw bytes of data to the **EEPROM**, waitting for the standby state of the **EEPROM**, and then reads back 
the data from the same address and compares it.

Test result:
------------
if write to **EEPROM** data is same to read data form **EEPROM**, print pass, if not print fail.

IO connection:
--------------
+-----------------+------------------+
| PAD             |    PAD           |
+=================+==================+
| pad_i2cn_scl    | eeprom_scl       |
+-----------------+------------------+
| pad_i2cn_sda    | eeprom_sda       |
+-----------------+------------------+

.. Note::
    The 'n' in "i2cn" indicate the peripheral number.
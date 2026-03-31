Master_slave_fullduplex_ssm_soft
================================

Function description:
---------------------
This demo is used to test SPI transport data in fullduplex SSM **soft** mode.

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
| pad_qspin_dq0   | pad_qspim_dq0    |
+-----------------+------------------+
| pad_qspin_dq1   | pad_qspim_dq1    |
+-----------------+------------------+

.. Note::
    * Software mode manages **CS** by csi, there is no need to connect cs pad.
    * The 'm' and 'n' in "qspim" and "qspin" indicate the peripheral number.
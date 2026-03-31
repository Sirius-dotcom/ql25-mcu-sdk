Spi_master_slave_padma
======================

Function description:
---------------------
This demo shows master send data and slave receive data use UDMA mode, in addition, data is initialized to sram.

Padma configure
^^^^^^^^^^^^^^^

Master_tx_dma_channel:
::::::::::::::::::::::

=========  =============  ====================  ====================  =================  
  Mode      buffer size     Dst Address Mode      Src Address Mode     Transfer Width    
---------  -------------  --------------------  --------------------  -----------------
  normal      40 bytes       non-increasing          increasing            32 bits  
=========  =============  ====================  ====================  =================

Slave_rx_dma_channel:
:::::::::::::::::::::

=========  =============  ====================  ====================  =================  
  Mode      buffer size     Dst Address Mode      Src Address Mode     Transfer Width    
---------  -------------  --------------------  --------------------  -----------------
  normal      40 bytes        increasing           non-increasing          32 bits  
=========  =============  ====================  ====================  =================

Test result:
------------
If the data of slave received is equal to master transmitted, print pass, if not print fail.

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
/*!
    \file  README.TXT
    \brief description of the SPI ctest
    \version 2021-3-18, v1.0.1
*/

Function description:
    This demo shows master send data and slave receive data use UDMA mode, in addition, data is initialized to sram.

Test result:
    if receive data is equal to send data,printf pass, if not printf fail.

IO Connection information:
    Connect QSPI3 SCK    to QSPI2 SCK .
    Connect QSPI3 CS     to QSPI2 CS  .
    Connect QSPI3 DQ0    to QSPI2 DQ0 .
    Connect QSPI3 DQ1    to QSPI2 DQ1 .

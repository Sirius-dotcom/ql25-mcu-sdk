/*!
    \file  README.TXT
    \brief description of the SPI interrupt ctest
    \version 2021-3-18, v1.0.1
*/

Function description:
    This demo shows master send data andslave receive data use UDMA mode.

Test result:
    if receive data is equal to send data, and the interrupt trigger, printf pass,if not printf fail.

IO Connection information:
    Connect QSPI1 SCK    to QSPI2 SCK .
    Connect QSPI1 CS     to QSPI2 CS  .
    Connect QSPI1 DQ0    to QSPI2 DQ0 .
    Connect QSPI1 DQ1    to QSPI2 DQ1 .

/*!
    \file  README.TXT
    \brief description of the SPI ctest
    \version 2021-3-18, v1.0.1
*/

Function description:
    This demo shows master send data and slave receive data use UDMA continuous mode.

Test result:
        The data will be sent in sequence, once transfer completed fifth time, turning off DMA enable and comparing the data.
If the data of QSPI2 received is equal to QSPI1 transmitted, print pass, if not print fail.

IO Connection information:
    Connect QSPI1 SCK    to QSPI2 SCK .
    Connect QSPI1 CS     to QSPI2 CS  .
    Connect QSPI1 DQ0    to QSPI2 DQ0 .
    Connect QSPI1 DQ1    to QSPI2 DQ1 .

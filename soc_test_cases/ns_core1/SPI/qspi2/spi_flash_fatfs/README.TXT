/*!
    \file  README.TXT
    \brief SPI write-read flash and mount fatfs test
    \version 2021-3-18, v1.0.1
*/

Function description:
    This demo is used to test SPI write/read flash and mount fatfs test.

Test result:
    If the terminal displays the desired print and the file is successfully copied as filename cp.txt,
the execution is considered successful, otherwise it fails.

IO Connection information:
    Connect QSPI2SCK    to flash CLK .
    Connect QSPI2CS     to flash CS  .
    Connect QSPI2DQ0    to flash MOSI .
    Connect QSPI2DQ1    to flash MISO .

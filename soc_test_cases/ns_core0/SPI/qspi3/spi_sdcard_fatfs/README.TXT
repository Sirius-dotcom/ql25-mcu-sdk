/*!
    \file  README.TXT
    \brief SPI write-read sdcard and mount fatfs test
    \version 2021-3-18, v1.0.1
*/

Function description:
    This demo is used to test SPI write/read sdcard and mount fatfs test.

Test result:
    If the terminal displays the desired print and the file is successfully copied as filename cp.txt,
the execution is considered successful, otherwise it fails.

IO Connection information:
    Connect QSPI3SCK    to sdcard CLK .
    Connect QSPI3CS     to sdcard CS  .
    Connect QSPI3DQ0    to sdcard MOSI .
    Connect QSPI3DQ1    to sdcard MISO .

/*!
    \file  README.TXT
    \brief description of the {per_inst} write to and read from EEPROM via dma demo
    \version 2021-3-18, v1.0.1
*/

Function description:
    This demo shows the {per_inst} write to EEPROM, and then read from EEPROM at the same address in dma mode.
{per_inst} writes a page of data(16 bytes) to the EEPROM, waitting for the standby state of the EEPROM, and then reads back 
the data from the same address and compares it.

Test result:
    if write to EEPROM data is same to read data form EEPROM, print pass, if not print fail.

IO Connection information:
    Connect I2C0 SCL    to EEPROM SCL .
    Connect I2C0 SDA    to EEPROM SDA .
    Connect GND         to EEPROM GND .
    Connect VCC         to EEPROM VCC .

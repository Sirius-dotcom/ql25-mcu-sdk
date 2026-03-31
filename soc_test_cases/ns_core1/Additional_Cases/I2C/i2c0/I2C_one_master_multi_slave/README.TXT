/*!
    \file  README.TXT
    \brief description of the {per_inst} write to and read from EEPROM(24LC04B) demo
    \version 2021-3-18, v1.0.1
*/

Function description:
    This demo shows the {per_inst} write to EEPROM, and then read from EEPROM(24LC04B) at the same address in cpu mode.
{per_inst} writes a page of data(16 bytes) to the EEPROM, waitting for the standby state of the EEPROM, and then reads back 
the data from the same address and compares it.
And then {per_inst} master receiving and {per1_inst} slave transmitting in cpu mode.

Test result:
    if write to EEPROM data is same to read data form EEPROM, and the data of **I2Cn** received is equal to **I2Cm** transmitted,
print pass, if not print fail.

IO Connection information:
    Connect I2C0 SCL    to EEPROM SCL .
    Connect I2C0 SDA    to EEPROM SDA .
    Connect I2C0 SCL    to I3 SCL .
    Connect I2C0 SDA    to I3 SDA .
    Connect GND         to EEPROM GND .
    Connect VCC         to EEPROM VCC .

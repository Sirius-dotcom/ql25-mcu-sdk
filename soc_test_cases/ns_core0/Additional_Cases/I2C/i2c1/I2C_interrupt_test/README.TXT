/*!
    \file  README.TXT
    \brief I2C1 master and I3 slave transmitting, receiving and end of transmit interrupt in cpu mode
    \version 2021-3-18, v1.0.1
*/

Function description:
    This demo shows the I2C1 master and I3 slave transmitting, receiving and end of transmit interrupt 
in cpu mode. 

Test result:
    If the data of receiving is equal to transmitting, and related interrupt is triggered, print pass, if not print fail.

IO Connection information:
    Connect I2C1 SCL    to I3 SCL .
    Connect I2C1 SDA    to I3 SDA .

NOTE: 
    eot interrupt only happened in master receiving.
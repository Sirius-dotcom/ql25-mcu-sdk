/*!
    \file  README.TXT
    \brief I2C1 master transmitting and I3 slave receiving in dma mode and trigger EOT interrupt demo
    \version 2021-3-18, v1.0.1
*/

Function description:
    This demo shows the I2C1 master transmitting and I3 slave receiving in dma mode,and that provides 
a description of how to use DMA channel(1 to 4) from RAM to I2C1 tx data buffer, and then DMA Channel(1 to 4) is 
configured to transfer the contents of I3 tx data buffer stored in "source_address" to the reception buffer 
declared in RAM(destination_address).At the same time, setting the tx_size and rx_size of I2C and DMA(Both settings must 
be equal), the eot interrupt will be triggered after the transmission is completed.

Test result:
    If the data of I3 received is equal to I2C1 transmitted, and of EOT interrupts of TX&RX triggered, 
print pass, if not print fail.

IO Connection information:
    Connect I2C1 SCL    to I3 SCL .
    Connect I2C1 SDA    to I3 SDA .

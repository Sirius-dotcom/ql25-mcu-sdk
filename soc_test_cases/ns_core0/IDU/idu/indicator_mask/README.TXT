/*!
    \file  README.TXT
    \brief description of the IDU ctest
    \version 2021-3-18, V1.0.0
*/

    This demo shows the function of irq_distribute_mask in IDU.
    Set the RTC alarm_int irq, every core can answer the irq, the core which handle the irq will modify the mask register value, then other core can not answer the irq, write a value    in a R/W register, wait 1ms, if read back value equal to the write value, it means the other core not answer the irq to modify the value, print pass.

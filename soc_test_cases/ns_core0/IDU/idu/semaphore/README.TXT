/*!
    \file  README.TXT
    \brief description of the IDU ctest
    \version 2021-3-18, V1.0.0
*/

    This demo shows the function of semaphore in IDU.
    write a value which corresponds to the core_id in the semaphore_status register, then other core can not modify the value, write a value in a R/W register, wait 1ms, read back the value, if the value equal to the write value, print pass.

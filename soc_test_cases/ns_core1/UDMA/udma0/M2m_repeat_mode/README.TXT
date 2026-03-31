/*!
    \file  README.TXT
    \brief description of the DMA communication in memory to memory repeat mode
    \version 2021-3-18, V1.0.0
*/
Function description:
    This demo shows the DMA communication in memory to memory repeat mode, source and destination memory are in increasing address mode with data moving.
However, every transmission, the source and destination start address is fixed.
Test result:    
    After the communication is complete, counter the time, if destination buffer is equal to source data, and the repeat times 
equal to the set times, printf pass, if not printf fail.
IO connection information:
    None

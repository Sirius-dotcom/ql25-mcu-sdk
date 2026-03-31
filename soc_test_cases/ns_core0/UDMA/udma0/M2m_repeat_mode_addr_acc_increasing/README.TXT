/*!
    \file  README.TXT
    \brief description of the DMA communication in memory to memory repeat mode
    \version 2024-6-15, V1.0.0
*/
Function description:
    This demo shows the DMA communication in memory to memory repeat mode, source and destination memory are in increasing address mode with data moving.
and, every transmission, the source & destinaton data moving address is also increasing.
Test result:    
    After the communication is complete, counter the time, if destination buffer is equal to source data, and the repeat times 
equal to the set times, printf pass, if not printf fail.
IO connection information:
    None

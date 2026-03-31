/*!
    \file  README.TXT
    \brief description of the DMA full & half transfer interrupt
    \version 2024-6-15, V1.0.0
*/
Function description:
    This demo shows how to trigger full & half transfer interrupt and enter interrupt service function only in last transmission for repeat mode, 
the source & destination address is increasing automatically, and dma working in repeat mode.
Test result:
    If successfully trigger the interrupt and enter interrupt service function only in last transmission, and the destination buffer is equal to source data, printf pass on, if not printf fail.
IO connection information:
    None
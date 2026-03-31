Dma_config
==================

Function description:
---------------------
    This demo shows the cpu config basic timer trigger a interrupt.
    Configure the timer overflow time as 0.2S, and the initial timer to enable update
    interrupt and update DMA interrupt.DMA will fill the TIM_ARR register with data four times.
    Due to the configuration of update interrupt, four interrupts will be generated.

Test result:
------------
    If config success printf pass,if not printf fail.
    
IO connection:
--------------
NONE

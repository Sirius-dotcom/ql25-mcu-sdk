Dma_config
==================

Function description:
---------------------
    This demo shows how to use DMA with ADVANCED_TIMERn update request to transfer data from memory
    to ADVANCED_TIMERn capture compare register 0.
    ADVANCED_TIMERnCLK is fixed to systemcoreclock 8MHz, the ADVANCED_TIMERn prescaler is equal to 8.

    | For instance:
    | ADVANCED_TIMERn counter clock used is 1MHz.
    | The objective is to configure ADVANCED_TIMERn channel 0 to generate PWM signal with a frequency equal to 1KHz and a variable duty cycle(25%,50%,75%) that is changed by the DMA after a specific number of Update DMA request.

Test result:
------------
    | The output signal as the following description:
    | The channel 1 duty cycle is set to 25% / %50 / %75

IO connection:
--------------
NONE

Output pad:
--------------  
+---------------------------+
| PAD                       | 
+===========================+
| pad_ADVANCED_TIMERn_bkin  |
+---------------------------+
| pad_ADVANCED_TIMERn_etr   | 
+---------------------------+
| pad_ADVANCED_TIMERn_ch1_i |
+---------------------------+
| pad_ADVANCED_TIMERn_ch2_i | 
+---------------------------+
| pad_ADVANCED_TIMERn_ch3_i | 
+---------------------------+

.. Note::
    The 'm' and 'n' in "advtimer_timerm" and "advtimer_timern" indicate the peripheral number.

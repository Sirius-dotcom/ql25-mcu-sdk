Ocinactive_int
==================

Function description:
---------------------
    The TIMxCLK frequency is set to systemcoreclock 8MHz, the prescaler is 4000, so the
    ADVANCED_TIMERn counter clock is 2KHz.

    | For instance:
    | And generate 3 signals with 3 different delays:
    | ADVANCED_TIMERn_CH0 delay = 2000/2000 = 1s.
    | ADVANCED_TIMERn_CH1 delay = 4000/2000 = 2s.
    | ADVANCED_TIMERn_CH2 delay = 6000/2000 = 3s.

Test result:
------------
    | The output signal as the following description:
    | When when TIMx_CNT=TIMx_CCR1 irq is comming

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

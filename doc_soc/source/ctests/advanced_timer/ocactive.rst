Ocactive
=========

Function description:
---------------------
    This demo shows how to configure the TIMER peripheral to generate three 
    different signals with three different delays.
    The TIMxCLK frequency is set to systemcoreclock 8MHz, the prescaler is 800
    so the ADVANCED_TIMERn counter clock is 10KHz.

    | For instance:
    | And generate 3 signals with 3 different delays:
    | ADVANCED_TIMERn_CH0 delay = 4000/10K  = 0.4s.
    | ADVANCED_TIMERn_CH1 delay = 8000/10K  = 0.8s.
    | ADVANCED_TIMERn_CH2 delay = 12000/10K = 1.2s.

Test result:
------------
    | The output signal as the following description:
    | When TIMx_CNT=TIMx_CCR1 signal to high

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
    
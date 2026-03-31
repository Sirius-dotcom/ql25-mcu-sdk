Singlepulse
==================

Function description:
---------------------
    This demo shows how to configure the ADVANCED_TIMERn peripheral in single pulse mode.
    The external signal is connected to ADVANCED_TIMERn CH0 pin and the falling edge is
    used as active edge.then The single pulse signal is output on ADVANCED_TIMERn CH1 pin.
    The ADVANCED_TIMERnCLK frequency is set to systemcoreclock 8MHz,the prescaler is 8.

    | For instance:
    | ADVANCED_TIMERn counter clock is 1MHz.
    | Single pulse value = (ADVANCED_TIMERn_period - ADVANCED_TIMERn_pulse) / ADVANCED_TIMERn counter clock = (1500 - 500) / 1MHz = 1 ms.

Test result:
------------
    | The output signal as the following description:
    | The channel 2 output single pulse

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
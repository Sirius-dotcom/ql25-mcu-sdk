Complementarysignals
===========================

Function description:
---------------------
    This demo shows how to configure the ADVANCED_TIMERn peripheral to generate three complementary ADVANCED_TIMERn signals. 
    ADVANCED_TIMERnCLK is fixed to systemcoreclock 8MHz, the ADVANCED_TIMERn prescaler is equal to 400, so the ADVANCED_TIMERn 
    counter clock used is 20KHz and the PWM frequency is 1.25Hz.

Test result:
------------
    | The output signal as the following description:
    | The channel 1 duty cycle is set to 25%.
    | The channel 2 duty cycle is set to 50%.
    | The channel 3 duty cycle is set to 75%.
    
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

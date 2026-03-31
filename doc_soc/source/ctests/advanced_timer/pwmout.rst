Pwmout
=========

Function description:
---------------------
    This demo shows how to configure the TIMER peripheral in PWM (Pulse Width Modulation) mode.
    The TIMxCLK frequency is set to systemcoreclock 8MHz, the prescaler is 8. 

    | For instance:
    | ADVANCED_TIMERn counter clock is 1MHz pwm freq 62.5Hz.
    | ADVANCED_TIMERn Channel1 duty cycle = (4000/ 16000)* 100 = 25%.
    | ADVANCED_TIMERn Channel2 duty cycle = (8000/ 16000)* 100 = 50%.
    | ADVANCED_TIMERn Channel3 duty cycle = (12000/ 16000)* 100 = 75%.

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

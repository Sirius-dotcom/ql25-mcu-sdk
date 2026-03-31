Deadtime_break
==================

Function description:
---------------------
    This demo shows how to configure the ADVANCED_TIMERn peripheral to generate complementary signals, to insert a defined dead time
    value, to use the break feature and to lock the desired parameters.
    ADVANCED_TIMERnCLK is fixed to systemcoreclock 8MHz, the ADVANCED_TIMERn prescaler is equal to 8 ,so
    the ADVANCED_TIMERn counter clock used is 1MHz . the Clock division is ADV_TIMER_TIM_CR1_CKD_CKD4 so
    the dead-time and sampling clock is 4 * 1MHz.

    | For instance:
    | The dead time is computed as the following description:
    | DTG[7:5] = 0xx: DT = DTG[7:0] * tDTS.
    | DTG[7:5] = 10x: DT = (64 + DTG[5:0]) * 2 * tDTS.
    | DTG[7:5] = 110: DT = (32 + DTG[4:0]) * 8 * tDTS.
    | DTG[7:5] = 111: DT = (32 + DTG[4:0]) * 16 * tDTS.

Test result:
------------
    | The output signal as the following description:
    | ADVANCED_TIMERn_BRKIN connect to 3V3 then the break of the wave can be observed through the oscilloscope.
    | The channel 1 freq is 1KHz.
    | The channel 1 duty is set to 30% actual is %17.
    
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

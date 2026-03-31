Six_steps
==========

Function description:
---------------------
    This demo shows how to configure the ADVANCED_TIMERn peripheral to generate three complementary 
    ADVANCED_TIMERn signals(for BLDC) with dead time.ADVANCED_TIMERnCLK is fixed to systemcoreclock 8MHz
    
    | For instance:
    | The ADVANCED_TIMERn prescaler is equal to 8 so the ADVANCED_TIMERn counter clock used is 1MHz and the PWM frequency is 1KHz.

Test result:
------------
step 1
    pad_ADVANCED_TIMERn_bkin:
    pad_ADVANCED_TIMERn_etr:
    pad_ADVANCED_TIMERn_ch1_i: output 1KHz %30 duty PWM signal
    pad_ADVANCED_TIMERn_ch2_i:
    pad_ADVANCED_TIMERn_ch3_i:
step 2
    pad_ADVANCED_TIMERn_bkin:
    pad_ADVANCED_TIMERn_etr:
    pad_ADVANCED_TIMERn_ch1_i:
    pad_ADVANCED_TIMERn_ch2_i: output 1KHz %30 duty PWM signal
    pad_ADVANCED_TIMERn_ch3_i:
step 3
    pad_ADVANCED_TIMERn_bkin:
    pad_ADVANCED_TIMERn_etr:
    pad_ADVANCED_TIMERn_ch1_i:
    pad_ADVANCED_TIMERn_ch2_i: output 1KHz %30 duty PWM signal
    pad_ADVANCED_TIMERn_ch3_i:
step 4
    pad_ADVANCED_TIMERn_bkin:
    pad_ADVANCED_TIMERn_etr:
    pad_ADVANCED_TIMERn_ch1_i:
    pad_ADVANCED_TIMERn_ch2_i:
    pad_ADVANCED_TIMERn_ch3_i: output 1KHz %30 duty PWM signal
step 5
    pad_ADVANCED_TIMERn_bkin:
    pad_ADVANCED_TIMERn_etr:
    pad_ADVANCED_TIMERn_ch1_i:
    pad_ADVANCED_TIMERn_ch2_i:
    pad_ADVANCED_TIMERn_ch3_i: output 1KHz %30 duty PWM signal
step 6
    pad_ADVANCED_TIMERn_bkin:
    pad_ADVANCED_TIMERn_etr:
    pad_ADVANCED_TIMERn_ch1_i: output 1KHz %30 duty PWM signal
    pad_ADVANCED_TIMERn_ch2_i:
    pad_ADVANCED_TIMERn_ch3_i:

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
Exttrigger
==================

Function description:
---------------------
    This demo shows how to start TIMER peripherals with an external trigger.
    ADVANCED_TIMERn is configured as slave timer for an external trigger connected to ADVANCED_TIMERn_CH3 pin:

    | The ADVANCED_TIMERn_CH1 is used as trigger input.
    | Rising edge is used to start the ADVANCED_TIMERn: event mode.
    | The starts of the ADVANCED_TIMERn counter are controlled by the external trigger.

Test result:
------------
    | The output signal as the following description:
    | The channel 2 duty cycle is set to %50.

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

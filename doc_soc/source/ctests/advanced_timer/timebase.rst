Timebase
==========

Function description:
---------------------
    This demo shows how to configure the ADVANCED_TIMER peripheral in timing mode with the corresponding 
    interrupt request for channel 0 in order to generate a time base.
    The TIMxCLK frequency is set to systemcoreclock 8MHz, the prescaler is 400. 

    | For instance: ADVANCED_TIMER counter clock is 20KHz.
    | ADVANCED_TIMER configuration is timing mode, and the timing is 0.2s(4000/20000 = 0.2s), and the CH0 update rate = ADVANCED_TIMER counter clock/CH0CV = 20000/4000 = 5Hz.
    | Enable the channel 0 interrupt, so the ADVANCED_TIMER generates an interrupt each 0.2s, in the handler routine, LED1 is toggled each 0.2s.

Test result:
------------
    When the timer generates an interrupt for 0.2S, the pass state is generated,print "pass", if not, print "fail".

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
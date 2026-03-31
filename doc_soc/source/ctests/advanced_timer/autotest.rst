Autotest
=========

Function description:
---------------------
    This demo shows how to configure the ADVANCED_TIMERn peripheral to auto test.
    ADV_TIMEn_CH1N_O output a single pulse,ADV_TIMEn_CH2 and ADV_TIMEn_CH4 sampling the signals.

Test result:
------------
    This demo captures the channel by reading ADVANCEDTIMER_CH_2 and ADVANCEDTIMER_CH_4, 
    when ADVANCEDTIMER_CH_4 minus ADVANCEDTIMER_CH_2 equal 500 print "pass", if not, print "fail".
    
IO connection:
--------------
+--------------------+---------------------+
| PAD                | PAD                 |
+====================+=====================+
| pad_ADV_TIMEn_CH2  | pad_ADV_TIMEn_CH1N_O|
+--------------------+---------------------+
| pad_ADV_TIMEn_CH4  | pad_ADV_TIMEn_CH1N_O|
+--------------------+---------------------+

.. Note::
    The 'm' and 'n' in "advtimer_timerm" and "advtimer_timern" indicate the peripheral number.

lgpio_evasion_mode
==================

Function description:
---------------------
    * This demo shows LGPIO in interrupt mode.
    * Configure two gpio_pins, LGPO_PIN0 as input and LGPO_PIN1 as output.

Test result:
------------
By default, pin0 of lgpio0 is configured as a high-level model through TB. This will cause pin0 to be configured 
as a rising edge to trigger an interrupt when pin0 is input, which will trigger an interrupt upon power on. This 
problem must be avoided through power on initialization. If output IO as same as input IO, printf pass,if not printf fail.

IO connection:
--------------
+-----------------+------------------+
| PAD             |    PAD           |
+=================+==================+
| pad_lgpio0_pin0 | pad_lgpio0_pin1  |
+-----------------+------------------+

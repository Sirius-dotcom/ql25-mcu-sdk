autotest
========

Function description:
---------------------
    * This demo shows LGPIO auto test.
    * Configure two gpio_pins, LGPO_PIN0 as input and LGPO_PIN1 as output, and connect through IO connection informatio.

Test result:
------------
It is judged by writing data to the output port of LGPIO1 and reading data from the input port of LGPIO0,
if output IO as same as input IO, printf pass,if not printf fail.

IO connection:
--------------
+-----------------+------------------+
| PAD             |    PAD           |
+=================+==================+
| pad_lgpio0_pin0 | pad_lgpio0_pin1  |
+-----------------+------------------+

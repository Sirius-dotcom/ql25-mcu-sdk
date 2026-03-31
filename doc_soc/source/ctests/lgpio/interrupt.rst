interrupt
=========

Function description:
---------------------
    * This demo shows LGPIO in interrupt mode.
    * Configure lgpio as external input interrupt (high level). When the external input is high level, an interrupt will be generated.

Test result:
------------
If there is an external input high level interrupt, printf pass, if not printf fail.

IO connection:
--------------
None

.. Note::
    The high level signal is provided from tb force.

/*!
    \file  README.TXT
    \brief description of the {per_inst} communication with {per1_inst} in normal mode demo
    \version 2021-3-18, v1.0.1
*/

Function description:
    This demo show {per_inst},{per1_inst} send data to bus at the same time, and then {per_inst} will enter into arb_lost status, and resend 
fail frame.
Test result:
    If the sent and received data are equal and enter arb_lost interrupt, print pass, if not print fail.

IO Connection information:
    |----------------|   |-------------------|   |-------------------|   |---------------|
    |             TX <---> TX           CANH <--->CANH            TX <---> TX            |
    |      XKAN1     |   |     XKAN1_phy     |   |     XKAN2_phy     |   |      XKAN2    |
    |             RX <---> RX           CANL <--->CANL            RX <---> RX            |
    |----------------|   |-------------------|   |-------------------|   |---------------|
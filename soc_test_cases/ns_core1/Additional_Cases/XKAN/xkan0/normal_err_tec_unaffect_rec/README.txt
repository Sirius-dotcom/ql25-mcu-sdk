/*!
    \file  README.TXT
    \brief description of the {per_inst} communication with {per1_inst} in normal mode demo
    \version 2021-3-18, v1.0.1
*/

Function description:
    This demo shows the {per_inst} communication with {per1_inst} triggers error under testbench disturb.

Test result:
    tec > 128: {per_inst} enter into passive state;
    tec > 256: {per_inst} enter into bus-off state, and can't tx and rx. 
    after 128*11(bit1), {per_inst} enter into active state, send right frame, print pass, if not print fail.

IO Connection information:
    |----------------|   |-------------------|   |-------------------|   |---------------|
    |             TX <---> TX           CANH <--->CANH            TX <---> TX            |
    |      XKAN1     |   |     XKAN1_phy     |   |     XKAN2_phy     |   |      XKAN2    |
    |             RX <---> RX           CANL <--->CANL            RX <---> RX            |
    |----------------|   |-------------------|   |-------------------|   |---------------|
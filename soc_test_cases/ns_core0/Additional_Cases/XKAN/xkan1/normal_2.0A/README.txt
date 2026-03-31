/*!
    \file  README.TXT
    \brief description of the {per_inst} communication with {per1_inst} in normal mode demo
    \version 2021-3-18, v1.0.1
*/

Function description:
    This demo shows the {per_inst} communication with {per1_inst} through CAN2.0A data frame protocol in normal mode,
and then compares the consistency of the sent and received data.

Test result:
    If the sent and received data are equal, print pass, if not print fail.

IO Connection information:
    |----------------|   |-------------------|   |-------------------|   |---------------|
    |             TX <---> TX           CANH <--->CANH            TX <---> TX            |
    |      XKAN1     |   |     XKAN1_phy     |   |     XKAN2_phy     |   |      XKAN2    |
    |             RX <---> RX           CANL <--->CANL            RX <---> RX            |
    |----------------|   |-------------------|   |-------------------|   |---------------|
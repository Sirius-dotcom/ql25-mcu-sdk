WWDG0 NMI Interrupt Test

Function Description:
    Tests WWDG0 NMI interrupt generation and handling.

Test Process:
    1. CPU0: Enable WWDG0 clock and reset
    2. Configure NMI selection registers 
    3. Enable WWDG interrupt
    4. Configure WWDG parameters
    5. Register NMI_Handler
    6. Wait for watchdog timeout
    7. NMI interrupt triggered

Expected Result:
    - NMI_Handler executed: Test passes
    - NMI interrupt generated successfully
    - Prints "pass" in NMI handler

WWDG1 Delay Feed Reset Test

Function Description:
    Tests WWDG reset when not fed within the specified window time.

Test Process:
    1. Enable WWDG1 clock and reset
    2. Check and clear reset status
    3. Verify WWDG reset flag
    4. Configure WWDG with counter and window values
    5. Wait without feeding the watchdog
    6. Watchdog timeout triggers reset

Expected Result:
    - WWDG reset detected: Test passes
    - Reset flag cleared successfully
    - System resets due to watchdog timeout

WWDG0 Early Feed Reset Test

Function Description:
    Tests WWDG reset when fed too early (before the window opens).

Test Process:
    1. Enable WWDG0 clock and reset
    2. Check and clear reset status
    3. Verify WWDG reset flag 
    4. Configure WWDG with large counter value
    5. Feed the watchdog too early (before window opens)
    6. Early feed triggers reset

Expected Result:
    - WWDG reset detected: Test passes
    - Reset flag cleared successfully
    - System resets due to early feed violation

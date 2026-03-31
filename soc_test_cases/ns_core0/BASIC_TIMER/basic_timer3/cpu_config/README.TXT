/*!
    \file  README.TXT
    \brief Basic Timer CPU Configuration Test

    \version 2024-12-02, V2.0.0
*/
    Function Description:
    This example demonstrates how to configure BASIC_TIMER3 to generate CPU interrupt based on timer overflow.
    The timer is configured for precise timing with interrupt-driven test validation.
    
    Timer Configuration:
    - Timer Mode: Basic timing mode
    - Period: 200 counts
    - Counter Direction: Up-counting only

    Interrupt Configuration:
    - Interrupt Source: Update interrupt (UIE)

    Test Logic:
    - Timer starts and runs until overflow occurs
    - Overflow triggers update interrupt
    - Interrupt handler sets flag and disables timer/interrupt
    - Main function checks flag after 5ms delay
    - Test passes if interrupt occurred within timeout

    Test Procedure:
    1. Configure timer for 200 count period
    2. Enable update interrupt
    3. Start timer operation
    4. Wait for timer overflow and interrupt generation
    5. Interrupt handler sets completion flag
    6. Main function validates interrupt occurrence

    Expected Results:
    - Timer runs for exactly 200 counts
    - Update interrupt occurs at counter overflow
    - Flag is set by interrupt handler
    - Timer and interrupt are disabled after first overflow
    - Test passes when interrupt is detected within timing window

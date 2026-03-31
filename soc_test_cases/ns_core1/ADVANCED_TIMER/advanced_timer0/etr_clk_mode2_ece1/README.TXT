/*!
    \file  README.TXT
    \brief Advanced Timer ETR External Clock Mode 2 with ECE Test

    \version 2025-12-03, V2.0.0
*/
    Function Description:
    This example demonstrates how to configure ADVANCED_TIMER0 in external clock mode 2 with external clock enable (ECE).
    The timer uses ETR input as external clock source with specific configuration for mode 2 operation.
    
    Timer Configuration:
    - Timer Clock: ETR external clock signal
    - External Trigger: ETR configured for external clock mode 2
    - Trigger Source: ETRF (filtered ETR signal)
    - Clock Source: External Clock Enable (ECE) bit set
    - PWM Period: 16000 counts
    - Clock Division: CKD1 (no division)

    ETR Configuration:
    - External Trigger Polarity: Non-inverted (ETP_DISABLE)
    - External Trigger Prescaler: No prescaling (ETPS_PRESCALER)
    - External Trigger Filter: No filtering (ETF_NO_FILTER)
    - External Clock Enable: ECE active for mode 2
    - Input Source: ETR pin (ADV_ETR)

    PWM Output Configuration:
    - Channel 1: PWM mode 1, 25% duty cycle (4000/16000)
    - Channel 2: PWM mode 1, 50% duty cycle (8000/16000)
    - Complementary outputs: Disabled
    - Shadow registers: Disabled (immediate update)
    - Primary output: Enabled

    Clock Mode Details:
    - Mode 2: External clock mode 2 (different from mode 1)
    - ECE Control: External clock enable override
    - ETR Filtering: Minimal filtering for fast response
    - Prescaling: None for maximum resolution
    - Polarity: Standard non-inverted operation

    IO Connection Information:
    - ADVANCED_TIMER0_TIM_ETR: External clock input
    - ADVANCED_TIMER0_TIM_CH1: PWM output (25% duty cycle)
    - ADVANCED_TIMER0_TIM_CH2: PWM output (50% duty cycle)

    Test Procedure:
    1. Configure ETR for external clock mode 2 operation
    2. Set ECE bit to enable external clock functionality
    3. Configure PWM channels with desired duty cycles
    4. Apply external clock signal to ETR pin
    5. Monitor PWM outputs 
    6. Verify proper mode 2 operation characteristics

    Expected Results:
    - Timer operates in external clock mode 2 configuration
    - Correct duty cycle generation with external clock source

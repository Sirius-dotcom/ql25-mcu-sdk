/*!
\file  readmecn.txt
\Brief Advanced Timer Deadband and Brake Function Test Instructions (Chinese Version)

\version 2019-6-5, V1.0.0
*/
Function description:
This example demonstrates how to configure the ADVANCED_TIMER2 peripheral to generate complementary signals with dead zone insertion, braking function and parameter locking.
    
Timer configuration (simulation environment):
-SystemCoreClock: 8MHz (default value of simulation environment)
-Timer clock: SystemCoreClock/8=1MHz (simulation environment)
-Timer cycle: 1000 counts
-Clock division: CKD4 (deadband and sampling clock=1/4MHz)
-CH1 duty cycle: 50% (500/1000), CH1N complementary: 50%
-Output mode: PWM mode 1
    
Deadband configuration:
-Deadband value: 0x40 (0b01000000)
- DTG[7:5] = 010£¬ Belongs to "0xx" category
-Formula: DT=DTG [7:0] * tDTS
- tDTS = 1/(CKDTS) = 1/(4MHz) = 0.25us
-Deadband time=0x40 * 0.25 us=64 *0.25 us=16 us
    
Brake configuration:
-Brake polarity: active at low level
-Automatic output enabling: software automatic
-Locking level: OFF (no locking)
-Braking status: disabled by default
    
IO connection information:
-ADVANCED_TIMER2_TIM_CH1: PWM output (50% duty cycle)
-ADVANCED_TIMER2_TIM_CH1N: Complementary output with dead zone
    
Test steps:
1. Monitor CH1 and CH1N output
2. Observe the dead time between complementary signals
# Skill: utb-interrupt-exception

## Purpose
Guide ECLIC interrupt configuration, ISR design patterns, exception handling, and cross-core interrupt usage for QL25 AMP.

## When to use
- adding or modifying an ISR
- configuring ECLIC priorities or levels
- IDU doorbell or cross-core interrupt work
- trap/exception handling changes
- debugging spurious interrupts or missed wakeups

## ECLIC architecture basics
- each core has its own ECLIC instance
- priority/level split depends on `cliccfg`
- vector and non-vector modes are both possible
- `mtvt` or `mtvec` determines interrupt entry mode

Key concepts:
- clear pending correctly
- keep ISR minimal
- yield correctly after `xxxFromISR()`

## FreeRTOS integration rules
- only ISRs within the allowed priority band may call `xxxFromISR()`
- ISRs above that ceiling must never call FreeRTOS APIs
- master and slave configure ECLIC independently

## Preferred ISR patterns

### ISR -> task notification
Use for 1:1 wakeup such as MAC RX, DMA completion, or IDU doorbell.

### ISR -> queue
Use only when payload data must cross the ISR boundary.

### ISR flag + polling
Use only in baremetal or pre-scheduler cases.

## IDU doorbell rules
- IDU is a signal path, not a payload path
- ISR must only acknowledge, notify the task, and exit
- ring draining or IPC parsing belongs in task context

## Exception handling
Common causes to check:
- illegal instruction
- load/store access fault
- misaligned access
- instruction access fault

For trap work, inspect `mcause`, `mepc`, and `mtval`.

## ISR registration checklist
1. confirm owner core
2. choose FreeRTOS-safe priority if needed
3. choose correct trigger type
4. register handler correctly
5. clear pending source
6. keep ISR body minimal
7. handle `portYIELD_FROM_ISR`
8. verify no spurious re-entry

## Do / Don't
- Do use task notifications for simple ISR->task wakeup.
- Do configure ECLIC per core.
- Do keep ISR logic minimal.
- Don't block in ISR.
- Don't parse IPC messages or packets in ISR.
- Don't assume cross-core task wakeup works directly.
- Don't hardcode priority assumptions without checking `cliccfg`.

## Related skills
- `utb-freertos-osal`
- `utb-core-ownership`
- `utb-ipc-and-shared-memory`
- `utb-hal-driver-ownership`
- `utb-debug-workflow`

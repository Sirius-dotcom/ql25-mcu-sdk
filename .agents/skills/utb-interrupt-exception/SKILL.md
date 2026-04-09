# Skill: utb-interrupt-exception

## Purpose
Guide ECLIC interrupt configuration, ISR design patterns, exception handling, and cross-core interrupt (IDU doorbell) usage for QL25 AMP.

## When to use
- adding or modifying an ISR
- configuring ECLIC priorities or levels
- registering interrupt handlers
- IDU doorbell / cross-core interrupt work
- exception handler or trap handler changes
- debugging spurious interrupts, missed wakeups, or priority inversion
- reviewing ISR→task handoff correctness

## ECLIC architecture basics

QL25 uses Nuclei ECLIC (Enhanced Core Local Interrupt Controller):
- each core has its own independent ECLIC instance
- supports configurable priority bits and level bits
- interrupt nesting is controlled by level/priority configuration
- vector mode and non-vector mode available
- vector table base set by `mtvt` CSR (vector mode) or `mtvec` (non-vector mode)

Key registers per interrupt source:
- `clicintip[i]` — interrupt pending
- `clicintie[i]` — interrupt enable
- `clicintattr[i]` — trigger type (level/edge/positive/negative), vector/non-vector
- `clicintctl[i]` — priority and level encoding

Global:
- `cliccfg` — defines how `clicintctl` bits split into level vs priority
- `mth` (or `mintthresh`) — current interrupt threshold

## Priority and level configuration

### FreeRTOS integration
```c
configKERNEL_INTERRUPT_PRIORITY         /* lowest: kernel tick, PendSV equivalent */
configMAX_SYSCALL_INTERRUPT_PRIORITY    /* ceiling for FreeRTOS-safe ISRs */
```

Rules:
- ISRs that call `xxxFromISR()` FreeRTOS APIs must have effective priority <= `configMAX_SYSCALL_INTERRUPT_PRIORITY`
- ISRs above this ceiling must NEVER call any FreeRTOS API (they preempt the kernel)
- the kernel tick interrupt runs at `configKERNEL_INTERRUPT_PRIORITY`

### Priority assignment strategy
| Priority band | Usage | FreeRTOS API allowed? |
|---|---|---|
| 0 (lowest) | kernel tick, idle work | yes (kernel internal) |
| 1–6 | peripheral ISRs: UART, SPI, MAC, DMA, IDU doorbell | yes |
| 7+ (above MAX_SYSCALL) | ultra-low-latency ISRs (if any) | NO |

Adjust actual numbers based on `cliccfg` bit-width configuration. The current slave config uses `configMAX_SYSCALL_INTERRUPT_PRIORITY=7` conservatively.

### Per-core independence
- master and slave configure their own ECLIC independently
- interrupt numbers may overlap between cores (same IRQn, different ECLIC instance)
- do not assume one core's ECLIC configuration affects the other

## ISR design patterns

### Pattern 1: ISR → Task notification (preferred for 1:1 wakeup)
```c
/* ISR */
void XXX_IRQHandler(void)
{
    hw_clear_interrupt_pending();    /* acknowledge HW source first */

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(s_target_task, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/* Task */
void task_xxx(void *arg)
{
    for (;;) {
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(timeout_ms));
        /* process work */
    }
}
```
Used for: IDU doorbell, MAC Rx, DMA completion.

### Pattern 2: ISR → Queue (when payload must cross boundary)
```c
void XXX_IRQHandler(void)
{
    hw_clear_interrupt_pending();

    event_t evt = { .type = XXX, .data = read_hw_register() };
    BaseType_t xWoken = pdFALSE;
    xQueueSendFromISR(s_event_queue, &evt, &xWoken);
    portYIELD_FROM_ISR(xWoken);
}
```
Used for: events with associated data that must be captured at ISR time.
Note: queue-based ISR requires `configSUPPORT_DYNAMIC_ALLOCATION=1` or static queue creation on slave.

### Pattern 3: ISR flag + polling (for non-RTOS or bare minimum)
```c
volatile uint32_t g_isr_flag = 0;

void XXX_IRQHandler(void)
{
    hw_clear_interrupt_pending();
    g_isr_flag = 1;
}
/* Task polls g_isr_flag periodically — only use when notification is unavailable */
```
Avoid this pattern when FreeRTOS is running. Use only in baremetal or pre-scheduler context.

## IDU doorbell (cross-core interrupt)

The IDU (Interrupt Distribution Unit) provides cross-core signaling:
- master can trigger interrupt on slave, slave can trigger on master
- mapped to `INTER_CORE_IRQn` on each core
- used as IPC doorbell: "new message in ring, come check"

Registration:
```c
ECLIC_SetLevelIRQ(INTER_CORE_IRQn, level);
ECLIC_SetPriorityIRQ(INTER_CORE_IRQn, priority);
ECLIC_EnableIRQ(INTER_CORE_IRQn);
```

ISR must:
1. clear IDU pending (hardware acknowledge)
2. notify the IPC service task
3. yield if higher-priority task woken

Do NOT process IPC messages inside the doorbell ISR. The ISR only wakes the task; the task drains the ring.

## Exception handling

### Trap handler
- `mtvec` points to the trap entry
- trap handler distinguishes interrupt vs exception via `mcause` MSB
- exception causes: illegal instruction, load/store fault, ecall, breakpoint, misaligned access

### Recommended exception handler structure
```c
void trap_handler(void)
{
    uint32_t mcause = read_csr(mcause);
    uint32_t mepc   = read_csr(mepc);
    uint32_t mtval  = read_csr(mtval);

    if (mcause & 0x80000000) {
        /* interrupt — should not reach here if vector mode handles it */
    } else {
        /* exception */
        uint32_t cause = mcause & 0x1F;
        /* log cause, mepc, mtval */
        /* enter fatal loop or trigger recovery */
    }
}
```

### Common exception causes in this project
| Cause | Typical root cause |
|---|---|
| Illegal instruction | wrong image loaded, ILM corruption, missing `fence.i` after load |
| Load access fault | accessing unmapped region, wrong core accessing other core's local memory |
| Store access fault | writing to read-only region, PMP violation |
| Misaligned access | packed struct without proper handling, unaligned DMA buffer |
| Instruction access fault | PC jumped to unmapped region (stack overflow corrupted return address) |

## ISR registration checklist

When adding a new ISR:
1. **Identify owner core** — which core owns this peripheral?
2. **Choose priority** — within FreeRTOS-safe band if calling `xxxFromISR`
3. **Choose trigger type** — level vs edge, based on hardware spec
4. **Register handler** — via ECLIC vector table or `ECLIC_SetVector()`
5. **Clear pending** — acknowledge HW source before or at ISR entry
6. **Minimize ISR body** — defer work to task
7. **Handle yield** — `portYIELD_FROM_ISR()` if task notification used
8. **Test** — verify ISR fires, task wakes, no spurious re-entry

## What is easy to misunderstand
- ECLIC priority bits vs level bits depend on `cliccfg` — do not hardcode assumptions about how many bits are priority vs level.
- `configMAX_SYSCALL_INTERRUPT_PRIORITY=7` is a conservative placeholder; must be validated against actual ECLIC bit-width configuration.
- "vector mode" means each interrupt has its own entry point; "non-vector mode" means all interrupts funnel through one handler that reads `mcause`.
- IDU doorbell is NOT a mailbox — it carries no data, only a signal. Data travels through shared memory rings.
- An ISR on core 0 cannot directly wake a FreeRTOS task on core 1 (AMP, not SMP). Cross-core wakeup goes through IDU + local ISR + local task notification.

## What to inspect in code
Search:
- `ECLIC_SetLevelIRQ`
- `ECLIC_SetPriorityIRQ`
- `ECLIC_EnableIRQ`
- `ECLIC_SetVector`
- `INTER_CORE_IRQn`
- `IRQHandler`
- `FromISR`
- `portYIELD_FROM_ISR`
- `mtvec`
- `mtvt`
- `mcause`
- `configMAX_SYSCALL_INTERRUPT_PRIORITY`
- `configKERNEL_INTERRUPT_PRIORITY`
- `cliccfg`

Inspect:
- `SoC/ns_core*/Common/Include/ns_core*.h` — IRQn enum definitions
- `SoC/ns_core*/Common/Include/ns_idu.h` — IDU register definitions
- `NMSIS/Core/Include/core_feature_eclic.h` — ECLIC API
- `OS/FreeRTOS/Source/portable/GCC/RISC-V/portmacro.h` — port macros
- `application/freertos/ql25_amp_*/FreeRTOSConfig.h` — priority config

## Do / Don't
- Do keep ISR body minimal: acknowledge, notify, yield.
- Do verify FreeRTOS API calls are only in ISRs within the safe priority band.
- Do use task notifications for 1:1 ISR→task wakeup (lightest mechanism).
- Do configure ECLIC per-core independently.
- Do clear interrupt pending before or at ISR entry to avoid re-entry.
- Don't call blocking FreeRTOS APIs (`xQueueSend`, `xSemaphoreTake`, `vTaskDelay`) inside ISR.
- Don't process IPC messages or packets inside ISR — defer to task.
- Don't assume cross-core task notification works directly (use IDU + local ISR).
- Don't hardcode ECLIC priority numbers without checking `cliccfg` bit allocation.
- Don't use `printf` or heavy logging inside ISR.

## Related skills
- `utb-freertos-osal`
- `utb-core-ownership`
- `utb-ipc-and-shared-memory`
- `utb-hal-driver-ownership`
- `utb-debug-workflow`

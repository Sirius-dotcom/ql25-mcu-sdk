# Skill: utb-driver-implementation

## Purpose
Provide a concrete step-by-step template for implementing a new HAL driver wrapper in this project, from requirement to integration.

## When to use
- adding a new peripheral driver (MAC, SPI, UART, Timer, Flash, DMA)
- wrapping an existing vendor SDK driver with project policy
- porting a driver from baremetal to RTOS-aware usage
- adding DMA support to an existing driver
- reviewing a driver implementation for completeness

## Architecture recap

This repo follows a **wrapper-and-policy** model:

```
Application task
    ↓ calls
platform/hal/include/hal_xxx.h      ← project API contract
    ↓ implements
platform/hal/src/ql25_xxx.c         ← project wrapper + policy
    ↓ calls
SoC/ns_core*/Common/Source/Drivers/ ← vendor SDK driver
    ↓ accesses
Hardware registers
```

The wrapper layer adds:
- ownership enforcement (which core may call)
- ISR context safety
- DMA lifecycle management
- timeout and error policy
- RTOS integration (ISR→task notification)

The vendor SDK driver provides:
- register access primitives
- basic init/deinit
- transfer start/stop
- status polling

## Step-by-step: Adding a new driver

### Step 1: Define ownership

Before writing code, answer:
- which core owns this peripheral? (check `utb-core-ownership`)
- is the peripheral exclusive to one core or shared?
- if shared: what is the arbitration protocol?
- is DMA involved? which DMA channel? who owns it?

Document the answer in the header file comment.

### Step 2: Create header file

File: `platform/hal/include/hal_xxx.h`

Template:
```c
#ifndef HAL_XXX_H
#define HAL_XXX_H

#include <stdint.h>

/**
 * @brief XXX HAL driver
 *
 * Owner: core N (master/slave)
 * Context: task only (or ISR-safe if applicable)
 * DMA: yes/no, channel N, owner: core N
 */

/* Error codes */
#define HAL_XXX_OK           0
#define HAL_XXX_ERR_BUSY    (-1)
#define HAL_XXX_ERR_TIMEOUT (-2)
#define HAL_XXX_ERR_PARAM   (-3)
#define HAL_XXX_ERR_DMA     (-4)

/* Configuration */
typedef struct {
    uint32_t    field1;
    uint32_t    field2;
    /* keep struct small, fixed-size, no pointers to dynamic memory */
} hal_xxx_cfg_t;

/* Status / statistics */
typedef struct {
    uint32_t    tx_count;
    uint32_t    rx_count;
    uint32_t    err_count;
    uint32_t    timeout_count;
} hal_xxx_stats_t;

/**
 * Initialize the XXX peripheral.
 * Must be called from task context after scheduler start (or from main before scheduler).
 * @return HAL_XXX_OK on success, negative error code on failure.
 */
int hal_xxx_init(const hal_xxx_cfg_t *cfg);

/**
 * Deinitialize. Disables interrupt, stops DMA, releases hardware.
 */
void hal_xxx_deinit(void);

/* ... operation APIs ... */

/**
 * Get accumulated statistics (read-only snapshot).
 */
const hal_xxx_stats_t *hal_xxx_stats_get(void);

#endif /* HAL_XXX_H */
```

Design rules:
- error codes are negative integers, success is 0
- config struct is plain C, fixed-size, no dynamic members
- stats struct for observability
- document owner, context, DMA in the file header

### Step 3: Create implementation file

File: `platform/hal/src/ql25_xxx.c`

Structure:
```c
#include "hal_xxx.h"
#include "nuclei_sdk_soc.h"     /* vendor SDK + SoC headers */

/* ---- Private state (file scope, static) ---- */

static volatile uint32_t s_xxx_initialized = 0;
static hal_xxx_stats_t   s_xxx_stats;

/* ISR→task notification handle (only if RTOS-aware) */
#if defined(RTOS_FREERTOS)
#include "FreeRTOS.h"
#include "task.h"
static TaskHandle_t s_xxx_notify_task = NULL;
#endif

/* ---- ISR ---- */

void XXX_IRQHandler(void)
{
    /* 1. Acknowledge HW interrupt source */
    XXX_ClearPendingIRQ();

    /* 2. Minimal work: read status, update counters */
    s_xxx_stats.rx_count++;

    /* 3. Notify task */
#if defined(RTOS_FREERTOS)
    if (s_xxx_notify_task) {
        BaseType_t xWoken = pdFALSE;
        vTaskNotifyGiveFromISR(s_xxx_notify_task, &xWoken);
        portYIELD_FROM_ISR(xWoken);
    }
#endif
}

/* ---- Public API ---- */

int hal_xxx_init(const hal_xxx_cfg_t *cfg)
{
    if (!cfg) return HAL_XXX_ERR_PARAM;
    if (s_xxx_initialized) return HAL_XXX_ERR_BUSY;

    /* 1. Reset peripheral */
    /* 2. Apply configuration from cfg */
    /* 3. Configure ECLIC: priority, level, enable */
    ECLIC_SetLevelIRQ(XXX_IRQn, xxx_level);
    ECLIC_SetPriorityIRQ(XXX_IRQn, xxx_priority);
    ECLIC_EnableIRQ(XXX_IRQn);

    /* 4. Clear stats */
    memset(&s_xxx_stats, 0, sizeof(s_xxx_stats));

    /* 5. Mark initialized */
    s_xxx_initialized = 1;

    return HAL_XXX_OK;
}

void hal_xxx_deinit(void)
{
    ECLIC_DisableIRQ(XXX_IRQn);
    /* stop DMA if active */
    /* reset peripheral */
    s_xxx_initialized = 0;
    s_xxx_notify_task = NULL;
}

const hal_xxx_stats_t *hal_xxx_stats_get(void)
{
    return &s_xxx_stats;
}
```

### Step 4: Add to build system

In `platform/hal/build.mk`:
```makefile
# 条件编译：仅在所属核编译
ifeq ($(SOC),ns_core0)
C_SRCS += $(PLATFORM_HAL_DIR)src/ql25_xxx.c    # if slave-owned
endif
```

Or if both cores need the driver (rare):
```makefile
C_SRCS += $(PLATFORM_HAL_DIR)src/ql25_xxx.c
```

### Step 5: Integrate with application

In the owning core's `main.c`:
```c
/* Init */
hal_xxx_cfg_t cfg = { .field1 = ..., .field2 = ... };
int ret = hal_xxx_init(&cfg);
if (ret != HAL_XXX_OK) {
    /* handle init failure */
}

/* If ISR→task notification needed: */
hal_xxx_set_notify_task(xTaskGetCurrentTaskHandle());
```

### Step 6: Validate (see utb-testing-validation)

## DMA-enabled driver additions

When the driver uses DMA, add these to the template:

### In header:
```c
/* DMA buffer requirements */
#define HAL_XXX_DMA_BUF_ALIGN   4       /* alignment requirement */
#define HAL_XXX_DMA_BUF_MAX     1024    /* max single transfer size */
```

### In implementation:
```c
/* DMA buffer ownership protocol:
 *
 * TX path:
 *   1. caller fills buffer
 *   2. caller calls hal_xxx_tx_submit(buf, len)  → ownership transfers to DMA
 *   3. DMA completion ISR fires                  → ownership returns to driver
 *   4. driver notifies task                      → ownership returns to caller
 *
 * RX path:
 *   1. driver submits empty buffer to DMA        → ownership at DMA
 *   2. DMA completion ISR fires                  → ownership returns to driver
 *   3. driver notifies task with buffer pointer   → ownership transfers to caller
 *   4. caller processes and returns buffer        → ownership returns to driver
 */
```

Mandatory checks:
- buffer alignment verified at submit time
- buffer size within DMA transfer limit
- no caller access to buffer while DMA owns it
- DMA abort/timeout path returns buffer ownership to driver
- completion ISR does not assume buffer content is immediately visible (add fence if needed)

## Polling driver variant (no ISR)

For simple peripherals or baremetal:
```c
int hal_xxx_poll_ready(uint32_t timeout_cycles)
{
    uint32_t start = SysTimer_GetLoadValue();
    while (!(XXX_REG->STATUS & READY_BIT)) {
        if ((SysTimer_GetLoadValue() - start) > timeout_cycles) {
            s_xxx_stats.timeout_count++;
            return HAL_XXX_ERR_TIMEOUT;
        }
    }
    return HAL_XXX_OK;
}
```

Rules:
- always bounded timeout
- count timeouts for diagnostics
- never spin forever

## Common mistakes to avoid

| Mistake | Correct approach |
|---|---|
| Calling vendor SDK init directly from app | Wrap in `hal_xxx_init()` with policy |
| Sharing peripheral across cores without protocol | Define owner in header, enforce at wrapper |
| Large local buffer in ISR | Use file-scope static buffer or ring |
| DMA submit without checking alignment | Validate at submit, return error |
| Missing ECLIC disable in deinit | Always disable IRQ + stop DMA in deinit |
| No error return on timeout | All waits must be bounded with error return |
| Using `printf` in ISR for debug | Use counter + post-mortem inspection |
| Forgetting `portYIELD_FROM_ISR` | Always check and yield after `FromISR` API |

## File naming convention

| File | Location |
|---|---|
| `hal_xxx.h` | `platform/hal/include/` |
| `ql25_xxx.c` | `platform/hal/src/` |
| Build integration | `platform/hal/build.mk` |
| Application usage | `application/freertos/ql25_amp_{master,slave}/main.c` |

## What to inspect in code
Search:
- `platform/hal/include`
- `platform/hal/src`
- `SoC/ns_core*/Common/Source/Drivers`
- `build.mk`
- `C_SRCS`
- `hal_`
- `_IRQHandler`
- `ECLIC_`

## Do / Don't
- Do follow the wrapper-and-policy model — never bypass to raw registers from app code.
- Do define ownership, context, and DMA protocol in the header comment.
- Do use negative error codes consistently.
- Do add stats counters for observability.
- Do add conditional compilation in `build.mk` for core-specific drivers.
- Do validate DMA buffer alignment and size at submit time.
- Don't add dynamic allocation inside driver code.
- Don't add heavyweight abstractions (vtables, callback chains, plugin systems).
- Don't add RTOS dependencies unconditionally — guard with `#if defined(RTOS_FREERTOS)`.
- Don't access vendor driver internals from application code when a wrapper exists.

## Related skills
- `utb-hal-driver-ownership`
- `utb-core-ownership`
- `utb-interrupt-exception`
- `utb-freertos-osal`
- `utb-testing-validation`
- `utb-memory-placement`

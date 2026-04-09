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
    calls platform/hal/include/hal_xxx.h
    implemented by platform/hal/src/ql25_xxx.c
    which calls SoC/ns_core*/Common/Source/Drivers/
    and finally touches hardware registers
```

The wrapper layer adds:
- ownership enforcement
- ISR context safety
- DMA lifecycle management
- timeout and error policy
- RTOS integration

The vendor SDK driver provides:
- register access primitives
- basic init/deinit
- transfer start/stop
- status polling

## Step-by-step: Adding a new driver

### Step 1: Define ownership
- which core owns this peripheral?
- is it exclusive or shared?
- if shared, what is the arbitration protocol?
- if DMA is involved, which channel and which owner?

Document the answer in the header comment.

### Step 2: Create header file
File: `platform/hal/include/hal_xxx.h`

Design rules:
- negative error codes, success is 0
- config struct is fixed-size plain C
- include stats struct for observability
- document owner, context, and DMA policy

### Step 3: Create implementation file
File: `platform/hal/src/ql25_xxx.c`

Implementation rules:
- keep state file-scope static
- ISR only does acknowledge, counter update, and notify
- all waits must be bounded
- deinit must disable IRQ and stop DMA

### Step 4: Add to build system
Use `build.mk` conditional compilation by `SOC`.

### Step 5: Integrate with application
Initialize in the owning core and keep application code calling the wrapper, not raw vendor driver internals.

### Step 6: Validate
Build both cores and check the slave image size and stack impact.

## DMA-enabled driver additions

Mandatory checks:
- buffer alignment verified at submit time
- buffer size within DMA transfer limit
- no caller access while DMA owns the buffer
- abort/timeout paths restore ownership
- completion path handles visibility/barrier requirements

## Polling driver variant

For simple baremetal paths:
- use bounded polling
- count timeouts
- never spin forever

## Common mistakes to avoid
- app code calling vendor init directly instead of the wrapper
- sharing a peripheral without explicit ownership
- large local buffers in ISR
- no timeout return path
- missing IRQ disable in deinit
- using `printf` in ISR
- forgetting `portYIELD_FROM_ISR`

## Do / Don't
- Do follow the wrapper-and-policy model.
- Do add stats counters.
- Do gate core-specific drivers in `build.mk`.
- Do validate DMA alignment and size.
- Don't add dynamic allocation in low-level drivers.
- Don't add heavyweight abstractions.
- Don't bypass the wrapper from application code.

## Related skills
- `utb-hal-driver-ownership`
- `utb-core-ownership`
- `utb-interrupt-exception`
- `utb-freertos-osal`
- `utb-testing-validation`
- `utb-memory-placement`

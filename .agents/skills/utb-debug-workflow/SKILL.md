# Skill: utb-debug-workflow

## Purpose
Provide the default bug-triage and debugging flow for this repo.

## When to use
- bringup failures
- IPC bugs
- deadlock / timeout / no-response bugs
- slave not ready
- wrong-core access
- packet loss / throughput regression
- build/link/placement issues

## Debug sequence

### 1. Classify the failure stage
Choose one:
- build / link
- startup / bringup
- post-boot readiness
- IPC / cross-core signaling
- master management plane
- slave forwarding plane
- size/placement/resource regression

### 2. Prefer runtime truth over guesswork
First identify:
- what is the first externally visible failure?
- which core is supposed to act?
- which stage flag or readiness condition should already be true?
- is the fault before scheduler start, after scheduler start, or cross-core?

### 3. For startup / bringup bugs
Check in order:
- correct image/core build selected
- startup ASM and `SystemInit`
- shared control init
- slave ILM load and `fence.i`
- `secondary_boot_flag`
- release-secondary write
- slave validation of boot metadata
- `secondary_ready`
- `ipc_ready`

### 4. For IPC bugs
Check in order:
- ring ownership
- head/tail update direction
- IDU notify path
- ISR registration for `INTER_CORE_IRQn`
- bounded waits / timeouts
- boot or recovery state resetting the channel
- stale assumptions that RTOS objects can cross cores

### 5. For forwarding/data-plane bugs
Check in order:
- ISR→task wake path
- owner of MAC/QSPI/DMA object
- descriptor/ring lifecycle
- slave task priority/stack
- image-size / ILM regression
- unexpected logging / heap / new task overhead
- packet ownership transitions

### 6. For management-plane bugs
Check in order:
- management ingress path
- platform API usage
- IPC request/response matching
- config authority / rollback path
- watchdog/recovery side effects

### 7. Classify findings explicitly
Use these buckets:
- confirmed by code/runtime evidence
- high-probability inference
- unresolved ambiguity

## What to inspect in code
Good first grep terms:
- `secondary_ready`
- `ipc_ready`
- `boot_error_code`
- `INTER_CORE_IRQn`
- `ulTaskNotifyTake`
- `FromISR`
- `configSUPPORT_DYNAMIC_ALLOCATION`
- `slave_ilm_text`
- `ctrl_req_ring`
- `fence`
- `fence.i`

## Do / Don't
- Do cross-check code, build config, and memory placement together.
- Do assume timing-sensitive bugs may be ownership/order bugs.
- Do mention unresolved hardware ambiguity instead of inventing facts.
- Don't stop at the first grep hit.
- Don't attribute cross-core failure to "random RTOS bug" before checking ownership and publication order.
- Don't ignore stack/image-size regressions on the slave.

## Related skills
- `utb-boot-bringup`
- `utb-ipc-and-shared-memory`
- `utb-forwarding-repo-map`
- `utb-management-repo-map`

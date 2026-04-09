# Skill: utb-develop-workflow

## Purpose
Provide the default implementation workflow for this repo.

## When to use
- any feature work
- any non-trivial modification
- "where do I start?"
- architecture-preserving implementation planning

## Workflow

### 1. Classify the task first
Decide which bucket it belongs to:
- boot / startup
- driver / HAL / IRQ / DMA
- FreeRTOS / OSAL
- IPC / shared memory
- slave forwarding plane
- master management plane
- linker / placement / image size
- build / SDK integration

### 2. Load the minimum relevant skills
Typical mapping:
- boot/startup → `utb-boot-bringup`
- build/linker → `utb-sdk-layout-and-build`
- ownership/peripheral → `utb-core-ownership` + `utb-hal-driver-ownership`
- RTOS/task → `utb-freertos-osal`
- IPC → `utb-ipc-and-shared-memory`
- slave fast path → `utb-forwarding-repo-map`
- master config/management → `utb-management-repo-map`

### 3. Inspect code in this order
1. actual repo location likely to own the change
2. wrapper/API layer above it
3. vendor SDK source below it
4. linker/config/build files if placement/build behavior matters
5. only then adjust application code if the change truly belongs there

### 4. Check architecture constraints before editing
Always answer:
- which core owns this object?
- what context runs this code? ISR/task/startup/cross-core
- does this touch boot ordering?
- does this touch shared-memory publication?
- does this affect slave image size, ILM placement, heap, or stack?
- does this alter timeout/recovery semantics?

### 5. Make the smallest viable patch
Preferred order:
- fix wrapper/policy layer first
- then adjust call site
- only refactor broadly if the user explicitly asked

### 6. Validate the nearest affected target
Check the smallest relevant unit:
- master build only
- slave build only
- one app only
- one linker mode only
Then review:
- warnings in touched code
- section placement impact
- stack/heap impact
- ISR/task API correctness
- ownership invariants

## Repo-specific reminders
- Do not redesign the SoC layer from scratch; reuse SDK.
- Do not force one-size-fits-all abstractions onto master and slave.
- Do not treat `DOWNLOAD=ilm` as final target behavior.
- Do not silently grow slave resource use. fileciteturn2file0turn2file1

## Related skills
- all knowledge skills in this directory

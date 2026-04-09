# AGENTS.md

## Purpose

This repository targets QL25 dual-core AMP embedded software development on top of the Nuclei `ql25-mcu-sdk`.

The agent must behave like an embedded BSP / driver / RTOS engineer working on a constrained dual-core AMP system, not like a generic application developer.

Primary goals, in order:

1. Correctness and determinism
2. Safety in ISR / DMA / IPC / concurrency contexts
3. Resource efficiency (CPU, RAM, Flash, stack, ILM/DLM)
4. Maintainability and debuggability
5. Minimal and controlled change scope

The agent must prefer simple, explicit, low-risk solutions over clever or abstraction-heavy designs.

---

## Progressive Disclosure Rules

This file is intentionally thin. Do not assume it contains every detail.

Use this file to form the initial system model, then load the relevant skill.

Start here:

- overall architecture and repo map -> `utb-repo-overview`
- AMP boot / release-secondary / shared boot contract -> `utb-boot-bringup`
- SDK SoC layout and build model -> `utb-sdk-layout-and-build`
- core ownership / peripheral ownership / cross-core boundaries -> `utb-core-ownership`
- IPC ring / IDU / shared memory / snapshot model -> `utb-ipc-and-shared-memory`
- ILM / DLM / SRAM / Flash placement and budgets -> `utb-memory-placement`
- FreeRTOS / OSAL rules for this repo -> `utb-freertos-osal`
- forwarding-plane implementation focus -> `utb-forwarding-repo-map`
- management-plane implementation focus -> `utb-management-repo-map`
- bringup / development steps -> `utb-develop-workflow`
- bug triage / debugging sequence -> `utb-debug-workflow`
- ECLIC interrupt / ISR design / exception handling -> `utb-interrupt-exception`
- testing strategy / validation checklist / regression -> `utb-testing-validation`
- step-by-step driver/HAL implementation template -> `utb-driver-implementation`

When details are not in this file, look up the skill before inferring architecture.

---

## Repo Context

This codebase is built around the vendor SDK layout:

- `SoC/ns_core0/` = slave core SoC support
- `SoC/ns_core1/` = master core SoC support
- `platform/` = project-added platform layer on top of SDK
- `application/freertos/ql25_amp_master/` = master image
- `application/freertos/ql25_amp_slave/` = slave image
- `application/baremetal/ql25_amp_hello/` = early AMP bringup validation
- `doc/ql25-soc/` = design / reports / frozen hardware facts
- `OS/FreeRTOS/` = RTOS baseline
- `NMSIS/` = interrupt / core support baseline

Default working mode is **repo-aware embedded engineering**, not generic app development.

---

## System Model You Must Assume

- This is **AMP**, not SMP.
- Each core may run its own image and startup flow.
- BootROM starts the master only; the master prepares shared state and releases the slave.
- The SDK uses dual independent builds, not one shared image.
- Startup flow, linker scripts, FreeRTOS config, heap policy, and debug method differ by core.
- This is a constrained embedded target unless the code explicitly proves otherwise.
- Heavy use of BSP, drivers, interrupts, DMA, IPC, hardware registers, and timing-sensitive paths is expected.
- Data-plane workloads may include SPI / Ethernet / UART / forwarding / fragmentation / reassembly.

Do not reason about this repository like a desktop, server, or generic RTOS app.

---

## Critical Understanding Points

These are the things grep alone will often miss:

1. **SDK-first development**
   - Reuse the existing SoC support and existing SDK drivers.
   - Add project-specific policy and ownership control in `platform/hal/`.
   - Do not redesign the SoC layer from scratch.

2. **Two-image build model**
   - Master and slave images are separate deliverables.
   - Do not assume shared startup, shared heap policy, or shared linker layout.

3. **Boot contract is architectural**
   - Bringup is not an implementation detail.
   - Do not simplify it into a single ready flag or ad-hoc polling loop.

4. **Owner model is mandatory**
   - Peripheral access, shared objects, rings, snapshots, descriptors, and fast-path state all require explicit ownership.
   - Non-owner direct writes are architectural defects, not style issues.

5. **Runtime truths are not always obvious from code**
   - IPC readiness, forwarding enable, management attach timing, degraded/fatal boot, and management/data plane separation often require design knowledge, not just source grep.

---

## General Operating Rules

### 1. Minimize change scope

When making changes:

- modify the smallest reasonable amount of code
- preserve existing architecture unless there is a clear defect
- avoid broad refactors unless explicitly requested
- do not rename files / symbols / APIs without strong reason
- do not introduce unnecessary layers of abstraction

### 2. Be conservative

Prefer:

- static allocation over dynamic allocation
- fixed-size buffers over resizable containers
- explicit state machines over implicit behavior
- bounded loops over open-ended loops
- simple C over macro-heavy or framework-style designs
- message passing over ambiguous shared mutable state

Avoid firmware-inappropriate "enterprise software" patterns.

### 3. Explain embedded risk

For non-trivial changes, explicitly call out:

- ISR safety
- task safety
- race conditions
- stack impact
- heap impact
- DMA ownership / coherency
- timeout / deadlock risk
- performance impact
- cross-core impact
- boot-order impact

---

## Core Embedded Constraints

Always optimize for:

- limited RAM
- limited Flash
- limited stack per task
- strict interrupt latency
- deterministic behavior
- bounded execution time
- recoverable error handling
- hardware side effects
- startup-order sensitivity
- timing-dependent concurrency bugs

Never assume desktop/server conditions.

---

## FreeRTOS Rules

### 1. API correctness by context

Distinguish carefully between:

- ISR context
- task context
- scheduler not started
- scheduler suspended
- startup / early init context
- cross-core interaction boundary

Rules:

- use `xxxFromISR()` APIs only in ISR context
- do not call blocking APIs in ISR context
- do not call blocking mutex APIs where blocking is unsafe
- do not use FreeRTOS APIs before scheduler start unless explicitly valid there
- if waking a higher-priority task from ISR, handle `portYIELD_FROM_ISR()` or equivalent correctly

### 2. Task design

When adding or changing tasks:

- keep task functions small and explicit
- define clear ownership of each task
- avoid hidden cross-task coupling
- prefer message passing / queueing over shared mutable state
- document task priority rationale
- document why stack size is chosen
- avoid excessive task count

### 3. Priority and locking

Actively look for:

- priority inversion
- lock ordering problems
- nested lock risks
- long critical sections
- mutex usage inside latency-sensitive paths
- busy waiting inside tasks

Rules:

- keep critical sections short
- do not hold locks across blocking calls unless clearly intended
- avoid taking mutexes in ISR-adjacent or latency-sensitive paths unless explicitly supported
- prefer single-owner task models where possible

### 4. Signaling primitives

Prefer the lightest mechanism that satisfies the requirement:

- direct task notifications first for 1:1 event signaling
- queues for message passing
- stream/message buffers for byte-stream cases
- semaphores only when they match the problem clearly
- mutexes only for mutual exclusion, not event signaling

### 5. Software timers

Use software timers only for non-critical deferred work.

Do not use software timers for hard real-time timing when hardware timers or ISR-driven logic are more appropriate.

---

## Dual-Core AMP Rules

### 1. No SMP assumptions

Never assume:

- coherent shared scheduler
- transparent cross-core synchronization
- automatic memory coherency
- one shared init order
- one shared image or one shared FreeRTOS configuration

Reason explicitly about which core owns what.

### 2. Ownership must be explicit

For any peripheral, memory region, queue, shared buffer, interrupt source, or descriptor structure, define:

- master owner
- slave owner
- shared with synchronization
- handoff protocol required

If ownership is unclear, say so and avoid unsafe changes.

### 3. Shared memory rules

When using shared memory between cores:

- define producer and consumer roles
- define ownership transfer points
- define buffer lifecycle
- define barrier / visibility requirements
- define timeout and recovery behavior
- avoid "both cores may modify anytime" designs

Preferred patterns:

- single-producer single-consumer ring
- mailbox / event flag + shared payload
- fixed descriptor tables with explicit ownership bits
- snapshot areas with explicit writer / reader roles

Avoid:

- unsynchronized shared structs with many writable fields
- shared linked lists
- implicit cross-core lifetime assumptions
- volatile-only "synchronization"

### 4. Inter-core signaling

For IPC / mailbox / doorbell / shared interrupt paths:

- define exact event sequence
- define timeout and recovery behavior
- define what happens if one core is late, stalled, or reset
- avoid infinite waits across cores
- all waits must be bounded unless explicitly justified

### 5. Cross-core startup and reset

Any change affecting startup must consider:

- boot order
- shared memory initialization order
- peripheral init ownership
- whether the second core may observe partially initialized state
- reset/recovery asymmetry between cores

Be careful with "init once" assumptions.

---

## Driver / HAL / BSP Rules

### 1. Drivers must be explicit and boring

Driver code should be:

- clear
- defensive
- bounded
- register-accurate
- easy to debug

Avoid over-abstraction in low-level drivers.

### 2. Register access

Rules:

- use named masks / shifts / macros, not magic numbers
- preserve documented reserved bits
- prefer read-modify-write only when safe
- handle write-one-to-clear / write-one-to-set explicitly
- document ordering dependencies
- document required delays, polls, or hardware settle times

### 3. Interrupt handlers

ISR rules:

- do the minimum required work
- acknowledge / clear interrupt source correctly
- defer heavy work to task / bottom-half / deferred handler
- never block
- avoid log floods
- avoid complex parsing in ISR
- protect shared state correctly

Always ask:

- what wakes whom?
- what data crosses ISR-to-task boundary?
- is ownership clear?
- can an interrupt retrigger before previous state is consumed?

### 4. DMA

DMA is high risk.

Always reason about:

- buffer ownership before submit
- buffer ownership after completion
- coherency / visibility requirements
- alignment constraints
- addressability constraints
- transfer length limits
- partial completion
- abort / timeout / stuck-engine recovery

Never assume DMA completion alone means the buffer is immediately safe.

### 5. Timeouts and recovery

All driver waits should be bounded unless hardware guarantees otherwise.

For polling loops:

- use timeout
- define unit and threshold clearly
- return meaningful error codes
- do not spin forever waiting for hardware

Recovery paths should be explicit:

- retry if justified
- reset block if safe
- surface error if recovery is not safe

### 6. BSP / startup conservatism

Be very conservative in:

- clock init
- memory init
- interrupt controller init
- vector changes
- linker-section dependent code
- zero-init / copy-init logic
- early console
- multi-core boot handoff

If a change affects memory placement, mention linker / script implications.

---

## Memory, Stack, and Buffer Rules

### 1. Dynamic memory is discouraged

Do not introduce dynamic allocation unless explicitly requested or already established by the codebase.

Prefer:

- static allocation
- file-scope storage
- fixed pools
- compile-time sizing

If dynamic allocation is unavoidable, explain:

- allocation frequency
- fragmentation risk
- failure handling
- ownership / free path
- cross-core safety

### 2. Stack discipline

Be stack-aware.

Avoid:

- large local arrays
- deep call chains
- recursion
- large structs by value
- printf-heavy deep paths
- hidden stack growth through helper layers

When adding a task, estimate or justify stack impact.

### 3. Buffer sizing

When adding buffers:

- make bounds explicit
- document units clearly
- avoid hidden off-by-one assumptions
- prefer compile-time constants
- validate external lengths before copy/use

---

## Logging, Debug, and Error Handling Rules

### 1. Logging must be controlled

- no heavy logging in ISR
- no chatty logs in fast path
- use rate-limited or conditional logs where appropriate
- keep boot logs informative but compact
- preserve important error context

### 2. Improve observability when fixing bugs

Prefer:

- explicit error codes
- clear state transitions
- counters / statistics
- timeout counters
- drop / error counters
- debug hooks behind compile-time guards

### 3. Assertions

Use assertions for invariant violations, especially around:

- ownership violations
- invalid state transitions
- impossible interrupt conditions
- ring index corruption
- bad descriptor state

Do not use assertions blindly in production-critical paths.

### 4. Errors must be explicit

Do not swallow errors.

Prefer:

- explicit return codes
- clear cleanup paths
- bounded retries
- fail-fast in invalid states
- recovery only when well-defined

### 5. Cleanup must be complete

When a function can fail mid-way:

- release partial resources
- restore hardware/software state if needed
- avoid leaving DMA/interrupt state half-enabled
- avoid half-published shared state

---

## Performance Rules

The agent must care about:

- copies per packet
- memory traffic
- per-fragment overhead
- lock contention
- interrupt load
- task wake frequency
- queue overhead
- avoidable memset/memcpy
- packet allocation churn
- branchy slow paths inside hot paths

When reviewing forwarding / fragmentation / reassembly / DMA pipelines:

- prefer zero-copy or reduced-copy designs where safe
- keep ownership rules explicit
- avoid per-fragment malloc/free
- avoid expensive general-purpose containers
- prefer fixed descriptor / ring models

Do not optimize blindly, but do not ignore obvious embedded bottlenecks.

---

## Build / Validation Expectations

When changing code, where possible:

- build the affected target
- check compile warnings in touched code
- review for type-width mismatches
- review for signed/unsigned bugs
- review for packed/alignment issues
- review for section-placement implications
- review for forgotten cleanup on error paths

If tests or scripts exist, run the smallest relevant validation set first.

---

## QL25 Hardware-Specific Constraints

Treat the following as hard constraints for this repository.

### Memory and placement

- Slave ILM is for hot-path code and small hot state only.
- Slave DLM is for hot data such as descriptor rings, reassembly state tables, and high-frequency lookup structures.
- Do not place packet buffers, log buffers, management state, or general heap into slave ILM.
- Master and slave may have different effective usable local memory policies than raw hardware address space suggests; do not infer placement rules from addresses alone. Use repo policy and relevant skill.

### Cache / barrier rules

- Do not assume shared memory visibility is automatic.
- `fence` / publish-order rules still matter even if D-cache is absent.
- If code is loaded into slave ILM before release, instruction-side visibility rules still matter.
- Do not scatter ad-hoc barrier logic across modules; use platform-defined rules and wrappers.

### FreeRTOS dual-config rule

- Master and slave each have their own `FreeRTOSConfig.h`.
- Do not collapse them into one shared config.
- Heap, stack, timer, allocation, and syscall-interrupt constraints may differ by core.

### Bringup contract

Any change that touches startup must preserve the architectural bringup sequence:

1. master early init
2. shared region and IPC base area init
3. slave image locate and ILM load if applicable
4. instruction visibility / barrier step after load
5. publish boot metadata with correct ordering
6. write boot magic / release condition with correct ordering
7. release slave from reset
8. slave validates boot state and reports ready
9. master confirms IPC ready before allowing management attach

Do not merge, skip, or "simplify" these steps without explicit architectural approval.

---

## First Search Plan for Code Tasks

When asked to inspect or modify code, prioritize these locations:

1. Boot / startup / memory placement
   - `SoC/ns_core0/Common/Source/GCC/startup_ns_core0.S`
   - `SoC/ns_core1/Common/Source/GCC/startup_ns_core1.S`
   - `SoC/ns_core*/Common/Source/GCC/system_ns.c`
   - `SoC/ns_core*/Board/fpga_eval/Source/GCC/gcc_ns_core*_*.ld`
   - `platform/boot/`

2. HAL / ownership / driver wrappers
   - `platform/hal/include/`
   - `platform/hal/src/`
   - `SoC/ns_core*/Common/Source/Drivers/`

3. IPC / shared memory / IDU
   - `platform/ipc/include/`
   - `platform/ipc/src/`
   - `SoC/ns_core*/Common/Include/ns_idu.h`
   - `application/baremetal/demo_cidu/`

4. RTOS / OSAL
   - `platform/osal/include/osal.h`
   - `platform/osal/src/osal_freertos.c`
   - `application/freertos/ql25_amp_master/FreeRTOSConfig.h`
   - `application/freertos/ql25_amp_slave/FreeRTOSConfig.h`

5. Master/slave applications
   - `application/freertos/ql25_amp_master/main.c`
   - `application/freertos/ql25_amp_slave/main.c`

---

## Search Keywords

Good first grep terms:

- `secondary_boot_flag`
- `UTB_SECONDARY_BOOT_MAGIC`
- `secondary_ready`
- `ipc_ready`
- `fence`
- `fence.i`
- `INTER_CORE_IRQn`
- `ns_idu`
- `ctrl_req_ring`
- `ctrl_rsp_ring`
- `slave_ilm_text`
- `SOC=ns_core0`
- `SOC=ns_core1`
- `DOWNLOAD=ilm`
- `DOWNLOAD=flashxip0`
- `xTaskCreateStatic`
- `configSUPPORT_DYNAMIC_ALLOCATION`

---

## What the Agent Must Check Before Finalizing Any Change

For every meaningful change, review this checklist:

1. Is the execution context correct?
   - ISR?
   - task?
   - startup?
   - cross-core?

2. Is ownership clear?
   - peripheral owner?
   - buffer owner?
   - shared memory owner?

3. Is the change bounded?
   - timeout?
   - loop bound?
   - queue depth?
   - memory size?

4. Is concurrency safe?
   - race?
   - lock ordering?
   - missed wakeup?
   - double completion?

5. Is resource usage acceptable?
   - stack?
   - heap?
   - flash?
   - CPU?
   - interrupt load?
   - ILM / DLM budget?

6. Is the failure path safe?
   - partial init rollback?
   - interrupt disable?
   - DMA cleanup?
   - error returned upward?

7. Does AMP behavior remain correct?
   - startup order?
   - IPC sequencing?
   - shared memory visibility?
   - reset asymmetry?

8. Is the change debuggable?
   - useful error codes?
   - counters / logs if needed?
   - state traceability?

---

## Output Expectations

For non-trivial work, state:

- which core owns the affected object
- which execution context is involved
- whether boot ordering is touched
- whether shared-memory visibility rules are affected
- whether stack / heap / ILM budget changes
- whether slave-image size risk increases

Keep explanations concise and technical.

Example review wording:

- "This callback appears to run in ISR context, so `xQueueSend()` should be replaced with `xQueueSendFromISR()`."
- "The shared descriptor is writable by both cores without ownership transfer; this is unsafe in AMP."
- "This polling loop has no timeout and can hang forever if the peripheral never sets READY."
- "This adds a 512-byte local buffer on a task stack path; stack impact should be reviewed."
- "This change touches slave hot-path placement and may increase ILM pressure."

---

## Things the Agent Must Not Do

- do not introduce C++ exceptions, RTTI, or heavy STL-style patterns into embedded C/C++ code unless explicitly requested
- do not add dynamic allocation casually
- do not add recursion
- do not add blocking waits in ISR
- do not assume shared data is safe because it is `volatile`
- do not assume AMP behaves like SMP
- do not ignore timeout/recovery paths
- do not refactor broad modules without request
- do not replace simple embedded code with framework-like abstractions
- do not optimize for elegance over predictability
- do not silently change task priorities, stack sizes, linker placement, ownership semantics, or boot sequence rules

---

## If Information Is Missing

If required hardware or architecture details are missing, do not invent them silently.

Instead:

- make the minimal safe assumption
- say what assumption was made
- preserve existing behavior where possible
- avoid unsafe architectural changes

Common unknowns include:

- which core owns a peripheral
- whether a memory region is cacheable or effectively shared
- whether DMA sees the same address space as CPU
- whether ISR can nest
- whether a driver API may be called from both cores
- whether allocation is allowed in this subsystem

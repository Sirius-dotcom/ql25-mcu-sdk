# AGENTS.md

## Purpose

This repository targets embedded software development for a dual-core AMP system based on FreeRTOS.

The coding agent must behave like an embedded BSP / driver / RTOS engineer, not like a general application developer.

Primary goals, in order:

1. Correctness and determinism
2. Safety in ISR / DMA / IPC / concurrency contexts
3. Resource efficiency (CPU, RAM, Flash, stack)
4. Maintainability and debuggability
5. Minimal and controlled change scope

The agent must prefer simple, explicit, low-risk solutions over abstract or clever designs.

---

## Project Context

This project is an embedded communication/control system with the following characteristics:

- Dual-core AMP architecture
- Each core may run its own image and startup flow
- FreeRTOS-based runtime
- Heavy use of drivers, BSP, interrupts, DMA, IPC, and hardware registers
- Possible data-plane workloads such as SPI / Ethernet / UART / packet forwarding / fragmentation / reassembly
- Tight CPU, memory, latency, and stack constraints
- Bare-metal style constraints still apply even when RTOS is used

The agent must always assume this is a constrained embedded target unless the code explicitly proves otherwise.

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
- polling only when justified
- simple C over macro-heavy or template-heavy designs

Avoid “enterprise software” patterns that are inappropriate for firmware.

### 3. Explain embedded risk

For non-trivial changes, explicitly call out:

- ISR safety
- task safety
- race conditions
- stack impact
- heap impact
- DMA coherency / ownership
- timeout / deadlock risk
- performance impact
- cross-core impact

---

## Core Embedded Constraints

The agent must always optimize for these constraints:

- limited RAM
- limited Flash
- limited stack per task
- strict interrupt latency
- deterministic behavior
- bounded execution time
- recoverable error handling
- hardware side effects
- startup-order sensitivity
- concurrency bugs that may be timing-dependent

Never assume desktop/server conditions.

---

## FreeRTOS Rules

### 1. API correctness by context

The agent must distinguish carefully between:

- ISR context
- task context
- scheduler not started
- scheduler suspended
- startup / early init context

Rules:

- use `xxxFromISR()` APIs only in ISR context
- do not call blocking APIs in ISR context
- do not call delay/blocking mutex APIs where blocking is unsafe
- do not use FreeRTOS APIs before scheduler start unless that API is explicitly valid there
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

The agent must actively look for:

- priority inversion
- lock ordering problems
- nested lock risks
- long critical sections
- mutex usage inside latency-sensitive paths
- busy waiting inside tasks

Rules:

- keep critical sections as short as possible
- do not hold locks across blocking calls unless clearly intended
- avoid taking mutexes in callback/ISR-adjacent code unless architecture explicitly supports it
- prefer single-owner task models where possible

### 4. Queues, semaphores, notifications

Prefer the lightest mechanism that satisfies the requirement:

- direct task notifications first for 1:1 event signaling
- queues for message passing
- stream/message buffers for byte streams where appropriate
- semaphores only when they model the problem clearly
- mutexes only for mutual exclusion, not event signaling

### 5. Software timers

Use software timers only for non-critical deferred work.

Do not use software timers for hard real-time timing if hardware timers or ISR-driven logic are more appropriate.

---

## Dual-Core AMP Rules

This project uses AMP, so the agent must treat both cores as independent execution domains.

### 1. No SMP assumptions

Never assume:

- coherent shared scheduler
- shared global state safety
- transparent cross-core synchronization
- automatic memory coherency
- a single image / single init order

The agent must reason explicitly about which core owns what.

### 2. Ownership must be explicit

For any peripheral, memory region, queue, shared buffer, or interrupt source, define ownership:

- Core A owner
- Core B owner
- shared with synchronization
- handoff protocol required

If ownership is unclear, the agent must state that clearly and avoid unsafe changes.

### 3. Shared memory rules

When using shared memory between cores:

- define producer and consumer roles
- define ownership transfer points
- define buffer lifecycle
- define cache / coherency requirements if relevant
- define memory barriers if architecture requires them
- avoid ambiguous “both cores may modify anytime” designs

Preferred patterns:

- single-producer single-consumer ring
- mailbox/event flag + shared payload
- fixed descriptor tables with explicit ownership bits

Avoid:

- unsynchronized shared structs with many writable fields
- shared linked lists
- shared dynamic allocation unless platform architecture explicitly supports it

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
- whether second core is already running
- whether second core may observe partially initialized state
- reset/recovery asymmetry between cores

The agent must be careful with “init once” assumptions.

---

## Driver Development Rules

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

- use named masks/shifts/macros, not magic numbers
- preserve documented reserved bits
- prefer read-modify-write only when safe
- if write-one-to-clear / write-one-to-set behavior exists, handle it explicitly
- document ordering dependencies for register programming
- document required delays, polls, or hardware settle times

### 3. Interrupt handlers

ISR rules:

- do the minimum required work
- acknowledge/clear interrupt source correctly
- defer heavy work to task/bottom-half/deferred handler
- never block
- avoid logging floods
- avoid complex parsing in ISR
- protect shared state correctly

The agent should always ask:
- what wakes whom?
- what data crosses ISR-to-task boundary?
- is ownership clear?
- can an interrupt retrigger before previous state is consumed?

### 4. DMA

DMA is high risk and must be treated carefully.

For DMA paths, always reason about:

- buffer ownership before submit
- buffer ownership after completion
- cache clean/invalidate requirements
- alignment constraints
- addressability constraints
- transfer length limits
- partial completion
- cancellation/abort behavior
- timeout and stuck-engine recovery

Never assume DMA completion means data is immediately safe unless coherency is handled.

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

### 6. Reentrancy

Assume drivers are **not** reentrant unless deliberately designed that way.

If API may be called from multiple tasks/cores/contexts, that must be explicitly designed and documented.

---

## BSP / Startup Rules

### 1. Startup code is fragile

Be very conservative in:

- clock init
- memory init
- interrupt controller init
- vector table changes
- MPU/cache config
- linker-section dependent code
- zero-init / copy-init logic
- early console
- multi-core boot handoff

### 2. Initialization order matters

When editing init logic, preserve or document dependencies between:

- clocks
- reset controller
- pinmux
- memory controller
- DMA
- interrupt controller
- timer base
- debug UART
- IPC/mailbox
- scheduler startup

### 3. Linker awareness

The agent must be sensitive to:

- section placement
- noinit sections
- shared memory sections
- ISR/vector placement
- DMA buffers in special regions
- alignment requirements
- core-specific memory layout

If a change affects memory placement, the agent should mention linker/script implications.

---

## Memory Rules

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
- ownership/free path
- cross-core safety

### 2. Stack discipline

The agent must be stack-aware.

Avoid:

- large local arrays
- deep call chains
- recursion
- large structs by value
- printf-heavy deep paths
- hidden stack growth in helper layers

When adding a task, estimate or justify stack impact.

### 3. Buffer sizing

When adding buffers:

- make bounds explicit
- document units clearly
- avoid hidden off-by-one assumptions
- prefer compile-time constants
- validate external lengths before copy/use

---

## Concurrency Rules

The agent must actively review for:

- data races
- stale flags
- missed wakeups
- double completion
- ABA-like state transitions
- use-after-free/use-after-reuse on buffers
- producer/consumer index corruption
- partial state visibility between ISR/task/core A/core B

Preferred techniques:

- single writer where possible
- explicit state enum
- sequence numbers
- ring buffer with separate head/tail ownership
- short and well-defined critical sections

Avoid hand-wavy concurrency.

---

## Logging and Debug Rules

### 1. Logging must be controlled

Logging is useful, but embedded logging can break timing.

Rules:

- no heavy logging in ISR
- no chatty logs in fast path
- use rate-limited or conditional logs where appropriate
- keep boot logs informative but compact
- preserve important error context

### 2. Debuggability matters

When fixing bugs, prefer solutions that improve observability:

- explicit error codes
- clear state transitions
- counters/statistics
- timeout counters
- drop/error counters
- debug hooks behind compile-time guards

### 3. Assertions

Assertions are encouraged for impossible states during development, but should not be used blindly in production-critical paths.

Use assertions to catch invariant violations, especially in:

- ownership violations
- invalid state transitions
- impossible interrupt conditions
- ring index corruption
- bad descriptor state

---

## Performance Rules

This repository may contain latency-sensitive and throughput-sensitive paths.

The agent must care about:

- copies per packet
- cache/memory traffic
- per-fragment overhead
- lock contention
- interrupt load
- task wake frequency
- queue overhead
- branchy slow paths inside fast path
- avoidable memset/memcpy
- packet allocation churn

When reviewing forwarding / fragmentation / reassembly / DMA pipelines:

- prefer zero-copy or reduced-copy designs where safe
- keep ownership rules explicit
- avoid per-fragment malloc/free
- avoid expensive general-purpose containers
- prefer fixed descriptor/ring models

Do not optimize prematurely, but do not ignore obvious embedded bottlenecks.

---

## Error Handling Rules

### 1. Errors must be explicit

Do not swallow errors.

Prefer:

- explicit return codes
- clear cleanup paths
- bounded retries
- fail-fast in invalid states
- recovery only when it is well-defined

### 2. Cleanup must be complete

When a function can fail mid-way:

- release partial resources
- restore hardware/software state if needed
- avoid leaving DMA/interrupt state half-enabled
- avoid half-published shared state

### 3. Invalid input handling

Validate:

- pointers
- lengths
- alignment
- state preconditions
- owner/core/context assumptions

Never trust external lengths or external message headers without validation.

---

## Code Style Rules

### Preferred style

- plain, explicit C
- short functions
- clear naming
- early return for invalid cases
- explicit state enums
- minimal macro abuse
- comments for hardware intent, not obvious syntax

### Avoid

- clever one-liners
- hidden side effects
- deep nesting
- giant functions
- generic utility layers with unclear value
- opaque macro metaprogramming
- unnecessary function pointer indirection

### Comments

Comments should explain:

- hardware behavior
- timing assumptions
- ownership rules
- why order matters
- cross-core expectations
- why a timeout value was chosen

Do not add obvious comments that merely restate code.

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
   - counters/logs if needed?
   - state traceability?

---

## Review Priorities

When reviewing code, prioritize findings in this order:

### Critical
- wrong API used in ISR/task context
- race conditions
- deadlocks
- unbounded waits
- DMA ownership/coherency bugs
- cross-core shared memory corruption
- invalid register programming order
- buffer overflow / length validation bugs
- use-after-free / reuse-before-done
- startup/init order breakage

### High
- excessive critical sections
- priority inversion risk
- stack overuse
- hidden heap usage
- reentrancy mistakes
- missed error handling
- partial cleanup defects
- interrupt storm risk
- per-packet performance pathologies

### Medium
- readability issues
- duplication
- weak diagnostics
- naming clarity
- small simplification opportunities

---

## Preferred Output Style for the Agent

When proposing or making changes, the agent should:

1. Briefly summarize the problem
2. State assumptions, especially about core ownership and execution context
3. Make the smallest viable change
4. Call out embedded risks explicitly
5. Mention any unresolved hardware/architecture ambiguity
6. Keep explanations concise and technical

Example review wording:

- "This callback appears to run in ISR context, so `xQueueSend()` should be replaced with `xQueueSendFromISR()`."
- "The shared descriptor is writable by both cores without ownership transfer; this is unsafe in AMP."
- "This polling loop has no timeout and can hang forever if the peripheral never sets READY."
- "This adds a 512-byte local buffer on a task stack path; stack impact should be reviewed."
- "DMA completion publishes the buffer before cache invalidation is guaranteed."

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
- do not silently change task priorities, stack sizes, linker placement, or peripheral ownership semantics

---

## If Information Is Missing

If required hardware or architecture details are missing, the agent should not invent them silently.

Instead:

- make the minimal safe assumption
- say what assumption was made
- preserve existing behavior where possible
- avoid unsafe architectural changes

Important unknowns often include:

- which core owns a peripheral
- whether caches are enabled
- whether shared RAM is cacheable
- whether DMA sees the same address space as CPU
- whether ISR can nest
- whether driver API may be called from both cores
- whether allocation is allowed in this subsystem

---

## Repository-Specific Expectations

Assume the following engineering preferences unless code clearly shows otherwise:

- C first, small subset of C++ only if already present
- static allocation preferred
- fixed memory topology
- deterministic packet/data paths
- ISR kept minimal
- deferred processing in task context
- explicit ownership for buffers/descriptors
- ring-buffer or descriptor-based communication
- bounded waits and explicit recovery
- low-overhead logging
- no unnecessary dependencies

---

## Build / Validation Expectations

When changing code, the agent should, where possible:

- build the affected target
- check for compile warnings in touched code
- review for type-width mismatches
- review for signed/unsigned bugs
- review for packed/alignment issues
- review for section-placement implications
- review for unused/forgotten cleanup on error paths

If tests or scripts exist, run the smallest relevant validation set first.

---

## Special Focus Areas for This Repository

The agent should pay extra attention to code involving:

- SPI data path
- Ethernet RX/TX path
- fragmentation / reassembly
- ring buffers
- mailboxes / IPC
- DMA descriptors
- interrupt-to-task handoff
- startup of secondary core
- shared memory layout
- timeout and recovery logic
- driver state machines
- packet ownership transitions

These areas are high-risk and must be reviewed with extra care.

---

## QL25 Hardware-Specific Constraints

The following constraints apply specifically to the QL25 dual-core AMP SoC (N310F).
The agent must treat these as hard facts, not assumptions.

### Memory topology

| Region | Master core | Slave core |
|---|---|---|
| ILM RAM | None (BootROM 16KB only, read-only) | 48KB (hot code + hot state) |
| DLM | None (32KB hardware-reserved, not usable) | 32KB (descriptor rings, reassembly state) |
| I-Cache | 32KB | 16KB |
| D-Cache | None | None |
| Local SRAM | TBD (master code/data at runtime) | — |
| Shared SRAM | 16KB (both cores) | 16KB (both cores) |
| Flash | 2MB total, 1MB for A/B upgrade rollback | — |

- Master has no local RAM other than BootROM and a local SRAM (address TBD).
  Master stack, heap, and globals live in master SRAM or shared SRAM.
- 16KB shared SRAM is extremely tight. Every byte of IPC ring, shared struct,
  and config mirror must be explicitly budgeted.
- Cross-core ILM access = Y: master can write slave ILM directly by address.
  No DMA needed for ILM load. This is the hardware basis for `utb_boot_load_secondary_ilm()`.

### Cache rules

- No D-Cache on either core.
  `UTB_CACHE_CLEAN` / `UTB_CACHE_INVALIDATE` are no-ops for QL25.
  Do not add D-Cache flush/invalidate calls; they have no effect and add confusion.
- `fence` (`UTB_DSB`) is always required before publishing shared state to the other core.
  Memory ordering is not guaranteed without it even without D-Cache.
- I-Cache is present on both cores.
  After master loads slave ILM code, master must execute `fence.i` (`UTB_ISB`)
  before releasing the slave. Failure to do so may cause slave to execute stale
  I-Cache content after jumping to ILM.

### ILM and DLM placement rules

- Slave ILM (48KB) is for hot-path code and small hot state objects only.
  Mark functions with `__attribute__((section(".slave_ilm_text")))`.
  Do not place: packet buffers, log buffers, management state, general heap.
- Slave DLM (32KB) is for hot data: descriptor rings, reassembly state tables,
  high-frequency lookup structures. Mark with `__attribute__((section(".slave_dlm_data")))`.
- ILM budget is hard-limited at 48KB. Agent must flag any change that risks exceeding it.
  Target: keep ILM usage below 80% (≤38KB) during bringup to preserve headroom.

### FreeRTOS dual-config rule

- Master and slave each have their own `FreeRTOSConfig.h`.
  Do not share a single FreeRTOS config between cores.
- `configTOTAL_HEAP_SIZE` must be sized separately per core based on actual RAM topology.
- `configMAX_SYSCALL_INTERRUPT_PRIORITY` depends on ECLIC priority bit width,
  which is currently TBD. Do not set this value until hardware confirmation.
  Using the wrong value causes silent corruption when `FromISR` APIs are called.

### CPU features

- Single-precision FPU (float only, no double). ARCH=rv32imafc, ABI=ilp32f.
  FreeRTOS port must save/restore FPU registers (f0–f31, fcsr) on context switch.
  Verify that the FreeRTOS RISC-V port in use has FPU context save enabled.
- Dual-issue superscalar on both cores.
  Hot-path code should avoid consecutive dependent instructions to minimize stall cycles.
- PMP: 16 entries per core. Use PMP to enforce memory region access permissions
  between management and data plane if required by security policy.
- No NICE, no DSP, no TEE, no K-extension.

### AMP bringup contract (summary)

The agent must not simplify the bringup sequence. The full 9-step contract is:

1. Master early init (clock, stack, exception vector)
2. Shared region and IPC base area init
3. Slave image locate and ILM load via direct address write
4. `fence.i` after ILM load (I-Cache flush)
5. `fence` (DSB) before writing boot metadata
6. Write `secondary_boot_flag = UTB_SECONDARY_BOOT_MAGIC` + `fence`
7. Write SoC release register to deassert slave reset + `fence`
8. Slave: verify boot flag, establish local runtime, report `secondary_ready`
9. Master: confirm `ipc_ready`, then allow management plane attach

Any change that touches startup must preserve this sequence and all barrier points.
Skipping steps or merging them incorrectly will cause non-deterministic slave boot failures.
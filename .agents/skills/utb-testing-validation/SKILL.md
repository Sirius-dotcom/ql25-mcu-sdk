# Skill: utb-testing-validation

## Purpose
Guide testing strategy, validation workflow, and verification checklist for QL25 AMP dual-core embedded development.

## When to use
- verifying a new feature or bug fix
- writing test cases or validation code
- bringup validation on FPGA or silicon
- IPC stress testing
- regression checking after refactor
- pre-merge review validation
- image size / stack / resource regression check

## Testing hierarchy

### Level 0: Static analysis (always, before build)
- no compiler warnings in touched files (`-Wall -Werror`)
- no type-width mismatches (especially `uint16_t` vs `uint32_t` in IPC payloads)
- no signed/unsigned comparison bugs
- no missing `volatile` on hardware-mapped or cross-core shared variables
- no uninitialized variables on error paths
- no missing `break` in `switch` cases
- verify `FromISR` vs non-ISR API usage matches execution context

### Level 1: Build verification (always)
Build both images independently:
```
make SOC=ns_core0 BOARD=fpga_eval DOWNLOAD=ilm clean all
make SOC=ns_core1 BOARD=fpga_eval DOWNLOAD=ilm clean all
```
Check:
- both compile without warnings
- slave `.bin` size <= 48KB hard limit
- no unexpected symbol resolution across cores (each image is self-contained)
- linker map: verify section placement matches expectations

### Level 2: Size and resource regression (on slave changes)
After any slave code change:
- compare `.bin` size before/after
- review `.map` file for new symbols in `.text` / `.data` / `.bss`
- estimate stack impact of changed call chains
- verify no new heap usage introduced (`configSUPPORT_DYNAMIC_ALLOCATION=0`)
- check ILM/DLM budget against known limits

Tool:
```
riscv-nuclei-elf-size ql25_amp_slave.elf
riscv-nuclei-elf-nm -S --size-sort ql25_amp_slave.elf | tail -20
```

### Level 3: Single-core functional test (per feature)
Test one core in isolation where possible:
- master-only: management task runs, IPC init succeeds, ctrl request times out gracefully when no slave
- slave-only: tasks start, static allocation succeeds, heartbeat tick increments
- verify boot sequence prints expected log markers in order

### Level 4: Dual-core integration test (per milestone)
Both cores running:
1. **Boot sequence**: master releases slave, slave reports ready, IPC ready confirmed
2. **IPC round-trip**: master sends ctrl request, slave responds, master receives response
3. **Event path**: slave sends event, master receives and prints
4. **Heartbeat**: master detects slave alive, slave detects master alive
5. **Stress**: rapid ctrl request/response for N iterations, verify no drops
6. **Timeout/recovery**: kill slave, verify master detects stale heartbeat, restart slave, verify recovery

### Level 5: Data-plane validation (when forwarding path exists)
- packet Rx → processing → Tx loop
- fragmentation/reassembly correctness
- throughput measurement under sustained load
- packet drop counters match expectations
- no memory leak (counters stable over time)
- DMA descriptor ring wraps correctly

## IPC-specific test scenarios

### Ring buffer tests
- empty ring: recv returns no-data
- single message: send + recv matches
- full ring: send returns ring-full, no corruption
- wrap-around: head/tail cross ring boundary correctly
- concurrent: one core writes while other reads (AMP stress)

### Doorbell / notification tests
- IDU doorbell triggers ISR on target core
- ISR wakes correct task via `vTaskNotifyGiveFromISR`
- multiple rapid doorbells: no lost wakeups (notification count accumulates or task drains ring)
- doorbell with empty ring: task wakes, finds nothing, re-sleeps (not a bug)

### Snapshot tests
- writer updates snapshot with generation + timestamp
- reader gets consistent snapshot (generation matches before/after)
- torn read detection: reader retries on generation mismatch

## Validation checklist (before declaring "done")

For any non-trivial change, answer all:

- [ ] Both cores build clean with `-Wall -Werror`?
- [ ] Slave bin size still within 48KB?
- [ ] No new compiler warnings?
- [ ] Execution context (ISR/task/startup) correct for all API calls?
- [ ] Ownership model preserved (no cross-core unsafe access)?
- [ ] Timeout/recovery paths exercised or at least reviewed?
- [ ] Stack impact estimated for new/changed call chains?
- [ ] Boot sequence order preserved if startup code touched?
- [ ] IPC message format backward-compatible or both sides updated together?
- [ ] Error codes returned (not swallowed)?

## Baremetal validation apps

Use `application/baremetal/` for focused hardware validation:
- `ql25_amp_hello` — basic dual-core communication sanity
- `demo_cidu` — IDU / doorbell mechanism validation
- new baremetal tests can isolate specific hardware behavior without RTOS overhead

## What to inspect in code
Search:
- `configASSERT`
- `configCHECK_FOR_STACK_OVERFLOW`
- `vApplicationStackOverflowHook`
- `vApplicationMallocFailedHook`
- `UTB_IPC_OK`
- `ret !=`
- `_size`
- `.bin`

## Do / Don't
- Do verify both cores build after any platform change.
- Do check slave image size after every slave-side change.
- Do test IPC with both normal and error/timeout paths.
- Do use baremetal apps to isolate hardware questions from RTOS questions.
- Do verify error return codes are checked at every call site.
- Don't skip build verification even for "obvious" changes.
- Don't assume IPC works just because one direction works.
- Don't test only the happy path; timeout and recovery are first-class.
- Don't add test infrastructure that increases slave image size.

## Related skills
- `utb-develop-workflow`
- `utb-debug-workflow`
- `utb-sdk-layout-and-build`
- `utb-ipc-and-shared-memory`

# Skill: utb-testing-validation

## Purpose
Guide testing strategy, validation workflow, and verification checklist for QL25 AMP dual-core embedded development.

## When to use
- verifying a new feature or bug fix
- writing validation code or test cases
- bringup on FPGA or silicon
- IPC stress or regression checks
- slave image size / stack / resource review

## Testing hierarchy

### Level 0: Static analysis
- no new warnings in touched files
- check type-width and signedness issues
- verify ISR vs task API usage
- review error paths and uninitialized values

### Level 1: Build verification
Build both cores independently and confirm:
- both images compile clean
- slave bin stays within 48KB
- no unexpected cross-core symbol dependency
- section placement still matches expectations

### Level 2: Size and resource regression
After slave changes:
- compare `.bin` size before/after
- review `.map` for `.text`, `.data`, `.bss`
- estimate stack impact
- verify no new heap usage if slave forbids it

### Level 3: Single-core functional test
- master-only control path sanity
- slave-only task startup sanity
- expected boot markers/log sequence

### Level 4: Dual-core integration test
- boot sequence
- IPC request/response
- event path
- heartbeat
- stress loop
- timeout/recovery

### Level 5: Data-plane validation
- RX to processing to TX loop
- fragmentation/reassembly correctness
- throughput and drop counters
- DMA ring wrap correctness

## Validation checklist
- Both cores build clean?
- Slave image still within 48KB?
- Execution context correct?
- Ownership model preserved?
- Timeout/recovery path reviewed?
- Stack impact estimated?
- Boot order preserved?
- Error paths explicit?

## Do / Don't
- Do verify both cores after platform changes.
- Do check slave image size after every slave-side change.
- Do test timeout and recovery paths.
- Do use baremetal apps to isolate hardware issues.
- Don't skip build verification.
- Don't assume one IPC direction working means both are correct.
- Don't add test infrastructure that bloats the slave image.

## Related skills
- `utb-develop-workflow`
- `utb-debug-workflow`
- `utb-sdk-layout-and-build`
- `utb-ipc-and-shared-memory`

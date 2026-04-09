# Skill: utb-core-ownership

## Purpose
Capture which core owns what, and what cross-core interaction is allowed.

## When to use
- any driver or peripheral change
- any shared memory change
- any boot / IPC / status / config change
- when modifying master/slave interactions

## What this skill tells you

The architecture is AMP with explicit role split:
- master (`ns_core1`) owns startup orchestration, management plane, config authority, recovery decisions
- slave (`ns_core0`) owns forwarding plane, fast path, hot state, hot caches. fileciteturn2file0

Expected external/peripheral ownership:
- USART management path → master
- QSPI data path → slave
- Flash writes / config / rollback → master
- Flash reads for slave-owned region → restricted/partitioned
- Timer tick → per-core private
- IDU / inter-core interrupt → shared signaling, but each side owns its local handler path
- DMA channels are partitioned by owner and use case, with CH0~CH3 intended for slave data plane and CH4~CH5 for master-side usage. fileciteturn2file0

Architecture rules from the repo policy:
- no SMP assumptions
- ownership must be explicit
- shared memory must have producer/consumer and lifecycle rules
- non-owner direct writes are unsafe. fileciteturn2file1

## What is easy to misunderstand
- "shared layer" does not mean both cores may freely write the same objects.
- non-owner debug shortcuts are still architecture violations.
- config truth stays on the master even if the slave caches a local applied view.

## What to inspect in code
Search:
- `owner`
- `ns_usart`
- `ns_qspi`
- `ns_qspi_xip`
- `INTER_CORE_IRQn`
- `UTB_SHARED`
- `ctrl_req_ring`
- `secondary_ready`
- `ipc_ready`

Inspect:
- `platform/hal/*`
- `platform/ipc/*`
- `application/freertos/ql25_amp_master/main.c`
- `application/freertos/ql25_amp_slave/main.c`

## Do / Don't
- Do state owner changes explicitly in reviews.
- Do keep peripheral access funneled through owner-aware wrappers.
- Don't let the master directly modify slave fast-path state structures.
- Don't let the slave directly own authoritative persistent config.

## Related skills
- `utb-hal-driver-ownership`
- `utb-ipc-and-shared-memory`
- `utb-management-repo-map`
- `utb-forwarding-repo-map`

# Skill: utb-ipc-and-shared-memory

## Purpose
Explain the repo’s intended IPC model and where to look for its implementation.

## When to use
- IPC bugs
- shared memory changes
- ring/full/overflow problems
- IDU doorbell changes
- boot/shared control changes
- request/response/snapshot/event path changes

## What this skill tells you

The repo’s IPC baseline is:
- shared memory objects
- SPSC-style ring usage
- IDU / inter-core interrupt as notify-only doorbell
- control/config request-response
- snapshots for status/statistics
- event/fault path as separate traffic class. fileciteturn2file0

Planned and/or referenced code:
- `platform/ipc/include/ipc.h`
- `platform/ipc/include/ipc_shared.h`
- `platform/ipc/src/ipc_ring.c`
- `platform/ipc/src/ipc_idu.c`
- `utb_ipc_ring.c`
- `utb_ipc_notify.c`
- `utb_ipc_init.c`
- `utb_ipc_event.c`
- `utb_ipc_snapshot.c`
- `utb_ipc_stats.c`
- optional master-only `utb_ipc_recovery.c`. fileciteturn2file0

Important repo policy:
- FreeRTOS objects are never cross-core IPC.
- shared memory requires explicit producer/consumer roles and publication ordering.
- waits across cores should be bounded and recovery-aware. fileciteturn2file1

The plan also gives a concrete shared control example containing:
- `secondary_boot_flag`
- `secondary_entry`
- `secondary_ready`
- `ipc_ready`
- `boot_error_code`
- master→slave ring
- slave→master ring. fileciteturn2file0

## What is easy to misunderstand
- core-local queue semantics are not IPC semantics.
- IDU should notify, not carry business payload.
- a "shared struct" with many writable fields is not a safe default IPC design in AMP.

## What to inspect in code
Search:
- `ipc_shared.h`
- `ipc_ring`
- `notify`
- `IDU`
- `INTER_CORE_IRQn`
- `secondary_boot_flag`
- `ctrl_req`
- `snapshot`
- `event_ring`
- `txn_id`

Also inspect:
- `application/baremetal/demo_cidu/`
- `ns_idu.h`
- `ns_idu.c`

## Do / Don't
- Do keep producer/consumer ownership separate.
- Do keep notifications bounded and explicit.
- Do account for peer-late/peer-reset scenarios.
- Don't build cross-core behavior on raw RTOS handles.
- Don't let both cores write the same ring indices.
- Don't conflate request/response traffic with high-frequency stats snapshots.

## Related skills
- `utb-boot-bringup`
- `utb-core-ownership`
- `utb-debug-workflow`

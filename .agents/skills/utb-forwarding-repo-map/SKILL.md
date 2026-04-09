# Skill: utb-forwarding-repo-map

## Purpose
Provide a repo-oriented map for slave fast path / data plane work.

## When to use
- packet path changes
- fragmentation/reassembly work
- ISR→task handoff in the data plane
- ILM hot-path placement
- throughput or packet-loss debugging

## What this skill tells you

The slave image is the data-plane owner and is intentionally constrained:
- fast path on slave
- hot path targeted at ILM
- static allocation preference
- small task count
- low-overhead ISR→task wakeups
- no casual logging, heap, or extra abstractions. fileciteturn2file0turn2file1

Concrete repo targets:
- `application/freertos/ql25_amp_slave/main.c`
- slave `FreeRTOSConfig.h`
- `platform/hal/ql25_mac.c`
- `platform/ipc/*`
- possible fast-path functions marked into `.slave_ilm_text`. fileciteturn2file0

The plan gives a sample slave task shape:
- IPC service task
- heartbeat/snapshot task
- data-plane task
with fast-path wakeup using task notification from ISR. fileciteturn2file0

The design baseline says the forwarding plane owns:
- protocol fast path
- fragmentation / reassembly
- forwarding state machine
- high-frequency status and counters
- direct data-path HAL use with minimal management-plane intrusion. fileciteturn0file0

## What is easy to misunderstand
- slave "application" code is still architecture-sensitive platform/data-plane code, not user-level business logic.
- moving convenience helpers into the slave image can break ILM/size/stack budgets quickly.
- packet path correctness depends on ownership and bounded timing, not just functional parsing.

## What to inspect in code
Search:
- `ql25_amp_slave`
- `task_rx_fast`
- `task_data_plane`
- `mac_rx_poll`
- `mac_tx_submit`
- `slave_ilm_text`
- `reasm`
- `fragment`
- `notify`
- `ulTaskNotifyTake`

Inspect:
- slave `main.c`
- slave `FreeRTOSConfig.h`
- `platform/hal/`
- `platform/ipc/`
- linker script and map file if size or placement matters

## Do / Don't
- Do keep ISR minimal and hand off work explicitly.
- Do track stack and image-size impact on the slave.
- Do prefer fixed pools and descriptor/ring models.
- Don't add per-packet malloc/free.
- Don't move management-plane behavior into fast path.
- Don't add chatty logs or heavy formatting to slave packet paths.

## Related skills
- `utb-memory-placement`
- `utb-hal-driver-ownership`
- `utb-freertos-osal`
- `utb-debug-workflow`

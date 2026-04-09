# Skill: utb-management-repo-map

## Purpose
Provide a repo-oriented map for master-side management plane, config application, health monitoring, and recovery coordination.

## When to use
- CLI/management ingress work
- config apply / rollback changes
- master-side IPC control requests
- watchdog/recovery work
- diagnostics / status aggregation changes

## What this skill tells you

The master image owns:
- startup orchestration
- management ingress
- config authority
- status/statistics collection from slave
- watchdog / recovery / degraded-boot decisions. fileciteturn2file0turn0file0

Concrete repo targets:
- `application/freertos/ql25_amp_master/main.c`
- master `FreeRTOSConfig.h`
- `platform/api/*`
- `platform/storage/*`
- `platform/ipc/*`
- `platform/sysf/*`
- `platform/boot/*`. fileciteturn2file0

The plan’s sample master shape includes:
- management task
- IPC receive/service task
- watchdog/heartbeat or recovery task. fileciteturn2file0

The repo policy also emphasizes:
- no broad architectural shortcuts
- explicit error handling
- bounded waits
- clear debugability and counters. fileciteturn2file1

## What is easy to misunderstand
- master is not allowed to directly patch slave private fast-path structures.
- config apply should flow through platform/API and IPC contracts, not ad-hoc shared-memory writes.
- recovery logic is asymmetric; master and slave are not peers with equal reset authority.

## What to inspect in code
Search:
- `ql25_amp_master`
- `utb_platform_cfg_`
- `utb_platform_status_`
- `watchdog`
- `heartbeat`
- `rollback`
- `storage`
- `ctrl_request_sync`
- `ipc_recv`
- `management`

Inspect:
- master `main.c`
- `platform/api/include/*`
- `platform/storage/*`
- `platform/sysf/*`
- master `FreeRTOSConfig.h`

## Do / Don't
- Do keep config truth and rollback logic on the master.
- Do use IPC/API boundaries for control-plane interactions.
- Do preserve observable error codes and state transitions.
- Don't embed slave-private knowledge into management code when an API contract exists.
- Don't bypass platform storage interfaces for raw flash writes unless explicitly needed.

## Related skills
- `utb-core-ownership`
- `utb-ipc-and-shared-memory`
- `utb-freertos-osal`
- `utb-develop-workflow`

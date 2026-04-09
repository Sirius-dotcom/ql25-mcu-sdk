# Skill: utb-boot-bringup

## Purpose
Guide any work touching startup, secondary-core release, boot metadata, readiness, or early runtime.

## When to use
- startup bugs
- slave release / boot flag changes
- shared-boot-metadata changes
- bringup logging / phase tracking
- secondary ILM loading changes
- `system_ns.c`, startup ASM, or boot platform code changes

## What this skill tells you

Boot is architectural and must follow the staged contract:
1. master early init
2. shared region / IPC base init
3. locate slave image and load slave ILM
4. `fence.i` after ILM load
5. `fence` before publishing boot metadata
6. write `secondary_boot_flag = UTB_SECONDARY_BOOT_MAGIC`
7. release slave reset
8. slave validates boot flag, builds local runtime, reports `secondary_ready`
9. master confirms `ipc_ready`, then allows management attach. fileciteturn2file0turn2file1

The development plan is explicit that this sequence must not be simplified into a single ready flag or informal polling shortcut. fileciteturn2file0

Repository-intended code locations:
- `platform/boot/include/utb_boot.h`
- `platform/boot/src/utb_boot.c`
- `SoC/ns_core*/Common/Source/GCC/system_ns.c`
- `startup_ns_core0.S`
- `startup_ns_core1.S`. fileciteturn2file0

Expected shared control structure includes:
- `secondary_boot_flag`
- `secondary_entry`
- `secondary_ready`
- `ipc_ready`
- `boot_error_code`
- master→slave and slave→master rings. fileciteturn2file0

## What is easy to misunderstand
- No D-cache does **not** remove the need for `fence` / `fence.i`.
- Slave "released" is not the same as slave "ready".
- Boot failure handling is expected to distinguish fatal vs degraded/forwarding-disabled behavior. fileciteturn2file0

## What to inspect in code
Search:
- `UTB_SECONDARY_BOOT_MAGIC`
- `secondary_boot_flag`
- `secondary_ready`
- `ipc_ready`
- `utb_boot_`
- `fence`
- `fence.i`
- `startup_ns_core0`
- `SystemInit`

## Do / Don't
- Do preserve all barrier points.
- Do keep boot metadata publishing explicit and ordered.
- Do verify slave sees only fully initialized shared state.
- Don't merge stage transitions casually.
- Don't add application-layer logic into early startup unless required.
- Don't change release semantics without checking both cores.

## Related skills
- `utb-memory-placement`
- `utb-ipc-and-shared-memory`
- `utb-debug-workflow`

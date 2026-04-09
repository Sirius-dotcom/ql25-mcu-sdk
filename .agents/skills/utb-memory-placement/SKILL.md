# Skill: utb-memory-placement

## Purpose
Explain where code and data should live, and what size/resource rules matter.

## When to use
- linker changes
- section placement
- image size growth
- adding buffers, rings, stacks, snapshots, or reassembly state
- placing hot-path code/data
- memory-budget reviews

## What this skill tells you

The plan and repo policy assume strong placement discipline:
- slave ILM is for hot-path code and small hot state only
- slave DLM is for hot data like descriptor rings and reassembly tables
- external SRAM banks are partitioned by role
- shared SRAM usage must be explicitly budgeted
- flash XIP is the long-term runtime shape; ILM mode is for bringup/debug. fileciteturn2file0turn2file1

Typical planning assumptions:
- `.slave_ilm_text` for hot code
- slave image size and ILM usage must be watched aggressively
- avoid putting packet buffers, log buffers, management state, or general heap in ILM
- use DLM / external SRAM for control data and larger buffers. fileciteturn2file0turn2file1

The plan also proposes concrete SRAM partitioning and shared control placement, e.g. using one bank such as `SRAM3` for `UTB_SHARED`, while keeping data-plane-heavy banks for slave-side buffering/reassembly. fileciteturn2file0

## What is easy to misunderstand
- "hardware has region X" does not mean it is the right place for arbitrary objects.
- a successful link is not proof of correct placement.
- stack, heap, and shared-memory growth can silently break slave constraints even before functional failures appear.

## What to inspect in code
Search:
- `.slave_ilm_text`
- `.slave_dlm_data`
- `__attribute__((section(`
- `UTB_SHARED_BASE`
- `SRAM`
- `HEAPSZ`
- `STACKSZ`
- `configTOTAL_HEAP_SIZE`
- `uxTaskGetStackHighWaterMark`

Inspect:
- linker scripts under `SoC/ns_core*/Board/fpga_eval/Source/GCC/`
- `FreeRTOSConfig.h` files
- memory map docs under `doc/ql25-soc/`

## Do / Don't
- Do mention stack, heap, ILM, DLM, and shared-SRAM impact in reviews.
- Do prefer fixed pools and explicit section placement for slave fast path.
- Don't add large local arrays to slave tasks.
- Don't grow shared structs casually.
- Don't put debug/log formatting buffers into hot regions.

## Related skills
- `utb-sdk-layout-and-build`
- `utb-forwarding-repo-map`
- `utb-freertos-osal`

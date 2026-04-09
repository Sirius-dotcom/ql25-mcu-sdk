# Skill: utb-freertos-osal

## Purpose
Explain how FreeRTOS is used in this repo and when OSAL must or must not be used.

## When to use
- task/queue/mutex/timer changes
- `FreeRTOSConfig.h` changes
- OSAL implementation changes
- master/slave task design questions
- ISR-to-task handoff questions

## What this skill tells you

The repository policy is strict about execution context correctness, bounded waits, low task count, and explicit ownership. fileciteturn2file1

The development plan expects:
- `platform/osal/include/osal.h`
- `platform/osal/src/osal_freertos.c`
- business/platform layers should use OSAL
- raw FreeRTOS APIs should be mostly confined to OSAL and limited bringup/demo code. fileciteturn2file0

But the repo is not symmetric:
- master image can include OSAL and dynamic allocation where accepted by that image policy
- slave image is heavily constrained and may intentionally avoid OSAL if OSAL would introduce dynamic allocation or unnecessary code size. The plan explicitly documents a slave configuration with static-only allocation, zero heap, and omission of OSAL to fit the image/resource envelope. fileciteturn2file0

Important consequences:
- do not force OSAL into slave fast-path code if it breaks the static-allocation model
- keep master and slave `FreeRTOSConfig.h` separate
- check `configSUPPORT_DYNAMIC_ALLOCATION`, `configSUPPORT_STATIC_ALLOCATION`, `configMAX_PRIORITIES`, software timer settings, and stack sizes per core. fileciteturn2file0

## What is easy to misunderstand
- "platform layers must use OSAL" is a general rule, not an excuse to break the slave image budget.
- FreeRTOS objects are per-core; they are not cross-core IPC primitives.
- task notification is often the lightest correct handoff for ISR→task on a single core. fileciteturn2file0turn2file1

## What to inspect in code
Search:
- `osal_`
- `xTaskCreateStatic`
- `xTaskNotify`
- `FromISR`
- `configSUPPORT_DYNAMIC_ALLOCATION`
- `configSUPPORT_STATIC_ALLOCATION`
- `configUSE_TIMERS`
- `uxTaskGetStackHighWaterMark`

Inspect:
- `platform/osal/include/osal.h`
- `platform/osal/src/osal_freertos.c`
- `application/freertos/ql25_amp_master/FreeRTOSConfig.h`
- `application/freertos/ql25_amp_slave/FreeRTOSConfig.h`
- master/slave `main.c`

## Do / Don't
- Do choose the lightest synchronization primitive that matches the problem.
- Do justify task priority and stack changes.
- Do keep slave task count and stack sizes tight.
- Don't share one RTOS config across cores.
- Don't use blocking APIs from ISR.
- Don't use FreeRTOS queue/semaphore handles across cores.

## Related skills
- `utb-ipc-and-shared-memory`
- `utb-management-repo-map`
- `utb-forwarding-repo-map`

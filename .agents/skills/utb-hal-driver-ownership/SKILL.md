# Skill: utb-hal-driver-ownership

## Purpose
Guide modifications to driver wrappers, peripheral access policy, IRQ/DMA ownership, and HAL semantics.

## When to use
- peripheral changes
- DMA changes
- IRQ handler registration
- adding or modifying `platform/hal/*`
- wrapping vendor SDK drivers

## What this skill tells you

The repository is expected to **reuse SDK drivers** from `SoC/ns_core*/Common/Source/Drivers/` and apply project-specific architecture in `platform/hal/`. This is a wrapper-and-policy model, not a rewrite-from-scratch model. fileciteturn2file0

Planned wrappers include:
- `hal_usart.h` / `ql25_usart.c`
- `hal_qspi.h` / `ql25_qspi.c`
- `hal_flash.h` / `ql25_flash.c`
- `hal_timer.h` / `ql25_timer.c`
- `hal_mac.h` / `ql25_mac.c`
- `hal_cache.h`. fileciteturn2file0

The repo policy requires:
- explicit owner definition
- ISR context correctness
- DMA ownership/coherency reasoning
- bounded waits and explicit recovery
- no unsafe cross-core driver access. fileciteturn2file1

## What is easy to misunderstand
- a vendor driver being callable from both cores does not mean the architecture allows that.
- "owner check" belongs near the access wrapper boundary, not scattered randomly through application code.
- no D-cache does not remove the need to reason about DMA lifecycle and publication ordering.

## What to inspect in code
Search:
- `platform/hal`
- `ns_usart`
- `ns_qspi`
- `ns_qspi_xip`
- `ns_udma`
- `ns_idu`
- `ECLIC_`
- `FromISR`
- `DMA`
- `timeout`

Inspect:
- `platform/hal/include/*`
- `platform/hal/src/*`
- `SoC/ns_core*/Common/Source/Drivers/*`

## Do / Don't
- Do keep wrappers explicit and "boring."
- Do define sync/async/ISR-safe semantics clearly.
- Do keep ISR work minimal and hand off to task context where appropriate.
- Don't let applications grab raw vendor driver internals if a platform wrapper exists.
- Don't hide ownership transfer or DMA lifecycle in ambiguous helper functions.

## Related skills
- `utb-core-ownership`
- `utb-freertos-osal`
- `utb-forwarding-repo-map`

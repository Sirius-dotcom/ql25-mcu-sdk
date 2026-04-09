# Skill: utb-sdk-layout-and-build

## Purpose
Explain the actual SDK-based build model and the most relevant source locations for code changes.

## When to use
- build issues
- linker / section placement issues
- "where is startup defined?"
- "which linker script is in effect?"
- "how do master and slave differ?"

## What this skill tells you

The project builds on Nuclei's `ql25-mcu-sdk`, with **dual independent SoC builds**:
- master: `SOC=ns_core1`
- slave: `SOC=ns_core0` fileciteturn2file0

Typical build modes:
- `DOWNLOAD=ilm` for FPGA bringup and debug
- `DOWNLOAD=flashxip0` for target runtime shape
- do not treat `DOWNLOAD=ilm` as final product mode. fileciteturn2file0

Important SDK locations:
- `SoC/ns_core*/build.mk`
- `SoC/ns_core*/Common/Include/ns.h`
- `SoC/ns_core*/Common/Include/ns_core*_feature.h`
- `SoC/ns_core*/Common/Source/GCC/startup_ns_core*.S`
- `SoC/ns_core*/Common/Source/GCC/system_ns.c`
- `SoC/ns_core*/Board/fpga_eval/Source/GCC/gcc_ns_core*_ilm.ld`
- `SoC/ns_core*/Board/fpga_eval/Source/GCC/gcc_ns_core*_flashxip0.ld`. fileciteturn2file0

The plan expects a linker extension for slave hot-path placement:
- `.slave_ilm_text` mapped with VMA in slave ILM and LMA in flash
- explicit size assertion to keep hot-path section within budget. fileciteturn2file0

## What is easy to misunderstand
- This is not SMP and not a shared `make SMP=2` flow.
- Master and slave can use different FreeRTOS configs, heap policies, and app composition.
- Build errors involving section placement may stem from the selected `DOWNLOAD=*` linker script, not just code size. fileciteturn2file0

## What to inspect in code
Search:
- `build.mk`
- `LINKER_SCRIPT`
- `DOWNLOAD=`
- `gcc_ns_core0_flashxip0.ld`
- `gcc_ns_core1_flashxip0.ld`
- `.slave_ilm_text`
- `ASSERT(SIZEOF`

## Do / Don't
- Do treat master/slave ELF outputs as separate artifacts.
- Do review the active linker script before changing memory placement.
- Don't assume a section used in `ilm` mode is valid in `flashxip0` mode without checking VMA/LMA rules.
- Don't share one FreeRTOS config across cores.

## Related skills
- `utb-boot-bringup`
- `utb-memory-placement`
- `utb-freertos-osal`

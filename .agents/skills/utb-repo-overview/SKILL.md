# Skill: utb-repo-overview

## Purpose
Build a repo-aware mental model before reading code. Use this when the task is broad, architectural, or when you are unsure where implementation should live.

## When to use
- first contact with the repository
- "where should this code go?"
- "which directory owns this behavior?"
- architecture review
- repo restructuring questions

## What this skill tells you

This repo is structured around the vendor QL25 SDK plus project-added platform layers. The default code organization is:

- `SoC/ns_core0/` — slave core SDK SoC support
- `SoC/ns_core1/` — master core SDK SoC support
- `platform/boot/` — AMP boot orchestration
- `platform/hal/` — owner-aware wrappers over SDK drivers
- `platform/osal/` — FreeRTOS abstraction boundary
- `platform/ipc/` — shared memory ring + IDU notification
- `platform/storage/` — flash/config/rollback service
- `platform/sysf/` — system service framework
- `platform/api/` — upper-layer platform contract
- `application/freertos/ql25_amp_master/` — master runtime image
- `application/freertos/ql25_amp_slave/` — slave runtime image
- `application/baremetal/ql25_amp_hello/` — early AMP bringup validation app. fileciteturn2file0

The repo follows a layered model:
- L1 startup/bringup
- L2 BSP/HAL/driver
- L3 OSAL
- L4 shared base services (IPC/storage/sysf)
- L5 platform service API
- L6 business/application bearing
- L7 external ingress boundary. fileciteturn2file0

## What is easy to misunderstand
- `SoC/ns_core*` are not "project-local implementation only"; they are vendor baseline plus extension points.
- `platform/` is where project policy and architecture are meant to land.
- master/slave are separate app directories because the two images have materially different constraints. fileciteturn2file0

## What to inspect in code
Search:
- `platform/`
- `SoC/ns_core0/`
- `SoC/ns_core1/`
- `ql25_amp_master`
- `ql25_amp_slave`
- `DOWNLOAD=ilm`
- `DOWNLOAD=flashxip0`

## Do / Don't
- Do preserve the dual-image repo model.
- Do add policy wrappers in `platform/hal` rather than modifying every vendor driver call site.
- Don't collapse master/slave into one generic application without explicit request.
- Don't put architecture glue directly into app `main.c` if it belongs in `platform/`.

## Related skills
- `utb-sdk-layout-and-build`
- `utb-core-ownership`
- `utb-develop-workflow`

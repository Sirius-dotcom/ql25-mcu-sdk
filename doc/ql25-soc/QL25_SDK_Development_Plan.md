# QL25 芯片 Nuclei SDK 移植与平台开发计划

> 文档版本：v3.0
> 适用对象：平台开发、BSP 开发、驱动开发、集成验证
> 设计基线：`doc/ql25-soc/UTB_AMP_Platform_Design_Outline.md`
> 目标芯片：QL25，双核 AMP，xunlian v5.5.0（N300 系列）
> 当前验证环境：芯莱定制 SDK（`ql25-mcu-sdk`）+ FPGA 验证环境
> SDK 基线：芯莱提供的 ql25-mcu-sdk（双核分离构建，`SoC/ns_core0` + `SoC/ns_core1`）

---

## 1. 文档目标

本文档将 `UTB_AMP_Platform_Design_Outline.md` 落成一份可以直接执行的开发计划，重点回答以下问题：

- 先做什么，后做什么。
- 每个阶段依赖哪些硬件事实。
- 每个阶段的代码落点在哪里。
- 每个阶段的完成判据是什么。
- 哪些地方必须严格遵守设计文档，不能边做边改。

本计划默认遵循以下前提：

- BootROM 只启动主核（Core 1）。
- 主核完成早期初始化后释放从核（Core 0）。
- 从核 ILM 48 KB（硬件容量，64k地址为未来扩展，实际ram 只有48K），**热点代码区控制在 48 KB 以内**，留余量给未来迭代。
- 运行形态以 `Flash XIP + 从核 ILM 热搬移` 为目标形态。
- `DOWNLOAD=ilm` 仅作为 FPGA bring-up 和调试模式，**不作为最终运行模式**。
- SDK 已提供完整外设驱动（USART、QSPI、I2C、LGPIO、UDMA 等），**平台层复用 SDK 已有驱动，不从零开发**。
- 验收门槛（设计文档冻结值）：
  - 从核转发吞吐 `>= 25 Mbps`
  - `31` 节点场景系统内存占用 `<= 192 KB`
  - 目标工况 `0` 丢包

---

## 2. 实施原则

### 2.1 架构原则

- 采用双核 AMP，不走 SMP 共享调度模型。
- 主核（ns_core1）负责启动总控、管理面、配置 authority、恢复裁决。
- 从核（ns_core0）负责转发面、快路径、热点状态与热点缓存。
- 共享对象必须使用明确 owner 模型，禁止双方同时写。
- 上层业务通过平台接口访问底层能力，禁止跨层直连内部实现。

### 2.2 启动原则

- 启动链路必须按设计文档第 5 章执行，**不能简化为单个 ready 标志轮询**。
- 主核必须先完成共享区、IPC 基础区、从核镜像准备，再释放从核。
- 从核启动前后必须满足共享内存可见性要求（fence barrier）。
- 若共享区、从核镜像、IPC 基础能力任一失败，必须进入 `fatal` 或 `forwarding-disabled`，不得假装系统可运行。

### 2.3 内存与缓存原则

- 从核 ILM 64 KB 硬件容量，**热点代码区控制在 48 KB 以内**，只放快路径热点代码、热点状态、关键描述符缓存。
- 不允许把从核 ILM 当作通用 heap。
- 共享内存、descriptor ring、DMA buffer 的 cache 策略必须显式定义。
- **模块禁止直接散落内联汇编做 cache 维护**，统一通过平台宏封装。
- 两核均无 D-Cache，I-Cache only（Core 0: 16KB, Core 1: 32KB），fence/fence.i 仍为必须。

### 2.4 SDK 使用原则

芯莱定制 SDK（`ql25-mcu-sdk`）已提供双核独立 SoC 目录和完整外设驱动，**不是从通用 evalsoc 模板移植，而是直接基于定制 SDK 开发**。推荐复用：

| SDK 路径 | 用途 |
|---|---|
| `SoC/ns_core0/` | 从核 SoC 移植、启动、链接脚本、外设驱动 |
| `SoC/ns_core1/` | 主核 SoC 移植、启动、链接脚本、外设驱动 |
| `SoC/ns_core0/Common/Source/Drivers/` | 已有外设驱动（USART、QSPI、I2C、LGPIO、UDMA 等）|
| `application/baremetal/smphello/` | 双核 hart 路由与栈初始化参考 |
| `application/baremetal/demo_cidu/` | 核间中断（IDU）实现参考 |
| `application/freertos/demo/` | FreeRTOS 单核端口参考 |
| `NMSIS/Core/Include/core_feature_eclic.h` | ECLIC 中断接口 |

---

## 3. 计划输出物

| 模块 | 路径 | 说明 |
|---|---|---|
| SoC/BSP 基础（已由 SDK 提供）| `SoC/ns_core0/` `SoC/ns_core1/` | 芯莱已提供，按需扩展 |
| 启动编排 | `platform/boot/` | AMP bring-up 逻辑，不暴露给业务层 |
| 硬件抽象封装 | `platform/hal/` | 在 SDK 驱动基础上封装 owner 模型与平台宏 |
| RTOS 适配 | `platform/osal/` | FreeRTOS 隔离层 |
| 核间通信 | `platform/ipc/` | 共享内存 ring + IDU 核间中断 |
| 存储服务 | `platform/storage/` | Flash 分区、配置持久化、回滚 |
| 系统服务框架 | `platform/sysf/` | 生命周期、事件、统计、告警 |
| 平台接口 | `platform/api/` | 对上统一能力契约层 |
| 联调工程 | `application/freertos/ql25_amp_demo/` | AMP 平台联调与验收 |
| 调试配置 | `SoC/ns_core0/Board/fpga_eval/openocd_ns_core0.cfg` | OpenOCD 双核调试 |
| 验收文档 | `doc/ql25-soc/QL25_*_Report.md` | Bringup/IPC/性能/内存预算报告 |

---

## 4. SDK 现有目录结构与扩展规划

SDK 已提供完整的双核 SoC 支持，`platform/` 目录需要新建。

```
ql25-mcu-sdk/
│
│  ════════════════════════════════════════════════════
│  【SDK 已有】芯莱定制 SoC 支持（双核分离构建）
│  ════════════════════════════════════════════════════
│
├── SoC/
│   ├── ns_core0/                              # 从核（Core 0）SoC 支持
│   │   ├── build.mk                           # CORE=core0_n300f, BOARD=fpga_eval
│   │   ├── ns_core0_feature.mk                # CPU 特性导出（xunlian v5.5.0, rv32imafc）
│   │   ├── Common/
│   │   │   ├── Include/
│   │   │   │   ├── ns.h                       # SoC 外设基址、IRQn 枚举（128 个中断）
│   │   │   │   ├── ns_core0_feature.h         # CPU 特性宏（ILM/DLM 地址、I-Cache 等）
│   │   │   │   ├── system_ns.h                # SystemInit 声明
│   │   │   │   ├── ns_sdk_soc.h               # SDK SoC 头文件
│   │   │   │   ├── ns_usart.h                 # USART 驱动头文件
│   │   │   │   ├── ns_qspi.h                  # QSPI 驱动头文件
│   │   │   │   ├── ns_i2c.h                   # I2C 驱动头文件
│   │   │   │   ├── ns_lgpio.h                 # LGPIO 驱动头文件
│   │   │   │   ├── ns_udma.h                  # UDMA 驱动头文件
│   │   │   │   ├── ns_idu.h                   # IDU（核间中断）头文件
│   │   │   │   ├── ns_advanced_timer.h        # 高级定时器
│   │   │   │   ├── ns_basic_timer.h           # 基础定时器
│   │   │   │   ├── ns_wwdg.h                  # 窗口看门狗
│   │   │   │   ├── ns_rtc.h                   # RTC
│   │   │   │   ├── ns_iomux.h                 # IO 复用
│   │   │   │   ├── ns_xkan.h                  # XKAN 加速器
│   │   │   │   └── ns_pa2m_udma.h             # PA2M DMA
│   │   │   └── Source/
│   │   │       ├── Drivers/                   # 【已有】完整外设驱动实现
│   │   │       │   ├── ns_usart.c
│   │   │       │   ├── ns_qspi.c / ns_qspi_xip.c
│   │   │       │   ├── ns_i2c.c
│   │   │       │   ├── ns_lgpio.c
│   │   │       │   ├── ns_udma.c
│   │   │       │   ├── ns_idu.c
│   │   │       │   ├── ns_advanced_timer.c
│   │   │       │   ├── ns_basic_timer.c
│   │   │       │   ├── ns_wwdg.c
│   │   │       │   ├── ns_rtc.c
│   │   │       │   ├── ns_iomux.c
│   │   │       │   ├── ns_xkan.c
│   │   │       │   ├── ns_pa2m_udma.c
│   │   │       │   └── ns_misc.c
│   │   │       ├── GCC/
│   │   │       │   ├── startup_ns_core0.S     # 从核启动（hart 路由 + 栈准备）
│   │   │       │   ├── intexc_ns_core0.S      # M-mode 异常/中断入口
│   │   │       │   ├── intexc_ns_core0_s.S    # S-mode（如有）
│   │   │       │   └── system_ns.c            # 时钟/ECLIC 早期初始化
│   │   │       └── Stubs/newlib/              # newlib 桩函数
│   │   └── Board/fpga_eval/
│   │       ├── Include/
│   │       │   ├── fpga_eval.h                # 板级资源定义
│   │       │   └── ns_sdk_hal.h               # 调试 USART 配置
│   │       ├── Source/
│   │       │   └── ns_eval.c                  # 板级初始化
│   │       └── Source/GCC/
│   │           ├── gcc_ns_core0_ilm.ld        # ILM 模式（FPGA bring-up）
│   │           ├── gcc_ns_core0_flash.ld      # Flash 模式
│   │           └── gcc_ns_core0_flashxip0.ld  # Flash XIP 目标模式
│   │
│   └── ns_core1/                              # 主核（Core 1）— 结构与 ns_core0 对称
│       ├── build.mk                           # CORE=core1_n300f
│       ├── ns_core0_feature.mk
│       ├── Common/...                         # 同 ns_core0 结构
│       └── Board/fpga_eval/
│           └── Source/GCC/
│               ├── gcc_ns_core0_ilm.ld
│               ├── gcc_ns_core0_flash.ld
│               └── gcc_ns_core0_flashxip0.ld
│
│  ════════════════════════════════════════════════════
│  【SDK 已有】OS / NMSIS / Components / Middleware
│  ════════════════════════════════════════════════════
│
├── OS/
│   ├── FreeRTOS/                              # FreeRTOS 内核与 RISC-V 移植层
│   ├── RTThread/                              # RT-Thread（备选 RTOS）
│   └── UCOSII/                                # uCOS-II（备选 RTOS）
│
├── NMSIS/                                     # NMSIS 核心库（ECLIC、Timer、PMP、Cache 等）
│
├── Components/
│   ├── NOR_FLASH/                             # NOR Flash 驱动组件
│   └── SPI_SDCard/                            # SPI SD 卡组件
│
├── Middleware/
│   └── FATFS/                                 # FAT 文件系统
│
│  ════════════════════════════════════════════════════
│  【需新建】平台层（AMP bring-up、HAL 封装、IPC、OSAL）
│  ════════════════════════════════════════════════════
│
├── platform/
│   ├── boot/                                  # 【Layer 1 扩展】AMP bring-up 编排
│   │   ├── include/
│   │   │   ├── utb_boot.h                     # 对外接口（仅 system_ns.c 调用）
│   │   │   └── utb_boot_internal.h            # 内部结构，不对业务层暴露
│   │   └── src/
│   │       └── utb_boot.c                     # 9 步 bring-up 链路实现
│   │
│   ├── hal/                                   # 【Layer 2 扩展】在 SDK 驱动上封装 owner 模型
│   │   ├── include/
│   │   │   ├── hal_usart.h                    # USART 抽象（基于 SDK ns_usart）
│   │   │   ├── hal_qspi.h                     # QSPI 抽象（基于 SDK ns_qspi）
│   │   │   ├── hal_flash.h                    # Flash 抽象（基于 SDK ns_qspi_xip）
│   │   │   ├── hal_timer.h                    # Timer 抽象（基于 SDK ns_basic_timer/ns_advanced_timer）
│   │   │   ├── hal_mac.h                      # MAC/PHY 快路径子集（从核 owner，ISR-safe）
│   │   │   ├── hal_irq.h                      # 中断使能/屏蔽抽象
│   │   │   └── hal_cache.h                    # 统一 cache 维护宏（UTB_CACHE_* / UTB_DSB / UTB_ISB）
│   │   └── src/
│   │       ├── ql25_usart.c                   # 封装 ns_usart 驱动，加 owner 检查
│   │       ├── ql25_qspi.c                    # 封装 ns_qspi 驱动
│   │       ├── ql25_flash.c                   # 主核独占，封装 ns_qspi_xip
│   │       ├── ql25_timer.c                   # 封装 ns_basic_timer / ns_advanced_timer
│   │       └── ql25_mac.c                     # 从核快路径专用
│   │
│   ├── osal/                                  # 【Layer 3】RTOS 差异隔离
│   │   ├── include/
│   │   │   └── osal.h                         # task/queue/mutex/sem/timer/time
│   │   └── src/
│   │       └── osal_freertos.c                # FreeRTOS 实现
│   │
│   ├── ipc/                                   # 【Layer 4】核间通信
│   │   ├── include/
│   │   │   ├── ipc.h                          # 对上接口：ipc_send / ipc_recv / ipc_notify
│   │   │   └── ipc_shared.h                   # 共享内存布局
│   │   └── src/
│   │       ├── ipc_ring.c                     # ring 读写 + fence + 核间中断触发
│   │       └── ipc_idu.c                      # IDU 核间中断注册与清除
│   │
│   ├── storage/                               # 【Layer 4】存储服务
│   │   ├── include/
│   │   │   └── storage.h
│   │   └── src/
│   │       └── storage.c                      # 调用 hal_flash，主核 owner
│   │
│   ├── sysf/                                  # 【Layer 4】系统服务框架
│   │   ├── include/
│   │   │   └── sysf.h
│   │   └── src/
│   │       └── sysf.c
│   │
│   └── api/                                   # 【Layer 5】平台服务接口
│       └── include/
│           ├── utb_platform_ctrl.h
│           ├── utb_platform_cfg.h
│           ├── utb_platform_status.h
│           ├── utb_platform_stats.h
│           └── utb_platform_alarm.h
│
│  ════════════════════════════════════════════════════
│  【需新建】AMP 联调工程
│  ════════════════════════════════════════════════════
│
└── application/
    ├── baremetal/
    │   └── ql25_amp_hello/                    # Phase 2 验证：双核各自打印 hartid + bring-up stage
    └── freertos/
        ├── ql25_amp_master/                   # 【已实现】主核管理面（SOC=ns_core1 构建）
        │   ├── main.c                         # 3 任务：IPC 服务 + 心跳/恢复 + 管理面
        │   ├── FreeRTOSConfig.h               # 动态分配、12KB heap、软件定时器
        │   └── Makefile                       # 含 OSAL + IPC
        └── ql25_amp_slave/                    # 【已实现】从核数据面（SOC=ns_core0 构建）
            ├── main.c                         # 3 任务：IPC 服务 + 心跳/快照 + 数据面(占位)
            ├── FreeRTOSConfig.h               # 纯静态分配、零堆、激进裁剪
            └── Makefile                       # HEAPSZ=0、-Os、不含 OSAL
```

**层次与目录对应速查表：**

| 设计文档层次 | 目录位置 | 说明 |
|---|---|---|
| L1 启动与引导层 | `SoC/ns_core1/.../startup_ns_core1.S`（主核）`SoC/ns_core0/.../startup_ns_core0.S`（从核）`system_ns.c` `platform/boot/` | SDK 已有启动入口 + 新建 AMP bring-up 编排 |
| L2 BSP/HAL/Driver | `SoC/ns_core0/` + `SoC/ns_core1/` + `platform/hal/` | SDK 已有驱动 + 新建 owner 封装层 |
| L3 OSAL | `platform/osal/` | FreeRTOS 隔离，与 HAL 不同维度 |
| L4 共享基础服务 | `platform/ipc/` `platform/storage/` `platform/sysf/` | 不与业务混写；IPC ring 单 producer |
| L5 平台服务接口 | `platform/api/include/` | 对上统一契约，屏蔽底层实现细节 |
| L6 业务承载层 | `application/freertos/ql25_amp_master/` + `ql25_amp_slave/` | 独立双 app 目录，主核管理面 / 从核快路径 |
| L7 对外接口层 | 主核 `main.c` 中 USART 接收框架 + 从核 `main.c` 中数据面入口 | 外部 MCU 接入边界，只做路由分发 |

---

## 5. 先决条件冻结（Phase 0 前置）

**以下硬件事实已由芯莱定制 SDK 确认。SDK 代码中已包含具体地址和配置，可直接提取。**

| 项目 | 已确认内容（从 SDK 提取）|
|---|---|
| Hart ID | 主核 hartid 通过 `HartID_Info_Typedef` 结构区分 `core_id` 位；`BOOT_HARTID=0` |
| CPU 型号 | **xunlian v5.5.0（N300 系列），rv32imafc，单精度 FPU，双发射**；无 D-Cache；PMP 16 项；ECLIC 128 个中断，`CFG_CLICINTCTLBITS=3` |
| 从核本地存储 | **Core 0 ILM 64KB** @ `0x08000000`；**Core 0 DLM 32KB** @ `0x08010000`；I-Cache 16KB |
| 主核本地存储 | **Core 1 ILM 64KB** @ `0x09000000`；**Core 1 DLM 32KB** @ `0x09010000`；I-Cache 32KB |（只是预留地址空间，没有实际ILM，DLM）
| 外部 SRAM | **4 banks 共 384KB**：SRAM0 128KB @ `0x30000000`，SRAM1 128KB @ `0x30020000`，SRAM2 64KB @ `0x30040000`，SRAM3 64KB @ `0x30060000` |
| 共享内存 | 从外部 SRAM 中划分（建议 SRAM2 64KB 或 SRAM3 64KB 作为共享区）；cacheable 属性需确认 |
| Flash | **XIP 基址 `0x20000000`**，256MB 地址空间；总大小待确认（设计目标 2MB，1MB 用于升级回滚）|
| 中断 | ECLIC 128 个外部中断（两核相同）；**核间中断 `INTER_CORE_IRQn = 19`**（SDK 已定义）；IDU 基址 `0x10010000` |
| 外设 | **USART x5**（`0x10030000`~`0x10034000`）、**QSPI x4**（XIP0 + QSPI1~3）、**I2C x2**、**LGPIO x4**、**Advanced Timer x4**、**Basic Timer x8**、**UDMA x1（8 DMA 通道）**、**WWDG x2**、**RTC x3**、**XKAN x2**、**IOMUX** |
| 时钟 | 主频待确认（设计目标 200 MHz）；Timer/外设时钟源由 `system_ns.c` 配置 |
| BootROM | 从核释放方式待确认；SDK 使用 `BOOT_HARTID` 区分，但 AMP 模式下从核启动机制需与硬件团队对齐 |
| 调试 | OpenOCD 配置已提供：`openocd_ns_core0.cfg`、`openocd_ns_core1.cfg`（独立目标）；`openocd_ns_core0_all.cfg`（全芯片） |

### SDK 确认的核间对比

| 项目 | Core 0（从核，ns_core0）| Core 1（主核，ns_core1）|
|---|---|---|
| CPU | xunlian v5.5.0, rv32imafc | xunlian v5.5.0, rv32imafc |
| FPU | 单精度 | 单精度 |
| 双发射 | Y | Y |
| I-Cache | **16 KB**（ICACHE_ADDR_WIDTH=14）| **32 KB**（ICACHE_ADDR_WIDTH=15）|
| D-Cache | 无 | 无 |
| ILM | **64 KB** @ `0x08000000` | **64 KB** @ `0x09000000` |
| DLM | **32 KB** @ `0x08010000` | **32 KB** @ `0x09010000` |
| PMP | 16 项 | 16 项 |
| ECLIC | 128 个中断 | 128 个中断 |
| Slave Port | Y（外部可访问 ILM/DLM）| Y（外部可访问 ILM/DLM）|
| 外设访问 | 共享外设总线 `0x10000000` | 共享外设总线 `0x10000000` |

> **与设计文档差异说明：** 设计文档原始假设"主核无 ILM/DLM"、"从核 ILM 48KB 无 DLM"、"从核 I-Cache 16KB"。SDK 确认两核均有 64KB ILM + 32KB DLM，且 Core 1 I-Cache 为 32KB（大于 Core 0 的 16KB）。实际只是预留地址空间，主核没有ILM，DLM，只有从核core0 有48K ILM。

**Phase 0 交付物：**
- `doc/ql25-soc/QL25_Hardware_Facts.md`（基于 SDK 提取，补充待确认项）
- `doc/ql25-soc/QL25_Memory_Map.md`（含外部 SRAM 分配方案 + Flash 分区草案）
- `doc/ql25-soc/QL25_Boot_Contract.md`

---

## 6. 总体阶段划分

```
Phase 0  硬件事实冻结（大部分已由 SDK 确认）
Phase 1  SoC/BSP 扩展与 AMP 适配       ← 平台底座（在 SDK 基础上扩展）
Phase 2  AMP 启动链路与 bring-up        ← 平台底座
Phase 3  OSAL、IPC 与基础平台层         ← 平台底座
Phase 4  外设 HAL 封装与 owner 落地     ← 业务承载前提（复用 SDK 驱动）
Phase 5  从核转发面与主核管理面联调      ← 业务承载前提
Phase 6  集成、验证与验收               ← 验收闭环，不能省
```

> 不要反过来从业务代码倒逼平台实现。

---

## 7. Phase 0：硬件事实冻结

### 已由 SDK 确认的事实

- 两核 ILM/DLM 基址与大小（见第 5 章表格）。
- CPU 特性（`ns_core0_feature.h` / `ns_core1_feature.h`）。
- 外设基址与 IRQ 编号（`ns.h`）。
- 外部 SRAM 4 banks 共 384KB。
- Flash XIP 基址 `0x20000000`。
- 核间中断 `INTER_CORE_IRQn = 19`，IDU 基址 `0x10010000`。
- OpenOCD 调试配置。

### 仍需确认的事实

- 外部 SRAM 的 cacheable 属性（是否 non-cacheable）。
- 从核释放机制（寄存器地址、入口配置方式）。
- Flash 实际容量与分区方案。
- 外部 SRAM 在 AMP 中的分配方案（哪些 bank 给主核 / 从核 / 共享）。
- 系统主频（设计目标 200 MHz，SDK 中未固定）。

### 完成判据

- 所有待确认项全部落实。
- 外部 SRAM 分配方案确定（建议方案：SRAM0 给从核数据面、SRAM1 给从核重组缓存、SRAM2 给主核、SRAM3 给共享区/IPC）。
- Flash 分区草案已确认。

### 风险

若 SRAM 分配方案、从核释放方式和 Flash 分区未冻结，后续代码大概率返工。

---

## 8. Phase 1：SoC/BSP 扩展与 AMP 适配

**目标：** 基于 SDK 已有 SoC 支持，主核单核能编译、下载、调试，USART 正常输出，ECLIC 中断可用。

> SDK 已经提供了完整的 `SoC/ns_core0/` 和 `SoC/ns_core1/`，本阶段重点是验证 SDK 可用性并为 AMP 扩展做准备。

### Step 1.1 — 构建验证

SDK 已提供 `build.mk`，关键配置：

```makefile
# SoC/ns_core0/build.mk（已有）
BOARD ?= fpga_eval
override CORE := core0_n300f           # N300F：单精度 FPU；ARCH=rv32imafc，ABI=ilp32f
# DOWNLOAD 由命令行指定：ilm（调试）、flash、flashxip0（目标模式）
SMP ?=                                 # AMP 模式下不使用 SMP
BOOT_HARTID ?= 0
HEAPSZ ?= 2K
STACKSZ ?= 2K

# 链接脚本自动选择
LINKER_SCRIPT ?= $(NUCLEI_SDK_SOC_BOARD)/Source/GCC/gcc_ns_core0_$(DOWNLOAD).ld
```

> **注意**：SDK 的双核模型是**分离构建**（主核用 `SOC=ns_core1`，从核用 `SOC=ns_core0`），不是 SMP=2 单构建。AMP 工程需要分别编译两个 ELF。

### Step 1.2 — CPU 特性确认

SDK 提供的 `ns_core0_feature.h` 已包含完整特性定义：

```c
// ns_core0_feature.h（已有，关键项摘录）
#define CFG_CPU_NAME "xunlian"
#define CFG_CPU_VERSION "v5.5.0"
#define CFG_HAS_PMP
#define CFG_PMP_ENTRY_NUM 16
#define CFG_HAS_ICACHE
#define CFG_ICACHE_ADDR_WIDTH 14          // Core 0 I-Cache 16KB
#define CFG_HAS_ILM
#define CFG_ILM_BASE_ADDR 0x08000000
#define CFG_ILM_ADDR_WIDTH 16             // 64KB
#define CFG_HAS_DLM
#define CFG_DLM_BASE_ADDR 0x08010000
#define CFG_DLM_ADDR_WIDTH 15             // 32KB
#define CFG_IRQ_NUM 128
#define CFG_CLICINTCTLBITS 3
#define CFG_HAS_DUAL_ISSUE
#define CFG_FPU_SINGLE
#define CFG_HAS_ECLIC
#define CFG_TMR_PRIVATE 1                 // 每核私有 Timer
#define CFG_HAS_SLAVE_PORT                // 外部可访问 ILM/DLM
```

### Step 1.3 — 内存布局（已由 SDK 链接脚本定义）

ILM 模式（`gcc_ns_core0_ilm.ld`）：
```ld
MEMORY {
  ilm (rxa!w) : ORIGIN = 0x08000000, LENGTH = 64K   /* Core 0 ILM */
  ram (wxa!r) : ORIGIN = 0x08010000, LENGTH = 32K   /* Core 0 DLM */
}
```

Flash XIP 模式（`gcc_ns_core0_flashxip0.ld`）：
```ld
MEMORY {
  flash (rxa!w) : ORIGIN = 0x20000000, LENGTH = 256M  /* Flash XIP */
  ram (wxa!r)   : ORIGIN = 0x08010000, LENGTH = 32K   /* Core 0 DLM */
}
```

**AMP 扩展需求**：从核 Flash XIP 链接脚本需增加 `.slave_ilm_text` 段，将热路径代码 LMA 放在 Flash、VMA 映射到从核 ILM：

```ld
/* gcc_ns_core0_flashxip0.ld 扩展（需新增段）*/
MEMORY {
  flash (rxa!w) : ORIGIN = 0x20000000, LENGTH = 256M
  ilm   (rwx)  : ORIGIN = 0x08000000, LENGTH = 64K   /* Core 0 ILM（从核）*/
  ram   (wxa!r) : ORIGIN = 0x08010000, LENGTH = 32K   /* Core 0 DLM（从核）*/
}

SECTIONS {
  /* 从核热路径代码：VMA=从核 ILM，LMA=Flash（主核启动时搬运） */
  .slave_ilm_text : {
      _slave_ilm_start = .;
      *(.slave_ilm_text .slave_ilm_text.*)
      _slave_ilm_end = .;
  } >ilm AT>flash
  _slave_ilm_lma_start = LOADADDR(.slave_ilm_text);

  ASSERT(SIZEOF(.slave_ilm_text) <= 49152, "ILM hot-path sections exceed 48 KB budget")
}
```

### Step 1.4 — 启动文件职责边界

SDK 已提供 `startup_ns_core1.S`（主核）和 `startup_ns_core0.S`（从核），负责：hart 路由、栈准备、跳转 `_init` → `SystemInit` → `main`。

**AMP 扩展**：从核的 `startup_ns_core0.S` 需要修改，增加等待 boot flag 的逻辑（或在 `system_ns.c` 中增加）：

```c
/* SoC/ns_core0/Common/Source/system_ns.c 扩展 */
void SystemInit(void) {
    /* 从核：等待主核释放 */
    utb_boot_slave_wait_release();
    /* 从核早期初始化 */
    utb_boot_slave_early_init();
}
```

### 完成判据

- 主核 `SOC=ns_core1 DOWNLOAD=ilm` 模式下 hello world 正常运行，USART 输出。
- 从核 `SOC=ns_core0 DOWNLOAD=ilm` 模式下单核可独立运行。
- 主核 Timer/ECLIC 中断可响应。
- OpenOCD `openocd_ns_core1.cfg` 可连接主核并单步调试。

---

## 9. Phase 2：AMP 启动链路与 bring-up

**目标：** 在不引入业务逻辑的前提下，跑通设计文档第 5 章定义的完整 AMP bring-up 链路。

### 9.1 启动链路必须包含的 9 个阶段

```
1. 主核早期初始化（时钟、栈、异常向量）— SDK system_ns.c 已有
2. 共享区与 IPC 基础区建立（外部 SRAM 初始化）
3. 从核镜像定位与 ILM 热段搬运（从 Flash 搬到 Core 0 ILM @ 0x08000000）
4. UTB_CACHE_CLEAN_INV(共享区) + UTB_DSB()
5. 写入从核 boot metadata 与 boot flag + UTB_DSB()
6. 写 SoC 复位控制寄存器，release secondary + UTB_DSB()
7. 从核 early runtime 启动（cache invalidate → 验证 boot flag → 建立本地 runtime）
8. 从核上报 secondary_ready
9. 主核判定 IPC alive，allow 管理面 attach
```

> 不允许把以上链路简化为单个 `g_slave_core_ready` 标志，否则在有 I-Cache 的场景下可能出现从核读到旧数据的随机故障。

### 9.2 共享控制块

```c
/* platform/ipc/include/ipc_shared.h */

#define UTB_SECONDARY_BOOT_MAGIC  0xA55A5AA5UL
#define IPC_RING_DEPTH            16
#define IPC_MSG_PAYLOAD_MAX       64

typedef struct {
    uint8_t  payload[IPC_MSG_PAYLOAD_MAX];
} ipc_msg_t;

typedef struct {
    volatile uint32_t head;
    volatile uint32_t tail;
    ipc_msg_t         ring[IPC_RING_DEPTH];
} ipc_ring_t;

/* 共享区头部固定布局（放在外部 SRAM 共享 bank，如 SRAM3 @ 0x30060000）*/
typedef struct {
    volatile uint32_t secondary_boot_flag;  /* 主核写 MAGIC 释放从核 */
    volatile uint32_t secondary_entry;      /* 从核入口地址（如需动态配置）*/
    volatile uint32_t secondary_ready;      /* 从核写 1 表示 early runtime 就绪 */
    volatile uint32_t ipc_ready;            /* 双核 IPC 可用标志 */
    volatile uint32_t boot_error_code;      /* 启动失败错误码 */
    uint32_t          _pad[3];
    ipc_ring_t        master_to_slave;      /* 主→从（主核 producer） */
    ipc_ring_t        slave_to_master;      /* 从→主（从核 producer） */
} utb_shared_ctrl_t;

/* 共享区基址（使用外部 SRAM bank，需 Phase 0 确定具体 bank）*/
#define UTB_SHARED_BASE   SRAM3_MEM_BASE   /* 0x30060000，64KB */
#define UTB_SHARED  ((utb_shared_ctrl_t *)UTB_SHARED_BASE)
```

### 9.3 cache 一致性宏（platform/hal/include/hal_cache.h）

```c
/* 所有模块统一使用以下宏，禁止自行内联汇编做 cache 维护 */

/* QL25 无 D-Cache，CACHE 系列宏为空操作；fence 必须保留 */
#define UTB_CACHE_CLEAN(addr, size)      /* no-op：无 D-Cache */
#define UTB_CACHE_INVALIDATE(addr, size) /* no-op：无 D-Cache */
#define UTB_CACHE_CLEAN_INV(addr, size)  /* no-op：无 D-Cache */
#define UTB_DSB()   __asm volatile("fence" ::: "memory")     /* 必须：保证写入顺序对另一核可见 */
#define UTB_ISB()   __asm volatile("fence.i" ::: "memory")   /* 必须：ILM 加载后刷新 I-Cache */
```

**使用规则：**
- 主核发布共享对象前：`UTB_CACHE_CLEAN_INV` → `UTB_DSB()` → 更新可见标志
- 从核消费共享对象前：`UTB_CACHE_INVALIDATE` → `UTB_DSB()` → 读取内容
- ILM 加载完成后：`UTB_DSB()` → `UTB_ISB()`（刷新指令 cache 后才可跳转执行）

### 9.4 `platform/boot/utb_boot.h` 接口

```c
int  utb_boot_locate_secondary_image(uint32_t *lma, uint32_t *size);
int  utb_boot_load_secondary_ilm(uint32_t lma, uint32_t vma, uint32_t size);
int  utb_boot_shared_region_init(void);
int  utb_boot_release_secondary(uint32_t entry);
int  utb_boot_wait_secondary_ready(uint32_t timeout_ms);
int  utb_boot_check_ipc_alive(void);
utb_boot_stage_t utb_boot_get_stage(void);

/* 从核侧 */
int  utb_boot_slave_wait_release(void);
int  utb_boot_slave_early_init(void);
```

### 9.5 核间中断实现

SDK 已提供 IDU 驱动（`ns_idu.h` / `ns_idu.c`）和 `INTER_CORE_IRQn = 19`。IPC 通知直接使用：

```c
/* platform/ipc/src/ipc_idu.c */
#include "ns_idu.h"

void utb_ipc_notify_peer(void) {
    /* 通过 IDU 触发对端核的 INTER_CORE_IRQn */
    /* 具体 API 参考 SDK ns_idu 驱动 */
}

void utb_ipc_idu_init(void) {
    /* 注册 INTER_CORE_IRQn 中断处理函数 */
    ECLIC_SetShvIRQ(INTER_CORE_IRQn, ECLIC_NON_VECTOR_INTERRUPT);
    ECLIC_SetLevelIRQ(INTER_CORE_IRQn, 2);
    ECLIC_EnableIRQ(INTER_CORE_IRQn);
}
```

### 9.6 验证工程

新增 `application/baremetal/ql25_amp_hello/`，验证内容：

- 主核打印 hartid + 各 bring-up stage（通过 USART0 输出）
- 主核通过外部访问接口写入从核 ILM（Core 0 ILM @ `0x08000000`，Slave Port = Y）
- 主核 release 从核
- 从核验证 boot flag 后打印 hartid
- bring-up 失败时输出明确错误码，区分 `fatal` 与 `forwarding-disabled`

### 完成判据

- 两核可稳定启动，从核 ready 可稳定上报。
- bring-up 失败路径可观察到明确错误码。
- OpenOCD 可分别连接、单步观察两个核（使用 `openocd_ns_core0_all.cfg`）。

---

## 10. Phase 3：OSAL、IPC 与基础平台层

**目标：** 双核 FreeRTOS 独立调度，IPC ping/pong 可靠往返，platform/api 对上可用。

### 10.1 OSAL 接口

```c
/* platform/osal/include/osal.h */

typedef void* osal_task_t;
typedef void* osal_queue_t;
typedef void* osal_mutex_t;
typedef void* osal_sem_t;

osal_task_t  osal_task_create(const char *name, void(*fn)(void*), void *arg,
                               uint32_t stack_words, uint32_t prio);
osal_queue_t osal_queue_create(uint32_t depth, uint32_t item_size);
int          osal_queue_send(osal_queue_t q, const void *item, uint32_t timeout_ms);
int          osal_queue_recv(osal_queue_t q, void *item, uint32_t timeout_ms);
osal_mutex_t osal_mutex_create(void);
void         osal_mutex_lock(osal_mutex_t m);
void         osal_mutex_unlock(osal_mutex_t m);
uint32_t     osal_time_ms(void);
void         osal_delay_ms(uint32_t ms);
```

- 业务层与平台服务层只能调用 OSAL，禁止直接调用 FreeRTOS 私有 API。
- FreeRTOS 原生 API 只允许出现在 `osal_freertos.c` 和 bring-up/demo 验证代码中。
- SDK 同时提供 RT-Thread 和 uCOS-II，但初期以 FreeRTOS 为主。

### 10.2 IPC 实现要点

- `master_to_slave` 和 `slave_to_master` 各为单向 ring，单 producer，无需 mutex。
- ring 控制字（head/tail）与 payload 分别规划 cache 策略。
- 对外只暴露 `ipc_send()` / `ipc_recv()` / `ipc_notify()`，内部 ring 细节不泄漏。
- 核间通知通过 IDU（`INTER_CORE_IRQn = 19`，参考 SDK `ns_idu` 驱动）实现。

### 10.3 主从核 FreeRTOS 配置分离

```c
/* FreeRTOSConfig_master.h */
#define configTOTAL_HEAP_SIZE   (24 * 1024)   /* 主核 heap 在 DLM 或外部 SRAM */
#define configTICK_RATE_HZ      1000
#define configMAX_PRIORITIES    9

/* FreeRTOSConfig_slave.h */
#define configTOTAL_HEAP_SIZE   (24 * 1024)   /* 从核 heap 在 DLM 或外部 SRAM，不进 ILM */
#define configTICK_RATE_HZ      1000
#define configMAX_PRIORITIES    9
#define configSUPPORT_DYNAMIC_ALLOCATION  0   /* 从核禁止动态分配 */
```

> **注意**：两核各有 32KB DLM，FreeRTOS heap 可放在 DLM 中，大块数据缓冲放在外部 SRAM。这比设计文档原始假设（共享 SRAM 16KB）宽裕很多。

### 完成判据

- 主从核 FreeRTOS 各自独立调度。
- IPC ping/pong 在 1 万次循环中无死锁、无错序、无假满。
- `secondary_ready` / `ipc_ready` 可查询。

---

## 11. Phase 4：外设 HAL 封装与 owner 落地

**目标：** 在 SDK 已有驱动（`ns_usart`、`ns_qspi`、`ns_udma` 等）基础上，封装 owner 模型，杜绝非 owner 核直接操作外设。

### 外设 Owner 基线

| 外设 / 资源 | SDK 驱动 | Owner | 说明 |
|---|---|---|---|
| USART0 管理口 | `ns_usart.c` | 主核 | CLI、日志、诊断入口 |
| QSPI 数据口 | `ns_qspi.c` | 从核 | 快路径数据收发 |
| MAC + PHY | 待适配 | 从核 | 转发面直接使用，ISR-safe |
| Flash（QSPI XIP） | `ns_qspi_xip.c` | 主核（写）| 配置持久化、升级、回滚控制 |
| Flash 读访问 | `ns_qspi_xip.c` | 分区管理 | 从核按约定区域只读 |
| Timer（Tick）| `ns_basic_timer.c` | 各核私有 | SDK 已定义 `CFG_TMR_PRIVATE=1` |
| WWDG | `ns_wwdg.c` | 主核 | 系统级恢复裁决 |
| UDMA | `ns_udma.c` / `ns_pa2m_udma.c` | 按通道分配 | CH0~3 从核数据面，CH4~5 主核，CH6~7 预留 |
| LGPIO | `ns_lgpio.c` | 按引脚分配 | 状态指示、调试辅助 |
| I2C | `ns_i2c.c` | 主核 | 传感器/管理总线 |

### HAL 封装层设计

```c
/* platform/hal/include/hal_usart.h — 封装 SDK ns_usart 驱动 */

#include "ns_usart.h"

/* owner 检查宏：主核 build 中编译 USART HAL，从核 build 中不链接 */
int  hal_usart_init(USART_TypeDef *usart, uint32_t baudrate);
int  hal_usart_send(USART_TypeDef *usart, const uint8_t *data, uint32_t len);
int  hal_usart_recv(USART_TypeDef *usart, uint8_t *data, uint32_t len, uint32_t timeout_ms);
```

### HAL MAC 快路径接口（ISR-safe，低开销）

```c
/* platform/hal/include/hal_mac.h */

typedef struct {
    uint8_t  *buf;
    uint16_t  len;
    uint16_t  flags;
} mac_pkt_t;

int  mac_rx_poll(mac_pkt_t *pkt);          /* 从 RX 队列取帧，ISR-safe */
int  mac_tx_submit(const mac_pkt_t *pkt);  /* 提交到 TX 队列，ISR-safe */
void mac_rx_irq_enable(void);
void mac_tx_done_irq_enable(void);
```

### 完成判据

- USART 管理口可稳定收发。
- MAC/PHY 基础收发可通（回环测试）。
- Timer 双核独立运行，互不干扰。
- Flash 分区读写策略跑通。

---

## 12. Phase 5：从核转发面与主核管理面联调

**目标：** 在 `ql25_amp_demo` 工程中，验证 AMP 双核 FreeRTOS 可以承载转发面和管理面。

### 12.1 构建方式

AMP 工程采用**独立 app 目录**方案（详见附录三），分别构建：

```bash
# 从核 ELF（整个 bin 加载到 ILM，<= 48KB）
make SOC=ns_core0 BOARD=fpga_eval DOWNLOAD=ilm \
     PROGRAM=application/freertos/ql25_amp_slave clean all

# 主核 ELF（FPGA bringup 用 ilm，目标模式用 flashxip0）
make SOC=ns_core1 BOARD=fpga_eval DOWNLOAD=ilm \
     PROGRAM=application/freertos/ql25_amp_master clean all
```

### 12.2 从核转发面任务结构

```c
/* application/freertos/ql25_amp_demo/slave_main.c（SOC=ns_core0 构建）*/

void slave_main(void) {
    mac_rx_irq_enable();
    mac_tx_done_irq_enable();

    xTaskCreate(task_rx_fast,  "rx",  512, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(task_ipc_recv, "ipc", 256, NULL, 2, NULL);
    xTaskCreate(task_stats,    "sta", 256, NULL, 1, NULL);
    vTaskStartScheduler();
}

static void task_rx_fast(void *arg) {
    mac_pkt_t pkt;
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);   /* 等 MAC RX 中断唤醒 */
        while (mac_rx_poll(&pkt) == 0)
            utb_forward_packet(&pkt);              /* 热路径，放 .slave_ilm_text */
    }
}
```

### 12.3 从核 ILM 使用规则

**建议放入 ILM 的对象（标记 `__attribute__((section(".slave_ilm_text")))`）：**
- 快路径热点函数（分片、重组、转发决策）
- 重组热点状态机
- 关键 descriptor 辅助对象
- 高频访问小型查找表

**禁止放入 ILM 的对象：**
- 大块 packet buffer
- 日志缓冲
- 管理状态
- 低频配置对象
- 通用 heap

> ILM 硬件容量 64KB，热点代码预算控制在 `<= 48KB`（75%），建议 bring-up 阶段先控制在 `<= 38KB`，留余量给后续迭代。

### 12.4 主核管理面任务结构

```c
/* application/freertos/ql25_amp_demo/master_main.c（SOC=ns_core1 构建）*/

int main(void) {
    SystemInit();   /* 内含 platform/boot/ 完整 bring-up 编排 */

    xTaskCreate(task_usart_mgmt, "mgmt", 1024, NULL, 3, NULL);
    xTaskCreate(task_ipc_recv,   "ipc",  512,  NULL, 4, NULL);
    xTaskCreate(task_watchdog,   "wdt",  256,  NULL, 5, NULL);
    vTaskStartScheduler();
}

static void task_usart_mgmt(void *arg) {
    utb_sys_req_t req;
    for (;;) {
        hal_usart_recv(USART0, (uint8_t *)&req, sizeof(req), portMAX_DELAY);
        /* 参数校验 */
        utb_platform_cfg_set(req.cmd, req.payload, req.len);
        utb_platform_cfg_apply(200);   /* 通过 L5 接口下发，不直接操作 IPC */
    }
}
```

### 完成判据

- 主核 CLI 命令可下发到从核并生效。
- 从核状态、统计、告警可回报主核。
- 从核快路径基本收发跑通。
- 配置 apply/rollback 路径闭环可验证。

---

## 13. Phase 6：集成、验证与验收

基于设计文档第 15 章建立验收闭环。

### A. 启动与稳定性

- 主核稳定进入启动入口，可释放从核。
- 从核 ready 报告稳定，`ipc_ready` 稳定建立。
- bring-up 失败可观察到明确错误码。

### B. 功能

- 主核管理面可用，从核转发面可用。
- IPC 双向通信稳定（1 万次无错）。
- Flash 配置持久化、回滚路径可用。

### C. 性能

- 从核转发吞吐 `>= 25 Mbps`（用 mtime 计数器测量）。
- 目标工况 `0` 丢包。
- 关键路径观测点可采集：吞吐、队列深度、缓存峰值、错误计数。

### D. 内存

- `31` 节点场景总内存占用 `<= 192 KB`（`size slave.elf` + 运行时 heap 统计）。
- 从核 ILM 实际使用率 `<= 75%`（48KB / 64KB 硬件容量）。
- 从核 heap 不进入 ILM。

### E. 栈与优先级

- 所有关键任务做 `uxTaskGetStackHighWaterMark()` 观测。
- 满载场景下所有任务高水位保留不低于 20%。
- ISR 优先级满足 FreeRTOS 端口要求（允许调用 `FromISR` API 的 ISR 必须满足优先级约束）。

### 验收输出物

- `doc/ql25-soc/QL25_Bringup_Checklist.md`
- `doc/ql25-soc/QL25_IPC_Test_Report.md`
- `doc/ql25-soc/QL25_Performance_Report.md`
- `doc/ql25-soc/QL25_Memory_Budget_Report.md`

---

## 14. 关键技术约束（强制执行）

1. 禁止把 AMP 启动简化为单个共享标志，必须走完整 9 步链路。
2. 禁止把 `DOWNLOAD=ilm` 当成最终产品运行模式，目标形态为 `flashxip0`。
3. 禁止把从核 ILM 当成通用内存池或 heap（热点代码预算 ≤ 48KB / 64KB 硬件容量）。
4. 禁止共享对象双 owner 写。
5. 禁止业务层直接操作共享内存地址、ring index、cache 维护细节。
6. 禁止主核直接改写从核快路径私有状态。
7. 禁止模块自行内联汇编做 cache 维护，必须统一用平台宏。
8. 禁止在未定义 cache 策略前上线 DMA、IPC、descriptor ring 逻辑。

---

## 15. 里程碑

| 里程碑 | 完成标准 |
|---|---|
| M0 | 硬件事实冻结完成（SDK 已确认大部分，补齐剩余待确认项）|
| M1 | 主核单核 hello（`SOC=ns_core1 DOWNLOAD=ilm`），USART、ECLIC 正常 |
| M2 | AMP 9 步 bring-up 跑通，主核可释放从核，从核 ready 稳定上报 |
| M3 | 主从核 FreeRTOS 独立运行，IPC ping/pong 1 万次无错 |
| M4 | HAL 封装层落地（基于 SDK 驱动），owner 模型生效 |
| M5 | `ql25_amp_demo` 跑通（`DOWNLOAD=flashxip0`），主核管理面与从核转发面联调成功 |
| M6 | 吞吐 `>=25Mbps`、内存 `<=192KB`、0 丢包、栈高水位全部达到验收门槛 |

---

## 16. 推荐实施顺序

```
1. 冻结剩余硬件事实（外部 SRAM 分配、从核释放机制、Flash 分区）
2. 验证 SDK SoC 构建 → 主核 + 从核分别单核跑起来
3. 新建 platform/boot/ → 实现完整 9 步 AMP bring-up
4. 新建 platform/osal/ 和 platform/ipc/ → 双核 FreeRTOS + IPC（基于 IDU）
5. 新建 platform/hal/ → 在 SDK 驱动上封装 owner 模型
6. 进入转发面和管理面联调 → 最后才碰业务代码
```

---

## 17. 构建命令

```bash
# === 主核构建（SOC=ns_core1）===

# FPGA bring-up 模式（ILM，早期调试）
make SOC=ns_core1 BOARD=fpga_eval CORE=core1_n300f DOWNLOAD=ilm

# 目标运行模式（Flash XIP）
make SOC=ns_core1 BOARD=fpga_eval CORE=core1_n300f DOWNLOAD=flashxip0

# === 从核构建（SOC=ns_core0）===

# FPGA bring-up 模式
make SOC=ns_core0 BOARD=fpga_eval CORE=core0_n300f DOWNLOAD=ilm

# 目标运行模式
make SOC=ns_core0 BOARD=fpga_eval CORE=core0_n300f DOWNLOAD=flashxip0

# === 下载与调试 ===

# 主核下载
make SOC=ns_core1 BOARD=fpga_eval upload

# 从核下载
make SOC=ns_core0 BOARD=fpga_eval upload

# 双核调试（使用 _all 配置）
openocd -f SoC/ns_core0/Board/fpga_eval/openocd_ns_core0_all.cfg
```

---

## 18. 参考文件索引

| 路径 | 用途 |
|---|---|
| `doc/ql25-soc/UTB_AMP_Platform_Design_Outline.md` | **设计基线（权威文档）** |
| `SoC/ns_core0/` | 从核（Core 0）SoC 完整支持（SDK 已有）|
| `SoC/ns_core1/` | 主核（Core 1）SoC 完整支持（SDK 已有）|
| `SoC/ns_core0/Common/Include/ns.h` | 外设基址、IRQn 枚举（核间中断 IRQ=19）|
| `SoC/ns_core0/Common/Include/ns_core0_feature.h` | Core 0（从核）CPU 特性（ILM/DLM 地址、I-Cache 等）|
| `SoC/ns_core1/Common/Include/ns_core1_feature.h` | Core 1（主核）CPU 特性 |
| `SoC/ns_core0/Common/Source/Drivers/` | SDK 已有外设驱动（USART、QSPI、I2C 等）|
| `SoC/ns_core0/Common/Source/GCC/startup_ns_core0.S` | 从核启动文件 |
| `SoC/ns_core1/Common/Source/GCC/startup_ns_core1.S` | 主核启动文件 |
| `SoC/ns_core0/Board/fpga_eval/Source/GCC/gcc_ns_core0_*.ld` | 从核链接脚本 |
| `SoC/ns_core1/Board/fpga_eval/Source/GCC/gcc_ns_core1_*.ld` | 主核链接脚本 |
| `SoC/ns_core0/Board/fpga_eval/openocd_ns_core0.cfg` | 从核 OpenOCD 配置 |
| `SoC/ns_core0/Board/fpga_eval/openocd_ns_core0_all.cfg` | 全芯片 OpenOCD（双核调试）|
| `application/baremetal/smphello/` | 双核 hart 路由与 spinlock 参考 |
| `application/baremetal/demo_cidu/` | **IDU 核间中断实现参考（IPC 核间通知依赖此）** |
| `application/freertos/demo/` | FreeRTOS 单核端口参考 |
| `NMSIS/Core/Include/core_feature_eclic.h` | ECLIC 中断接口 |
| `OS/FreeRTOS/` | FreeRTOS 内核与 RISC-V 移植层 |
| `ns_core0_config.json` / `ns_core1_config.json` | 构建/测试配置 |

---

## 19. 结论

QL25 的平台开发在芯莱定制 SDK 基础上，已有完整的 SoC 支持和外设驱动，**核心工作是新建 `platform/` 层实现 AMP 编排、IPC、owner 模型和平台接口**。

**五件事做对，后续联调才有稳定基础：**

1. **启动链路做对** — 9 步完整链路，不是单标志
2. **内存与 cache 策略做对** — 共享区使用外部 SRAM，fence barrier 必须保留
3. **owner 模型做对** — 在 SDK 驱动上加 owner 封装，每个外设只有一个核能写
4. **IPC 契约做对** — 单 producer ring，boot flag 魔数验证，IDU 核间通知
5. **验收口径做对** — 吞吐、内存、丢包、栈高水位全部量化

---

## 附录 A：SDK 与设计文档差异对照表

| 项目 | 设计文档原始假设 | SDK 实际值 | 影响 |
|---|---|---|---|
| SoC 目录 | `SoC/ql25/`（单一目录）| `SoC/ns_core0/` + `SoC/ns_core1/`（分离构建）| 构建方式改为双 SOC 分别编译 |
| Board 名称 | `ql25_evb` | `fpga_eval` | 所有构建命令需更新 |
| CPU 名称 | N310 | xunlian v5.5.0（N300 系列）| CPU 型号描述更正 |
| CORE 变量 | `n300f` | `core0_n300f` / `core1_n300f` | 构建命令需带核前缀 |
| Download 模式 | `sram` / `flashxip` | `ilm` / `flashxip0` | 命令行参数更新 |
| Core 0（从核）ILM | 无（仅 BootROM 16KB）| **64 KB** @ `0x08000000` | 从核可利用 ILM 放热点转发代码 |
| Core 0（从核）DLM | 无（32KB 不可用）| **32 KB** @ `0x08010000` | 从核有独立数据区，descriptor ring 等可放此 |
| Core 1（主核）ILM | 48 KB | **64 KB** @ `0x09000000` | 主核可利用 ILM 减少外部 SRAM 依赖 |
| Core 1（主核）DLM | 无 | **32 KB** @ `0x09010000` | 主核有独立数据区 |
| Core 0（从核）I-Cache | 16 KB | **16 KB**（ADDR_WIDTH=14）| 从核 I-Cache 较小，注意热路径代码布局 |
| Core 1（主核）I-Cache | 32 KB | **32 KB**（ADDR_WIDTH=15）| 主核 I-Cache 较大 |
| IRQ 数量 | 48 个 | **128 个** | 中断资源更充裕 |
| 共享 SRAM | 16 KB 内部 | **外部 SRAM 4 banks 共 384KB**（可灵活分配）| 共享区不再紧张 |
| 核间中断 | CIDU | **IDU**（`INTER_CORE_IRQn = 19`）| 使用 SDK 的 `ns_idu` 驱动 |
| 外设命名 | UART / SPI / GPIO | **USART / QSPI / LGPIO** | 代码和文档中外设名需统一 |
| 启动文件 | `startup_ql25.S` | `startup_ns_core0.S` / `startup_ns_core1.S` | 文件名更新 |
| 链接脚本 | `gcc_ql25_*.ld` | `gcc_ns_core0_*.ld` / `gcc_ns_core1_*.ld` | 文件名更新 |
| Feature 文件 | `cpufeature.h` | `ns_core0_feature.h` / `ns_core1_feature.h` | 文件名更新 |
| 系统头文件 | `system_ql25.h` | `system_ns.h` | 文件名更新 |
| 外设驱动 | 需全部从零开发 | **SDK 已提供完整驱动** | HAL 层改为封装已有驱动而非重新实现 |
| RTOS 支持 | 仅 FreeRTOS | **FreeRTOS + RT-Thread + uCOS-II** | 有备选 RTOS |


##附录二：已实现部分

###创建完成。文件清单：


platform/osal/
├── include/
│   └── osal.h              ← 统一接口定义（26 个 API）
├── src/
│   └── osal_freertos.c     ← FreeRTOS V10.3.1 实现
└── build.mk                ← SDK 构建集成

application/freertos/osal_demo/
├── main.c                  ← 单核验证 demo
├── FreeRTOSConfig.h        ← 验证用配置（tick=1000Hz，heap=16KB）
└── Makefile                ← 构建入口
OSAL 接口总览
类别	API	封装对象
Task	create / delete / delay_ms / stack_watermark	xTaskCreate / vTaskDelete / vTaskDelay / uxTaskGetStackHighWaterMark
Queue	create / delete / send / recv / waiting	xQueueCreate / xQueueSendToBack / xQueueReceive / uxQueueMessagesWaiting
Mutex	create / delete / lock / unlock	xSemaphoreCreateMutex / xSemaphoreTake / xSemaphoreGive
Semaphore	create / delete / wait / post	xSemaphoreCreateCounting / xSemaphoreTake / xSemaphoreGive
Timer	create / delete / start / stop / reset	xTimerCreate / xTimerStart / xTimerStop / xTimerReset
Time	time_ms / tick_get	xTaskGetTickCount
Critical	critical_enter / critical_exit	taskENTER_CRITICAL / taskEXIT_CRITICAL
Scheduler	scheduler_start / scheduler_is_running	vTaskStartScheduler / xTaskGetSchedulerState
关键设计决策
超时统一 ms — 内部通过 pdMS_TO_TICKS() 转换，OSAL_WAIT_FOREVER → portMAX_DELAY
ISR 不走 OSAL — ISR 中直接调 FreeRTOS FromISR 系列 API，避免额外开销
FreeRTOS 头文件隔离 — 只在 osal_freertos.c 中 #include "FreeRTOS.h"，业务层只 #include "osal.h"
不用动态类型识别 — handle 都是 void* 强转，零开销
验证 demo 构建命令

# 从核单核验证
make SOC=ns_core0 BOARD=fpga_eval DOWNLOAD=ilm RTOS=FreeRTOS \
     APP=application/freertos/osal_demo

# 或主核单核验证
make SOC=ns_core1 BOARD=fpga_eval DOWNLOAD=ilm RTOS=FreeRTOS \
     APP=application/freertos/osal_demo
Demo 会依次验证 task、queue（50 条消息收发）、mutex（两任务竞争 1000 次递增）、semaphore（30 次同步）、timer（50ms 周期回调计数）、stack watermark，最终打印 PASS/FAIL 汇总。

---

## 附录三：AMP 双核应用工程与 platform 条件编译（已实现）

### 1. 架构决策

基于以下硬约束，AMP 联调工程采用**独立 app 目录 + platform 条件编译**方案：

| 约束 | 影响 |
|---|---|
| 从核整个 bin 加载到 ILM，**<= 48KB** | 极端裁剪，禁用堆、printf、OSAL |
| 主从核职责天然不同 | 单 app 双构建会导致大量 `#if` 分支，维护成本高 |
| 启动链路不同 | 主核 9 步 bring-up，从核等待释放后 init |
| FreeRTOS 配置差异大 | 从核纯静态分配 vs 主核动态分配 |

**方案选择理由：**

- **(B) 两个独立 app 目录** 而非单 app 双构建 — 主从核镜像是两个独立交付物，生命周期不同，大小审计独立
- **platform 按 `$(SOC)` 条件编译** 而非物理拆目录 — 模块边界尚未稳定，先保持简单；后期如某模块长期成双分支再考虑物理拆

### 2. 文件清单

```
application/freertos/
├── ql25_amp_master/                    # 主核管理面应用
│   ├── Makefile                        # SOC=ns_core1, RTOS=FreeRTOS, 动态分配
│   ├── FreeRTOSConfig.h                # 12KB heap, 软件定时器, 宽松配置
│   └── main.c                          # 3 任务：IPC 服务 + 心跳/恢复 + 管理面
│
└── ql25_amp_slave/                     # 从核数据面应用
    ├── Makefile                        # SOC=ns_core0, HEAPSZ=0, -Os, 不含 OSAL
    ├── FreeRTOSConfig.h                # 纯静态分配, 禁堆/禁定时器, 激进裁剪
    └── main.c                          # 3 任务：IPC 服务 + 心跳/快照 + 数据面(占位)

platform/ipc/
└── build.mk                            # 按 SOC 条件编译，ns_core0 排除 recovery.c
```

### 3. 构建命令

```bash
# 从核（整个 bin 加载到 ILM，<= 48KB）
make SOC=ns_core0 BOARD=fpga_eval DOWNLOAD=ilm \
     PROGRAM=application/freertos/ql25_amp_slave clean all

# 主核（FPGA bringup 用 ilm，目标模式用 flashxip0）
make SOC=ns_core1 BOARD=fpga_eval DOWNLOAD=ilm \
     PROGRAM=application/freertos/ql25_amp_master clean all
```

### 4. platform/ipc/build.mk 条件编译

从原来的 `C_SRCDIRS`（整个目录）改为 `C_SRCS`（逐文件列举），按 `$(SOC)` 排除从核不需要的模块：

```makefile
# 公共源文件（双核共用）
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_ring.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_notify.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_init.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_event.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_snapshot.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_stats.c

# 恢复模块：仅主核编译
ifneq ($(SOC),ns_core0)
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_recovery.c
endif
```

IPC 层按核裁剪明细：

| 文件 | ns_core0（从核） | ns_core1（主核） | 说明 |
|---|---|---|---|
| `utb_ipc_ring.c` | ✓ | ✓ | SPSC ring 底层（双核共用） |
| `utb_ipc_notify.c` | ✓ | ✓ | IDU 门铃（双核共用） |
| `utb_ipc_init.c` | ✓ | ✓ | 初始化 + ctrl 通道 + 同步请求 |
| `utb_ipc_event.c` | ✓（producer） | ✓（consumer） | 事件 ring |
| `utb_ipc_snapshot.c` | ✓（writer） | ✓（reader） | seqlock 快照 |
| `utb_ipc_stats.c` | ✓ | ✓ | 诊断计数器 |
| `utb_ipc_recovery.c` | **✗** | ✓ | 重启检测 + 恢复（主核专用） |

### 5. 任务架构

```
从核 (SOC=ns_core0, DOWNLOAD=ilm)         主核 (SOC=ns_core1)
┌──────────────────────┐                   ┌──────────────────────┐
│ task_ipc_service (4) │◄── ctrl_req ────  │ task_management  (3) │
│   命令接收/响应       │─── ctrl_rsp ───► │   同步请求/快照读取    │
│                      │                   │                      │
│ task_heartbeat   (3) │─── event ──────► │ task_ipc_service (5) │
│   心跳 + 快照写       │                   │   事件分派            │
│                      │                   │                      │
│ task_data_plane  (2) │   seqlock snap    │ task_heartbeat   (4) │
│   转发（占位）        │═════════════════►│   存活检测 + 恢复      │
└──────────────────────┘                   └──────────────────────┘
```

### 6. 从核 FreeRTOS 配置要点（48KB ILM 约束）

| 配置项 | 从核值 | 主核值 | 说明 |
|---|---|---|---|
| `configSUPPORT_STATIC_ALLOCATION` | **1** | 0 | 从核全静态 |
| `configSUPPORT_DYNAMIC_ALLOCATION` | **0** | 1 | 从核禁堆 |
| `configTOTAL_HEAP_SIZE` | **0** | 12KB | 从核零堆 |
| `configUSE_TIMERS` | **0** | 1 | 省一个 timer task TCB+栈 |
| `configMINIMAL_STACK_SIZE` | **128** words | 256 words | 从核 idle 栈 512B |
| `configMAX_PRIORITIES` | **5** | 8 | 从核够用即可 |
| `configUSE_MUTEXES` | **0** | 1 | 从核不需要 |
| `configUSE_COUNTING_SEMAPHORES` | **0** | 1 | 从核不需要 |
| `HEAPSZ`（链接器） | **0** | 2K(默认) | C 运行时堆 |
| `STACKSZ`（链接器） | **1K** | 2K(默认) | C 运行时栈 |
| OSAL | **不包含** | 包含 | OSAL 使用动态分配，与从核冲突 |
| printf | **不使用** | 使用 | 从核省 ~8-12KB .text |

### 7. 从核静态内存预算

| 项目 | 大小 | 说明 |
|---|---|---|
| FreeRTOS 内核 .text | ~8-10 KB | 裁剪后（禁定时器/禁动态分配） |
| IPC 库 .text | ~3-4 KB | ring + notify + init + event + snapshot + stats |
| 应用 .text | ~2 KB | main.c + 命令分派 |
| FreeRTOS .data/.bss | ~0.5 KB | 内核全局状态 |
| 3 任务 TCB + 栈 | ~2.1 KB | IPC(768B) + HB(512B) + DP(512B) + 3×TCB(~100B) |
| Idle 任务 TCB + 栈 | ~0.6 KB | 128 words + TCB |
| IPC .data/.bss | ~0.1 KB | g_ipc_diag + 局部 static |
| 应用 .data/.bss | ~0.2 KB | s_dp_status + s_version + s_uptime_sec |
| C 运行时栈 | 1 KB | `STACKSZ=1K` |
| **合计估算** | **~18-21 KB** | **占 48KB 的 38-44%，余量充足** |

> 注：以上为估算值，实际以构建后 `.map` 文件为准。bringup 阶段目标控制在 80%（<= 38KB）以内。

### 8. 关键设计点

**门铃唤醒机制：**
- ISR 回调 `ipc_doorbell_cb()` 使用 `vTaskNotifyGiveFromISR()` 唤醒 IPC 服务 task
- IPC 服务 task 用 `ulTaskNotifyTake(pdTRUE, timeout)` 阻塞等待，50ms 兜底超时
- 这是 FreeRTOS 最轻量的 ISR→task 唤醒机制（零分配、零队列开销）

**门铃回调二次注册：**
- `utb_ipc_init()` 内部调用 `utb_ipc_notify_init(空回调)` 完成 IDU 硬件初始化
- `main()` 中在 `utb_ipc_init()` 之后二次调用 `utb_ipc_notify_init(ipc_doorbell_cb)` 注册真实回调
- 安全性：`utb_ipc_notify_init()` 仅注册函数指针，不分配资源，二次调用无副作用

**从核不含 OSAL 的原因：**
- `osal_freertos.c` 内部使用 `xTaskCreate()`、`xQueueCreate()` 等动态分配 API
- 从核 `configSUPPORT_DYNAMIC_ALLOCATION = 0`，编译 OSAL 会报错
- 从核直接调用 FreeRTOS 静态 API（`xTaskCreateStatic` 等）+ IPC API

**事件上报不设独立 task：**
- 从核事件上报（`utb_ipc_event_send()`）和快照写入作为函数调用嵌入现有任务
- 心跳 task 负责周期性快照更新，数据面 task 在异常时发送事件
- 省一个 TCB + 栈（~640B），在 48KB 约束下有意义

**主核 ctrl_rsp_ring 的双消费者问题：**
- `utb_ipc_ctrl_request_sync()` 在管理 task 中轮询 ctrl_rsp_ring 匹配 txn_id
- IPC 服务 task 也 drain ctrl_rsp_ring 清除超时残留响应
- 两者不会同时访问（sync API 在管理 task 中阻塞轮询期间，IPC task 等待门铃）
- 遗留响应被 IPC 服务 task 丢弃，不做业务处理

### 9. 后续演进路径

```
当前（bringup 阶段）：
  独立 app + platform build.mk 条件编译
  ↓
中期（模块稳定后）：
  如某模块双分支代码超过 50%，考虑物理拆 master/ slave/ common
  ↓
长期（量产阶段）：
  完整 CI 构建双 image + 联合烧录脚本 + 版本绑定
```
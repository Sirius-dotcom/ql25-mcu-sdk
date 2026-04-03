# QL25 BootROM 技术说明

## 概述

BootROM 是 QL25 SoC 主核（Core 1）上电后执行的第一段代码，固化在 16KB 片上 ROM 中。
职责单一：**将主核应用镜像加载到目标位置并跳转执行**。

从核（Core 0）的启动不由 BootROM 负责，而是由主核应用层完成。

## 硬件前提

| 项目 | 说明 |
|------|------|
| 执行核 | Core 1（主核） |
| ROM 大小 | 16KB，只读，上电可用 |
| 主核 ILM/DLM | 无（SDK 预留了地址空间但硬件不存在） |
| 临时栈 | 片上 SRAM3 顶部（0x30070000 向下生长） |
| 片上 SRAM | 4 bank 共 384KB（SRAM0~3），上电即可用，无需初始化 |
| Flash | 2MB，通过 QSPI XIP0 映射到 0x20000000 |
| 从核 Core 0 | 有 ILM 48KB + DLM 32KB，上电被 hold 在复位 |
| 串口 | USART0 @ 0x10030000，115200 8N1 |
| Boot Pin | 2 个 GPIO，上电采样，4 种启动模式 |

## 文件结构

```
platform/bootrom/
  ├── Makefile              # 独立构建，不依赖 SDK 构建系统
  ├── bootrom.ld            # 链接脚本（16KB ROM 限制 + 无 .data 断言）
  ├── include/
  │   └── bootrom.h         # 地址定义、数据结构、函数声明
  └── src/
      ├── bootrom_entry.S   # 汇编入口（向量表 + CPU 最小初始化）
      └── bootrom_main.c    # C 主逻辑（全部功能实现）
```

## 启动流程

```
上电复位
  │
  ▼
_bootrom_start (bootrom_entry.S)
  │  关中断
  │  设 SP = 0x30070000（SRAM3 顶部）
  │  配置 NMI / ECLIC / MTVEC
  │  使能 FPU / 性能计数器 / 分支预测
  │  call bootrom_main()
  │
  ▼
bootrom_main() (bootrom_main.c)
  │
  ├─ Step 1: hold Core 0 复位（MISC_CTRL5 bit0）
  ├─ Step 2: 初始化 USART0（115200 8N1）
  ├─ Step 3: 三级启动模式检测
  │    │
  │    ├─ 第一级：读 boot pin（硬件强制）
  │    │    01 → UART 下载
  │    │    10 → JTAG 调试
  │    │    11 → Reserved（兜底 UART）
  │    │    00 → 进入第二级
  │    │
  │    ├─ 第二级：UART Shell 窗口（~1-2 秒）
  │    │    按空格 → 进入 BootROM Shell（md/mw/info/go/boot）
  │    │    超时   → 进入第三级
  │    │
  │    └─ 第三级：自动检测 Flash
  │         有效镜像 → Flash 启动
  │         空片     → 自动 fallback UART 下载
  │
  ├─ Step 4: 按模式分派
  │    ├─ Flash：A/B 分区选择 → 验证 → 加载 → 跳转
  │    ├─ UART ：同步握手 → 接收镜像 → 验证 → 跳转
  │    └─ JTAG ：写标记字 → 串口提示 → WFI 等调试器
  │
  └─ Step 5: 全部失败 → fatal 死循环
```

## Boot Pin 模式

| BOOT_PIN1 | BOOT_PIN0 | 模式 | 说明 |
|-----------|-----------|------|------|
| 0 | 0 | Flash 启动 | 量产默认，内部下拉 |
| 0 | 1 | UART 下载 | 强制进入 UART 协议下载 |
| 1 | 0 | JTAG 调试 | CPU 最小初始化后停住 |
| 1 | 1 | Reserved | 安全兜底到 UART 下载 |

Boot pin 默认内部下拉 → 00 = Flash 启动。外部通过拨码开关或跳线帽上拉切换模式。

## BootROM Shell

boot pin = 00 时，BootROM 在 UART 上等待约 1~2 秒的交互窗口。
按下空格键即可进入 Shell，无需改 boot pin。

```
QL25 BootROM v1.0
Press SPACE to enter shell...

=== QL25 BootROM Shell ===
QL25>
```

| 命令 | 格式 | 功能 |
|------|------|------|
| md | `md <addr> [len]` | 内存/寄存器 dump（十六进制 + ASCII） |
| mw | `mw <addr> <val>` | 内存/寄存器写入（32-bit，写后回读验证） |
| info | `info` | 显示分区表状态 + A/B 镜像头 + CRC 在线校验 |
| go | `go <addr>` | 跳转到指定地址执行（fence + fence.i + 关中断） |
| boot | `boot flash\|uart\|jtag` | 手动选择启动模式 |
| help | `help` | 命令帮助 |

地址支持 `0x` 前缀和裸十六进制。长度默认 64 字节，单次最大 256 字节。

量产固化时将 `BOOTROM_MENU_TIMEOUT_CYCLES` 设为 0 即可禁用 Shell，消除启动延迟。

## Flash 分区布局

```
Flash 2MB (XIP @ 0x20000000)
┌────────────────────────────────────┐ 0x20000000
│ Partition Table (1KB)              │ magic="PTBL", active_slot, boot_count
├────────────────────────────────────┤ 0x20000400
│ Slot A (≈1MB - 1KB)               │ [image_header_t 64B] + [payload]
│                                    │
├────────────────────────────────────┤ 0x20100000
│ Slot B (1MB)                       │ [image_header_t 64B] + [payload]
│                                    │
└────────────────────────────────────┘ 0x20200000
```

A/B 分区启动逻辑：
1. 分区表有效 → 按 `active_slot` 字段选择主分区
2. 主分区验证失败 → 尝试备份分区
3. 分区表无效 → 默认从 Slot A 启动（首次烧录场景）
4. 两个分区都失败 → fallback 到 UART 下载

## 镜像头格式（64 字节）

```c
typedef struct {
    uint32_t magic;          /* 0x00: 0x514C3235 ("QL25") */
    uint32_t hdr_version;    /* 0x04: 头格式版本（当前 = 1） */
    uint32_t image_size;     /* 0x08: payload 大小（不含头） */
    uint32_t entry_point;    /* 0x0C: 入口地址 */
    uint32_t load_addr;      /* 0x10: 加载目标地址 */
    uint32_t image_crc32;    /* 0x14: payload CRC32 */
    uint32_t boot_mode;      /* 0x18: 0=ILM(搬运), 1=XIP(直接执行) */
    uint32_t slot_id;        /* 0x1C: 0=Slot A, 1=Slot B */
    uint32_t version;        /* 0x20: 固件版本号 */
    uint32_t hdr_crc32;      /* 0x24: 头部 CRC32（计算时本字段为 0） */
    uint8_t  reserved[24];   /* 0x28~0x3F: 预留扩展 */
} image_header_t;            /* 共 64 字节 */
```

验证流程：magic → hdr_crc32 → image_size 范围 → entry_point 非零 → payload CRC32

## UART 下载协议

波特率 115200，8N1。

```
BootROM                          Host
   │                               │
   │── "QL25BOOT\n" ──────────────>│  同步标记（循环发送，最多 100 次）
   │                               │
   │<──── 64 字节镜像头 ───────────│  image_header_t
   │                               │
   │  验证头部                      │
   │── ACK ('A') 或 NAK ('N') ───>│
   │                               │
   │<──── image_size 字节 payload ─│
   │                               │
   │  验证 CRC32                    │
   │── ACK ('A') 或 NAK ('N') ───>│
   │                               │
   │  jump to entry_point           │
```

NAK 时可重试（回到同步标记阶段）。超时未收到主机响应则返回 `BOOTROM_ERR_UART_TIMEOUT`。

## JTAG 调试模式

进入 JTAG 模式后：
- CPU 状态干净（中断关、SP 有效、FPU 已使能）
- SRAM3 起始地址写入标记字 `0x4A544147`（"JTAG"），供调试器脚本检测
- USART0 输出提示后进入 WFI 循环

调试器可以：
1. `halt` 停住 CPU
2. 通过 JTAG 写内存（SRAM / Flash / Core 0 ILM）
3. `load` ELF 文件
4. 设置 PC → `resume` 执行

## 构建

```bash
cd platform/bootrom
make                    # 构建 bootrom.elf + bootrom.bin
make size               # 查看 ROM 大小
make dump               # 反汇编
make clean
```

工具链：`riscv-nuclei-elf-gcc`，架构 rv32imafc，ABI ilp32f。
编译选项：`-Os -nostdlib -nostartfiles -ffunction-sections -fdata-sections`。

链接脚本强制约束：
- `.text + .rodata` 总大小 <= 16KB（超出则链接报错）
- `.data` 段必须为空（BootROM 在 ROM 中，不能有可写全局变量）

## ROM 空间预算

| 模块 | 估算大小 |
|------|---------|
| bootrom_entry.S（向量表 + CPU 初始化） | ~200B |
| CRC32（nibble 查表法） | ~120B |
| UART 驱动（init/putc/puts/getc/send/recv） | ~300B |
| Flash 读取 | ~60B |
| 镜像验证 + 加载 | ~350B |
| Boot pin + 模式检测 | ~200B |
| Shell 工具（hex I/O、行输入、解析） | ~450B |
| Shell 命令（md/mw/info/go） | ~1050B |
| Shell 主循环 + help | ~300B |
| A/B Flash 启动 | ~150B |
| UART 下载协议 | ~300B |
| 跳转 + fatal + main | ~250B |
| 字符串常量(.rodata) | ~850B |
| **合计** | **~4.6KB** |
| **ROM 余量** | **~11.4KB (71%)** |

## 设计约束

1. **无可写全局变量** — ROM 不可写，所有状态通过栈局部变量传递
2. **不依赖 C 库** — 无 printf / malloc / memcpy，全部自实现
3. **不依赖 SDK** — 独立构建，只引用 `riscv_encoding.h`（CSR 定义）
4. **不启动从核** — 从核启动是主核应用层的职责
5. **不开中断** — BootROM 阶段不使用中断，异常统一进入死循环

## 与主核应用的衔接

BootROM 跳转到应用前的状态：

| 项目 | 状态 |
|------|------|
| 中断 | MIE = 0，已关闭 |
| SP | 0x30070000（SRAM3 顶部，应用 startup 会重设） |
| FPU | 已使能（FS = Initial） |
| mcycle/minstret | 已使能 |
| 分支预测 | 已使能 |
| Core 0 | 保持复位 |
| USART0 | 已初始化（115200 8N1，应用可直接复用） |
| I-Cache | 已执行 fence.i |
| MTVEC | 指向 BootROM 异常处理（应用 startup 需重配） |

应用 startup（startup_ns_core1.S）需要：
1. 重设 SP 到自己的栈区域
2. 重配 MTVEC / MTVT 指向应用向量表
3. 搬运 .data、清 .bss
4. 启动 FreeRTOS scheduler
5. 在合适时机加载从核镜像到 Core 0 ILM 并释放 Core 0

---

## 硬件适配清单（仿真/固化前必须确认）

### 已从 SDK 确认的值（无需修改）

以下值已从 SDK 头文件中验证，与硬件定义一致：

| 项目 | 值 | SDK 来源 |
|------|-----|---------|
| USART0_BASE | 0x10030000 | ns.h: `USART0_BASE` |
| MISC_BASE | 0x12000000 | ns.h: `MISC_CTL_BASE` |
| MISC_CTRL5_OFS | 0xC94 | ns_misc.h: `MISC_CTRL5_OFS` |
| MISC_CTRL5 bit 定义 | BIT(0)=core0_stop, BIT(1)=core1_stop | ns_misc.h |
| QSPI_XIP0_BASE | 0x10060000 | ns.h: `QSPI_XIP0_BASE` |
| Flash XIP 基址 | 0x20000000 | ns.h: `QSPI_XIP0_XIP_BASE` |
| USART 寄存器偏移 | 0x00/0x04/0x08/0x0C/0x10/0x14/0x18 | ns_usart.h |
| 系统时钟（默认） | 8MHz | system_ns.c: `SYSTEM_CLOCK` |

### 仍需确认（阻塞仿真）

| # | 项目 | 当前值 | 位置 | 说明 |
|---|------|--------|------|------|
| 1 | **BOOTROM_BASE** | 0x00000000 | bootrom.h:24, bootrom.ld:14 | ROM 映射基址。如果主核复位向量不是 0x0，需修改 |
| 2 | **系统时钟频率** | 8MHz | bootrom_main.c:91 | SDK 默认 8MHz 是内部 RC 振荡器。BootROM 阶段 PLL 未配置，用 8MHz 应该正确。但需确认 QL25 上电默认时钟源和频率 |
| 3 | **QSPI XIP 上电是否可用** | 假设可用 | bootrom_main.c:150 | 如果需要手动初始化 QSPI 才能 XIP 读 Flash，需补充 `bootrom_qspi_init()` |

### 仍需确认（不阻塞基本仿真，影响 boot pin）

| # | 项目 | 当前值 | 位置 | 说明 |
|---|------|--------|------|------|
| 4 | **BOOT_PIN_GPIO_BASE** | 0x10050000 | bootrom.h | LGPIO0 控制器基址，需根据原理图确认 |
| 5 | **BOOT_PIN0/1_BIT** | bit 0 / bit 1 | bootrom.h | Boot pin 对应的 GPIO 位号，需根据原理图确认 |
| 6 | **LGPIO_DATAIN_OFS** | 0x00 | bootrom.h | GPIO 输入数据寄存器偏移 |
| 7 | **工具链路径** | riscv-nuclei-elf- | Makefile:14 | 需指向实际安装路径 |

### 适配步骤

1. 确认 BootROM 映射基址（#1），修改 `bootrom.h` + `bootrom.ld`
2. 确认上电默认时钟频率（#2），修改 `bootrom_main.c:91`
3. 确认 QSPI XIP 上电行为（#3），必要时补充初始化
4. 根据原理图填入 boot pin GPIO 定义（#4~#6）
5. `make` 构建，`make size` 确认不超 16KB
6. 用 `bootrom.bin` 进行 RTL 仿真或 ROM 固化

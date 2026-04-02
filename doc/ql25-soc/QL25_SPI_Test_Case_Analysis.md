# QL25 SPI/QSPI 测试用例分析与测试策略

> 版本: v1.0
> 日期: 2026-04-01
> 目的: 分析 SDK 中全部 SPI 测试用例的工作原理、外部依赖，并给出无 EDA Testbench 时的 SPI 接口验证策略

---

## 1. 测试用例总览

SDK 在 `soc_test_cases/ns_core0/SPI/` 下共有 **64 个测试用例**，覆盖 4 个 QSPI 控制器：

| 控制器 | 测试数量 | 说明 |
|--------|---------|------|
| QSPI1 | 24 | 通用 SPI 控制器，支持 master/slave |
| QSPI2 | 10 | 通用 SPI 控制器，常作为 slave 端 |
| QSPI3 | 24 | 通用 SPI 控制器，支持 master/slave |
| QSPI_XIP0 | 6 | XIP（片上执行）专用控制器 |

---

## 2. 测试分类与外部依赖

### 2.1 无外部依赖 — 寄存器测试（4 个）

| 测试路径 | simargs | 说明 |
|----------|---------|------|
| `qspi1/reg_test/` | `+TEST_TO_VALUE=3000000000` | QSPI1 寄存器读写校验 |
| `qspi2/reg_test/` | `+TEST_TO_VALUE=3000000000` | QSPI2 寄存器读写校验 |
| `qspi3/reg_test/` | `+TEST_TO_VALUE=3000000000` | QSPI3 寄存器读写校验 |
| `qspi_xip0/reg_test/` | `+TEST_TO_VALUE=3000000000` | QSPI_XIP0 寄存器读写校验 |

**特点**：纯软件读写寄存器，验证寄存器字段掩码、默认值、读写权限。**可直接运行，无需任何外部连接。**

### 2.2 需要 Testbench 内部连线 — 环回/主从测试（约 36 个）

这些测试需要 EDA Testbench 把两个 QSPI 实例的引脚对接，或提供 MOSI→MISO 自环回。

#### 自环回测试（autotest）

| 测试 | `+TEST_LOOPBACK` | 原理 |
|------|-------------------|------|
| `qspi1/spi_autotest/` | `qspi1_autotest_connect` | QSPI1 MOSI→MISO 环回 |
| `qspi2/spi_autotest/` | `qspi2_autotest_connect` | QSPI2 MOSI→MISO 环回 |
| `qspi3/spi_autotest/` | `qspi3_autotest_connect` | QSPI3 MOSI→MISO 环回 |

**工作原理**：master 发送数据，testbench 将 MOSI 信号直接接回 MISO，master 接收并比较。DUT 始终是主动方。

#### 主从全双工测试

| 测试 | `+TEST_LOOPBACK` | 原理 |
|------|-------------------|------|
| `qspi1/master_slave_fullduplex_ssm_hard/` | `qspi1_qspi2_connect` | QSPI1(master)↔QSPI2(slave) 硬件 CS |
| `qspi1/master_slave_fullduplex_ssm_soft/` | `qspi1_qspi2_connect` | 软件 CS 版本 |
| `qspi1/master_transmit_slave_receive_int/` | `qspi1_qspi2_connect` | 中断驱动的主发从收 |
| `qspi3/master_slave_fullduplex_ssm_hard/` | `qspi3_qspi2_connect` | QSPI3(master)↔QSPI2(slave) |
| ... | ... | 同模式的更多变体 |

**工作原理**：Testbench 将两个 QSPI 实例的 SCK/CS/DQ 引脚对接。一端配置为 master，另一端配置为 slave，双向传输数据后比较。

#### 协议模式测试

| 测试 | `+TEST_LOOPBACK` | 验证内容 |
|------|-------------------|---------|
| `spi_2_line` | `qspiX_qspi2_connect` | Dual-line (2 线) 协议模式 |
| `spi_4_line` | `qspiX_qspi2_4line_connect` | Quad-line (4 线) 协议模式 |
| `spi_half_duplex` | `qspiX_qspi2_half_connect` | 半双工模式 |

#### DMA 传输测试

| 测试 | `+TEST_LOOPBACK` | 验证内容 |
|------|-------------------|---------|
| `spi_master_slave_padma` | `qspiX_qspi2_4line_connect` | PADMA DMA 主从传输 |
| `spi_master_slave_padma_all` | `qspiX_qspi2_connect` | PADMA 综合测试 |
| `spi_master_slave_padma_continuous` | `qspiX_qspi2_connect` | PADMA 连续传输 |
| `spi_master_slave_padma_it` | `qspiX_qspi2_connect` | PADMA 中断完成 |

#### 异常条件测试

| 测试 | `+TEST_LOOPBACK` | 验证内容 |
|------|-------------------|---------|
| `underflow_overflow_int` | `qspiX_qspi2_connect` | FIFO 上溢/下溢中断 |

### 2.3 需要外部设备模型 — Flash/PSRAM/SD 卡测试（约 24 个）

这些测试需要 Testbench 挂载行为模型（Behavioral Model）。

#### NOR Flash 测试

| 测试 | `+TEST_LOOPBACK` | 需要的模型 |
|------|-------------------|-----------|
| `spi_flash_cpu` | `qspiX_flash_connect` | W25Q NOR Flash 模型 |
| `spi_flash_3byte_addr_4line` | `qspiX_flash_connect` | 同上，Quad 模式 |
| `spi_flash_4byte_addr_1line` | `qspiX_flash_connect` | 同上，4 字节地址 |
| `spi_flash_ddr_wr` | `qspiX_flash_connect` | 同上，DDR 模式 |
| `spi_flash_fifo` | `qspiX_flash_connect` | 同上，FIFO 操作 |
| `spi_flash_padma` | `qspiX_flash_connect` | 同上，DMA 传输 |

**Flash 操作流程**（以 `spi_flash_cpu` 为例）：
1. `Spi_NorFlash_ReadJEDECID()` — 读 JEDEC ID（期望 `0xEF7019` = Winbond W25Q256）
2. `Spi_NorFlash_Erase_Sector()` — 擦除 4KB 扇区
3. `Spi_NorFlash_Write_Page()` — 写入一页（256 字节）
4. `Spi_NorFlash_Read()` — 以 1-line / 2-line / 4-line 模式分别读回
5. `memcmp()` 比较写入数据与读回数据

**Flash 命令集**（`nor_flash.h` 定义）：

| 命令 | 编码 | 用途 |
|------|------|------|
| `FLASH_WriteEnable` | 0x06 | 写使能 |
| `FLASH_ReadData` | 0x03 | 单线读 |
| `FLASH_FastReadData` | 0x0B | 单线快速读 |
| `FLASH_FastReadDual` | 0x3B | 双线快速读 |
| `FLASH_FastReadQuad` | 0x6B | 四线快速读 |
| `FLASH_PageProgram` | 0x02 | 单线页编程 |
| `FLASH_PageProgramQuad` | 0x32 | 四线页编程 |
| `FLASH_SectorErase` | 0x20 | 扇区擦除 (4KB) |
| `FLASH_JedecDeviceID` | 0x9F | 读 JEDEC ID |
| `FLASH_DdrReadQuad` | 0xED | DDR 四线读 |

#### PSRAM 测试

| 测试 | `+TEST_LOOPBACK` | 需要的模型 |
|------|-------------------|-----------|
| `spi_psram` | `qspiX_psram_connect` | SPI PSRAM 模型 |

**PSRAM 命令**：`PSRAM_READ(0x03)`, `PSRAM_WRITE(0x02)`, `PSRAM_FAST_READ_QUAD(0xEB)`, `PSRAM_QUAD_WRITE(0x38)`

#### XIP 测试

| 测试 | `+TEST_LOOPBACK` | 需要的模型 |
|------|-------------------|-----------|
| `spi_xip_flash_wr` | `qspi_xip0_flash_connect` | Flash 模型 + XIP 通路 |
| `spi_xip_flash_wr_cache` | `qspi_xip0_flash_connect` | 同上 + I-Cache |
| `spi_xip_flash_ddr_wr` | `qspi_xip0_ddr_flash_connect` | DDR Flash 模型 |
| `spi_xip_psram` | `qspi_xip0_psram_connect` | PSRAM 模型 |

#### 文件系统测试

| 测试 | simargs | 说明 |
|------|---------|------|
| `spi_flash_fatfs` | 无 simargs 文件 | NOR Flash 上的 FAT 文件系统 |
| `spi_sdcard_fatfs` | 无 simargs 文件 | SD 卡上的 FAT 文件系统 |

---

## 3. `+TEST_LOOPBACK` 机制详解

`+TEST_LOOPBACK=<连接名>` 是传递给 EDA 仿真器的 plusarg 参数，Testbench 的 top module 根据该参数决定实例化哪些连接和模型。

### 连接名汇总

| 连接名 | Testbench 动作 | 需要外部模型？ |
|--------|---------------|---------------|
| `qspi1_autotest_connect` | QSPI1 MOSI→MISO 短接 | 否 |
| `qspi2_autotest_connect` | QSPI2 MOSI→MISO 短接 | 否 |
| `qspi3_autotest_connect` | QSPI3 MOSI→MISO 短接 | 否 |
| `qspi1_qspi2_connect` | QSPI1↔QSPI2 双线对接 | 否 |
| `qspi1_qspi2_4line_connect` | QSPI1↔QSPI2 四线对接 | 否 |
| `qspi1_qspi2_half_connect` | QSPI1↔QSPI2 半双工 | 否 |
| `qspi3_qspi2_connect` | QSPI3↔QSPI2 双线对接 | 否 |
| `qspi3_qspi2_4line_connect` | QSPI3↔QSPI2 四线对接 | 否 |
| `qspiX_flash_connect` | QSPIx 接 NOR Flash VIP | **是 (W25Q)** |
| `qspiX_psram_connect` | QSPIx 接 PSRAM VIP | **是** |
| `qspi_xip0_flash_connect` | XIP0 接 Flash VIP | **是 (W25Q)** |
| `qspi_xip0_psram_connect` | XIP0 接 PSRAM VIP | **是** |
| `qspi_xip0_ddr_flash_connect` | XIP0 接 DDR Flash VIP | **是** |

### 测试判定机制

所有测试通过 `simulation_pass()` / `simulation_fail()` 向仿真器发出通过/失败信号（定义在 `SoC/ns_core0/Common/Source/ns_common.c`）。测试结束后执行 `while(1){}` 等待仿真器停止。

---

## 4. 无 EDA Testbench 时的 SPI 测试策略

**核心问题**：SDK 中 64 个 SPI 测试用例中，60 个依赖 `+TEST_LOOPBACK` 提供的 Testbench 连线或设备模型。如果没有完整的 EDA 仿真环境，如何验证 SPI 接口？

### 4.1 可直接运行的测试（无需外部依赖）

**寄存器测试（4 个）可直接运行：**

```
soc_test_cases/ns_core0/SPI/qspi1/reg_test/
soc_test_cases/ns_core0/SPI/qspi2/reg_test/
soc_test_cases/ns_core0/SPI/qspi3/reg_test/
soc_test_cases/ns_core0/SPI/qspi_xip0/reg_test/
```

**验证范围**：寄存器地址映射正确、字段读写行为正确、默认值正确。这是 SPI 控制器最基础的验证。

### 4.2 FPGA 硬件板级测试策略

在 FPGA 评估板（`fpga_eval`）上，可以通过以下方式测试 SPI 接口：

#### 方案 A：外接真实 Flash 芯片

**前提**：FPGA 板上已连接或可飞线连接 W25Q 系列 NOR Flash 到某个 QSPI 控制器。

**可运行的测试**：
- `spi_flash_cpu` — 基本的 Flash 读写验证
- `spi_flash_3byte_addr_4line` — Quad 模式验证
- `spi_flash_4byte_addr_1line` — 4 字节地址验证

**修改要点**：
1. 删除 `main.simargs` 中的 `+TEST_LOOPBACK` 和 `+fast_sim`（硬件上不需要）
2. 确认 IOMUX 配置与板级引脚匹配
3. 将 `simulation_pass()` / `simulation_fail()` 替换为串口打印输出
4. 调整 `FLASH_JEDECID` 宏为实际 Flash 芯片的 ID
5. 适当降低 `SCKDIV` 分频值（FPGA 时钟通常低于 ASIC）

**示例改造**（基于 `spi_flash_cpu/main.c`）：

```c
// 替换 simulation_pass/simulation_fail
// simulation_pass();  →  printf("SPI Flash Test PASSED\n");
// simulation_fail();  →  printf("SPI Flash Test FAILED\n");

// 根据实际 Flash 修改 JEDEC ID
#define FLASH_JEDECID  0xEF4018  // 示例：W25Q128
```

#### 方案 B：两个 QSPI 实例对接（板级主从环回）

**前提**：FPGA 板上可以将两个 QSPI 控制器的引脚物理对接（如 QSPI1 ↔ QSPI2）。

**可运行的测试**：
- `master_slave_fullduplex_ssm_hard` — 主从全双工
- `spi_autotest` — 自环回
- `spi_2_line` / `spi_4_line` — 多线模式

**接线要求**（以 QSPI1 master ↔ QSPI2 slave 为例）：

| QSPI1 (Master) | 连接 | QSPI2 (Slave) |
|----------------|------|----------------|
| SCK | → | SCK |
| CS0 | → | CS0 |
| DQ0 (MOSI) | → | DQ0 (MOSI) |
| DQ1 (MISO) | ← | DQ1 (MISO) |

Quad 模式还需连接 DQ2、DQ3。

#### 方案 C：MOSI→MISO 自环回（最简单）

**前提**：将单个 QSPI 控制器的 MOSI 引脚飞线到 MISO 引脚。

**可运行的测试**：
- `spi_autotest` — 发送数据从 MOSI 环回到 MISO

**局限**：只能验证单线模式下 master 的基本收发功能。

### 4.3 自编简易测试（无需任何外部连接）

如果板上没有任何外部 SPI 设备可连接，可以编写以下测试来验证 SPI 控制器的基本功能：

#### 测试 1：QSPI 控制器初始化与寄存器验证

```c
#include <stdio.h>
#include "ns_sdk_hal.h"

void test_qspi_init(QSPI_TypeDef *qspi, const char *name)
{
    QSPI_InitTypeDef init = {0};
    QSPI_StructInit(&init);

    init.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    init.DataSize   = QSPI_FMT_LEN_8B;
    init.CPOL       = QSPI_SCKMODE_CPOL_LOW;
    init.CPHA       = QSPI_SCKMODE_CPHA_EDGE1;
    init.SCKDIV     = QSPI_SCKDIV_PRESCALER_8;
    init.Endian     = QSPI_FMT_ENDIAN_MSB;
    init.SSM        = QSPI_CR_SSM_HARD;
    init.DevMode    = QSPI_CR_MODE_MASTER;

    QSPI_Init(qspi, &init);

    /* 验证初始化后的状态 */
    printf("[%s] TX_FULL=%d, RX_EMPTY=%d\n",
           name,
           QSPI_GetFlag(qspi, QSPI_STATUS_TX_FULL),
           QSPI_GetFlag(qspi, QSPI_STATUS_RX_EMPTY));

    /* 发送单字节（即使无外部设备，TX FIFO 应能接受数据） */
    QSPI_SendData(qspi, 0xA5);
    printf("[%s] After send: TX_FULL=%d\n",
           name,
           QSPI_GetFlag(qspi, QSPI_STATUS_TX_FULL));

    printf("[%s] Init and TX FIFO test OK\n", name);
}

void main(void)
{
    /* 时钟和复位配置 */
#ifdef MISC_HAS_QSPI1_HAS_CLK
    qspi1_clk_en(ENABLE);
#endif
#ifdef MISC_HAS_QSPI1_RST
    qspi1_set_rst(DISABLE);
    qspi1_set_rst(ENABLE);
#endif

    test_qspi_init(QSPI1, "QSPI1");

    printf("SPI basic init test PASSED\n");
    while (1) {}
}
```

**验证范围**：时钟使能、复位释放、初始化配置、TX FIFO 写入、状态标志位读取。

#### 测试 2：FIFO 深度和水印中断验证

```c
/* 连续写入 TX FIFO 直到满，验证 TX_FULL 标志 */
int count = 0;
while (RESET == QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_FULL)) {
    QSPI_SendData(QSPI1, count & 0xFF);
    count++;
    if (count > 256) {
        printf("ERROR: TX FIFO never reports full\n");
        break;
    }
}
printf("TX FIFO depth = %d entries\n", count);
```

#### 测试 3：时钟输出验证（示波器/逻辑分析仪）

配置 QSPI 为 master 并发送数据，用示波器观察 SCK 引脚，验证：
- SCK 频率是否符合 `SCKDIV` 分频配置
- CPOL/CPHA 是否正确
- CS 是否正确拉低/拉高

### 4.4 推荐的渐进测试路径

```
阶段 1: 纯软件验证（无需外部硬件）
  ├── reg_test × 4 个控制器         → 寄存器映射正确性
  └── 自编 FIFO/初始化测试          → 控制器基本功能

阶段 2: 最小硬件验证（MOSI→MISO 飞线）
  └── spi_autotest                  → 单线收发通路

阶段 3: 主从验证（两个 QSPI 对接）
  ├── master_slave_fullduplex       → 全双工数据通路
  ├── spi_2_line / spi_4_line       → 多线协议
  └── underflow_overflow_int        → 异常处理

阶段 4: 外设验证（外接 Flash/PSRAM）
  ├── spi_flash_cpu                 → Flash 基本读写
  ├── spi_flash_3byte_addr_4line    → Quad 模式
  ├── spi_flash_padma               → DMA 传输
  └── spi_psram                     → PSRAM 访问

阶段 5: 高级功能验证
  ├── spi_xip_flash_wr              → XIP 执行
  ├── spi_flash_fatfs               → 文件系统
  └── spi_flash_ddr_wr              → DDR 模式
```

### 4.5 SDK 测试用例改造为硬件测试的通用修改清单

将 SDK 中的 EDA 仿真测试用例移植到 FPGA 板级测试时，需要做以下修改：

| 修改项 | 原始（仿真） | 改造后（硬件） |
|--------|-------------|---------------|
| 结果输出 | `simulation_pass()` / `simulation_fail()` | `printf("PASS\n")` / `printf("FAIL\n")` |
| simargs | `+TEST_LOOPBACK=...` | 删除（硬件上物理连线替代） |
| 超时 | `+TEST_TO_VALUE=...` | 删除或改为软件看门狗 |
| 时钟分频 | `SCKDIV=8` | 可能需调大（FPGA 时钟较低） |
| IOMUX | 仿真 pad 配置 | 确认与 FPGA 板级引脚一致 |
| JEDEC ID | `0xEF7019` (W25Q256) | 根据实际 Flash 型号修改 |
| 打印 | 少量 printf | 可增加详细日志辅助调试 |

---

## 5. QSPI 驱动 API 速查

### 初始化与配置

| API | 功能 |
|-----|------|
| `QSPI_Init()` | 用配置结构体初始化 QSPI |
| `QSPI_StructInit()` | 填充默认配置 |
| `QSPI_BusWidthSwitch()` | 切换协议模式 (SINGLE/DUAL/QUAD) |
| `QSPI_DataWidthConfig()` | 设置数据宽度 (8/16/32 bit) |
| `QSPI_DdrModeEnable()` | 使能 DDR 模式 |

### 数据传输

| API | 功能 |
|-----|------|
| `QSPI_SendData()` | 写 TX FIFO |
| `QSPI_ReceiveData()` | 读 RX FIFO |
| `QSPI_TransmitReceive()` | 全双工收发 |
| `QSPI_DirectionConfig()` | 设置 TX/RX 方向 |

### FIFO 控制

| API | 功能 |
|-----|------|
| `QSPI_TxmarkConfig()` | TX FIFO 水印 |
| `QSPI_RxmarkConfig()` | RX FIFO 水印 |
| `QSPI_ClearFifo()` | 清空 FIFO |
| `QSPI_RxFifoEnable()` | 使能/禁止 RX FIFO |

### DMA

| API | 功能 |
|-----|------|
| `QSPI_TxsizeConfig()` | TX DMA 大小 |
| `QSPI_RxsizeConfig()` | RX DMA 大小 |
| `QSPI_TxDmaStart()` | 启动 TX DMA |
| `QSPI_RxDmaStart()` | 启动 RX DMA |

### 片选与状态

| API | 功能 |
|-----|------|
| `QSPI_CSModeConfig()` | CS 模式 (AUTO/HOLD/OFF) |
| `QSPI_CSIDConfig()` | 选择 CS 线 (CS0-3) |
| `QSPI_GetFlag()` | 读状态标志 (TX_FULL, RX_EMPTY, BUSY 等) |
| `QSPI_ITConfig()` | 使能/禁止中断 |
| `QSPI_ClearFlag()` | 清除标志 |
| `QSPI_ForceEnable()` | 强制输出控制（WP/HOLD 引脚） |

### NOR Flash 高层 API（`nor_flash.h`）

| API | 功能 |
|-----|------|
| `Spi_NorFlash_ReadJEDECID()` | 读 JEDEC ID |
| `Spi_NorFlash_Read()` | 按指定模式读数据 |
| `Spi_NorFlash_Write_Page()` | 写一页 (≤256 字节) |
| `Spi_NorFlash_Erase_Sector()` | 擦除 4KB 扇区 |
| `Spi_NorFlash_Erase_Chip()` | 全片擦除 |
| `Spi_NorFlash_EnableQuadMode_WQ25xx()` | 使能 Winbond Quad 模式 |
| `Spi_NorFlash_Enter4ByteMode()` | 进入 4 字节地址模式 |
| `Spi_NorFlash_Wait_Busy()` | 等待 Flash 就绪 |

---

## 6. 总结

| 类别 | 数量 | 外部依赖 | 可直接在 FPGA 板上运行？ |
|------|------|---------|------------------------|
| 寄存器测试 | 4 | 无 | **可以** |
| 自环回测试 | 3 | MOSI→MISO 飞线 | **简单飞线即可** |
| 主从对接测试 | ~20 | 两个 QSPI 引脚对接 | **需要板级连线** |
| Flash 测试 | ~18 | W25Q Flash 芯片 | **需要外接 Flash** |
| PSRAM 测试 | ~6 | PSRAM 芯片 | **需要外接 PSRAM** |
| XIP 测试 | 6 | Flash/PSRAM + XIP 通路 | **需要 Flash + XIP 配置** |
| 文件系统测试 | 4 | Flash 或 SD 卡 | **需要存储介质** |

**关键结论**：SDK 的 SPI 测试用例是为 EDA 仿真设计的，通过 `+TEST_LOOPBACK` 参数让 Testbench 提供连线和设备模型。在没有 EDA 环境时，需要在 FPGA 板上用物理连线和真实器件替代 Testbench 的角色。建议按「阶段 1 → 阶段 5」渐进式推进，从纯寄存器验证开始，逐步扩展到完整的外设验证。

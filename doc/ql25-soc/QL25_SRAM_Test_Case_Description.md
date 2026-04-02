# QL25 外部 SRAM 读写测试用例说明

> 版本: v1.0
> 日期: 2026-04-02
> 用例路径: `soc_test_cases/ns_core{0,1}/SRAM/sram{0,1,2,3}/Bkup_ram_wr_test/`

---

## 1. 测试目的

验证 QL25 SoC 的两个 CPU 核（Core 0 从核、Core 1 主核）能否正确访问全部 4 个外部 SRAM bank。

这是 AMP 双核架构的**硬件基础验证**，因为：

- **共享内存区**（IPC 控制块、描述符环、boot flag）将分配在外部 SRAM 中
- 两核都需要对外部 SRAM 进行读写操作
- 如果任一核对任一 SRAM bank 访问异常，后续 AMP bring-up、IPC 通信、数据面缓存都会失败

---

## 2. 被测对象

### 2.1 SRAM 硬件参数

QL25 SoC 外挂 4 个 SRAM bank，总计 384 KB：

| Bank | 基址 | 大小 | SDK 宏定义 |
|------|------|------|-----------|
| SRAM0 | `0x30000000` | 128 KB | `SRAM0_MEM_BASE` / `SRAM0_MEM_SIZE` |
| SRAM1 | `0x30020000` | 128 KB | `SRAM1_MEM_BASE` / `SRAM1_MEM_SIZE` |
| SRAM2 | `0x30040000` | 64 KB | `SRAM2_MEM_BASE` / `SRAM2_MEM_SIZE` |
| SRAM3 | `0x30060000` | 64 KB | `SRAM3_MEM_BASE` / `SRAM3_MEM_SIZE` |

> 定义位置: `SoC/ns_core0/Common/Include/ns.h` 第 955~972 行

### 2.2 测试矩阵

| 用例路径 | 执行核 | 目标 Bank |
|---------|--------|----------|
| `ns_core0/SRAM/sram0/Bkup_ram_wr_test/` | Core 0 (从核) | SRAM0 |
| `ns_core0/SRAM/sram1/Bkup_ram_wr_test/` | Core 0 (从核) | SRAM1 |
| `ns_core0/SRAM/sram2/Bkup_ram_wr_test/` | Core 0 (从核) | SRAM2 |
| `ns_core0/SRAM/sram3/Bkup_ram_wr_test/` | Core 0 (从核) | SRAM3 |
| `ns_core1/SRAM/sram0/Bkup_ram_wr_test/` | Core 1 (主核) | SRAM0 |
| `ns_core1/SRAM/sram1/Bkup_ram_wr_test/` | Core 1 (主核) | SRAM1 |
| `ns_core1/SRAM/sram2/Bkup_ram_wr_test/` | Core 1 (主核) | SRAM2 |
| `ns_core1/SRAM/sram3/Bkup_ram_wr_test/` | Core 1 (主核) | SRAM3 |

**共 8 个用例**，代码完全相同，仅通过 `SRAMx_MEM_BASE` 宏区分目标 bank。两核的用例之间也完全一致（`diff` 无差异），仅编译时链接到不同核的 SoC 库。

---

## 3. 测试原理

测试通过 CPU 直接读写 SRAM 地址，用**写后读回比较**（Write-then-Read-back-Compare）的方式验证数据完整性。不使用 DMA，不经过 Cache（QL25 无 D-Cache），是最直接的总线访问测试。

### 3.1 内存访问宏

```c
// SoC/ns_core0/Common/Include/ns.h
#define REG8(addr)    (*(volatile uint8_t  *)((uintptr_t)(addr)))
#define REG16(addr)   (*(volatile uint16_t *)((uintptr_t)(addr)))
#define REG32(addr)   (*(volatile uint32_t *)((uintptr_t)(addr)))
```

使用 `volatile` 指针保证每次读写都实际产生总线事务，不会被编译器优化掉。

### 3.2 测试区域

每个用例只测试目标 SRAM bank 的**起始 0x100 字节**（256 字节）：

```c
#define TEST_MEM_SIZE  0x100
#define TEST_MEM_BASE  (SRAMx_MEM_BASE)   // 由 sram0/1/2/3 目录决定
```

> 注意：测试范围是 256 字节，不是整个 bank。这是因为 EDA 仿真下完整遍历 128KB 耗时太长。如需全量测试，可修改 `TEST_MEM_SIZE`。

---

## 4. 测试方法

测试由 3 组子测试组成，通过独立的状态标志 `state0`、`state1`、`state2` 分别跟踪，全部通过才判为 PASS。

### 4.1 子测试 1：连续写后立即读回比较（`state0`）

验证连续地址的基本读写功能。

| 函数名 | 访问粒度 | 地址范围 | 数据模式 |
|--------|---------|---------|---------|
| `ContinueWriteWordFastToReadCompareSRAM` | 32-bit (word) | `[BASE, BASE+0x100)` | `(i<<24)+(i<<16)+(i<<8)+i` |
| `ContinueWriteHwordFastToReadCompareSRAM` | 16-bit (half-word) | `[BASE+0x40, BASE+0x50)` | `i+3 + ((i+3)<<8)` |
| `ContinueWriteByteFastToReadCompareSRAM` | 8-bit (byte) | `[BASE+0x80, BASE+0x90)` | `i+5` |

**工作流程**（以 word 为例）：

```
for 每个地址 (步长 4):
    ① 计算写入数据 w_dat
    ② REG32(addr) = w_dat        ← CPU 发起 store 到总线
    ③ val = REG32(addr)           ← CPU 发起 load 从总线
    ④ if (w_dat != val) → 报错，置 state0 = 0，跳出
```

**验证点**：
- 总线 store → load 往返数据正确
- 3 种粒度（byte / half-word / word）均工作
- 连续地址无地址译码错误

### 4.2 子测试 2：Walking-ones 位翻转测试（`state1`）

验证 SRAM 每一位都能独立置 1 和清 0，排除相邻位短路、数据线悬空等硬件缺陷。

| 函数名 | 访问粒度 | 测试地址 |
|--------|---------|---------|
| `bit_set_clear_in_word_test` | 32-bit | `BASE`, `BASE+1`, `BASE+2`, `BASE+3` |
| `bit_set_clear_in_hword_test` | 16-bit | `BASE`, `BASE+1`, `BASE+3` |
| `bit_set_clear_in_byte_test` | 8-bit | `BASE` |

**工作流程**（以 word 为例）：

```
// 置 1 扫描
for bit = 0..31:
    wdat = 1 << bit              ← 只有 1 位为 1
    REG32(addr) = wdat
    rdat = REG32(addr)
    if (wdat != rdat) → 报错

// 清 0 扫描
for bit = 0..31:
    wdat = ~(1 << bit)           ← 只有 1 位为 0
    REG32(addr) = wdat
    rdat = REG32(addr)
    if (wdat != rdat) → 报错
```

**验证点**：
- 32 条数据线每一条独立可控
- 无相邻位干扰（stuck-at / bridging fault 检测）
- **非对齐访问**：word 操作在 `BASE+1`、`BASE+2`、`BASE+3` 地址执行，验证总线对非自然对齐地址的处理

### 4.3 子测试 3：先写后读分离比较（`state2`）

子测试 1 是"写一个读一个"，子测试 3 是"先批量写、再批量读"，验证 SRAM 能保持数据不丢失。

| 写入函数 | 读回函数 | 粒度 | 数据模式 |
|---------|---------|------|---------|
| `WriteWordDiffDataToSRAM` | `ReadWordDiffDataFromSRAMToCompare` | word | 0xF0F0F0F0, 0xC0C3C0C3, 0x3C3C3C3C |
| `WriteByteDiffDataToSRAM` | `ReadByteDiffDataFromSRAMToCompare` | byte | 0xD0 起递增 |
| `WriteHwordDiffDataToSRAM` | `ReadHwordDiffDataFromSRAMToCompare` | half-word | 0xD0D0 起递增 |

**工作流程**：

```
① WriteDiffData → 批量写入 16 字节
② ReadDiffDataToCompare → 批量读回 16 字节并逐一比较
```

**验证点**：
- 数据持久性（写入后不立即读回，中间有其他操作）
- 不同数据模式覆盖（全 0、全 1、交替 01/10 模式）

---

## 5. 测试判定

```c
if (state0 && state1 && state2) {
    simulation_pass();    // 3 组子测试全部通过
} else {
    simulation_fail();    // 任一子测试失败
}
```

| 结果 | 含义 |
|------|------|
| `simulation_pass()` | 全部读写比较通过，SRAM 对该核可正常访问 |
| `simulation_fail()` | 至少一个比较失败，具体错误由串口打印定位 |

---

## 6. 外部依赖

| 项目 | 要求 |
|------|------|
| 外部激励 | **不需要**，纯 CPU 读写操作 |
| Testbench 模型 | **不需要**，无 `+TEST_LOOPBACK` 参数 |
| simargs | `+TEST_TO_VALUE=3000000000`（仿真超时值，单位为仿真周期） |
| 硬件连接 | 无，SRAM 为 SoC 片上外挂存储 |

---

## 7. 如何运行

### 7.1 EDA 仿真运行

```bash
# 以 Core 1（主核）测试 SRAM0 为例
make SOC=ns_core1 CORE=core1_n300 DOWNLOAD=ilm BOARD=fpga_eval \
     PROGRAM=soc_test_cases/ns_core1/SRAM/sram0/Bkup_ram_wr_test clean all

# 以 Core 0（从核）测试 SRAM2 为例
make SOC=ns_core0 CORE=core0_n300 DOWNLOAD=ilm BOARD=fpga_eval \
     PROGRAM=soc_test_cases/ns_core0/SRAM/sram2/Bkup_ram_wr_test clean all
```

### 7.2 按 EDA 标准格式运行

以下命令采用 Quick-start 中推荐的 EDA 运行格式：

```bash
ctest_gun -t <CTEST_NAME> --bm=ilm
make run TESTNAME=<CTEST_NAME>_ilm
```

#### Core 0（从核）SRAM 测试

```bash
ctest_gun -t SRAM_sram0_Bkup_ram_wr_test --bm=ilm
make run TESTNAME=SRAM_sram0_Bkup_ram_wr_test_ilm

ctest_gun -t SRAM_sram1_Bkup_ram_wr_test --bm=ilm
make run TESTNAME=SRAM_sram1_Bkup_ram_wr_test_ilm

ctest_gun -t SRAM_sram2_Bkup_ram_wr_test --bm=ilm
make run TESTNAME=SRAM_sram2_Bkup_ram_wr_test_ilm

ctest_gun -t SRAM_sram3_Bkup_ram_wr_test --bm=ilm
make run TESTNAME=SRAM_sram3_Bkup_ram_wr_test_ilm
```

#### Core 1（主核）SRAM 测试

```bash
ctest_gun -t SRAM_sram0_Bkup_ram_wr_test --bm=ilm
make run TESTNAME=SRAM_sram0_Bkup_ram_wr_test_ilm

ctest_gun -t SRAM_sram1_Bkup_ram_wr_test --bm=ilm
make run TESTNAME=SRAM_sram1_Bkup_ram_wr_test_ilm

ctest_gun -t SRAM_sram2_Bkup_ram_wr_test --bm=ilm
make run TESTNAME=SRAM_sram2_Bkup_ram_wr_test_ilm

ctest_gun -t SRAM_sram3_Bkup_ram_wr_test --bm=ilm
make run TESTNAME=SRAM_sram3_Bkup_ram_wr_test_ilm
```

说明：

- SRAM 用例是单核测试，不需要 `+TEST_BOOT_TYPE=parallel_boot`
- `CTEST_NAME` 对应 `soc_test_cases/ns_core0/SRAM/sram.ctestlist` 和 `soc_test_cases/ns_core1/SRAM/sram.ctestlist` 中的目标名
- `--bm=ilm` 对应当前文档描述的 ILM 仿真方式

### 7.3 完整测试矩阵

建议按以下顺序执行，确保两核对所有 SRAM bank 的访问均正常：

```
Core 1 (主核):  sram0 → sram1 → sram2 → sram3
Core 0 (从核):  sram0 → sram1 → sram2 → sram3
```

---

## 8. 如何观察结果

### 8.1 通过（PASS）

仿真日志中出现 `simulation_pass` 调用（具体表现取决于 Testbench 的 pass/fail 检测机制），且**无任何 `ERR` 打印**。

### 8.2 失败（FAIL）

串口会打印详细的错误信息，可直接定位故障类型：

**连续读写失败（state0）：**
```
Fast RW addr 30000008 match err, write: 08080808, read : 00000000
```
含义：地址 `0x30000008` 写入 `0x08080808` 后读回 `0x00000000`，总线写入未生效或读回数据被篡改。

**位翻转失败（state1）：**
```
1<< ERR A: 30000000, w: 00000004, r 00000000
```
含义：对地址 `0x30000000` 写入 `bit 2 = 1`（`0x00000004`）后读回 `0x00000000`，bit 2 无法被置 1。可能是数据线第 2 位故障。

```
0<< ERR A: 30000000, w: fffffffb, r ffffffff
```
含义：对地址 `0x30000000` 写入 `bit 2 = 0`（`0xFFFFFFFB`）后读回 `0xFFFFFFFF`，bit 2 无法被清 0。

**批量读回失败（state2）：**
```
WThenR err A: 30000004, r: 00000000
```
含义：先写后读，地址 `0x30000004` 的数据在批量写入后丢失或被覆盖。

### 8.3 错误排查指引

| 错误现象 | 可能原因 |
|---------|---------|
| 所有地址读回全 0 | SRAM 未上电 / 时钟未使能 / 地址译码错误 |
| 特定地址失败 | 地址线故障（某位 stuck）/ SRAM bank 内部缺陷 |
| 特定 bit 位失败 | 数据线故障 / SRAM 存储单元缺陷 |
| 非对齐地址失败 | 总线不支持非对齐访问 / 总线桥配置问题 |
| byte/half-word 写正常但 word 失败 | 总线宽度配置错误 |
| state0 过但 state2 失败 | 数据保持性问题（SRAM 刷新 / 竞争） |
| Core 0 过但 Core 1 失败（或反之） | 某核到 SRAM 的总线路径故障 / 仲裁问题 |
| 所有 bank 都失败 | AHB/APB 总线桥配置问题 / 基址映射错误 |

---

## 9. 测试局限性与补充建议

### 9.1 本测试的局限

| 局限 | 说明 |
|------|------|
| 测试范围小 | 仅覆盖每个 bank 的起始 256 字节，不覆盖全地址空间 |
| 单核独占访问 | 未测试两核同时访问同一 SRAM bank 的仲裁场景 |
| 无 DMA 通路测试 | 仅 CPU 直接访问，未覆盖 DMA 到 SRAM 的路径 |
| 无压力/性能测试 | 未测量 SRAM 访问延迟和带宽 |
| 无数据保持测试 | 未覆盖长时间数据保持（掉电/休眠后恢复） |

### 9.2 后续建议补充的测试

| 测试 | 目的 | 对应 AMP 场景 |
|------|------|-------------|
| 全地址空间遍历 | 覆盖 SRAM 所有存储单元 | 确认完整 384KB 可用 |
| **双核并发访问** | 两核同时读写同一 bank | 验证总线仲裁和数据一致性 |
| DMA 读写测试 | UDMA 搬运数据到 SRAM | 验证 DMA 到 SRAM 路径 |
| 跨 bank 连续访问 | 从 SRAM0 尾部跨到 SRAM1 头部 | 验证 bank 边界无死区 |
| fence 可见性测试 | Core A 写 → fence → Core B 读 | 验证共享内存 fence 语义正确 |

---

## 10. 与 AMP 开发计划的关系

本测试对应开发计划中 **Phase 0（硬件事实冻结）** 和 **Phase 2（AMP 启动链路）** 的前置验证：

```
SRAM 测试全通过
    │
    ├── 确认 → 外部 SRAM 可作为共享内存区
    ├── 确认 → 两核都能访问，AMP IPC 基础可行
    ├── 确认 → SRAM bank 分配方案可落地
    │           (SRAM0/1 → 从核数据面, SRAM2 → 主核, SRAM3 → 共享 IPC)
    │
    └── 下一步 → IDU 核间中断测试 → AMP bring-up 开发
```

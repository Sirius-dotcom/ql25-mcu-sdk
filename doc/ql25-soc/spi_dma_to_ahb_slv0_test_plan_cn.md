# SPI 接收 DMA 到 SRAM 并写入 AHB_SLV0_RATIO_MEM 用例设计与落地方案

## 1. 目的

本文档定义两个新增测试用例，用于验证 QL25 外部 SPI 接口在多核场景下的数据接收与 AHB 窗口写入能力。

设计约束如下：

- 从核是 `core0`，有 ILM
- 主核是 `core1`，无 ILM
- 外部 EDA 仿真会从 SPI 发包
- 包长度固定为 `730 bytes`
- SPI 接收必须使用 DMA
- 第一阶段必须先 DMA 到本地 SRAM
- 第二阶段再由 CPU 将数据写到 `0x18000000`
- 同时必须验证 DMA 收到的 SRAM 内容正确

目标地址窗口：

- `AHB_SLV0_RATIO_MEM_BASE = 0x18000000`
- 地址范围：`0x18000000 ~ 0x18003FFF`
- 容量：16KB

730 字节写入该窗口不会越界。

## 2. 用例定义

### 2.1 用例一：core0 SPI RX DMA -> SRAM -> 0x18000000

建议名称：

- `SPI_qspi1_spi_rx_dma_to_ahb_slv0`

建议目录：

- `soc_test_cases/ns_core0/SPI/qspi1/spi_rx_dma_to_ahb_slv0`

用途：

- 验证 `core0` 作为 SPI 从设备时，能否通过 RX DMA 正确接收外部 EDA 发来的 730 字节数据包
- 验证 DMA 接收后的 SRAM 缓冲区数据正确
- 验证 `core0` 再将该缓冲区写入 `0x18000000`
- 验证 `0x18000000` 中的数据与预期一致

该用例验证完整数据路径：

- EDA SPI Master
- 外部 SPI 口
- SPI 从设备接收
- RX DMA
- SRAM 缓冲区
- core0 CPU 拷贝
- `0x18000000` AHB 窗口

### 2.2 用例二：core1 直接写 0x18000000

建议名称：

- `ahb_slv0_ratio_mem_write`

建议目录：

- `soc_test_cases/ns_core1/TOP_CTEST/ahb_slv0_ratio_mem_write`

用途：

- 验证 `core1` 直接访问 `0x18000000` 的读写能力
- 给用例一提供基线

该用例不依赖 SPI，不依赖 DMA，用于先证明：

- 主核访问 `0x18000000` 是通的
- 目标 AHB 窗口本身没有问题

## 3. 为什么拆成两个用例

如果把“SPI + DMA + SRAM + core0 + AHB 窗口”全部压进一个用例，失败时很难定位问题。

拆分后可以按层验证：

1. `core1` 直写 `0x18000000`
2. `core0` SPI RX DMA 到 SRAM
3. `core0` 把 SRAM 拷贝到 `0x18000000`

这样可以快速判断问题是在：

- AHB 窗口
- core0 访问路径
- SPI 接收
- DMA 配置
- 外部 EDA 发包时序

## 4. 总体实现策略

### 4.1 关键策略

第一版实现必须采用：

- `SPI RX DMA -> SRAM buffer -> CPU memcpy -> 0x18000000`

不建议第一版做：

- `SPI RX DMA -> 0x18000000` 直写

原因：

- 先分离验证 SPI DMA 与 AHB 窗口
- 更容易看出 DMA 是否真的收到了正确数据
- 便于打印和回读校验

### 4.2 推荐 SPI 实例

建议优先使用：

- `qspi1`

原因：

- `qspi1` 目录已有大量现成用例
- 便于复用 `spi_flash_padma` 的 DMA 配置方式

如果项目板级连接实际使用的是 `qspi3`，则目录与宏定义对应替换即可。

## 5. 用例一详细设计

## 5.1 功能目标

`core0` 完成以下动作：

1. 配置 `qspi1` 为 SPI 从设备
2. 配置 RX DMA，将 SPI 收到的 730 字节搬到 SRAM 缓冲区
3. 等待 DMA 完成
4. 验证 SRAM 缓冲区内容和期望值一致
5. 将 SRAM 缓冲区逐字节写入 `0x18000000`
6. 从 `0x18000000` 回读 730 字节
7. 验证回读数据和期望值一致
8. 输出 PASS/FAIL

## 5.2 数据流

数据流如下：

```text
EDA SPI Master
    ->
QSPI1 Slave
    ->
QSPI1 RXDATA
    ->
UDMA RX
    ->
core0 SRAM rx_buf[730]
    ->
core0 CPU copy
    ->
0x18000000 (AHB_SLV0_RATIO_MEM)
    ->
CPU readback
    ->
compare with expected payload
```

## 5.3 软件结构建议

建议文件：

- `main.c`
- `Makefile`
- `npk.yml`
- `README.TXT`
- `main.simargs`

### main.c 建议函数划分

1. `payload_build_expected()`
2. `iomux_config()`
3. `spi_slave_init()`
4. `udma_clock_reset_init()`
5. `udma_rx_config()`
6. `udma_wait_done()` 或 `UDMA0_IRQHandler()`
7. `compare_sram_buf()`
8. `copy_buf_to_ahb_slv0()`
9. `compare_ahb_slv0_buf()`
10. `dump_head_tail()`
11. `main()`

## 5.4 关键数据结构

建议定义：

```c
#define PKT_LEN                 730
#define AHB_SLV0_ADDR           0x18000000UL

static uint8_t expected_buf[PKT_LEN];
static uint8_t rx_buf[PKT_LEN];
static uint8_t rdback_buf[PKT_LEN];

static volatile uint32_t udma_rx_done = 0;
static volatile uint32_t udma_rx_error = 0;
```

说明：

- `expected_buf` 保存期望包内容
- `rx_buf` 是 DMA 接收的 SRAM 缓冲区
- `rdback_buf` 是从 `0x18000000` 回读后的数据

## 5.5 EDA 发包内容约定

建议固定包格式，以方便仿真、日志和波形核对。

建议 payload：

- Byte0 = `0x55`
- Byte1 = `0xAA`
- Byte2 = `0xDA`  // 730 低字节
- Byte3 = `0x02`  // 730 高字节
- Byte4 ~ Byte729 = `(i ^ 0x5A) & 0xFF`

其中 `i` 是全包偏移。

这样有几个好处：

- 头 4 字节可快速识别是否对齐
- 后续 payload 不会太单调
- 便于首尾采样打印

## 5.6 SPI 配置建议

建议参数：

- `DevMode = QSPI_CR_MODE_SLAVE`
- `ProtolMode = QSPI_FMT_PROTO_SINGLE`
- `DataSize = QSPI_FMT_LEN_8B`
- `Endian = QSPI_FMT_ENDIAN_MSB`
- `SSM = QSPI_CR_SSM_HARD`
- `CPOL`、`CPHA` 与 EDA testbench 保持一致

第一版建议使用：

- `CPOL = QSPI_SCKMODE_CPOL_LOW`
- `CPHA = QSPI_SCKMODE_CPHA_EDGE1`

如果 testbench 现有驱动采用别的模式，以 testbench 为准。

## 5.7 DMA 配置建议

直接参考现有：

- `soc_test_cases/ns_core0/SPI/qspi1/spi_flash_padma/main.c`

用例一只需要 RX DMA。

推荐配置：

- 传输方向：`QSPI1->RXDATA -> rx_buf`
- 宽度：`8bit`
- 长度：`730`
- 源地址不递增
- 目标地址递增
- 模式：`PA2M_MODE_NORMAL`

配置逻辑建议：

```c
dst = &rx_buf[0]
src = &QSPI1->RXDATA
buffer_size = 730
dst_inc = enable
src_inc = disable
width = 8bit
per_sel = UDMA_SEL_QSPI1_RX_DMA
```

启动顺序建议：

1. 清空 `rx_buf`
2. 配好 UDMA
3. 使能 `QSPI_RxDmaStart(QSPI1, ENABLE)`
4. 等待外部 SPI 发包
5. 等待 DMA 完成标志
6. 关闭 `QSPI_RxDmaStart(QSPI1, DISABLE)`

## 5.8 SRAM 校验要求

这是必须项。

DMA 完成后，必须先校验 `rx_buf`：

```text
expected_buf[730]  vs  rx_buf[730]
```

如果这里失败：

- 立即打印失败
- 不进入 AHB 窗口写阶段
- 直接 `simulation_fail()`

这样才能把“SPI/DMA 问题”和“AHB 窗口问题”分开。

## 5.9 写入 0x18000000 的方式

第一版建议使用 CPU 逐字节或 `memcpy` 写入：

```c
volatile uint8_t *dst = (volatile uint8_t *)AHB_SLV0_ADDR;
for (i = 0; i < PKT_LEN; i++) {
    dst[i] = rx_buf[i];
}
```

建议不要直接写 32-bit：

- 730 不是 4 字节对齐长度
- 第一版按 8-bit 最稳

## 5.10 AHB 窗口回读校验

写完后，再从 `0x18000000` 回读到 `rdback_buf`：

```c
volatile uint8_t *src = (volatile uint8_t *)AHB_SLV0_ADDR;
for (i = 0; i < PKT_LEN; i++) {
    rdback_buf[i] = src[i];
}
```

然后比较：

```text
expected_buf[730]  vs  rdback_buf[730]
```

如果失败：

- 打印首尾若干字节
- `simulation_fail()`

## 5.11 用例一的通过标准

以下条件必须全部满足：

1. SPI 从设备成功接收到外部 730 字节包
2. RX DMA 完成标志正常到达
3. `rx_buf` 与 `expected_buf` 完全一致
4. `0x18000000` 回读数据与 `expected_buf` 完全一致
5. 软件打印 PASS，并调用 `simulation_pass()`

## 5.12 用例一日志建议

建议打印如下内容：

```text
[SPI_RX_DMA_AHB] start
[SPI_RX_DMA_AHB] waiting spi packet len=730
[SPI_RX_DMA_AHB] dma done
[SPI_RX_DMA_AHB] sram compare pass
[SPI_RX_DMA_AHB] ahb_slv0 copy done
[SPI_RX_DMA_AHB] ahb compare pass
[SPI_RX_DMA_AHB] PASS
```

失败日志建议：

```text
[SPI_RX_DMA_AHB] dma timeout
[SPI_RX_DMA_AHB] sram compare fail at idx=...
[SPI_RX_DMA_AHB] ahb compare fail at idx=...
```

## 5.13 用例一调试打印建议

建议打印首尾 16 字节：

- `expected_buf[0..15]`
- `rx_buf[0..15]`
- `rdback_buf[0..15]`
- `expected_buf[714..729]`
- `rx_buf[714..729]`
- `rdback_buf[714..729]`

这样足够定位：

- 包头错位
- 中间长度错
- 尾部截断

## 6. 用例二详细设计

## 6.1 功能目标

`core1` 完成以下动作：

1. 构造同样的 730 字节 `expected_buf`
2. 直接写入 `0x18000000`
3. 回读到 `rdback_buf`
4. 比较数据是否一致
5. 输出 PASS/FAIL

## 6.2 目的

该用例不验证 SPI，不验证 DMA，只验证：

- `core1` 是否能访问 `0x18000000`
- `ahb_slv0_ratio_mem` 是否对主核可写可读

该用例是用例一的基线。

## 6.3 软件结构建议

建议文件：

- `main.c`
- `Makefile`
- `npk.yml`
- `README.TXT`

### main.c 建议函数划分

1. `payload_build_expected()`
2. `write_ahb_slv0()`
3. `readback_ahb_slv0()`
4. `compare_ahb_slv0()`
5. `dump_head_tail()`
6. `main()`

## 6.4 用例二的通过标准

以下条件必须全部满足：

1. `core1` 成功写入 `0x18000000`
2. 回读 730 字节与 `expected_buf` 完全一致
3. 软件打印 PASS，并调用 `simulation_pass()`

## 7. 目录与命名建议

## 7.1 用例一

建议路径：

- `soc_test_cases/ns_core0/SPI/qspi1/spi_rx_dma_to_ahb_slv0`

建议目标名：

- `SPI_qspi1_spi_rx_dma_to_ahb_slv0`

## 7.2 用例二

建议路径：

- `soc_test_cases/ns_core1/TOP_CTEST/ahb_slv0_ratio_mem_write`

建议目标名：

- `ahb_slv0_ratio_mem_write`

## 7.3 是否加入 ctestlist

建议加入各自清单：

- `soc_test_cases/ns_core0/SPI/spi.ctestlist`
- `soc_test_cases/ns_core1/top_ctest` 对应清单或按项目现有规则加入

## 8. 代码落地顺序建议

建议严格按以下顺序开发。

### 步骤 1：先完成用例二

先验证：

- `core1` 是否能稳定写 `0x18000000`

如果用例二失败，则说明：

- AHB 窗口访问有问题
- 或主核访问权限/映射有问题

此时不应继续推进用例一。

### 步骤 2：完成用例一的 SPI RX DMA 到 SRAM

先只做到：

- SPI RX DMA
- 验证 `rx_buf`

暂时不要写 `0x18000000`

这样可以先确认：

- 外部 EDA 发包正确
- SPI 从设备时序正确
- DMA 收包正确

### 步骤 3：增加 SRAM -> 0x18000000 拷贝与回读

在步骤 2 确认稳定后，再加：

- `rx_buf -> 0x18000000`
- 回读并比对

## 9. EDA 仿真侧配合要求

用例一要求 EDA 仿真环境提供 SPI Master 发包逻辑。

## 9.1 发包要求

- 包长度固定为 `730`
- 包内容固定，且软件和 testbench 使用同一套生成规则
- 片选、时钟和数据时序必须与 SPI slave 配置一致

## 9.2 推荐测试时序

1. SoC 复位释放
2. core0 完成 SPI slave 和 DMA 初始化
3. 软件打印 `waiting spi packet`
4. EDA testbench 开始拉低 `CS`
5. 发送 730 字节
6. 释放 `CS`
7. 等待 DMA 完成
8. 软件输出比较结果

## 9.3 testbench 建议增加的检查项

建议在 EDA testbench 端也增加 3 类观察：

1. 发包日志
2. SPI 总线波形检查
3. 最终读取 `0x18000000` 的内容做辅助校验

## 10. 结果观察方法

## 10.1 软件日志观察

最直接的观察方式：

- 串口终端打印
- 仿真 console 输出

建议把所有关键阶段打印清楚：

- 初始化开始
- DMA 启动
- DMA 完成
- SRAM 校验结果
- AHB 回读结果
- 最终 PASS/FAIL

## 10.2 仿真 PASS/FAIL

最终必须依赖：

- `simulation_pass()`
- `simulation_fail()`

不能只靠 `printf`

## 10.3 波形观察

用例一建议在 Verdi 中观察：

- `SPI CS`
- `SPI SCK`
- `SPI DQ0/DQ1`
- QSPI RX FIFO 变化
- UDMA 请求与完成中断
- 写 `0x18000000` 的 AHB 事务

## 10.4 地址窗口观察

重点观察地址：

- `0x18000000`
- `0x18000001`
- `0x18000002`
- `0x18000003`
- `0x180002D9`

说明：

- `730 = 0x2DA`
- 最后一个有效字节地址是 `0x18000000 + 0x2D9`

## 11. 建议的失败分类

建议按以下错误类型分类打印：

- `ERR_DMA_TIMEOUT`
- `ERR_SRAM_MISMATCH`
- `ERR_AHB_MISMATCH`
- `ERR_SPI_CONFIG`
- `ERR_UNEXPECTED_LENGTH`

这样后续回归日志更容易筛查。

## 12. 风险点与规避方式

## 12.1 SPI 模式不一致

风险：

- `CPOL/CPHA` 和 EDA 侧不一致

表现：

- 包头错位
- 全包错
- 偶发性错字节

规避：

- 软件和 testbench 明确统一模式

## 12.2 DMA 长度和包长度不一致

风险：

- 发包长度不是 730
- DMA 配成别的长度

表现：

- 尾部缺失
- DMA 不完成

规避：

- EDA 和软件都固定使用 `PKT_LEN = 730`

## 12.3 AHB 窗口本身不可写

风险：

- `0x18000000` 访问权限或路径有问题

规避：

- 先做用例二作为基线

## 12.4 直接 DMA 到 AHB 窗口导致问题难分

风险：

- 无法区分 DMA 是否收到正确数据

规避：

- 第一版严格执行 `DMA -> SRAM -> CPU copy -> AHB`

## 13. 推荐验收顺序

建议验收顺序：

1. `core1` 直写 `0x18000000` 通过
2. `core0` SPI RX DMA 到 SRAM 通过
3. `core0` SRAM 写入 `0x18000000` 并回读通过
4. 全链路用例稳定通过

## 14. 后续可扩展方向

在第一版稳定后，可以继续扩展：

- DMA 直接写 `0x18000000`
- 支持 `qspi3`
- 用中断替代 DMA 完成轮询
- 增加 CRC 校验
- 增加写 `0x18004000`、`0x18008000` 的变体
- 增加 730 字节以外的边界长度测试

## 15. 结论

本方案建议新增两个测试例：

1. `core0`：`SPI RX DMA -> SRAM -> 0x18000000`
2. `core1`：直接写 `0x18000000`

并采用以下策略保证可调试性：

- 先做主核直写窗口基线
- 再做从核 SPI RX DMA 到 SRAM
- 最后做 SRAM 到 AHB 窗口写入

其中，用例一必须同时验证两层数据：

- DMA 收到的 SRAM 数据
- 写入 `0x18000000` 后的回读数据

这是本方案的核心验收要求。

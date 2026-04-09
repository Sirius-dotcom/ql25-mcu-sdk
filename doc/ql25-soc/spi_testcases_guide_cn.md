# QL25 SPI 测试用例说明

## 1. 文档目的

本文档整理 `soc_test_cases/ns_core0/SPI/qspi_xip0` 目录下的测试用例，说明每个用例的：

- 测试目标
- 基本原理
- 连接方式
- 结果判定方法
- 适用场景

本文档内容已按 UTF-8 中文重写，当前先覆盖 `qspi_xip0` 相关用例。

## 2. qspi_xip0 概述

`qspi_xip0` 与 `qspi1/qspi2/qspi3` 的定位不同。前者主要面向片上 XIP 控制器访问外部存储器，验证重点不是普通 SPI 主从对接，而是：

- 通过 `QSPI_XIP0` 访问外部 Flash / PSRAM
- 普通 SPI 模式读写
- XIP 方式内存映射读写
- 单线 / 双线 / 四线访问
- DDR 读模式
- Cache 场景下的 XIP 访问一致性
- 控制器寄存器可访问性

目录位置：

- `soc_test_cases/ns_core0/SPI/qspi_xip0/reg_test`
- `soc_test_cases/ns_core0/SPI/qspi_xip0/spi_xip_flash_wr`
- `soc_test_cases/ns_core0/SPI/qspi_xip0/spi_xip_flash_wr_cache`
- `soc_test_cases/ns_core0/SPI/qspi_xip0/spi_xip_flash_ddr_wr`
- `soc_test_cases/ns_core0/SPI/qspi_xip0/spi_xip_psram`

## 3. 总体连接说明

### 3.1 连接外部 Flash 时

- `QSPI_XIP0SCK -> Flash SCK`
- `QSPI_XIP0CS -> Flash CS`
- `QSPI_XIP0DQ0 -> Flash DQ0 / IO0`
- `QSPI_XIP0DQ1 -> Flash DQ1 / IO1`
- `QSPI_XIP0DQ2 -> Flash DQ2 / IO2`
- `QSPI_XIP0DQ3 -> Flash DQ3 / IO3`

### 3.2 连接外部 PSRAM 时

- `QSPI_XIP0SCK -> PSRAM SCK`
- `QSPI_XIP0CS -> PSRAM CS`
- `QSPI_XIP0DQ0 -> PSRAM DQ0`
- `QSPI_XIP0DQ1 -> PSRAM DQ1`
- 如启用四线模式，额外连接 `QSPI_XIP0DQ2`、`QSPI_XIP0DQ3`

### 3.3 结果观察方法

大部分用例都采用以下方式判定：

- 串口打印比较结果，例如 `cmp pass` / `cmp error`
- 仿真中调用 `simulation_pass()` / `simulation_fail()`
- 某些用例会先打印器件 ID，用于确认外设已正常响应

## 4. 用例逐项说明

## 4.1 reg_test

### 测试目标

验证 `QSPI_XIP0` 控制器寄存器的可访问性、复位值和基础读写路径。

### 测试原理

`reg_test.c` 会逐项访问 XIP 控制器寄存器，覆盖的寄存器包括但不限于：

- `SCKDIV`
- `SCKMODE`
- `DDR_SCKSAMPLE`
- `FORCE`
- `CSID`
- `CSDEF`
- `CSMODE`
- `VERSION`
- `ADDR_WRAP`
- `BOUNDARY_CFG`
- `DELAY0`
- `DELAY1`
- `FIFO_NUM`
- `TXDATA`
- `TX_MARK`
- `RX_MARK`
- `FCTRL`
- `IE`
- `SDR_SCKSAMPLE`

其核心目的是确认：

- 寄存器地址映射正确
- 读写字段行为符合预期
- 时钟、复位、控制字段具备基本可用性

### 结果判定

- 串口输出寄存器测试通过/失败
- 失败时调用 `simulation_fail()`
- 全部通过时调用 `simulation_pass()`

### 测试命令

```bash
ctest_gun -t SPI_qspi_xip0_reg_test --bm=ilm
make run TESTNAME=SPI_qspi_xip0_reg_test_ilm
```

### 适用场景

- XIP 控制器 bring-up 初期
- 换版后先确认寄存器层没有基础问题

## 4.2 spi_xip_flash_wr

### 测试目标

验证 `QSPI_XIP0` 对外部 Flash 的普通 SPI 读写能力和 XIP 映射读能力。

### 测试原理

该用例分两部分：

1. 普通 SPI 模式
   - 读取 JEDEC ID
   - 擦除扇区
   - 写入一页数据
   - 分别用单线、双线、四线方式读回并比较
2. XIP 模式
   - 配置 XIP 读时序
   - 通过 `memcpy(read, (void *)QSPI_XIP0_MEM_BASE, PAGESIZE)` 直接从 XIP 地址空间取数
   - 分别验证单线、双线、四线 XIP 读回数据

### 主要覆盖点

- Flash 基本命令收发
- JEDEC ID 读取
- 单线普通读写
- 双线快速读
- 四线快速读
- XIP 地址映射读
- `QSPI_XIP_TimingConfig()` 的时序配置

### 关键日志

代码中会打印如下典型日志：

- `JEDEC[x]:`
- `1 line data cmp pass`
- `2 line data cmp pass`
- `4 line data cmp pass`
- `1 line XIP read data cmp pass`
- `2 line XIP read data cmp pass`
- `4 line XIP read data cmp pass`

任一比较失败都会打印 `cmp error` 并调用 `simulation_fail()`

### 测试命令

```bash
ctest_gun -t SPI_qspi_xip0_spi_xip_flash_wr --bm=ilm
make run TESTNAME=SPI_qspi_xip0_spi_xip_flash_wr_ilm
```

### 适用场景

- 验证 XIP Flash 基础连线
- 验证从普通 SPI 模式切换到 XIP 模式是否正常
- 验证单/双/四线读取链路

## 4.3 spi_xip_flash_wr_cache

### 测试目标

验证在 Cache 相关场景下，XIP Flash 读写与数据一致性是否正常。

### 测试原理

该用例仍然基于外部 Flash，但重点不再是普通 SPI 多协议读写，而是：

- 将数据写入外部 Flash
- 配置 XIP 访问路径
- 在 Cache 开启/刷新/失效相关流程下执行 XIP 读回比较

源码中对 cache line 和 cache flush/invalidate 做了专门处理，说明这个用例主要用于验证：

- XIP 读路径和 cache 的交互
- 开 cache 后是否能正确拿到最新 Flash 内容

### 关键日志

- `JEDEC[x]:`
- `4 line XIP read data cmp pass`

### 说明

代码中还有一条注释：

`The soc doesn't have a dcache, so there is no need to care about this case.`

因此如果当前 SoC 配置下没有 D-Cache，这个用例更多是保留验证路径，实际影响会比较有限。

### 测试命令

```bash
ctest_gun -t SPI_qspi_xip0_spi_xip_flash_wr_cache --bm=ilm
make run TESTNAME=SPI_qspi_xip0_spi_xip_flash_wr_cache_ilm
```

### 适用场景

- 带 Cache 配置的系统验证
- 后续 SoC / CPU 配置变化后确认 XIP 一致性

## 4.4 spi_xip_flash_ddr_wr

### 测试目标

验证 `QSPI_XIP0` 在 Flash DDR 访问模式下的读写与 XIP 读取能力。

### 测试原理

该用例对外部 Flash 做如下验证：

- 初始化 `QSPI_XIP0`
- 读取 JEDEC ID
- 普通四线读写比较
- 配置 DDR 相关寄存器和时序
- 通过 XIP 地址空间执行 DDR 读
- 对读回数据做一致性比较

代码中可见以下 DDR 相关控制：

- `QSPI_XIP_CR_DDR_ENABLE`
- `QSPI_XIP_DdrEnableTypedef`
- `CMD_DDR_EN / ADDR_DDR_EN / DATA_DDR_EN`

说明该用例的重点是验证 DDR 数据相位和时序配置是否正确。

### 关键日志

- `JEDEC:`
- `4 line data cmp pass`
- `4 line XIP read data cmp pass`

### 测试命令

```bash
ctest_gun -t SPI_qspi_xip0_spi_xip_flash_ddr_wr --bm=ilm
make run TESTNAME=SPI_qspi_xip0_spi_xip_flash_ddr_wr_ilm
```

### 适用场景

- 验证 Flash DDR 模式支持
- 验证高性能读取链路
- DDR 时序调试和板级连线确认

## 4.5 spi_xip_psram

### 测试目标

验证 `QSPI_XIP0` 对外部 PSRAM 的普通 SPI 访问和 XIP 模式访问能力。

### 测试原理

该用例包含两部分：

1. 普通 SPI 模式
   - 读取 PSRAM ID
   - 单线写、单线读并比较
   - 进入 Quad 模式
   - 四线写、四线读并比较
2. XIP 模式
   - 配置 XIP 写时序和读时序
   - 通过 `memcpy((void *)QSPI_XIP0_MEM_BASE, write, PAGESIZE)` 执行 XIP 写
   - 再通过 `memcpy(read, (void *)QSPI_XIP0_MEM_BASE, PAGESIZE)` 执行 XIP 读
   - 对结果做比较

### 主要覆盖点

- PSRAM 器件 ID 读取
- 单线 SPI 读写
- 四线 SPI 读写
- 进入/退出 Quad 模式
- XIP 写
- XIP 读

### 关键日志

- `MFID:`
- `KGD:`
- `EID:`
- `1 line data cmp pass`
- `4 line data cmp pass`
- `4 line XIP read data cmp pass`

### 注意事项

源码中 `spi_xip_mode_test()` 的比较语句写成了：

```c
if (0 == memcmp(write, read, 0))
```

按代码字面理解，这一比较长度为 `0`，会恒等返回相等。  
因此如果你要把这个用例作为严格回归判据，建议后续把它修正为 `memcmp(write, read, PAGESIZE)`。

### 测试命令

```bash
ctest_gun -t SPI_qspi_xip0_spi_xip_psram --bm=ilm
make run TESTNAME=SPI_qspi_xip0_spi_xip_psram_ilm
```

### 适用场景

- 验证 XIP 控制器对外部 PSRAM 的支持
- 验证内存映射访问路径
- 验证四线 PSRAM 读写链路

## 5. 运行建议

建议按如下顺序验证：

1. `reg_test`
2. `spi_xip_flash_wr`
3. `spi_xip_flash_wr_cache`
4. `spi_xip_flash_ddr_wr`
5. `spi_xip_psram`

推荐原因：

- 先确认寄存器层没有问题
- 再确认普通 Flash 访问和 XIP 访问
- 再验证 Cache / DDR / PSRAM 这些扩展路径

## 6. 小结

`qspi_xip0` 这一组测试和普通 `qspi1/2/3` 的主从通信类用例不同，它更偏向“存储访问控制器”验证。核心关注点是：

- 控制器寄存器是否正常
- 外部 Flash / PSRAM 是否正确连线
- 普通 SPI 模式是否可用
- XIP 地址映射访问是否可用
- 四线和 DDR 模式是否可用
- Cache 打开后是否还能保证访问一致性

如果后续需要，我可以继续把 `qspi1/qspi2/qspi3` 的说明也按同样风格整理成 UTF-8 中文版。

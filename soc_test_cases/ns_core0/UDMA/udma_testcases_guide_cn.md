# UDMA 测试例说明

## 1. 目录说明

当前 `soc_test_cases/ns_core0/UDMA` 目录下的测试例主要集中在 `udma0`，内容以 `M2M` 搬运模式和 `reg_test` 为主。

相关目录：

- `soc_test_cases/ns_core0/UDMA/udma0/reg_test`
- `soc_test_cases/ns_core0/UDMA/udma0/M2m`
- `soc_test_cases/ns_core0/UDMA/udma0/M2m_burst`
- `soc_test_cases/ns_core0/UDMA/udma0/M2m_int`
- `soc_test_cases/ns_core0/UDMA/udma0/M2m_continue_mode`
- `soc_test_cases/ns_core0/UDMA/udma0/M2m_fixed_addr`
- `soc_test_cases/ns_core0/UDMA/udma0/M2m_src_c_dst_f`
- `soc_test_cases/ns_core0/UDMA/udma0/M2m_src_f_dst_c`
- `soc_test_cases/ns_core0/UDMA/udma0/M2m_repeat_mode`
- `soc_test_cases/ns_core0/UDMA/udma0/M2m_repeat_mode_trigger_last_transfer_irq`
- `soc_test_cases/ns_core0/UDMA/udma0/M2m_repeat_mode_addr_acc_increasing`
- `soc_test_cases/ns_core0/UDMA/udma0/M2m_lla`
- `soc_test_cases/ns_core0/UDMA/udma0/M2m_lla_multi_nodes`

## 2. 是否已有“往 AHB 总线写”的 DMA 用例

结论：`当前 UDMA 目录下没有明确面向 AHB 从设备寄存器/固定外设地址写入的专用测试例。`

现有 `M2M` 用例虽然有“固定地址”场景，例如：

- `M2m_fixed_addr`
- `M2m_src_c_dst_f`
- `M2m_src_f_dst_c`

但这些用例的源地址和目的地址都仍然放在内部存储地址窗口，例如：

- `SRC_DATA_ACC_ADDR_BASE = 0x08000000 + 32768 - 0x200`
- `DES_DATA_ACC_ADDR_BASE = 0x08000000 + 32768 - 0x100`

也就是说，这些 case 验证的是：

- 地址自增/固定模式
- 数据宽度转换
- 中断
- repeat 模式
- LLA 链表模式

它们并不是“DMA 向某个 AHB 外设寄存器地址持续写入”或“DMA 从某个 AHB slave 固定地址读写”的专门验证。

如果要找“DMA 往 AHB 从设备写”的思路，`SPI` 目录下的某些 case 更接近，例如把 DMA 目的地址指向 AHB slave 映射地址。但那不在当前 `UDMA` 测试目录内。

## 3. 测试例总览

### 3.1 reg_test

- 目录：`udma0/reg_test`
- 功能：验证 UDMA 寄存器访问、时钟、复位、配置字段等基础功能
- 适用场景：确认 UDMA 模块本身寄存器接口正常
- FPGA 清单：不在 `udma.fpga_ctestlist` 中

测试命令：

```bash
ctest_gun -t UDMA_udma0_reg_test --bm=ilm
make run TESTNAME=UDMA_udma0_reg_test_ilm
```

### 3.2 M2m

- 目录：`udma0/M2m`
- 功能：基础内存到内存搬运
- 地址模式：源地址递增，目的地址递增
- 判定方法：DMA 完成后比较源 buffer 和目的 buffer

测试命令：

```bash
ctest_gun -t UDMA_udma0_M2m --bm=ilm
make run TESTNAME=UDMA_udma0_M2m_ilm
```

### 3.3 M2m_burst

- 目录：`udma0/M2m_burst`
- 功能：验证 burst 搬运模式
- 地址模式：源地址递增，目的地址递增
- 关键点：开启 `UDMA_BurstConfig()`

测试命令：

```bash
ctest_gun -t UDMA_udma0_M2m_burst --bm=ilm
make run TESTNAME=UDMA_udma0_M2m_burst_ilm
```

### 3.4 M2m_int

- 目录：`udma0/M2m_int`
- 功能：验证半传输和全传输中断
- 地址模式：源地址递增，目的地址递增
- 关键点：使能 `UDMA_HTRANS_IRQ` 和 `UDMA_FTRANS_IRQ`

测试命令：

```bash
ctest_gun -t UDMA_udma0_M2m_int --bm=ilm
make run TESTNAME=UDMA_udma0_M2m_int_ilm
```

### 3.5 M2m_continue_mode

- 目录：`udma0/M2m_continue_mode`
- 功能：验证 continue 模式下连续传输
- 地址模式：源地址递增，目的地址递增
- 判定方法：统计完成次数并检查目的数据

测试命令：

```bash
ctest_gun -t UDMA_udma0_M2m_continue_mode --bm=ilm
make run TESTNAME=UDMA_udma0_M2m_continue_mode_ilm
```

### 3.6 M2m_fixed_addr

- 目录：`udma0/M2m_fixed_addr`
- 功能：验证固定地址模式
- 地址模式：单次传输时源地址固定、目的地址固定；每轮完成后软件把首地址加 1，再重新启动下一轮
- 说明：这是“固定地址模式”测试，不是 AHB 外设地址测试

测试命令：

```bash
ctest_gun -t UDMA_udma0_M2m_fixed_addr --bm=ilm
make run TESTNAME=UDMA_udma0_M2m_fixed_addr_ilm
```

### 3.7 M2m_src_c_dst_f

- 目录：`udma0/M2m_src_c_dst_f`
- 功能：验证“源地址递增、目的地址固定”
- 地址模式：
  - 源地址递增
  - 目的地址固定
- 数据效果：最终固定目的地址处保留最后一次写入的数据
- 说明：很接近“往固定地址写”，但地址仍是内部存储，不是专门的 AHB 外设口

测试命令：

```bash
ctest_gun -t UDMA_udma0_M2m_src_c_dst_f --bm=ilm
make run TESTNAME=UDMA_udma0_M2m_src_c_dst_f_ilm
```

### 3.8 M2m_src_f_dst_c

- 目录：`udma0/M2m_src_f_dst_c`
- 功能：验证“源地址固定、目的地址递增”
- 地址模式：
  - 源地址固定
  - 目的地址递增
- 数据效果：目的 buffer 中所有位置都应等于固定源地址处的数据

测试命令：

```bash
ctest_gun -t UDMA_udma0_M2m_src_f_dst_c --bm=ilm
make run TESTNAME=UDMA_udma0_M2m_src_f_dst_c_ilm
```

### 3.9 M2m_repeat_mode

- 目录：`udma0/M2m_repeat_mode`
- 功能：验证 repeat 模式
- 地址模式：每次传输内部按配置搬运，但每轮重新开始时起始地址固定
- 关键点：使用 `UDMA_RptConfig()`

测试命令：

```bash
ctest_gun -t UDMA_udma0_M2m_repeat_mode --bm=ilm
make run TESTNAME=UDMA_udma0_M2m_repeat_mode_ilm
```

### 3.10 M2m_repeat_mode_trigger_last_transfer_irq

- 目录：`udma0/M2m_repeat_mode_trigger_last_transfer_irq`
- 功能：验证 repeat 模式下只在最后一次传输触发目标中断
- 关键点：同时关注 repeat 和中断行为

测试命令：

```bash
ctest_gun -t UDMA_udma0_M2m_repeat_mode_trigger_last_transfer_irq --bm=ilm
make run TESTNAME=UDMA_udma0_M2m_repeat_mode_trigger_last_transfer_irq_ilm
```

### 3.11 M2m_repeat_mode_addr_acc_increasing

- 目录：`udma0/M2m_repeat_mode_addr_acc_increasing`
- 功能：验证 repeat 模式下，每轮的数据搬运地址也持续递增
- 关键点：对比普通 repeat 模式，此用例更强调地址累计变化

测试命令：

```bash
ctest_gun -t UDMA_udma0_M2m_repeat_mode_addr_acc_increasing --bm=ilm
make run TESTNAME=UDMA_udma0_M2m_repeat_mode_addr_acc_increasing_ilm
```

### 3.12 M2m_lla

- 目录：`udma0/M2m_lla`
- 功能：验证单链表配置下的 LLA 传输
- 关键点：使能链表后，当前传输结束自动装载下一组配置

测试命令：

```bash
ctest_gun -t UDMA_udma0_M2m_lla --bm=ilm
make run TESTNAME=UDMA_udma0_M2m_lla_ilm
```

### 3.13 M2m_lla_multi_nodes

- 目录：`udma0/M2m_lla_multi_nodes`
- 功能：验证多节点链表传输
- 关键点：连续执行多组链表节点配置，检查最后结果是否正确

测试命令：

```bash
ctest_gun -t UDMA_udma0_M2m_lla_multi_nodes --bm=ilm
make run TESTNAME=UDMA_udma0_M2m_lla_multi_nodes_ilm
```

## 4. FPGA 清单与全量清单差异

`udma.ctestlist` 包含完整用例集合，`udma.fpga_ctestlist` 只保留了部分 case。

当前未放入 FPGA 清单的用例包括：

- `UDMA_udma0_reg_test`
- `UDMA_udma0_M2m_repeat_mode_trigger_last_transfer_irq`
- `UDMA_udma0_M2m_repeat_mode_addr_acc_increasing`
- `UDMA_udma0_M2m_lla`
- `UDMA_udma0_M2m_lla_multi_nodes`

## 5. 如果需要补“DMA 往 AHB 总线写”的专用用例，建议方向

建议单独新增一个 case，而不是直接复用现有 `M2M` 目录。

建议特征：

- 源地址使用普通 SRAM buffer
- 目的地址使用固定 AHB slave 地址或外设 FIFO/寄存器地址
- `DstInc` 关闭
- 根据目标外设要求配置传输宽度
- 增加超时和错误中断检查
- 判定方式使用：
  - AHB slave 存储窗口回读
  - 外设状态寄存器
  - 二次 DMA 回读对比

如果后续要补这类 case，命名可以参考：

- `udma0/M2m_to_ahb_slv`
- `udma0/M2m_to_ahb_fixed_addr`


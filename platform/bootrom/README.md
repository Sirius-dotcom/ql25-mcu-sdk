# QL25 BootROM README

## 1. 文档目的

本文档说明当前 QL25 `platform/bootrom` 的定位、启动职责、构建方式、配置档位、硬件前提和遗留确认项。

本文档对应的是**当前已经收敛后的 BootROM 方案**：

- BootROM 只负责最小启动闭环
- BootROM 只负责启动第二阶段 BootLoader
- 更复杂的升级、Flash 管理和调试能力建议放入 BootLoader

## 2. 当前 BootROM 定位

当前 BootROM 的职责是：

- 上电后执行最小初始化
- 保持 `Core0` 处于复位保持
- 初始化 `USART0`
- 初始化 `QSPI_XIP0`
- 根据 `BOOT_PIN` 选择启动模式
- 从 Flash 中查找并启动 BootLoader
- 在 Flash 启动失败时 fallback 到 UART 下载

当前 BootROM **不是**最终业务应用的完整启动管理器，也**不建议**继续承载以下能力：

- Flash probe
- 在线升级
- Flash 擦除 / 写入 / 回读管理
- 回滚策略细化
- 多 Flash 型号兼容表
- 复杂日志和大量调试命令

以上能力建议放在 BootLoader 中实现。

## 3. 推荐启动链路

当前推荐启动链路如下：

```text
上电复位
-> BootROM
-> 初始化最小启动环境
-> 选择启动模式
-> 从 Flash / UART 拉起 BootLoader
-> BootLoader 执行升级、恢复、镜像管理
-> BootLoader 启动最终业务应用
```

## 4. Flash 布局语义

当前 BootROM 仍采用 A/B 结构，但其语义已经明确为 **BootLoader A/B**：

```text
Flash 2MB
0x20000000 ~ 0x200003FF : Partition Table (1KB)
0x20000400 ~ 0x200FFFFF : BootLoader Slot A
0x20100000 ~ 0x201FFFFF : BootLoader Slot B
```

也就是说，当前 BootROM 加载的是：

- `BootLoader Slot A`
- 或 `BootLoader Slot B`

而不是最终业务应用镜像。

## 5. 当前保留能力

当前 BootROM 仍保留以下能力：

- `BOOT_PIN` 启动模式选择
- Flash A/B 启动
- 分区表基础校验
- 镜像头校验
- payload CRC 校验
- UART 下载兜底
- JTAG 调试等待模式

这些能力保留的原因是：

- 仍然适合当前 RTL 联调
- 仍然适合板级 bring-up
- 不会阻碍后续将复杂逻辑迁到 BootLoader

## 6. `QSPI_XIP0` 初始化说明

SDK 通用 startup 不会初始化 `QSPI_XIP0`。

因此当前 BootROM 已内置 `bootrom_qspi_init()`，用于在启动阶段完成最小 XIP0 初始化。

当前 `bootrom_qspi_init()` 采用的策略是：

- single line
- SDR
- 3-byte address
- `0x03 Read Data`

选择该配置的原因：

- 兼容性最高
- 适合首版启动闭环
- 适合 RTL bring-up

### 6.1 `FFMT/FFMT1` 的选择依据

`FFMT/FFMT1` 需要和以下三项匹配：

1. Flash 厂商和型号支持的读命令
2. 板级连线方式
3. RTL Flash model 的实现

如果 Flash model 不支持当前的 `0x03 + 3-byte address` 读方式，则需要修改 `bootrom_qspi_init()` 中的 `FFMT/FFMT1` 配置。

## 7. 关于 Flash probe 的结论

如果后续要支持在线升级和 Flash 烧写，可以增加 `probe` 功能。

这里的 `probe` 指：

- 通过 `0x9F` 等命令读取 JEDEC ID
- 识别 Flash 厂商、型号、容量
- 根据识别结果选择合适的读命令、dummy cycle、页大小、擦除粒度、4-byte address 模式、QE 位处理方式等

但当前不建议把 `probe` 做进最小 BootROM，原因是：

- BootROM 空间有限
- BootROM 应尽量保持最小启动能力
- probe 和烧写支持会显著增加复杂度

因此当前建议是：

- BootROM：固定使用最保守读法完成启动
- BootLoader：负责 Flash probe、升级、擦写、回滚、镜像管理

## 8. BootROM 双配置说明

当前 BootROM 已支持两种构建配置：

### 8.1 `PROFILE=debug`

用途：

- RTL 联调
- 真板 bring-up
- 启动问题定位

特点：

- 保留 BootROM Shell
- 保留 `md` / `mw` / `info` / `go` / `boot` / `help`
- 保留较完整启动日志
- 保留 A/B、UART 兜底、CRC

构建命令：

```bash
make CROSS_COMPILE=riscv64-unknown-elf- PROFILE=debug
```

产物：

- `bootrom_debug.elf`
- `bootrom_debug.bin`
- `bootrom_debug.map`

当前编译结果：

- `bootrom_debug.bin = 5352 bytes`

### 8.2 `PROFILE=minimal`

用途：

- 量产前最小启动路径验证
- ROM 空间收敛

特点：

- 去掉 BootROM Shell
- 去掉大部分调试字符串
- 保留最小启动路径
- 保留 A/B、UART 兜底、CRC

构建命令：

```bash
make CROSS_COMPILE=riscv64-unknown-elf- PROFILE=minimal
```

产物：

- `bootrom_minimal.elf`
- `bootrom_minimal.bin`
- `bootrom_minimal.map`

当前编译结果：

- `bootrom_minimal.bin = 1848 bytes`

## 9. 构建方法

进入目录：

```bash
cd platform/bootrom
```

构建 debug 版：

```bash
make CROSS_COMPILE=riscv64-unknown-elf- PROFILE=debug
```

构建 minimal 版：

```bash
make CROSS_COMPILE=riscv64-unknown-elf- PROFILE=minimal
```

清理：

```bash
make clean
```

## 10. 当前已验证的构建产物

当前已实际编译通过并生成以下文件：

- `bootrom_debug.elf`
- `bootrom_debug.bin`
- `bootrom_debug.map`
- `bootrom_minimal.elf`
- `bootrom_minimal.bin`
- `bootrom_minimal.map`

## 11. 当前仍需确认的硬件 / RTL 问题

以下内容仍需在硬件或 RTL 侧确认：

- `BOOT_PIN0_BIT`
- `BOOT_PIN1_BIT`
- `BOOT_PIN_GPIO_BASE`
- `LGPIO_IVAL_OFS`
- `LGPIO_IEN_OFS`
- Flash model 是否支持当前 `0x03 + 3-byte address`
- Flash 容量和当前 slot 布局是否一致
- `BOOTROM_BASE` 是否确实映射到主核复位入口
- 上电默认时钟是否与当前波特率假设一致

如果以上条件未确认，BootROM 仍可用于代码联调，但不应直接作为最终 RTL 启动验收基线。

## 12. 当前不再适用的旧理解

以下理解对当前版本已经不再适用：

- “BootROM 直接负责最终业务应用的完整启动管理”
- “BootROM 应该继续堆叠 Flash probe、升级、擦写和回滚策略”
- “BootROM 里的 Shell 应长期作为正式量产能力”

当前更合理的边界是：

- BootROM 负责最小启动
- BootLoader 负责复杂管理

## 13. 建议的 QL25 BootLoader Shell 命令集

BootLoader 比 BootROM 更适合承载完整 Shell。建议命令集如下。

### 13.1 基础命令

- `help`
  显示命令帮助
- `version`
  显示 BootLoader 版本、构建时间、启动原因
- `reboot`
  复位系统
- `boot`
  启动默认业务镜像

### 13.2 内存与寄存器命令

- `md <addr> [len]`
  内存或寄存器查看
- `mw <addr> <value>`
  内存或寄存器写入
- `go <addr>`
  跳转到指定地址执行

### 13.3 Flash 管理命令

- `flash probe`
  读取 JEDEC ID，识别 Flash 厂商和型号
- `flash info`
  显示 Flash 容量、sector size、page size、当前读模式
- `flash read <addr> <len>`
  读取 Flash 内容
- `flash erase <addr> <len>`
  擦除指定区域
- `flash write <addr> <src> <len>`
  将内存中的数据写入 Flash
- `flash verify <addr> <src> <len>`
  比较 Flash 和内存内容

### 13.4 镜像管理命令

- `img info`
  显示当前 A/B 镜像头、版本、CRC、状态
- `img verify <slot>`
  校验指定 slot 的镜像头和 payload
- `img boot <slot>`
  从指定 slot 启动
- `img active <slot>`
  设置 active slot
- `img rollback`
  触发回滚到备用 slot

### 13.5 升级命令

- `upgrade uart`
  从 UART 接收升级包
- `upgrade xmodem`
  通过 XMODEM 接收镜像
- `upgrade slot <slot>`
  将接收镜像写入指定 slot
- `upgrade verify`
  校验升级结果
- `upgrade commit`
  提交升级结果，更新 active slot

### 13.6 调试与诊断命令

- `log level <n>`
  设置日志级别
- `crc <addr> <len>`
  计算内存区 CRC
- `dump part`
  查看分区表
- `dump boot`
  查看启动信息和最近失败原因

## 14. BootLoader Shell 的建议分档

建议 BootLoader 也采用分档策略：

### debug 版

- 保留完整 shell
- 保留升级和 Flash 命令
- 保留详细日志

### release 版

- 默认关闭危险命令
- 可只保留 `help`、`version`、`boot`、`img info`
- 擦写类命令仅在工厂模式或授权模式下开放

## 15. 结论

当前 QL25 BootROM 方案已经收敛为：

- BootROM 只负责最小启动
- BootROM 只负责拉起 BootLoader
- Flash probe、在线升级、复杂 shell 都应放入 BootLoader

当前建议：

- 联调阶段使用 `PROFILE=debug`
- ROM 收敛和量产评估阶段使用 `PROFILE=minimal`
- 下一阶段新增独立 BootLoader 工程，并实现建议的 BootLoader Shell 命令集

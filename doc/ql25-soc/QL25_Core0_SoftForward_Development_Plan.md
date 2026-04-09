# QL25 Core0 软转发开发计划

## 1. 目标

本文档面向当前 `ql25-mcu-sdk` 实际代码基线，规划 Core0 数据面软转发开发路径。

目标范围：

- 基于 QL25 双核 AMP 架构，在 Core0 上实现软 MAC 收发、分片解析、重组、转发输出。
- 复用 SDK 现有驱动与已落地平台基础，不重复建设 BootROM、IPC、OSAL 已完成部分。
- 保持 owner 明确：Core0 负责 MAC/PHY/快路径，Core1 负责管理、配置、状态汇聚。

不在本计划首阶段内的内容：

- Core1 完整管理面业务
- 全量平台 API 整理
- 大范围重构现有 `platform/` 目录

## 2. 当前基线结论

结合仓库现状，当前可直接复用的基础如下：

- `platform/ipc/` 已存在，已按共享内存 + IDU 方向实现基础 IPC。
- `platform/osal/` 已存在，已有 FreeRTOS 抽象层。
- `platform/bootrom/` 已存在，说明启动链路已有独立推进。
- `soc_test_cases/ns_core0/Additional_Cases/UTB/mac_phy_loopback/` 已验证 UTB MAC/PHY 回环。

从现有测试代码可确认的硬件访问事实：

- MAC 基地址：`0x18000000`
- `mac_tx_symb`：`0x18000100`
- `mac_rx_symb`：`0x18000104`
- 当前已验证用法：将 `mac_tx_symb` / `mac_rx_symb` 视为 8-bit streaming MMIO port，通过 UDMA M2M 对固定地址做单字节搬运。

当前缺口：

- 仓库中尚无 Core0 软 MAC 驱动封装层
- 尚无 Core0 转发任务/状态机/重组模块
- 尚无与软 MAC 中断、统计寄存器、异常恢复对应的软件框架
- 尚无面向 Core0 转发面的独立联调工程

## 3. 开发原则

- 先做最小闭环，再扩展功能，不直接上完整业务。
- 快路径优先静态内存，不引入动态分配。
- ISR 只做确认、搬运完成通知、错误快照，不做重组和复杂解析。
- Core0 私有快路径状态不暴露给 Core1，跨核只走 IPC 快照和事件。
- 先按轮询/受控中断把链路跑通，再决定是否做更激进的 DMA/并行优化。

## 4. 软件落点

建议新增或扩展以下模块：

- `platform/utb_hal/include/utb_hal_mac.h`
- `platform/utb_hal/src/utb_hal_mac.c`
- `platform/utb_port/include/utb_fwd.h`
- `platform/utb_port/src/utb_fwd.c`
- `platform/utb_port/include/utb_reasm.h`
- `platform/utb_port/src/utb_reasm.c`
- `platform/utb_port/include/utb_slice.h`
- `platform/utb_port/src/utb_slice.c`
- `application/freertos/ql25_utb_fwd_demo/`

目录职责建议：

- `utb_hal_mac`: MAC 寄存器、`mac_tx_symb`/`mac_rx_symb`、中断状态、DMA 提交
- `utb_slice`: 单片格式解析、长度检查、首片/中间片/尾片判定
- `utb_reasm`: 31 路重组槽、超时、异常回收
- `utb_fwd`: Core0 转发主状态机与任务编排
- `ql25_utb_fwd_demo`: Core0 联调工程，先单核/后双核

## 5. 分阶段计划

### Phase 0：软 MAC 驱动最小化落地

目标：

- 建立 MAC 寄存器定义、位域宏、只读/写一清接口
- 封装 `mac_tx_symb`、`mac_rx_symb` 的 CPU/UDMA 访问
- 封装 `mac_int_status`、`mac_int_clr_mask`、统计寄存器读取

交付：

- `utb_hal_mac.h/.c`
- `utb_mac_regs.h`
- 基于现有 `mac_phy_loopback` 提炼的 HAL 回环自测接口

验收：

- 软件可初始化 `mac_en`、`fixed_length`、`ahb_symb_timer`
- 可读出中断与 DFX 统计寄存器
- CPU 模式和 UDMA 模式都能完成 `tx_symb -> phy -> rx_symb` 回环

### Phase 1：Core0 接收闭环

目标：

- 从 `mac_rx_symb` 收到符号流并形成软件可处理的 slice
- 建立最小 RX 状态机
- 错误分类至少覆盖：长度错、CRC 错、RX 超时、FIFO 满

实现要点：

- 第一版建议先用“中断唤醒 + 任务内读 FIFO/端口”的保守模式
- `mac_rx_state`、`mac_int_status` 作为状态观测面
- 不在 ISR 内做重组，仅发任务通知

验收：

- 可稳定接收单包
- 可正确识别有效 slice 与错误 slice
- 关键错误计数与寄存器统计趋势一致

### Phase 2：分片解析与重组

目标：

- 按设计文档实现分片头解析
- 建立 31 路静态重组槽
- 支持首片、中间片、尾片、重组超时、异常新首片覆盖

实现要点：

- 重组 metadata 放 Core0 DLM 或本地 SRAM 热区
- 大缓冲不放 ILM
- `BASIC_TIMER1` 或等效低频时基负责 GC 扫描

验收：

- 单路重组闭环通过
- 多路交错场景可工作
- 超时和乱序能有明确丢弃路径，不残留脏槽

### Phase 3：转发输出

目标：

- 将已完成重组的数据重新切片并发往 `mac_tx_symb`
- 打通 RX -> reasm -> forward -> TX 闭环

实现要点：

- 第一版先 CPU 提交，第二版再切 UDMA
- `mac_tx_symb` 固定地址写入继续遵循单字节 streaming 语义
- TX 超时、PHY 请求长度错、FIFO 满要有 bounded recovery

验收：

- Core0 本地完成端到端转发
- 环回或对侧激励下可对比输入输出一致性
- TX 异常不会卡死任务和 DMA 通道

### Phase 4：Core1/IPC 联调

目标：

- Core1 通过 IPC 配置 Core0 转发参数
- Core0 周期上报状态、统计、告警

验收：

- Core1 可下发 enable/disable、阈值、超时等控制
- Core0 可上报 ready、drop、timeout、reasm_active 等快照

### Phase 5：性能收敛

目标：

- 满足文档目标吞吐和稳定性
- 缩小 CPU 搬运开销，确定是否需要全链路 DMA 化

优化顺序建议：

- 优先减少 copy 次数
- 再优化中断频率
- 最后再评估多缓冲/DMA 双缓冲

验收：

- 吞吐达到目标值
- 长稳运行无死锁、无持续漏包
- 栈水位、静态内存、ILM 占用可量化

## 6. 关键技术决策

### 6.1 第一版收发策略

建议第一版采用：

- RX：中断置位 + 任务上下文读 `mac_rx_symb`
- TX：任务上下文写 `mac_tx_symb`
- UDMA 只用于阶段性优化和回环验证

原因：

- 先确认软 MAC 寄存器与异常语义
- 降低 DMA ownership、异常恢复、部分提交的复杂度
- 便于快速对齐设计文档中的状态机与统计寄存器

### 6.2 ILM/DLM 使用建议

- ILM 仅放热路径函数：解析、重组推进、切片发送核心逻辑
- DLM/本地 SRAM 放重组槽、descriptor、统计
- 报文缓冲放共享 SRAM 或外部 SRAM，不放 ILM

### 6.3 中断使用建议

优先关注的中断位：

- MCU 可读 AHB 切片
- MCU 可写 AHB 切片
- RX 超时
- TX 超时
- RX FIFO 满
- TX FIFO 满

策略：

- ISR 中读取 `mac_int_status`
- W1C 清中断
- 累加软件统计
- 按事件类型通知 RX/TX/错误处理任务

## 7. 里程碑

| 里程碑 | 完成标准 |
|---|---|
| M0 | `utb_hal_mac` 落地，寄存器访问和回环 HAL 自测通过 |
| M1 | Core0 单核 RX 闭环跑通 |
| M2 | 31 路重组模块跑通，超时回收正常 |
| M3 | Core0 单核转发闭环跑通 |
| M4 | Core0/Core1 IPC 联调完成 |
| M5 | 吞吐、稳定性、内存占用达到验收门槛 |

## 8. 当前需要确认的问题

以下问题建议在开始写 Core0 主代码前确认，否则实现中容易返工：

1. `mac_tx_symb` / `mac_rx_symb` 软件侧是否可以长期按 8-bit streaming port 语义使用，还是仅测试场景成立？
2. `mac_int_status[0]` / `[1]` 是否分别可直接作为 “可写一个切片” / “可读一个切片” 的稳定软件语义？
3. `fixed_length` 在当前产品协议里是否固定为全局统一值，还是会按模式动态改变？
4. RX 侧从 PHY 上来的 slice 格式里，分片头字段定义是否已经冻结？特别是 `frag_id`、首尾片标记、长度字段。
5. Core0 是否独占 UTB MAC/PHY 和相关 UDMA 通道，Core1 是否完全不碰这些资源？
6. 第一阶段是否允许先用轮询/任务收发跑通，而不是一开始就上完整中断 + DMA？
7. `D:\\H563_FreeRTOS_Test\\Src` 中可参考的分片重组代码，是否有你建议优先对照的文件名或模块名？

## 9. 建议的下一步

建议实际开发顺序如下：

1. 先把 `soc_test_cases/ns_core0/Additional_Cases/UTB/mac_phy_loopback` 抽象成 `utb_hal_mac`。
2. 在 Core0 单核工程里做“收一包、解析一包、丢弃或缓存一包”的最小 RX 闭环。
3. 再引入 31 路重组和超时回收。
4. 最后做 TX 转发和 Core1 联调。


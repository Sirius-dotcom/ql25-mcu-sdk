# QL25 BootROM 与 BootLoader 分层设计说明

## 1. 目的

本文用于明确 QL25 启动软件的分层边界，回答以下问题：

- BootROM 和 BootLoader 分别负责什么
- 当前 QL25 更适合哪种启动架构
- 后续如果要支持在线升级、Flash 烧写、回滚，应该把能力放在哪一层

结论先行：

- **BootROM** 负责最小启动闭环
- **BootLoader** 负责可升级、可演进的启动管理和在线升级能力
- 对 QL25 来说，推荐采用 **BootROM + BootLoader** 两级启动架构

## 2. 基本定义

### 2.1 BootROM

BootROM 是芯片上电复位后执行的第一段代码，固化在片内 ROM 中。

特点：

- 上电即执行
- 不易修改
- 容量通常较小
- 必须足够稳定
- 应尽量减少策略复杂度

### 2.2 BootLoader

BootLoader 是由 BootROM 拉起的第二阶段软件，一般存放在 Flash 中。

特点：

- 可升级
- 容量相对更宽松
- 可以承载更复杂的策略和流程
- 适合持续演进

## 3. 推荐启动链路

推荐采用如下两级启动流程：

```text
上电复位
-> BootROM
-> 初始化最小启动环境
-> 选择启动源
-> 加载并跳转 BootLoader
-> BootLoader 初始化外设和存储策略
-> 选择业务镜像
-> 加载并跳转业务应用
```

如果需要在线升级，则流程扩展为：

```text
上电复位
-> BootROM
-> 加载 BootLoader
-> BootLoader probe Flash
-> 检查升级标志 / 回滚标志 / active slot
-> 必要时擦写或恢复镜像
-> 校验业务镜像
-> 启动业务应用
```

## 4. 分层职责建议

### 4.1 BootROM 职责

BootROM 只应保留“最小可启动”能力，建议包括：

- 读取 `BOOT_PIN`
- 最小串口初始化
- 最小 `QSPI_XIP0` 初始化
- 选择启动模式
- 读取镜像头
- 做基础合法性检查
- 加载第二阶段镜像
- 提供 UART 兜底下载
- 跳转到下一阶段

BootROM 中建议保留的原则：

- 功能闭环尽量短
- 外设初始化尽量少
- 协议尽量保守
- 代码尽量稳定
- 空间占用尽量低

### 4.2 BootLoader 职责

BootLoader 负责可升级、可演进的复杂逻辑，建议包括：

- Flash probe
- 读取 JEDEC ID
- 识别 Flash 厂商、型号、容量
- 根据器件配置读命令、dummy cycle、QE 位、4-byte address 模式
- 在线升级协议
- Flash 擦除、写入、校验
- A/B 分区管理
- 升级回滚
- 镜像版本管理
- 更强的镜像完整性校验
- 可选的签名校验
- 最终加载并跳转业务应用

## 5. 为什么不建议把复杂功能塞进 BootROM

### 5.1 BootROM 不易修改

BootROM 一旦固化，后续修复成本高。凡是未来可能变化的逻辑，都不适合优先放进 BootROM。

### 5.2 BootROM 空间有限

QL25 当前 BootROM 空间预算是 16KB。若继续加入以下能力，会明显膨胀：

- 多厂商 Flash 兼容
- probe 和参数表
- 擦除写入流程
- 忙状态轮询
- 回滚策略
- 安全校验

### 5.3 BootROM 更适合最小可靠路径

BootROM 的核心目标不是“功能丰富”，而是：

- 上电能起来
- 失败能兜底
- RTL 能联调
- 真板能稳定启动

## 6. Flash probe 应放在哪一层

### 6.1 probe 的含义

这里的 `probe` 一般指：

- 发送 `0x9F` 等命令读取 JEDEC ID
- 识别 Flash 厂商和型号
- 获取容量或参数信息
- 决定后续读写流程

### 6.2 probe 的作用

不同 Flash 器件在以下方面可能不同：

- 支持的读命令不同
- dummy cycle 要求不同
- sector size 不同
- page size 不同
- 4-byte address 模式处理不同
- QE 位位置和使能方式不同

因此如果要支持：

- 在线升级
- Flash 烧写
- 多型号兼容

则 probe 很有价值。

### 6.3 为什么 probe 更适合 BootLoader

probe 适合放在 BootLoader，而不是最小 BootROM，原因是：

- probe 会增加代码体积
- probe 会引入器件兼容表和更多分支逻辑
- probe 主要服务于“升级”和“烧写”，不是最小启动必须项
- BootLoader 可升级，后续适配新器件更容易

因此建议：

- **BootROM** 固定使用最保守读法完成启动
- **BootLoader** 再做 probe 和后续复杂处理

## 7. QL25 当前阶段建议

结合 QL25 当前状态，建议分两步走。

### 7.1 第一阶段

先把 BootROM 做成稳定的最小启动版本：

- 支持 Boot Pin 启动模式选择
- 支持最小 UART 下载
- 支持最小 XIP0 初始化
- 支持从固定 Flash 区读取镜像
- 支持基础镜像头校验
- 支持 payload CRC 校验
- 支持跳转启动

这阶段目标是：

- RTL 联调可用
- 真板 bring-up 可用
- 失败路径可诊断

### 7.2 第二阶段

增加 Flash 中的 BootLoader：

- 固定放在预留分区
- BootROM 永远只负责加载 BootLoader
- BootLoader 再负责业务镜像管理

这阶段可逐步加入：

- Flash probe
- 在线升级
- 擦写和回读校验
- A/B 分区
- 回滚
- 更强校验或签名

## 8. 推荐职责边界

### 8.1 建议放在 BootROM 的功能

- Boot pin 采样
- 最小串口初始化
- 最小 QSPI/XIP 初始化
- 固定位置镜像头读取
- 基础镜像头合法性检查
- 最小下载协议
- 最小错误处理
- 跳转

### 8.2 不建议放在 BootROM 的功能

- 多 Flash 型号兼容表
- Flash probe
- 擦除写入完整流程
- 在线升级状态机
- 回滚策略
- 文件系统
- 复杂日志
- 网络升级
- 大量调试命令

### 8.3 建议放在 BootLoader 的功能

- Flash probe
- Flash 写保护管理
- 擦除/写入/回读校验
- A/B 分区管理
- 镜像回滚
- 版本比较
- 升级恢复
- 签名校验
- 升级协议扩展

## 9. 面向 RTL 的建议

如果当前主要目标是 RTL 仿真联调，建议：

- BootROM 保持最小化
- 读 Flash 先用最保守的单线 `0x03`
- 先验证启动主流程
- 不在 BootROM 中引入过多器件兼容逻辑

等 RTL 主流程稳定后，再把更复杂的 Flash 适配和升级能力放到 BootLoader。

## 10. 面向量产的建议

如果后续面向量产，建议采用：

```text
BootROM
-> BootLoader
-> App
```

并形成以下职责划分：

- BootROM：稳定启动
- BootLoader：升级与恢复
- App：业务功能

这样做的好处：

- BootROM 变更最少
- 升级能力集中在可维护的软件层
- 量产问题更容易通过升级修复
- 不同 Flash 器件的兼容也更容易演进

## 11. 总结

对 QL25 来说，推荐采用两级启动架构：

- **BootROM** 做最小、稳定、不可频繁变化的启动闭环
- **BootLoader** 做可升级、可扩展的存储管理和在线升级功能

关于 Flash 访问策略，当前建议是：

- BootROM 使用最保守的固定读方式
- BootLoader 再加入 probe 和复杂 Flash 管理能力

这样能兼顾：

- RTL 联调效率
- 真板 bring-up 稳定性
- 后续在线升级和量产演进能力

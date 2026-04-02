# QL25 IDU（核间中断分发单元）测试用例说明

> 版本: v1.0
> 日期: 2026-04-02
> 用例路径: `soc_test_cases/ns_core{0,1}/IDU/idu/`

---

## 1. 测试目的

验证 QL25 SoC 的 **IDU（Interrupt Distribution Unit，中断分发单元）** 各项功能是否正常工作。

IDU 是 QL25 双核 AMP 架构的核心硬件组件，提供：

- **核间中断（ICI）**：一个核向另一个核发送中断通知
- **事件（Event）与唤醒（WFE）**：一个核发送事件唤醒处于 WFE 低功耗状态的另一个核
- **中断分发（Indicator / Claim）**：外设中断路由到指定核
- **硬件信号量（Semaphore）**：多核互斥访问共享资源
- **NMI 锁定（NMI Lock）**：NMI 选择寄存器的锁定保护

这些功能直接支撑 AMP 平台层的 IPC 通信、从核释放、共享资源保护等关键能力。

---

## 2. IDU 硬件参数

| 项目 | 值 | SDK 定义 |
|------|---|---------|
| IDU 基址 | `0x10010000` | `IDU_BASE` (`ns.h:995`) |
| 核间中断号 | `19` | `INTER_CORE_IRQn` (`ns.h:105`) |
| Hart 数量 | 运行时读取 | `IDU_GetHartNum(IDU_BASE)` |
| Cluster 数量 | 运行时读取 | `IDU_GetClusterNum(IDU_BASE)` |
| 信号量数量 | 运行时读取 | `IDU_GetSemaphoreNum(IDU_BASE)` |

### IDU 驱动 API 速查（`ns_idu.h`）

| 功能分类 | API | 说明 |
|---------|-----|------|
| **拓扑查询** | `IDU_GetHartNum()` | 获取 hart 总数 |
| | `IDU_GetClusterNum()` | 获取 cluster 总数 |
| | `IDU_GetSemaphoreNum()` | 获取信号量总数 |
| **核间中断** | `IDU_SendICI(base, send_id, recv_id)` | 向目标核发送 ICI |
| | `IDU_GetProcessxICI_Status(base, core_id)` | 读取 ICI 状态（哪些核发来了 ICI） |
| | `IDU_ProcessxClearICI(base, core_id, mask)` | 清除指定 ICI pending |
| **事件** | `IDU_SetEventSel(base, core_id, mask)` | 设置事件选择（唤醒哪些核） |
| | `IDU_SetIrqAsEvent(base, int_id)` | 将外设中断转为 WFE 事件源 |
| **中断分发** | `IDU_GetIndicator(base, int_id)` | 读取中断路由目标 |
| | `IDU_SetIndicator(base, int_id, mask)` | 设置中断路由到哪些核 |
| | `IDU_GetClaim(base, int_id)` | 读取 claim 状态 |
| | `IDU_SetClaim(base, int_id, mask)` | 设置 claim 掩码 |
| | `IDU_IsFirstClaim(base, int_id, core_id)` | 检查当前核是否首次 claim 该中断 |
| **信号量** | `IDU_SetSemaphoreStatus(base, id, val)` | 写信号量值 |
| | `IDU_GetSemaphoreStatus(base, id)` | 读信号量值 |
| | `IDU_CheckSemaphoreStatus(base, id, val)` | 原子检查并获取信号量 |
| **NMI** | `IDU_LockNMI(base)` | 锁定 NMI 选择寄存器 |
| | `IDU_SetNMISel(base, core_id, val)` | 设置 NMI 选择 |
| | `IDU_GetLockNMI(base, core_id)` | 读取 NMI 锁定状态 |

---

## 3. 测试用例总览

共 8 种测试，两核各一套（共 16 个用例目录）。**所有 IDU 测试都需要双核并行启动。**

| 用例名 | 源文件 | 测试的 IDU 功能 | AMP 重要度 |
|--------|--------|---------------|-----------|
| **intercore** | `intercore.c` | 核间中断（ICI）收发 | **关键** |
| **event** | `main.c` | WFE 事件唤醒 | **关键** |
| **semaphore** | `semaphore.c` | 硬件信号量互斥 | **高** |
| **broadcast** | `broadcast.c` | LGPIO 中断广播分发 | 高 |
| **indicator_mask** | `mask.c` | 中断路由与 mask 修改 | 高 |
| **irq_as_event** | `main.c` | 外设中断转 WFE 事件 | 中 |
| **event_lgpio** | `main.c` | LGPIO 触发的事件唤醒 | 中 |
| **nmi_lock** | `nmi_lock.c` | NMI 寄存器锁定保护 | 低 |
| **smp_boot** | `smp_boot.c` | SMP 启动（空壳测试） | 不适用 |

---

## 4. 各测试详细说明

### 4.1 intercore — 核间中断收发（ICI）

**测试目的**：验证一个核能否通过 IDU 向另一个核发送中断，以及接收核能否正确响应。

**这是 AMP IPC 的硬件基础**——开发计划中的 `utb_ipc_notify_peer()` 将直接基于此功能实现。

**测试流程**：

```
Core 0 (processor_id=0, core_id=0):
  ① 使能 IDU 时钟
  ② 注册 INTER_CORE_IRQn (19) 中断处理函数
  ③ 使能全局中断
  ④ 释放 Core 1 (soc_clk_core1_stop_on_reset(DISABLE))
  ⑤ 等待 Core 1 的 ICI 到来（如果自身非 processor_id 0，则先等 Core 0 的 ICI）
  ⑥ 向所有 cluster 发送 ICI: IDU_SendICI(IDU_BASE, self_id, target_id)

Core 1 (processor_id=1, core_id=0):
  ① 使能 IDU 时钟
  ② 注册 INTER_CORE_IRQn 中断处理函数
  ③ 使能全局中断
  ④ 向所有 cluster 发送 ICI

INTER_CORE_IRQHandler (中断处理函数):
  ① IDU_GetProcessxICI_Status() — 读取是哪个核发来的 ICI
  ② IDU_ProcessxClearICI() — 清除对应 pending 位
  ③ count++
  ④ 当收到所有预期的 ICI 后 → simulation_pass()
```

**验证点**：
- `IDU_SendICI()` 能触发目标核的 `INTER_CORE_IRQn` 中断
- 中断处理函数中能正确读取发送方 ID
- `IDU_ProcessxClearICI()` 能正确清除 pending
- 双向 ICI 均可工作

**判定**：ISR 中 `count` 达到预期值（收到所有 cluster 的 ICI）→ `simulation_pass()`

---

### 4.2 event — WFE 事件唤醒

**测试目的**：验证一个核能否通过 IDU Event 机制唤醒处于 WFE（Wait For Event）低功耗状态的另一个核。

**这是 AMP 从核释放的关键机制之一**——主核完成初始化后，可通过事件唤醒从核。

**测试流程**：

```
单 cluster 场景 (QL25 双核):
  Core 0 (processor_id=0):
    ① 释放 Core 1
    ② 对所有非 0 核设置事件选择: IDU_SetEventSel(base, j, 0xFFFFFFFF)
    ③ 写 CSR 0x812 = 1（触发事件发送）
    ④ simulation_pass()

  Core 1 (processor_id≠0):
    ① 进入 __WFE() 等待事件
    ② 被唤醒后 → simulation_pass()

多 cluster 场景:
  Core 0 (processor_id=0):
    ① 释放其他核
    ② 注册 INTER_CORE_IRQn，等待所有其他核发来 ICI (ready 标志)
    ③ 收到全部 ICI 后，向所有核发送事件唤醒
    ④ simulation_pass()

  其他核:
    ① 向 Core 0 发送 ICI 表示自己已准备好
    ② 进入 __WFE()
    ③ 被唤醒后 → simulation_pass()
```

**验证点**：
- `IDU_SetEventSel()` 正确配置事件目标
- `__WFE()` 能正确进入等待状态
- CSR 0x812 写 1 能触发事件
- 目标核能被唤醒并继续执行

**判定**：被唤醒的核执行到 `simulation_pass()`

---

### 4.3 semaphore — 硬件信号量

**测试目的**：验证 IDU 硬件信号量的原子获取和释放功能。

**测试流程**：

```
Core 0 (processor_id=0):
  ① 释放 Core 1
  ② 获取信号量数量: IDU_GetSemaphoreNum()
  ③ 循环测试信号量 0:
     a. IDU_SetSemaphoreStatus(base, 0, self_hart_id) — 写入自己的 hart_id 占用信号量
     b. 延时 100 NOP（给其他核竞争机会）
     c. IDU_GetSemaphoreStatus(base, 0) — 读回检查是否仍为自己的值
     d. 如果值被改变 → 信号量互斥失败 → simulation_fail()
     e. 值未变 → 验证 Indicator 读写 → 释放信号量 (写 0xFFFFFFFF) → count++
  ④ count 达到 2 后，用 CheckSemaphoreStatus() 测试信号量 (semaphore_num-1)
  ⑤ 全部通过 → simulation_pass()

Core 1:
  同样的逻辑，与 Core 0 竞争同一信号量
```

**验证点**：
- 信号量写入后在短时间内不被其他核覆盖（互斥语义）
- `IDU_CheckSemaphoreStatus()` 原子检查功能正确
- `IDU_SetIndicator()` / `IDU_GetIndicator()` 在信号量保护下的读写一致性
- 释放信号量（写 `0xFFFFFFFF`）后其他核可获取

**判定**：两次信号量获取-检查-释放循环全部成功 → `simulation_pass()`

---

### 4.4 broadcast — LGPIO 中断广播

**测试目的**：验证外设中断（LGPIO）通过 IDU 广播分发到指定核。

**测试流程**：

```
Core 0:
  ① 使能 LGPIO0、IDU 时钟
  ② IDU_SetClaim() 配置 LGPIO0 中断的 claim 掩码
  ③ 注册 LGPIO0_IRQn 中断处理函数
  ④ 初始化 LGPIO0 并触发软件中断: LGPIO_SetSoftIT()
  ⑤ ISR 中检测到 LGPIO0 中断 → simulation_pass()

Core 1:
  直接 simulation_pass()（本测试验证广播到 Core 0）
```

**验证点**：
- `IDU_SetClaim()` 正确配置中断路由
- LGPIO 软件中断能通过 IDU 路由到目标核
- 目标核的 ECLIC 能正确收到该中断

---

### 4.5 indicator_mask — 中断路由掩码

**测试目的**：验证中断 Indicator Mask 的动态修改——一个核处理中断后修改 mask，使后续中断不再路由到自己。

**测试流程**：

```
Core 0:
  ① 配置 RTC0 定时中断
  ② 注册 RTC0_IRQn 中断处理函数
  ③ RTC 中断到来时:
     a. IDU_IsFirstClaim() 检查是否首次 claim
     b. IDU_SetIndicator() 验证 Indicator 读写
     c. IDU_SetClaim() 重置 claim
     d. count++
  ④ 第 2 次中断后:
     a. IDU_SetIndicator() 清除自己的 bit — 从此不再接收该中断
     b. simulation_pass()
```

**验证点**：
- `IDU_IsFirstClaim()` FCFC（First-Come-First-Claim）模式正确
- 修改 Indicator mask 后，中断不再路由到被排除的核
- claim/release 流程完整

---

### 4.6 irq_as_event — 外设中断转 WFE 事件

**测试目的**：验证通过 `IDU_SetIrqAsEvent()` 将外设中断（LGPIO）配置为 WFE 事件源，唤醒处于 WFE 的核。

**测试流程**：

```
Core 0:
  ① IDU_SetIrqAsEvent(base, LGPIO0_IRQn - 20) — 将 LGPIO0 中断注册为事件
  ② IDU_SetEventSel() — 配置所有核接收该事件
  ③ 等待 Core 1 通过信号量报告就绪
  ④ 初始化 LGPIO0 并触发软件中断
  ⑤ simulation_pass()

Core 1:
  ① IDU_SetSemaphoreStatus() 报告就绪
  ② __WFE() 等待
  ③ LGPIO 中断转为事件后被唤醒 → simulation_pass()
```

**验证点**：
- `IDU_SetIrqAsEvent()` 能正确将外设中断转换为事件源
- 事件能唤醒处于 `__WFE()` 的核

---

### 4.7 event_lgpio — LGPIO 触发事件唤醒

**测试目的**：类似 `irq_as_event`，但通过 LGPIO 外部引脚上升沿（由 RTC 定时输出模拟）触发事件唤醒。

**测试流程**：

```
Core 0:
  ① 配置 LGPIO0 上升沿事件检测
  ② 配置 IOMUX 将 RTC 输出映射到 LGPIO0 输入引脚
  ③ 释放 Core 1
  ④ IDU_SetEventSel() 配置事件目标
  ⑤ 等待 Core 1 信号量就绪
  ⑥ 启动 RTC 输出（触发 LGPIO0 上升沿事件）
  ⑦ simulation_pass()

Core 1:
  ① 信号量报告就绪
  ② __WFE() 等待 LGPIO 事件
  ③ 被唤醒 → simulation_pass()
```

**验证点**：
- LGPIO 上升沿事件能通过 IDU 路由为 WFE 唤醒源
- 信号量用于核间同步（确保 Core 1 先进入 WFE 再触发事件）

---

### 4.8 nmi_lock — NMI 寄存器锁定

**测试目的**：验证 NMI 选择寄存器的锁定保护机制。

**测试流程**：

```
Core 0:
  ① IDU_SetNMISel(base, 0, 0) — 锁定前写 0
  ② IDU_GetLockNMI() — 读回应为 0 → count++
  ③ IDU_LockNMI() — 锁定 NMI 寄存器
  ④ IDU_SetNMISel(base, 0, 1) — 尝试写 1
  ⑤ IDU_GetLockNMI() — 读回应仍为 0（锁定后写入无效）→ 如果 ≠1 则 count++
  ⑥ count == 2 → 复位 IDU → simulation_pass()
```

**验证点**：
- 锁定前 NMI 选择寄存器可正常读写
- `IDU_LockNMI()` 后写入被忽略，值不变
- IDU 复位能恢复可写状态

---

### 4.9 smp_boot — SMP 启动（空壳）

**注意**：`smp_boot.c` 只有 `simulation_pass(); while(1);`，是一个空壳测试，仅验证双核并行启动不崩溃。

---

## 5. 双核并行启动机制

**所有 IDU 测试的 simargs 都包含 `+TEST_BOOT_TYPE=parallel_boot`**，这告知 EDA Testbench 让两个核同时启动执行各自的代码。

与单核测试的区别：

| 启动模式 | 说明 | 适用场景 |
|---------|------|---------|
| 默认（单核） | 只有 boot core 执行，另一核处于 reset | USART、SPI、SRAM 等单核测试 |
| `parallel_boot` | 两核同时释放，各自从自己的 ILM 启动 | **IDU 所有测试** |

**EDA 运行时需要同时编译两核的 ELF：**

```bash
# 编译 Core 0（从核）的 IDU 测试
make SOC=ns_core0 CORE=core0_n300 DOWNLOAD=ilm \
     PROGRAM=soc_test_cases/ns_core0/IDU/idu/intercore clean all

# 编译 Core 1（主核）的 IDU 测试
make SOC=ns_core1 CORE=core1_n300 DOWNLOAD=ilm \
     PROGRAM=soc_test_cases/ns_core1/IDU/idu/intercore clean all

# 两个 ELF 同时加载到仿真器，+TEST_BOOT_TYPE=parallel_boot 双核并行启动
```

### 核内角色分配逻辑

IDU 测试代码通过 `hart_id` 区分当前核的角色：

```c
hart_id.d = __RV_CSR_READ(CSR_MHARTID);

if (hart_id.b.core_id == 0 && hart_id.b.processor_id == 0) {
    // 主角色：负责初始化、释放另一核、发送 ICI/Event
} else {
    // 从角色：接收 ICI/Event、等待唤醒、竞争信号量
}
```

部分测试中 `processor_id == 0` 的核还会调用 `soc_clk_core1_stop_on_reset(DISABLE)` 显式释放另一核。

---

## 6. 外部依赖

| 项目 | 要求 |
|------|------|
| 外部激励 | **不需要**（部分测试使用 LGPIO 软件中断或 RTC 定时中断，均为芯片内部资源） |
| Testbench 模型 | **不需要**，无 `+TEST_LOOPBACK` 参数 |
| simargs | `+TEST_BOOT_TYPE=parallel_boot`（双核并行启动） |
| 特殊说明 | `event_lgpio` 需要 LGPIO0 和 RTC0 外设可用；`broadcast` / `irq_as_event` 需要 LGPIO0 |
| 时钟使能 | 需要 `MISC_HAS_IDU_HAS_CLK` 宏定义存在（SDK 已定义） |

---

## 7. 如何运行与观察

### 7.1 推荐运行顺序

按功能复杂度递增，便于逐步排查问题：

```
① smp_boot        → 仅验证双核并行启动不崩溃（空壳）
② event           → 验证 WFE 事件唤醒
③ intercore       → 验证核间中断收发（AMP IPC 基础）
④ semaphore       → 验证硬件信号量互斥
⑤ nmi_lock        → 验证 NMI 锁定
⑥ broadcast       → 验证中断广播分发
⑦ indicator_mask  → 验证中断路由掩码动态修改
⑧ irq_as_event    → 验证外设中断转事件
⑨ event_lgpio     → 验证 LGPIO+RTC 触发事件唤醒
```

### 7.2 通过（PASS）

- 两核均执行到 `simulation_pass()`
- 仿真日志无 `ERR` / `fail` 打印
- `intercore` 测试中 ISR 的 `count` 达到预期值

### 7.3 失败（FAIL）

| 现象 | 可能原因 |
|------|---------|
| 仿真超时，无 pass/fail | 核未被唤醒（`__WFE` 未返回）或 ICI 未触发 |
| `simulation_fail()` 在 semaphore 中触发 | 信号量互斥失败，值被另一核覆盖 |
| `simulation_fail()` 在 indicator_mask 中触发 | Indicator 读写不一致 |
| Core 1 未启动 | `soc_clk_core1_stop_on_reset()` 未生效 / `+TEST_BOOT_TYPE=parallel_boot` 未传 |
| ISR 未进入 | `INTER_CORE_IRQn` 未正确注册到 ECLIC / 全局中断未使能 |
| `count` 未达到预期 | ICI 发送成功但 ISR 中 clear 逻辑有误 / cluster_num 读取错误 |

### 7.4 调试建议

1. **先跑 `smp_boot`**：如果空壳都失败，说明双核并行启动机制本身有问题
2. **再跑 `event`**：如果 WFE 唤醒失败，检查 `IDU_SetEventSel()` 的参数和 CSR 0x812 写入
3. **`intercore` 失败时**：在 ISR 中加 `printf` 打印 `status_value` 和 `count`，确认 ICI 是否到达
4. **检查 IDU 时钟**：确认 `MISC_HAS_IDU_HAS_CLK` 宏存在且 `idu_clk_en(ENABLE)` 被调用

---

## 8. 与 AMP 开发计划的关系

| IDU 测试 | 对应平台功能 | 开发计划阶段 |
|---------|------------|------------|
| **intercore** | `utb_ipc_notify_peer()` — IPC 核间通知 | Phase 3 (IPC 层) |
| **event** | `utb_boot_release_secondary()` — 从核释放与唤醒 | Phase 2 (AMP bringup) |
| **semaphore** | 共享资源互斥保护 | Phase 3 (IPC 层) |
| indicator_mask | 外设中断在双核间的路由分配 | Phase 4 (HAL owner 模型) |
| broadcast | 外设中断广播策略 | Phase 4 |
| nmi_lock | NMI 安全保护 | Phase 6 (集成验证) |

**关键路径**：

```
IDU intercore 通过
    │
    ├── 确认 → INTER_CORE_IRQn 可用，ICI 收发正常
    ├── 确认 → ISR 注册和清除机制正确
    │
    └── 可以开始 → platform/ipc/src/ipc_idu.c 开发
                    (utb_ipc_notify_peer / utb_ipc_idu_init)

IDU event 通过
    │
    ├── 确认 → WFE 唤醒机制可用
    │
    └── 可以开始 → platform/boot/src/utb_boot.c 中从核释放逻辑

IDU semaphore 通过
    │
    ├── 确认 → 硬件信号量互斥可用
    │
    └── 可以用于 → 共享资源保护（如共享 SRAM 区域的互斥访问）
```

---

## 9. 测试局限性与补充建议

### 9.1 本测试的局限

| 局限 | 说明 |
|------|------|
| 单向 ICI | `intercore` 测试中 ICI 是从各核到 Core 0，未严格测试双向全双工 ICI |
| 信号量压力不足 | `semaphore` 只竞争 2 次，未做高频竞争压力测试 |
| 固定拓扑假设 | 代码中 `processor_id == 0` 硬编码为主角色，不适用于角色互换场景 |
| 无超时测试 | 未验证 WFE/ICI 在超时场景下的行为（如一核挂死） |

### 9.2 建议补充的 AMP 相关测试

| 测试 | 目的 |
|------|------|
| **ICI 双向全双工** | Core 0→1 和 Core 1→0 同时发 ICI，验证无死锁 |
| **ICI 高频压力** | 连续快速发送大量 ICI，验证 pending/clear 不丢失 |
| **信号量高频竞争** | 两核交替快速获取/释放同一信号量 1000 次 |
| **SRAM + ICI 联合** | Core A 写 SRAM → fence → 发 ICI → Core B 收 ICI → 读 SRAM，验证 IPC 完整链路 |
| **WFE 超时恢复** | Core B 进入 WFE 后 Core A 延迟唤醒，验证超时后系统仍可恢复 |

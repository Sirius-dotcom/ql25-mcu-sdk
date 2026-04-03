# QL25 IPC 开发计划

## 概述

IPC（Inter-Processor Communication）是主核管理面与从核数据面协同的核心系统层。
基于 **共享内存 SPSC ring + IDU 核间中断** 架构，不使用 FreeRTOS 跨核原语。

设计参考：`UTB_AMP_Platform_Design_Outline.md` 第 11 章。

## 硬件基础

| 资源 | 地址 | 说明 |
|------|------|------|
| IDU | 0x10010000 | 核间中断分发单元，`IDU_SendICI()` 触发 `INTER_CORE_IRQn`(19) |
| 共享 SRAM3 | 0x30060000 | 64KB，IPC ring + 快照 + 控制块 |
| Core 0 (从核) | processor_id=0 | 数据面，ring consumer（ctrl_req）/ producer（ctrl_rsp, event） |
| Core 1 (主核) | processor_id=1 | 管理面，ring producer（ctrl_req）/ consumer（ctrl_rsp, event） |

**关键硬件特性：**
- QL25 无 D-Cache → cache clean/invalidate 为空操作
- `fence` 仍然必须 → 保证内存写入顺序对另一核可见
- IDU 使用前必须 `idu_clk_en(ENABLE)`
- IDU 测试例（intercore/event/semaphore）已在 FPGA 验证通过

## 开发分步

### 第一步：IPC 基础层（当前）

**目标：** 双核能通过共享内存 + IDU 中断收发控制命令并得到响应。

```
platform/ipc/
├── include/
│   └── utb_ipc.h           ← 公共定义（共享内存布局、消息头、ring、API）
├── src/
│   ├── utb_ipc_ring.c      ← SPSC lock-free ring（通用实现）
│   ├── utb_ipc_notify.c    ← IDU 门铃封装（SendICI + ISR）
│   └── utb_ipc_init.c      ← 共享内存初始化 + IPC 状态管理
└── build.mk
```

**实现内容：**

| 模块 | 内容 | 说明 |
|------|------|------|
| SPSC Ring | `utb_ipc_ring_init/push/pop/pending/is_full` | lock-free，head/tail 分离，固定槽位大小 |
| IDU 门铃 | `utb_ipc_notify_init/kick/handler` | 封装 IDU_SendICI + INTER_CORE_IRQHandler |
| 共享内存 | SRAM3 布局定义 + 初始化 | 控制块 + ctrl_req_ring + ctrl_rsp_ring |
| ctrl 通道 | `utb_ipc_ctrl_request/respond` | 主核→从核命令 + 从核→主核响应 |
| barrier | `UTB_FENCE()` / `UTB_FENCE_I()` | RISC-V fence 指令封装 |

**共享内存布局（SRAM3 @ 0x30060000，64KB）：**

```
偏移        大小      内容
0x0000      256B      IPC 控制块（boot_flag, ready 标志, 版本, 状态）
0x0100      1KB       ctrl_req_ring（8 槽 × 128B）
0x0500      1KB       ctrl_rsp_ring（8 槽 × 128B）
0x0900      2KB       event_ring（16 槽 × 128B，预留，第二步）
0x1100      256B      state_snapshot（预留，第二步）
0x1200      256B      stats_snapshot（预留，第二步）
0x1300      ...       未分配（约 59KB 余量）
```

**Ring 参数：**

| Ring | 深度 | 槽位大小 | Producer | Consumer | 满时行为 |
|------|------|---------|----------|----------|---------|
| ctrl_req_ring | 8 | 128B | 主核 | 从核 | 等待 + 超时返回 |
| ctrl_rsp_ring | 8 | 128B | 从核 | 主核 | 等待 + 超时返回 |
| event_ring | 16 | 128B | 从核 | 主核 | 丢弃最旧非 URGENT（第二步） |

**API（第一步提供）：**

```c
/* 初始化（主核调用，从核验证） */
int  utb_ipc_init(int is_primary);

/* 控制命令（主核→从核） */
int  utb_ipc_ctrl_request(uint16_t cmd_id, const void *payload, uint16_t len,
                          uint32_t timeout_ms);

/* 控制响应（从核→主核） */
int  utb_ipc_ctrl_respond(uint16_t txn_id, int32_t result,
                          const void *payload, uint16_t len);

/* 接收（对端 ring 有数据时调用） */
int  utb_ipc_ctrl_recv(utb_ipc_msg_hdr_t *hdr, void *payload, uint16_t max_len);

/* IDU 门铃 */
void utb_ipc_kick_peer(void);

/* 状态查询 */
int  utb_ipc_is_peer_ready(void);
```

### 第二步：事件与快照

**前置：** 第一步完成并在 FPGA 双核验证通过。

| 模块 | 内容 |
|------|------|
| event_ring | 告警/故障/metadata 单向事件（从核→主核） |
| state_snapshot | 从核状态快照（overwrite + generation 序号） |
| stats_snapshot | 从核统计快照（周期更新，主核轮询读取） |
| mgmt_ring | 管理帧上送（从核→主核，独立于 event_ring） |

### 第三步：完整消息契约与健壮性

**前置：** 第二步完成。

| 模块 | 内容 |
|------|------|
| 消息类型 | 全部 7 种 payload 结构定义与解析 |
| timeout/retry | 控制/配置类消息的超时与有限重试 |
| backpressure | ring 满时的反压计数与溢出处理 |
| recovery | 对端重启检测、事务 fail、IPC 重建 |
| 可观测性 | depth/timeout/retry/drop/invalid/recovery 计数器 |

## SPSC Ring 设计要点

```
           Producer (writer)                    Consumer (reader)
           只更新 tail                           只更新 head
                │                                    │
    ┌───┬───┬───┼───┬───┬───┬───┬───┐
    │   │   │   │///│///│///│   │   │    /// = 待消费数据
    └───┴───┴───┼───┴───┴───┼───┴───┘
                ▲           ▲
               head        tail
          (consumer 写)  (producer 写)
```

- head 和 tail 都是 `volatile uint32_t`，通过 `fence` 保证可见性
- 不使用 mutex / spinlock / 关中断
- 空判断：`head == tail`
- 满判断：`(tail + 1) % depth == head`
- 槽位大小固定，地址 = base + slot_size * index

## IDU 门铃机制

```
主核                                从核
  │                                  │
  │  写 ring slot                    │
  │  fence                           │
  │  更新 tail                       │
  │  fence                           │
  │  IDU_SendICI(IDU_BASE, 1, 0) ──>│ INTER_CORE_IRQn 触发
  │                                  │  ISR: 读 ICI status
  │                                  │  ISR: 清 ICI pending
  │                                  │  ISR: 唤醒 IPC task
  │                                  │  task: ring_pop() 处理消息
```

## QL25 特殊约束

1. **无 D-Cache** → `UTB_CACHE_CLEAN/INVALIDATE` 为空宏，但 `fence` 必须保留
2. **fence vs dsb** → RISC-V 用 `fence`（不是 ARM 的 `dsb`），设计文档中 `UTB_DSB()` 在 QL25 上映射为 `fence`
3. **FreeRTOS 原语禁止跨核** → queue/semaphore/event group 不得用于 IPC
4. **IDU 时钟** → 使用前必须 `idu_clk_en(ENABLE)`
5. **HartID 映射** → `processor_id` = mhartid bits[9:1]，Core 0 = 0，Core 1 = 1

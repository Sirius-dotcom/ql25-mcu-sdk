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
0x0000      256B      IPC 控制块（magic, ready 标志, 版本, 状态）
0x0100      1056B     ctrl_req_ring（32B desc + 8 × 128B slot）
0x0600      1056B     ctrl_rsp_ring（32B desc + 8 × 128B slot）
0x0B00      2080B     event_ring（32B desc + 16 × 128B slot）
0x1400      256B      state_snapshot（seqlock 模型）
0x1500      256B      stats_snapshot（seqlock 模型）
0x1600      ...       未分配（约 58KB 余量）
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

### 第二步：事件与快照（已实现）

| 模块 | 文件 | 内容 |
|------|------|------|
| event_ring | `utb_ipc_event.c` | 告警/故障/metadata 单向事件（从核→主核），depth=16 |
| state_snapshot | `utb_ipc_snapshot.c` | 从核状态快照（seqlock 模型，256B 区域） |
| stats_snapshot | `utb_ipc_snapshot.c` | 从核统计快照（同 seqlock 模型，独立 256B 区域） |

**事件类型（cmd_id 字段）：**
- `UTB_IPC_EVT_ALARM` (0x0001) — 告警
- `UTB_IPC_EVT_FAULT` (0x0002) — 故障
- `UTB_IPC_EVT_METADATA` (0x0003) — 元数据
- `UTB_IPC_EVT_LINK_UP` (0x0004) / `LINK_DOWN` (0x0005) — 链路状态

**快照 seqlock 读写模型：** 写者递增 generation（奇数=写入中，偶数=完成），读者双次读 generation 检测撕裂。无锁、无阻塞、有界重试（16 次）。

**mgmt_ring** 留待第三步，独立于 event_ring。

### 第三步：完整消息契约与健壮性（已实现）

| 模块 | 文件 | 内容 |
|------|------|------|
| 消息契约 | `utb_ipc_msg.h` | 7 种 payload 结构 + 命令 ID + 编译期大小断言 |
| timeout/retry | `utb_ipc_init.c` | `utb_ipc_ctrl_request_sync()` 阻塞轮询 + 有界重试 |
| backpressure | `utb_ipc_event.c` | ring 满时 tail-drop + `g_ipc_diag` 计数 |
| recovery | `utb_ipc_recovery.c` | boot_gen 重启检测 + 心跳存活判定 + `utb_ipc_recover()` |
| 可观测性 | `utb_ipc_stats.c` | `g_ipc_diag` 全局计数器（ctrl/event/snap/timeout/retry/recovery） |

**命令 ID 命名空间：**
- 0x01xx：控制命令（NOP / HEARTBEAT / RESET_DP / GET_STATUS / GET_VERSION）
- 0x02xx：配置命令（SET_CONFIG / GET_CONFIG / DEL_CONFIG）
- 0x00xx：事件类型（ALARM / FAULT / METADATA / LINK_UP / LINK_DOWN）

**控制块新增字段：**
- `primary_hb_tick` / `secondary_hb_tick`：心跳时间戳
- `primary_boot_gen` / `secondary_boot_gen`：启动代数（跨重启递增）

**同步请求 API：** `utb_ipc_ctrl_request_sync(cmd_id, req, req_len, rsp, rsp_max, &result, timeout_cycles, max_retry)` — 发送 + 轮询 rsp ring 匹配 txn_id，超时重试。

**反压策略：** 严格 SPSC 合约，ring 满时 tail-drop（不修改 head）。URGENT 标志标记优先级但不强制投递。

**恢复流程：** 应用层周期调用 `utb_ipc_check_peer_restart()` → 检测到重启 → `utb_ipc_recover()` → 等待 `utb_ipc_is_peer_ready()`

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

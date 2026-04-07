# QL25 IPC 层开发者手册

## 1. 概述

IPC（Inter-Processor Communication）是 QL25 双核 AMP 系统中管理面（主核 Core 1）与数据面（从核 Core 0）之间的唯一通信机制。

**核心设计原则：**

- 基于共享内存 SPSC（Single-Producer Single-Consumer）无锁环形缓冲区
- 使用 IDU（Interrupt Distribution Unit）核间中断作为门铃通知
- FreeRTOS 的 queue / semaphore / event group **禁止用于跨核通信**
- QL25 无 D-Cache，cache 操作为空宏，但 `fence` 必须保留

**核标识：**

| 核 | ID | 角色 | 宏定义 |
|----|-----|------|--------|
| Core 0 | 0 | 从核（数据面） | `UTB_CORE_SECONDARY` |
| Core 1 | 1 | 主核（管理面） | `UTB_CORE_PRIMARY` |

---

## 2. 源文件结构

```
platform/ipc/
├── include/
│   ├── utb_ipc.h           ← 公共定义（内存布局、消息头、ring、所有 API）
│   └── utb_ipc_msg.h       ← 消息契约（命令 ID、7 种 payload 结构）
├── src/
│   ├── utb_ipc_ring.c      ← SPSC 无锁环形缓冲区（通用实现）
│   ├── utb_ipc_notify.c    ← IDU 门铃封装（SendICI + ISR）
│   ├── utb_ipc_init.c      ← 初始化 + ctrl 通道 + 同步请求
│   ├── utb_ipc_event.c     ← 事件通道（从核→主核，单向）
│   ├── utb_ipc_snapshot.c  ← 状态/统计快照（seqlock 模型）
│   ├── utb_ipc_stats.c     ← 诊断计数器管理
│   └── utb_ipc_recovery.c  ← 心跳、重启检测、IPC 恢复
└── build.mk                ← SDK 构建集成（include 后自动编译）
```

**集成方式：** 在应用 Makefile 中加入以下一行即可：

```makefile
include $(NUCLEI_SDK_ROOT)/platform/ipc/build.mk
```

---

## 3. 共享内存布局

共享内存位于 SRAM3，基地址 `0x30060000`，总大小 64KB。

```
偏移          大小        内容                           所有者
───────────────────────────────────────────────────────────────
0x0000        256B        IPC 控制块                     主核初始化，双核读写
0x0100        1056B       ctrl_req_ring (8×128B)         主核写，从核读
0x0600        1056B       ctrl_rsp_ring (8×128B)         从核写，主核读
0x0B00        2080B       event_ring (16×128B)           从核写，主核读
0x1400        256B        state_snapshot                 从核写，主核读
0x1500        256B        stats_snapshot                 从核写，主核读
0x1600        ~58KB       未分配（可扩展）
```

**已使用：** 5632 字节（约 8.6%）。每个区域之间有对齐间隔，不存在重叠。

---

## 4. 通信通道一览

### 4.1 ctrl 通道（请求/响应，双向）

| 属性 | 值 |
|------|----|
| 方向 | 主核 → 从核（request），从核 → 主核（response） |
| Ring | ctrl_req_ring + ctrl_rsp_ring，各 8 槽 × 128B |
| 消息头 | 24B（`utb_ipc_msg_hdr_t`） |
| 最大 payload | 104B（`UTB_IPC_CTRL_PAYLOAD_MAX`） |
| 事务配对 | 通过 `txn_id` 字段关联 request 和 response |
| 满时行为 | 返回 `UTB_IPC_ERR_FULL` |

### 4.2 event 通道（单向事件上报）

| 属性 | 值 |
|------|----|
| 方向 | 从核 → 主核（单向） |
| Ring | event_ring，16 槽 × 128B |
| 最大 payload | 104B（`UTB_IPC_EVENT_PAYLOAD_MAX`） |
| 事件类型 | `UTB_IPC_EVT_ALARM` / `FAULT` / `METADATA` / `LINK_UP` / `LINK_DOWN` |
| 满时行为 | tail-drop：丢弃新事件，计入 `g_ipc_diag.evt_tx_drop` |

### 4.3 snapshot 通道（overwrite 快照）

| 属性 | 值 |
|------|----|
| 方向 | 从核写，主核读 |
| 区域 | state_snapshot（256B）+ stats_snapshot（256B） |
| 可用数据 | 240B（`UTB_IPC_SNAP_DATA_SIZE`） |
| 一致性模型 | seqlock（generation 序号，无锁，有界重试 16 次） |
| 写者限制 | 单写者（从核），不可多任务并发写同一快照 |

---

## 5. 消息头格式

每条消息的前 24 字节是统一的消息头 `utb_ipc_msg_hdr_t`：

```
偏移  大小  字段          说明
────────────────────────────────────────
 0    2    magic         0x4950 ("IP")，用于校验
 2    1    version       IPC 契约版本（当前 = 1）
 3    1    msg_type      消息类型（CTRL_REQ=1, CTRL_RSP=2, EVENT=3）
 4    2    payload_len   payload 实际字节数
 6    2    flags         标志位（F_ACK_REQ / F_URGENT / F_RETRY）
 8    4    seq           发送方序号（per-ring 递增）
12    2    txn_id        事务号（request/response 配对）
14    1    src_core      源核 ID
15    1    dst_core      目的核 ID
16    2    cmd_id        命令/事件 ID
18    2    _reserved     对齐填充
20    4    result        响应结果码（请求时为 0）
```

---

## 6. 命令 ID 与 Payload 定义

所有 payload 结构定义在 `utb_ipc_msg.h` 中。

### 6.1 控制命令（cmd_id 0x01xx）

| cmd_id | 宏 | 方向 | 请求 payload | 响应 payload |
|--------|----|------|-------------|-------------|
| 0x0100 | `UTB_CMD_NOP` | 主→从 | 无 | 无（仅返回 result=0 表示连通） |
| 0x0101 | `UTB_CMD_HEARTBEAT` | 双向 | `utb_ipc_heartbeat_payload_t` (8B) | 同左 |
| 0x0102 | `UTB_CMD_RESET_DP` | 主→从 | 无 | 无 |
| 0x0103 | `UTB_CMD_GET_STATUS` | 主→从 | 无 | `utb_ipc_status_payload_t` (40B) |
| 0x0104 | `UTB_CMD_GET_VERSION` | 主→从 | 无 | `utb_ipc_version_payload_t` (24B) |

### 6.2 配置命令（cmd_id 0x02xx）

| cmd_id | 宏 | 说明 | payload |
|--------|----|------|---------|
| 0x0200 | `UTB_CMD_SET_CONFIG` | 下发配置 | `utb_ipc_config_payload_t` (96B) |
| 0x0201 | `UTB_CMD_GET_CONFIG` | 读取配置 | 请求：cfg_id；响应：同结构含 data |
| 0x0202 | `UTB_CMD_DEL_CONFIG` | 删除配置 | 请求：cfg_id |

### 6.3 事件（cmd_id 0x00xx）

| cmd_id | 宏 | payload |
|--------|----|---------|
| 0x0001 | `UTB_IPC_EVT_ALARM` | `utb_ipc_alarm_payload_t` (16B) |
| 0x0002 | `UTB_IPC_EVT_FAULT` | `utb_ipc_fault_payload_t` (20B) |
| 0x0003 | `UTB_IPC_EVT_METADATA` | 自定义（不超过 104B） |
| 0x0004 | `UTB_IPC_EVT_LINK_UP` | `utb_ipc_link_payload_t` (8B) |
| 0x0005 | `UTB_IPC_EVT_LINK_DOWN` | `utb_ipc_link_payload_t` (8B) |

---

## 7. API 使用指南

### 7.1 初始化

双核各自调用一次 `utb_ipc_init()`，主核必须先于从核完成。

```c
/* 主核（Core 1）启动代码中 */
int ret = utb_ipc_init(1);  /* is_primary = 1 */
if (ret != UTB_IPC_OK) { /* 错误处理 */ }

/* 从核（Core 0）启动代码中 */
int ret = utb_ipc_init(0);  /* is_primary = 0 */
if (ret != UTB_IPC_OK) { /* 超时或版本不匹配 */ }
```

主核初始化流程：
1. 保存 `boot_gen` → 清零共享内存 → 恢复 `boot_gen + 1`
2. 写入控制块（version, ready 标志等），最后写 magic（发布模式）
3. 初始化 3 个 ring + 2 个 snapshot
4. 注册 IDU 门铃 ISR
5. 设置 `primary_ready = 1`

从核初始化流程：
1. 有界等待 magic + primary_ready
2. 验证 version
3. 注册 IDU 门铃 ISR
4. 递增 `secondary_boot_gen`，设置 `secondary_ready = 1`

### 7.2 发送控制请求（异步）

```c
/* 主核：向从核发送 GET_STATUS 请求 */
int ret = utb_ipc_ctrl_request(UTB_CMD_GET_STATUS, NULL, 0);
```

### 7.3 发送控制请求（同步，等待响应）

```c
/* 主核：同步查询从核状态，超时 0x100000 cycle，最多重试 3 次 */
utb_ipc_status_payload_t status;
int32_t result;
int ret = utb_ipc_ctrl_request_sync(
    UTB_CMD_GET_STATUS,
    NULL, 0,                              /* 请求无 payload */
    &status, sizeof(status),              /* 响应输出缓冲 */
    &result,                              /* 响应结果码 */
    0x00100000,                           /* 单次超时（cycle 计数） */
    UTB_IPC_SYNC_RETRY_MAX                /* 最大重试 3 次 */
);
if (ret == UTB_IPC_OK && result == 0) {
    /* status.dp_state, status.rx_packets, ... */
}
```

**注意：** 同步请求会阻塞调用任务，栈开销约 140 字节。禁止在 ISR 中调用。

### 7.4 处理控制请求（从核侧）

```c
/* 从核 IPC 服务 task 主循环 */
utb_ipc_msg_hdr_t hdr;
uint8_t payload[UTB_IPC_CTRL_PAYLOAD_MAX];

int ret = utb_ipc_recv(
    UTB_IPC_SHM_BASE + UTB_IPC_CTRL_REQ_OFS,
    &hdr, payload, sizeof(payload));

if (ret == UTB_IPC_OK) {
    switch (hdr.cmd_id) {
    case UTB_CMD_GET_STATUS: {
        utb_ipc_status_payload_t rsp;
        /* 填充 rsp 各字段... */
        utb_ipc_ctrl_respond(hdr.txn_id, 0, &rsp, sizeof(rsp));
        break;
    }
    case UTB_CMD_NOP:
        utb_ipc_ctrl_respond(hdr.txn_id, 0, NULL, 0);
        break;
    /* ... 其他命令 */
    }
}
```

### 7.5 发送事件（从核）

```c
/* 从核：上报链路 UP 事件 */
utb_ipc_link_payload_t evt;
evt.port_id = 0;
evt.link_state = 1;
evt.speed_mbps = 100;
evt.timestamp_ms = current_tick_ms;

int ret = utb_ipc_event_send(UTB_IPC_EVT_LINK_UP, &evt, sizeof(evt));
if (ret == UTB_IPC_ERR_FULL) {
    /* ring 满，事件被丢弃 */
}

/* 紧急事件（标记 URGENT 标志） */
utb_ipc_fault_payload_t fault;
fault.fault_id = 0x0001;
fault.fatal = 1;
/* ... */
utb_ipc_event_send_urgent(UTB_IPC_EVT_FAULT, &fault, sizeof(fault));
```

### 7.6 接收事件（主核）

```c
/* 主核 IPC 服务 task */
utb_ipc_msg_hdr_t hdr;
uint8_t payload[UTB_IPC_EVENT_PAYLOAD_MAX];

while (utb_ipc_event_recv(&hdr, payload, sizeof(payload)) == UTB_IPC_OK) {
    switch (hdr.cmd_id) {
    case UTB_IPC_EVT_LINK_UP: {
        utb_ipc_link_payload_t *p = (utb_ipc_link_payload_t *)payload;
        /* 处理链路 UP：p->port_id, p->speed_mbps */
        break;
    }
    case UTB_IPC_EVT_FAULT: {
        utb_ipc_fault_payload_t *p = (utb_ipc_fault_payload_t *)payload;
        if (p->fatal) { /* 不可恢复故障处理 */ }
        break;
    }
    /* ... */
    }
}
```

### 7.7 快照读写

```c
/* 从核：周期性更新统计快照（如每 1 秒） */
my_stats_t stats;
/* 填充 stats... */
utb_ipc_snapshot_write(UTB_IPC_STATS_SNAP_ADDR,
                       &stats, sizeof(stats),
                       current_tick);

/* 主核：读取从核统计快照 */
my_stats_t stats;
uint32_t gen, ts;
int ret = utb_ipc_snapshot_read(UTB_IPC_STATS_SNAP_ADDR,
                                &stats, sizeof(stats),
                                &gen, &ts);
if (ret == UTB_IPC_OK) {
    /* 使用 stats 数据 */
}
```

### 7.8 心跳与健康检测

```c
/* 双核各自在周期性 task 中调用（如每 500ms） */
utb_ipc_heartbeat_tick(current_tick);

/* 主核检查从核是否存活（2000 tick 超时） */
if (!utb_ipc_is_peer_alive(current_tick, 2000)) {
    /* 从核心跳超时，可能需要恢复 */
}

/* 检测对端是否重启 */
if (utb_ipc_check_peer_restart()) {
    /* 对端 boot_gen 变化，执行恢复 */
    utb_ipc_recover();
    /* 等待对端重新就绪 */
    while (!utb_ipc_is_peer_ready()) { /* ... */ }
}
```

### 7.9 诊断计数器

```c
/* 读取诊断信息 */
const utb_ipc_diag_t *diag = utb_ipc_diag_get();

printf("ctrl tx=%u rx=%u full=%u invalid=%u\n",
       diag->ctrl_tx, diag->ctrl_rx,
       diag->ctrl_tx_full, diag->ctrl_rx_invalid);
printf("evt tx=%u drop=%u rx=%u\n",
       diag->evt_tx, diag->evt_tx_drop, diag->evt_rx);
printf("sync timeout=%u retry=%u\n",
       diag->sync_timeout, diag->sync_retry);
printf("peer_restart=%u recovery=%u\n",
       diag->peer_restart, diag->recovery);
```

---

## 8. 错误码

| 值 | 宏 | 说明 |
|----|----|------|
| 0 | `UTB_IPC_OK` | 成功 |
| -1 | `UTB_IPC_ERR_TIMEOUT` | 操作超时 |
| -2 | `UTB_IPC_ERR_FULL` | ring 满 |
| -3 | `UTB_IPC_ERR_EMPTY` | ring 空 |
| -4 | `UTB_IPC_ERR_PARAM` | 参数非法（如 NULL 指针） |
| -5 | `UTB_IPC_ERR_NOT_READY` | IPC 未就绪（magic/ready 未设置） |
| -6 | `UTB_IPC_ERR_OVERFLOW` | payload 超过最大长度 |
| -7 | `UTB_IPC_ERR_PEER_DOWN` | 对端异常 |
| -8 | `UTB_IPC_ERR_INVALID` | 消息头 magic 校验失败 |
| -9 | `UTB_IPC_ERR_BUSY` | 快照写者正在更新（读者重试耗尽） |
| -10 | `UTB_IPC_ERR_PEER_RESTART` | 对端重启，事务失效 |

---

## 9. SPSC Ring 工作原理

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

**关键特性：**

- `head` 和 `tail` 都是 `volatile uint32_t`，通过 `fence` 保证跨核可见性
- 不使用 mutex / spinlock / 关中断
- 空判断：`head == tail`
- 满判断：`(tail + 1) % depth == head`（牺牲一个槽位）
- 槽位地址 = ring 基地址 + 32B(desc) + slot_size × index

**写入序列（producer）：**

```
1. 构造消息头 + payload
2. 检查 ring 是否满 → 满则返回 ERR_FULL
3. 将消息写入 slot[tail]
4. fence                    ← 确保数据写入完成
5. tail = (tail + 1) % depth
6. fence                    ← 确保 tail 更新对 consumer 可见
7. IDU_SendICI()            ← 通知对端
```

**读取序列（consumer）：**

```
1. 检查 head == tail → 相等则返回 ERR_EMPTY
2. fence                    ← 确保先读到 tail 再读 slot
3. 从 slot[head] 读取消息头 + payload
4. 校验消息头 magic
5. fence                    ← 确保读完 slot 数据
6. head = (head + 1) % depth
7. fence                    ← 确保 head 更新对 producer 可见
```

---

## 10. Seqlock 快照原理

快照使用 seqlock 模型保证读者看到完整的数据，无需加锁。

**写者（从核，单写者）：**

```
generation++          → 变为奇数（标记「写入中」）
fence
写入 data[] + timestamp
fence
generation++          → 变为偶数（标记「写入完成」）
fence
```

**读者（主核）：**

```
gen1 = generation     → 若奇数则跳过（写者正在更新）
fence
读取 data[] + timestamp
fence
gen2 = generation     → 若 gen1 != gen2 则重试（读取期间发生了写入）
```

最多重试 16 次。若写者持续高频更新导致每次读取都被打断，返回 `UTB_IPC_ERR_BUSY`。

---

## 11. IDU 门铃机制

```
主核                                  从核
  │                                    │
  │  写 ring slot                      │
  │  fence                             │
  │  更新 tail                         │
  │  fence                             │
  │  IDU_SendICI(IDU_BASE, 1, 0) ───> │  INTER_CORE_IRQn(19) 触发
  │                                    │  ISR: IDU_GetProcessxICI_Status()
  │                                    │  ISR: IDU_ProcessxClearICI()
  │                                    │  ISR: 回调（唤醒 IPC task）
  │                                    │  task: ring_pop() 处理消息
```

ISR 回调当前为空实现（轮询模式）。集成 FreeRTOS 后应改为任务通知唤醒。

---

## 12. 心跳与恢复机制

### 12.1 心跳

每核周期性调用 `utb_ipc_heartbeat_tick(tick)` 将当前 tick 写入控制块的 `hb_tick` 字段。对端通过 `utb_ipc_is_peer_alive(tick, max_age)` 检查心跳年龄。

### 12.2 重启检测

每核在 `utb_ipc_init()` 中递增控制块的 `boot_gen` 字段。对端通过 `utb_ipc_check_peer_restart()` 缓存并对比 `boot_gen`，检测到变化即判定对端发生了重启。

### 12.3 恢复流程

```
应用层 IPC 服务 task：
  1. utb_ipc_check_peer_restart() 返回 1
  2. 终止当前进行中的同步请求
  3. 调用 utb_ipc_recover() → 内部重新执行 utb_ipc_init()
  4. 等待 utb_ipc_is_peer_ready() 返回 1
  5. 恢复正常通信
```

---

## 13. 诊断计数器

`g_ipc_diag`（类型 `utb_ipc_diag_t`）是核私有的全局变量，每核各一份。各 IPC 模块在操作成功/失败时自动递增对应计数器。

| 计数器 | 说明 |
|--------|------|
| `ctrl_tx` | ctrl 消息发送成功次数 |
| `ctrl_tx_full` | ctrl 消息发送失败（ring 满） |
| `ctrl_rx` | ctrl 消息接收次数 |
| `ctrl_rx_invalid` | ctrl 消息头校验失败 |
| `evt_tx` | 事件发送成功次数 |
| `evt_tx_drop` | 事件因 ring 满被丢弃 |
| `evt_rx` | 事件接收次数 |
| `sync_timeout` | 同步请求超时次数 |
| `sync_retry` | 同步请求重试次数 |
| `hb_tx` | 心跳发送次数 |
| `peer_restart` | 检测到对端重启次数 |
| `recovery` | IPC 恢复次数 |
| `snap_write` | 快照写入次数 |
| `snap_read` | 快照读取成功次数 |
| `snap_read_busy` | 快照读取因写者忙失败 |
| `last_error` | 最后一次错误码 |

调用 `utb_ipc_diag_reset()` 可清零所有计数器。从核可将自己的 `g_ipc_diag` 写入 `stats_snapshot` 发布给主核。

---

## 14. 开发注意事项

### 14.1 内存屏障

QL25 无 D-Cache，但 `UTB_FENCE()` 仍然必须使用。它对应 RISC-V `fence` 指令，保证一个核的写入按顺序对另一核可见。

**必须使用 fence 的场景：**
- 写完 ring slot 数据后，更新 tail 之前
- 更新 tail 后（使对端可见）
- 写完控制块字段后
- seqlock 的 generation 递增前后

### 14.2 ISR 安全

| 函数 | ISR 安全 | 说明 |
|------|---------|------|
| `utb_ipc_ring_push/pop` | 是 | 无锁操作 |
| `utb_ipc_kick_peer` | 是 | 仅写 IDU 寄存器 |
| `utb_ipc_ctrl_request_sync` | **否** | 阻塞轮询，禁止 ISR 调用 |
| `utb_ipc_heartbeat_tick` | 是 | 单次 volatile 写 |
| `utb_ipc_snapshot_write` | 否 | 耗时逐字节写，不宜在 ISR 中 |

### 14.3 栈开销

| 函数 | 栈开销 |
|------|--------|
| `utb_ipc_ctrl_request` | ~40B |
| `utb_ipc_ctrl_request_sync` | ~140B（24B hdr + 104B tmp_payload + 局部变量） |
| `utb_ipc_event_send` | ~40B |
| `utb_ipc_snapshot_write` | ~20B |

`ctrl_request_sync` 的 140B 栈开销需注意调用链深度。推荐调用方 task 栈 >= 512B。

### 14.4 新增命令步骤

1. 在 `utb_ipc_msg.h` 中定义 `UTB_CMD_xxx` 和对应 payload 结构
2. 添加编译期断言确保 payload 不超过 `UTB_IPC_CTRL_PAYLOAD_MAX`
3. 在从核 IPC 服务 task 的 `switch(hdr.cmd_id)` 中添加处理分支
4. 如需同步调用，使用 `utb_ipc_ctrl_request_sync()`

### 14.5 新增事件步骤

1. 在 `utb_ipc.h` 中定义 `UTB_IPC_EVT_xxx`
2. 在 `utb_ipc_msg.h` 中定义 payload 结构 + 编译期断言
3. 从核调用 `utb_ipc_event_send()` 或 `utb_ipc_event_send_urgent()`
4. 主核在 event 处理循环的 `switch(hdr.cmd_id)` 中添加分支

---

## 15. API 速查表

### 初始化与状态

| 函数 | 说明 |
|------|------|
| `utb_ipc_init(is_primary)` | IPC 层初始化 |
| `utb_ipc_is_peer_ready()` | 查询对端是否就绪 |
| `utb_ipc_get_core_id()` | 获取本核 ID（0 或 1） |

### 控制通道

| 函数 | 说明 |
|------|------|
| `utb_ipc_ctrl_request(cmd_id, payload, len)` | 发送控制请求（异步） |
| `utb_ipc_ctrl_request_sync(...)` | 发送控制请求并等待响应（同步） |
| `utb_ipc_ctrl_respond(txn_id, result, payload, len)` | 发送控制响应 |
| `utb_ipc_recv(ring_addr, &hdr, payload, max_len)` | 从指定 ring 接收消息 |

### 事件通道

| 函数 | 说明 |
|------|------|
| `utb_ipc_event_send(event_id, payload, len)` | 发送事件 |
| `utb_ipc_event_send_urgent(event_id, payload, len)` | 发送紧急事件 |
| `utb_ipc_event_recv(&hdr, payload, max_len)` | 接收事件 |
| `utb_ipc_event_pending()` | 查询待处理事件数 |

### 快照

| 函数 | 说明 |
|------|------|
| `utb_ipc_snapshot_write(addr, data, len, timestamp)` | 写入快照（从核） |
| `utb_ipc_snapshot_read(addr, data, max_len, &gen, &ts)` | 读取快照（主核） |

### 心跳与恢复

| 函数 | 说明 |
|------|------|
| `utb_ipc_heartbeat_tick(current_tick)` | 更新本核心跳 |
| `utb_ipc_is_peer_alive(current_tick, max_age)` | 检查对端心跳是否新鲜 |
| `utb_ipc_check_peer_restart()` | 检测对端是否重启 |
| `utb_ipc_recover()` | 重新初始化 IPC |

### 诊断

| 函数 | 说明 |
|------|------|
| `utb_ipc_diag_get()` | 获取诊断计数器指针 |
| `utb_ipc_diag_reset()` | 清零所有计数器 |

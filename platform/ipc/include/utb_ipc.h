/**
 * @file utb_ipc.h
 * @brief QL25 双核 IPC 公共定义
 *
 * 基于共享内存 SPSC ring + IDU 核间中断的 AMP IPC 层。
 * 主核（Core 1）为管理面，从核（Core 0）为数据面。
 *
 * 约束：
 *   - FreeRTOS queue/semaphore/event group 不得用于跨核通信
 *   - 跨核通信的唯一合法机制是共享内存 + IDU 核间中断
 *   - QL25 无 D-Cache，cache 操作为空，但 fence 必须保留
 */

#ifndef UTB_IPC_H
#define UTB_IPC_H

#include <stdint.h>

/* ========================================================================
 * 核标识
 * ======================================================================== */

#define UTB_CORE_SECONDARY      0       /* Core 0: 从核（数据面） */
#define UTB_CORE_PRIMARY        1       /* Core 1: 主核（管理面） */

/* ========================================================================
 * 内存屏障（QL25 RISC-V）
 * ======================================================================== */

/*
 * QL25 无 D-Cache，不需要 cache clean/invalidate。
 * 但 fence 仍然必须：保证一个核的写入按顺序对另一核可见。
 */
#define UTB_FENCE()         __asm volatile("fence" ::: "memory")
#define UTB_FENCE_I()       __asm volatile("fence.i" ::: "memory")
#define UTB_CACHE_CLEAN(addr, size)         ((void)(addr), (void)(size))
#define UTB_CACHE_INVALIDATE(addr, size)    ((void)(addr), (void)(size))

/* ========================================================================
 * 共享内存布局（SRAM3 @ 0x30060000，64KB）
 * ======================================================================== */

#define UTB_IPC_SHM_BASE        0x30060000UL
#define UTB_IPC_SHM_SIZE        (64 * 1024)

/*
 * 布局（每个 ring 实际大小 = 32B desc + depth × slot_size）：
 *   0x0000 ~ 0x00FF : IPC 控制块（256B）
 *   0x0100 ~ 0x051F : ctrl_req_ring（32 + 8×128 = 1056B）
 *   0x0600 ~ 0x0A1F : ctrl_rsp_ring（32 + 8×128 = 1056B）
 *   0x0B00 ~ 0x131F : event_ring（32 + 16×128 = 2080B）
 *   0x1400 ~ 0x14FF : state_snapshot（256B）
 *   0x1500 ~ 0x15FF : stats_snapshot（256B）
 *   0x1600 ~ 0xFFFF : 未分配（约 58KB）
 */
#define UTB_IPC_CTL_OFS         0x0000
#define UTB_IPC_CTL_SIZE        256

#define UTB_IPC_CTRL_REQ_OFS    0x0100
#define UTB_IPC_CTRL_RSP_OFS    0x0600
#define UTB_IPC_EVENT_OFS       0x0B00
#define UTB_IPC_STATE_SNAP_OFS  0x1400
#define UTB_IPC_STATS_SNAP_OFS  0x1500

/* ========================================================================
 * IPC 控制块（共享内存起始 256B）
 * ======================================================================== */

#define UTB_IPC_MAGIC           0x49504321UL    /* "IPC!" */
#define UTB_IPC_VERSION         1

/*
 * IPC 控制块由主核初始化，从核验证。
 * 所有字段 4 字节对齐，通过 volatile 访问。
 */
typedef struct {
    volatile uint32_t magic;            /* 0x00: UTB_IPC_MAGIC */
    volatile uint32_t version;          /* 0x04: UTB_IPC_VERSION */
    volatile uint32_t primary_ready;    /* 0x08: 主核写 1 表示初始化完成 */
    volatile uint32_t secondary_ready;  /* 0x0C: 从核写 1 表示就绪 */
    volatile uint32_t primary_seq;      /* 0x10: 主核发送序号 */
    volatile uint32_t secondary_seq;    /* 0x14: 从核发送序号 */
    volatile uint32_t error_count;      /* 0x18: 错误计数（调试用） */
    volatile uint32_t primary_hb_tick;  /* 0x1C: 主核心跳 tick */
    volatile uint32_t secondary_hb_tick;/* 0x20: 从核心跳 tick */
    volatile uint32_t primary_boot_gen; /* 0x24: 主核启动代数（递增） */
    volatile uint32_t secondary_boot_gen;/* 0x28: 从核启动代数（递增） */
    volatile uint32_t reserved[53];     /* 0x2C ~ 0xFF: 预留 */
} utb_ipc_ctl_t;

/* 编译期断言：控制块必须恰好 256B */
typedef char _utb_assert_ctl_size
    [(sizeof(utb_ipc_ctl_t) == 256) ? 1 : -1];

/* 控制块访问（static inline，所有 IPC 源文件共用） */
static inline volatile utb_ipc_ctl_t *utb_ipc_ctl(void)
{
    return (volatile utb_ipc_ctl_t *)(UTB_IPC_SHM_BASE + UTB_IPC_CTL_OFS);
}

/* ========================================================================
 * 消息头（每个 ring 槽位的前 24 字节）
 * ======================================================================== */

#define UTB_IPC_MSG_MAGIC       0x4950          /* "IP" */

/* 消息类型 */
#define UTB_IPC_MSG_CTRL_REQ    1       /* 控制请求（主核→从核） */
#define UTB_IPC_MSG_CTRL_RSP    2       /* 控制响应（从核→主核） */
#define UTB_IPC_MSG_EVENT       3       /* 事件（从核→主核） */
#define UTB_IPC_MSG_STATE       4       /* 状态快照 */
#define UTB_IPC_MSG_STATS       5       /* 统计快照 */

/* flags 标志位 */
#define UTB_IPC_F_ACK_REQ       (1U << 0)       /* 需要对端 ACK */
#define UTB_IPC_F_URGENT        (1U << 1)       /* 紧急（不可丢弃） */
#define UTB_IPC_F_RETRY         (1U << 2)       /* 重试标记 */

/*
 * 消息头结构体 — 24 字节，4 字节自然对齐。
 *
 * 字段偏移：
 *   0:  magic(2) + version(1) + msg_type(1)
 *   4:  payload_len(2) + flags(2)
 *   8:  seq(4)
 *  12:  txn_id(2) + src_core(1) + dst_core(1)
 *  16:  cmd_id(2) + reserved(2)
 *  20:  result(4)
 *  24:  total
 */
typedef struct {
    uint16_t magic;             /* UTB_IPC_MSG_MAGIC */
    uint8_t  version;           /* IPC 契约版本 */
    uint8_t  msg_type;          /* UTB_IPC_MSG_xxx */
    uint16_t payload_len;       /* payload 实际长度 */
    uint16_t flags;             /* UTB_IPC_F_xxx */
    uint32_t seq;               /* 发送序号 */
    uint16_t txn_id;            /* 事务关联号（req/rsp 配对） */
    uint8_t  src_core;          /* 源核 */
    uint8_t  dst_core;          /* 目的核 */
    uint16_t cmd_id;            /* 命令 ID（请求时有效） */
    uint16_t _reserved;         /* 对齐填充 */
    int32_t  result;            /* 响应结果码（请求时为 0） */
} utb_ipc_msg_hdr_t;

/* 消息头大小 = 24 字节 */
#define UTB_IPC_MSG_HDR_SIZE    24

/* 编译期断言：结构体大小必须与常量一致 */
typedef char _utb_assert_msg_hdr_size
    [(sizeof(utb_ipc_msg_hdr_t) == UTB_IPC_MSG_HDR_SIZE) ? 1 : -1];

/* ========================================================================
 * SPSC Ring 描述符
 * ======================================================================== */

/*
 * Ring 内存布局（连续区域）：
 *   [0 .. sizeof(ring_desc) - 1]       : ring 描述符（head/tail/配置）
 *   [sizeof(ring_desc) .. end]         : slot[0] ~ slot[depth-1]
 *
 * 每个 slot 的前 UTB_IPC_MSG_HDR_SIZE 字节是消息头，
 * 后面是 payload（最大 slot_size - UTB_IPC_MSG_HDR_SIZE）。
 */

#define UTB_IPC_RING_MAGIC      0x52494E47UL    /* "RING" */

typedef struct {
    volatile uint32_t magic;    /* UTB_IPC_RING_MAGIC（初始化标记） */
    volatile uint32_t head;     /* consumer 读索引（只由 consumer 更新） */
    volatile uint32_t tail;     /* producer 写索引（只由 producer 更新） */
    uint32_t depth;             /* 槽位数（2 的幂，初始化后只读） */
    uint32_t slot_size;         /* 每槽字节数（初始化后只读） */
    uint32_t reserved[3];       /* 对齐到 32B */
} utb_ipc_ring_desc_t;

#define UTB_IPC_RING_DESC_SIZE  32

/* Ring 配置 */
#define UTB_IPC_CTRL_RING_DEPTH     8
#define UTB_IPC_CTRL_RING_SLOT_SIZE 128     /* 头 24B + payload 最大 104B = 128B */
#define UTB_IPC_EVENT_RING_DEPTH    16
#define UTB_IPC_EVENT_RING_SLOT_SIZE 128    /* 与 ctrl 相同 */

/* payload 最大长度 = slot_size - 消息头 */
#define UTB_IPC_CTRL_PAYLOAD_MAX    (UTB_IPC_CTRL_RING_SLOT_SIZE - UTB_IPC_MSG_HDR_SIZE)
#define UTB_IPC_EVENT_PAYLOAD_MAX   (UTB_IPC_EVENT_RING_SLOT_SIZE - UTB_IPC_MSG_HDR_SIZE)

/* ========================================================================
 * 错误码
 * ======================================================================== */

#define UTB_IPC_OK              0
#define UTB_IPC_ERR_TIMEOUT     (-1)    /* 操作超时 */
#define UTB_IPC_ERR_FULL        (-2)    /* ring 满 */
#define UTB_IPC_ERR_EMPTY       (-3)    /* ring 空 */
#define UTB_IPC_ERR_PARAM       (-4)    /* 参数非法 */
#define UTB_IPC_ERR_NOT_READY   (-5)    /* IPC 未就绪 */
#define UTB_IPC_ERR_OVERFLOW    (-6)    /* payload 超长 */
#define UTB_IPC_ERR_PEER_DOWN   (-7)    /* 对端异常 */
#define UTB_IPC_ERR_INVALID     (-8)    /* 消息非法 */
#define UTB_IPC_ERR_BUSY        (-9)    /* 快照正在被写入 */
#define UTB_IPC_ERR_PEER_RESTART (-10)  /* 对端重启，事务失效 */

/* ========================================================================
 * 事件类型（event_ring 消息的 cmd_id 字段）
 * ======================================================================== */

#define UTB_IPC_EVT_ALARM       0x0001  /* 告警（链路状态变化等） */
#define UTB_IPC_EVT_FAULT       0x0002  /* 故障（不可恢复错误） */
#define UTB_IPC_EVT_METADATA    0x0003  /* 元数据上报 */
#define UTB_IPC_EVT_LINK_UP     0x0004  /* 链路 UP */
#define UTB_IPC_EVT_LINK_DOWN   0x0005  /* 链路 DOWN */

/* ========================================================================
 * 快照结构体（共享内存中的 seqlock 模型）
 *
 * 写者（从核）：
 *   1. generation++ (变为奇数，表示写入中)
 *   2. fence
 *   3. 写入 data[]
 *   4. fence
 *   5. generation++ (变为偶数，表示写入完成)
 *   6. fence
 *
 * 读者（主核）：
 *   1. 读 generation → gen1（若为奇数则重试）
 *   2. fence
 *   3. 读 data[]
 *   4. fence
 *   5. 读 generation → gen2
 *   6. 若 gen1 != gen2 则重试
 * ======================================================================== */

#define UTB_IPC_SNAP_MAGIC      0x534E4150UL    /* "SNAP" */
#define UTB_IPC_SNAP_DATA_SIZE  240             /* 256 - 16 = 240B 可用数据 */

typedef struct {
    volatile uint32_t magic;        /* UTB_IPC_SNAP_MAGIC */
    volatile uint32_t generation;   /* seqlock 序号（偶数=有效，奇数=写入中） */
    volatile uint32_t timestamp;    /* 最后更新的 tick 计数 */
    uint32_t          reserved;     /* 对齐到 16B 头 */
    uint8_t           data[UTB_IPC_SNAP_DATA_SIZE]; /* 快照数据 */
} utb_ipc_snapshot_t;

/* 编译期断言：快照结构体必须恰好 256B，与共享内存布局间距一致 */
typedef char _utb_assert_snap_size
    [(sizeof(utb_ipc_snapshot_t) == 256) ? 1 : -1];

/* ========================================================================
 * Ring API（底层，通用 SPSC ring 操作）
 * ======================================================================== */

/**
 * 初始化 ring 描述符和槽位区域。
 * @param shm_addr  ring 所在共享内存起始地址
 * @param depth     槽位数（必须 >= 2）
 * @param slot_size 每槽字节数
 */
void utb_ipc_ring_init(uint32_t shm_addr, uint32_t depth, uint32_t slot_size);

/**
 * 向 ring 写入一条消息（producer 调用）。
 * @return UTB_IPC_OK 成功，UTB_IPC_ERR_FULL ring 满
 */
int utb_ipc_ring_push(uint32_t shm_addr, const utb_ipc_msg_hdr_t *hdr,
                      const void *payload, uint16_t payload_len);

/**
 * 从 ring 读取一条消息（consumer 调用）。
 * @param hdr       输出消息头
 * @param payload   输出 payload 缓冲区
 * @param max_len   payload 缓冲区最大长度
 * @return UTB_IPC_OK 成功，UTB_IPC_ERR_EMPTY ring 空
 */
int utb_ipc_ring_pop(uint32_t shm_addr, utb_ipc_msg_hdr_t *hdr,
                     void *payload, uint16_t max_len);

/** ring 中待消费的消息数 */
uint32_t utb_ipc_ring_pending(uint32_t shm_addr);

/** ring 是否满 */
int utb_ipc_ring_is_full(uint32_t shm_addr);

/* ========================================================================
 * IDU 门铃 API
 * ======================================================================== */

/**
 * 初始化 IDU 门铃（使能时钟 + 注册 ISR）。
 * 双核都需要调用。
 * @param notify_cb  收到对端中断时的回调（在 ISR 中调用，需简短）
 */
void utb_ipc_notify_init(void (*notify_cb)(void));

/** 向对端核发送门铃中断 */
void utb_ipc_kick_peer(void);

/** 获取本核的 processor_id（0 或 1） */
uint32_t utb_ipc_get_core_id(void);

/* ========================================================================
 * IPC 高层 API
 * ======================================================================== */

/**
 * IPC 层初始化。
 * @param is_primary  非零表示主核（初始化共享内存），零表示从核（验证并注册）
 * @return UTB_IPC_OK 成功
 */
int utb_ipc_init(int is_primary);

/**
 * 查询对端是否就绪。
 * @return 非零表示就绪
 */
int utb_ipc_is_peer_ready(void);

/**
 * 发送控制请求（主核→从核）。
 * @param cmd_id    命令 ID
 * @param payload   命令参数（可以为 NULL）
 * @param len       参数长度
 * @return UTB_IPC_OK 成功，< 0 错误
 */
int utb_ipc_ctrl_request(uint16_t cmd_id, const void *payload, uint16_t len);

/**
 * 发送控制响应（从核→主核）。
 * @param txn_id    请求中的事务号（用于配对）
 * @param result    结果码
 * @param payload   响应数据（可以为 NULL）
 * @param len       数据长度
 * @return UTB_IPC_OK 成功，< 0 错误
 */
int utb_ipc_ctrl_respond(uint16_t txn_id, int32_t result,
                         const void *payload, uint16_t len);

/**
 * 接收一条消息（从对应 ring 中 pop）。
 * @param ring_shm_addr  ring 共享内存地址（ctrl_req 或 ctrl_rsp）
 * @param hdr            输出消息头
 * @param payload        输出 payload
 * @param max_len        payload 缓冲区大小
 * @return UTB_IPC_OK 成功，UTB_IPC_ERR_EMPTY 无消息
 */
int utb_ipc_recv(uint32_t ring_shm_addr, utb_ipc_msg_hdr_t *hdr,
                 void *payload, uint16_t max_len);

/* ========================================================================
 * 事件 API（从核→主核，单向 SPSC ring）
 * ======================================================================== */

/**
 * 发送事件（从核调用）。
 * @param event_id  事件类型 UTB_IPC_EVT_xxx
 * @param payload   事件数据（可为 NULL）
 * @param len       数据长度
 * @return UTB_IPC_OK 成功，UTB_IPC_ERR_FULL ring 满
 */
int utb_ipc_event_send(uint16_t event_id, const void *payload, uint16_t len);

/**
 * 接收事件（主核调用）。
 * @param hdr       输出消息头
 * @param payload   输出 payload
 * @param max_len   payload 缓冲区大小
 * @return UTB_IPC_OK 成功，UTB_IPC_ERR_EMPTY 无事件
 */
int utb_ipc_event_recv(utb_ipc_msg_hdr_t *hdr, void *payload, uint16_t max_len);

/** event_ring 中待消费的事件数 */
uint32_t utb_ipc_event_pending(void);

/* ========================================================================
 * 快照 API（从核写、主核读，seqlock 模型）
 * ======================================================================== */

/**
 * 初始化快照区域（主核调用，在 utb_ipc_init 中自动调用）。
 * @param shm_addr  快照共享内存地址
 */
void utb_ipc_snapshot_init(uint32_t shm_addr);

/**
 * 写入快照（从核调用）。
 * 使用 seqlock 保证读者看到完整数据。
 * @param shm_addr   快照共享内存地址
 * @param data       数据源
 * @param len        数据长度（<= UTB_IPC_SNAP_DATA_SIZE）
 * @param timestamp  当前 tick 计数
 * @return UTB_IPC_OK 成功
 */
int utb_ipc_snapshot_write(uint32_t shm_addr, const void *data,
                           uint32_t len, uint32_t timestamp);

/**
 * 读取快照（主核调用）。
 * 内部重试直到读到一致的数据（有界重试）。
 * @param shm_addr    快照共享内存地址
 * @param data        输出缓冲区
 * @param max_len     缓冲区大小
 * @param out_gen     输出当前 generation（可为 NULL）
 * @param out_ts      输出 timestamp（可为 NULL）
 * @return UTB_IPC_OK 成功，UTB_IPC_ERR_BUSY 写者持续写入（重试耗尽）
 */
int utb_ipc_snapshot_read(uint32_t shm_addr, void *data, uint32_t max_len,
                          uint32_t *out_gen, uint32_t *out_ts);

/* 便捷宏：state / stats 快照地址 */
#define UTB_IPC_STATE_SNAP_ADDR (UTB_IPC_SHM_BASE + UTB_IPC_STATE_SNAP_OFS)
#define UTB_IPC_STATS_SNAP_ADDR (UTB_IPC_SHM_BASE + UTB_IPC_STATS_SNAP_OFS)
#define UTB_IPC_EVENT_RING_ADDR (UTB_IPC_SHM_BASE + UTB_IPC_EVENT_OFS)

/* ========================================================================
 * IPC 诊断计数器（核私有，不在共享内存中）
 *
 * 每核各自维护一份，不存在跨核竞争。
 * 从核通过 stats_snapshot 发布给主核。
 * ======================================================================== */

typedef struct {
    /* ctrl ring */
    uint32_t ctrl_tx;               /* ctrl 发送成功计数 */
    uint32_t ctrl_tx_full;          /* ctrl 发送失败（ring 满） */
    uint32_t ctrl_rx;               /* ctrl 接收计数 */
    uint32_t ctrl_rx_invalid;       /* ctrl 接收消息头非法 */
    /* event ring */
    uint32_t evt_tx;                /* event 发送成功计数 */
    uint32_t evt_tx_drop;           /* event 发送丢弃（ring 满） */
    uint32_t evt_rx;                /* event 接收计数 */
    /* timeout / retry（同步请求） */
    uint32_t sync_timeout;          /* 同步请求超时次数 */
    uint32_t sync_retry;            /* 同步请求重试次数 */
    /* heartbeat / recovery */
    uint32_t hb_tx;                 /* 心跳发送计数 */
    uint32_t peer_restart;          /* 检测到对端重启次数 */
    uint32_t recovery;              /* IPC 恢复次数 */
    /* snapshot */
    uint32_t snap_write;            /* 快照写入次数 */
    uint32_t snap_read;             /* 快照读取次数 */
    uint32_t snap_read_busy;        /* 快照读取因写者忙失败 */
    /* general */
    uint32_t last_error;            /* 最后一次错误码 */
} utb_ipc_diag_t;                   /* 16 × 4 = 64B */

extern utb_ipc_diag_t g_ipc_diag;

/** 重置所有诊断计数器 */
void utb_ipc_diag_reset(void);

/** 获取诊断计数器指针（只读） */
const utb_ipc_diag_t *utb_ipc_diag_get(void);

/* ========================================================================
 * 同步控制请求（阻塞式，带超时重试）
 *
 * 栈开销：约 140 字节（响应 payload 临时缓冲）。
 * 仅限 task 上下文调用，禁止在 ISR 中使用。
 * ======================================================================== */

#define UTB_IPC_SYNC_RETRY_MAX  3       /* 默认最大重试次数 */

/**
 * 发送控制请求并等待响应。
 * @param cmd_id          命令 ID
 * @param req_payload     请求 payload（可为 NULL）
 * @param req_len         请求 payload 长度
 * @param rsp_payload     响应 payload 输出缓冲区（可为 NULL）
 * @param rsp_max_len     响应缓冲区大小
 * @param out_result      输出响应结果码（可为 NULL）
 * @param timeout_cycles  单次等待超时（循环计数）
 * @param max_retry       最大重试次数（0 = 不重试）
 * @return UTB_IPC_OK 成功，UTB_IPC_ERR_TIMEOUT 全部超时
 */
int utb_ipc_ctrl_request_sync(uint16_t cmd_id,
                               const void *req_payload, uint16_t req_len,
                               void *rsp_payload, uint16_t rsp_max_len,
                               int32_t *out_result,
                               uint32_t timeout_cycles, uint32_t max_retry);

/* ========================================================================
 * 心跳 API
 *
 * 应用层周期性调用（如每 500ms），更新控制块中的心跳 tick。
 * ======================================================================== */

/**
 * 更新本核心跳时间戳。
 * @param current_tick  当前 tick 值（由调用者提供，避免 IPC 层依赖特定时钟源）
 */
void utb_ipc_heartbeat_tick(uint32_t current_tick);

/**
 * 检查对端是否存活（心跳是否新鲜）。
 * @param current_tick  当前 tick
 * @param max_age       最大允许心跳年龄（tick 单位）
 * @return 1 = 存活，0 = 超时/未启动
 */
int utb_ipc_is_peer_alive(uint32_t current_tick, uint32_t max_age);

/* ========================================================================
 * 恢复 API
 * ======================================================================== */

/**
 * 检测对端是否重启（通过 boot_gen 变化检测）。
 * 首次调用缓存当前 gen，后续调用对比。
 * @return 1 = 检测到重启，0 = 未变化
 */
int utb_ipc_check_peer_restart(void);

/**
 * IPC 恢复：重新初始化本核 IPC 状态。
 * 应在检测到对端重启后由应用层调用。
 * @return UTB_IPC_OK 成功
 */
int utb_ipc_recover(void);

/* ========================================================================
 * 紧急事件发送（ring 满时尽力投递）
 * ======================================================================== */

/**
 * 发送紧急事件。标记 UTB_IPC_F_URGENT 标志。
 * ring 满时同样返回 ERR_FULL 但计入不同统计。
 * @param event_id  事件类型 UTB_IPC_EVT_xxx
 * @param payload   事件数据（可为 NULL）
 * @param len       数据长度
 * @return UTB_IPC_OK 成功，UTB_IPC_ERR_FULL ring 满
 */
int utb_ipc_event_send_urgent(uint16_t event_id,
                               const void *payload, uint16_t len);

#endif /* UTB_IPC_H */

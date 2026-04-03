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
 * 布局：
 *   0x0000 ~ 0x00FF : IPC 控制块（256B）
 *   0x0100 ~ 0x04FF : ctrl_req_ring（8 × 128B = 1KB）
 *   0x0500 ~ 0x08FF : ctrl_rsp_ring（8 × 128B = 1KB）
 *   0x0900 ~ 0x10FF : event_ring 预留（16 × 128B = 2KB）
 *   0x1100 ~ 0x11FF : state_snapshot 预留（256B）
 *   0x1200 ~ 0x12FF : stats_snapshot 预留（256B）
 *   0x1300 ~ 0xFFFF : 未分配（约 59KB）
 */
#define UTB_IPC_CTL_OFS         0x0000
#define UTB_IPC_CTL_SIZE        256

#define UTB_IPC_CTRL_REQ_OFS    0x0100
#define UTB_IPC_CTRL_RSP_OFS    0x0500
#define UTB_IPC_EVENT_OFS       0x0900      /* 预留，第二步 */
#define UTB_IPC_STATE_SNAP_OFS  0x1100      /* 预留，第二步 */
#define UTB_IPC_STATS_SNAP_OFS  0x1200      /* 预留，第二步 */

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
    volatile uint32_t reserved[57];     /* 0x1C ~ 0xFF: 预留 */
} utb_ipc_ctl_t;

/* ========================================================================
 * 消息头（每个 ring 槽位的前 24 字节）
 * ======================================================================== */

#define UTB_IPC_MSG_MAGIC       0x4950          /* "IP" */

/* 消息类型 */
#define UTB_IPC_MSG_CTRL_REQ    1       /* 控制请求（主核→从核） */
#define UTB_IPC_MSG_CTRL_RSP    2       /* 控制响应（从核→主核） */
#define UTB_IPC_MSG_EVENT       3       /* 事件（从核→主核，第二步） */
#define UTB_IPC_MSG_STATE       4       /* 状态快照（第二步） */
#define UTB_IPC_MSG_STATS       5       /* 统计快照（第二步） */

/* flags 标志位 */
#define UTB_IPC_F_ACK_REQ       (1U << 0)       /* 需要对端 ACK */
#define UTB_IPC_F_URGENT        (1U << 1)       /* 紧急（不可丢弃） */

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
    int32_t  result;            /* 响应结果码（请求时为 0） */
} utb_ipc_msg_hdr_t;

/* 消息头大小 = 24 字节 */
#define UTB_IPC_MSG_HDR_SIZE    sizeof(utb_ipc_msg_hdr_t)

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
#define UTB_IPC_CTRL_RING_SLOT_SIZE 128     /* 头 24B + payload 最大 104B */
#define UTB_IPC_EVENT_RING_DEPTH    16      /* 第二步 */

/* payload 最大长度 = slot_size - 消息头 */
#define UTB_IPC_CTRL_PAYLOAD_MAX    (UTB_IPC_CTRL_RING_SLOT_SIZE - UTB_IPC_MSG_HDR_SIZE)

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

#endif /* UTB_IPC_H */

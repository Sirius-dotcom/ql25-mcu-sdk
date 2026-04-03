/**
 * @file utb_ipc_init.c
 * @brief IPC 层初始化 + 高层 ctrl 消息 API
 *
 * 主核负责：
 *   1. 清零共享内存
 *   2. 初始化控制块 + ring 描述符
 *   3. 标记 primary_ready
 *   4. 注册 IDU 门铃
 *
 * 从核负责：
 *   1. 验证控制块 magic / version
 *   2. 等待 primary_ready
 *   3. 注册 IDU 门铃
 *   4. 标记 secondary_ready
 */

#include "utb_ipc.h"

/* ========================================================================
 * 内部工具
 * ======================================================================== */

static void ipc_memset_zero(void *dst, uint32_t len)
{
    uint8_t *d = (uint8_t *)dst;
    for (uint32_t i = 0; i < len; i++) {
        d[i] = 0;
    }
}

/* 共享内存区域地址 */
static volatile utb_ipc_ctl_t *ipc_ctl(void)
{
    return (volatile utb_ipc_ctl_t *)(UTB_IPC_SHM_BASE + UTB_IPC_CTL_OFS);
}

static uint32_t ctrl_req_ring_addr(void)
{
    return UTB_IPC_SHM_BASE + UTB_IPC_CTRL_REQ_OFS;
}

static uint32_t ctrl_rsp_ring_addr(void)
{
    return UTB_IPC_SHM_BASE + UTB_IPC_CTRL_RSP_OFS;
}

/* 本核序号计数器（核私有，不在共享内存中） */
static uint32_t s_local_seq;
static uint16_t s_local_txn;

/* ========================================================================
 * IPC 门铃回调（ISR 上下文）
 * ======================================================================== */

/*
 * 收到对端 IDU 中断时的回调。
 * 在 ISR 中执行，必须简短。
 *
 * 当前实现：空操作。
 * 实际使用时应唤醒 IPC 服务 task（通过 FreeRTOS 任务通知）。
 *
 * 示例（集成 FreeRTOS 后）：
 *   static void ipc_notify_cb(void) {
 *       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 *       vTaskNotifyGiveFromISR(s_ipc_task_handle, &xHigherPriorityTaskWoken);
 *       portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
 *   }
 */
static void ipc_notify_cb(void)
{
    /* 第一步：空实现，轮询方式使用 */
    /* 第二步集成 FreeRTOS 后改为任务通知唤醒 */
}

/* ========================================================================
 * 主核初始化
 * ======================================================================== */

static int ipc_init_primary(void)
{
    volatile utb_ipc_ctl_t *ctl = ipc_ctl();

    /*
     * Step 1: 清零整个 IPC 共享区域。
     *
     * 只清前 5KB（控制块 + ctrl_req + ctrl_rsp），不清预留区域。
     * 后续第二步的 event_ring / snapshot 由对应初始化函数清零。
     */
    uint32_t init_size = UTB_IPC_CTL_SIZE +
                         (UTB_IPC_RING_DESC_SIZE + UTB_IPC_CTRL_RING_DEPTH * UTB_IPC_CTRL_RING_SLOT_SIZE) * 2;
    ipc_memset_zero((void *)UTB_IPC_SHM_BASE, init_size);

    UTB_FENCE();

    /* Step 2: 初始化控制块 */
    ctl->version = UTB_IPC_VERSION;
    ctl->primary_ready = 0;
    ctl->secondary_ready = 0;
    ctl->primary_seq = 0;
    ctl->secondary_seq = 0;
    ctl->error_count = 0;

    UTB_FENCE();

    /* 写 magic 表示控制块有效（最后写，类似发布模式） */
    ctl->magic = UTB_IPC_MAGIC;

    UTB_FENCE();

    /* Step 3: 初始化两个 ctrl ring */
    utb_ipc_ring_init(ctrl_req_ring_addr(),
                      UTB_IPC_CTRL_RING_DEPTH,
                      UTB_IPC_CTRL_RING_SLOT_SIZE);

    utb_ipc_ring_init(ctrl_rsp_ring_addr(),
                      UTB_IPC_CTRL_RING_DEPTH,
                      UTB_IPC_CTRL_RING_SLOT_SIZE);

    /* Step 4: 初始化 IDU 门铃 */
    utb_ipc_notify_init(ipc_notify_cb);

    /* Step 5: 标记主核就绪 */
    UTB_FENCE();
    ctl->primary_ready = 1;
    UTB_FENCE();

    s_local_seq = 0;
    s_local_txn = 0;

    return UTB_IPC_OK;
}

/* ========================================================================
 * 从核初始化
 * ======================================================================== */

static int ipc_init_secondary(void)
{
    volatile utb_ipc_ctl_t *ctl = ipc_ctl();

    /* Step 1: 等待主核初始化完成（有界等待） */
    uint32_t timeout = 0x01000000;  /* 约 1~2 秒 */
    while (ctl->magic != UTB_IPC_MAGIC && timeout > 0) {
        timeout--;
    }
    if (ctl->magic != UTB_IPC_MAGIC) {
        return UTB_IPC_ERR_NOT_READY;
    }

    /* Step 2: 验证版本 */
    UTB_FENCE();
    if (ctl->version != UTB_IPC_VERSION) {
        return UTB_IPC_ERR_INVALID;
    }

    /* Step 3: 等待 primary_ready */
    timeout = 0x01000000;
    while (ctl->primary_ready == 0 && timeout > 0) {
        timeout--;
    }
    if (ctl->primary_ready == 0) {
        return UTB_IPC_ERR_NOT_READY;
    }

    /* Step 4: 初始化 IDU 门铃 */
    utb_ipc_notify_init(ipc_notify_cb);

    /* Step 5: 标记从核就绪 */
    UTB_FENCE();
    ctl->secondary_ready = 1;
    UTB_FENCE();

    s_local_seq = 0;
    s_local_txn = 0;

    return UTB_IPC_OK;
}

/* ========================================================================
 * 公共初始化入口
 * ======================================================================== */

int utb_ipc_init(int is_primary)
{
    if (is_primary) {
        return ipc_init_primary();
    }
    return ipc_init_secondary();
}

int utb_ipc_is_peer_ready(void)
{
    volatile utb_ipc_ctl_t *ctl = ipc_ctl();
    uint32_t core_id = utb_ipc_get_core_id();

    if (core_id == UTB_CORE_PRIMARY) {
        return (ctl->secondary_ready != 0) ? 1 : 0;
    }
    return (ctl->primary_ready != 0) ? 1 : 0;
}

/* ========================================================================
 * 控制命令发送（主核→从核）
 * ======================================================================== */

int utb_ipc_ctrl_request(uint16_t cmd_id, const void *payload, uint16_t len)
{
    if (len > UTB_IPC_CTRL_PAYLOAD_MAX) {
        return UTB_IPC_ERR_OVERFLOW;
    }

    /* 构造消息头 */
    utb_ipc_msg_hdr_t hdr;
    hdr.magic = UTB_IPC_MSG_MAGIC;
    hdr.version = UTB_IPC_VERSION;
    hdr.msg_type = UTB_IPC_MSG_CTRL_REQ;
    hdr.payload_len = len;
    hdr.flags = UTB_IPC_F_ACK_REQ;
    hdr.seq = s_local_seq++;
    hdr.txn_id = s_local_txn++;
    hdr.src_core = UTB_CORE_PRIMARY;
    hdr.dst_core = UTB_CORE_SECONDARY;
    hdr.result = (int32_t)cmd_id;   /* 复用 result 字段存 cmd_id */

    /* 写入 ctrl_req_ring */
    int ret = utb_ipc_ring_push(ctrl_req_ring_addr(), &hdr, payload, len);
    if (ret != UTB_IPC_OK) {
        return ret;
    }

    /* 通知从核 */
    utb_ipc_kick_peer();
    return UTB_IPC_OK;
}

/* ========================================================================
 * 控制响应发送（从核→主核）
 * ======================================================================== */

int utb_ipc_ctrl_respond(uint16_t txn_id, int32_t result,
                         const void *payload, uint16_t len)
{
    if (len > UTB_IPC_CTRL_PAYLOAD_MAX) {
        return UTB_IPC_ERR_OVERFLOW;
    }

    utb_ipc_msg_hdr_t hdr;
    hdr.magic = UTB_IPC_MSG_MAGIC;
    hdr.version = UTB_IPC_VERSION;
    hdr.msg_type = UTB_IPC_MSG_CTRL_RSP;
    hdr.payload_len = len;
    hdr.flags = 0;
    hdr.seq = s_local_seq++;
    hdr.txn_id = txn_id;
    hdr.src_core = UTB_CORE_SECONDARY;
    hdr.dst_core = UTB_CORE_PRIMARY;
    hdr.result = result;

    int ret = utb_ipc_ring_push(ctrl_rsp_ring_addr(), &hdr, payload, len);
    if (ret != UTB_IPC_OK) {
        return ret;
    }

    utb_ipc_kick_peer();
    return UTB_IPC_OK;
}

/* ========================================================================
 * 消息接收（通用）
 * ======================================================================== */

int utb_ipc_recv(uint32_t ring_shm_addr, utb_ipc_msg_hdr_t *hdr,
                 void *payload, uint16_t max_len)
{
    return utb_ipc_ring_pop(ring_shm_addr, hdr, payload, max_len);
}

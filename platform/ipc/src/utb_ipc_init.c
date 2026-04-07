/**
 * @file utb_ipc_init.c
 * @brief IPC 层初始化 + ctrl 消息 API + 同步请求
 *
 * 主核负责：
 *   1. 保存 boot_gen → 清零共享内存 → 恢复 boot_gen
 *   2. 初始化控制块 + ring 描述符 + 快照
 *   3. 标记 primary_ready
 *   4. 注册 IDU 门铃
 *
 * 从核负责：
 *   1. 验证控制块 magic / version
 *   2. 等待 primary_ready
 *   3. 注册 IDU 门铃
 *   4. 递增 boot_gen，标记 secondary_ready
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

static void ipc_memcpy(void *dst, const void *src, uint32_t len)
{
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    for (uint32_t i = 0; i < len; i++) {
        d[i] = s[i];
    }
}

/* 共享内存区域地址 */
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
    /* 空实现，轮询方式使用 */
    /* 集成 FreeRTOS 后改为任务通知唤醒 */
}

/* ========================================================================
 * 主核初始化
 * ======================================================================== */

static int ipc_init_primary(void)
{
    volatile utb_ipc_ctl_t *ctl = utb_ipc_ctl();

    /*
     * Step 1: 保存 boot_gen（清零会覆盖它）。
     * 首次启动时 boot_gen 已经是 0（未初始化的 SRAM 可能非零，
     * 但 boot_gen 的语义是递增计数，绝对值不重要）。
     */
    uint32_t prev_boot_gen = ctl->primary_boot_gen;

    /*
     * Step 2: 清零整个 IPC 使用区域。
     * 清零范围：从 SHM_BASE 到 stats_snapshot 末尾。
     */
    uint32_t init_size = UTB_IPC_STATS_SNAP_OFS + sizeof(utb_ipc_snapshot_t);
    ipc_memset_zero((void *)UTB_IPC_SHM_BASE, init_size);

    UTB_FENCE();

    /* Step 3: 初始化控制块 */
    ctl->version = UTB_IPC_VERSION;
    ctl->primary_ready = 0;
    ctl->secondary_ready = 0;
    ctl->primary_seq = 0;
    ctl->secondary_seq = 0;
    ctl->error_count = 0;
    ctl->primary_hb_tick = 0;
    ctl->secondary_hb_tick = 0;
    ctl->primary_boot_gen = prev_boot_gen + 1;
    ctl->secondary_boot_gen = 0;

    UTB_FENCE();

    /* 写 magic 表示控制块有效（最后写，发布模式） */
    ctl->magic = UTB_IPC_MAGIC;

    UTB_FENCE();

    /* Step 4: 初始化所有 ring */
    utb_ipc_ring_init(ctrl_req_ring_addr(),
                      UTB_IPC_CTRL_RING_DEPTH,
                      UTB_IPC_CTRL_RING_SLOT_SIZE);

    utb_ipc_ring_init(ctrl_rsp_ring_addr(),
                      UTB_IPC_CTRL_RING_DEPTH,
                      UTB_IPC_CTRL_RING_SLOT_SIZE);

    utb_ipc_ring_init(UTB_IPC_SHM_BASE + UTB_IPC_EVENT_OFS,
                      UTB_IPC_EVENT_RING_DEPTH,
                      UTB_IPC_EVENT_RING_SLOT_SIZE);

    /* Step 5: 初始化快照区域 */
    utb_ipc_snapshot_init(UTB_IPC_STATE_SNAP_ADDR);
    utb_ipc_snapshot_init(UTB_IPC_STATS_SNAP_ADDR);

    /* Step 6: 初始化 IDU 门铃 */
    utb_ipc_notify_init(ipc_notify_cb);

    /* Step 7: 标记主核就绪 */
    UTB_FENCE();
    ctl->primary_ready = 1;
    UTB_FENCE();

    s_local_seq = 0;
    s_local_txn = 0;
    utb_ipc_diag_reset();

    return UTB_IPC_OK;
}

/* ========================================================================
 * 从核初始化
 * ======================================================================== */

static int ipc_init_secondary(void)
{
    volatile utb_ipc_ctl_t *ctl = utb_ipc_ctl();

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

    /* Step 5: 递增 boot_gen + 标记从核就绪 */
    UTB_FENCE();
    ctl->secondary_boot_gen++;
    ctl->secondary_ready = 1;
    UTB_FENCE();

    s_local_seq = 0;
    s_local_txn = 0;
    utb_ipc_diag_reset();

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
    volatile utb_ipc_ctl_t *ctl = utb_ipc_ctl();
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
    hdr.cmd_id = cmd_id;
    hdr._reserved = 0;
    hdr.result = 0;

    int ret = utb_ipc_ring_push(ctrl_req_ring_addr(), &hdr, payload, len);
    if (ret != UTB_IPC_OK) {
        g_ipc_diag.ctrl_tx_full++;
        g_ipc_diag.last_error = (uint32_t)ret;
        return ret;
    }

    g_ipc_diag.ctrl_tx++;
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
    hdr.cmd_id = 0;
    hdr._reserved = 0;
    hdr.result = result;

    int ret = utb_ipc_ring_push(ctrl_rsp_ring_addr(), &hdr, payload, len);
    if (ret != UTB_IPC_OK) {
        g_ipc_diag.ctrl_tx_full++;
        g_ipc_diag.last_error = (uint32_t)ret;
        return ret;
    }

    g_ipc_diag.ctrl_tx++;
    utb_ipc_kick_peer();
    return UTB_IPC_OK;
}

/* ========================================================================
 * 消息接收（通用）
 * ======================================================================== */

int utb_ipc_recv(uint32_t ring_shm_addr, utb_ipc_msg_hdr_t *hdr,
                 void *payload, uint16_t max_len)
{
    int ret = utb_ipc_ring_pop(ring_shm_addr, hdr, payload, max_len);
    if (ret == UTB_IPC_OK) {
        g_ipc_diag.ctrl_rx++;
    } else if (ret == UTB_IPC_ERR_INVALID) {
        g_ipc_diag.ctrl_rx_invalid++;
        g_ipc_diag.last_error = (uint32_t)ret;
    }
    return ret;
}

/* ========================================================================
 * 同步控制请求（阻塞轮询，带超时重试）
 *
 * 栈开销：约 140 字节（24B hdr + 104B payload + 局部变量）。
 * 仅限 task 上下文，禁止 ISR 调用。
 *
 * 流程：
 *   1. 记录即将分配的 txn_id
 *   2. 调用 utb_ipc_ctrl_request() 发送
 *   3. 轮询 ctrl_rsp_ring 等待匹配 txn_id 的响应
 *   4. 超时后重发（设置 F_RETRY），最多 max_retry 次
 *   5. 非匹配的响应直接丢弃（过期事务残留）
 * ======================================================================== */

int utb_ipc_ctrl_request_sync(uint16_t cmd_id,
                               const void *req_payload, uint16_t req_len,
                               void *rsp_payload, uint16_t rsp_max_len,
                               int32_t *out_result,
                               uint32_t timeout_cycles, uint32_t max_retry)
{
    uint32_t attempt;

    for (attempt = 0; attempt <= max_retry; attempt++) {
        /* 记录即将分配的 txn_id（ctrl_request 内部 post-increment） */
        uint16_t expected_txn = s_local_txn;

        /* 发送请求 */
        int ret = utb_ipc_ctrl_request(cmd_id, req_payload, req_len);
        if (ret != UTB_IPC_OK) {
            return ret;
        }

        if (attempt > 0) {
            g_ipc_diag.sync_retry++;
        }

        /* 轮询等待响应 */
        uint32_t countdown = timeout_cycles;
        while (countdown > 0) {
            utb_ipc_msg_hdr_t rsp_hdr;
            uint8_t tmp_payload[UTB_IPC_CTRL_PAYLOAD_MAX];

            ret = utb_ipc_ring_pop(ctrl_rsp_ring_addr(),
                                   &rsp_hdr, tmp_payload,
                                   (uint16_t)sizeof(tmp_payload));

            if (ret == UTB_IPC_OK) {
                g_ipc_diag.ctrl_rx++;

                /* 匹配 txn_id？ */
                if (rsp_hdr.txn_id == expected_txn) {
                    /* 拷贝响应 payload */
                    if (rsp_payload != (void *)0 && rsp_max_len > 0) {
                        uint16_t copy_len = rsp_hdr.payload_len;
                        if (copy_len > rsp_max_len) {
                            copy_len = rsp_max_len;
                        }
                        ipc_memcpy(rsp_payload, tmp_payload, copy_len);
                    }
                    if (out_result != (void *)0) {
                        *out_result = rsp_hdr.result;
                    }
                    return UTB_IPC_OK;
                }
                /* 非匹配：过期事务残留，丢弃 */
            } else if (ret == UTB_IPC_ERR_INVALID) {
                g_ipc_diag.ctrl_rx_invalid++;
            }
            /* ERR_EMPTY: ring 空，继续等待 */

            countdown--;
        }

        /* 本次超时 */
        g_ipc_diag.sync_timeout++;
    }

    g_ipc_diag.last_error = (uint32_t)UTB_IPC_ERR_TIMEOUT;
    return UTB_IPC_ERR_TIMEOUT;
}

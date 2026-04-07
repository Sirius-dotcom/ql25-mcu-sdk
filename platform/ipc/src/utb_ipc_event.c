/**
 * @file utb_ipc_event.c
 * @brief 事件 ring 实现（从核→主核，单向）
 *
 * 复用 SPSC ring 底层，提供事件发送/接收的高层接口。
 * Producer：从核（数据面），Consumer：主核（管理面）。
 *
 * 事件类型通过 msg_hdr.cmd_id 区分（UTB_IPC_EVT_xxx）。
 *
 * 反压策略：
 *   ring 满时丢弃新事件，计入 g_ipc_diag.evt_tx_drop。
 *   不修改 ring head（严格保持 SPSC 合约）。
 */

#include "utb_ipc.h"

/* 本核序号（核私有） */
static uint32_t s_evt_seq;

/* ========================================================================
 * 内部：构建事件消息头并推送
 * ======================================================================== */

static int event_push(uint16_t event_id, uint16_t flags,
                      const void *payload, uint16_t len)
{
    if (len > UTB_IPC_EVENT_PAYLOAD_MAX) {
        return UTB_IPC_ERR_OVERFLOW;
    }

    utb_ipc_msg_hdr_t hdr;
    hdr.magic = UTB_IPC_MSG_MAGIC;
    hdr.version = UTB_IPC_VERSION;
    hdr.msg_type = UTB_IPC_MSG_EVENT;
    hdr.payload_len = len;
    hdr.flags = flags;
    hdr.seq = s_evt_seq++;
    hdr.txn_id = 0;
    hdr.src_core = UTB_CORE_SECONDARY;
    hdr.dst_core = UTB_CORE_PRIMARY;
    hdr.cmd_id = event_id;
    hdr._reserved = 0;
    hdr.result = 0;

    uint32_t ring_addr = UTB_IPC_SHM_BASE + UTB_IPC_EVENT_OFS;
    int ret = utb_ipc_ring_push(ring_addr, &hdr, payload, len);
    if (ret == UTB_IPC_ERR_FULL) {
        g_ipc_diag.evt_tx_drop++;
        g_ipc_diag.last_error = (uint32_t)ret;
        return ret;
    }
    if (ret != UTB_IPC_OK) {
        g_ipc_diag.last_error = (uint32_t)ret;
        return ret;
    }

    g_ipc_diag.evt_tx++;
    utb_ipc_kick_peer();
    return UTB_IPC_OK;
}

/* ========================================================================
 * 事件发送（从核调用）
 * ======================================================================== */

int utb_ipc_event_send(uint16_t event_id, const void *payload, uint16_t len)
{
    return event_push(event_id, 0, payload, len);
}

/* ========================================================================
 * 紧急事件发送（从核调用）
 *
 * 标记 UTB_IPC_F_URGENT。ring 满时同样丢弃（保持 SPSC 合约），
 * 但计入同一 drop 计数器供应用层监控。
 * ======================================================================== */

int utb_ipc_event_send_urgent(uint16_t event_id,
                               const void *payload, uint16_t len)
{
    return event_push(event_id, UTB_IPC_F_URGENT, payload, len);
}

/* ========================================================================
 * 事件接收（主核调用）
 * ======================================================================== */

int utb_ipc_event_recv(utb_ipc_msg_hdr_t *hdr, void *payload, uint16_t max_len)
{
    uint32_t ring_addr = UTB_IPC_SHM_BASE + UTB_IPC_EVENT_OFS;
    int ret = utb_ipc_ring_pop(ring_addr, hdr, payload, max_len);
    if (ret == UTB_IPC_OK) {
        g_ipc_diag.evt_rx++;
    }
    return ret;
}

/* ========================================================================
 * 事件查询
 * ======================================================================== */

uint32_t utb_ipc_event_pending(void)
{
    uint32_t ring_addr = UTB_IPC_SHM_BASE + UTB_IPC_EVENT_OFS;
    return utb_ipc_ring_pending(ring_addr);
}

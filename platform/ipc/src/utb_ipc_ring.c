/**
 * @file utb_ipc_ring.c
 * @brief SPSC lock-free ring 实现
 *
 * Single-Producer Single-Consumer 无锁环形缓冲区。
 * head 只由 consumer 更新，tail 只由 producer 更新。
 * 两个索引通过 volatile 访问 + fence 保证跨核可见性。
 *
 * 不使用 mutex / spinlock / 关中断。
 */

#include "utb_ipc.h"

/* ========================================================================
 * 内部工具
 * ======================================================================== */

/* 获取 ring 描述符指针 */
static volatile utb_ipc_ring_desc_t *ring_desc(uint32_t shm_addr)
{
    return (volatile utb_ipc_ring_desc_t *)shm_addr;
}

/* 获取 slot 起始地址（描述符之后） */
static uint8_t *ring_slot(uint32_t shm_addr, uint32_t index)
{
    volatile utb_ipc_ring_desc_t *rd = ring_desc(shm_addr);
    uint32_t slot_base = shm_addr + UTB_IPC_RING_DESC_SIZE;
    return (uint8_t *)(slot_base + rd->slot_size * index);
}

/* 字节拷贝（不依赖 libc） */
static void ipc_memcpy(void *dst, const void *src, uint32_t len)
{
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    for (uint32_t i = 0; i < len; i++) {
        d[i] = s[i];
    }
}

/* 字节置零 */
static void ipc_memset(void *dst, uint8_t val, uint32_t len)
{
    uint8_t *d = (uint8_t *)dst;
    for (uint32_t i = 0; i < len; i++) {
        d[i] = val;
    }
}

/* ========================================================================
 * Ring 初始化
 * ======================================================================== */

void utb_ipc_ring_init(uint32_t shm_addr, uint32_t depth, uint32_t slot_size)
{
    volatile utb_ipc_ring_desc_t *rd = ring_desc(shm_addr);

    /* 清零整个 ring 区域（描述符 + 所有 slot） */
    uint32_t total = UTB_IPC_RING_DESC_SIZE + depth * slot_size;
    ipc_memset((void *)shm_addr, 0, total);

    /* 写入描述符 */
    rd->head = 0;
    rd->tail = 0;
    rd->depth = depth;
    rd->slot_size = slot_size;

    UTB_FENCE();

    /* 最后写 magic，表示初始化完成 */
    rd->magic = UTB_IPC_RING_MAGIC;

    UTB_FENCE();
}

/* ========================================================================
 * Ring Push（producer 调用）
 * ======================================================================== */

int utb_ipc_ring_push(uint32_t shm_addr, const utb_ipc_msg_hdr_t *hdr,
                      const void *payload, uint16_t payload_len)
{
    volatile utb_ipc_ring_desc_t *rd = ring_desc(shm_addr);

    if (rd->magic != UTB_IPC_RING_MAGIC) {
        return UTB_IPC_ERR_NOT_READY;
    }

    uint32_t head = rd->head;   /* 读 consumer 的 head（volatile） */
    uint32_t tail = rd->tail;   /* 读自己的 tail */
    uint32_t depth = rd->depth;

    /* 满判断：tail 的下一个位置 == head */
    uint32_t next_tail = (tail + 1) % depth;
    if (next_tail == head) {
        return UTB_IPC_ERR_FULL;
    }

    /* payload 长度校验 */
    uint32_t max_payload = rd->slot_size - UTB_IPC_MSG_HDR_SIZE;
    if (payload_len > max_payload) {
        return UTB_IPC_ERR_OVERFLOW;
    }

    /* 写入 slot：先写消息头，再写 payload */
    uint8_t *slot = ring_slot(shm_addr, tail);
    ipc_memcpy(slot, hdr, UTB_IPC_MSG_HDR_SIZE);

    if (payload != (void *)0 && payload_len > 0) {
        ipc_memcpy(slot + UTB_IPC_MSG_HDR_SIZE, payload, payload_len);
    }

    /*
     * barrier 序列：
     * 1. fence 确保 slot 数据对另一核可见
     * 2. 更新 tail（发布写入）
     * 3. fence 确保 tail 更新对另一核可见
     */
    UTB_FENCE();
    rd->tail = next_tail;
    UTB_FENCE();

    return UTB_IPC_OK;
}

/* ========================================================================
 * Ring Pop（consumer 调用）
 * ======================================================================== */

int utb_ipc_ring_pop(uint32_t shm_addr, utb_ipc_msg_hdr_t *hdr,
                     void *payload, uint16_t max_len)
{
    volatile utb_ipc_ring_desc_t *rd = ring_desc(shm_addr);

    if (rd->magic != UTB_IPC_RING_MAGIC) {
        return UTB_IPC_ERR_NOT_READY;
    }

    uint32_t head = rd->head;
    uint32_t tail = rd->tail;   /* 读 producer 的 tail（volatile） */

    /* 空判断 */
    if (head == tail) {
        return UTB_IPC_ERR_EMPTY;
    }

    UTB_FENCE();    /* 确保先读到 tail 再读 slot 数据 */

    /* 读取 slot */
    uint8_t *slot = ring_slot(shm_addr, head);
    ipc_memcpy(hdr, slot, UTB_IPC_MSG_HDR_SIZE);

    /* 校验消息头 magic */
    if (hdr->magic != UTB_IPC_MSG_MAGIC) {
        return UTB_IPC_ERR_INVALID;
    }

    /* 拷贝 payload */
    uint16_t copy_len = hdr->payload_len;
    if (copy_len > max_len) {
        copy_len = max_len;
    }
    if (payload != (void *)0 && copy_len > 0) {
        ipc_memcpy(payload, slot + UTB_IPC_MSG_HDR_SIZE, copy_len);
    }

    /*
     * barrier 序列：
     * 1. fence 确保读完 slot 数据
     * 2. 更新 head（释放槽位）
     * 3. fence 确保 head 更新对 producer 可见
     */
    UTB_FENCE();
    rd->head = (head + 1) % rd->depth;
    UTB_FENCE();

    return UTB_IPC_OK;
}

/* ========================================================================
 * Ring 查询
 * ======================================================================== */

uint32_t utb_ipc_ring_pending(uint32_t shm_addr)
{
    volatile utb_ipc_ring_desc_t *rd = ring_desc(shm_addr);
    uint32_t head = rd->head;
    uint32_t tail = rd->tail;
    uint32_t depth = rd->depth;

    if (tail >= head) {
        return tail - head;
    }
    return depth - head + tail;
}

int utb_ipc_ring_is_full(uint32_t shm_addr)
{
    volatile utb_ipc_ring_desc_t *rd = ring_desc(shm_addr);
    uint32_t next_tail = (rd->tail + 1) % rd->depth;
    return (next_tail == rd->head) ? 1 : 0;
}

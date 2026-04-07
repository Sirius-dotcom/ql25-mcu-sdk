/**
 * @file utb_ipc_snapshot.c
 * @brief 状态/统计快照实现（seqlock 模型）
 *
 * 从核（写者）使用 generation 序号保护数据一致性：
 *   - generation 奇数 = 正在写入
 *   - generation 偶数 = 数据有效
 *
 * 主核（读者）通过双次读取 generation 检测撕裂读。
 * 无锁、无阻塞、无 ISR 依赖。
 *
 * 适用于 state_snapshot 和 stats_snapshot 两个区域。
 */

#include "utb_ipc.h"

/* ========================================================================
 * 内部工具
 * ======================================================================== */

static void snap_memcpy(void *dst, const volatile void *src, uint32_t len)
{
    uint8_t *d = (uint8_t *)dst;
    const volatile uint8_t *s = (const volatile uint8_t *)src;
    for (uint32_t i = 0; i < len; i++) {
        d[i] = s[i];
    }
}

static void snap_memcpy_to_volatile(volatile void *dst, const void *src, uint32_t len)
{
    volatile uint8_t *d = (volatile uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    for (uint32_t i = 0; i < len; i++) {
        d[i] = s[i];
    }
}

static void snap_memset(volatile void *dst, uint8_t val, uint32_t len)
{
    volatile uint8_t *d = (volatile uint8_t *)dst;
    for (uint32_t i = 0; i < len; i++) {
        d[i] = val;
    }
}

/* ========================================================================
 * 快照初始化（主核调用）
 * ======================================================================== */

void utb_ipc_snapshot_init(uint32_t shm_addr)
{
    volatile utb_ipc_snapshot_t *snap = (volatile utb_ipc_snapshot_t *)shm_addr;

    /* 清零整个快照区域 */
    snap_memset((volatile void *)shm_addr, 0, sizeof(utb_ipc_snapshot_t));

    UTB_FENCE();

    /* 初始化头部 */
    snap->generation = 0;   /* 偶数 = 有效但数据为空 */
    snap->timestamp = 0;
    snap->reserved = 0;

    UTB_FENCE();

    /* 最后写 magic */
    snap->magic = UTB_IPC_SNAP_MAGIC;

    UTB_FENCE();
}

/* ========================================================================
 * 快照写入（从核调用）
 *
 * Seqlock 写序列：
 *   1. generation++ → 奇数（标记写入中）
 *   2. fence
 *   3. 写数据 + timestamp
 *   4. fence
 *   5. generation++ → 偶数（标记写入完成）
 *   6. fence
 *
 * 注意：写者必须是单线程/单任务，不可多写者并发。
 * ======================================================================== */

int utb_ipc_snapshot_write(uint32_t shm_addr, const void *data,
                           uint32_t len, uint32_t timestamp)
{
    if (data == (void *)0 && len > 0) {
        return UTB_IPC_ERR_PARAM;
    }
    if (len > UTB_IPC_SNAP_DATA_SIZE) {
        return UTB_IPC_ERR_OVERFLOW;
    }

    volatile utb_ipc_snapshot_t *snap = (volatile utb_ipc_snapshot_t *)shm_addr;

    if (snap->magic != UTB_IPC_SNAP_MAGIC) {
        return UTB_IPC_ERR_NOT_READY;
    }

    /* Step 1: 标记写入中（generation → 奇数） */
    snap->generation++;
    UTB_FENCE();

    /* Step 2: 写入数据 */
    snap_memcpy_to_volatile(snap->data, data, len);

    /* 剩余部分清零（避免读者读到旧数据残留） */
    if (len < UTB_IPC_SNAP_DATA_SIZE) {
        snap_memset(snap->data + len, 0, UTB_IPC_SNAP_DATA_SIZE - len);
    }

    snap->timestamp = timestamp;

    UTB_FENCE();

    /* Step 3: 标记写入完成（generation → 偶数） */
    snap->generation++;
    UTB_FENCE();

    g_ipc_diag.snap_write++;
    return UTB_IPC_OK;
}

/* ========================================================================
 * 快照读取（主核调用）
 *
 * Seqlock 读序列：
 *   1. 读 gen1（若奇数则重试）
 *   2. fence
 *   3. 读数据
 *   4. fence
 *   5. 读 gen2
 *   6. 若 gen1 != gen2 则重试
 *
 * 有界重试：最多 16 次。若写者持续写入（极高频更新），
 * 返回 UTB_IPC_ERR_BUSY。
 * ======================================================================== */

#define SNAP_READ_MAX_RETRY  16

int utb_ipc_snapshot_read(uint32_t shm_addr, void *data, uint32_t max_len,
                          uint32_t *out_gen, uint32_t *out_ts)
{
    if (data == (void *)0 && max_len > 0) {
        return UTB_IPC_ERR_PARAM;
    }

    volatile utb_ipc_snapshot_t *snap = (volatile utb_ipc_snapshot_t *)shm_addr;

    if (snap->magic != UTB_IPC_SNAP_MAGIC) {
        return UTB_IPC_ERR_NOT_READY;
    }

    uint32_t copy_len = max_len;
    if (copy_len > UTB_IPC_SNAP_DATA_SIZE) {
        copy_len = UTB_IPC_SNAP_DATA_SIZE;
    }

    for (int retry = 0; retry < SNAP_READ_MAX_RETRY; retry++) {
        /* 读 generation（若奇数，写者正在更新，重试） */
        uint32_t gen1 = snap->generation;
        if (gen1 & 1U) {
            continue;
        }

        UTB_FENCE();

        /* 读数据 */
        snap_memcpy(data, snap->data, copy_len);
        uint32_t ts = snap->timestamp;

        UTB_FENCE();

        /* 读 generation 再次确认 */
        uint32_t gen2 = snap->generation;
        if (gen1 == gen2) {
            /* 一致读取成功 */
            if (out_gen != (void *)0) {
                *out_gen = gen1;
            }
            if (out_ts != (void *)0) {
                *out_ts = ts;
            }
            g_ipc_diag.snap_read++;
            return UTB_IPC_OK;
        }
        /* gen1 != gen2: 读取期间发生了写入，重试 */
    }

    g_ipc_diag.snap_read_busy++;
    return UTB_IPC_ERR_BUSY;
}

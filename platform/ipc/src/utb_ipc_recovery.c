/**
 * @file utb_ipc_recovery.c
 * @brief 心跳、对端重启检测、IPC 恢复
 *
 * 心跳机制：
 *   每核周期性写入 hb_tick 到控制块，对端读取并比较年龄。
 *
 * 重启检测：
 *   每核在 utb_ipc_init() 中递增 boot_gen。
 *   对端缓存 boot_gen，检测到变化即判定重启。
 *
 * 恢复流程：
 *   1. 应用层检测到 peer_restart → 2. 调用 utb_ipc_recover()
 *   → 3. 重新初始化本核 IPC → 4. 等待对端 ready
 */

#include "utb_ipc.h"

/* ========================================================================
 * 对端重启检测
 * ======================================================================== */

/* 缓存的对端 boot_gen（核私有） */
static uint32_t s_cached_peer_gen;
static int      s_peer_gen_valid;

int utb_ipc_check_peer_restart(void)
{
    volatile utb_ipc_ctl_t *ctl = utb_ipc_ctl();
    uint32_t core = utb_ipc_get_core_id();
    uint32_t peer_gen;

    if (core == UTB_CORE_PRIMARY) {
        peer_gen = ctl->secondary_boot_gen;
    } else {
        peer_gen = ctl->primary_boot_gen;
    }

    /* 首次调用：缓存当前值，不报重启 */
    if (!s_peer_gen_valid) {
        s_cached_peer_gen = peer_gen;
        s_peer_gen_valid = 1;
        return 0;
    }

    if (peer_gen != s_cached_peer_gen) {
        s_cached_peer_gen = peer_gen;
        g_ipc_diag.peer_restart++;
        return 1;
    }
    return 0;
}

/* ========================================================================
 * IPC 恢复
 * ======================================================================== */

int utb_ipc_recover(void)
{
    g_ipc_diag.recovery++;

    uint32_t core = utb_ipc_get_core_id();
    int is_primary = (core == UTB_CORE_PRIMARY) ? 1 : 0;

    /* 重置 peer_gen 缓存，下次 check 时重新采样 */
    s_peer_gen_valid = 0;

    return utb_ipc_init(is_primary);
}

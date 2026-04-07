/**
 * @file utb_ipc_stats.c
 * @brief IPC 诊断计数器管理
 *
 * 全局 g_ipc_diag 由各 IPC 模块直接递增。
 * 核私有（每核各一份 .bss），无跨核竞争。
 */

#include "utb_ipc.h"

/* 全局诊断计数器实例 */
utb_ipc_diag_t g_ipc_diag;

void utb_ipc_diag_reset(void)
{
    uint8_t *p = (uint8_t *)&g_ipc_diag;
    for (uint32_t i = 0; i < sizeof(utb_ipc_diag_t); i++) {
        p[i] = 0;
    }
}

const utb_ipc_diag_t *utb_ipc_diag_get(void)
{
    return &g_ipc_diag;
}

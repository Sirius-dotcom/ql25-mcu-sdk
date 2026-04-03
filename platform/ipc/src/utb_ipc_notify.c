/**
 * @file utb_ipc_notify.c
 * @brief IDU 核间中断门铃封装
 *
 * 封装 IDU 的 ICI（Inter-Core Interrupt）机制，提供：
 *   - 向对端核发送门铃通知
 *   - INTER_CORE_IRQn ISR 处理 + 回调
 *
 * 依赖 SDK IDU 驱动：ns_idu.h / ns_idu.c
 */

#include "utb_ipc.h"
#include "ns.h"
#include "ns_idu.h"

/* ========================================================================
 * 内部状态（每核独立，非共享）
 * ======================================================================== */

/*
 * ISR 回调指针。存在栈外是因为 ISR 需要访问。
 * 这是核私有变量，不在共享内存中，不存在跨核竞争。
 */
static void (*s_notify_cb)(void);

/* 本核 processor_id（mhartid bits[9:1]） */
static uint32_t s_local_core_id;

/* ========================================================================
 * 获取核 ID
 * ======================================================================== */

uint32_t utb_ipc_get_core_id(void)
{
    return s_local_core_id;
}

/* ========================================================================
 * INTER_CORE_IRQn ISR
 * ======================================================================== */

/*
 * IDU 核间中断处理。
 *
 * 职责：
 *   1. 读 ICI status（哪些核发了中断过来）
 *   2. 清除 pending bit
 *   3. 调用回调（回调中通常只做唤醒 IPC task）
 *
 * ISR 中不做 ring 解析，保持最短执行时间。
 */
void INTER_CORE_IRQHandler(void)
{
    uint32_t status = IDU_GetProcessxICI_Status(IDU_BASE, s_local_core_id);

    /* 清除所有 pending 的 ICI bit */
    if (status != 0) {
        IDU_ProcessxClearICI(IDU_BASE, s_local_core_id, status);
    }

    /* 调用上层回调 */
    if (s_notify_cb != (void *)0) {
        s_notify_cb();
    }
}

/* ========================================================================
 * 初始化
 * ======================================================================== */

void utb_ipc_notify_init(void (*notify_cb)(void))
{
    /* 读取本核 processor_id */
    uint32_t mhartid;
    __asm volatile("csrr %0, mhartid" : "=r"(mhartid));
    s_local_core_id = (mhartid >> 1) & 0x1FF;  /* bits[9:1] */

    s_notify_cb = notify_cb;

    /* 使能 IDU 时钟（幂等操作，两核都调用无害） */
#ifdef MISC_HAS_IDU_HAS_CLK
    idu_clk_en(ENABLE);
#endif

    /*
     * 注册 INTER_CORE_IRQn 中断。
     *
     * ECLIC 配置：
     *   - level: 2（中高优先级，不阻塞最高优先级的硬件中断）
     *   - priority: 0
     *   - trigger: 后沿触发（IDU ICI 特性）
     *
     * 注意：ECLIC_Register_IRQ 内部会调用 ECLIC_SetLevelIRQ / ECLIC_EnableIRQ。
     *       如果目标环境没有这个 API，需要手动配置 ECLIC。
     */
    ECLIC_Register_IRQ(INTER_CORE_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                       ECLIC_POSTIVE_EDGE_TRIGGER, 2, 0,
                       (void *)INTER_CORE_IRQHandler);
}

/* ========================================================================
 * 发送门铃
 * ======================================================================== */

void utb_ipc_kick_peer(void)
{
    uint32_t peer_id;

    if (s_local_core_id == UTB_CORE_PRIMARY) {
        peer_id = UTB_CORE_SECONDARY;
    } else {
        peer_id = UTB_CORE_PRIMARY;
    }

    UTB_FENCE();    /* 确保 ring 数据写入完成后再发中断 */
    IDU_SendICI(IDU_BASE, s_local_core_id, peer_id);
}

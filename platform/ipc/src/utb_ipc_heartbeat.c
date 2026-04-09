/**
 * @file utb_ipc_heartbeat.c
 * @brief IPC heartbeat helpers shared by both cores
 */

#include "utb_ipc.h"

void utb_ipc_heartbeat_tick(uint32_t current_tick)
{
    volatile utb_ipc_ctl_t *ctl = utb_ipc_ctl();
    uint32_t core = utb_ipc_get_core_id();

    if (core == UTB_CORE_PRIMARY) {
        ctl->primary_hb_tick = current_tick;
    } else {
        ctl->secondary_hb_tick = current_tick;
    }
    UTB_FENCE();

    g_ipc_diag.hb_tx++;
}

int utb_ipc_is_peer_alive(uint32_t current_tick, uint32_t max_age)
{
    volatile utb_ipc_ctl_t *ctl = utb_ipc_ctl();
    uint32_t core = utb_ipc_get_core_id();
    uint32_t peer_tick;
    uint32_t age;

    if (core == UTB_CORE_PRIMARY) {
        peer_tick = ctl->secondary_hb_tick;
    } else {
        peer_tick = ctl->primary_hb_tick;
    }

    if (peer_tick == 0u) {
        return 0;
    }

    age = current_tick - peer_tick;
    return (age <= max_age) ? 1 : 0;
}

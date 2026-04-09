#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "utb_ipc.h"

#include "slave_hb.h"

#define HB_PERIOD_MS                500u

static slave_hb_ctx_t s_hb_ctx;

void slave_hb_init_context(slave_hb_ctx_t *ctx,
                           volatile uint32_t *uptime_sec,
                           dp_forward_runtime_t *dp_runtime,
                           utb_ipc_status_payload_t *dp_status)
{
    memset(ctx, 0, sizeof(*ctx));
    ctx->uptime_sec = uptime_sec;
    ctx->dp_runtime = dp_runtime;
    ctx->dp_status = dp_status;
    s_hb_ctx = *ctx;
}

void slave_hb_task(void *arg)
{
    TickType_t last_wake;

    (void)arg;
    last_wake = xTaskGetTickCount();

    for (;;) {
        uint32_t tick;
        const utb_ipc_diag_t *diag;

        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(HB_PERIOD_MS));

        tick = xTaskGetTickCount();
        *(s_hb_ctx.uptime_sec) = tick / configTICK_RATE_HZ;

        utb_ipc_heartbeat_tick(tick);

        dp_forward_fill_status(s_hb_ctx.dp_runtime, *(s_hb_ctx.uptime_sec), s_hb_ctx.dp_status);
        utb_ipc_snapshot_write(UTB_IPC_STATE_SNAP_ADDR,
                               s_hb_ctx.dp_status,
                               (uint32_t)sizeof(*(s_hb_ctx.dp_status)),
                               tick);

        diag = utb_ipc_diag_get();
        utb_ipc_snapshot_write(UTB_IPC_STATS_SNAP_ADDR,
                               diag,
                               (uint32_t)sizeof(utb_ipc_diag_t),
                               tick);
    }
}

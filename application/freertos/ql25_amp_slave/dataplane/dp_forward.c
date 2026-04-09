#include <stdbool.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "utb_hal.h"

#include "dp_forward.h"
#include "dp_mac.h"
#include "dp_spi.h"

#define DP_RX_WAIT_MS               100u
#define DP_EVENT_TIMEOUT_THRESHOLD  32u
#define DP_FAULT_INIT               1u
#define DP_FAULT_SPI                2u

static void dp_forward_report_fault(const dp_forward_ctx_t *ctx,
                                    uint16_t fault_id,
                                    uint32_t info0)
{
    if ((ctx != NULL) && (ctx->fault_report != NULL)) {
        ctx->fault_report(fault_id, info0);
    }
}

static void dp_forward_wait_forever(void)
{
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void dp_forward_reset_runtime(dp_forward_runtime_t *runtime)
{
    memset(runtime, 0, sizeof(*runtime));
    runtime->dp_state = UTB_DP_STATE_READY;
}

void dp_forward_fill_status(const dp_forward_runtime_t *runtime,
                            uint32_t uptime_sec,
                            utb_ipc_status_payload_t *status)
{
    status->dp_state = runtime->dp_state;
    status->link_state = runtime->link_state;
    status->uptime_sec = uptime_sec;
    status->rx_packets = runtime->rx_packets;
    status->tx_packets = runtime->tx_packets;
    status->rx_bytes = runtime->rx_bytes;
    status->tx_bytes = runtime->tx_bytes;
    status->rx_errors = runtime->rx_errors;
    status->tx_errors = runtime->tx_errors;
    status->rx_drops = runtime->rx_drops;
    status->tx_drops = runtime->tx_drops;
}

void dp_forward_task(void *arg)
{
    dp_forward_ctx_t *ctx;
    dp_forward_runtime_t *runtime;
    uint32_t timeout_accum;
    int ret;

    ctx = (dp_forward_ctx_t *)arg;
    if ((ctx == NULL) || (ctx->runtime == NULL) || (ctx->reset_req == NULL) ||
        (ctx->rx_buf == NULL) || (ctx->rx_buf_len == 0u)) {
        dp_forward_wait_forever();
    }
    runtime = ctx->runtime;
    timeout_accum = 0u;

    dp_forward_reset_runtime(runtime);
    dp_spi_iomux_config();

    ret = dp_mac_init();
    if (ret != UTB_OK) {
        runtime->dp_state = UTB_DP_STATE_ERROR;
        runtime->rx_errors++;
        dp_forward_report_fault(ctx, DP_FAULT_INIT, (uint32_t)(uint16_t)(-ret));
        dp_forward_wait_forever();
    }

    ret = dp_spi_init();
    if (ret != UTB_OK) {
        runtime->dp_state = UTB_DP_STATE_ERROR;
        runtime->tx_errors++;
        dp_forward_report_fault(ctx, DP_FAULT_INIT, (uint32_t)(uint16_t)(-ret));
        dp_forward_wait_forever();
    }

    dp_mac_reset_state();
    runtime->dp_state = UTB_DP_STATE_RUNNING;

    for (;;) {
        if (*(ctx->reset_req) != 0u) {
            *(ctx->reset_req) = 0u;
            dp_forward_reset_runtime(runtime);
            dp_mac_reset_state();
            runtime->dp_state = UTB_DP_STATE_RUNNING;
            timeout_accum = 0u;
        }

        {
            bool int_ready;
            bool fifo_ready;

            ret = dp_mac_wait_rx_ready(DP_RX_WAIT_MS, &int_ready, &fifo_ready);
            if (int_ready) {
                runtime->rx_ready_int_hits++;
            }
            if (fifo_ready) {
                runtime->rx_ready_fifo_hits++;
            }
        }

        if (ret != UTB_OK) {
            timeout_accum++;
            runtime->rx_timeouts++;
            if ((timeout_accum % DP_EVENT_TIMEOUT_THRESHOLD) == 0u) {
                utb_hal_mac_stats_t mac_stats;

                memset(&mac_stats, 0, sizeof(mac_stats));
                dp_mac_get_stats(&mac_stats);
                if ((mac_stats.rx_timeout_wrong_num != 0u) ||
                    (mac_stats.phy_len_wrong_num != 0u) ||
                    (mac_stats.rx_fifo_overflow_symb_num != 0u) ||
                    (mac_stats.ahb_rx_timeout_num != 0u)) {
                    runtime->rx_errors++;
                    dp_forward_report_fault(ctx, DP_FAULT_SPI, mac_stats.rx_timeout_wrong_num);
                }
            }
            continue;
        }

        timeout_accum = 0u;
        dp_mac_read_slice(ctx->rx_buf, ctx->rx_buf_len);
        dp_mac_ack_rx_slice();

        runtime->link_state = 1u;
        runtime->rx_packets++;
        runtime->rx_bytes += ctx->rx_buf_len;

        ret = dp_spi_transfer(ctx->rx_buf, ctx->rx_buf_len);
        if (ret != UTB_OK) {
            runtime->tx_errors++;
            runtime->tx_drops++;
            dp_forward_report_fault(ctx, DP_FAULT_SPI, (uint32_t)(uint16_t)(-ret));
            continue;
        }

        runtime->tx_packets++;
        runtime->tx_bytes += ctx->rx_buf_len;
    }
}

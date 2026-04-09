#ifndef DP_FORWARD_H
#define DP_FORWARD_H

#include <stdint.h>

#include "utb_ipc_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    volatile uint8_t dp_state;
    volatile uint8_t link_state;
    volatile uint32_t rx_packets;
    volatile uint32_t tx_packets;
    volatile uint32_t rx_bytes;
    volatile uint32_t tx_bytes;
    volatile uint32_t rx_errors;
    volatile uint32_t tx_errors;
    volatile uint32_t rx_drops;
    volatile uint32_t tx_drops;
    volatile uint32_t rx_timeouts;
    volatile uint32_t rx_ready_int_hits;
    volatile uint32_t rx_ready_fifo_hits;
    volatile uint32_t fault_events;
} dp_forward_runtime_t;

typedef void (*dp_forward_fault_report_fn_t)(uint16_t fault_id, uint32_t info0);

typedef struct {
    volatile uint32_t *reset_req;
    dp_forward_runtime_t *runtime;
    uint8_t *rx_buf;
    uint32_t rx_buf_len;
    dp_forward_fault_report_fn_t fault_report;
} dp_forward_ctx_t;

void dp_forward_reset_runtime(dp_forward_runtime_t *runtime);
void dp_forward_fill_status(const dp_forward_runtime_t *runtime,
                            uint32_t uptime_sec,
                            utb_ipc_status_payload_t *status);
void dp_forward_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif /* DP_FORWARD_H */

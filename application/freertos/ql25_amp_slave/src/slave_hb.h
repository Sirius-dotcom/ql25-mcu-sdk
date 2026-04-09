#ifndef SLAVE_HB_H
#define SLAVE_HB_H

#include <stdint.h>

#include "dp_forward.h"
#include "utb_ipc_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    volatile uint32_t *uptime_sec;
    dp_forward_runtime_t *dp_runtime;
    utb_ipc_status_payload_t *dp_status;
} slave_hb_ctx_t;

void slave_hb_init_context(slave_hb_ctx_t *ctx,
                           volatile uint32_t *uptime_sec,
                           dp_forward_runtime_t *dp_runtime,
                           utb_ipc_status_payload_t *dp_status);
void slave_hb_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif /* SLAVE_HB_H */

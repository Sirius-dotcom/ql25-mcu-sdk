#ifndef SLAVE_IPC_H
#define SLAVE_IPC_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include "dp_forward.h"
#include "utb_ipc_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    TaskHandle_t ipc_task_handle;
    volatile uint32_t *uptime_sec;
    volatile uint32_t *dp_reset_req;
    dp_forward_runtime_t *dp_runtime;
    utb_ipc_status_payload_t *dp_status;
    const utb_ipc_version_payload_t *version;
} slave_ipc_ctx_t;

void slave_ipc_init_context(slave_ipc_ctx_t *ctx,
                            volatile uint32_t *uptime_sec,
                            volatile uint32_t *dp_reset_req,
                            dp_forward_runtime_t *dp_runtime,
                            utb_ipc_status_payload_t *dp_status,
                            const utb_ipc_version_payload_t *version);
void slave_ipc_set_task_handle(slave_ipc_ctx_t *ctx, TaskHandle_t task_handle);
void slave_ipc_doorbell_cb(void);
void slave_ipc_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif /* SLAVE_IPC_H */

#include <string.h>

#include "utb_ipc.h"
#include "utb_ipc_msg.h"

#include "slave_ipc.h"

#define IPC_POLL_TIMEOUT_MS         50u

static slave_ipc_ctx_t s_ipc_ctx;

static void slave_ipc_refresh_status(void)
{
    dp_forward_fill_status(s_ipc_ctx.dp_runtime, *(s_ipc_ctx.uptime_sec), s_ipc_ctx.dp_status);
}

static void slave_handle_command(const utb_ipc_msg_hdr_t *hdr,
                                 const void *payload, uint16_t len)
{
    int32_t result;

    (void)payload;
    (void)len;
    result = UTB_IPC_OK;

    switch (hdr->cmd_id) {
    case UTB_CMD_NOP:
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK, (void *)0, 0);
        break;

    case UTB_CMD_HEARTBEAT: {
        utb_ipc_heartbeat_payload_t rsp;

        rsp.sender_tick = xTaskGetTickCount();
        rsp.uptime_sec = *(s_ipc_ctx.uptime_sec);
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK, &rsp, (uint16_t)sizeof(rsp));
        break;
    }

    case UTB_CMD_GET_STATUS:
        slave_ipc_refresh_status();
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK,
                             s_ipc_ctx.dp_status, (uint16_t)sizeof(*(s_ipc_ctx.dp_status)));
        break;

    case UTB_CMD_GET_VERSION:
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK,
                             (void *)s_ipc_ctx.version, (uint16_t)sizeof(*(s_ipc_ctx.version)));
        break;

    case UTB_CMD_RESET_DP:
        *(s_ipc_ctx.dp_reset_req) = 1u;
        utb_ipc_ctrl_respond(hdr->txn_id, UTB_IPC_OK, (void *)0, 0);
        break;

    case UTB_CMD_SET_CONFIG:
    case UTB_CMD_GET_CONFIG:
    case UTB_CMD_DEL_CONFIG:
        result = UTB_IPC_ERR_PARAM;
        utb_ipc_ctrl_respond(hdr->txn_id, result, (void *)0, 0);
        break;

    default:
        result = UTB_IPC_ERR_INVALID;
        utb_ipc_ctrl_respond(hdr->txn_id, result, (void *)0, 0);
        break;
    }
}

void slave_ipc_init_context(slave_ipc_ctx_t *ctx,
                            volatile uint32_t *uptime_sec,
                            volatile uint32_t *dp_reset_req,
                            dp_forward_runtime_t *dp_runtime,
                            utb_ipc_status_payload_t *dp_status,
                            const utb_ipc_version_payload_t *version)
{
    memset(ctx, 0, sizeof(*ctx));
    ctx->uptime_sec = uptime_sec;
    ctx->dp_reset_req = dp_reset_req;
    ctx->dp_runtime = dp_runtime;
    ctx->dp_status = dp_status;
    ctx->version = version;
}

void slave_ipc_set_task_handle(slave_ipc_ctx_t *ctx, TaskHandle_t task_handle)
{
    ctx->ipc_task_handle = task_handle;
    s_ipc_ctx = *ctx;
}

void slave_ipc_doorbell_cb(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (s_ipc_ctx.ipc_task_handle == NULL) {
        return;
    }
    vTaskNotifyGiveFromISR(s_ipc_ctx.ipc_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void slave_ipc_task(void *arg)
{
    uint32_t ctrl_req_addr;

    (void)arg;
    ctrl_req_addr = UTB_IPC_SHM_BASE + UTB_IPC_CTRL_REQ_OFS;
    utb_ipc_notify_init(slave_ipc_doorbell_cb);

    for (;;) {
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(IPC_POLL_TIMEOUT_MS));

        for (;;) {
            utb_ipc_msg_hdr_t hdr;
            uint8_t payload[UTB_IPC_CTRL_PAYLOAD_MAX];
            int ret;

            ret = utb_ipc_recv(ctrl_req_addr, &hdr, payload, (uint16_t)sizeof(payload));
            if (ret == UTB_IPC_ERR_EMPTY) {
                break;
            }
            if (ret != UTB_IPC_OK) {
                continue;
            }

            slave_handle_command(&hdr, payload, hdr.payload_len);
        }
    }
}

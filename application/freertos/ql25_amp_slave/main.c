#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "utb_ipc.h"
#include "utb_ipc_msg.h"
#include "dp_forward.h"
#include "dp_mac.h"
#include "slave_hb.h"
#include "slave_ipc.h"

#define IPC_TASK_STACK_WORDS        192u
#define HB_TASK_STACK_WORDS         160u
#define DP_TASK_STACK_WORDS         256u

#define IPC_TASK_PRIO               4u
#define HB_TASK_PRIO                3u
#define DP_TASK_PRIO                2u

static StaticTask_t s_ipc_tcb;
static StackType_t  s_ipc_stack[IPC_TASK_STACK_WORDS];

static StaticTask_t s_hb_tcb;
static StackType_t  s_hb_stack[HB_TASK_STACK_WORDS];

static StaticTask_t s_dp_tcb;
static StackType_t  s_dp_stack[DP_TASK_STACK_WORDS];

static volatile uint32_t s_uptime_sec;
static volatile uint32_t s_dp_reset_req;
static dp_forward_runtime_t s_dp_runtime;
static utb_ipc_status_payload_t s_ipc_status;
static utb_ipc_status_payload_t s_hb_status;
static uint8_t s_dp_rx_buf[DP_MAC_SLICE_LEN];
static dp_forward_ctx_t s_dp_ctx;
static slave_ipc_ctx_t s_ipc_ctx;
static slave_hb_ctx_t s_hb_ctx;

static const utb_ipc_version_payload_t s_version = {
    .major      = 0,
    .minor      = 1,
    .patch      = 0,
    .build_id   = 0,
    .build_date = __DATE__,
};

static void ql25_dp_report_fault(uint16_t fault_id, uint32_t info0)
{
    utb_ipc_fault_payload_t fault;

    memset(&fault, 0, sizeof(fault));
    fault.fault_id = fault_id;
    fault.fatal = 0u;
    fault.module_id = 1u;
    fault.info0 = info0;
    fault.timestamp_ms = xTaskGetTickCount();
    if (utb_ipc_event_send(UTB_IPC_EVT_FAULT, &fault, (uint16_t)sizeof(fault)) == UTB_IPC_OK) {
        s_dp_runtime.fault_events++;
    }
}

int main(void)
{
    int ret;
    TaskHandle_t ipc_task_handle;
    TaskHandle_t hb_task_handle;
    TaskHandle_t dp_task_handle;

    memset(&s_ipc_status, 0, sizeof(s_ipc_status));
    memset(&s_hb_status, 0, sizeof(s_hb_status));
    memset(&s_dp_runtime, 0, sizeof(s_dp_runtime));
    memset(s_dp_rx_buf, 0, sizeof(s_dp_rx_buf));
    memset(&s_dp_ctx, 0, sizeof(s_dp_ctx));

    ret = utb_ipc_init(0);
    if (ret != UTB_IPC_OK) {
        for (;;) {
        }
    }

    s_dp_runtime.dp_state = UTB_DP_STATE_INIT;
    s_dp_ctx.reset_req = &s_dp_reset_req;
    s_dp_ctx.runtime = &s_dp_runtime;
    s_dp_ctx.rx_buf = s_dp_rx_buf;
    s_dp_ctx.rx_buf_len = (uint32_t)sizeof(s_dp_rx_buf);
    s_dp_ctx.fault_report = ql25_dp_report_fault;
    slave_ipc_init_context(&s_ipc_ctx, &s_uptime_sec, &s_dp_reset_req,
                           &s_dp_runtime, &s_ipc_status, &s_version);
    slave_hb_init_context(&s_hb_ctx, &s_uptime_sec, &s_dp_runtime, &s_hb_status);

    ipc_task_handle = xTaskCreateStatic(
        slave_ipc_task, "ipc",
        IPC_TASK_STACK_WORDS, (void *)0, IPC_TASK_PRIO,
        s_ipc_stack, &s_ipc_tcb);
    if (ipc_task_handle == NULL) {
        for (;;) {
        }
    }
    slave_ipc_set_task_handle(&s_ipc_ctx, ipc_task_handle);

    hb_task_handle = xTaskCreateStatic(
        slave_hb_task, "hb",
        HB_TASK_STACK_WORDS, &s_hb_ctx, HB_TASK_PRIO,
        s_hb_stack, &s_hb_tcb);
    if (hb_task_handle == NULL) {
        for (;;) {
        }
    }

    dp_task_handle = xTaskCreateStatic(
        dp_forward_task, "dp",
        DP_TASK_STACK_WORDS, &s_dp_ctx, DP_TASK_PRIO,
        s_dp_stack, &s_dp_tcb);
    if (dp_task_handle == NULL) {
        for (;;) {
        }
    }

    vTaskStartScheduler();

    for (;;) {
    }
}

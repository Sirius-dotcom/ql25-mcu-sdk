
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <log.h>
#include "ql25_pub.h"
#include "uart_iface.h"

int ql25_send_msg_to_adp(utb_ql25_ctrl_t *ql25_ctrl, const uint8_t* data, int len)
{
    uart_send_msg(data, len);
    return 0;
}

void* ql25_mng_rx_thread(void *arg)
{
    utb_hal_handle_t *hdle = (utb_hal_handle_t*)arg;
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t *)hdle->private;

    if (!ql25_ctrl->func_mng_callback) {
        log_fatal("uart_task param is null");
        return NULL;
    }

    while (!ql25_ctrl->pre_destroy) {
        uint8_t *msg = UTB_MALLOC(256);
        if (!msg) {
            log_fatal("uart_task malloc failed");
            osal_delay_ms(1000);
            continue;
        }
        memset(msg, 0, 256);
        uint32_t len = 0;
        if (uart_rcv_msg(msg, &len) != 0) {
            UTB_FREE(msg);
            continue;
        }
        ql25_ctrl->func_mng_callback(0, msg, len); /* msg在回调函数处理完毕后释放 */
    }
    return NULL;
}

utb_thread_handle_t *ql25_if_init_thread(utb_ql25_ctrl_t *ql25_ctrl)
{
    utb_os_thread_init_t init_cfg = {
        .thread_name = "mng_rcv",
        .sched_priority = 2,
        .stak_depth = 1024,
        .arg = ql25_ctrl,
        .func_thread_call_back = ql25_mng_rx_thread,
    };

    return utb_os_thread_create(&init_cfg);
}

void ql25_if_deinit_thread(utb_ql25_ctrl_t *ql25_ctrl)
{
    ql25_ctrl->pre_destroy = 1;
    utb_os_thread_destroy(ql25_ctrl->rx_mng_thread_hd);
}


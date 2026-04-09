#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "dp_mac.h"

#define DP_MAC_TIMER                0x8000u
#define DP_AHB_TIMER                0xC350u
#define DP_IDLE_SLEEP_MS            1u

int dp_mac_init(void)
{
    utb_hal_mac_cfg_t mac_cfg;

    mac_cfg.fixed_length = DP_MAC_SLICE_LEN;
    mac_cfg.mac_timer = DP_MAC_TIMER;
    mac_cfg.ahb_symb_timer = DP_AHB_TIMER;
    mac_cfg.mac_en = 1u;

    return utb_hal_mac_init(&mac_cfg);
}

void dp_mac_reset_state(void)
{
    utb_hal_mac_clear_ints(0xFFFFu);
    utb_hal_mac_clear_stats();
}

int dp_mac_wait_rx_ready(uint32_t timeout_ms, bool *int_ready, bool *fifo_ready)
{
    uint32_t waited_ms;

    if (int_ready != NULL) {
        *int_ready = false;
    }
    if (fifo_ready != NULL) {
        *fifo_ready = false;
    }

    for (waited_ms = 0; waited_ms < timeout_ms; waited_ms++) {
        utb_hal_mac_fifo_slice_state_t fifo_state;
        uint32_t int_status;

        int_status = utb_hal_mac_get_int_status();
        utb_hal_mac_get_rx_fifo_slice_state(&fifo_state);

        if ((int_status & (1u << UTB_HAL_MAC_INT_RX_SLICE_READABLE)) != 0u) {
            if (int_ready != NULL) {
                *int_ready = true;
            }
            return UTB_OK;
        }

        if (fifo_state.committed_slices != 0u) {
            if (fifo_ready != NULL) {
                *fifo_ready = true;
            }
            return UTB_OK;
        }

        vTaskDelay(pdMS_TO_TICKS(DP_IDLE_SLEEP_MS));
    }

    return UTB_ERR_TIMEOUT;
}

void dp_mac_read_slice(uint8_t *buf, uint32_t len)
{
    uint32_t i;

    for (i = 0; i < len; i++) {
        buf[i] = utb_hal_mac_read_symb8();
    }
}

void dp_mac_ack_rx_slice(void)
{
    utb_hal_mac_clear_ints(1u << UTB_HAL_MAC_INT_RX_SLICE_READABLE);
}

void dp_mac_get_stats(utb_hal_mac_stats_t *stats)
{
    utb_hal_mac_get_stats(stats);
}

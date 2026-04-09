#ifndef DP_MAC_H
#define DP_MAC_H

#include <stdbool.h>
#include <stdint.h>

#include "utb_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DP_MAC_SLICE_LEN            472u

int dp_mac_init(void);
void dp_mac_reset_state(void);
int dp_mac_wait_rx_ready(uint32_t timeout_ms, bool *int_ready, bool *fifo_ready);
void dp_mac_read_slice(uint8_t *buf, uint32_t len);
void dp_mac_ack_rx_slice(void);
void dp_mac_get_stats(utb_hal_mac_stats_t *stats);

#ifdef __cplusplus
}
#endif

#endif /* DP_MAC_H */

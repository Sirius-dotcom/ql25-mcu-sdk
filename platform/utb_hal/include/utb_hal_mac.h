#ifndef UTB_HAL_MAC_H
#define UTB_HAL_MAC_H

#include <stdint.h>

#include "utb_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UTB_HAL_MAC_INT_TX_SLICE_WRITABLE = 0,
    UTB_HAL_MAC_INT_RX_SLICE_READABLE = 1,
    UTB_HAL_MAC_INT_RESERVED_2 = 2,
    UTB_HAL_MAC_INT_PHY_LEN_WRONG = 3,
    UTB_HAL_MAC_INT_TX_FIFO_FULL = 4,
    UTB_HAL_MAC_INT_PHY_REQ_LEN_WRONG = 5,
    UTB_HAL_MAC_INT_RX_FIFO_FULL = 6,
    UTB_HAL_MAC_INT_TX_TIMEOUT = 7,
    UTB_HAL_MAC_INT_RX_TIMEOUT = 8,
    UTB_HAL_MAC_INT_PHY_RX_TIMEOUT = 9,
    UTB_HAL_MAC_INT_AHB_TX_TIMEOUT = 10,
    UTB_HAL_MAC_INT_AHB_RX_TIMEOUT = 11,
} utb_hal_mac_int_t;

typedef struct {
    uint16_t fixed_length;
    uint16_t mac_timer;
    uint16_t ahb_symb_timer;
    uint8_t mac_en;
} utb_hal_mac_cfg_t;

typedef struct {
    uint32_t int_status;
    uint32_t phy_len_wrong_num;
    uint32_t tx_full_write_wrong_num;
    uint32_t rx_full_write_wrong_num;
    uint32_t rx_timeout_wrong_num;
    uint32_t mcu_len_right_num;
    uint32_t total_slice_num_2phy;
    uint32_t valid_slice_num_2phy;
    uint32_t tx_timeout_wrong_num;
    uint32_t phy_req_len_wrong_num;
    uint32_t rx_fifo_overflow_symb_num;
    uint32_t ahb_rx_timeout_num;
} utb_hal_mac_stats_t;

typedef struct {
    uint16_t available_slices;
    uint16_t committed_slices;
} utb_hal_mac_fifo_slice_state_t;

typedef struct {
    uint16_t unwritten_bytes;
    uint16_t written_bytes;
} utb_hal_mac_fifo_byte_state_t;

typedef struct {
    uint16_t ahb_byte_cnt;
    uint16_t mac_byte_cnt;
    uint8_t state;
} utb_hal_mac_path_state_t;

int utb_hal_mac_init(const utb_hal_mac_cfg_t *cfg);
int utb_hal_mac_enable(uint8_t enable);
int utb_hal_mac_set_fixed_length(uint16_t fixed_length, uint16_t mac_timer);
int utb_hal_mac_set_ahb_symb_timer(uint16_t ahb_symb_timer);

uint32_t utb_hal_mac_get_int_status(void);
void utb_hal_mac_clear_ints(uint32_t int_mask);
void utb_hal_mac_mask_ints(uint32_t int_mask);
void utb_hal_mac_unmask_all_ints(void);

void utb_hal_mac_clear_stats(void);
void utb_hal_mac_get_stats(utb_hal_mac_stats_t *stats);
void utb_hal_mac_get_tx_fifo_slice_state(utb_hal_mac_fifo_slice_state_t *state);
void utb_hal_mac_get_rx_fifo_slice_state(utb_hal_mac_fifo_slice_state_t *state);
void utb_hal_mac_get_tx_fifo_byte_state(utb_hal_mac_fifo_byte_state_t *state);
void utb_hal_mac_get_rx_fifo_byte_state(utb_hal_mac_fifo_byte_state_t *state);
void utb_hal_mac_get_tx_path_state(utb_hal_mac_path_state_t *state);
void utb_hal_mac_get_rx_path_state(utb_hal_mac_path_state_t *state);

void utb_hal_mac_write_symb8(uint8_t value);
uint8_t utb_hal_mac_read_symb8(void);
void utb_hal_mac_write_symb32(uint32_t value);
uint32_t utb_hal_mac_read_symb32(void);

#ifdef __cplusplus
}
#endif

#endif /* UTB_HAL_MAC_H */

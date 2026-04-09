#include "utb_hal_mac.h"

#include "ql25_mac_reg.h"

#define UTB_HAL_MAC_INT_MASK_ALL              0x0000FFFFu

static volatile ql25_mac_reg_t *utb_hal_mac_regs(void)
{
    return (volatile ql25_mac_reg_t *)QL25_MAC_BASE_ADDR;
}

static uint32_t utb_hal_mac_read_ctrl(void)
{
    return utb_hal_mac_regs()->mac_ctrl;
}

static void utb_hal_mac_write_ctrl(uint32_t value)
{
    utb_hal_mac_regs()->mac_ctrl = value;
}

int utb_hal_mac_enable(uint8_t enable)
{
    uint32_t reg_value;

    reg_value = utb_hal_mac_read_ctrl();
    if (enable != 0u) {
        reg_value |= (1u << 16);
    } else {
        reg_value &= ~(1u << 16);
    }
    utb_hal_mac_write_ctrl(reg_value);

    return UTB_OK;
}

int utb_hal_mac_set_ahb_symb_timer(uint16_t ahb_symb_timer)
{
    uint32_t reg_value;

    reg_value = utb_hal_mac_read_ctrl();
    reg_value &= ~0xFFFFu;
    reg_value |= (uint32_t)ahb_symb_timer;
    utb_hal_mac_write_ctrl(reg_value);

    return UTB_OK;
}

int utb_hal_mac_set_fixed_length(uint16_t fixed_length, uint16_t mac_timer)
{
    uint32_t reg_value;

    reg_value = ((uint32_t)fixed_length << 16) | (uint32_t)mac_timer;
    utb_hal_mac_regs()->mac_fixed_length = reg_value;

    return UTB_OK;
}

int utb_hal_mac_init(const utb_hal_mac_cfg_t *cfg)
{
    uint32_t int_mask;

    if (cfg == NULL) {
        return UTB_ERR_PARAM;
    }

    int_mask = UTB_HAL_MAC_INT_MASK_ALL;
    utb_hal_mac_regs()->mac_int_clr_mask = int_mask;
    utb_hal_mac_regs()->mac_num_clr = 0xFFFFFFFFu;
    utb_hal_mac_regs()->mac_int_clr_mask = 0u;
    utb_hal_mac_regs()->mac_num_clr = 0u;

    utb_hal_mac_set_fixed_length(cfg->fixed_length, cfg->mac_timer);
    utb_hal_mac_set_ahb_symb_timer(cfg->ahb_symb_timer);
    utb_hal_mac_enable(cfg->mac_en);

    return UTB_OK;
}

uint32_t utb_hal_mac_get_int_status(void)
{
    return utb_hal_mac_regs()->mac_int_status & UTB_HAL_MAC_INT_MASK_ALL;
}

void utb_hal_mac_clear_ints(uint32_t int_mask)
{
    utb_hal_mac_regs()->mac_int_clr_mask = (int_mask & UTB_HAL_MAC_INT_MASK_ALL);
}

void utb_hal_mac_mask_ints(uint32_t int_mask)
{
    uint32_t mask_value;

    mask_value = (int_mask & UTB_HAL_MAC_INT_MASK_ALL) << 16;
    utb_hal_mac_regs()->mac_int_clr_mask = mask_value;
}

void utb_hal_mac_unmask_all_ints(void)
{
    utb_hal_mac_regs()->mac_int_clr_mask = 0u;
}

void utb_hal_mac_clear_stats(void)
{
    utb_hal_mac_regs()->mac_num_clr = 0xFFFFFFFFu;
    utb_hal_mac_regs()->mac_num_clr = 0u;
}

void utb_hal_mac_get_stats(utb_hal_mac_stats_t *stats)
{
    volatile ql25_mac_reg_t *regs;

    if (stats == NULL) {
        return;
    }

    regs = utb_hal_mac_regs();
    stats->int_status = regs->mac_int_status;
    stats->phy_len_wrong_num = regs->phy_len_wrong_num;
    stats->tx_full_write_wrong_num = regs->tx_full_write_wrong_num;
    stats->rx_full_write_wrong_num = regs->rx_full_write_wrong_num;
    stats->rx_timeout_wrong_num = regs->rx_timeout_wrong_num;
    stats->mcu_len_right_num = regs->mcu_len_right_num;
    stats->total_slice_num_2phy = regs->total_slice_num_2phy;
    stats->valid_slice_num_2phy = regs->valid_slice_num_2phy;
    stats->tx_timeout_wrong_num = regs->tx_timeout_wrong_num;
    stats->phy_req_len_wrong_num = regs->phy_req_len_wrong_num;
    stats->rx_fifo_overflow_symb_num = regs->rx_fifo_overflow_symb_num;
    stats->ahb_rx_timeout_num = regs->ahb_rx_timeout_num;
}

void utb_hal_mac_get_tx_fifo_slice_state(utb_hal_mac_fifo_slice_state_t *state)
{
    uint32_t reg_value;

    if (state == NULL) {
        return;
    }

    reg_value = utb_hal_mac_regs()->tx_fifo_state;
    state->available_slices = (uint16_t)(reg_value >> 16);
    state->committed_slices = (uint16_t)(reg_value & 0xFFFFu);
}

void utb_hal_mac_get_rx_fifo_slice_state(utb_hal_mac_fifo_slice_state_t *state)
{
    uint32_t reg_value;

    if (state == NULL) {
        return;
    }

    reg_value = utb_hal_mac_regs()->rx_fifo_state;
    state->available_slices = (uint16_t)(reg_value >> 16);
    state->committed_slices = (uint16_t)(reg_value & 0xFFFFu);
}

void utb_hal_mac_get_tx_fifo_byte_state(utb_hal_mac_fifo_byte_state_t *state)
{
    uint32_t reg_value;

    if (state == NULL) {
        return;
    }

    reg_value = utb_hal_mac_regs()->tx_fifo_state1;
    state->unwritten_bytes = (uint16_t)((reg_value >> 16) & 0x1FFFu);
    state->written_bytes = (uint16_t)(reg_value & 0xFFFFu);
}

void utb_hal_mac_get_rx_fifo_byte_state(utb_hal_mac_fifo_byte_state_t *state)
{
    uint32_t reg_value;

    if (state == NULL) {
        return;
    }

    reg_value = utb_hal_mac_regs()->rx_fifo_state1;
    state->unwritten_bytes = (uint16_t)(reg_value >> 16);
    state->written_bytes = (uint16_t)(reg_value & 0x1FFFu);
}

void utb_hal_mac_get_tx_path_state(utb_hal_mac_path_state_t *state)
{
    uint32_t reg_value;

    if (state == NULL) {
        return;
    }

    reg_value = utb_hal_mac_regs()->mac_tx_status;
    state->ahb_byte_cnt = (uint16_t)((reg_value >> 12) & 0x03FFu);
    state->mac_byte_cnt = (uint16_t)((reg_value >> 2) & 0x03FFu);
    state->state = (uint8_t)(reg_value & 0x3u);
}

void utb_hal_mac_get_rx_path_state(utb_hal_mac_path_state_t *state)
{
    uint32_t reg_value;

    if (state == NULL) {
        return;
    }

    reg_value = utb_hal_mac_regs()->mac_rx_status;
    state->ahb_byte_cnt = (uint16_t)((reg_value >> 12) & 0x03FFu);
    state->mac_byte_cnt = (uint16_t)((reg_value >> 2) & 0x03FFu);
    state->state = (uint8_t)(reg_value & 0x3u);
}

void utb_hal_mac_write_symb8(uint8_t value)
{
    volatile uint8_t *port;

    port = (volatile uint8_t *)&utb_hal_mac_regs()->mac_tx_symb;
    *port = value;
}

uint8_t utb_hal_mac_read_symb8(void)
{
    volatile uint8_t *port;

    port = (volatile uint8_t *)&utb_hal_mac_regs()->mac_rx_symb;
    return *port;
}

void utb_hal_mac_write_symb32(uint32_t value)
{
    utb_hal_mac_regs()->mac_tx_symb = value;
}

uint32_t utb_hal_mac_read_symb32(void)
{
    return utb_hal_mac_regs()->mac_rx_symb;
}


#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ql25_pub.h"
#include "ql25_mac_drv.h"
#include "ql25_phy_drv.h"
#include "ql25_sys_drv.h"
#include "ql25_dbg.h"

static int utb_hal_disp_reg(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len, uint32_t addr, uint32_t cnt, uint32_t filt)
{
    int use_len = 0;
    int line = cnt >> 2;
    uint32_t line_cnt = 4;
    for (int i = 0; i <= line; i++)
    {
        uint32_t base_addr = addr + i * 0x10;
        uint32_t value[4] = { 0 };
        uint32_t had_valid = 0;
        if (i == line) {
            line_cnt = cnt % 4;
            if (line_cnt == 0) {
                break;
            }
        }
        for (uint32_t j = 0; j < line_cnt; j++) {
            if (!ql25_check_reg_addr_is_valid(base_addr + j * 4)) {
                break;
            }

            value[j] = *(uint32_t*)(ql25_ctrl->phy_reg_stub + base_addr + j * 4);
#if 0    
            value[j] = (*((volatile uint32_t *)(base_addr + j * 4)));
#endif
            if (value[j] != 0) {
                had_valid = 1;
            }
        }

        if (filt && !had_valid) {
            continue;
        }
        
        use_len += snprintf(dbg_str + use_len, len - use_len, "\r\n0x%04x:", base_addr);
        for (uint32_t j = 0; j < line_cnt; j++) {
            use_len += snprintf(dbg_str + use_len, len - use_len, " %08lX", value[j]);
            UTB_CHECK_LEN(len, use_len);
        }
    }
    return use_len;
}

int utb_hal_dbg_read_reg(utb_hal_handle_t *hdle, char *dbg_str, int len, uint32_t addr, uint32_t cnt)
{
    int use_len = 0;
    uint32_t line = cnt >> 2;
    uint32_t value = 0;
    uint32_t max_addr = addr + cnt * 4;
    addr = addr & 0xFFFFfffc;

    UTB_DBG_CHECK_PARAMS_AND_RETURN(hdle, dbg_str, len);

    use_len += snprintf(dbg_str + use_len, len - use_len, "read register, 0x%08x, %d\r\n", addr, cnt);
    UTB_CHECK_LEN(len, use_len);

    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t *)hdle->private;
    use_len += utb_hal_disp_reg(ql25_ctrl, dbg_str + use_len, len - use_len, addr, cnt, 0);
    UTB_CHECK_LEN(len, use_len);

    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\n");
    return use_len;
}

int utb_hal_dbg_write_reg(utb_hal_handle_t *hdle, char *dbg_str, int len, uint32_t addr, uint32_t value)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(hdle, dbg_str, len);
    use_len += snprintf(dbg_str + use_len, len - use_len, "write reg : %08lx << %08lx\n", addr, value);
    (void)utb_hal_write_reg(hdle, addr, value);
    return use_len;
}

int utb_hal_dbg_test_spi_tx(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);
    return use_len;
}

int utb_hal_dbg_test_spi_rx(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);
    return use_len;
}

int utb_hal_dbg_forward_log_level(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);
    return use_len;
}

int utb_hal_dbg_syms_cfg(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len, uint32_t dir)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);

    int valid = 0;
    char syms_cfg[64] = { 0 };
    int ret = ql25_phy_get_syms_cfg(ql25_ctrl, dir, syms_cfg);
    if (ret != 0) {
        use_len += snprintf(dbg_str + use_len, len - use_len, "get tx syms failed\n");
        return use_len;
    }

    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\n%s sym:\n", (dir == UTB_DIR_TX) ? "tx" : "rx");
    UTB_CHECK_LEN(len, use_len);

    for (int i = 0; i< 64; i++) {
        if (syms_cfg[i] == 0) {
            continue;
        }
        use_len += snprintf(dbg_str + use_len, len - use_len, "%02d:%c ", i, syms_cfg[i]);
        UTB_CHECK_LEN(len, use_len);
        valid++;
        if ((valid % 16) == 0) {
            use_len += snprintf(dbg_str + use_len, len - use_len, "\r\n");
            UTB_CHECK_LEN(len, use_len);
        }
    }
    if (valid == 0) {
        use_len += snprintf(dbg_str + use_len, len - use_len, "no valid %s sym\n", (dir == UTB_DIR_TX) ? "tx" : "rx");
        UTB_CHECK_LEN(len, use_len);
    }
    return use_len;
}

int utb_hal_dbg_syms_nid(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);

    int valid = 0;
    char syms_cfg[64] = { 0 };
    int ret = ql25_phy_get_syms_nid(ql25_ctrl, syms_cfg);
    if (ret != 0) {
        use_len += snprintf(dbg_str + use_len, len - use_len, "get syms nid failed\r\n");
        return use_len;
    }

    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\nsym nid:\r\n");
    UTB_CHECK_LEN(len, use_len);

    for (int i = 0; i< 64; i++) {
        if (syms_cfg[i] == 0xFF) {
            continue;
        }
        use_len += snprintf(dbg_str + use_len, len - use_len, "%02d:%u ", i, syms_cfg[i]);
        UTB_CHECK_LEN(len, use_len);
        valid++;
        if ((valid % 16) == 0) {
            use_len += snprintf(dbg_str + use_len, len - use_len, "\r\n");
            UTB_CHECK_LEN(len, use_len);
        }
    }
    if (valid == 0) {
        use_len += snprintf(dbg_str + use_len, len - use_len, "no valid sym nid\r\n");
        UTB_CHECK_LEN(len, use_len);
    }
    return use_len;
}

int utb_hal_dbg_all_syms_cfg(utb_hal_handle_t *hdle, char *dbg_str, int len)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(hdle, dbg_str, len);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t *)hdle->private;

    use_len += utb_hal_dbg_syms_cfg(ql25_ctrl, dbg_str + use_len, len - use_len, UTB_DIR_TX);
    UTB_CHECK_LEN(len, use_len);

    use_len += utb_hal_dbg_syms_cfg(ql25_ctrl, dbg_str + use_len, len - use_len, UTB_DIR_RX);
    UTB_CHECK_LEN(len, use_len);

    use_len += utb_hal_dbg_syms_nid(ql25_ctrl, dbg_str + use_len, len - use_len);
    UTB_CHECK_LEN(len, use_len);
    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\n");
    return use_len;
}

static int utb_hal_dbg_reg_dump(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len, uint32_t filt,
    utb_dump_reg_info_t *reg_info, uint32_t reg_num)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);

    uint32_t reg_data = 0xFFFFFFFF;
    uint32_t tmp_data;
    for(uint32_t i = 0, j; i < reg_num; i++) {
        uint32_t reg_addr = reg_info[i].addr_start;
        j = i;

        do {
            if(reg_info[j].reg_type & 0x1) {
                
                reg_data = *(uint32_t*)(ql25_ctrl->phy_reg_stub + reg_addr);
#if 0    
                reg_data = (*((volatile uint32_t *)(reg_addr)));
#endif
                if (0xFFFFFFFF == reg_data) {
                    goto next_judge;
                }
            }

            if (reg_info[j].reg_disp) {
                tmp_data = reg_data & GENMASK(reg_info[j].bit_end, reg_info[j].bit_start);
                tmp_data = tmp_data >> reg_info[j].bit_start;
                if ((0 == tmp_data) && (filt)) {
                    goto next_judge;
                }
                if (reg_info[i].addr_end) {
                    use_len += snprintf(dbg_str + use_len, len - use_len,
                        "    %s%d: 0x%x\r\n", reg_info[j].name, (reg_addr - reg_info[i].addr_start)/4, tmp_data);
                } else {
                    use_len += snprintf(dbg_str + use_len, len - use_len,
                        "    %s: 0x%x\r\n", reg_info[j].name, tmp_data);
                }               
                UTB_CHECK_LEN(len, use_len);
            }

next_judge:
            if (reg_info[j].reg_type & 0x2) {
                if(reg_addr == reg_info[i].addr_end) {
                i = j;
                break;
                }
                j = i;
                reg_addr += 4;
            } else {
                j++;
            }
        } while(reg_info[i].addr_end && (reg_addr <= reg_info[i].addr_end));
    }

    return use_len;
}
#if 0

utb_dump_reg_info_t g_mac_reg_info[] = {
    {1,1,31, 16,0x800C, 0, "fixed_length"},
    {1,0,15, 0, 0,      0, "mac_timer"},
    {1,1,31, 16,0x8010, 0, "tx_fifo_available_slices"},
    {1,0,15, 0, 0,      0, "tx_fifo_committed_slices"},
    {1,1,31, 16,0x8014, 0, "rx_fifo_available_slices"},
    {1,0,15, 0, 0,      0, "rx_fifo_committed_slices"},
    {1,0,15, 0, 0,      0, "mac_int_status"},
    {1,1,31, 16,0x801C, 0, "mac_int_mask"},
    {1,0,15, 0, 0,      0, "mac_int_clear"},
    {1,1,31, 16,0x8020, 0, "mac_crc_1_num"},
    {1,0,15, 0, 0,      0, "mac_crc_0_num"},
    {1,0,12, 12,0,      0, "empty_slices_to_mcu_num_clr"},
    {1,0,11, 11,0,      0, "valid_slices_to_mcu_num_clr"},
    {1,0,10, 10,0,      0, "empty_slices_from_phy_num_clr"},
    {1,0,9,  9, 0,      0, "all_slices_from_phy_num_clr"},
    {1,0,8,  8, 0,      0, "status_timeout_wrong_num_clr"},
    {1,0,6,  6, 0,      0, "rx_full_write_wrong_num_clr"},
    {1,0,4,  4, 0,      0, "tx_full_write_wrong_num_clr"},
    {1,0,3,  3, 0,      0, "phy_len_wrong_num_clr"},
    {1,0,2,  2, 0,      0, "mcu_len_wrong_num_clr"},
    {1,0,1,  1, 0,      0, "mac_crc_1_num_clr"},
    {1,0,0,  0, 0,      0, "mac_crc_0_num_clr"},
    {1,1,31, 16,0x8028, 0, "mcu_len_wrong_num"},
    {1,0,15, 0, 0,      0, "phy_len_wrong_num"},
    {1,1,31, 16,0x802C, 0, "tx_full_write_wrong_num"},
    {1,1,31, 16,0x8030, 0, "rx_full_write_wrong_num"},
    {1,0,15, 0, 0,      0, "status_timeout_wrong_num"},
    {1,1,31, 16,0x8038, 0, "all_slices_from_phy_num"},
    {1,0,15, 0, 0,      0, "empty_slices_from_phy_num"},
    {1,1,31, 16,0x803C, 0, "valid_slices_to_mcu_num"},
    {1,0,15, 0, 0,      0, "empty_slices_to_mcu_num"},
    {1,1,31, 16,0x8040, 0, "mcu_vld_slice_num"},
    {1,0,15, 0, 0,      0, "mcu_invld_slice_num"},
    {1,0,15, 0, 0,      0, "mcu_len_right_num"},
    {1,0,15, 0, 0,      0, "total_slice_num_2phy"},
    {1,1,31, 16,0x804C, 0, "valid_slice_num_2phy"},
    {1,0,15, 0, 0,      0, "empty_slice_num_2phy"},
    {1,1,31, 16,0x8050, 0, "tx_unwriten_bytes"},
    {1,0,15, 0, 0,      0, "tx_writen_bytes"},
    {1,1,31, 16,0x8054, 0, "rx_unwriten_bytes"},
    {1,0,15, 0, 0,      0, "rx_writen_bytes"},
};

int utb_hal_dbg_mac_dump(utb_hal_handle_t *hdle, char *dbg_str, int len, uint32_t filt)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(hdle, dbg_str, len);

    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\nmac reg info:\r\n");
    UTB_CHECK_LEN(len, use_len);

    utb_hal_dbg_read_reg(utb_hal_handle_t * hdle, char * dbg_str, int len, uint32_t addr, uint32_t cnt)

    use_len += utb_hal_dbg_reg_dump(hdle, dbg_str + use_len, len - use_len, filt,
        g_mac_reg_info, sizeof(g_mac_reg_info) / sizeof(utb_dump_reg_info_t));

    return use_len;
}

utb_dump_reg_info_t g_phy_reg_info[] = {
    {1, 1, 20, 20, 0x0000,   0,         "utb_enable"},
    {1, 0, 16, 16, 0,        0,         "tx_pre"},
    {1, 0, 12, 12, 0,        0,         "rx_enable"},
    {1, 0, 8,  8,  0,        0,         "tx_enable"},
    {1, 0, 4,  4,  0,        0,         "rx_disable"},
    {1, 0, 0,  0,  0,        0,         "tx_disable"},
    {1, 1, 9,  8,  0x0004,   0,         "msl"},
    {1, 0, 5,  4,  0,        0,         "tran_mode"},
    {1, 0, 0,  0,  0,        0,         "cc_type"},
    {1, 1, 23, 0,  0x0008,   0,         "mcs"},
    {1, 1, 23, 0,  0x000C,   0,         "crc24"},
    {1, 1, 7,  4,  0x0010,   0,         "sub_num"},
    {1, 0, 2,  0,  0,        0,         "sub_start"},
    {1, 1, 4,  4,  0x0014,   0,         "node_type"},
    {1, 0, 3,  0,  0,        0,         "sup_frm_order"},
    {1, 1, 8,  8,  0x0018,   0,         "cci "},
    {1, 0, 7,  0,  0,        0,         "nid"},
    {1, 1, 18, 16, 0x001C,   0,         "ncb_rs_ctrl"},
    {1, 0, 13, 0,  0,        0,         "ncoded_b_ctrl"},
    {1, 1, 17, 8,  0x0020,   0,         "nbytes_ctrl"},
    {1, 0, 7,  0,  0,        0,         "nrs_ctrl"},
    {1, 1, 18, 16, 0x0024,   0,         "ncb_data"},
    {1, 0, 13, 0,  0,        0,         "ncoded_b_data"},
    {1, 1, 17, 8,  0x0028,   0,         "nbytes_data"},
    {1, 0, 7,  0,  0,        0,         "nrs_data"},
    {1, 1, 24, 24, 0x002C,   0,         "rx_mode"},
    {1, 0, 23, 16, 0,        0,         "npa_cnt_ctrl"},
    {1, 0, 15, 8,  0,        0,         "d_err_cnt_ctrl"},
    {1, 0, 6,  4,  0,        0,         "cc_det_thr"},
    {1, 0, 2,  0,  0,        0,         "scc_cnt_max"},
    {1, 1, 23, 0,  0x0030,   0,         "rx_nnd"},
    {1, 1, 29, 24, 0x0034,   0,         "fft_win_shift"},
    {1, 0, 21, 16, 0,        0,         "rx_npd"},
    {1, 1, 7,  0,  0x0038,   0,         "clk_abn_cnt_max"},    
    {1, 1, 27, 0,  0x003C,   0,         "init_dpll"},    
    {1, 1, 31, 16, 0x0040,   0,         "corsync_fc_max"},    
    {1, 0, 15, 0,  0,        0,         "finsync_fc_max"},    
    {1, 1, 31, 16, 0x0044,   0,         "demcrtl_fc_max"},
    {1, 0, 15, 0,  0,        0,         "mcsde_fc_max"},
    {1, 1, 15, 0,  0x0048,   0,         "frmsync_fc_max"},
    {1, 3, 31, 0,  0x004C,   0x00C8,        "rx_frm_sym_conf"},
    {1, 1, 15, 0,  0x00CC,   0,         "rx2txstage1_dly"},
    {1, 1, 15, 0,  0x00D0,   0,         "rx_frm_ofs"},
    {1, 1, 7,  0,  0x00D4,   0,         "tx_dig_gain"},
    {1, 1, 31, 0,  0x00D8,   0,         "filter_start_flag0"},
    {1, 1, 31, 0,  0x00DC,   0,         "filter_start_flag1"},
    {1, 1, 5,  0,  0x00E0,   0,         "filter_start_flag2"},
    {1, 1, 29, 29, 0x00E4,   0,         "adc_edge_sel"},
    {1, 0, 28, 28, 0,        0,         "dac_edge_sel"},
    {1, 0, 19, 16, 0,        0,         "afe_ctrl_sw"},
    {1, 0, 15, 8,  0,        0,         "rx_afe_gain_ana"},
    {1, 0, 7,  4,  0,        0,         "tx_afe_on_ahd_ana"},
    {1, 0, 3,  0,  0,        0,         "tx_afe_off_dly_ana"},
    {1, 1, 24, 16, 0x00E8,   0,         "rd_rxsymtonid_int"},
    {1, 0, 8,  0,  0,        0,         "rd_txsymtype_int"},
    {1, 1, 12, 4,  0x00EC,   0,         "tx_sym_num"},
    {1, 0, 0,  0,  0,        0,         "slave_tx_flag"},
    {1, 1, 31, 0,  0x00F0,   0,         "out_cnt_rxbuf"},
    {1, 1, 31, 0,  0x00F4,   0,         "in_cnt_rxbuf"},
    {1, 1, 27, 0,  0x00F8,   0,         "ref_clk_freq"},
    {1, 0, 7,  4,  0x00FC,   0,         "nca_dem"},
    {1, 0, 1,  0,  0,        0,         "msl_dem"},
    {1, 1, 23, 0,  0x0100,   0,         "mcs_dem"},
    {1, 1, 8,  8,  0x0104,   0,         "rx_fsm_err_flag"},
    {1, 0, 6,  4,  0,        0,         "rx_rsm"},
    {1, 0, 0,  0,  0,        0,         "rx_state_pl"},
    {1, 1, 31, 0,  0x0108,   0,         "rx_frame_idx"},
    {1, 1, 21, 16, 0x010C,   0,         "rx_sym_idx"},
    {1, 0, 15, 8,  0,        0,         "rx_data_abn_cnt"},
    {1, 0, 7,  0,  0,        0,         "rx_ctr_abn_cnt"},
    {1, 1, 24, 20, 0x0110,   0,         "rs_err_cnt"},
    {1, 0, 16, 12, 0,        0,         "rs_berl_err_cnt"},
    {1, 0, 8,  8,  0,        0,         "rs_err_flag"},
    {1, 0, 7,  0,  0,        0,         "in_len_rsdec"},
    {1, 1, 31, 16, 0x0114,   0,         "rs_wr_err_cnt"},
    {1, 0, 15, 0,  0,        0,         "rs_rd_empty_cnt"},
    {1, 1, 29, 0,  0x011C,   0,         "clk_phase_shift"},
    {1, 1, 7,  0,  0x0120,   0,         "clk_offset_abn"},
    {1, 1, 28, 24, 0x0124,   0,         "pilot_fft_shift"},
    {1, 0, 21, 16, 0,        0,         "data_fft_shift"},
    {1, 0, 11, 0,  0,        0,         "sub_amp_max"},
    {1, 1, 31, 0,  0x0128,   0,         "lpbk_dly_mstr"},
    {1, 1, 15, 0,  0x012C,   0,         "sub0_noise_amp"},
    {1, 1, 15, 0,  0x0130,   0,         "sub1_noise_amp"},
    {1, 1, 15, 0,  0x0134,   0,         "sub2_noise_amp"},
    {1, 1, 15, 0,  0x0138,   0,         "sub3_noise_amp"},
    {1, 1, 15, 0,  0x013C,   0,         "sub4_noise_amp"},
    {1, 1, 15, 0,  0x0140,   0,         "sub5_noise_amp"},
    {1, 1, 15, 0,  0x0144,   0,         "sub6_noise_amp"},
    {1, 1, 15, 0,  0x0148,   0,         "sub7_noise_amp"},
    {1, 1, 18, 16, 0x014C,   0,         "ncb_dem"},
    {1, 0, 9,  0,  0,        0,         "inbytes_data"},
    {1, 1, 23, 16, 0x0150,   0,         "rs_data"},
    {1, 0, 13, 0,  0,        0,         "codedbits_data"},
    {1, 1, 13, 4,  0x0154,   0,         "inbytes_ctrl"},
    {1, 0, 2,  0,  0,        0,         "ncb_ctrl"},
    {1, 0, 23, 16, 0,        0,         "rs_ctrl"},
    {1, 0, 13, 0,  0,        0,         "codedbits_ctrl"},
    {1, 1, 9,  8,  0x015C,   0,         "cc_chan_id"},
    {1, 0, 4,  4,  0,        0,         "cc_result_flag"},
    {1, 0, 2,  0,  0,        0,         "cc_nid_num"},
    {1, 1, 31, 24, 0x0160,   0,         "cc_nid0"},
    {1, 0, 23, 16, 0,        0,         "cc_nid1"},
    {1, 0, 15, 8,  0,        0,         "cc_nid2"},
    {1, 0, 7,  0,  0,        0,         "cc_nid3"},
    {1, 1, 8,  0,  0x0164,   0,         "sym_to_nid"},
    {1, 1, 8,  8,  0x0168,   0,         "dig_lb_en"},
    {1, 0, 4,  4,  0,        0,         "led_red"},
    {1, 0, 0,  0,  0,        0,         "led_green"},
    {1, 0, 3,  0,  0x0800,   0,         "tx_sym_type_tb"},
    {1, 2, 29, 0,  0x1800,   0,         "nid_filt_vs_loc_statis"},
    {1, 2, 11, 0,  0x1A00,   0,         "nid_pow_vs_exp_statis"},
    {1, 3, 31, 0,  0x1C00,   0x1D14,    "rx_cd_sym_statis"},
    {1, 3, 31, 0,  0x1E00,   0x1F14,    "rx_crc32_ces_statis"},
    {1, 3, 31, 0,  0x2000,   0x2114,    "rx_crc32_cfs_statis"},
    {0, 1, 31, 1,  0x2200,   0x2314,    "reserve_31_1"},
    {1, 2, 0,  0,  0,        0,         "rx_crc32_statis"},
};

int utb_hal_dbg_phy_dump(utb_hal_handle_t *hdle, char *dbg_str, int len, uint32_t filt)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(hdle, dbg_str, len);

    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\nphy reg info:\r\n");
    UTB_CHECK_LEN(len, use_len);

    use_len += utb_hal_dbg_reg_dump(hdle, dbg_str + use_len, len - use_len, filt,
        g_phy_reg_info, sizeof(g_phy_reg_info) / sizeof(utb_dump_reg_info_t));

    return use_len;

}
#else

int utb_hal_dbg_mac_dump(utb_hal_handle_t *hdle, char *dbg_str, int len, uint32_t filt)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(hdle, dbg_str, len);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t *)hdle->private;

    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\nmac reg info:\r\n");
    UTB_CHECK_LEN(len, use_len);

    uint32_t base_addr = 0x8000;
    uint32_t cnt = 30;
    use_len += utb_hal_disp_reg(ql25_ctrl, dbg_str + use_len, len - use_len, base_addr, cnt, filt);
    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\n");
    return use_len;
}

int utb_hal_dbg_phy_dump(utb_hal_handle_t *hdle, char *dbg_str, int len, uint32_t filt)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(hdle, dbg_str, len);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t *)hdle->private;

    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\nphy reg info:\r\n");
    UTB_CHECK_LEN(len, use_len);

    uint32_t reg_info[][2] = {
        { 0x0, 90 }, { 0x1800, 70 }, { 0x1A00, 70 }, { 0x1C00, 70 },
        { 0x1E00, 70 }, { 0x2000, 70 }, { 0x2200, 70 }
    };
    uint32_t num = sizeof(reg_info) / sizeof(reg_info[0]);

    for (uint32_t i = 0; i < sizeof(num); i++) {
        use_len += utb_hal_disp_reg(ql25_ctrl, dbg_str + use_len, len - use_len,
            reg_info[i][0], reg_info[i][1], filt);
        UTB_CHECK_LEN(len, use_len);
    }
    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\n");

    return use_len;
}


#endif


int utb_hal_dbg_get_snr(utb_hal_handle_t *hdle, char *dbg_str, int len)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(hdle, dbg_str, len);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t *)hdle->private;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);

    uint32_t bandnum = 0;
    (void)ql25_get_phy_rx_nca_dem(ql25_ctrl, &bandnum);
    for (uint32_t i = 0; i < bandnum; i++) {
        double snr = 0;
        (void)ql25_get_snr(ql25_ctrl, i, &snr);
        use_len += snprintf(dbg_str + use_len, len - use_len, "snr%d= %.3f  ", i, snr);
        UTB_CHECK_LEN(len, use_len);
    }
    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\n");

    return use_len;
}

int utb_hal_dbg_disp_phy_agc(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);

    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\nagc: ");
    UTB_CHECK_LEN(len, use_len);
    
    for (int i = 0; i < 70; i++) {
        uint32_t reg_data = ql25_get_pwrdiff_of_nid(ql25_ctrl, i);
        if (reg_data) {
            use_len += snprintf(dbg_str + use_len, len - use_len, "%d:0x%x ", i, reg_data);
            UTB_CHECK_LEN(len, use_len);
        }
    }
    return use_len;
}

int utb_hal_dbg_disp_phy_rx_crc(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);

    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\nrx crc_err_a: ");
    UTB_CHECK_LEN(len, use_len);
    for (int i = 0; i < 70; i++) {
        uint32_t reg_data = ql25_get_crc_cnt_of_nid(ql25_ctrl, i);
        if (reg_data) {
            use_len += snprintf(dbg_str + use_len, len - use_len, "%d:0x%x ", i, reg_data);
            UTB_CHECK_LEN(len, use_len);
        }
    }
    return use_len;
}

int utb_hal_dbg_disp_phy_symb_cnt(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);

    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\nrx cnt: ");
    UTB_CHECK_LEN(len, use_len);
    for (int i = 0; i < 70; i++) {
        uint32_t reg_data = ql25_get_symb_cnt_of_nid(ql25_ctrl, i);
        if (reg_data) {
            use_len += snprintf(dbg_str + use_len, len - use_len, "%d:0x%x ", i, reg_data);
            UTB_CHECK_LEN(len, use_len);
        }
    }
    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\n");
    return use_len;
}

int utb_hal_dbg_get_phy_rx_state(utb_hal_handle_t *hdle, char *dbg_str, int len)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(hdle, dbg_str, len);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t *)hdle->private;

    use_len += utb_hal_dbg_disp_phy_agc(ql25_ctrl, dbg_str + use_len, len - use_len);
    UTB_CHECK_LEN(len, use_len);

    use_len += utb_hal_dbg_disp_phy_symb_cnt(ql25_ctrl, dbg_str + use_len, len - use_len);
    UTB_CHECK_LEN(len, use_len);

    use_len += utb_hal_dbg_disp_phy_rx_crc(ql25_ctrl, dbg_str + use_len, len - use_len);
    return use_len;
}

int utb_hal_dbg_disp_mac_ingr_cnt(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len, uint32_t filt)
{
    int use_len = 0;
    utb_dump_reg_info_t reg_info[] = {
        {1,1,31, 0,  0x8068, 0, "mcu2mac_valid_slice_num"},
        {1,1,31, 0,  0x8044, 0, "mcu2mac_len_right_num"},
        {1,1,31, 0,  0x806c, 0, "mac2phy_valid_slice_num"},
        {1,1,31, 0,  0x802c, 0, "tx_full_write_wrong_num(bytes)"},
        {1,1,28, 16, 0x8050, 0, "tx_unwriten_bytes(bytes)"},
    };
    uint32_t reg_num = sizeof(reg_info) / sizeof(reg_info[0]);

    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);
    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\nmac-ingress cnt:\r\n");
    UTB_CHECK_LEN(len, use_len);

    use_len += utb_hal_dbg_reg_dump(ql25_ctrl, dbg_str + use_len, len - use_len, filt, reg_info, reg_num);
    return use_len;
}

int utb_hal_dbg_disp_mac_egr_cnt(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len, uint32_t filt)
{
    int use_len = 0;
    utb_dump_reg_info_t reg_info[] = {
        {1,1,31, 0, 0x8020, 0, "mac_crc_0_num"},
        {1,1,31, 0, 0x8058, 0, "mac_crc_1_num"},
        {1,1,31, 0, 0x8060, 0, "all_slices_from_phy_num"},
        {1,1,31, 0, 0x8038, 0, "mac_recv_phy_num"},
        {1,1,31, 0, 0x8064, 0, "mac2_mcu_valid_num"},
        {1,1,31, 0, 0x8030, 0, "rx_full_write_wrong_num(bytes)"},
        {1,1,12, 0, 0x8050, 0, "rx_writen_bytes(bytes)"},
    };
    uint32_t reg_num = sizeof(reg_info) / sizeof(reg_info[0]);

    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);
    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\nmac-egress cnt:\r\n");
    UTB_CHECK_LEN(len, use_len);

    use_len += utb_hal_dbg_reg_dump(ql25_ctrl, dbg_str + use_len, len - use_len, filt, reg_info, reg_num);
    return use_len;
}

int utb_hal_dbg_display_mac_cnt(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len, uint32_t filt)
{
    int use_len = 0;
    use_len += utb_hal_dbg_disp_mac_ingr_cnt(ql25_ctrl, dbg_str + use_len, len - use_len, filt);
    UTB_CHECK_LEN(len, use_len);

    use_len += utb_hal_dbg_disp_mac_egr_cnt(ql25_ctrl, dbg_str + use_len, len - use_len, filt);
    return use_len;
}

int utb_hal_dbg_display_phy_cnt(utb_ql25_ctrl_t *ql25_ctrl, char *dbg_str, int len, uint32_t filt)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(ql25_ctrl, dbg_str, len);

    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\nphy cnt:");
    UTB_CHECK_LEN(len, use_len);
    use_len += utb_hal_dbg_disp_phy_rx_crc(ql25_ctrl, dbg_str + use_len, len - use_len);
    UTB_CHECK_LEN(len, use_len);

    if (2 == filt) //enable get more messages
    {
        use_len += utb_hal_dbg_disp_phy_agc(ql25_ctrl, dbg_str + use_len, len - use_len);
        UTB_CHECK_LEN(len, use_len);
        use_len += utb_hal_dbg_syms_cfg(ql25_ctrl, dbg_str + use_len, len - use_len, UTB_DIR_TX);
        UTB_CHECK_LEN(len, use_len);
        use_len += utb_hal_dbg_syms_nid(ql25_ctrl, dbg_str + use_len, len - use_len);
    }
    return use_len;
}

int utb_hal_dbg_display_cnt(utb_hal_handle_t *hdle, char *dbg_str, int len, uint32_t filt)
{
    int use_len = 0;
    UTB_DBG_CHECK_PARAMS_AND_RETURN(hdle, dbg_str, len);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t *)hdle->private;

    /* 显示mac统计 */
    use_len += utb_hal_dbg_display_mac_cnt(ql25_ctrl, dbg_str + use_len, len - use_len, filt);
    UTB_CHECK_LEN(len, use_len);

    /* 显示phy统计     */
    use_len += utb_hal_dbg_display_phy_cnt(ql25_ctrl, dbg_str + use_len, len - use_len, filt);
    UTB_CHECK_LEN(len, use_len);

    /* 显示软转统计 todo */
    use_len += snprintf(dbg_str + use_len, len - use_len, "\r\nsoft-forward cnt: ");
    UTB_CHECK_LEN(len, use_len);

    return use_len;
}


/*
 * Copyright (c) 2025 QuickLink Ltd.
 *
 * Author: zhaowei <zhaowei@xlianzn.com>
 *
 * UTB Driver struct definition
 */
 
#ifndef QL25_PHY_DRIVER_H
#define QL25_PHY_DRIVER_H

typedef struct _tag_utb_params {

    uint32_t nsd0;      // 数据符号, 承载的字节数
    uint32_t nrs0;      // 数据符号, RS编码块长度
    uint32_t nsc0;      // 数据符号, 承载的编码比特长度
    uint32_t ncb0;      // 数据符号, 承载的RS编码块数量

    uint32_t nsd1;      // 控制符号, 承载的字节数
    uint32_t nrs1;      // 控制符号, RS编码块长度
    uint32_t nsc1;      // 控制符号, 承载的编码比特长度
    uint32_t ncb1;      // 控制符号, 承载的RS编码块数量

    uint32_t nca;       // 通信使用的 子带数量.
} ql25_params_t;

typedef struct {    
    uint32_t data[4];
    uint32_t sidx;
    uint32_t slen;
} rci_ctx_t;

void ql25_gen_params(uint8_t msl, uint8_t *tcm, ql25_params_t *params);
int ql25_set_phy_crc24(utb_ql25_ctrl_t *ql25_ctrl, uint8_t msl, uint8_t *tcm);
int ql25_set_phy_nca(utb_ql25_ctrl_t *ql25_ctrl, uint32_t nca);
int ql25_set_phy_master_slave_mode(utb_ql25_ctrl_t *ql25_ctrl, uint8_t msl, uint8_t nid);
int ql25_set_phy_nid(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid);
int ql25_set_phy_ofdm_config(utb_ql25_ctrl_t *ql25_ctrl, ql25_params_t *params);
int ql25_init_phy_nnd(utb_ql25_ctrl_t *ql25_ctrl);
int ql25_set_phy_rx_node_ctrl(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid, uint8_t msl);
int ql25_init_phy_rx_cfg(utb_ql25_ctrl_t *ql25_ctrl);
int ql25_set_phy_rx_abn_max(utb_ql25_ctrl_t *ql25_ctrl, uint32_t clk_abn_cnt_max);
int ql25_init_phy_rx_dpll(utb_ql25_ctrl_t *ql25_ctrl);
int ql25_get_phy_tx_gain(utb_ql25_ctrl_t *ql25_ctrl, uint32_t *gain);
int ql25_set_phy_tx_gain(utb_ql25_ctrl_t *ql25_ctrl, uint32_t gain);
int ql25_get_phy_rx_afe_gain(utb_ql25_ctrl_t *ql25_ctrl, uint32_t *gain);
int ql25_set_phy_rx_afe_gain(utb_ql25_ctrl_t *ql25_ctrl, uint32_t gain);
int ql25_init_phy_cci(utb_ql25_ctrl_t *ql25_ctrl);
int ql25_init_phy_rx_frm_ctrl(utb_ql25_ctrl_t *ql25_ctrl);
int ql25_get_phy_rx_msl_tcm(utb_ql25_ctrl_t *ql25_ctrl, uint8_t *msl, uint32_t *mcm, uint8_t *tcm);
int ql25_get_phy_rx_nca_dem(utb_ql25_ctrl_t *ql25_ctrl, uint32_t *band_num);
int ql25_set_phy_rx_sfrm_ctrl(utb_ql25_ctrl_t *ql25_ctrl, uint32_t sidx, uint32_t slen);
int ql25_erase_phy_rx_sfrm_ctrl(utb_ql25_ctrl_t *ql25_ctrl, uint32_t sidx, uint32_t slen);
int ql25_set_phy_rx2tx_delay(utb_ql25_ctrl_t *ql25_ctrl, uint32_t delay);
int ql25_set_phy_cc_type(utb_ql25_ctrl_t *ql25_ctrl, uint32_t cc_type);
uint32_t ql25_get_phy_rx_rsm(utb_ql25_ctrl_t *ql25_ctrl);
uint32_t ql25_get_pwrdiff_of_nid(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid);
uint32_t ql25_get_symb_cnt_of_nid(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid);
uint32_t ql25_get_crc_cnt_of_nid(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid);
uint32_t ql25_get_phy_rx_fsm_err_flag(utb_ql25_ctrl_t *ql25_ctrl);
uint32_t ql25_get_phy_tx_flag(utb_ql25_ctrl_t *ql25_ctrl);
int ql25_set_phy_txrx_en(utb_ql25_ctrl_t *ql25_ctrl, uint32_t tx_en, uint32_t rx_en);
int ql25_set_phy_utb_en(utb_ql25_ctrl_t *ql25_ctrl);
int ql25_get_snr(utb_ql25_ctrl_t *ql25_ctrl, uint32_t band_num, double *snr);
int ql25_get_phy_nid(utb_ql25_ctrl_t *ql25_ctrl, uint8_t *nid);

int ql25_set_phy_symb_config(utb_ql25_ctrl_t *ql25_ctrl, uint32_t sidx, uint32_t mtype);
int ql25_set_phy_symb_cfg_range(utb_ql25_ctrl_t *ql25_ctrl, uint32_t sidx, uint32_t slen);
int ql25_set_phy_symb_cfg_erase(utb_ql25_ctrl_t *ql25_ctrl, uint32_t sidx, uint32_t slen );
int ql25_set_sym2nid_config(utb_ql25_ctrl_t *ql25_ctrl, uint32_t sidx, uint32_t slen, uint8_t nid);
uint32_t ql25_get_rx_frame_idx(utb_ql25_ctrl_t *ql25_ctrl);
int ql25_phy_clr_cnt(utb_ql25_ctrl_t *ql25_ctrl);
int ql25_reset_phy_all_reg(utb_ql25_ctrl_t *ql25_ctrl);
int ql25_init_phy_tx(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid, uint8_t msl, uint8_t *tcm, ql25_params_t *params);
int ql25_init_phy_rx(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid, uint8_t msl);
int ql25_set_phy_rx_en(utb_ql25_ctrl_t *ql25_ctrl, uint32_t en);
int ql25_set_phy_tx_en(utb_ql25_ctrl_t *ql25_ctrl, uint32_t en);
int ql25_phy_get_syms_cfg(utb_ql25_ctrl_t *ql25_ctrl, uint32_t dir, char *syms_cfg);
int ql25_phy_get_syms_nid(utb_ql25_ctrl_t *ql25_ctrl, char *syms_cfg);

int ql25_init_phy_selftest(utb_ql25_ctrl_t *ql25_ctrl);
#endif //QL25_PHY_DRIVER_H




#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ql25_pub.h"
#include "ql25_phy_drv.h"
#include "ql25_phy_reg.h"

static int ql25_read_phy(utb_ql25_ctrl_t *ql25_ctrl, uint32_t reg_offset_addr, uint32_t *data)
{
    *data = *(uint32_t*)(ql25_ctrl->phy_reg_stub + reg_offset_addr);
#if 0
    *data = (*((volatile uint32_t *)(UTB_PHY_BASE + reg_offset_addr)));
#endif
    return 0;
}

static int ql25_write_phy(utb_ql25_ctrl_t *ql25_ctrl, uint32_t reg_offset_addr, uint32_t data)
{
    *(uint32_t*)(ql25_ctrl->phy_reg_stub + reg_offset_addr) = data;
#if 0
    (*((volatile uint32_t *)(UTB_PHY_BASE + reg_offset_addr))) = data;
#endif
    return 0;
}

static int ql25_set_phy_msl(utb_ql25_ctrl_t *ql25_ctrl, uint8_t msl)
{
    PHY_Mode_Configuration_t reg_value;
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Mode_Configuration), &reg_value.value); 
    UTB_CHECK_RET_AND_RETUN(ret);

    reg_value.reg.msl = msl;
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Mode_Configuration), reg_value.value); 
}

static int ql25_set_phy_mcs(utb_ql25_ctrl_t *ql25_ctrl, uint8_t *tcm)
{
    uint32_t mcs = 0;
    for (int i = 7; i >= 0; i--) {
        mcs = (mcs << 3) | tcm[i];
    }
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Enc_Mcs), mcs); 
}

// msl 符号长度, 取值 0/1/2/3
// tcm 是整数数组, 长度 8 .
// rult 是数组, 长度 9, 最后一个字段是 nca
void ql25_gen_params(uint8_t msl, uint8_t *tcm, ql25_params_t *params)
{
    int i;
    int nmw;
    int nca;        // 使用的边带数量.

    // 根据 msl 约束子带数量.
    // 1,2,4,8
    nmw = (int)(1ul << msl);
    for ( i=nmw; i<8; i++ )  {
        tcm[i] = 0;
    }

    // 用户可以选择 使用更少的 子带.
    nca = 0;        
    for ( i=0; i<nmw; i++ )  {
            
        // 如果 tcm[i] 配置为 0 , 表示后边子带都不使用了.
        if ( tcm[i] == 0 ) {
            break;
        }
        
        nca ++;
    }
    

    const uint32_t bitorg[8] = { 0, 2, 3, 4,  5, 6,  8,  9 };
    const uint32_t bitcod[8] = { 0, 4, 4, 6, 10, 8, 12, 12 };
    uint32_t nsr = 0;       // 编码之前 bits 汇总.
    uint32_t nsc = 0;       // 编码之后 bits 汇总.
    uint32_t ncb;
    uint32_t nrs = 0;
    uint32_t tmp;
    uint32_t prs;


    for ( i=0; i<nca; i++ )  {
        uint8_t idx = tcm[i];
        nsr += bitorg[idx];
        nsc += bitcod[idx];
    }
    
    nsc = nsc * 96;         // 每个子带, 包括 96 个子载波.
    nsr = nsr * (96 >> 3);  // 换算成字节单位.
    // fmt.Println( "nsr:", nsr )
    
    // Ncb = floor((Kcc+254)/255);  RS 编码快数量.
    ncb = (nsr >> 8) + 1;
    if ( (255 * ncb) < nsr ) {
        ncb += 1;
    }
    
    // Nrs = floor((Kcc+Ncb-1)/Ncb)
    tmp = nsr + ncb - 1;
    if ( ncb == 1 )  {
        nrs = tmp;
    } else if ( ncb == 2 )  {
        nrs = tmp >> 1;
    } else if ( ncb == 4 )  {
        nrs = tmp >> 2;
    } else {
        nrs = (tmp * 683) >> 11;
    }
    
    // 当 Nrs 大于 127时，Prs=32；当Nrs小于或等于127时，Prs=16。
    prs = 16;
    if ( nrs > 127 )  {
        prs = 32;
    }
    
    // 汇总输出.
    params->nsd0 = nsr - (prs * ncb) - 6;       // nsd, 字节数
    params->ncb0 = ncb;                         // RS 编码块数量
    params->nrs0 = nrs;                         // RS 编码块长度.
    params->nsc0 = nsc;                         // 编码之后的 bit 总数.

    // 重新计算控制帧, 载波数量减半.
    nsc = nsc >> 1;
    nsr = nsr >> 1;
    // fmt.Println( "nsr:", nsr )
    
    // Ncb = floor((Kcc+254)/255);  RS 编码快数量.
    ncb = (nsr >> 8) + 1;
    if ( (255 * ncb) < nsr ) {
        ncb += 1;
    }
    
    // Nrs = floor((Kcc+Ncb-1)/Ncb)
    tmp = nsr + ncb - 1;
    if ( ncb == 1 )  {
        nrs = tmp;
    } else if ( ncb == 2 )  {
        nrs = tmp >> 1;
    } else if ( ncb == 4 )  {
        nrs = tmp >> 2;
    } else {
        nrs = (tmp * 683) >> 11;
    }
    
    // 当 Nrs 大于 127时，Prs=32；当Nrs小于或等于127时，Prs=16。
    prs = 16;
    if ( nrs > 127 )  {
        prs = 32;
    }
    
    // nsd, 字节数
    params->nsd1 = nsr - (prs * ncb) - 5;
    params->ncb1 = ncb;
    params->nrs1 = nrs;
    params->nsc1 = nsc;

    params->nca = nca;

    return;
}

/*
要求输入一个 bytes 类型数组.
返回一个 uint32 整数结果.
*/
static uint32_t ql25_crc24(uint8_t * pdat, int tlen)
{
    uint32_t temp;
    uint32_t crcv = 0;
    uint32_t poly = 0xC60001;
    
    
    for ( int i=0; i<tlen; i++ )  {
        
        temp = (crcv ^ pdat[i]) & 0x1;
        crcv = crcv >> 1;

        if ( temp != 0 )  {
            crcv = crcv ^ poly;
        }
    }
    
    return crcv;
}

/*
msl 符号长度, 取值 0/1/2/3
tcm 是整数数组, 长度 8 .
*/
static uint32_t ql25_tcm_crc( uint8_t msl, uint8_t *tcm)
{
    int nmw;
    int i;
    uint8_t data[24];

    nmw = (int)(1ul << msl);
    for ( i=nmw; i<8; i++ )  {
        tcm[i] = 0;
    }
    

    for ( i=0; i<8; i++ )  {
        
        if ( tcm[i] == 0 )  {
            data[ (i*3) + 0 ] = 0;
            data[ (i*3) + 1 ] = 0;
            data[ (i*3) + 2 ] = 0;
        }
        
        for ( int j=2; j>=0; j-- )  {
            data[ (i*3) + 2 - j ] = (tcm[i] >> j) & 0x1;
        }
    }
    
    return ql25_crc24(data, 24);
}

int ql25_set_phy_crc24(utb_ql25_ctrl_t *ql25_ctrl, uint8_t msl, uint8_t *tcm)
{
    uint32_t crc24;
    crc24 = ql25_tcm_crc(msl, tcm);
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Enc_CRC24), crc24);
}

int ql25_set_phy_nca(utb_ql25_ctrl_t *ql25_ctrl, uint32_t nca)
{
    PHY_Sbb_Configuration_t reg_value;
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Sbb_Configuration), &reg_value.value);    
    UTB_CHECK_RET_AND_RETUN(ret);

    reg_value.reg.sub_num = nca;
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Sbb_Configuration), reg_value.value);    
}

/* nid为0则为master，否则为slave */
int ql25_set_phy_master_slave_mode(utb_ql25_ctrl_t *ql25_ctrl, uint8_t msl, uint8_t nid)
{
    PHY_MSN_Configuration_t reg_value;
    reg_value.value = 0;
    if (nid == 0) {
        reg_value.reg.sup_frm_order = 5 - msl;
        reg_value.reg.node_type = 1;
    }
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_MSN_Configuration), reg_value.value);    
}


/* nid为0则为master，否则为slave */
int ql25_set_phy_nid(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid)
{
    PHY_CC_Configuration_t reg_value = { 0 };
    if ((nid == 0) || (nid == 0xff)) {
        reg_value.reg.cci = 1;
    }
    reg_value.reg.nid = nid;
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_CC_Configuration), reg_value.value);
}

int ql25_get_phy_nid(utb_ql25_ctrl_t *ql25_ctrl, uint8_t *nid)
{
    PHY_CC_Configuration_t reg_value = { 0 };
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_CC_Configuration), &reg_value.value);
    UTB_CHECK_RET_AND_RETUN(ret);

    *nid = reg_value.reg.nid;
    return ret;
}


/* todo还得跟王聪对齐一下如何配置 */
int ql25_set_phy_txrx_en(utb_ql25_ctrl_t *ql25_ctrl, uint32_t tx_en, uint32_t rx_en)
{
    PHY_Configuration_t reg_value = { 0 };

    uint8_t nid = 0;
    int ret = ql25_get_phy_nid(ql25_ctrl, &nid);
    UTB_CHECK_RET_AND_RETUN(ret);

    if (nid != 0) {
        reg_value.reg.rx_auto_recover = 1;
    }
    reg_value.reg.utb_enable = 1;
    reg_value.reg.tx_pre = tx_en;
    reg_value.reg.tx_enable = tx_en;
    reg_value.reg.rx_enable = rx_en;
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Configuration), reg_value.value);    
}

int ql25_set_phy_rx_en(utb_ql25_ctrl_t *ql25_ctrl, uint32_t en)
{
    PHY_Configuration_t reg_value = { 0 };
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Configuration), &reg_value.value);
    UTB_CHECK_RET_AND_RETUN(ret);

    /* 芯片不允许重复开启，所以只能先关后开 */
    reg_value.reg.rx_disable = 1;
    ret = ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Configuration), reg_value.value);    
    UTB_CHECK_RET_AND_RETUN(ret);

    if (en) {
        reg_value.reg.rx_disable = 0;
        reg_value.reg.rx_enable = 1;
        ret = ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Configuration), reg_value.value);
    }
    return ret;
}

int ql25_set_phy_tx_en(utb_ql25_ctrl_t *ql25_ctrl, uint32_t en)
{
    PHY_Configuration_t reg_value = { 0 };
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Configuration), &reg_value.value);
    UTB_CHECK_RET_AND_RETUN(ret);

    /* 芯片不允许重复开启，所以只能先关后开 */
    reg_value.reg.tx_disable = 1;
    ret = ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Configuration), reg_value.value);    
    UTB_CHECK_RET_AND_RETUN(ret);

    if (en) {
        reg_value.reg.tx_disable = 0;
        reg_value.reg.tx_enable = 1;
        reg_value.reg.tx_pre = 1;
        ret = ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Configuration), reg_value.value);
    }
    return ret;
}

int ql25_set_phy_utb_en(utb_ql25_ctrl_t *ql25_ctrl)
{
    PHY_Configuration_t reg_value = { 0 };

    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Configuration), &reg_value.value);    
    UTB_CHECK_RET_AND_RETUN(ret);

    reg_value.reg.utb_enable = 1;
#if 0
    uint8_t nid = 0;    
    ret = ql25_get_phy_nid(ql25_ctrl, &nid);
    UTB_CHECK_RET_AND_RETUN(ret);
    if (nid != 0) {
        reg_value.reg.rx_auto_recover = 1;
    }
#endif
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Configuration), reg_value.value);    
}

int ql25_set_phy_ofdm_config(utb_ql25_ctrl_t *ql25_ctrl, ql25_params_t *params)
{
    PHY_OFDMC_Configuration1_t ofdmc_cfg1 = { 0 };
    PHY_OFDMC_Configuration2_t ofdmc_cfg2 = { 0 };
    PHY_OFDMD_Configuration1_t ofdmd_cfg1 = { 0 };
    PHY_OFDMD_Configuration2_t ofdmd_cfg2 = { 0 };

    ofdmc_cfg1.reg.ncb_rs_ctrl = params->ncb1;
    ofdmc_cfg1.reg.ncoded_b_ctrl =params->nsc1;
    ofdmc_cfg2.reg.nbytes_ctrl = params->nsd1;
    ofdmc_cfg2.reg.nrs_ctrl =params->nrs1;
    ofdmd_cfg1.reg.ncb_data = params->ncb0;
    ofdmd_cfg1.reg.ncoded_b_data =params->nsc0;
    ofdmd_cfg2.reg.nbytes_data = params->nsd0;
    ofdmd_cfg2.reg.nrs_data =params->nrs0;

    int ret = ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_OFDMC_Configuration2), ofdmc_cfg2.value);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_OFDMD_Configuration2), ofdmd_cfg2.value);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_OFDMC_Configuration1), ofdmc_cfg1.value);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_OFDMD_Configuration1), ofdmd_cfg1.value);
    return ret;
}

int ql25_init_phy_nnd(utb_ql25_ctrl_t *ql25_ctrl)
{
    PHY_Rx_NND_t reg_value = { 0 };
    reg_value.reg.rx_nnd = 0; /* 默认值配置,按照当前默认值配置 */
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_NND), reg_value.value);
}

int ql25_set_phy_rx_node_ctrl(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid, uint8_t msl)
{
    PHY_Rx_Node_Ctrl_t reg_value = { 0 };
    reg_value.reg.fft_win_shift = 0x18;
    if (nid == 0) {
        reg_value.reg.rx_npd = 0x20;
        reg_value.reg.rx_npd_adjust_m = (0xFF00 >> (3 - msl) | 0x8000);
    } else {
        reg_value.reg.rx_npd = 0x10;
    }
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Node_Ctrl), reg_value.value);
}

int ql25_init_phy_rx_cfg(utb_ql25_ctrl_t *ql25_ctrl)
{
    PHY_Rx_Configuration_t reg_value = { 0 };
    /* 初始化为默认值 */
    reg_value.reg.scc_cnt_max = 4;
    reg_value.reg.cc_det_thr = 2;
    reg_value.reg.d_err_cnt_ctrl = 0x40;
    reg_value.reg.npa_cnt_ctrl = 0x40;

    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Configuration), reg_value.value);
}

int ql25_set_phy_rx_abn_max(utb_ql25_ctrl_t *ql25_ctrl, uint32_t clk_abn_cnt_max)
{    
    PHY_Rx_Abn_Max_t reg_value = { 0 };
    reg_value.reg.clk_abn_cnt_max = clk_abn_cnt_max;
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Abn_Max), reg_value.value);
}


int ql25_init_phy_rx_dpll(utb_ql25_ctrl_t *ql25_ctrl)
{   
    PHY_Rx_dpll_t reg_value = { 0 };
    reg_value.reg.init_dpll = 0xD1B7175; /* 默认值配置,按照当前默认值配置 */
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_dpll), reg_value.value);
}

int ql25_get_phy_tx_gain(utb_ql25_ctrl_t *ql25_ctrl, uint32_t *gain)
{
    PHY_Tx_Configuration_t reg_value = { 0 };    
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Tx_Configuration), &reg_value.value);
    UTB_CHECK_RET_AND_RETUN(ret);
    *gain = reg_value.reg.tx_dig_gain;
    return ret;
}

int ql25_set_phy_tx_gain(utb_ql25_ctrl_t *ql25_ctrl, uint32_t gain)
{
    PHY_Tx_Configuration_t reg_value = { 0 };    
    reg_value.reg.tx_dig_gain = gain;
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Tx_Configuration), reg_value.value);
}

int ql25_get_phy_rx_afe_gain(utb_ql25_ctrl_t *ql25_ctrl, uint32_t *gain)
{
    PHY_Tx_Afe_Ctrl_t reg_value = { 0 };    
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Tx_Afe_Ctrl), &reg_value.value);
    UTB_CHECK_RET_AND_RETUN(ret);

    *gain = reg_value.reg.rx_afe_gain_ana;
    return ret;
}

int ql25_set_phy_rx_afe_gain(utb_ql25_ctrl_t *ql25_ctrl, uint32_t gain)
{
    PHY_Tx_Afe_Ctrl_t reg_value = { 0 };    
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Tx_Afe_Ctrl), &reg_value.value);
    UTB_CHECK_RET_AND_RETUN(ret);

    reg_value.reg.rx_afe_gain_ana = gain;
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Tx_Afe_Ctrl), reg_value.value);
}

int ql25_init_phy_cci(utb_ql25_ctrl_t *ql25_ctrl)
{
    PHY_CC_Configuration_t reg_value;
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_CC_Configuration), &reg_value.value);
    UTB_CHECK_RET_AND_RETUN(ret);

    reg_value.reg.cci = 1; /* 初始化使能竞争标记，slave分到nid后再关闭 */
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_CC_Configuration), reg_value.value);
}

int ql25_init_phy_rx_frm_ctrl(utb_ql25_ctrl_t *ql25_ctrl)
{
    PHY_Rx_Frm_Ctrl_1_t frm_ctl1 = { 0 };
    PHY_Rx_Frm_Ctrl_2_t frm_ctl2 = { 0 };
    PHY_Rx_Frm_Ctrl_3_t frm_ctl3 = { 0 };

    frm_ctl1.reg.finsync_fc_max = 0x0800;
    frm_ctl1.reg.corsync_fc_max = 0x0400;
    int ret = ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Frm_Ctrl_1), frm_ctl1.value);
    UTB_CHECK_RET_AND_RETUN(ret);

    frm_ctl2.reg.mcsde_fc_max = 0x0200;
    frm_ctl2.reg.demcrtl_fc_max = 0x0200;
    ret = ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Frm_Ctrl_2), frm_ctl2.value);
    UTB_CHECK_RET_AND_RETUN(ret);

    frm_ctl3.reg.frmsync_fc_max = 0x0080;
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Frm_Ctrl_3), frm_ctl3.value);
}

int ql25_get_phy_rx_msl_tcm(utb_ql25_ctrl_t *ql25_ctrl, uint8_t *msl, uint32_t *mcm, uint8_t *tcm)
{
    PHY_Rx_Status_2_t state2 = { 0 };
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Status_2), &state2.value);
    UTB_CHECK_RET_AND_RETUN(ret);
    *mcm = state2.reg.mcs_dem;
    for (int i=0; i<8; i++ )  {
        tcm[i] = state2.reg.mcs_dem & 0x7;
        state2.reg.mcs_dem = state2.reg.mcs_dem >> 3;
    }

    PHY_Rx_Status_1_t state1 = { 0 };
    ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Status_1), &state1.value);
    UTB_CHECK_RET_AND_RETUN(ret);
    *msl = state1.reg.msl_dem;
    return ret;
}

int ql25_get_phy_rx_nca_dem(utb_ql25_ctrl_t *ql25_ctrl, uint32_t *band_num)
{
    PHY_Mode_Configuration_t state1 = { 0 };
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Mode_Configuration), &state1.value);
    UTB_CHECK_RET_AND_RETUN(ret);
    *band_num = 1 << state1.reg.msl;
    return ret;
}

int ql25_rci0_import(utb_ql25_ctrl_t *ql25_ctrl, rci_ctx_t *pctx)
{
    for (int i = 0; i < 4; i++) {
        int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Sfrm_Ctrl[i]), &pctx->data[i]);
        UTB_CHECK_RET_AND_RETUN(ret);
    }

    // 当前状态跟 hardware 一致, 所以 slen = 0.
    pctx->sidx = 0;
    pctx->slen = 0;
    return 0;
}

void ql25_rci0_range(rci_ctx_t *pctx, uint32_t sidx, uint32_t slen )
{
    if (slen <= 1)  {
        UTB_PRINT("rci0 range, slen fail\n");
        return;
    }

    // 16 个符号占用一个 32 位寄存器.
    uint32_t dix = sidx >> 4;
    uint32_t ofs = (sidx & 0xF) << 1;
    pctx->data[dix] &= ~(0x3ul << ofs);
    pctx->data[dix] |= 0x1ul << ofs;

    for (uint32_t i = 1; i < slen; i++)  {

        dix = (sidx + i) >> 4;
        ofs = ((sidx + i) & 0xF) << 1;
        pctx->data[dix] &= ~(0x3ul << ofs);
        pctx->data[dix] |= 0x2ul << ofs;
    }

    // merge range.
    if (pctx->slen == 0)  {
        pctx->sidx = sidx;
        pctx->slen = slen;
        return;
    }

    if (sidx < pctx->sidx)  {
        pctx->slen = pctx->sidx - sidx;
        pctx->sidx = sidx;
    }

    if ((sidx + slen) > (pctx->sidx + pctx->slen))  {
        pctx->slen = sidx + slen - pctx->sidx;
    }

    return;
}

void ql25_rci0_erase(rci_ctx_t * pctx, uint32_t sidx, uint32_t slen)
{
    // 16 个符号占用一个 32 位寄存器.
    for (uint32_t i=0; i<slen; i++ )  {
        uint32_t dix = (sidx + i) >> 4;
        uint32_t ofs = ((sidx + i) & 0xF) << 1;
        pctx->data[dix] &= ~(0x3ul << ofs);
    }

    // merge range.
    if (pctx->slen == 0) {
        pctx->sidx = sidx;
        pctx->slen = slen;
        return;
    }

    if (sidx < pctx->sidx) {
        pctx->slen = pctx->sidx - sidx;
        pctx->sidx = sidx;
    }

    if ((sidx + slen) > (pctx->sidx + pctx->slen)) {
        pctx->slen = sidx + slen - pctx->sidx;
    }

    return;
}

int ql25_rci0_flush(utb_ql25_ctrl_t *ql25_ctrl, rci_ctx_t * pctx)
{
    for (int i = 0; i < 4; i++) {
        int ret = ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Sfrm_Ctrl[i]), pctx->data[i]);
        UTB_CHECK_RET_AND_RETUN(ret);
    }

    // 当前状态跟 hardware 同步之后, 所以 slen = 0.
    pctx->sidx = 0;
    pctx->slen = 0;

    return 0;
}

int ql25_set_phy_rx_sfrm_ctrl(utb_ql25_ctrl_t *ql25_ctrl, uint32_t sidx, uint32_t slen)
{
    rci_ctx_t trci0 = { 0 };
    int ret = ql25_rci0_import(ql25_ctrl, &trci0 );
    UTB_CHECK_RET_AND_RETUN(ret);

    ql25_rci0_range(&trci0, sidx, slen);
    return ql25_rci0_flush(ql25_ctrl, &trci0);    
}

int ql25_erase_phy_rx_sfrm_ctrl(utb_ql25_ctrl_t *ql25_ctrl, uint32_t sidx, uint32_t slen)
{
    rci_ctx_t trci0 = { 0 };
    int ret = ql25_rci0_import(ql25_ctrl, &trci0 );
    UTB_CHECK_RET_AND_RETUN(ret);

    ql25_rci0_erase(&trci0, sidx, slen);
    return ql25_rci0_flush(ql25_ctrl, &trci0);
}

int ql25_set_phy_rx2tx_delay(utb_ql25_ctrl_t *ql25_ctrl, uint32_t delay)
{
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx2Tx_Delay), delay); /* 初始化时固定值 */
}

uint32_t ql25_get_phy_rx_rsm(utb_ql25_ctrl_t *ql25_ctrl)
{
    PHY_Rx_Status_3_t reg_value = { 0 };
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Status_3), &reg_value.value);
    if (ret) {
        return 0;
    }
    return reg_value.reg.rx_rsm;
}

uint32_t ql25_get_pwrdiff_of_nid(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid)
{
    PHY_MEM_Signal_Powe_t reg_value = { 0 };
    if (nid >= 70) {
        return 0;
    }

    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_MEM_Signal_Powe[nid]), &reg_value.value);
    if (ret) {
        return 0;
    }
    return reg_value.reg.nid_pow_vs_exp_statis;
}

uint32_t ql25_get_symb_cnt_of_nid(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid)
{
    uint32_t reg_value = 0;
    if (nid >= 70) {
        return 0;
    }

    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_MEM_Node_Symbol[nid]), &reg_value);
    if (ret) {
        return 0;
    }
    return reg_value;
}

uint32_t ql25_get_crc_cnt_of_nid(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid)
{
    uint32_t reg_value = 0;
    if (nid >= 70) {
        return 0;
    }

    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_MEM_CRCError_Cum[nid]), &reg_value);
    if (ret) {
        return 0;
    }
    return reg_value;
}

uint32_t ql25_get_phy_rx_fsm_err_flag(utb_ql25_ctrl_t *ql25_ctrl)
{
    /* 仅在slave同步完成前查看，与rx_rsm同一个寄存器，多读一个寄存器不影响此时业务 */
    PHY_Rx_Status_3_t reg_value = { 0 };
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Status_3), &reg_value.value);
    if (ret) {
        return 0;
    }
    uint32_t err_flag = reg_value.reg.rx_fsm_err_flag ? 0 : 1; /* 0故障，1正常 */
    return err_flag;
}

uint32_t ql25_get_phy_tx_flag(utb_ql25_ctrl_t *ql25_ctrl)
{
    PHY_Tx_Status_t reg_value = { 0 };
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Tx_Status), &reg_value.value);
    if (ret) {
        return 0;
    }
    return reg_value.reg.slave_tx_flag;
}

int ql25_set_phy_symb_config(utb_ql25_ctrl_t *ql25_ctrl, uint32_t sidx, uint32_t mtype)
{
    for (int frm = 0; frm < 8; frm++) {
        int ret = ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Tx_Mem[frm*64 + sidx]), mtype);
        UTB_CHECK_RET_AND_RETUN(ret);
    }
    return 0;
}

/*
简化流程, 配置起始和长度, 自动转换为 导频符号 和 若干数据符号.
不支持其他 符号类型.
*/
int ql25_set_phy_symb_cfg_range(utb_ql25_ctrl_t *ql25_ctrl, uint32_t sidx, uint32_t slen)
{
    int ret = ql25_set_phy_symb_config(ql25_ctrl, sidx, 1);/* 1:导频 2:控制 3:数据 */
    UTB_CHECK_RET_AND_RETUN(ret);
    for (uint32_t i = 1; i < slen; i++)  {
        if ((sidx + i) == 1) {
            ql25_set_phy_symb_config(ql25_ctrl, sidx + i, 2);/* 1:导频 2:控制 3:数据 */
            UTB_CHECK_RET_AND_RETUN(ret);
        } else {
            ql25_set_phy_symb_config(ql25_ctrl, sidx + i, 3);/* 1:导频 2:控制 3:数据 */
            UTB_CHECK_RET_AND_RETUN(ret);
        }
    }

    return 0;
}

int ql25_set_phy_symb_cfg_erase(utb_ql25_ctrl_t *ql25_ctrl, uint32_t sidx, uint32_t slen)
{
    for (uint32_t i = 0; i < slen; i++)  {
        int ret = ql25_set_phy_symb_config(ql25_ctrl, sidx + i, 0);
        UTB_CHECK_RET_AND_RETUN(ret);
    }
    return 0;
}

int ql25_set_sym2nid_config(utb_ql25_ctrl_t *ql25_ctrl, uint32_t sidx, uint32_t slen, uint8_t nid)
{
    for (uint32_t frm = 0; frm < 8; frm++)  {
        for (uint32_t i = 0; i < slen; i++)  {
            int ret = ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_MEM_NID[(frm * 64) + (sidx + i)]), nid);
            UTB_CHECK_RET_AND_RETUN(ret);
        }
    }

    return 0;
}

int ql25_get_snr(utb_ql25_ctrl_t *ql25_ctrl, uint32_t band_num, double *snr)
{
    if (band_num >= 8) {
        const char* func_name = "utb-phy-selftest";
        UTB_PRINT("Get snr fail, param band-num %u is out of range\n", band_num);
        return -1;
    }
    uint32_t addr = offsetof(utb_phy_reg_t, PHY_Rx_SNA0) + band_num;
    uint32_t value = 0;
    int ret = ql25_read_phy(ql25_ctrl, addr, &value);
    UTB_CHECK_RET_AND_RETUN(ret);

    *snr = 91.3581 - 20*log10(value);
    return ret;
}

uint32_t ql25_get_rx_frame_idx(utb_ql25_ctrl_t *ql25_ctrl)
{
    uint32_t reg_value = 0;
    ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Status_4), &reg_value);
    return reg_value;
}

int ql25_phy_clr_cnt(utb_ql25_ctrl_t *ql25_ctrl)
{
    /* todo */
    return 0;
}

int ql25_phy_get_tx_syms_cfg(utb_ql25_ctrl_t *ql25_ctrl, char *syms_cfg)
{
    char map[] = { 0,  'P', 'C', 'D', 'V'};
    for (int i = 0; i < 64; i++) {
        PHY_Tx_Mem_t reg_value = { 0 };
        int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Tx_Mem[i]), &reg_value.value);
        UTB_CHECK_RET_AND_RETUN(ret);

        if (reg_value.reg.tx_sym_type_tb < (sizeof(map) / sizeof(map[0]))) {
            syms_cfg[i] = map[reg_value.reg.tx_sym_type_tb];
        }
    }
    return 0;
}

int ql25_phy_get_rx_syms_cfg(utb_ql25_ctrl_t *ql25_ctrl, char *syms_cfg)
{
    uint32_t index = 0;
    char map[] = { 0,  'P', 'D'};

    for (int i = 0; i < 4; i++) {
        uint32_t value = 0;
        int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Sfrm_Ctrl[i]), &value);
        UTB_CHECK_RET_AND_RETUN(ret);

        for(int j = 0; j < 16; j++) {
            uint32_t sym = value & 0x3;
            if (sym < (sizeof(map) / sizeof(map[0]))) {
                syms_cfg[index] = map[sym];
            }
            index++;
            value >>= 2;
        }
    }
    return 0;
}

int ql25_phy_get_syms_cfg(utb_ql25_ctrl_t *ql25_ctrl, uint32_t dir, char *syms_cfg)
{
    if (dir == UTB_DIR_TX) {
        return ql25_phy_get_tx_syms_cfg(ql25_ctrl, syms_cfg);
    }
    return ql25_phy_get_rx_syms_cfg(ql25_ctrl, syms_cfg);
}


int ql25_phy_get_syms_nid(utb_ql25_ctrl_t *ql25_ctrl, char *syms_cfg)
{
    for (int i = 0; i < 64; i++) {
        uint32_t value = 0;
        int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_MEM_NID[i]), &value);
        UTB_CHECK_RET_AND_RETUN(ret);

        syms_cfg[i] = value & 0xFF;
    }
    return 0;
}

/* 将phy的所有寄存器清理到默认值 */
int ql25_reset_phy_all_reg(utb_ql25_ctrl_t *ql25_ctrl)
{
    uint32_t sidx = 0;
    uint32_t slen = 64;
    uint8_t default_nid = 0xff;
    /* 清tx、rx符号配置 */
    int ret = ql25_set_phy_symb_cfg_erase(ql25_ctrl, sidx, slen);
    ret |= ql25_erase_phy_rx_sfrm_ctrl(ql25_ctrl, sidx, slen);
    ret |=ql25_set_sym2nid_config(ql25_ctrl, sidx, slen, default_nid);
    /* 设置nid为默认值 */
    ret |=ql25_set_phy_nid(ql25_ctrl, default_nid);

    /* 寄存器清0 */
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Configuration), 0);    
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Mode_Configuration), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Enc_Mcs), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Enc_CRC24), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Sbb_Configuration), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_MSN_Configuration), 0);
//    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_CC_Configuration), 0);    
//    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Tx_Afe_Ctrl), 0);
//    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx2Tx_Delay), 0);
//    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Tx_Configuration), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_OFDMC_Configuration1), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_OFDMC_Configuration2), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_OFDMD_Configuration1), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_OFDMD_Configuration2), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Configuration), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_NND), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Node_Ctrl), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Abn_Max), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_dpll), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Frm_Ctrl_1), 0);
    ret |= ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Frm_Ctrl_2), 0);
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Rx_Frm_Ctrl_3), 0);
}


/* 主:芯片初始化时初始化tx 从:状态6的时候初始化tx*/
int ql25_init_phy_tx(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid, uint8_t msl, uint8_t *tcm, ql25_params_t *params)
{
    int ret = ql25_set_phy_nca(ql25_ctrl, params->nca);
    ret |= ql25_set_phy_master_slave_mode(ql25_ctrl, msl, nid);
    ret |= ql25_set_phy_nid(ql25_ctrl, nid);
    ret |= ql25_set_phy_msl(ql25_ctrl, msl);
    ret |= ql25_set_phy_ofdm_config(ql25_ctrl, params);
    ret |= ql25_set_phy_mcs(ql25_ctrl, tcm);
    ret |= ql25_set_phy_crc24(ql25_ctrl, msl, tcm);

    uint32_t tx_gain = 0xe5;
    for (int i = 0; i < 8; i++) {
        if (tcm[i] >= 6) {
            tx_gain = 0xA2;
            break;
        }
    }
    ret |= ql25_set_phy_tx_gain(ql25_ctrl, tx_gain);
    return ret;
}

int ql25_set_phy_cc_type(utb_ql25_ctrl_t *ql25_ctrl, uint32_t cc_type)
{
    PHY_Mode_Configuration_t reg_value;
    int ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Mode_Configuration), &reg_value.value); 
    UTB_CHECK_RET_AND_RETUN(ret);

    reg_value.reg.cc_type = cc_type;
    return ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Mode_Configuration), reg_value.value); 
}

int ql25_init_phy_rx(utb_ql25_ctrl_t *ql25_ctrl, uint8_t nid, uint8_t msl)
{
    //int ret = ql25_init_phy_nnd(ql25_ctrl);
    int ret = ql25_init_phy_rx_dpll(ql25_ctrl);
    ret |= ql25_set_phy_rx_abn_max(ql25_ctrl, 0x40);
    ret |= ql25_set_phy_rx_node_ctrl(ql25_ctrl, nid, msl);
    ret |= ql25_init_phy_rx_cfg(ql25_ctrl);
    ret |= ql25_init_phy_rx_frm_ctrl(ql25_ctrl);
    ret |= ql25_init_phy_cci(ql25_ctrl);
    if (nid != 0) {
        ret |= ql25_set_phy_rx2tx_delay(ql25_ctrl, 0x6);
        ret |= ql25_set_phy_cc_type(ql25_ctrl, 1);
    } else {
        ret |= ql25_set_phy_rx_afe_gain(ql25_ctrl, 0x6); /* 目前配置默认值 */
    }
    return ret;
}

int ql25_init_phy_selftest(utb_ql25_ctrl_t *ql25_ctrl)
{
    const char* func_name = "utb-phy-selftest";
    uint32_t reg[] = { 0xa5a5a5a5, 0x5a5a5a5a, 0xffffffff, 0};
    for (uint32_t i = 0; i < (sizeof(reg) / sizeof(uint32_t)); i++) {        
        int ret = ql25_write_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Tx_Node_Ctrl_0), reg[i]);
        UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s write 0x%x failed\n", func_name, reg[i]);

        uint32_t value = 0;
        ret = ql25_read_phy(ql25_ctrl, offsetof(utb_phy_reg_t, PHY_Tx_Node_Ctrl_0), &value);
        UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s read failed\n", func_name);

        if (value != reg[i]) {
        	UTB_PRINT("%s write 0x%x read-value 0x%x\n", func_name, reg[i], value);
            return -1;
        }
    }
    
    return 0;
}



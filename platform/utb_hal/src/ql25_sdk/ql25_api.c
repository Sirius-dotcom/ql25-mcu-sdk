
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ql25_pub.h"
#include "ql25_mac_drv.h"
#include "ql25_phy_drv.h"
//#include "ql25_afe_drv.h"
#include "ql25_sys_drv.h"
#include "ql25_dfx.h"
#include "ql25_adp_if.h"

#define UTB_MAX_NUM 1
utb_ql25_ctrl_t *g_ql25_ctrl[UTB_MAX_NUM] = { NULL };

void ql25_deinit_chip_ctrl(utb_ql25_ctrl_t *ql25_ctrl)
{
    if (ql25_ctrl) {
        UTB_FREE(ql25_ctrl->mac_reg_stub);
        UTB_FREE(ql25_ctrl->phy_reg_stub);
        UTB_FREE(ql25_ctrl->sys_reg_stub);
        utb_os_timer_delete(ql25_ctrl->dfx_info.dfx_hd);
        UTB_FREE(ql25_ctrl);
    }
}

utb_ql25_ctrl_t *ql25_init_chip_ctrl(uint32_t chip_id)
{
    utb_ql25_ctrl_t *ql25_ctrl = UTB_MALLOC(sizeof(utb_ql25_ctrl_t));
    if (!ql25_ctrl) {       
        return NULL;
    }
    memset(ql25_ctrl, 0, sizeof(utb_ql25_ctrl_t));

    ql25_ctrl->chip_id = chip_id;

    ql25_dfx_start(ql25_ctrl);
    if (ql25_ctrl->dfx_info.dfx_hd == NULL) {
        goto error;
    }

    /* 打桩 */
    ql25_ctrl->mac_reg_stub = UTB_MALLOC(0x200);
    ql25_ctrl->phy_reg_stub = UTB_MALLOC(0x2400);
    ql25_ctrl->sys_reg_stub = UTB_MALLOC(0x100);
    if (!ql25_ctrl->mac_reg_stub || !ql25_ctrl->phy_reg_stub || !ql25_ctrl->sys_reg_stub) {
    	UTB_PRINT("ql25_init_chip_ctrl init memery failed\n");
        goto error;
    }

    return ql25_ctrl;
error:
    ql25_deinit_chip_ctrl(ql25_ctrl);
    return NULL;
}

void utb_hal_close_chip(utb_hal_handle_t *hdle)
{
    if (hdle == NULL) {
        return;
    }
    UTB_FREE(hdle);
}

utb_hal_handle_t *utb_hal_open_chip(uint32_t chip_id)
{
    if (chip_id >= UTB_MAX_NUM) {
        return NULL;
    }
    utb_hal_handle_t *hdle = UTB_MALLOC(sizeof(utb_hal_handle_t));
    if (!hdle) {
        return NULL;
    }
    memset(hdle, 0, sizeof(utb_hal_handle_t));

    if (!g_ql25_ctrl[chip_id]) {
        g_ql25_ctrl[chip_id] = ql25_init_chip_ctrl(chip_id);
    }    

    if (!g_ql25_ctrl[chip_id]) {
        UTB_FREE(hdle);
        return NULL;
    }

    hdle->private = g_ql25_ctrl[chip_id];
    return hdle;
}

void utb_hal_reset_chip(utb_ql25_ctrl_t *ql25_ctrl)
{
    if (!ql25_ctrl) {
        return;
    }

    UTB_PRINT("reset_utb_chip ...\r\n");
    (void)ql25_sys_reset_chip(ql25_ctrl);

    osal_delay_ms(1);
    UTB_PRINT("reset all phy reg...\r\n");
    ql25_reset_phy_all_reg(ql25_ctrl);
    return;
}

/* 去初始化芯片,此函数调用后hdle销毁; */
void utb_hal_deinit_chip(utb_hal_handle_t *hdle)
{
    if (!hdle || !hdle->private) {
        return;
    }
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;

    ql25_if_deinit_thread(ql25_ctrl);
    utb_hal_reset_chip(ql25_ctrl);

    /* 屏蔽所有中断 */
    (void)ql25_mac_set_all_int_en(ql25_ctrl, 0);

    /* 销毁软转发相关线程、定时器、信号量，以及申请的内存 */

    /* 其他模块相关线程、定时器、信号量，以及申请的内存 */

    uint32_t chip_id = ql25_ctrl->chip_id;    
    ql25_deinit_chip_ctrl(ql25_ctrl);
    g_ql25_ctrl[chip_id] = NULL;

    return;
}

int utb_hal_chip_hardware(utb_ql25_ctrl_t *ql25_ctrl, utb_chip_init_params_t *int_params)
{
    const char* func_name = "utb-init-chip-hw";
    utb_hal_reset_chip(ql25_ctrl);

    int ret = ql25_init_phy_selftest(ql25_ctrl);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s phy selftest failed\n", func_name);

    ret = ql25_mac_init_selftest(ql25_ctrl);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s mac selftest failed\n", func_name);

    ret = ql25_mac_init(ql25_ctrl);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s init mac failed\n", func_name);

    ret = ql25_reset_phy_all_reg(ql25_ctrl);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s reset all phy reg failed\n", func_name);

    /* 初始化phy的rx */
    ret = ql25_init_phy_rx(ql25_ctrl, int_params->nid, int_params->msl);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s init phy rx failed.\n", func_name);
    uint32_t rx_en = 1;
    uint32_t tx_en = 0;

    if (int_params->nid == 0) {
        ql25_params_t params = { 0 };
        ql25_gen_params(int_params->msl, int_params->tcm, &params);
        ret = ql25_mac_set_fixed_length(ql25_ctrl, params.nsd0);
        UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s set mac mcu failed.\n", func_name);

        ret = ql25_init_phy_tx(ql25_ctrl, int_params->nid, int_params->msl, int_params->tcm, &params);
        UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s init phy tx failed.\n", func_name);
        tx_en = 1;
    }

    ret = ql25_set_phy_txrx_en(ql25_ctrl, tx_en, rx_en);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s set phy txrx failed.\n", func_name);
    return 0;
}

int utb_hal_init_chip(utb_hal_handle_t *hdle, utb_chip_init_params_t *int_params)
{
    const char* func_name = "utb-init-chip";
    if (!hdle || !hdle->private || (int_params == NULL)) {
    	UTB_PRINT("%s param err.\n", func_name);
        return -1;
    }
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    int ret = utb_hal_chip_hardware(ql25_ctrl, int_params);
    if (ret != 0) {
    	UTB_PRINT("%s init hw failed\n", func_name);
        goto error;
    }

    ql25_ctrl->func_mng_callback = int_params->func_mng_callback;
    if (ql25_ctrl->func_mng_callback) {
        ql25_ctrl->rx_mng_thread_hd = ql25_if_init_thread(ql25_ctrl);
        if (ret != 0) {
            UTB_PRINT("%s init if thread failed\n", func_name);
            goto error;
        }
    }
    
    return 0;
error:
    utb_hal_deinit_chip(hdle);
    return -1;
}

bool ql25_check_reg_addr_is_valid(uint32_t addr) {
    if (addr < 2400) {
        return true;
    }
#if 0
    if (((addr >= QL25_SYS_BASE_ADDR) && (addr < (QL25_SYS_BASE_ADDR + 0x400))) ||
        ((addr >= QL25_MAC_BASE_ADDR) && (addr < (QL25_MAC_BASE_ADDR + 0x8000)))) {
        return true;
    }
#endif
    return false;
}

int utb_hal_read_reg(utb_hal_handle_t *hdle, uint32_t reg_addr, uint32_t *data)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    *data = *(uint32_t*)(ql25_ctrl->phy_reg_stub + reg_addr);
#if 0    
    if (!ql25_check_reg_addr_is_valid(reg_addr)) {
        return -1;
    }
    *data = (*((volatile uint32_t *)(reg_addr)));
#endif
    return 0;
}
int utb_hal_write_reg(utb_hal_handle_t *hdle, uint32_t reg_addr, uint32_t data)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    *(uint32_t*)(ql25_ctrl->phy_reg_stub + reg_addr) = data;
#if 0    
    if (!ql25_check_reg_addr_is_valid(reg_addr)) {
        return -1;
    }

    (*((volatile uint32_t *)(reg_addr))) = data;
#endif
    return 0;
}

/* todo，后续需要从硬件中读取 */
int utb_hal_get_mac_addr(utb_hal_handle_t * hdle, uint8_t *mac_addr)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);    
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);    

/* todo */
    return 0;
}

int utb_hal_get_chip_ver(utb_hal_handle_t *hdle, uint32_t *ver)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    return utb_hal_read_reg(hdle, 0xA020, ver); /* todo 目前没有给这个寄存器手册正式编码需要明确 */
}

/* 初始化slave的tx模块 */
int utb_hal_chip_slave_tx_init(utb_hal_handle_t *hdle)
{
    uint32_t mcm;
    uint8_t msl = 0;
    uint8_t tcm[8] = { 0 };
    const char* func_name = "utb-init-slave-tx";

    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);
    /* 获取接收到的msl */
    int ret = ql25_get_phy_rx_msl_tcm(ql25_ctrl, &msl, &mcm, tcm);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s get msl failed\n", func_name);

    ql25_params_t params = { 0 };
    ql25_gen_params(msl, tcm, &params);

    ret = ql25_init_phy_tx(ql25_ctrl, 0xff, msl, tcm, &params); /* 初始化slave tx的时候还没有上线，此时nid为0xff*/
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s init phy failed\n", func_name);

    ret = ql25_mac_set_fixed_length(ql25_ctrl, params.nsd0);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s set mac mcu %u failed\n", func_name, params.nsd0);

    ret = ql25_set_phy_txrx_en(ql25_ctrl, 1, 0);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s set phy txrx failed.\n", func_name);

    /* 作为slave确认了，就可以将与adp相关的线程关掉 */    
    //ql25_if_deinit_all_thread(ql25_ctrl);
    UTB_PRINT("%s end\r\n", func_name);

    return 0;
}

#if 0 /* todo */
/* 提供给用户调整msl、tcm的接口，配置后除msl、mcm外mcs、crc24等也要跟着重新配置 */
int utb_hal_set_msl(uint32_t chip_id, uint32_t msl, uint32_t *tcm)
{
    return 0;
}
#endif

int utb_hal_set_nid(utb_hal_handle_t *hdle, uint8_t nid, uint8_t msl, uint8_t *tcm)
{
    const char* func_name = "utb-set-nid";
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);

    int ret;
    if (nid == 0) {
        utb_chip_init_params_t int_params;
        int_params.nid = 0;
        int_params.spi_freq_cfg = 25000000;
        int_params.msl = msl;
        for (int i = 0; i < 8; i++) {
            int_params.tcm[i] = tcm[i];
        }
        ret = utb_hal_chip_hardware(ql25_ctrl, &int_params);
    } else {
        ret = ql25_set_phy_nid(ql25_ctrl, nid);
    }
    return ret;
}

int utb_hal_change_msl(utb_hal_handle_t *hdle, uint8_t msl, uint8_t *tcm)
{
    const char* func_name = "utb-change-msl";
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);

    uint8_t nid = 0;
    int ret = ql25_get_phy_nid(ql25_ctrl, &nid);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s get nid failed.\n", func_name);
    if (nid != 0) {
        UTB_PRINT("%s is not master, nid %u", func_name, nid);
        return -1;
    }

    utb_chip_init_params_t int_params;    
    int_params.nid = 0;
    int_params.spi_freq_cfg = 25000000;
    int_params.msl = msl;
    for (int i = 0; i < 8; i++) {
        int_params.tcm[i] = tcm[i];
    }
    
    return utb_hal_chip_hardware(ql25_ctrl, &int_params);
}

uint32_t utb_hal_get_crc(utb_hal_handle_t *hdle, uint32_t src_nid)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);
    return ql25_get_crc_cnt_of_nid(ql25_ctrl, src_nid);
}

/* 配置tx符号 */
int utb_hal_set_tx_symb_cfg(utb_hal_handle_t *hdle, utb_hal_symb_cfg_t *symb_cfg)
{
    const char* func_name = "utb-cfg-tx-symb";
    /* b25不考虑超帧 */
    if (!hdle || !hdle->private || !symb_cfg || (symb_cfg->slen <= 1) || ((symb_cfg->sidx + symb_cfg->slen) > 64)) {
        return -1;
    }
    UTB_PRINT("%s sidx %u slen %u\n", func_name, symb_cfg->sidx, symb_cfg->slen);

    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    int ret = ql25_set_phy_symb_cfg_range(ql25_ctrl, symb_cfg->sidx, symb_cfg->slen);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s sidx %u slen %u failed\n", func_name, symb_cfg->sidx, symb_cfg->slen);

    ret = ql25_set_sym2nid_config(ql25_ctrl, symb_cfg->sidx, symb_cfg->slen, symb_cfg->snid);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s set symb2nid sidx %u slen %u nid %u failed\n",
        func_name, symb_cfg->sidx, symb_cfg->slen, symb_cfg->snid);

    return 0;
}

/* 清tx符号 */
int utb_hal_erase_tx_symb_cfg(utb_hal_handle_t *hdle, utb_hal_symb_cfg_t *symb_cfg)
{
    const char* func_name = "utb-erase-tx-symb";
    if (!hdle || !hdle->private || !symb_cfg || (symb_cfg->slen <= 1) || ((symb_cfg->sidx + symb_cfg->slen) > 64)) {
        return -1;
    }
    UTB_PRINT("%s sidx %u slen %u\n", func_name, symb_cfg->sidx, symb_cfg->slen);

    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    int ret = ql25_set_phy_symb_cfg_erase(ql25_ctrl, symb_cfg->sidx, symb_cfg->slen);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s sidx %u slen %u failed\n", func_name, symb_cfg->sidx, symb_cfg->slen);
    ret = ql25_set_sym2nid_config(ql25_ctrl, symb_cfg->sidx, symb_cfg->slen, 0xff);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s set sym2nid sidx %u slen %u failed\n", func_name, symb_cfg->sidx, symb_cfg->slen);
    return 0;
}

/* 配置rx符号 */
int utb_hal_set_rx_symb_cfg(utb_hal_handle_t *hdle, utb_hal_symb_cfg_t *symb_cfg)
{
    const char* func_name = "utb-cfg-rx-symb";
    if (!hdle || !hdle->private || !symb_cfg || (symb_cfg->slen <= 1) || ((symb_cfg->sidx + symb_cfg->slen) > 64)) {
        return -1;
    }

    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    int ret = ql25_set_phy_rx_sfrm_ctrl(ql25_ctrl, symb_cfg->sidx, symb_cfg->slen);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s sidx %u slen %u failed\n", func_name, symb_cfg->sidx, symb_cfg->slen);
    ret = ql25_set_sym2nid_config(ql25_ctrl, symb_cfg->sidx, symb_cfg->slen, symb_cfg->snid);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s set symb2nid sidx %u slen %u nid %u failed\n",
        func_name, symb_cfg->sidx, symb_cfg->slen, symb_cfg->snid);
    return 0;
}
int utb_hal_erase_rx_symb_cfg(utb_hal_handle_t *hdle, utb_hal_symb_cfg_t *symb_cfg)
{
    const char* func_name = "utb-erase-rx-symb";
    if (!hdle || !hdle->private || !symb_cfg || (symb_cfg->slen <= 1) || ((symb_cfg->sidx + symb_cfg->slen) > 64)) {
        return -1;
    }

    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    int ret = ql25_erase_phy_rx_sfrm_ctrl(ql25_ctrl, symb_cfg->sidx, symb_cfg->slen);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s sidx %u slen %u failed\n", func_name, symb_cfg->sidx, symb_cfg->slen);
    ret = ql25_set_sym2nid_config(ql25_ctrl, symb_cfg->sidx, symb_cfg->slen, 0xff);
    UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s set sym2nid sidx %u slen %u failed\n", func_name, symb_cfg->sidx, symb_cfg->slen);
    return 0;
}

/* 查询slave同步情况 */
uint32_t utb_hal_get_rx_rsm(utb_hal_handle_t *hdle, uint32_t *detail_rsm)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, 0);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, 0);

    *detail_rsm = ql25_get_phy_rx_rsm(ql25_ctrl);
    if (*detail_rsm == 6) {
        return 1;
    }
    return 0;
}

uint32_t utb_hal_get_rx_fsm_err_flag(utb_hal_handle_t *hdle)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, 0);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, 0);
    return ql25_get_phy_rx_fsm_err_flag(ql25_ctrl);
}

uint32_t utb_hal_get_rx_frame_idx(utb_hal_handle_t *hdle)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, 0);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, 0);
    return ql25_get_rx_frame_idx(ql25_ctrl);
}

uint32_t utb_hal_get_pwrdiff_of_nid(utb_hal_handle_t *hdle, uint8_t nid)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, 0);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, 0);
    return ql25_get_pwrdiff_of_nid(ql25_ctrl, nid);
}

int utb_hal_get_tx_gain(utb_hal_handle_t *hdle, uint32_t *gain)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);
    return ql25_get_phy_tx_gain(ql25_ctrl, gain);
}

int utb_hal_set_tx_gain(utb_hal_handle_t *hdle, uint32_t gain)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);
    return ql25_set_phy_tx_gain(ql25_ctrl, gain);
}

int utb_hal_get_rx_gain(utb_hal_handle_t *hdle, uint32_t *gain)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);
    return ql25_get_phy_rx_afe_gain(ql25_ctrl, gain);
}

int utb_hal_set_rx_gain(utb_hal_handle_t *hdle, uint32_t gain)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);
    return ql25_set_phy_rx_afe_gain(ql25_ctrl, gain);
}

int utb_hal_get_band_num(utb_hal_handle_t *hdle, uint32_t *band_num)
{
    if (!hdle || !hdle->private || !band_num) {
        return -1;
    }
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    return ql25_get_phy_rx_nca_dem(ql25_ctrl, band_num);
}

int utb_hal_get_snr(utb_hal_handle_t *hdle, uint32_t band_num, double *snr)
{
    if (!hdle || !hdle->private || !snr) {
        return -1;
    }
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    return ql25_get_snr(ql25_ctrl, band_num, snr);
}

int utb_hal_get_mode(utb_hal_handle_t *hdle, uint8_t *msl, uint32_t *mcm)
{
    int ret = 0;
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);

    uint8_t tcm[8];
    return ql25_get_phy_rx_msl_tcm(ql25_ctrl, msl, mcm, tcm);
}

int utb_hal_set_phy_txrx_en(utb_hal_handle_t *hdle, utb_dir_t dir, uint32_t en)
{
    int ret = 0;
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);

    if (dir == UTB_DIR_TX) {
        ret = ql25_set_phy_tx_en(ql25_ctrl, en);
    } else if (dir == UTB_DIR_RX) {
        ret = ql25_set_phy_rx_en(ql25_ctrl, en);
    } else {
        ret |= ql25_set_phy_tx_en(ql25_ctrl, en);
        ret = ql25_set_phy_rx_en(ql25_ctrl, en);
    }
    return 0;
}

int utb_hal_set_spi_freq(utb_hal_handle_t *hdle, uint32_t spi_freq_cfg)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);
    return ql25_sys_set_spi_freq(ql25_ctrl, spi_freq_cfg);
}

void utb_hal_clr_cnt(utb_hal_handle_t *hdle)
{
    if (!hdle || !hdle->private) {
        return;
    }

    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    ql25_phy_clr_cnt(ql25_ctrl);
    (void)ql25_mac_clr_cnt(ql25_ctrl);
    /* todo 软复位清统计 */
}

int utb_hal_send_mng_to_adp(utb_hal_handle_t *hdle, char* data, int len)
{
    UTB_CHECK_PTR_AND_RETUN(hdle, -1);
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)hdle->private;
    UTB_CHECK_PTR_AND_RETUN(ql25_ctrl, -1);

    return ql25_send_msg_to_adp(ql25_ctrl, data, len);
}


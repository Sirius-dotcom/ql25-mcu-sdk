
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "ql25_pub.h"
#include "ql25_mac_drv.h"
#include "ql25_phy_drv.h"
#include "ql25_sys_drv.h"

static void ql25_dfx_proc_reslut(utb_ql25_ctrl_t *ql25_ctrl, utb_dfx_item_t *dfx_item, uint32_t rsl, char *detail)
{
    if (rsl == UTB_DFX_CHECK_OK) {
        dfx_item->err_cnt = 0;
        dfx_item->sts = 0;
        return;
    }

    if (dfx_item->err_cnt < dfx_item->threshold) {
        dfx_item->err_cnt++;
        return;
    }
    dfx_item->sts = dfx_item->action;
    if (detail == NULL) {
    	UTB_PRINT("%s check fail\n", dfx_item->item_name);
    } else {
    	UTB_PRINT("%s check fail, detail:%s\n", dfx_item->item_name, detail);
    }
}

void ql25_dfx_force_en(utb_ql25_ctrl_t *ql25_ctrl, uint32_t en)
{
    ql25_ctrl->dfx_info.chip_dfx_en = en;
}

static void ql25_dfx_check_mac(utb_ql25_ctrl_t *ql25_ctrl)
{
    uint32_t mtu_rsl = UTB_DFX_CHECK_OK;
    uint32_t fsm_rsl = UTB_DFX_CHECK_OK;

    if (ql25_get_phy_rx_rsm(ql25_ctrl) >= 6) {
        uint8_t int_sts[IRQID_MAX] = { 0 };
        ql25_mac_get_all_int(ql25_ctrl, int_sts);
        for (int i = IRQID_MCU2MAC_LEN_ERR; i < IRQID_MAX; i++) {
            if (int_sts[i] == 0) {
                continue;
            }

            if ((i == IRQID_MCU2MAC_LEN_ERR) || (i == IRQID_PHY2MAC_LEN_ERR) || (i == IRQID_MCU2MAC_LEN_ERR)) {
                mtu_rsl = UTB_DFX_CHECK_FAIL;
            } else {
                fsm_rsl = UTB_DFX_CHECK_FAIL;
            }
        }
    }

    utb_dfx_item_t *dfx_item = &ql25_ctrl->dfx_info.dfx_item[UTB_DFX_IDX_MAC_MTU];
    ql25_dfx_proc_reslut(ql25_ctrl, dfx_item, mtu_rsl, NULL);
    dfx_item = &ql25_ctrl->dfx_info.dfx_item[UTB_DFX_IDX_MAC_FSM];
    ql25_dfx_proc_reslut(ql25_ctrl, dfx_item, fsm_rsl, NULL);

    if ((mtu_rsl == UTB_DFX_CHECK_FAIL) || (fsm_rsl == UTB_DFX_CHECK_FAIL)) {
        ql25_mac_clr_all_int(ql25_ctrl);
    }
    return;
}

/* 检查0xec的bit0 slave_tx_flag,如果为0，则只有复位才能恢复 */
static void ql25_dfx_check_phy_tx(utb_ql25_ctrl_t *ql25_ctrl)
{
    utb_dfx_item_t *dfx_item = &ql25_ctrl->dfx_info.dfx_item[UTB_DFX_IDX_PHY_TX];
    uint32_t rsl = UTB_DFX_CHECK_OK;

    if (dfx_item->check_en) {
        /* rx同步状态没到6，phy会自杀，tx关闭，此时不检测tx */
        if (ql25_get_phy_rx_rsm(ql25_ctrl) >= 6) {
            uint32_t tx_fsm = ql25_get_phy_tx_flag(ql25_ctrl);
            rsl = (tx_fsm == 1) ? UTB_DFX_CHECK_OK : UTB_DFX_CHECK_FAIL;
        }
    }
    ql25_dfx_proc_reslut(ql25_ctrl, dfx_item, rsl, NULL);

    return;
}

/* 检查0x104的bit8 rx_fsm_err_flag,如果为0，则只有复位才能恢复 */
static void ql25_dfx_check_phy_rx(utb_ql25_ctrl_t *ql25_ctrl)
{
    /* 理论初始化之后rx就使能了，所以rx要一直检测 */
    uint32_t err_flag = ql25_get_phy_rx_fsm_err_flag(ql25_ctrl);
    uint32_t rsl = (err_flag == 0) ? UTB_DFX_CHECK_OK : UTB_DFX_CHECK_FAIL;

    utb_dfx_item_t *dfx_item = &ql25_ctrl->dfx_info.dfx_item[UTB_DFX_IDX_PHY_RX];
    ql25_dfx_proc_reslut(ql25_ctrl, dfx_item, rsl, NULL);
    return;
}

static void ql25_dfx_check_phy(utb_ql25_ctrl_t *ql25_ctrl)
{
    /* 检查0x104的bit8 rx_fsm_err_flag */
    ql25_dfx_check_phy_tx(ql25_ctrl);
    ql25_dfx_check_phy_rx(ql25_ctrl);
    return;
}

static void ql25_dfx_timer(union sigval sv)
{
    utb_ql25_ctrl_t *ql25_ctrl = (utb_ql25_ctrl_t*)sv.sival_ptr;
    utb_dfx_info_t *dfx_info = &ql25_ctrl->dfx_info;
    if (!dfx_info->chip_dfx_en) {
        return;
    }

    ql25_dfx_check_phy(ql25_ctrl);
    ql25_dfx_check_mac(ql25_ctrl);

    uint32_t action = UTB_DFX_ACT_NONE;
    for (int i = 0; i < UTB_DFX_IDX_MAX; i++) {
        utb_dfx_item_t *dfx_item = &dfx_info->dfx_item[i];
        if (dfx_item->sts > action) {
            action = dfx_item->action;
        }
    }

    if (action != dfx_info->chip_dfx_action) {
        dfx_info->chip_dfx_action = action;
        if (!dfx_info->func_dfx_callback) {
            dfx_info->func_dfx_callback(ql25_ctrl, action);
        }
    }
    return;
}


void ql25_dfx_start(utb_ql25_ctrl_t *ql25_ctrl)
{
    utb_dfx_item_t items[UTB_DFX_IDX_MAX] = {
        { "phy-tx", 3, UTB_DFX_RESET_CHIP, 0, 0, 0, 0, 0, 0},
        { "phy-rx", 3, UTB_DFX_RESET_CHIP, 0, 0, 0, 0, 0, 0},
        { "mac-mtu", 3, UTB_DFX_ACT_NONE, 0, 0, 0, 0, 0, 0},
        { "mac-fsm", 3, UTB_DFX_WARN, 0, 0, 0, 0, 0, 0},
    };

    utb_dfx_info_t *dfx_info = &ql25_ctrl->dfx_info;
    utb_os_timer_init_t init_cfg = {
        .delay = 0,
        .interval = 3000,
        .param = ql25_ctrl,
        .func_timer_callback = ql25_dfx_timer,
    };

    dfx_info->chip_dfx_en = 0; /* 默认dfx不使能，等待芯片启动后再使能 */
    memcpy(dfx_info->dfx_item, items, sizeof(items));
    dfx_info->dfx_hd = utb_os_timer_create(&init_cfg);
}


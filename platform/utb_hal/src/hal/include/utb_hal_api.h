/*
 * Copyright (c) 2025 QuickLink Ltd.
 *
 * Author: zhaowei <zhaowei@xlianzn.com>
 *
 * UTB Driver struct definition
 */
 
#ifndef UTB_HAL_API_H
#define UTB_HAL_API_H

#include "utb_os_api.h"

typedef enum {
    UTB_DIR_TX = 0,
    UTB_DIR_RX = 1,
    UTB_DIR_TXRX = 2,
} utb_dir_t;

#define UTB_DFX_CHECK_OK    0
#define UTB_DFX_CHECK_FAIL  1

typedef enum {
    UTB_DFX_IDX_PHY_TX,
    UTB_DFX_IDX_PHY_RX,
    UTB_DFX_IDX_MAC_MTU,
    UTB_DFX_IDX_MAC_FSM,
    UTB_DFX_IDX_MAX,
} utb_dfx_idx_t;

typedef enum {
    UTB_DFX_ACT_NONE,
    UTB_DFX_WARN,
    UTB_DFX_RESET_CHIP,
} utb_dfx_action_t;

typedef void (*dfx_callback_t)(void *hdle, utb_dfx_action_t action);
typedef void (*mng_callback_t)(uint32_t chip_id, char *data, int len);

/* 初始化芯片 */
typedef struct {
    uint8_t nid;          /* 0：表示为master，其他表示为slave */
    uint8_t msl;          /* master传入，slave不关心 */
    uint8_t tcm[8];       /* master传入，slave不关心 */
    uint8_t rsv[2];
    uint32_t spi_freq_cfg; /* spi频率，传0表示默认值(25M) */
    dfx_callback_t func_dfx_callback; /* DFX回调函数 */    
    mng_callback_t func_mng_callback; /* adpter发过来的管理消息，回调函数 */
} utb_chip_init_params_t;

typedef struct {
    char     item_name[16];
    uint8_t  threshold;
    uint8_t  action;
    uint8_t  err_cnt;
    uint8_t  sts;
    uint8_t  check_en;
    uint8_t  rsv[3];
} utb_dfx_item_t;

typedef struct {
    utb_os_timer_handle_t *dfx_hd;    
    uint16_t chip_dfx_en;
    uint16_t chip_dfx_action;
    utb_dfx_item_t dfx_item[UTB_DFX_IDX_MAX];
    dfx_callback_t func_dfx_callback;
} utb_dfx_info_t;

typedef struct {
    void *private;
} utb_hal_handle_t;

typedef struct {
    uint32_t snid;   /* 符号对应的nid */
    uint32_t sidx;   /* 起始符号id */
    uint32_t slen;   /* 信道长度 */
    uint32_t mode;   /* 超帧模式(待扩展，v1r1仅支持单帧) */
    uint32_t sub_band_num;  /* 信道号(待扩展) */
} utb_hal_symb_cfg_t;

/* 功能：获取芯片的hal句柄
   入参：芯片id
   出参：无
   返回值：utb_hal_handle_t句柄，需上层保存，无需关注内容，后续使用句柄访问HAL其他接口 */
utb_hal_handle_t *utb_hal_open_chip(uint32_t chip_id);

/* 功能：销毁芯片的hal句柄
   入参：hal句柄
   出参：无
   返回值：无 */
void utb_hal_close_chip(utb_hal_handle_t *hdle);

/* 功能：初始化芯片，内部含芯片复位、TX、RX使能
   入参：hal句柄;初始化参数
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_init_chip(utb_hal_handle_t *hdle, utb_chip_init_params_t *int_params);

/* 功能：销毁芯片
   入参：hal句柄
   出参：无
   返回值：无 */
void utb_hal_deinit_chip(utb_hal_handle_t *hdle);


/* 功能：获取芯片mac地址
   入参：hal句柄
   出参：mac地址
   返回值：0:成功;other:失败 */
int utb_hal_get_mac_addr(utb_hal_handle_t * hdle, uint8_t *mac_addr);

/* 功能：获取芯片版本号
   入参：hal句柄
   出参：版本号
   返回值：0:成功;other:失败 */
int utb_hal_get_chip_ver(utb_hal_handle_t *hdle, uint32_t *ver);

/* 功能：slave同步后，初始化tx
   入参：hal句柄
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_chip_slave_tx_init(utb_hal_handle_t *hdle);

/* 功能：调整msl、tcm
   入参：hal句柄; msl; tcm
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_set_msl(utb_hal_handle_t *hdle, uint8_t msl, uint8_t *tcm);

/* 功能：设置nid(slave分配到nid或按照slave启动的芯片被告知需要按照master模式启动时调用)
   入参：hal句柄; nid; msl与tcm仅nid为0时需传入，nid不为0时可以传0;
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_set_nid(utb_hal_handle_t *hdle, uint8_t nid, uint8_t msl, uint8_t *tcm);

/* 功能：设置tx符号
   入参：hal句柄; 符号信息;
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_set_tx_symb_cfg(utb_hal_handle_t *hdle, utb_hal_symb_cfg_t *symb_cfg);

/* 功能：清tx符号
   入参：hal句柄; 符号信息;
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_erase_tx_symb_cfg(utb_hal_handle_t *hdle, utb_hal_symb_cfg_t *symb_cfg);

/* 功能：设置rx符号
   入参：hal句柄; 符号信息;
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_set_rx_symb_cfg(utb_hal_handle_t *hdle, utb_hal_symb_cfg_t *symb_cfg);

/* 功能：清rx符号
   入参：hal句柄; 符号信息;
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_erase_rx_symb_cfg(utb_hal_handle_t *hdle, utb_hal_symb_cfg_t *symb_cfg);

/* 功能：获取rx同步状态
   入参：hal句柄
   出参：具体的同步状态
   返回值：0:未同步;1:同步 */
uint32_t utb_hal_get_rx_rsm(utb_hal_handle_t *hdle, uint32_t *detail_rsm);

/* 功能：获取rx状态机是否有故障
   入参：hal句柄
   出参：无
   返回值：0:正常;1：有故障 */
uint32_t utb_hal_get_rx_fsm_err_flag(utb_hal_handle_t *hdle);
uint32_t utb_hal_get_rx_frame_idx(utb_hal_handle_t *hdle);
uint32_t utb_hal_get_pwrdiff_of_nid(utb_hal_handle_t *hdle, uint8_t nid);

/* 功能：获取tx增益
   入参：hal句柄
   出参：增益值
   返回值：0:成功;other:失败 */
int utb_hal_get_tx_gain(utb_hal_handle_t *hdle, uint32_t *gain);

/* 功能：设置tx增益
   入参：hal句柄; 增益值
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_set_tx_gain(utb_hal_handle_t *hdle, uint32_t gain);

/* 功能：获取rx增益
   入参：hal句柄
   出参：增益值
   返回值：0:成功;other:失败 */
int utb_hal_get_rx_gain(utb_hal_handle_t *hdle, uint32_t *gain);

/* 功能：设置rx增益
   入参：hal句柄; 增益值
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_set_rx_gain(utb_hal_handle_t *hdle, uint32_t gain);

/* 功能：读寄存器
   入参：hal句柄; 寄存器地址
   出参：寄存器值
   返回值：0:成功;other:失败 */
int utb_hal_read_reg(utb_hal_handle_t *hdle, uint32_t reg_addr, uint32_t *data);

/* 功能：写寄存器
   入参：hal句柄; 寄存器地址; 寄存器值
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_write_reg(utb_hal_handle_t *hdle, uint32_t reg_addr, uint32_t data);

/* 功能：获取子带数目
   入参：hal句柄
   出参：子带数目
   返回值：0:成功;other:失败 */
int utb_hal_get_band_num(utb_hal_handle_t *hdle, uint32_t *band_num);

/* 功能：获取指定子带的snr
   入参：hal句柄; 自带号
   出参：snr值
   返回值：0:成功;other:失败 */
int utb_hal_get_snr(utb_hal_handle_t *hdle, uint32_t band_num, double *snr);

/* 功能：更改msl\tcm配置
   入参：hal句柄; msl、tcm配置
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_change_msl(utb_hal_handle_t *hdle, uint8_t msl, uint8_t *tcm);

/* 功能：获取来自指定源nid的crc统计
   入参：hal句柄; 源nid
   出参：无
   返回值：crc统计计数 */
uint32_t utb_hal_get_crc(utb_hal_handle_t *hdle, uint32_t src_nid);

/* 功能：清芯片统计
   入参：hal句柄
   出参：无
   返回值：无 */
void utb_hal_clr_cnt(utb_hal_handle_t *hdle);

/* 由于芯片不允许重复使能，所以函数内部会先关后开 */
int utb_hal_set_phy_txrx_en(utb_hal_handle_t *hdle, utb_dir_t dir, uint32_t en);

/* 功能：清芯片统计
   入参：hal句柄; spi频率   0: 100M 1: 200M 
   出参：无
   返回值：0:成功;other:失败 */
int utb_hal_set_spi_freq(utb_hal_handle_t *hdle, uint32_t spi_freq_cfg);

int utb_hal_get_mode(utb_hal_handle_t *hdle, uint8_t *msl, uint32_t *mcm);

/* 诊断命令行api的入参、出参说明
   dbg_str由上层用户申请内存后传入，len为传输内存长度
   SDK在接口中奖诊断信息写入dbg_str, 返回值为写入数据的长度
*/
int utb_hal_dbg_read_reg(utb_hal_handle_t *hdle, char *dbg_str, int len, uint32_t addr, uint32_t cnt);
int utb_hal_dbg_mac_dump(utb_hal_handle_t *hdle, char *dbg_str, int len, uint32_t filt);
int utb_hal_dbg_phy_dump(utb_hal_handle_t *hdle, char *dbg_str, int len, uint32_t filt);
int utb_hal_dbg_get_phy_rx_state(utb_hal_handle_t *hdle, char *dbg_str, int len);
int utb_hal_dbg_display_cnt(utb_hal_handle_t *hdle, char *dbg_str, int len, uint32_t filt);
int utb_hal_dbg_get_snr(utb_hal_handle_t *hdle, char *dbg_str, int len);
int utb_hal_dbg_all_syms_cfg(utb_hal_handle_t *hdle, char *dbg_str, int len);
int utb_hal_send_mng_to_adp(utb_hal_handle_t *hdle, char* data, int len);

#endif //UTB_HAL_API_H

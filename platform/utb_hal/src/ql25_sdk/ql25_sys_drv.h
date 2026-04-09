/*
 * Copyright (c) 2025 QuickLink Ltd.
 *
 * Author: zhaowei <zhaowei@xlianzn.com>
 *
 * UTB Driver struct definition
 */
 
#ifndef QL25_SYS_DRIVER_H
#define QL25_SYS_DRIVER_H

int ql25_sys_reset_chip(utb_ql25_ctrl_t *ql25_ctrl);
int ql25_sys_set_spi_freq(utb_ql25_ctrl_t *ql25_ctrl, uint32_t spi_freq_cfg);
int ql25_sys_get_spi_filt_num(utb_ql25_ctrl_t *ql25_ctrl, uint32_t *filt_num);
int ql25_sys_set_spi_filt_num(utb_ql25_ctrl_t *ql25_ctrl, uint32_t filt_num);

#endif //ql25_SYS_DRIVER_H

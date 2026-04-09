
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ql25_pub.h"
#include "ql25_sys_drv.h"
#include "ql25_sys_reg.h"

int ql25_write_sys(utb_ql25_ctrl_t *ql25_ctrl, uint32_t reg_offset_addr, uint32_t data)
{
    *(uint32_t*)(ql25_ctrl->sys_reg_stub + reg_offset_addr) = data;
#if 0
    (*((volatile uint32_t *)(UTB_SYS_BASE + reg_offset_addr))) = data;
#endif
    return 0;
}

int ql25_read_sys(utb_ql25_ctrl_t *ql25_ctrl, uint32_t reg_offset_addr, uint32_t *data)
{
    *data = *(uint32_t*)(ql25_ctrl->sys_reg_stub + reg_offset_addr);
#if 0
    *data = (*((volatile uint32_t *)(UTB_SYS_BASE + reg_offset_addr)));
#endif
    return 0;
}

int ql25_sys_reset_chip(utb_ql25_ctrl_t *ql25_ctrl)
{
    chip_sw_rst_t reg_value = { 0 };
    reg_value.reg.chip_sw_rst_ctr = 0x1f;
    return ql25_write_sys(ql25_ctrl, offsetof(utb_sys_regs_t, chip_sw_rst), reg_value.value);
}

int ql25_sys_set_spi_freq(utb_ql25_ctrl_t *ql25_ctrl, uint32_t spi_freq_cfg)
{
    if (spi_freq_cfg > 1) {
        return -1;
    }

    spi_freq_cfg_t reg_value = { 0 };
    int ret = ql25_read_sys(ql25_ctrl, offsetof(utb_sys_regs_t, spi_freq_cfg), &reg_value.value);
    UTB_CHECK_RET_AND_RETUN(ret);

    reg_value.reg.spi_freq_cfg = spi_freq_cfg;
    return ql25_write_sys(ql25_ctrl, offsetof(utb_sys_regs_t, spi_freq_cfg), reg_value.value);
}

int ql25_sys_get_spi_filt_num(utb_ql25_ctrl_t *ql25_ctrl, uint32_t *filt_num)
{
    spi_freq_cfg_t reg_value = { 0 };
    int ret = ql25_read_sys(ql25_ctrl, offsetof(utb_sys_regs_t, spi_freq_cfg), &reg_value.value);
    UTB_CHECK_RET_AND_RETUN(ret);

    *filt_num = reg_value.reg.spi_filt_num;

    return ret;
}

int ql25_sys_set_spi_filt_num(utb_ql25_ctrl_t *ql25_ctrl, uint32_t filt_num)
{
    spi_freq_cfg_t reg_value = { 0 };
    int ret = ql25_read_sys(ql25_ctrl, offsetof(utb_sys_regs_t, spi_freq_cfg), &reg_value.value);
    UTB_CHECK_RET_AND_RETUN(ret);

    reg_value.reg.spi_filt_num = filt_num;
    return ql25_write_sys(ql25_ctrl, offsetof(utb_sys_regs_t, spi_freq_cfg), reg_value.value);
}


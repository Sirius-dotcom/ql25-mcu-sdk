

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "ql25_pub.h"
#include "ql25_mac_drv.h"
#include "ql25_mac_reg.h"
#include "log.h"
#include "utb_os_i2c.h"

int ql25_write_mac(utb_ql25_ctrl_t *ql25_ctrl, uint32_t reg_offset_addr, uint32_t data)
{
    *(uint32_t*)(ql25_ctrl->mac_reg_stub + reg_offset_addr) = data;;
#if 0
    (*((volatile uint32_t *)(QL25_MAC_BASE_ADDR + reg_offset_addr))) = data;
#endif    
    return 0;
}

int ql25_read_mac(utb_ql25_ctrl_t *ql25_ctrl, uint32_t reg_offset_addr, uint32_t *data)
{
    *data = *(uint32_t*)(ql25_ctrl->mac_reg_stub + reg_offset_addr);
#if 0
    *data = (*((volatile uint32_t *)(QL25_MAC_BASE_ADDR + reg_offset_addr)));
#endif
    return 0;
}

int ql25_mac_set_fixed_length(utb_ql25_ctrl_t *ql25_ctrl, uint32_t frag_size)
{
    mac_fixed_length_t reg_value = { 0 };
    reg_value.reg.fixed_length = frag_size;
    reg_value.reg.mac_timer = 0x8000;
    return ql25_write_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_fixed_length), reg_value.value);
}

void ql25_mac_set_all_int_en(utb_ql25_ctrl_t *ql25_ctrl, uint32_t en)
{
    uint32_t reg_value = 0;
    if (!en) {
        reg_value = 0x1FFFFFF;
    }
    (void)ql25_write_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_int_clr_mask), reg_value);
}

void ql25_mac_clr_all_int(utb_ql25_ctrl_t *ql25_ctrl)
{
    (void)ql25_write_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_int_clr_mask), 0xFFFCFFFC);
    (void)ql25_write_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_int_clr_mask), 0);
}

void ql25_mac_get_all_int(utb_ql25_ctrl_t *ql25_ctrl, uint8_t *int_sts)
{
    uint32_t reg_value = 0;
    (void)ql25_read_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_int_status), &reg_value);
    for (int i = 0; i < IRQID_MAX; i++) {
        int_sts[i] = reg_value & 0x1;
        reg_value = reg_value >> 1;
    }
}

int ql25_mac_init(utb_ql25_ctrl_t *ql25_ctrl)
{
    int ret = ql25_write_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_int_clr_mask), 0x1FFFFFF);
    UTB_CHECK_RET_AND_RETUN(ret);

    ret = ql25_write_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_num_clr), 0x1FFF);
    UTB_CHECK_RET_AND_RETUN(ret);
    ret = ql25_write_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_int_clr_mask), 0);
    UTB_CHECK_RET_AND_RETUN(ret);
    ret = ql25_write_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_num_clr), 0);
    UTB_CHECK_RET_AND_RETUN(ret);

    mac_ctrl_t reg_info = { 0 };
    reg_info.reg.mac_en = 1;
    ret = ql25_write_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_ctrl), reg_info.value);
    UTB_CHECK_RET_AND_RETUN(ret);

    return 0;
}

int ql25_mac_init_selftest(utb_ql25_ctrl_t *ql25_ctrl)
{
    const char* func_name = "utb-mac-selftest";
    uint32_t reg[] = { 0xa5a5a5a5, 0x5a5a5a5a, 0xffffffff, 0};
    for (uint32_t i = 0; i < (sizeof(reg) / sizeof(uint32_t)); i++) {        
        int ret = ql25_write_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_fixed_length), reg[i]);
        UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s write 0x%x failed\n", func_name, reg[i]);

        uint32_t value = 0;
        ret = ql25_read_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_fixed_length), &value);
        UTB_CHECK_RET_LOG_AND_RETUN(ret, "%s read failed\n", func_name);

        if (value != reg[i]) {
            log_error("%s write 0x%x read-value 0x%x\n", func_name, reg[i], value);
            return -1;
        }
    }
    
    return 0;
}

int ql25_mac_clr_cnt(utb_ql25_ctrl_t *ql25_ctrl)
{
    return ql25_write_mac(ql25_ctrl, offsetof(ql25_mac_reg_t, mac_num_clr), 0xFFFFFFFF);
}


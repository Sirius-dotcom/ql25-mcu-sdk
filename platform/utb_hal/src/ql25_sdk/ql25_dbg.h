/*
 * Copyright (c) 2025 QuickLink Ltd.
 *
 * Author: zhaowei <zhaowei@xlianzn.com>
 *
 * UTB Driver struct definition
 */
 
#ifndef QL25_DBG_H
#define QL25_DBG_H

#include "utb_hal_api.h"
#include "utb_os_api.h"

#define UTB_CHECK_LEN(len, use_len) if (use_len >= len) return len;
#define BITS_PER_LONG 32
#define GENMASK(h, l) (((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

typedef struct {
    uint8_t  reg_disp; //1,显示此计数; 0,不显示
    uint8_t  reg_type; //1,read addr; 0,don't read
    uint8_t  bit_end;
    uint8_t  bit_start;
    uint16_t addr_start;
    uint16_t addr_end;
    char*    name;
} utb_dump_reg_info_t;

#define UTB_DBG_CHECK_PARAMS_AND_RETURN(hdle, dbg_str, len) \
    do { \
        if (!(hdle) || !(dbg_str) || (len) <= 0) { \
            return 0; \
        } \
    } while(0)

#endif //QL25_DBG_H

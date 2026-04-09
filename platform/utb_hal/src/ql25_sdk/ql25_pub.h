/*
 * Copyright (c) 2025 QuickLink Ltd.
 *
 * Author: zhaowei <zhaowei@xlianzn.com>
 *
 * UTB Driver struct definition
 */
 
#ifndef QL25_PUB_H
#define QL25_PUB_H

#include <stdint.h> 
#include <stdio.h>
#include "utb_hal_api.h"
#include "utb_os_api.h"

#ifndef offsetof
/*
* 计算成员field在type类型中的地址偏移量
*/
#define offsetof(type, field)  ((size_t) &( ((type*)0)->field))
#endif
#define UTB_CHECK_RET_AND_RETUN(ret) \
    do { \
        if ((ret) != 0) { \
            return (ret); \
        } \
    } while(0)

#define UTB_CHECK_PTR_AND_RETUN(ptr, ret) \
    do { \
        if ((ptr) == 0) { \
            return (ret); \
        } \
    } while(0)

#define UTB_CHECK_RET_LOG_AND_RETUN(ret, fmt, ...) \
    do { \
        if ((ret) != 0) { \
            UTB_PRINT(fmt, ##__VA_ARGS__); \
            return (ret); \
        } \
    } while(0)

#define UTB_CHECK_RET_LOG_AND_GOTO(ret, error, fmt, ...) \
do { \
    if ((ret) != 0) { \
        UTB_PRINT(fmt, ##__VA_ARGS__); \
        goto error; \
    } \
} while(0)

    
/* 以下宏需要用户根据硬件实际情况自行修改 */
#define UTB_CHIP_NUM               1                   /* 设备上UTB芯片的数量 */
#define UTB_SPI_DEV_NAME_0         "/dev/spidev0.0"    /* 芯片0 spi口设备名称 */
#define UTB_I2C_ADDR_0             0x50                /* 芯片0 i2c slave地址 */
    
/* 多芯片时需要配置 */
#define UTB_SPI_DEV_NAME_1      "/dev/spidev0.1"    /* 芯片1 spi口设备名称 */    
#define UTB_SPI_DEV_NAME_2      "/dev/spidev0.2"    /* 芯片2 spi口设备名称 */    
#define UTB_SPI_DEV_NAME_3      "/dev/spidev0.3"    /* 芯片3 spi口设备名称 */

typedef struct {
    uint32_t chip_id;
    utb_dfx_info_t dfx_info;

    mng_callback_t func_mng_callback; /* adpter发过来的管理消息，回调函数 */
    utb_thread_handle_t *rx_mng_thread_hd;
    int data_soc_fd;
    int mng_soc_fd;    
    uint32_t pre_destroy;
    void *mac_reg_stub;
    void *phy_reg_stub;
    void *sys_reg_stub;
} utb_ql25_ctrl_t;


bool ql25_check_reg_addr_is_valid(uint32_t addr);
#endif //QL25_PUB_H

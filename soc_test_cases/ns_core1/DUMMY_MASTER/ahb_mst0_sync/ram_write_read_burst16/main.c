/*
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include "ns_sdk_hal.h"

#define MAIN_FAB
#define main_fab_num 11
addr_xlen slv_base[]={0x30000000,0x30020000,0x30040000,0x30060000,0x18000000,0x18004000,0x18008000,0x18010000,0x13000000,0x13004000,0x13008000,};
addr_xlen slv_size[]={0x20000,0x20000,0x10000,0x10000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,0x4000,};

#define BURST_LEN           (16)
#define BURST_NUM           (4)
#define DATA_LEN            (BURST_LEN * BURST_NUM)

#define DMS_BASE            (AHB_MST0_SYNC_BASE)

#define DMS_TX_CFG0         (0x0) /* tx trans start address */

#define DMS_TX_CFG1         (0x4)

#define DMS_TX_CFG2         (0x8)
#define DMS_TX_ENABLE       (0x1 << 0)

#define DMS_RX_CFG0         (0xC) /* rx trans start address */

#define DMS_RX_CFG1         (0x10)

#define DMS_RX_CFG2         (0x14)
#define DMS_RX_ENABLE       (0x1 << 0)

#define DMS_INTR_EN         (0x18)
#define DMS_ERR_ENABLE      (0x1 << 0)
#define DMS_TXINT_ENABLE    (0x1 << 1)
#define DMS_RXINT_ENABLE    (0x1 << 2)

#define DMS_STA             (0x1C)
#define DMS_ERR_FLAG        (0x1 << 0)
#define DMS_TX_FLAG         (0x1 << 1)
#define DMS_RX_FLAG         (0x1 << 2)

#define DMS_VER             (0x20)

#define DMS_TX_DATA32       (0x24)
#define DMS_RX_DATA32       (0x28)
#define DMS_TX_DATA64       (0x2C)
#define DMS_RX_DATA64       (0x30)
#define DMS_TX_DATA96       (0x34)
#define DMS_RX_DATA96       (0x38)
#define DMS_TX_DATA128      (0x3C)
#define DMS_RX_DATA128      (0x40)
#define DMS_TX_DATA160      (0x44)
#define DMS_RX_DATA160      (0x48)
#define DMS_TX_DATA192      (0x4C)
#define DMS_RX_DATA192      (0x50)
#define DMS_TX_DATA224      (0x54)
#define DMS_RX_DATA224      (0x58)
#define DMS_TX_DATA256      (0x5C)
#define DMS_RX_DATA256      (0x60)

#define DMS_TRANS_CG        (0xa4)

static inline void dms_tx_addr(uint32_t addr)
{
    *(uint32_t*)(DMS_BASE + DMS_TX_CFG0) = addr;
}

static inline void dms_tx_enable(void)
{
    *(uint32_t*)(DMS_BASE + DMS_TX_CFG2) |= DMS_TX_ENABLE;
}

static inline void dms_tx_disable(void)
{
    *(uint32_t*)(DMS_BASE + DMS_TX_CFG2) &= ~DMS_TX_ENABLE;
}

static inline void dms_rx_addr(uint32_t addr)
{
    *(uint32_t*)(DMS_BASE + DMS_RX_CFG0) = addr;
}

static inline void dms_rx_enable(void)
{
    *(uint32_t*)(DMS_BASE + DMS_RX_CFG2) |= DMS_RX_ENABLE;
}

static inline void dms_rx_disable(void)
{
    *(uint32_t*)(DMS_BASE + DMS_RX_CFG2) &= ~DMS_RX_ENABLE;
}

static inline void dms_irq_enable(int irq)
{
    *(uint32_t*)(DMS_BASE + DMS_INTR_EN) |= irq;
}

static inline void dms_irq_disable(int irq)
{
    *(uint32_t*)(DMS_BASE + DMS_INTR_EN) = irq;
}

static inline void dms_wait_status(int status)
{
    while(1) {
        if (status & *(uint32_t*)(DMS_BASE + DMS_STA)) {
            *(uint32_t*)(DMS_BASE + DMS_STA) = status;
            break;
        }
    }
}

static inline void dms_init(uint32_t burst, uint32_t burst_num)
{
    uint32_t reg = 0;
    reg |= burst;
    reg |= burst_num << 8;
    *(uint32_t*)(DMS_BASE + DMS_TRANS_CG) = reg;
}

static inline void dms_read_word(uint32_t* addr, uint32_t len, uint32_t* buf)
{
    dms_rx_addr(ADDR32(addr));
    dms_rx_enable();
    dms_wait_status(DMS_RX_FLAG);
    while(len) {
        if (len > 0) {
            *buf++ = *(uint32_t*)(DMS_BASE + DMS_RX_DATA32);
            len -= 1; addr += 1;
        }
    }
}

static inline void dms_write_word(uint32_t* addr, uint32_t len, uint32_t* buf)
{
    dms_tx_addr(ADDR32(addr));
    while(len) {
        if (len > 0) {
            *(uint32_t*)(DMS_BASE + DMS_TX_DATA32) = *buf++;
            len -= 1; addr += 1;
        }
    }
    dms_tx_enable();
    dms_wait_status(DMS_TX_FLAG);
}

ErrStatus MemoryCompare(uint32_t* src, uint32_t* dst, uint32_t length)
{
    while (length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

uint32_t buf_src[DATA_LEN] = {0};
uint32_t buf_dst[DATA_LEN] = {0};

void main(void)
{
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
    DisableDCache();
    #endif
    printf("dummy master ctest\n");
    #if (defined MAIN_FAB) || (defined PER_FAB)
    uint32_t *wraddr __ALIGNED(BURST_LEN*4) = (uint32_t *)(slv_base[0]);
    dms_init(BURST_LEN-1, BURST_NUM-1);
    *wraddr = 0;
    for (int i = 0;i < DATA_LEN;i++){
        buf_src[i] = i + 1;
    }
    dms_write_word(wraddr, DATA_LEN, buf_src);
    dms_read_word(wraddr, DATA_LEN, buf_dst);
    if (SUCCESS == MemoryCompare(buf_src, buf_dst, DATA_LEN)) {
        simulation_pass();
    } else {
        simulation_fail();
    }
    #else
    simulation_pass();
    #endif

    while(1) {}
}

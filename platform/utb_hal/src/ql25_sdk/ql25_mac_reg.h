/*
 * Copyright (c) 2025 QuickLink Ltd.
 *
 * Author: zhaowei <zhaowei@xlianzn.com>
 *
 * UTB Driver struct definition
 */
 
#ifndef QL25_MAC_REG_H
#define QL25_MAC_REG_H

#include <stdint.h> 

#define QL25_MAC_BASE_ADDR            (0x18000000UL)

typedef struct {
    volatile uint32_t mac_ctrl;                 /* 0x0000 reg 1   32 [31:17] rsv 0x0 RO  直接寻址    保留位
                                                    [16:16] mac_en  0x0 RW  直接寻址    MAC工作使能，高有效
                                                    [15:0]  ahb_symb_timer  0x7FF   RW  直接寻址    AHB总线切片读写定时器阈值，TX/RX共用
                                                */
    volatile uint32_t mac_tx_status;            /* 0x0004 reg 1   32 [31:22] rsv 0x0 RO  直接寻址    保留位
                                                    [21:12] ahb_tx_byte_cnt 0x0 RO  直接寻址    TX方向接受一个切片时，AHB写入的数据字节数
                                                    [11:2]  mac_tx_byte_cnt 0x0 RO  直接寻址    "TX方向发送一个切片时，mac_tx当前发送的数据字节数
                                                    当mac_tx挂死时，该寄存器表示已经发送出去的字节树
                                                    当mac_tx空闲时，该寄存器为0"
                                                    [1:0]   mac_tx_state    0x0 RO  直接寻址    "mac tx状态机：
                                                        0x0：IDLE
                                                        0x1：READ
                                                        0x2：DONE
                                                        0x3：保留位"
                                                */
    volatile uint32_t mac_rx_status;            /* 0x0008 reg 1   32 [31:22] rsv 0x0 RO  直接寻址    保留位
                                                    [21:12] ahb_rx_byte_cnt 0x0 RO  直接寻址    RX方向发送一个切片时，AHB读取的数据字节数
                                                    [11:2]  mac_rx_byte_cnt 0x0 RO  直接寻址    RX方向接收一个切片时，mac_rx当前收到的数据字节数
                                                    [1:0]   mac_rx_state    0x0 RO  直接寻址    "mac rx状态机：
                                                        0x0：IDLE
                                                        0x1：WRITE
                                                        0x2：CRC
                                                        0x3：na"
                                                */
    volatile uint32_t mac_fixed_length;         /* 0x000c reg 1   32  [31:16] fixed_length    0x26    RW  直接寻址    全局统一的分片长度
                                                    [15:0]  mac_timer   0x0FA0  RW  直接寻址    mac超时复位定时器
                                                */
    volatile uint32_t tx_fifo_state;            /* 0x0010 reg 1   32  [31:16] tx_fifo_available_slices    0x3F    RO  直接寻址    tx fifo 剩余可存的分片数
                                                    [15:0]  rx_fifo_committed_slices    0x0 RO  直接寻址    rx fifo 已经存放的分片数
                                                */
    volatile uint32_t rx_fifo_state;            /* 0x0014 reg 1   32  [31:16] rx_fifo_available_slices    0x7F    RO  直接寻址    rx fifo 剩余可存的分片数
                                                    [15:0]  tx_fifo_committed_slices    0x0 RO  直接寻址    tx fifo 已经存放的分片数    
                                                */
    volatile uint32_t mac_int_status;           /* 0x0018 reg 1   32  [31:16] rsv 0x0 RO  直接寻址    
                                                    [15:0]  mac_int_status  0x1 RO  直接寻址    "[15:0]:分别对应的中断：
                                                    [0]：MCU可以写入AHB切片
                                                    [1]：MCU可以读取AHB切片
                                                    [2]：保留位
                                                    [3]：PHY发来的长度不对
                                                    [4]：TX FIFO满写
                                                    [5]：PHY向mac请求数据的长度不对
                                                    [6]：RX FIFO满写
                                                    [7]：TX状态机超时
                                                    [8]：RX状态机超时
                                                    [9]：PHY RX超时中断
                                                    [10]：AHB写切片超时
                                                    [11]：AHB读切片超时"    
                                                */
    volatile uint32_t mac_int_clr_mask;         /* 0x001c reg 1   32  [31:16] mac_int_mask    0x0 RW  直接寻址    mac中断的mask，定义和上面一致，高表示屏蔽中断
                                                    [15:0]  mac_int_clear   0x0 RWC 直接寻址    mac中断的clear，定义和上面一致，写1清除中断状态    
                                                */
    volatile uint32_t mac_crc_num;              /* 0x0020 reg 1   32  [31:0]  mac_crc_0_num   0x0 RO  直接寻址    mac统计crc为0的报文个数 */
    volatile uint32_t mac_num_clr;              /* 0x0024 reg 1   32  
                                                    [31:25] reserve 0x0 RO  直接寻址    保留位
                                                    [24:24] ahb_rx_timeout_num_clr  0x0 RWC 直接寻址    MAC发送到MCU超时次数复位
                                                    [23:23] rx_fifo_overflow_symb_num_clr   0x0 RWC 直接寻址    RX_FIFO满写的符号个数复位
                                                    [22:22] phy_req_len_wrong_num_clr   0x0 RWC 直接寻址    MAC TX的phy_req_len和fix_length不一致的符号个数复位
                                                    [21:20] reserve1    0x0 RO  直接寻址    保留位
                                                    [19:19] mac_fifo_dbg_clr    0x0 RWC 直接寻址    mac的TX FIFO和RX FIFO的调试统计寄存器mac_tx_fifo_dbg和mac_rx_fifo_dbg清零
                                                    [18:18] mcu_len_right_num_clr   0x0 RWC 直接寻址    mac统计MCU发送的长度正确的报文个数复位
                                                    [17:15] reserve3    0x0 RO  直接寻址    保留位
                                                    [14:14] valid_slice_num_2phy_clr    0x0 RWC 直接寻址    mac统计发送到PHY的有效数据报文个数复位
                                                    [13:12] reserve2    0x0 RO  直接寻址    保留位
                                                    [11:11] valid_slices_to_mcu_num_clr 0x0 RWC 直接寻址    从rx fifo读出来发送到MCU的切片个数复位
                                                    [10:9]  reserve1    0x0 RO  直接寻址    保留位
                                                    [8:8]   rx_timeout_wrong_num_clr    0x0 RWC 直接寻址    mac rx状态机超时的报文个数复位
                                                    [7:7]   tx_timeout_wrong_num_clr    0x0 RWC 直接寻址    mac tx状态机超时的报文个数复位
                                                    [6:4]   reserve0    0x0 RO  直接寻址    保留位
                                                    [3:3]   phy_len_wrong_num_clr   0x0 RWC 直接寻址    mac统计phy发来的长度不对的报文个数复位
                                                    [2:2]   mcu_len_wrong_num_clr   0x0 RWC 直接寻址    mac统计MCU发送超时的报文个数复位
                                                    [1:1]   mac_crc_1_num_clr   0x0 RWC 直接寻址    mac统计crc为1的报文个数复位
                                                    [0:0]   mac_crc_0_num_clr   0x0 RWC 直接寻址    mac统计crc为0且非空的报文个数复位    
                                                */
    volatile uint32_t phy_len_wrong_num;        /* 0x0028 reg 1   32  [31:0]  phy_len_wrong_num   0x0 RO  直接寻址    mac统计phy发来的长度不对的报文个数 */
    volatile uint32_t tx_full_write_wrong_num;  /* 0x002c reg 1   32  [31:0]  tx_full_write_wrong_num 0x0 RO  直接寻址    mac统计TX FIFO满写的字节个数 */
    volatile uint32_t rx_full_write_wrong_num;  /* 0x0030 reg 1   32  [31:0]  rx_full_write_wrong_num 0x0 RO  直接寻址    mac统计RX FIFO满写的字节个数
                                                */
    volatile uint32_t rx_timeout_wrong_num;     /* 0x0034 reg 1   32  [31:0]  status_timeout_wrong_num    0x0 RO  直接寻址    状态机超时的报文个数*/
    volatile uint32_t empty_slices_from_phy_num;/* 0x0038 reg 1   32  [31:0]  empty_slices_from_phy_num   0x0 RO  直接寻址    从PHY收到的空切片个数，空切片判断：前8个字节都是0且总长度大于8B且crc校验成功的切片*/
    volatile uint32_t empty_slices_to_mcu_num;  /* 0x003c reg 1   32  [31:0]  empty_slices_to_mcu_num 0x0 RO  直接寻址    因rx fifo空发送到MCU的空数据片个数             */
    volatile uint32_t invalid_reg;              /* 0x0040 废弃 */
    volatile uint32_t mcu_len_right_num;        /* 0x0044 reg 1   32  [31:0]  mcu_len_right_num   0x0 RO  直接寻址    mac统计MCU发送的长度正确的报文个数 */
    volatile uint32_t total_slice_num_2phy;     /* 0x0048 reg 1   32  [31:0]  total_slice_num_2phy    0x0 RO  直接寻址    mac统计发送到PHY的总共数据报文个数 */
    volatile uint32_t slice_num_2phy;           /* 0x004c reg 1   32  [31:0]  empty_slice_num_2phy    0x0 RO  直接寻址    mac统计发送到PHY的空数据报文个数 */
    volatile uint32_t tx_fifo_state1;           /* 0x0050 reg 1   32  [31:29] rsv 0x0 RO  直接寻址    保留位
                                                    [28:16] tx_unwriten_bytes   0x1000  RO  直接寻址    tx fifo 剩余可写的字节数
                                                    [15:0]  rx_writen_bytes 0x0 RO  直接寻址    rx fifo 已经写入的字节数    
                                                */
    volatile uint32_t rx_fifo_state1;           /* 0x0054 reg 1   32 
                                                    [31:16] rx_unwriten_bytes   0x1000  RO  直接寻址    rx fifo 剩余可写的字节数
                                                    [15:13] rsv 0x0 RO  直接寻址    保留位
                                                    [12:0]  tx_writen_bytes 0x0 RO  直接寻址    tx fifo 已经写入的字节数    
                                                */
    volatile uint32_t mac_crc_1_num;            /* 0x0058 reg 1   32  [31:0]  mac_crc_1_num   0x0 RO  直接寻址    mac统计crc为1的报文个数 */
    volatile uint32_t mcu_len_wrong_num;        /* 0x005c reg 1   32  [31:0]  mcu_len_wrong_num   0x0 RO  直接寻址    mac统计MCU发送的长度不对的报文个数 */
    volatile uint32_t all_slices_from_phy_num;  /* 0x0060 reg 1   32  [31:0]  all_slices_from_phy_num 0x0 RO  直接寻址    从PHY收到的所有切片个数 */
    volatile uint32_t valid_slices_to_mcu_num;  /* 0x0064 reg 1   32  [31:0]  valid_slices_to_mcu_num 0x0 RO  直接寻址    从rx fifo读出来发送到MCU的切片个数 */
    volatile uint32_t mcu_vld_slice_num;        /* 0x0068 reg 1   32  [31:0]  mcu_vld_slice_num   0x0 RO  mac统计MCU发送的报文个数（都是有效的）*/
    volatile uint32_t valid_slice_num_2phy;     /* 0x006c reg 1   32  [31:0]  valid_slice_num_2phy    0x0 RO  直接寻址    mac统计发送到PHY的有效数据报文个数 */
    volatile uint32_t tx_timeout_wrong_num;     /* 0x0070 reg 1   32  [31:0]  tx_timeout_wrong_num    0x0 RO  直接寻址    mac tx状态机超时的报文个数 */
    volatile uint32_t mac_cfg;                  /* 0x0074 reg 1   32  [31:1]  rsv 0x0 RO  直接寻址    保留位
                                                    [0:0]   phy_abort_mac_en    0x0 RW  直接寻址    PHY中断MAC的使能位，高有效
                                                */
    volatile uint32_t rcv[2];                   /* 0x0078 0x0079保留寄存器 */
    volatile uint32_t mac_tx_fifo_dbg0;         /* 0x0080 reg 1   32  [31:16] commit_rbin_diff    0x0 RO  直接寻址    读commit时，当前指针和上一次commit指针之差
                                                    [15:0]  commit_wbin_diff    0x0 RO  直接寻址    写commit时，当前指针和上一次commit指针之差
                                                */
    volatile uint32_t mac_tx_fifo_dbg1;         /* 0x0084 reg 1   32  [31:16] rcommit_fail_num    0x0 RO  直接寻址    读commit时，指针之差不等于一个切片的次数
                                                    [15:0]  wcommit_fail_num    0x0 RO  直接寻址    写commit时，指针之差不等于一个切片的次数
                                                */
    volatile uint32_t mac_tx_fifo_dbg2;         /* 0x0088 reg 1   32  [31:0]  wcommit_pass_num    0x0 RO  直接寻址    读commit时，指针之差等于一个切片的次数 */
    volatile uint32_t mac_tx_fifo_dbg3;         /* 0x008c reg 1   32  [31:0]  rcommit_pass_num    0x0 RO  直接寻址    写commit时，指针之差等于一个切片的次数 */
    volatile uint32_t mac_tx_fifo_dbg4;         /* 0x0090 reg 1   32  [31:0]  rsv 0x0 RO  直接寻址    保留位 */
    volatile uint32_t mac_tx_fifo_dbg5;         /* 0x0094 reg 1   32  [31:0]  rsv 0x0 RO  直接寻址    保留位 */
    volatile uint32_t mac_tx_fifo_dbg6;         /* 0x0098 reg 1   32  [31:0]  rsv 0x0 RO  直接寻址    保留位 */
    volatile uint32_t mac_tx_fifo_dbg7;         /* 0x009c reg 1   32  [31:0]  rsv 0x0 RO  直接寻址    保留位 */
    volatile uint32_t mac_rx_fifo_dbg0;         /* 0x00a0 reg 1   32  [31:16] commit_rbin_diff    0x0 RO  直接寻址    读commit时，当前指针和上一次commit指针之差
                                                    [15:0]  commit_wbin_diff    0x0 RO  直接寻址    写commit时，当前指针和上一次commit指针之差
                                                */
    volatile uint32_t mac_rx_fifo_dbg1;         /* 0x00a4 reg 1   32  [31:16] rcommit_fail_num    0x0 RO  直接寻址    读commit时，指针之差不等于一个切片的次数
                                                    [15:0]  wcommit_fail_num    0x0 RO  直接寻址    写commit时，指针之差不等于一个切片的次数
                                                */
    volatile uint32_t mac_rx_fifo_dbg2;         /* 0x00a8 reg 1   32  [31:0]  wcommit_pass_num    0x0 RO  直接寻址    读commit时，指针之差等于一个切片的次数 */
    volatile uint32_t mac_rx_fifo_dbg3;         /* 0x00ac reg 1   32  [31:0]  rcommit_pass_num    0x0 RO  直接寻址    写commit时，指针之差等于一个切片的次数 */
    volatile uint32_t mac_rx_fifo_dbg4;         /* 0x00b0 reg 1   32  [31:0]  rsv 0x0 RO  直接寻址    保留位 */
    volatile uint32_t mac_rx_fifo_dbg5;         /* 0x00b4 reg 1   32  [31:0]  rsv 0x0 RO  直接寻址    保留位 */
    volatile uint32_t mac_rx_fifo_dbg6;         /* 0x00b8 reg 1   32  [31:0]  rsv 0x0 RO  直接寻址    保留位 */
    volatile uint32_t mac_rx_fifo_dbg7;         /* 0x00bc reg 1   32  [31:0]  rsv 0x0 RO  直接寻址    保留位 */
    volatile uint32_t rcv2;                     /* 0x00c0 保留  */
    volatile uint32_t phy_req_len_wrong_num;    /* 0x00c4 reg 1   32  [31:0]  phy_req_len_wrong_num   0x0 RO  直接寻址    MAC TX的PHY请求长度和fix_length不一致的符号个数 */
    volatile uint32_t rx_fifo_overflow_symb_num;/* 0x00c8 reg 1   32  [31:0]  rx_fifo_overflow_symb_num   0x0 RO  RX_FIFO满写的符号个数 */
    volatile uint32_t ahb_rx_timeout_num;       /* 0x00cc reg 1   32  [31:0]  ahb_rx_timeout_num   0x0 RO  MAC发送到MCU的超时次数 */
    volatile uint32_t mac_tx_symb;              /* 0x0100 reg 1   32  [31:0]  mac_tx_symb 0x0 WO  直接寻址    MCU通过AHB向MAC写入发送的符号数据 */
    volatile uint32_t mac_rx_symb;              /* 0x0104 reg 1   32  [31:0]  mac_rx_symb 0x0 RO  直接寻址    MCU通过AHB从MAC读出接收的符号数据 */
} ql25_mac_reg_t;

typedef union {
    struct {
#if IS_LITTLE_ENDIAN
        uint32_t rsv : 15;                   /* [31:17]  直接寻址    保留位 */
        uint32_t mac_en : 1;                 /* [16:16] 直接寻址    MAC工作使能，高有效 */
        uint32_t ahb_symb_timer : 16;        /* [15:0] 直接寻址    AHB总线切片读写定时器阈值，TX/RX共用 */
#else
        uint32_t ahb_symb_timer : 16;        /* [15:0] 直接寻址    AHB总线切片读写定时器阈值，TX/RX共用 */
        uint32_t mac_en : 1;                 /* [16:16] 直接寻址    MAC工作使能，高有效 */
        uint32_t rsv : 15;                   /* [31:17]  直接寻址    保留位 */
#endif
    } reg;
    uint32_t value;
} mac_ctrl_t;                                    

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t fixed_length : 16;          /* 16:31 */
        uint32_t mac_timer : 16;             /* 0 :15 */
#else
        uint32_t mac_timer : 16;             /* 0 :15 */
        uint32_t fixed_length : 16;          /* 16:31 */
#endif
    } reg;
    uint32_t value;
} mac_fixed_length_t;

#endif //QL25_MAC_REG_H

/*
 * Copyright (c) 2025 QuickLink Ltd.
 *
 * Author: zhaowei <zhaowei@xlianzn.com>
 *
 * UTB Driver struct definition
 */
 
#ifndef QL25_SYS_REG_H
#define QL25_SYS_REG_H

#define QL25_SYS_BASE_ADDR            (0x200000UL)

typedef struct {
    volatile uint32_t spi_freq_cfg;         /* Offset: 0xA000 (R/W)  TX queue packet cfg0 
                                            [31:8]  rsv 0x0 RO  直接寻址    保留位
                                            [7:4]   spi_filt_num    0x1 RW  直接寻址    "SPI接口输入信号过滤周期数：
                                            4‘d0：过滤1拍
                                            4‘d1：过滤2拍
                                            ......
                                            4‘d15：过滤16拍
                                            建议优先配置为1或者2"
                                            [3:2]   rsv 0x0 RO  直接寻址    保留位
                                            [1:0]   spi_freq_cfg    0x1 RW  直接寻址    "SPI接口时钟配置：
                                            [0]=1'b1：200MHz
                                            [0]=1'b0：100MHz
                                            [1]：保留位"
                                            */
    volatile uint32_t chip_sw_rst;          /* Offset: 0xA004 (R/W)  TX queue packet cfg1 
                                            [31:8]  rsv 0x0 RO  直接寻址    保留位
                                            [7:1]   chip_sw_rst_ctr 0x1F    RW  直接寻址    软复位时间，系统时钟的周期数
                                            [0:0]   chip_sw_rst 0x1 RW  直接寻址    芯片软复位，写0复位
                                            */
    volatile uint32_t afe_cfg0;             /* Offset: 0xA008 (R/W)  TX queue write request length 
                                            [31:31] afe_sel 0x0 RW  直接寻址    "AFE版本选择：
                                            1'b1：选择使用老版本AFE
                                            1'b0：选择使用新版本AFE"
                                            [30:8]  rsv 0x0 RO  直接寻址    保留位
                                            [7:7]   afe_cfg0_entres 0x0 RW  直接寻址    新版本AFE的entres
                                            [6:6]   afe_cfg0_envc   0x1 RW  直接寻址    新版本AFE的envc
                                            [5:0]   afe_cfg0_Dbagp  0x20    RW  直接寻址    新版本AFE的Dbagp[5:0]
                                            */
    volatile uint32_t afe_cfg1;             /* Offset: 0xA00C (R/W)  response of TX queue write request 
                                            [30:16] rsv 0x0 RO  直接寻址    保留位
                                            [15:10] afe_cfg1_dbgapl[5:0]    0x20    RW  直接寻址    老版本AFE的dbgapl[5:0]
                                            [9:9]   afe_cfg1_enldol 0x0 RW  直接寻址    老版本AFE的enldol
                                            [8:8]   afe_cfg1_envcrxl    0x0 RW  直接寻址    老版本AFE的envcrxl
                                            [7:4]   afe_cfg1_rsrxl[3:0] 0x8 RW  直接寻址    老版本AFE的rsrxl[3:0]
                                            [3:2]   afe_cfg1_rsin[1:0]  0x3 RW  直接寻址    老版本AFE的rsin[1:0]
                                            [1:1]   afe_cfg1_endr50l    0x0 RW  直接寻址    老版本AFE的endr50l
                                            [0:0]   afe_cfg1_enadcl 0x0 RW  直接寻址    老版本AFE的enadcl
                                            */
    volatile uint32_t afe_cfg2;             /* Offset: 0xA010 (R/W)  TX queue write data 
                                            [30:16] rsv 0x0 RO  直接寻址    保留位
                                            [15:15] afe_cfg2_envfpl 0x0 RW  直接寻址    老版本AFE的envfpl
                                            [14:14] afe_cfg2_enrxl  0x0 RW  直接寻址    老版本AFE的enrxl
                                            [13:13] afe_cfg2_endr1kl    0x0 RW  直接寻址    老版本AFE的endr1kl
                                            [12:12] afe_cfg2_entxl  0x0 RW  直接寻址    老版本AFE的entxl
                                            [11:11] afe_cfg2_resdacl    0x0 RW  直接寻址    老版本AFE的resdacl
                                            [10:8]  afe_cfg2_txsell[2:0]    0x0 RW  直接寻址    老版本AFE的txsell[2:0]
                                            [7:7]   afe_cfg2_enpll192l  0x0 RW  直接寻址    老版本AFE的enpll192l
                                            [6:6]   afe_cfg2_enpll250l  0x0 RW  直接寻址    老版本AFE的enpll250l
                                            [5:5]   afe_cfg2_entres 0x1 RW  直接寻址    老版本AFE的entres
                                            [4:4]   rsv 0x0 RW  直接寻址    保留位
                                            [3:0]   afe_cfg2_rstxl[3:0] 0x8 RW  直接寻址    老版本AFE的rstxl[3:0]
                                            */
    volatile uint32_t utb_adc_vld;          /* Offset: 0xA014 (R/W)  RX queue packet cfg0 
                                            [31:1]  rsv 0x0 RO  直接寻址    保留位
                                            [0:0]   utb_adc_vld 0x0 RW  直接寻址    "AFE的ADC输入数据有效标记
                                            1’b0：ADC输入无效
                                            1‘b1：ADC输入有效"
                                            */
    volatile uint32_t pll_clk25_en;         /* Offset: 0xA018 (R/W)  RX queue packet cfg1 
                                            [31:6]  rsv 0x0 RO  直接寻址    保留位
                                            [5:5]   mac_test_en_rx  0x0 RW  直接寻址    "MAC RX测试IO输出使能，高有效
                                            控制MAC的测试IO第1~10个"
                                            [4:4]   mac_test_en_tx  0x0 RW  直接寻址    "MAC TX测试IO输出使能，高有效
                                            控制MAC的测试IO第11~30个"
                                            [3:3]   mac_test_en_spi 0x0 RW  直接寻址    "MAC SPI测试IO输出使能，高有效
                                            控制MAC的测试IO第31~44个"
                                            [2:2]   phy_test_en 0x0 RW  直接寻址    PHY测试IO输出使能，高有效
                                            [1:1]   afe_test_en 0x0 RW  直接寻址    "AFE测试IO使能信号
                                            1’b0：工作模式
                                            DADCL[9:0]从AFE输出到PHY
                                            CKADCL从AFE输出到PHY
                                            DDACL[11:0]从PHY输出到AFE
                                            CKDACL从PHY输出到AFE
                                            SEL[6:0]从PHY输出到AFE
                                            1‘b1：测试模式
                                            DADCL[9:0]输出到IO，PHY的输入接0
                                            CKADCL输出到IO，PHY的输入接0
                                            DDACL[11:0]来自IO输入
                                            CKDACL来自IO输入
                                            SEL[6:0]来自IO输入"
                                            [0:0]   pll_clk25_en    0x0 RW  直接寻址    "PLL分频时钟25MHz门控使能
                                            1’b0：关闭时钟
                                            1‘b1：打开时钟"
                                            */
    volatile uint32_t wakeup_status;        /* Offset: 0xA01C (R/W)  RX queue read ack length 
                                            [31:3]  rsv 0x0 RO  直接寻址    保留位
                                            [2:2]   pwr_up2 0x0 RO  直接寻址    当前唤醒源是MCU，高有效
                                            [1:1]   pwr_up1 0x0 RO  直接寻址    当前唤醒源是MCU，高有效
                                            [0:0]   yp  0x0 RO  直接寻址    当前唤醒源是双绞线，高有效
                                            */
} utb_sys_regs_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t rsv_31_8 : 24;            /* [31:8] */
        uint32_t spi_filt_num : 4;         /* [4:4] */
        uint32_t rsv_3_2 : 2;              /* [3:2] */
        uint32_t spi_freq_cfg : 2;         /* [0:1] */
#else
        uint32_t spi_freq_cfg : 2;         /* [0:1] */
        uint32_t rsv_3_2 : 2;              /* [3:2] */
        uint32_t spi_filt_num : 4;         /* [4:4] */
        uint32_t rsv_31_8 : 24;            /* [31:8] */
#endif
    } reg;
    uint32_t value;
} spi_freq_cfg_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t rsv : 24;                 /* [31:8] */
        uint32_t chip_sw_rst_ctr : 7;      /* [7:1] */
        uint32_t chip_sw_rst : 1;          /* [0:0] */
#else
        uint32_t chip_sw_rst : 1;          /* [0:0] */
        uint32_t chip_sw_rst_ctr : 7;      /* [7:1] */
        uint32_t rsv : 24;                 /* [31:8] */
#endif
    } reg;
    uint32_t value;
} chip_sw_rst_t;

#endif //QL25_SYS_REG_H

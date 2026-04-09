/*
 * Copyright (c) 2025 QuickLink Ltd.
 *
 * Author: zhaowei <zhaowei@xlianzn.com>
 *
 * UTB Driver struct definition
 */
 
#ifndef QL25_PHY_REG_H
#define QL25_PHY_REG_H

#define QL25_PHY_BASE_ADDR            (0x18004000UL)

typedef struct {

    volatile uint32_t PHY_Configuration;             /* Offset: 0x0000_0000,整体配置参数  使能    
                                                        [31:25] reserve_31_25   0x0 RO      
                                                        [24:24] rx_auto_recover 0x0 RW  直接寻址    针对从节点，当从节点自杀后，会定期自动尝试接入以再次进入状态6
                                                        [23:21] reserve_23_21   0x0 RO      
                                                        [20:20] utb_enable  0x0 RW  直接寻址    "名称：UTB enable 
                                                        介绍：UTB使能
                                                        取值：
                                                        1:    UTB enable  高电平使能
                                                        1 ：UTB disable"
                                                        [19:17] reserve_19_17   0x0 RO      
                                                        [16:16] tx_pre      RWC 直接寻址    "名称：Tx Request signal pulse. Only for Master Node.
                                                        介绍：只有主节点可设置，并且只设置2次。TX发射机收到该脉冲信号会进入发送准备阶段。
                                                        该参数在tx enable之后，rx enable之前配置。以便主节点Tx 和 Rx 尽可能同步
                                                        取值：
                                                        高跳脉冲：Tx进入准备阶段"
                                                        [15:13] reserve_15_13   0x0 RO      
                                                        [12:12] rx_enable       RWC 直接寻址    "名称：RX enable pluse
                                                        介绍：脉冲信号。用于表示打开Rx接收机，此信号优先级高于rx_disable
                                                        取值：
                                                        高跳脉冲：Tx接收机开启"
                                                        [11:9]  reserve_11_9    0x0 RO      
                                                        [8:8]   tx_enable       RWC 直接寻址    "名称：Tx enable pluse
                                                        介绍：脉冲信号，设置此位为2，表示打开Tx发射机
                                                        取值：
                                                        高跳脉冲：Tx发射机开启"
                                                        [7:5]   reserve_7_5 0x0 RO      
                                                        [4:4]   rx_disable      RWC 直接寻址    "名称：Rx disable pluse
                                                        介绍：脉冲信号。用于表示关闭Rx接收机，接收机相关关状态和资源被清除，接收机被关闭
                                                        取值：
                                                        高跳脉冲：Rx接收机关闭"
                                                        [3:1]   reserve_3_1 0x0 RO      
                                                        [0:0]   tx_disable      RWC 直接寻址    "名称：Tx disable pluse
                                                        介绍：脉冲信号，设置此位为2，表示关闭TX发射机。
                                                        发射机相关状态和资源被清除，发射机被关闭。
                                                        取值：
                                                        高跳脉冲：Tx发射机关闭"
                                                        */
    volatile uint32_t PHY_Mode_Configuration;        /* Offset: 0x0000_0004,整体配置参数
                                                        [31:12] reserve_31_12   0x0 RO      
                                                        [11:10] reserve_11_10   0x0 RW  直接寻址    "名称：Mode of symbol length
                                                        介绍：符号长度模式(numerology)：fft length=128*(2^msl)
                                                        取值：0，1，2，3"
                                                        [9:8]   msl             
                                                        [7:6]   "reserve_7_6
                                                        "   0x0 RO      
                                                        [5:4]   "
                                                        tran_mode"      RW  直接寻址    "名称：Mode of transmission
                                                        介绍：传输模式选择
                                                        取值：
                                                        0：基本传输模式(dcf)
                                                        1：实时传输模式(fcf)
                                                        2：连续传输模式(scf)"
                                                        [3:1]   reserve_3_1 0x0 RO      
                                                        [0:0]   cc_type 0x0 RW  直接寻址    "名称：The type of contention channel
                                                        介绍：信道竞争类型。
                                                        取值：
                                                        0:固定优先级
                                                        1:循环优先级"
                                                     */
    volatile uint32_t PHY_Enc_Mcs;                   /* Offset: 0x0000_0008, PHY 编码调制选择寄存器
                                                        [31:24]]    reserve_31_24   0x0 RO      
                                                        [23:0]  mcs 0x0 RW  直接寻址    "名称：Mode of Modulation  coding selection
                                                        介绍：调制编码模式选择
                                                        取值：TBD
                                                        对应算法C代码中的Tcm[0-8]"
                                                     */
    volatile uint32_t PHY_Enc_CRC24;                 /* Offset: 0x0000_000C, 
                                                        [31:24] reserve_31_24   0x0 RO      
                                                        [23:0]  crc24   0x0 RW  直接寻址    "名称:   CRC-24
                                                        介绍：24位宽的CRC校验码
                                                        取值：TBD"
                                                     */
    volatile uint32_t PHY_Sbb_Configuration;         /* Offset: 0x0000_0010
                                                        [31:8]  reserve_31_8    0x0 RO  直接寻址    
                                                        [7:4]   sub_num 0x0 RW  直接寻址    "名称：The number of subband
                                                        介绍：子信道所包含子带数量
                                                        取值：TBD"
                                                        [3:3]   reserve_3   0x0 RO      
                                                        [2:0]   sub_start   0x0 RW  直接寻址    "名称：The starting position of the subband for channel allocation
                                                        介绍：信道分配子带的起始位置
                                                        取值：TBD"
                                                     */
    volatile uint32_t PHY_MSN_Configuration;         /* Offset: 0x0000_0014, 主/从 & 竞争信道配置参数
                                                        [31:5]  reserve_31_5    0x0 RO      
                                                        [4:4]   node_type   0x0 RW  直接寻址    "名称：Node Mode
                                                        介绍：TX发送端使用此寄存器所对应字段来确定主、从节点。在TX端，该值为0时，当RX接收状态机处于DATA状态或者MNODE状态时，为主节点方式。
                                                        RX接收端不使用该寄存器作为主从判断。
                                                        取值：
                                                        1: 为主节点，
                                                        0: 为从节点。"
                                                        [3:0]   sup_frm_order   0x0 RW  直接寻址    "名称：The order of super order
                                                        介绍：节点数量模数,一个共享业务超帧包含2^Mnn帧， Mnn取值范围是0~5-Msl
                                                        取值：范围0 -- (5-msl)"
                                                     */
    volatile uint32_t PHY_CC_Configuration;          /* Offset: 0x0000_0018,
                                                        [31:12] reserve_31_12       RO      
                                                        [11:9]  reserve_11_9    0x0 RO      
                                                        [8:8]   cci     RW  直接寻址    "名称：Channel Contention Identification
                                                        介绍：信道竞争标志
                                                        取值：
                                                        0：不进行信道竞争
                                                        1：进行信道竞争"
                                                        [7:0]   nid 0x0 RW  直接寻址    "名称：Node ID,  node  identification
                                                        介绍：节点ID号
                                                        发送端TX：RLT代码中采用tx_nid参与vcf竞争符号的处理，并没有使用全0的方式做为是否主从节点的判断。
                                                        接收端RX：模块内部nid全0，对应i_mnode为1，主节点标识；模块内部nid非全0，对应i_mnode为0，从节点标识。
                                                        取值："
                                                     */

    volatile uint32_t PHY_OFDMC_Configuration1;      /* Offset: 0x0000_001C, 控制符号配置参数
                                                        [31:20] reserve_31_20       RO      
                                                        [19:19] reserve_19  0x0 RO      
                                                        [18:16] ncb_rs_ctrl     RW  直接寻址    "名称：number of rs coded block by a control symbol
                                                        介绍：一个控制 OFDM 符号承载的 RS 编码块数量
                                                        取值：TBD"
                                                        [15:14] reserve_15_14   0x0 RO      
                                                        [13:0]  ncoded_b_ctrl       RW  直接寻址    "名称：number of coded bits by a control symbol
                                                        介绍：一个控制 OFDM 符号承载的编码比特长度
                                                        取值：TBD"
                                                     */
    volatile uint32_t PHY_OFDMC_Configuration2;      /* Offset: 0x0000_0020, 
                                                        [31:24] reserve_31_24       RO      
                                                        [23:18] reserve_23_18   0x0 RO      
                                                        [17:8]  nbytes_ctrl     RW  直接寻址    "名称：number of coded bytes by a control symbol
                                                        介绍：一个控制 OFDM 符号承载的字节数,RTL的修改随新版UTB用户手册一同进行。届时TX和RX的Nrs位宽统一为11bit
                                                        取值：TBD"
                                                        [7:0]   nrs_ctrl    0x0 RW  直接寻址    "名称：number of  rs by a control symbol
                                                        介绍：控制 OFDM 符号的 RS 编码块长度，RTL的修改随新版UTB用户手册一同进行。届时TX和RX的Nrs位宽统一为8bit
                                                        取值：TBD"
                                                     */    
    volatile uint32_t PHY_OFDMD_Configuration1;      /* Offset: 0x0000_0024, 数据符号配置参数
                                                        [31:20] reserve_31_20       RO      
                                                        [19:19] reserve_20  0x0 RO      
                                                        [18:16] ncb_data        RW  直接寻址    "名称：number of rs coded block by a data symbol
                                                        介绍：一个数据 OFDM 符号承载的 RS 编码块数量
                                                        取值：TBD"
                                                        [15:14] reserve_15_14   0x0 RO      
                                                        [13:0]  ncoded_b_data       RW  直接寻址    "名称：number of coded bits by a data symbol
                                                        介绍：一个数据 OFDM 符号承载的编码比特长度
                                                        取值：TBD"
                                                     */
    volatile uint32_t PHY_OFDMD_Configuration2;      /* Offset: 0x0000_0028, 
                                                        [31:24] reserve_31_24       RO      
                                                        [23:18] reserve_23_18   0x0 RO      
                                                        [17:8]  nbytes_data     RW  直接寻址    "名称：number of coded bytes by a data symbol
                                                        介绍：一个数据 OFDM 符号承载的字节数
                                                        取值：TBD"
                                                        [7:0]   nrs_data    0x0 RW  直接寻址    "名称：number of  rs by a data symbol
                                                        介绍：数据OFDM符号的RS编码块长度
                                                        取值：TBD"
                                                     */
    volatile uint32_t PHY_Rx_Configuration;          /* Offset: 0x0000_002C, RX配置参数
                                                        [31:25] reserve_31_25       RO      
                                                        [24:24] rx_mode 0x0 RW  直接寻址    "名称：receive mode
                                                        介绍：接收模式选择
                                                        取值：
                                                        0：初始化模式
                                                        1：正常接收模式"
                                                        [23:16] npa_cnt_ctrl    0x0 RW  直接寻址    "名称：noise power abnormal maximum times
                                                        介绍：数据解调异常的次数上限,对应算法c文件rxsm.c的Qsd，默认值为16
                                                        取值：TBD"
                                                        [15:8]  d_err_cnt_ctrl  0x0 RW  直接寻址    "名称：data demodulaiton error maximum times
                                                        介绍：调制解调异常的次数上限,对应算法c文件rxsm.c的Qcd，默认值为16
                                                        取值：TBD"
                                                        [7:7]   reserve_7       RO      
                                                        [6:4]   cc_det_thr  0x0 RW  直接寻址    "名称：Rx channel contention detection threshold
                                                        介绍：竞争信道检测门限,
                                                        取值：默认值为4，取值范围0~7"
                                                        [3:3]   reserve_3       RO      
                                                        [2:0]   scc_cnt_max 0x0 RW  直接寻址    "名称：Rx Sharable  Contention Channel Maximum counter
                                                        介绍：最大可竞争共享信道数量
                                                        取值：TBD"
                                                     */
    volatile uint32_t PHY_Rx_NND;                    /* Offset: 0x0000_0030, 
                                                        [31:24] reserve_31_24       RO      
                                                        [23:0]  rx_nnd  0x0 RW  直接寻址    "名称：receive node to node delay 
                                                        介绍：本节点相对管理节点的时间延迟时钟数，8.16d，8整数位，16小数位置
                                                        取值：取值范围：0~8*2^Msl*2^16-1"
                                                     */
    volatile uint32_t PHY_Rx_Node_Ctrl;              /* Offset: 0x0000_0034, 
                                                        [31:30] reserve_31_30       RO      
                                                        [29:24] fft_win_shift   0x0 RW  直接寻址    "名称：FFT window shift
                                                        介绍：128点时的fft开窗位置,业务/从节点rx的FFT开窗起点(不含时偏调整)
                                                        取值：TBD"
                                                        [23:22] reserve_23_22       RO      
                                                        [21:16] rx_npd  0x0 RW  直接寻址    "名称：The starting point of the receiving window for the master node
                                                        介绍：主节点的开窗起点 (名称和作用后期可能会调整)
                                                        取值："
                                                        [15:0]  rx_npd_adjust_m 0x0 RW  直接寻址    "名称：The window adjustment value of the master node
                                                        介绍：主节点开窗调整值，可前调，可后调,_m 是master缩写，意思是主节点有效
                                                        取值：TBD"
                                                        */
    volatile uint32_t PHY_Rx_Abn_Max;                /* Offset: 0x0000_0038, 
                                                        [31:8]  reserve_31_8        RO      
                                                        [7:0]   clk_abn_cnt_max 0x0 RW  直接寻址    "名称：sampling clock offset abnormal counter maximum
                                                        介绍：时钟偏差异常的次数上限,
                                                        在Rsm[0]==6的情况下，如果时钟偏差异常的次数大于此值，则认为异常，对应算法c文件rxsm.c的CTc，默认值为4
                                                        取值：TBD"
                                                        */
    volatile uint32_t PHY_Rx_dpll;                   /* Offset: 0x0000_003C, 
                                                        [31:28] reserve_31_28   0x0 RW  直接寻址    "名称：Initial DPLL clock parameters
                                                        介绍：初始dpll时钟参考
                                                        取值：TBD"
                                                        [27:0]  init_dpll
                                                        */
    volatile uint32_t PHY_Rx_Frm_Ctrl_1;             /* Offset: 0x0000_0040, 
                                                        [31:16] corsync_fc_max  0x0 RW  直接寻址    "名称：coarse sampling clock timing offset abnormal  frames uplimit
                                                        介绍：用于粗时钟同步的帧数上限,补充：在Rsm[0]==2下的超时门限，即如果Rsm[0]==2持续的时间超过门限，则报错
                                                        取值：TBD"                                                                            
                                                        [15:0]  finsync_fc_max  0x0 RW  直接寻址    "名称：fine sampling clock timing offset abnormal 
                                                        frames uplimit
                                                        介绍：用于细时钟同步的帧数上限,对应算法c文件rxsm.c的Tfc，默认值为32*64
                                                        取值：TBD"
                                                        */
    volatile uint32_t PHY_Rx_Frm_Ctrl_2;             /* Offset: 0x0000_0044, 
                                                        [31:16] demcrtl_fc_max  0x0 RW  直接寻址    "名称：Demodulation of control frames maximum
                                                        介绍：用于控制符号解调的帧数上限,对应算法c文件rxsm.c的Tmd，默认值为8*64
                                                        取值：TBD"          
                                                        [15:0]  mcsde_fc_max    0x0 RW  直接寻址    "名称：MCS demodulation frames maximum
                                                        介绍：用于调制编码模式解调的帧数上限,对应算法c文件rxsm.c的Tcd，默认值为8
                                                        取值：TBD"
                                                        */
    volatile uint32_t PHY_Rx_Frm_Ctrl_3;             /* Offset: 0x0000_0048, 
                                                        [31:16] reserve_31_16       RO      
                                                        [15:0]  frmsync_fc_max  0x0 RW  直接寻址    "名称：frame sync frames maximum
                                                        介绍：用于帧同步的帧周期数上限,对应算法c文件rxsm.c的Tfs，默认值为2*64
                                                        取值：TBD"
                                                        */
    volatile uint32_t PHY_Rx_Sfrm_Ctrl[32];          /* Offset: 0x0000_004C ~ 0x0000_00C8, 32 
                                                        [31:0]  rx_frm(#i)_sym(#j)_conf 0x0 RW  直接寻址    "名称：receive symbol type configuration per frame
                                                        介绍： Rx接收机信道配置向量
                                                        FRAME_NUMBER = 8      帧ID，超帧最大数量为8帧。
                                                        (for i = 0; i <= FRAME_NUMBER-1)
                                                        i 为frame ID 帧号                                                        
                                                        SYMBOL_NUMBER = 64   符号ID，每帧共有64个symbol。
                                                        共SYMBOL_NUMBER *2bit位宽
                                                        (for j = 0; j <= SYMBOL_NUMBER -1)                                                        
                                                        接收机信道配置向量，每帧为一个数组  [i][j]，最多为8帧 [8][64]
                                                        rx_frm(#i)_sym(#j)_conf：其中
                                                        i表示第i帧，取值1、2、4、8
                                                        j表示帧内第 就 j个符号类型，取值0-63。
                                                        模式取值0-2。模式的意义如下：
                                                        取值：
                                                        0：不接收；
                                                        1：模式选择 1，当Mtf==0 为导频或者竞争符号（当前帧内符号序号为63对应竞争符号），当Mtf=={1,2}为频分fcf/连续scf符号；
                                                        2 ：数据或者控制符号"
                                                        */
    volatile uint32_t PHY_Rx2Tx_Delay;               /* Offset: 0x0000_00CC, Tx，Rx相关配置
                                                        [31:16] reserve_31_16       RO      
                                                        [15:0]  rx2txstage1_dly 0x0 RW  直接寻址    "名称：Rx to Tx's stage 1 delay
                                                        介绍：该值必须>4。只有在slave节点使用此寄存器字段，master节点不使用。
                                                        以下是两个过程：
                                                        1、当Rx接收到符号时，会出一个脉冲信号标记符号头
                                                        2、Tx在stage1符号发送阶段会发送脉冲信号，表示符号发送开始
                                                        Delay_rx2txstage1意义：slave节点可以进行发送，也可以进行接收。但是只有当1发生，并且时间满足Delay_rx2txstage1设置的周期数之后，2才进行。
                                                        取值：TBD"
                                                        */
    volatile uint32_t PHY_Rx_Frm_Offset;             /* Offset: 0x0000_00D0, 
                                                        [31:16] reserve_31_16       RO      
                                                        [15:0]  rx_frm_ofs  0x0 RW  直接寻址    "名称：recevie frame offset
                                                        介绍：此寄存器字段仅用于RX接收端，TX发送端不关注
                                                        C model中有相关代码，但未找到被调用的地方。
                                                        取值：TBD"
                                                        */
    volatile uint32_t PHY_Tx_Configuration;          /* Offset: 0x0000_00D4, 
                                                        [31:8]  reserve_31_8        RO      
                                                        [7:0]   tx_dig_gain 0x0 RW  直接寻址    "名称：Tx digtal baseband gain
                                                        介绍：调节数字基带DAC增益，数值 可取0~255。但通常设置为182~229。_dig means digital
                                                        取值：
                                                        72 ：对应-20.05dB
                                                        162：对应-13dB
                                                        182：对应-12dB
                                                        229：对应-10dB
                                                        255：对应-9.05dB"
                                                        */
    volatile uint32_t PHY_Tx_Node_Ctrl_0;            /* Offset: 0x0000_00D8, */
    volatile uint32_t PHY_Tx_Node_Ctrl_1;            /* Offset: 0x0000_00DC,     
                                                        [31:0]  filter_start_flag0  0x0 RW  直接寻址    "名称：The filter is operating for the first time. The nid node flag is stored.
                                                        介绍：此寄存器仅对主节点有意义，从节点不用关注。
                                                        2个32bit 寄存器组合成一个[63:0]，64个节点对应64个Nid。
                                                        取值：
                                                        0：表示该Nid对应的用于监测Nid节点到本节点的滤波器是首次运行。
                                                        1 ：表示该Nid对应的用于监测Nid节点到本节点滤波器不是首次运行。"
                                                        [31:0]  filter_start_flag1  0x0 RW  直接寻址    
                                                        */
    volatile uint32_t PHY_Tx_Node_Ctrl_2;            /* Offset: 0x0000_00E0, 
                                                        [31:6]  reserve_31_6        RO      
                                                        [5:0]   filter_start_flag2  0x0 RW  直接寻址    "名称：The virtual filter is operating for the first time. The nid node flag is stored.
                                                        介绍：此寄存器仅对主节点有意义，从节点不用关注。
                                                        6bit 寄存器组合成一个[5:0]，6虚拟节点对应6个虚拟Nid。
                                                        此寄存器表示虚拟Nid对应的监测Nid节点到本节点的滤波器是否首次运行。
                                                        取值：
                                                        0：表示该Nid对应的滤波器是首次运行。
                                                        1： 表示该Nid对应的滤波器不是首次运行。"
                                                        */
    volatile uint32_t PHY_Tx_Afe_Ctrl;               /* Offset: 0x0000_00E4, 
                                                        [31:31] reserve_31_30   0x0 RO      
                                                        [30:30] use_adcclk  0x0 RW  直接寻址    是否使用afe提供的adc clk，1使用，0不使用
                                                        [29:29] adc_edge_sel    0x0 RW  直接寻址    afe adc数据使用时采样时刻选择：0-上升沿采样；1-下降沿采样
                                                        [28:28] dac_edge_sel    0x0 RW  直接寻址    afe dac数据使用时采样时刻选择：0-上升沿采样；1-下降沿采样
                                                        [27:20] reserve_27_20   0x0 RO      
                                                        [19:16] afe_ctrl_sw 0x0 RW  直接寻址    "4bit:afe_ctrl_sw:
                                                        [3]:配成1，则afe rx由软件控制，否则由硬件控制；
                                                        [2]:当afe rx由软件控制，则1为afe rx开启，0为afe rx关闭；
                                                        [1]:配成1，则afe tx由软件控制，否则由硬件控制；
                                                        [0]:当afe tx由软件控制，则1为afe tx开启，0为afe tx关闭；"
                                                        [15:8]  rx_afe_gain _ana    0x0 RW  直接寻址    "名称：The gain select of rx afe. It's analog gain. 
                                                        介绍：低7bit是给模拟配置的agc gain值，最高位为此配置是否生效。当最高位配1，则使用低7bit的gain值；否则使用硬件实时计算的值
                                                        取值：默认0，软件可配置，实时生效。"
                                                        [7:4]   tx_afe_on_ahd_ ana  0x0 RW  直接寻址    "名称：The cycle num to turn on afe before true data need to send
                                                        介绍：通过配置此寄存器，设置afe开启的提前时间，效果是会在真实数据发送前(11-x)个cycle提前开启afe。
                                                        _ahd means ahead
                                                        取值：默认6，即在真实数据发送前11个cycle就把AFE打开了。"
                                                        [3:0]   tx_afe_off_dly_ ana 0x0 RW  直接寻址    "名称：The cycle num to turn off afe after true data sended 
                                                        介绍：通过配置此寄存器，设置afe关闭的延迟时间，效果是会在真实数据发送后x个cycle再关闭afe。
                                                        _dly means delay
                                                        取值：默认6，即在真实数据发送完毕后6个cycle再把AFE关闭。"
                                                        */
    volatile uint32_t PHY_Mem_Rd_Configuration;      /* Offset: 0x0000_00E8, 
                                                        [31:25] reserve_31_25   0x0 RO      
                                                        [24:16] rd_rxsymtonid_int   0x0 RW  直接寻址    "TBD
                                                        名称：Set the symbol-to-nid  count memory read count times to trigger an interrupt.
                                                        介绍：设置读sym_to_nid 对应的mem次数，到达该次数后触发中断。
                                                        该接口可写入，但是没有中断效果。
                                                        因为RTL代码目前没有真实使用此信号。"
                                                        [15:9]  reserve_15_9    0x0 RO      
                                                        [8:0]   rd_txsymtype_int    0x0 RW  直接寻址    "TBD
                                                        名称：Set the symbol type memory read count times to trigger an interrupt.
                                                        介绍：设置读mem次数，到达该次数后触发中断。
                                                        该接口可写入，但是没有中断效果。
                                                        因为RTL代码目前没有真实使用此信号。"
                                                        */
    volatile uint32_t PHY_Tx_Status;                 /* Offset: 0x0000_00EC, 
                                                        [31:13] reserve_31_13       RO      
                                                        [12:4]  tx_sym_num  0x0 RO  直接寻址    "名称：The symbol count value of Tx
                                                        介绍：Tx发送端symbol 计数值,递增
                                                        取值：0~511，到达最大值后反转为0"
                                                        [3:1]   reserve_3_1     RO      
                                                        [0:0]   slave_tx_flag   0x0 RO  直接寻址    "名称：The flag of tx state in slave node
                                                        介绍：当作为从节点时（nmode=0），从节点TX发送，此位被置1。在发送期间该位一直为1，保持高电平。当RX接收状态机异常出错时（例如大量乱包，错包），RX和TX均被置0，停止工作。
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_BRD_Ptr;                /* Offset: 0x0000_00F0, 
                                                        [31:0]  out_cnt_rxbuf   0x0 RO  直接寻址    "名称：Out count of Rx buffer
                                                        介绍：Rx buffer 输出点计数值
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_BWR_Ptr;                /* Offset: 0x0000_00F4, 
                                                        [31:0]  in_cnt_rxbuf    0x0 RO  直接寻址    "名称：In counter of Rx buffer
                                                        介绍：Rx buffer 输入点计数值
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_T1_Ferq;                /* Offset: 0x0000_00F8, 
                                                        [31:28] reserve_31_28       RO      
                                                        [27:0]  ref_clk_freq    0x0d1b7175  RO  直接寻址    "名称:   Reference clock frequency,
                                                        i.e., the clock frequency of the master node
                                                        介绍：基准时钟频率,即主节点时钟频率
                                                        取值：0x0d1b7175"
                                                        */
    volatile uint32_t PHY_Rx_Status_1;               /* Offset: 0x0000_00FC, 
                                                        [31:8]  reserve_31_8        RO      
                                                        [7:4]   nca_dem 0x0 RO  直接寻址    "名称：The nca demodulated by Rx.
                                                        介绍：Nca，子信道所包含子带数量
                                                        取值：根据msl的取值确定范围。"
                                                        [3:2]   reserve_3_2     RO      
                                                        [1:0]   msl_dem 0x0 RO  直接寻址    "名称：The msl demodulated by Rx.
                                                        介绍：Msl ，OFDM符号长度模式
                                                        取值：0，1，2，3"
                                                        */
    volatile uint32_t PHY_Rx_Status_2;               /* Offset: 0x0000_0100, 
                                                        [31:24] reserve_31_24               
                                                        [23:0]  mcs_dem 0x0 RO  直接寻址    "名称：The tcm demodulated by Rx.
                                                        介绍：当Rx接收状态机进入状态5时，会更新此寄存器该字段。此24bit标识调制编码模式（Mcm，即Tcm）
                                                        取值：0~7，对应不同的调制模式"
                                                        */
    volatile uint32_t PHY_Rx_Status_3;               /* Offset: 0x0000_0104, 
                                                        [31:23] reserve_31_23   0x0 RO      
                                                        [22:16] agc_gain    0x0 RO      
                                                        [15:9]  reserve_15_9    0x0 RO      
                                                        [8:8]   rx_fsm_err_flag 0x0 RO  直接寻址    "名称：The flag of rx_fsm err(error)
                                                        介绍：当RX接收状态机异常出错时（例如大量乱包，错包），RX会自动停止工作，此时寄存器该字段被置0. 
                                                        取值：N/A"
                                                        [7:7]   reserve_7       RO      
                                                        [6:4]   rx_rsm  0x0 RO  直接寻址    "名称：The Rx's state machine（FSM）
                                                        介绍：Rx接收状态机的接收状态
                                                        取值：
                                                        0:  表示空闲状态
                                                        1：符号同步状态
                                                        2：帧同步状态
                                                        3：时钟粗同步状态
                                                        4：时钟细同步状态
                                                        5：调制编码解调状态
                                                        6：控制符号解调状态
                                                        7：数据解调状态
                                                        8：主节点接收状态"
                                                        [3:1]   reserve_3_1     RO      
                                                        [0:0]   rx_state_pl 0x0 RO  直接寻址    "名称：Receiver progress pulse signal
                                                        介绍：接收机进度脉冲信号。当此位上跳1，标志接收机1次状态转换
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_Status_4        ;       /* OffsetL 0x0000_0108, 
                                                        [31:0]  rx_frame_idx        RO  直接寻址    "名称：frame number
                                                        介绍：表示Rx接收机接收到的帧序号
                                                        取值：0~2^32-1，到达最大值后自动反转"
                                                        */
    volatile uint32_t PHY_Rx_Status_5        ;       /* OffsetL 0x0000_010C, 
                                                        [31:22] reserve_31_22       RO      
                                                        [21:16] rx_sym_idx  0x0 RO  直接寻址    "名称：symbol number
                                                        介绍：表示Rx接收机接收到的符号序号。
                                                        取值：0~63，到达最大值后自动反转"
                                                        [15:8]  rx_data_abn_cnt 0x0 RO  直接寻址    "名称：The number of abnormal packet of Rx.
                                                        介绍：数据包接收异常数量(Npe)（此字段为error提示）
                                                        取值：N/A"
                                                        [7:0]   rx_ctr_abn_cnt  0x0 RO  直接寻址    "名称：Abnormal reception of control symbols of Rx
                                                        介绍：控制符号接收异常（Nqa）（此字段为error提示）
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_DBBRS_Status_1  ;       /* OffsetL 0x0000_0110, 
                                                        [31:25] reserve_31_25               
                                                        [ 24:20]    rs_err_cnt  0x0 RO  直接寻址    "名称：The RS decoder error number
                                                        介绍：RS译码器错误数量
                                                        取值：N/A"
                                                        [19:17] reserve_19_17               
                                                        [16:12] rs_berl_err_cnt 0x0 RO  直接寻址    "名称：The RS decoder berl error number
                                                        介绍：RS译码器第二步伯利坎算法计算的错误个数
                                                        取值：N/A"
                                                        [11:9]  reserve_11_9        RO      
                                                        [8:8]   rs_err_flag 0x0 RO  直接寻址    "名称：The RS decoder error flag
                                                        介绍：RS译码器错误标志，1：有错误
                                                        取值：N/A"
                                                        [7:0]   in_len_rsdec    0x0 RO  直接寻址    "名称：The length for RS decoder to decode
                                                        介绍：RS译码器等待被译码的数据长度
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_DBBRS_Status_2  ;       /* OffsetL 0x0000_0114, 
                                                        [31:16] rs_wr_err_cnt   0x0 RO  直接寻址    "名称：rs fifo write full error 
                                                        介绍：初始值0 ，表示无错误。当出现rs fifo 写满错误时，计数值+1。复位或者到达最大值后归0，重新计数。
                                                        通常在TX发送时，MAC给的实际数据与PHY请求的数据量不符合，累计之后会触发该错误。
                                                        取值：N/A"
                                                        [15:0]  rs_rd_empty_cnt 0x0 RO  直接寻址    "名称：rs fifo read empty error 
                                                        介绍：初始值0 ，表示无错误。当出现rs fifo 读空错误时，计数值+1。复位或者到达最大值后归0，重新计数。
                                                        通常在TX发送时，MAC给的实际数据与PHY请求的数据量不符合，累计之后会触发该错误。
                                                        取值：N/A"
                                                        */
    volatile uint32_t rsv0;                          /* OffsetL 0x0000_0118 */ 
    volatile uint32_t PHY_Rx_Clk_Status_1    ;       /* OffsetL 0x0000_011C, 
                                                        [31:30] reserve_31_30       RO      
                                                        [29:0]  clk_phase_shift 0x0 RO  直接寻址    "名称：The phase shift of clk
                                                        介绍：时钟相位偏差(Aca)，由时钟同步模块计算产生，分为粗同步和精同步
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_Clk_Status_2    ;       /* OffsetL 0x0000_0120, 
                                                        [31:8]  reserve_31_8        RO      
                                                        [7:0]   clk_offset_abn  0x0 RO  直接寻址    "名称：sampling clock offset abnormal
                                                        介绍：时钟偏差异常(Nce)（此字段为error提示）
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_Ch_Status       ;       /* OffsetL 0x0000_0124, 
                                                        [31:29] reserve_31_29       RO      
                                                        [28:24] pilot_fft_shift 0x0 RO  直接寻址    "名称：The number of right shift bits for FFT in the pilot channel
                                                        介绍：导频信道FFT右移位数(Wwp)
                                                        取值：N/A"
                                                        [23:22] reserve_23_22       RO      
                                                        [21:16] data_fft_shift  0x0 RO  直接寻址    "名称：The number of right shift bits for FFT in the traffic channel
                                                        介绍：业务信道FFT右移位数(Wwd)
                                                        取值：N/A"
                                                        [15:12] reserve_15_12       RO      
                                                        [11:0]  sub_amp_max 0x7ff   RO  直接寻址    "名称：The maximum sub-channel signal amplitude within a frame
                                                        介绍：帧内最大子信道信号幅度（Gfm）
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_Node_Status     ;       /* OffsetL 0x0000_0128, 
                                                        [31:0]  lpbk_dly_mstr   0x0 RO  直接寻址    "名称：The clk phase shift of master self-receive 
                                                        介绍：仅用于管理节点，管理节点把帧内第0个符号对应的Rsm[2]保存到Rsm[7]，这样管理节点对帧内第0个符号进行自发自收，测量管理节点发送通道的基带（5.12MHz采样率上）到接收通道（5.12MHz采样率上）的延迟，30bits有符号数
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_SNA0            ;       /* OffsetL 0x0000_012C,
                                                        [31:16] reserve_31_16       RO      
                                                        [15:0]  sub0_noise_amp  0x0 RO  直接寻址    "名称：The noise amptitude of sub band0
                                                        介绍：0子带噪声幅度
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_SNA1            ;       /* OffsetL 0x0000_0130, 
                                                        [31:16] reserve_31_16       RO      
                                                        [15:0]  sub1_noise_amp  0x0 RO  直接寻址    "名称：The noise amptitude of sub band1
                                                        介绍：1子带噪声幅度
                                                        取值：N/A"
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_SNA2            ;       /* OffsetL 0x0000_0134, 
                                                        [31:16] reserve_31_16       RO      
                                                        [15:0]  sub2_noise_amp  0x0 RO  直接寻址    "名称：The noise amptitude of sub band2
                                                        介绍：2子带噪声幅度
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_SNA3            ;       /* OffsetL 0x0000_0138, 
                                                        [31:16] reserve_31_16       RO      
                                                        [15:0]  sub3_noise_amp  0x0 RO  直接寻址    "名称：The noise amptitude of sub band3
                                                        介绍：3子带噪声幅度
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_SNA4            ;       /* OffsetL 0x0000_013C, 
                                                        [31:16] reserve_31_16       RO      
                                                        [15:0]  sub4_noise_amp  0x0 RO  直接寻址    "名称：The noise amptitude of sub band4
                                                        介绍：4子带噪声幅度
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_SNA5            ;       /* OffsetL 0x0000_0140, 
                                                        [31:16] reserve_31_16       RO      
                                                        [15:0]  sub5_noise_amp  0x0 RO  直接寻址    "名称：The noise amptitude of sub band5
                                                        介绍：5子带噪声幅度
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_SNA6            ;       /* OffsetL 0x0000_0144, 
                                                        [31:16] reserve_31_16       RO      
                                                        [15:0]  sub6_noise_amp  0x0 RO  直接寻址    "名称：The noise amptitude of sub band6
                                                        介绍：6子带噪声幅度
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_SNA7            ;       /* OffsetL 0x0000_0148, 
                                                        [31:16] reserve_31_16       RO      
                                                        [15:0]  sub7_noise_amp  0x0 RO  直接寻址    "名称：The noise amptitude of sub band7
                                                        介绍：7子带噪声幅度
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_OFDMD_Status_1     ;       /* OffsetL 0x0000_014C, 
                                                        [31:19] reserve_31_19       RO      
                                                        [18:16] ncb_dem 0x0 RO  直接寻址    "名称：The Number of Reed-Solomon (RS) encoded blocks per data OFDM symbol
                                                        介绍：一个数据OFDM符号承载的RS编码块数量（Ncb）
                                                        取值：N/A"
                                                        [15:10] reserve_15_10       RO      
                                                        [9:0]   inbytes_data    0x0 RO  直接寻址    "名称：The Number of bytes per data OFDM symbol
                                                        介绍：一个数据OFDM符号承载的字节数（Nsd）
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_OFDMD_Status_2     ;       /* OffsetL 0x0000_0150, 
                                                        8   [31:24] reserve_31_24       RO      
                                                        8   [23:16] rs_data 0x0 RO  直接寻址    "名称：The length of Reed-Solomon (RS) encoded blocks per data OFDM symbol
                                                        介绍：一个数据OFDM符号的RS编码块长度（Nrs）
                                                        Nrs，数据OFDM符号的RS编码块长度
                                                        取值：N/A"
                                                        16  [15:14] reserve_15_14       RO      
                                                            [13:0]  codedbits_data  0x0 RO  直接寻址    "名称：The length of encoded bits carried by one data OFDM symbol
                                                        介绍：一个数据OFDM符号承载的编码比特长度（Nsc）
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_OFDMC_Status_1     ;       /* OffsetL 0x0000_0154, 
                                                        [31:14] reserve_31_14       RO      
                                                        [13:4]  inbytes_ctrl    0x0 RO  直接寻址    "名称：The length of encoded bits carried by one control OFDM symbol
                                                        介绍：一个控制OFDM符号承载的字节数（Nsd）
                                                        取值：N/A"
                                                        [3:3]   reserve_3       RO      
                                                        [2:0]   ncb_ctrl    0x0 RO  直接寻址    "名称：The number of RS coding blocks carried by one control OFDM symbol
                                                        介绍：一个控制OFDM符号承载的RS编码块数量（Ncb）
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_OFDMC_Status_2     ;       /* OffsetL 0x0000_0158, 
                                                        [31:24] reserve_31_24       RO      
                                                        [23:16] rs_ctrl 0x0 RO  直接寻址    "名称：The length of the RS coding block corresponding to one control OFDM symbol
                                                        介绍：一个控制OFDM符号对应的编码块的数量（Nrs）
                                                        取值：N/A"
                                                        [15:14] reserve_15_14       RO      
                                                        [13:0]  codedbits_ctrl  0x0 RO  直接寻址    "名称：The length of encoded bits carried by one control OFDM symbol
                                                        介绍：一个控制OFDM符号对应的（Nsc）
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_CC_Status       ;       /* OffsetL 0x0000_015C, 
                                                        [31:10] reserve_31_10       RO      
                                                        [9:8]   cc_chan_id  0x0 RO  直接寻址    "名称：The shared channel number occupied by the successful node in the contention-based shared channel
                                                        介绍：竞争共享信道成功节点所占有的共享信道序号
                                                        取值：N/A"
                                                        [7:5]   reserve_7_5     RO      
                                                        [4:4]   cc_result_flag  0x0 RO  直接寻址    "名称：The result of the local node's contention for the shared channel
                                                        介绍：本节点竞争共享信道结果，
                                                        取值：
                                                        0：不成功，1：竞争共享信道成功"
                                                        [3:3]   reserve_3       RO      
                                                        [2:0]   cc_nid_num  0x0 RO  直接寻址    "名称：The result of the local node's contention for the shared channel
                                                        介绍：检测到的发送竞争用户数量
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_Rx_CC_ID           ;       /* OffsetL 0x0000_0160, 
                                                        [31:24] cc_nid0 0x0 RO  直接寻址    "名称：The list of detected competing users' IDs, or the node ID that hits the contention.
                                                        介绍：检测到的竞争用户ID列表 / 竞争命中的节点ID
                                                        取值：N/A"
                                                        [23:16] cc_nid1 0x0 RO  直接寻址    "名称：The list of detected competing users' IDs, or the node ID that hits the contention.
                                                        介绍：检测到的竞争用户ID列表 / 竞争命中的节点ID
                                                        取值：N/A"
                                                        [15:8]  cc_nid2 0x0 RO  直接寻址    "名称：The list of detected competing users' IDs, or the node ID that hits the contention.
                                                        介绍：检测到的竞争用户ID列表 / 竞争命中的节点ID
                                                        取值：N/A"
                                                        [7:0]   cc_nid3 0x0 RO  直接寻址    "名称：The list of detected competing users' IDs, or the node ID that hits the contention.
                                                        介绍：检测到的竞争用户ID列表 / 竞争命中的节点ID
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_FCS_Status         ;       /* OffsetL 0x0000_0164, 
                                                        [31:9]  reserve_31_9    0x0 RO      
                                                        [8:0]   sym_to_nid  0x0 RO  直接寻址    "名称：The [i]-th channel of the frame, and the [j]-th Symbol of the same frame .
                                                        介绍：高3位表示从帧头开始计算，此信道是该帧的第i个信道。低6位表示，从帧头开始计算，以帧头符为起点的第 j个符号
                                                        取值：
                                                        i ：bit[8:6] 3bit为 信道ID。表示从帧头开始的第i个信道。
                                                        j ：bit[5:0] 6bit为 symbol ID，符号ID，表示从帧头开始的第j个符号。"
                                                        */
    volatile uint32_t PHY_TEST_Configuration ;       /* OffsetL 0x0000_0168,
                                                        [31:20] 12  invalid_power_thr   0x10    RW  直接寻址    如果调整大于该值的认定为无效，即判决为无信号，不会尝试调整AGC到最大值
                                                        [19:17] 3   reserve_19_17   0x0 RO      
                                                        [16:16] 1   bpsk_en 0x0 RW      如果配成0，就是正常模式，否则为bpsk模式
                                                        [15:13] 3   reserve_15_13   0x0 RO      
                                                        [12:12] 1   crc_freeze  0x0 RW      当配置为1时，crc不再更新，此时可以软件清除crc历史值；配置为0，恢复crc更新
                                                        [11:11] 1   reserve_11_11   0x0 RO      
                                                        [10:10] 1   super_frame_en  0x0 RW      超帧模式。1:超帧模式；0：非超帧模式。
                                                        [9:9]   1   state_acc_mode  0x0 RW      默认正常配置，如果为1，表示为了EDA理想场景下的加速模式
                                                        [8:8]   1   dig_lb_en   0x0 RW      "名称：DAC to ADC digital loopback。
                                                        loopback 缩写为LB
                                                        介绍：用于DA-->AD 环路测试。软件需要在初始化之前设置该值
                                                        取值：
                                                        0：正常工作模式，不环回
                                                        1：数字环回测试模式，DA-->AD。回环模式开启，主节点自发自收。此时应注意Rx接收端需要设置接收信道为“非主节点管理信道”"
                                                        [7:5]   3   reserve_7_5 0x0 RO      
                                                        [4:4]   1   tx_cnt_clr  0x0 RWC     phy发出来的有效切片个数的清零
                                                        [3:1]   3   reserve_3_1 0x0 RO      
                                                        [0:0]   1   rx_cnt_clr  0x0 RWC     phy收到的有效切片个数的清零
                                                        */

    volatile uint32_t rsv1[421];                     /* Offset: 0x0000_016C, */
    volatile uint32_t PHY_Tx_Mem[512];               /* Offset: 0x0000_0800,
                                                            [31:4]  reserve_31_4                
                                                            [3:0]   tx_sym_type_tb  0x0 RW  直接寻址    "名称：The symbol type table of Tx.
                                                        介绍：发送端需要配置此512x32bit memory，主节点和从节点都需要配置，超帧也需要使用。每个symbol对应32bit 位宽。若连续超帧为8帧，则所有512个32bit 符号类型全都需要配置。
                                                        用tx_sym_type_tb字段表示符号类型。
                                                        取值：
                                                        0：空
                                                        1：导频
                                                        2：控制
                                                        3：数据
                                                        4：竞争
                                                        5：频分
                                                        6：连续"
                                                        */
    volatile uint32_t PHY_MEM_NID[512];              /* Offset: 0x0000_1000, 
                                                        [31:8]  reserve_31_8                
                                                        [7:0]   sym2nid_tb  0x0 RW      "名称：Node identification
                                                        介绍：Nid，节点ID，64个物理节点，6个虚拟节点。
                                                        取值：
                                                        0~63：共64个物理节点，其值对应物理节点ID值
                                                        64~229：空置不用
                                                        250~255：共6个虚拟节点，其值对应虚拟节点ID值"
                                                        */

    volatile uint32_t PHY_MEM_Node_Delay[70]   ;    /* Offset: 0x0000_1800, 
                                                        [31:30] reserve_31_30               
                                                        [29:0]  nid_filt_vs_loc_statis  0x0 RW      "名称：The filtered value of the delay measurement from the Nid node to the local node
                                                        介绍：Nid节点到本节点的延迟测量值的滤波值（如果Nid>0还减去管理节点自发自收的延迟）
                                                        取值：N/A"
                                                        */
    volatile uint32_t rsv2[58];                      /* Offset: 0x0000_1918, 
                                                        [31:12] reserve_31_12               
                                                        [11:0]  nid_pow_vs_exp_statis   0x0 RW      "名称：The signal power from the Nid node compared to the expected value (Nid's power vs. expect )
                                                        介绍：来自Nid节点的信号功率比期望值(std=2^12)低 Rsp[Nid][4]/32 dB
                                                        取值：N/A"
                                                        */
    volatile uint32_t PHY_MEM_Signal_Powe[70]  ;    /* Offset: 0x0000_1A00,
                                                        [31:12] reserve_31_12               
                                                        [11:0]  nid_pow_vs_exp_statis   0x0 RW      "名称：The signal power from the Nid node compared to the expected value (Nid's power vs. expect )
                                                        介绍：来自Nid节点的信号功率比期望值(std=2^12)低 Rsp[Nid][4]/32 dB
                                                        取值：N/A"
                                                        */
    volatile uint32_t rsv3[58];                     /* Offset: 0x0000_1B18, */
    volatile uint32_t PHY_MEM_Node_Symbol[70]  ;    /* Offset: 0x0000_1C00, 
                                                        [31:0]  rx_cd_sym_statis    0x0 RW      "名称：The number of control symbols or data symbols received from the Nid node
                                                        介绍：接收来自Nid节点的控制符号/数据符号数
                                                        取值：N/A"
                                                        */
    volatile uint32_t rsv4[58];                     /* Offset: 0x0000_1D18, */
    volatile uint32_t PHY_MEM_CRCError_Cum[70] ;    /* Offset: 0x0000_1E00, 
                                                        [31:0]  rx_crc32_ces_statis 0x0 RW      "名称：CRC32 cumulative error statistics
                                                        介绍：CRC32累计错误统计
                                                        取值：N/A"
                                                        */
    volatile uint32_t rsv5[58];                      /* Offset: 0x0000_1F18, */
    volatile uint32_t PHY_MEM_CRCError_Cont[70];    /* Offset: 0x0000_2000, 
                                                        [31:0]  rx_crc32_cfs_statis 0x0 RW      "名称：Statistics on the continuous failure state of the Rx's CRC32 
                                                        介绍：CRC32连续失败次数统计
                                                        取值：N/A"
                                                        */
    volatile uint32_t rsv6[58];                      /* Offset: 0x0000_2118, */
    volatile uint32_t PHY_MEM_CRC32_Check[70]  ;    /* Offset: 0x0000_2200, 
                                                        [31:1]  reserve_31_1                
                                                        [0:0]   rx_crc32_statis 0x0 RW      "名称：Rx CRC32 check passes or not
                                                        介绍：CRC32校验是否通过
                                                        取值：N/A"
                                                        */
    volatile uint32_t rsv7[58];                      /* Offset: 0x0000_2318, */
                                                     /* Offset: 0x0000_2400, END*/
} utb_phy_reg_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_25 : 7;        /* [25 : 31] */
        uint32_t rx_auto_recover : 1;      /* [24 : 24] */
        uint32_t reserve_23_21 : 3;        /* [21 : 23] */
        uint32_t utb_enable : 1;           /* [20 : 20] */
        uint32_t reserve_19_17 : 3;        /* [17 : 19] */
        uint32_t tx_pre : 1;               /* [16 : 16] */
        uint32_t reserve_15_13 : 3;        /* [13 : 15] */
        uint32_t rx_enable : 1;            /* [12 : 12] */
        uint32_t reserve_11_9 : 3;         /* [9  : 11] */
        uint32_t tx_enable : 1;            /* [8  : 8] */
        uint32_t reserve_7_5 : 3;          /* [5  : 7] */
        uint32_t rx_disable : 1;           /* [4  : 4] */
        uint32_t reserve_3_1 : 3;          /* [1  : 3] */
        uint32_t tx_disable : 1;           /* [0  : 0] */
#else
        uint32_t tx_disable : 1;           /* [0  :  0] */
        uint32_t reserve_3_1 : 3;          /* [1  :  3] */
        uint32_t rx_disable : 1;           /* [4  :  4] */
        uint32_t reserve_7_5 : 3;          /* [5  :  7] */
        uint32_t tx_enable : 1;            /* [8  :  8] */
        uint32_t reserve_11_9 : 3;         /* [9  : 11] */
        uint32_t rx_enable : 1;            /* [12 : 12] */
        uint32_t reserve_15_13 : 3;        /* [13 : 15] */
        uint32_t tx_pre : 1;               /* [16 : 16] */
        uint32_t reserve_19_17 : 3;        /* [17 : 19] */
        uint32_t utb_enable : 1;           /* [20 : 20] */
        uint32_t reserve_23_21 : 3;        /* [21 : 23] */
        uint32_t rx_auto_recover : 1;      /* [24 : 24] */
        uint32_t reserve_31_25 : 7;        /* [25 : 31] */
#endif
    } reg;
    uint32_t value;
} PHY_Configuration_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_12 : 20;        /* [12 : 31] */
        uint32_t reserve_11_10 : 2;         /* [10 : 11] */
        uint32_t msl : 2;                   /* [8  :  9] */
        uint32_t reserve_7_6 : 2;           /* [6  :  7] */
        uint32_t tran_mode : 2;             /* [4  :  5] */
        uint32_t reserve_3_1 : 3 ;          /* [1  :  3] */
        uint32_t cc_type : 1;               /* [0  :  0] */
#else
        uint32_t cc_type : 1;               /* [0  :  0] */
        uint32_t reserve_3_1 : 3 ;          /* [1  :  3] */
        uint32_t tran_mode : 2;             /* [4  :  5] */
        uint32_t reserve_7_6 : 2;           /* [6  :  7] */
        uint32_t msl : 2;                   /* [8  :  9] */
        uint32_t reserve_11_10 : 2;         /* [10 : 11] */
        uint32_t reserve_31_12 : 20;        /* [12 : 12] */
#endif
    } reg;
    uint32_t value;
} PHY_Mode_Configuration_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_24 : 8;        /* [24 : 31] */
        uint32_t mcs : 24;                 /* [0  : 23] */
#else
        uint32_t mcs : 24;                 /* [0  : 23] */
        uint32_t reserve_31_24 : 8;        /* [24 : 31] */
#endif
    } reg;
    uint32_t value;
} PHY_Enc_Mcs_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_24 : 8;        /* [24 : 31] */
        uint32_t crc24 : 24;               /* [0  : 23] */
#else
        uint32_t mcs : 24;                 /* [0  : 23] */
        uint32_t reserve_31_24 : 8;        /* [24 : 31] */
#endif
    } reg;
    uint32_t value;
} PHY_Enc_CRC24_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_8 : 24;        /* [8 : 31] */
        uint32_t sub_num : 4;              /* [4  : 7] */
        uint32_t reserve_3 : 1;            /* [3 : 3] */
        uint32_t sub_start : 3;            /* [0  : 2] */
#else
        uint32_t sub_start : 3;            /* [0  : 2] */
        uint32_t reserve_3 : 1;            /* [3  : 3] */
        uint32_t sub_num : 4;              /* [4  : 7] */
        uint32_t reserve_31_8 : 24;        /* [8 : 31] */
#endif
    } reg;
    uint32_t value;
} PHY_Sbb_Configuration_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_5 : 27;          /* [5 : 31] */
        uint32_t node_type : 1;              /* [4  : 4] */
        uint32_t sup_frm_order : 4;          /* [0 : 3] */
#else
        uint32_t sup_frm_order : 4;          /* [0 : 3] */
        uint32_t node_type : 1;              /* [4  : 4] */
        uint32_t reserve_31_5 : 27;          /* [5 : 31] */
#endif
    } reg;
    uint32_t value;
} PHY_MSN_Configuration_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_12 : 20;          /* [12 : 31] */
        uint32_t reserve_11_9 : 3;            /* [9  : 11] */
        uint32_t cci : 1;                     /* [8  : 8] */
        uint32_t nid : 8;                     /* [0  : 7] */
#else
        uint32_t nid : 8;                     /* [0  : 7] */
        uint32_t cci : 1;                     /* [8  : 8] */
        uint32_t reserve_11_9 : 3;            /* [9  : 11] */
        uint32_t reserve_31_12 : 20;          /* [12 : 31] */
#endif
    } reg;
    uint32_t value;
} PHY_CC_Configuration_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_20 : 12;          /* [31:20] */
        uint32_t reserve_19 : 1;              /* [19:19] */
        uint32_t ncb_rs_ctrl : 3;             /* [18:16] */
        uint32_t reserve_15_14 : 2;           /* [15:14] */
        uint32_t ncoded_b_ctrl : 14;          /* [13:0] */
#else
        uint32_t ncoded_b_ctrl : 14;          /* [13:0] */
        uint32_t reserve_15_14 : 2;           /* [15:14] */
        uint32_t ncb_rs_ctrl : 3;             /* [18:16] */
        uint32_t reserve_19 : 1;              /* [19:19] */
        uint32_t reserve_31_20 : 12;          /* [31:20] */
#endif
    } reg;
    uint32_t value;
} PHY_OFDMC_Configuration1_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_24 : 8;          /* [31:24] */
        uint32_t reserve_23_18 : 6;          /* [23:18] */
        uint32_t nbytes_ctrl : 10;           /* [17:8] */
        uint32_t nrs_ctrl : 8;               /* [7:0] */
#else
        uint32_t nrs_ctrl : 8;               /* [7:0] */
        uint32_t nbytes_ctrl : 10;           /* [17:8] */
        uint32_t reserve_23_18 : 6;          /* [23:18] */
        uint32_t reserve_31_24 : 8;          /* [31:24] */
#endif
    } reg;
    uint32_t value;
} PHY_OFDMC_Configuration2_t;


typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_20 : 12;          /* [31:20] */
        uint32_t reserve_19 : 1;              /* [19:19] */
        uint32_t ncb_data : 3;                /* [18:16] */
        uint32_t reserve_15_14 : 2;           /* [15:14] */
        uint32_t ncoded_b_data : 14;          /* [13:0] */
#else
        uint32_t ncoded_b_data : 14;          /* [13:0] */
        uint32_t reserve_15_14 : 2;           /* [15:14] */
        uint32_t ncb_data : 3;                /* [18:16] */
        uint32_t reserve_19 : 1;              /* [19:19] */
        uint32_t reserve_31_20 : 12;          /* [31:20] */
#endif
    } reg;
    uint32_t value;
} PHY_OFDMD_Configuration1_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_24 : 8;          /* [31:24] */
        uint32_t reserve_23_18 : 6;          /* [23:18] */
        uint32_t nbytes_data : 10;           /* [17:8] */
        uint32_t nrs_data : 8;               /* [7:0] */
#else
        uint32_t nrs_data : 8;               /* [7:0] */
        uint32_t nbytes_data : 10;           /* [17:8] */
        uint32_t reserve_23_18 : 6;          /* [23:18] */
        uint32_t reserve_31_24 : 8;          /* [31:24] */
#endif
    } reg;
    uint32_t value;
} PHY_OFDMD_Configuration2_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_30 : 1;          /* [31:31] */
        uint32_t use_adcclk : 1;             /* [30:30] */
        uint32_t adc_edge_sel : 1;           /* [29:29] */
        uint32_t dac_edge_sel : 1;           /* [28:28] */
        uint32_t reserve_27_20 : 8;          /* [27:20] */
        uint32_t afe_ctrl_sw : 4;            /* [19:16] */
        uint32_t rx_afe_gain_ana : 8;        /* [15:8] */
        uint32_t tx_afe_on_ahd_ana : 4;      /* [7:4] */
        uint32_t tx_afe_off_dly_ana : 4;     /* [3:0] */
#else
        uint32_t tx_afe_off_dly_ana : 4;     /* [3:0] */
        uint32_t tx_afe_on_ahd_ana : 4;      /* [7:4] */
        uint32_t rx_afe_gain_ana : 8;        /* [15:8] */
        uint32_t afe_ctrl_sw : 4;            /* [19:16] */
        uint32_t reserve_27_20 : 8;          /* [27:20] */
        uint32_t dac_edge_sel : 1;           /* [28:28] */
        uint32_t adc_edge_sel : 1;           /* [29:29] */
        uint32_t use_adcclk : 1;             /* [30:30] */
        uint32_t reserve_31_30 : 1;          /* [31:31] */
#endif
    } reg;
    uint32_t value;
} PHY_Tx_Afe_Ctrl_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_8 : 24;          /* [31:8] */
        uint32_t tx_dig_gain : 8;            /* [7:0] */
#else
        uint32_t tx_dig_gain : 8;            /* [7:0] */
        uint32_t reserve_31_8 : 24;          /* [31:8] */
#endif
    } reg;
    uint32_t value;
} PHY_Tx_Configuration_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_8 : 24;          /* [31:8] */
        uint32_t nca_dem : 4;                /* [7:4] */
        uint32_t reserve_3_2 : 2;            /* [3:2] */
        uint32_t msl_dem : 2;                /* [1:0] */
#else
        uint32_t msl_dem : 2;                /* [1:0] */
        uint32_t reserve_3_2 : 2;            /* [3:2] */
        uint32_t nca_dem : 4;                /* [7:4] */
        uint32_t reserve_31_8 : 24;          /* [31:8] */
#endif
    } reg;
    uint32_t value;
} PHY_Rx_Status_1_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_24 : 8;          /* [31:24] */
        uint32_t mcs_dem : 24;               /* [23:0] */
#else
        uint32_t mcs_dem : 24;               /* [23:0] */
        uint32_t reserve_31_24 : 8;          /* [31:24] */
#endif
    } reg;
    uint32_t value;
} PHY_Rx_Status_2_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_23 : 9;        /* [31:23] */
        uint32_t agc_gain : 7;             /* [22:16] */
        uint32_t reserve_15_9 : 7;         /* [15:9] */
        uint32_t rx_fsm_err_flag : 1;      /* [8:8] */
        uint32_t reserve_7 : 1;            /* [7:7] */
        uint32_t rx_rsm : 3;               /* [6:4] */
        uint32_t reserve_3_1 : 3;          /* [3:1] */
        uint32_t rx_state_pl : 1;          /* [0:0] */
#else
        uint32_t rx_state_pl : 1;          /* [0:0] */
        uint32_t reserve_3_1 : 3;          /* [3:1] */
        uint32_t rx_rsm : 3;               /* [6:4] */
        uint32_t reserve_7 : 1;            /* [7:7] */
        uint32_t rx_fsm_err_flag : 1;      /* [8:8] */
        uint32_t reserve_15_9 : 7;         /* [15:9] */
        uint32_t agc_gain : 7;             /* [22:16] */
        uint32_t reserve_31_23 : 9;        /* [31:23] */
#endif
    } reg;
    uint32_t value;
} PHY_Rx_Status_3_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_24 : 8;          /* [31:24] */
        uint32_t rx_nnd : 24;                /* [23:0] */
#else
        uint32_t rx_nnd : 24;                /* [23:0] */
        uint32_t reserve_31_24 : 8;          /* [31:24] */
#endif
    } reg;
    uint32_t value;
} PHY_Rx_NND_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t corsync_fc_max : 16;          /* [31:16] */
        uint32_t finsync_fc_max : 16;          /* [15:0] */
#else
        uint32_t finsync_fc_max : 16;          /* [15:0] */
        uint32_t corsync_fc_max : 16;          /* [31:16] */
#endif
    } reg;
    uint32_t value;
} PHY_Rx_Frm_Ctrl_1_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t demcrtl_fc_max : 16;          /* [31:16] */
        uint32_t mcsde_fc_max : 16;            /* [15:0] */
#else
        uint32_t mcsde_fc_max : 16;            /* [15:0] */
        uint32_t demcrtl_fc_max : 16;          /* [31:16] */
#endif
    } reg;
    uint32_t value;
} PHY_Rx_Frm_Ctrl_2_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_16 : 16;           /* [31:16] */
        uint32_t frmsync_fc_max : 16;          /* [15:0] */
#else
        uint32_t frmsync_fc_max : 16;          /* [15:0] */
        uint32_t reserve_31_16 : 16;           /* [31:16] */
#endif
    } reg;
    uint32_t value;
} PHY_Rx_Frm_Ctrl_3_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_25 : 7;          /* [31:25] */
        uint32_t rx_mode : 1;                /* [24:24] */
        uint32_t npa_cnt_ctrl : 8;           /* [23:16] */
        uint32_t d_err_cnt_ctrl : 8;         /* [15:8] */
        uint32_t reserve_7 : 1;              /* [7:7] */
        uint32_t cc_det_thr : 3;             /* [6:4] */
        uint32_t reserve_3 : 1;              /* [3:3] */
        uint32_t scc_cnt_max : 3;            /* [2:0] */
#else
        uint32_t scc_cnt_max : 3;            /* [2:0] */
        uint32_t reserve_3 : 1;              /* [3:3] */
        uint32_t cc_det_thr : 3;             /* [6:4] */
        uint32_t reserve_7 : 1;              /* [7:7] */
        uint32_t d_err_cnt_ctrl : 8;         /* [15:8] */
        uint32_t npa_cnt_ctrl : 8;           /* [23:16] */
        uint32_t rx_mode : 1;                /* [24:24] */
        uint32_t reserve_31_25 : 7;          /* [31:25] */
#endif
    } reg;
    uint32_t value;
} PHY_Rx_Configuration_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_30 : 2;          /* [31:30] */
        uint32_t fft_win_shift : 6;          /* [29:24] */
        uint32_t reserve_23_22 : 2;          /* [23:22] */
        uint32_t rx_npd : 6;                 /* [21:16] */
        uint32_t rx_npd_adjust_m : 16;       /* [15:0] */
#else
        uint32_t rx_npd_adjust_m : 16;       /* [15:0] */
        uint32_t rx_npd : 6;                 /* [21:16] */
        uint32_t reserve_23_22 : 2;          /* [23:22] */
        uint32_t fft_win_shift : 6;          /* [29:24] */
        uint32_t reserve_31_30 : 2;          /* [31:30] */
#endif
    } reg;
    uint32_t value;
} PHY_Rx_Node_Ctrl_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_8 : 24;          /* [31:8] */
        uint32_t clk_abn_cnt_max : 8;        /* [7:0] */
#else
        uint32_t clk_abn_cnt_max : 8;        /* [7:0] */
        uint32_t reserve_31_8 : 24;          /* [31:8] */
#endif
    } reg;
    uint32_t value;
} PHY_Rx_Abn_Max_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_28 : 4;          /* [31:28] */
        uint32_t init_dpll : 28;             /* [27:0] */
#else
        uint32_t init_dpll : 28;             /* [27:0] */
        uint32_t reserve_31_28 : 4;          /* [31:24] */
#endif
    } reg;
    uint32_t value;
} PHY_Rx_dpll_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_12 : 20;          /* [31:12] */
        uint32_t nid_pow_vs_exp_statis : 12;  /* [11:0] */
#else
        uint32_t nid_pow_vs_exp_statis : 12;  /* [11:0] */
        uint32_t reserve_31_12 : 20;          /* [31:12] */
#endif
    } reg;
    uint32_t value;
} PHY_MEM_Signal_Powe_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_4 : 28;          /* [31:4] */
        uint32_t tx_sym_type_tb : 4;         /* [3:0] */
#else
        uint32_t tx_sym_type_tb : 4;         /* [3:0] */
        uint32_t reserve_31_4 : 28;          /* [31:4] */
#endif
    } reg;
    uint32_t value;
} PHY_Tx_Mem_t;

typedef union {
    struct {
#if IS_BIG_ENDIAN
        uint32_t reserve_31_13 : 19;        /* [31:13] */
        uint32_t tx_sym_num : 9;            /* [12:4] */
        uint32_t reserve_3_1 : 3;           /* [3:1] */
        uint32_t slave_tx_flag : 1;         /* [0:0] */
#else
        uint32_t slave_tx_flag : 1;         /* [0:0] */
        uint32_t reserve_3_1 : 3;           /* [3:1] */
        uint32_t tx_sym_num : 9;            /* [12:4] */
        uint32_t reserve_31_13 : 19;        /* [31:13] */
#endif
    } reg;
    uint32_t value;
} PHY_Tx_Status_t;

#endif //QL25_PHY_REG_H

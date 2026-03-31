/******************************************************************************
 * @file     ns.h
 * @brief    NMSIS Core Peripheral Access Layer Header File for
 *           ns SoC
 * @version  V1.00
 * @date     22. Nov 2019
 ******************************************************************************/
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
#ifndef __NS_H__
#define __NS_H__

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ns_core0_feature.h"

/** @addtogroup Nuclei
  * @{
  */

/** @addtogroup NS
  * @{
  */

/** @addtogroup Configuration_of_NMSIS
  * @{
  */

/** \brief SoC Download mode definition */
typedef enum {
    DOWNLOAD_MODE_FLASHXIP = 0,         /*!< Flashxip download mode */
    DOWNLOAD_MODE_FLASH = 1,            /*!< Flash download mode */
    DOWNLOAD_MODE_ILM = 2,              /*!< ilm download mode */
    DOWNLOAD_MODE_DDR = 3,              /*!< ddr download mode */
    DOWNLOAD_MODE_MAX,
} DownloadMode_Type;

/** \brief CPU Hart ID Information */
typedef union  HartID_Info {
  struct {
    volatile  uint32_t  core_id : 1;                  /*!< core ids */
    volatile  uint32_t  processor_id : 9;             /*!< processor id */
    volatile  uint32_t  RECV : 22;                     /*!< reserved */ 
  }b;
  volatile uint32_t d;
} HartID_Info_Typedef;

/** @addtogroup Peripheral_interrupt_number_definition
  * @{
  */
/* =========================================================================================================================== */
/* ================                                Interrupt Number Definition                                ================ */
/* =========================================================================================================================== */

typedef enum IRQn
{
/* =======================================  Nuclei Core Specific Interrupt Numbers  ======================================== */

    Reserved0_IRQn            =   0,              /*!<  Internal reserved */
    Reserved1_IRQn            =   1,              /*!<  Internal reserved */
    Reserved2_IRQn            =   2,              /*!<  Internal reserved */
    SysTimerSW_IRQn           =   3,              /*!<  System Timer SW interrupt */
    Reserved3_IRQn            =   4,              /*!<  Internal reserved */
    Reserved4_IRQn            =   5,              /*!<  Internal reserved */
    Reserved5_IRQn            =   6,              /*!<  Internal reserved */
    SysTimer_IRQn             =   7,              /*!<  System Timer Interrupt */
    Reserved6_IRQn            =   8,              /*!<  Internal reserved */
    Reserved7_IRQn            =   9,              /*!<  Internal reserved */
    Reserved8_IRQn            =  10,              /*!<  Internal reserved */
    Reserved9_IRQn            =  11,              /*!<  Internal reserved */
    Reserved10_IRQn           =  12,              /*!<  Internal reserved */
    Reserved11_IRQn           =  13,              /*!<  Internal reserved */
    Reserved12_IRQn           =  14,              /*!<  Internal reserved */
    Reserved13_IRQn           =  15,              /*!<  Internal reserved */
    Reserved14_IRQn           =  16,              /*!<  Internal reserved */
    Reserved15_IRQn           =  17,              /*!<  Internal reserved */
    Reserved16_IRQn           =  18,              /*!<  Internal reserved */

/* ===========================================  NS Specific Interrupt Numbers  ========================================= */
/* ToDo: add here your device specific external interrupt numbers. 19~1023 is reserved number for user. Maxmum interrupt supported
         could get from clicinfo.NUM_INTERRUPT. According the interrupt handlers defined in startup_Device.s
         eg.: Interrupt for Timer#1       eclic_tim1_handler   ->   TIM1_IRQn */
    
    INTER_CORE_IRQn                   = 19,
    UDMA0_IRQn                   = 20,
    USART0_IRQn                   = 21,
    USART1_IRQn                   = 22,
    USART2_IRQn                   = 23,
    USART3_IRQn                   = 24,
    USART4_IRQn                   = 25,
    I2C0_IRQn                   = 26,
    I2C1_IRQn                   = 27,
    QSPI_XIP0_IRQn                   = 28,
    QSPI1_IRQn                   = 29,
    QSPI2_IRQn                   = 30,
    QSPI3_IRQn                   = 31,
    ADVANCED_TIMER0_IRQn                   = 32,
    ADVANCED_TIMER1_IRQn                   = 33,
    ADVANCED_TIMER2_IRQn                   = 34,
    ADVANCED_TIMER3_IRQn                   = 35,
    BASIC_TIMER0_IRQn                   = 36,
    BASIC_TIMER1_IRQn                   = 37,
    BASIC_TIMER2_IRQn                   = 38,
    BASIC_TIMER3_IRQn                   = 39,
    BASIC_TIMER4_IRQn                   = 40,
    BASIC_TIMER5_IRQn                   = 41,
    BASIC_TIMER6_IRQn                   = 42,
    BASIC_TIMER7_IRQn                   = 43,
    LGPIO0_IRQn                   = 44,
    LGPIO1_IRQn                   = 45,
    LGPIO2_IRQn                   = 46,
    LGPIO3_IRQn                   = 47,
    RTC0_IRQn                   = 48,
    RTC1_IRQn                   = 49,
    RTC2_IRQn                   = 50,
    XKAN0_IRQn                   = 51,
    XKAN1_IRQn                   = 52,
    Reserved17_IRQn  =  53,           /*!<  Internal reserved */
    Reserved18_IRQn  =  54,           /*!<  Internal reserved */
    Reserved19_IRQn  =  55,           /*!<  Internal reserved */
    Reserved20_IRQn  =  56,           /*!<  Internal reserved */
    Reserved21_IRQn  =  57,           /*!<  Internal reserved */
    Reserved22_IRQn  =  58,           /*!<  Internal reserved */
    Reserved23_IRQn  =  59,           /*!<  Internal reserved */
    Reserved24_IRQn  =  60,           /*!<  Internal reserved */
    Reserved25_IRQn  =  61,           /*!<  Internal reserved */
    Reserved26_IRQn  =  62,           /*!<  Internal reserved */
    Reserved27_IRQn  =  63,           /*!<  Internal reserved */
    Reserved28_IRQn  =  64,           /*!<  Internal reserved */
    Reserved29_IRQn  =  65,           /*!<  Internal reserved */
    Reserved30_IRQn  =  66,           /*!<  Internal reserved */
    Reserved31_IRQn  =  67,           /*!<  Internal reserved */
    Reserved32_IRQn  =  68,           /*!<  Internal reserved */
    Reserved33_IRQn  =  69,           /*!<  Internal reserved */
    Reserved34_IRQn  =  70,           /*!<  Internal reserved */
    Reserved35_IRQn  =  71,           /*!<  Internal reserved */
    Reserved36_IRQn  =  72,           /*!<  Internal reserved */
    Reserved37_IRQn  =  73,           /*!<  Internal reserved */
    Reserved38_IRQn  =  74,           /*!<  Internal reserved */
    Reserved39_IRQn  =  75,           /*!<  Internal reserved */
    Reserved40_IRQn  =  76,           /*!<  Internal reserved */
    Reserved41_IRQn  =  77,           /*!<  Internal reserved */
    Reserved42_IRQn  =  78,           /*!<  Internal reserved */
    Reserved43_IRQn  =  79,           /*!<  Internal reserved */
    Reserved44_IRQn  =  80,           /*!<  Internal reserved */
    Reserved45_IRQn  =  81,           /*!<  Internal reserved */
    Reserved46_IRQn  =  82,           /*!<  Internal reserved */
    Reserved47_IRQn  =  83,           /*!<  Internal reserved */
    Reserved48_IRQn  =  84,           /*!<  Internal reserved */
    Reserved49_IRQn  =  85,           /*!<  Internal reserved */
    Reserved50_IRQn  =  86,           /*!<  Internal reserved */
    Reserved51_IRQn  =  87,           /*!<  Internal reserved */
    Reserved52_IRQn  =  88,           /*!<  Internal reserved */
    Reserved53_IRQn  =  89,           /*!<  Internal reserved */
    Reserved54_IRQn  =  90,           /*!<  Internal reserved */
    Reserved55_IRQn  =  91,           /*!<  Internal reserved */
    Reserved56_IRQn  =  92,           /*!<  Internal reserved */
    Reserved57_IRQn  =  93,           /*!<  Internal reserved */
    Reserved58_IRQn  =  94,           /*!<  Internal reserved */
    Reserved59_IRQn  =  95,           /*!<  Internal reserved */
    Reserved60_IRQn  =  96,           /*!<  Internal reserved */
    Reserved61_IRQn  =  97,           /*!<  Internal reserved */
    Reserved62_IRQn  =  98,           /*!<  Internal reserved */
    Reserved63_IRQn  =  99,           /*!<  Internal reserved */
    Reserved64_IRQn  =  100,           /*!<  Internal reserved */
    Reserved65_IRQn  =  101,           /*!<  Internal reserved */
    Reserved66_IRQn  =  102,           /*!<  Internal reserved */
    Reserved67_IRQn  =  103,           /*!<  Internal reserved */
    Reserved68_IRQn  =  104,           /*!<  Internal reserved */
    Reserved69_IRQn  =  105,           /*!<  Internal reserved */
    Reserved70_IRQn  =  106,           /*!<  Internal reserved */
    Reserved71_IRQn  =  107,           /*!<  Internal reserved */
    Reserved72_IRQn  =  108,           /*!<  Internal reserved */
    Reserved73_IRQn  =  109,           /*!<  Internal reserved */
    Reserved74_IRQn  =  110,           /*!<  Internal reserved */
    Reserved75_IRQn  =  111,           /*!<  Internal reserved */
    Reserved76_IRQn  =  112,           /*!<  Internal reserved */
    Reserved77_IRQn  =  113,           /*!<  Internal reserved */
    Reserved78_IRQn  =  114,           /*!<  Internal reserved */
    Reserved79_IRQn  =  115,           /*!<  Internal reserved */
    Reserved80_IRQn  =  116,           /*!<  Internal reserved */
    Reserved81_IRQn  =  117,           /*!<  Internal reserved */
    Reserved82_IRQn  =  118,           /*!<  Internal reserved */
    Reserved83_IRQn  =  119,           /*!<  Internal reserved */
    Reserved84_IRQn  =  120,           /*!<  Internal reserved */
    Reserved85_IRQn  =  121,           /*!<  Internal reserved */
    Reserved86_IRQn  =  122,           /*!<  Internal reserved */
    Reserved87_IRQn  =  123,           /*!<  Internal reserved */
    Reserved88_IRQn  =  124,           /*!<  Internal reserved */
    Reserved89_IRQn  =  125,           /*!<  Internal reserved */
    Reserved90_IRQn  =  126,           /*!<  Internal reserved */
    Reserved91_IRQn  =  127,           /*!<  Internal reserved */
    Reserved92_IRQn  =  128,           /*!<  Internal reserved */
    Reserved93_IRQn  =  129,           /*!<  Internal reserved */
    Reserved94_IRQn  =  130,           /*!<  Internal reserved */
    Reserved95_IRQn  =  131,           /*!<  Internal reserved */
    Reserved96_IRQn  =  132,           /*!<  Internal reserved */
    Reserved97_IRQn  =  133,           /*!<  Internal reserved */
    Reserved98_IRQn  =  134,           /*!<  Internal reserved */
    Reserved99_IRQn  =  135,           /*!<  Internal reserved */
    Reserved100_IRQn  =  136,           /*!<  Internal reserved */
    Reserved101_IRQn  =  137,           /*!<  Internal reserved */
    Reserved102_IRQn  =  138,           /*!<  Internal reserved */
    Reserved103_IRQn  =  139,           /*!<  Internal reserved */
    Reserved104_IRQn  =  140,           /*!<  Internal reserved */
    Reserved105_IRQn  =  141,           /*!<  Internal reserved */
    Reserved106_IRQn  =  142,           /*!<  Internal reserved */
    Reserved107_IRQn  =  143,           /*!<  Internal reserved */
    Reserved108_IRQn  =  144,           /*!<  Internal reserved */
    Reserved109_IRQn  =  145,           /*!<  Internal reserved */

    SOC_INT_MAX,
} IRQn_Type;
/**
  * @}
  */

/** @addtogroup Exception_Code_Definition
  * @{
  */
/* =========================================================================================================================== */
/* ================                                  Exception Code Definition                                ================ */
/* =========================================================================================================================== */

typedef enum EXCn {
/* =======================================  Nuclei N/NX Specific Exception Code  ======================================== */
    InsUnalign_EXCn          =   0,              /*!<  Instruction address misaligned */
    InsAccFault_EXCn         =   1,              /*!<  Instruction access fault */
    IlleIns_EXCn             =   2,              /*!<  Illegal instruction */
    Break_EXCn               =   3,              /*!<  Beakpoint */
    LdAddrUnalign_EXCn       =   4,              /*!<  Load address misaligned */
    LdFault_EXCn             =   5,              /*!<  Load access fault */
    StAddrUnalign_EXCn       =   6,              /*!<  Store or AMO address misaligned */
    StAccessFault_EXCn       =   7,              /*!<  Store or AMO access fault */
    UmodeEcall_EXCn          =   8,              /*!<  Environment call from User mode */
    MmodeEcall_EXCn          =  11,              /*!<  Environment call from Machine mode */
    NMI_EXCn                 = 0xfff,            /*!<  NMI interrupt */
} EXCn_Type;

/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section                           ================ */
/* =========================================================================================================================== */

/* ToDo: set the defines according your Device */
/* ToDo: define the correct core revision */
#if __riscv_xlen == 32

#ifndef __NUCLEI_CORE_REV
#define __NUCLEI_N_REV            0x0104    /*!< Core Revision r1p4 */
#else
#define __NUCLEI_N_REV            __NUCLEI_CORE_REV
#endif
typedef uint32_t addr_xlen   ;

#elif __riscv_xlen == 64

#ifndef __NUCLEI_CORE_REV
#define __NUCLEI_NX_REV           0x0100    /*!< Core Revision r1p0 */
#else
#define __NUCLEI_NX_REV           __NUCLEI_CORE_REV
#endif
typedef uint64_t addr_xlen   ;
#endif /* __riscv_xlen == 64 */

extern volatile HartID_Info_Typedef hart_id;
/* ToDo: define the correct core features for the  */
#define __ECLIC_PRESENT           1                     /*!< Set to 1 if ECLIC is present */
#define __ECLIC_BASEADDR          0x20000UL  /*!< Set to ECLIC baseaddr of your device */
#define __ECLIC_INTNUM            128        /*!< Set to 1 - 1024, total interrupt number of ECLIC Unit */

#define __PLIC_PRESENT            0                     /*!< Set to 1 if PLIC is present */

#define __SYSTIMER_PRESENT        1                     /*!< Set to 1 if System Timer is present */
#define __SYSTIMER_BASEADDR       0x00030000UL  /*!< Set to SysTimer baseaddr of your device */
#define __CLUSTER_IDU_PRESENT     1                       /*!< Set to 1 if cluster idu Unit is present */
#define __CLUSTER_IDU_BASEADDR    0x50000UL /*!< Set to cluster idu baseaddr of your device */
#define __PPI_BASE_ADDR       0x10000000
/*!< Set to 0, 1, or 2, 0 not present, 1 single floating point unit present, 2 double floating point unit present */
#if !defined(__riscv_flen)
#define __FPU_PRESENT             0
#elif __riscv_flen == 32
#define __FPU_PRESENT             1
#else
#define __FPU_PRESENT             2
#endif

/* __riscv_bitmanip/__riscv_dsp/__riscv_vector is introduced
 * in nuclei gcc 10.2 when b/p/v extension compiler option is selected.
 * For example:
 * -march=rv32imacb -mabi=ilp32 : __riscv_bitmanip macro will be defined
 * -march=rv32imacp -mabi=ilp32 : __riscv_dsp macro will be defined
 * -march=rv64imacv -mabi=lp64 : __riscv_vector macro will be defined
 */
#if defined(__riscv_bitmanip)
#define __BITMANIP_PRESENT        1                     /*!< Set to 1 if Bitmainpulation extension is present */
#else
#define __BITMANIP_PRESENT        0                     /*!< Set to 1 if Bitmainpulation extension is present */
#endif
#if defined(__riscv_dsp)
#define __DSP_PRESENT             1                     /*!< Set to 1 if Partial SIMD(DSP) extension is present */
#else
#define __DSP_PRESENT             0                     /*!< Set to 1 if Partial SIMD(DSP) extension is present */
#endif
#if defined(__riscv_vector)
#define __VECTOR_PRESENT          1                     /*!< Set to 1 if Vector extension is present */
#else
#define __VECTOR_PRESENT          0                     /*!< Set to 1 if Vector extension is present */
#endif

#define __PMP_PRESENT             1                     /*!< Set to 1 if PMP is present */
#define __PMP_ENTRY_NUM           16               /*!< Set to 8 or 16, the number of PMP entries */

#define __ICACHE_PRESENT          1                  /*!< Set to 1 if I-Cache is present */

#define __DCACHE_PRESENT          0                  /*!< Set to 1 if D-Cache is present */
#define __CCM_PRESENT           0                       /*!< Set to 1 if Cache Control and Mantainence Unit is present */
#define __Vendor_SysTickConfig    0                            /*!< Set to 1 if different SysTick Config is used */
#define __Vendor_EXCEPTION        0                            /*!< Set to 1 if vendor exception hander is present */

#ifndef __HARTID_OFFSET
/**
 * If your hart index is different to your hartid, you must define this __HARTID_OFFSET macro.
 * For example, if your cpu has 4 harts, and hartid start from 3, so the __HARTID_OFFSET should set to 3.
 * Which means hartid 3-6 means hart index 0-3, this is useful for the timer software interrupt and timer interrupt trigger register location
 */
#define __HARTID_OFFSET             hart_id.b.processor_id
#endif

/* Define boot hart id */
#ifndef BOOT_HARTID
/**
 * Choosen boot hart id in current cluster when in soc system, need to align with the value defined in startup_<Device>.S,
 * should start from 0, taken the mhartid bit 0-7 value
 */
#define BOOT_HARTID                 hart_id.b.core_id
#endif

/** @} */ /* End of group Configuration_of_NMSIS */

#include <nmsis_core.h>                            /*!< Nuclei N/NX class processor and core peripherals */
/* ToDo: include your system_ns.h file
         replace 'Device' with your device name */
#include "system_ns.h"                    /*!< ns System */

/* ========================================  Start of section using anonymous unions  ======================================== */
#if   defined (__GNUC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

#define RTC_FREQ 32768

#define SOC_TIMER_FREQ RTC_FREQ
/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_peripherals
  * @
  */

/****************************************************************************
 * TODO: Platform definitions
 *****************************************************************************/

#define SOC_ECLIC_NUM_INTERRUPTS    128

#define SOC_MTIMER_HANDLER          eclic_mtip_handler
#define SOC_SOFTINT_HANDLER         eclic_msip_handler

#define GPIO_BIT_ALL_ZERO           (0x0)
#define GPIO_BIT_ALL_ONE            (0xFFFFFFFF)

/* enum definitions */
typedef enum {
    DISABLE = 0,
    ENABLE = !DISABLE
} EventStatus, ControlStatus, FunctionalState;

typedef enum {
    FALSE = 0,
    TRUE = !FALSE
} BOOL;

typedef enum {
    RESET = 0,
    SET = 1,
    MAX = 0X7FFFFFFF
} FlagStatus;

typedef enum {
    ERROR = 0,
    SUCCESS = !ERROR
} ErrStatus;

/**
  * \brief  UDMA PeriphCfg Structure definition
  */
typedef struct {
    __IOM uint32_t RX_SADDR;                /*!< RX CHannel UDMA Transfer Address of Associated Buffer */
    __IOM uint32_t RX_SIZE;                 /*!< RX CHannel UDMA Transfer Size of Buffer */
    __IOM uint32_t RX_CFG;                  /*!< RX CHannel UDMA Transfer Configuration */
    __IOM uint32_t TX_SADDR;                /*!< TX CHannel UDMA Transfer Address of Associated Buffer */
    __IOM uint32_t TX_SIZE;                 /*!< TX CHannel UDMA Transfer Size of Buffer */
    __IOM uint32_t TX_CFG;                  /*!< TX CHannel UDMA Transfer Configuration */
    __IOM uint32_t CMD_SADDR;               /*!< TX CHannel UDMA Transfer Address of Associated Buffer */
    __IOM uint32_t CMD_SIZE;                /*!< TX CHannel UDMA Transfer Size of Buffer */
    __IOM uint32_t CMD_CFG;                 /*!< TX CHannel UDMA Transfer Configuration */
} UDMA_PeriphCfg_TypeDef;

/**
  * \brief  UDMA PeriphCfg Structure definition
  */
typedef struct {
    __IOM uint32_t RX_SADDR_H;                /*!< RX CHannel UDMA Transfer High 32bit Address of Associated Buffer */
    __IOM uint32_t TX_SADDR_H;                /*!< TX CHannel UDMA Transfer High 32bit Address of Associated Buffer */
    __IOM uint32_t CMD_SADDR_H;               /*!< CMD CHannel UDMA Transfer High 32bit Address of Associated Buffer */
} UDMA_PeriphCfg_CMD_Hi_TypeDef;

/**
  * \brief  UDMA PeriphCfg Structure definition
  */
typedef struct {
    __IOM uint32_t RX_SADDR_H;                /*!< RX CHannel UDMA Transfer High 32bit Address of Associated Buffer */
    __IOM uint32_t TX_SADDR_H;                /*!< TX CHannel UDMA Transfer High 32bit Address of Associated Buffer */
} UDMA_PeriphCfg_Hi_TypeDef;

typedef struct {
    volatile uint32_t rx_saddr;   /**< RX CHannel uDMA transfer address of associated buffer */
    volatile uint32_t rx_size;    /**< RX CHannel uDMA transfer size of buffer */
    volatile uint32_t rx_cfg;     /**< RX CHannel uDMA transfer configuration */
    volatile uint32_t tx_saddr;   /**<  TX CHannel uDMA transfer address of associated buffer */
    volatile uint32_t tx_size;    /**<  TX CHannel uDMA transfer size of buffer */
    volatile uint32_t tx_cfg;     /**<  TX CHannel uDMA transfer configuration */
    volatile uint32_t cmd_saddr;  /**<  TX CHannel uDMA transfer address of associated buffer */
    volatile uint32_t cmd_size;   /**<  TX CHannel uDMA transfer size of buffer */
    volatile uint32_t cmd_cfg;    /**<  TX CHannel uDMA transfer configuration */
} udma_core_t;

typedef struct {
    __IOM uint32_t CMD_SADDR;   /**< cmd UDMA buffer transfer address register, offset: 0x10 */
    __IOM uint32_t CMD_SIZE;    /**< cmd UDMA buffer transfer size register, offset: 0x14 */
    __IOM uint32_t CMD_CFG;     /**< cmd UDMA transfer configuration register, offset: 0x18 */
} cmd_t;

typedef enum {
    RX_CHANNEL      = 0,
    TX_CHANNEL      = 1,
    COMMAND_CHANNEL = 2
} udma_channel_e;

/* ----------------------------------------------------------------------------
   -- UDMA Common Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UDMA_Register_Masks UDMA Register Masks
 * @
 */
/*! @name RX_SADDR - RX TX UDMA buffer transfer address register */

/**
  * @brief UDMA (UDMA)
  */
typedef struct {
    __IOM uint32_t CG;
    __IOM uint32_t EVENTID;
} UDMA_TypeDef;

typedef struct {
    __IOM uint32_t MSRCADDR;
    __IOM uint32_t MDSTADDR;
    __IOM uint32_t MCTRL;
    __IOM uint32_t RPT;
    __IOM uint32_t MSIZE;
    __IOM uint32_t MSRCADDR_H;
    __IOM uint32_t MDSTADDR_H;
} UDMA_CHx_Cfg_TypeDef;

typedef struct {
    __IOM uint32_t MSRCADDR;
    __IOM uint32_t MDSTADDR;
    __IOM uint32_t MCTRL;
    __IOM uint32_t MSIZE;
    __IOM uint32_t MSRCADDR_H;
    __IOM uint32_t MDSTADDR_H;
} UDMA_PA_CHx_Cfg_TypeDef;

typedef struct {
    __IOM uint32_t CHX_IRQ_EN;
    __IOM uint32_t CHX_IRQ_STAT;
    __IOM uint32_t CHX_IRQ_CLR;
} UDMA_CHx_Irq_TypeDef;

/**
  * \brief  lgpio Register Structure definition
  */
typedef struct {
  __IOM uint32_t  PAD_GRP0_IVAL;                            /*!< Offset: 0x0 RW PAD GRP0 IVAL Register */
  __IOM uint32_t  PAD_GRP0_IEN;                             /*!< Offset: 0x4 RW PAD GRP0 IEN Register */
  __IOM uint32_t  GPIO_GRP0_MODE0;                          /*!< Offset: 0x8 RW GPIO GRP0 MODE0 Register */
  __IOM uint32_t  GPIO_GRP0_MODE1;                          /*!< Offset: 0xc RW GPIO GRP0 MODE1 Register */
  __IOM uint32_t  PAD_GRP0_OVAL;                            /*!< Offset: 0x10 RW PAD GRP0 OVAL Register */
  __IOM uint32_t  PAD_GRP0_RISE_IE;                         /*!< Offset: 0x14 RW PAD GRP0 RISE IE Register */
  __IOM uint32_t  PAD_GRP0_RISE_IP;                         /*!< Offset: 0x18 RW PAD GRP0 RISE IP Register */
  __IOM uint32_t  PAD_GRP0_FALL_IE;                         /*!< Offset: 0x1c RW PAD GRP0 FALL IE Register */
  __IOM uint32_t  PAD_GRP0_FALL_IP;                         /*!< Offset: 0x20 RW PAD GRP0 FALL IP Register */
  __IOM uint32_t  PAD_GRP0_HIGH_IE;                         /*!< Offset: 0x24 RW PAD GRP0 HIGH IE Register */
  __IOM uint32_t  PAD_GRP0_HIGH_IP;                         /*!< Offset: 0x28 RW PAD GRP0 HIGH IP Register */
  __IOM uint32_t  PAD_GRP0_LOW_IE;                          /*!< Offset: 0x2c RW PAD GRP0 LOW IE Register */
  __IOM uint32_t  PAD_GRP0_LOW_IP;                          /*!< Offset: 0x30 RW PAD GRP0 LOW IP Register */
  __IOM uint32_t  PAD_GRP0_IOF_SEL0;                        /*!< Offset: 0x34 RW PAD GRP0 IOF SEL0 Register */
  __IOM uint32_t  PAD_GRP0_IOF_SEL1;                        /*!< Offset: 0x38 RW PAD GRP0 IOF SEL1 Register */
  __IOM uint32_t  PAD_GRP0_IOF_SEL2;                        /*!< Offset: 0x3c RW PAD GRP0 IOF SEL2 Register */
  __IOM uint32_t  PAD_GRP0_OUT_XOR;                         /*!< Offset: 0x40 RW PAD GRP0 OUT XOR Register */
  __IOM uint32_t  PAD_GRP0_POUT_MASK;                       /*!< Offset: 0x44 RW PAD GRP0 POUT MASK Register */
  __IOM uint32_t  PAD_GRP0_SET;                             /*!< Offset: 0x48 RW PAD GRP0 SET Register */
  __IOM uint32_t  PAD_GRP0_CLR;                             /*!< Offset: 0x4c RW PAD GRP0 CLR Register */
  __IOM uint32_t  PAD_GRP0_TOGGLE;                          /*!< Offset: 0x50 RW PAD GRP0 TOGGLE Register */
  __IOM uint32_t  PAD_GRP0_PULL_MODE0;                      /*!< Offset: 0x54 RW PAD GRP0 PULL MODE0 Register */
  __IOM uint32_t  PAD_GRP0_PULL_MODE1;                      /*!< Offset: 0x58 RW PAD GRP0 PULL MODE1 Register */
  __IOM uint32_t  PAD_GRP0_DS0;                             /*!< Offset: 0x5c RW PAD GRP0 DS0 Register */
  __IOM uint32_t  PAD_GRP0_DS1;                             /*!< Offset: 0x60 RW PAD GRP0 DS1 Register */
  __IOM uint32_t  PAD_GRP0_DS2;                             /*!< Offset: 0x64 RW PAD GRP0 DS2 Register */
  __IOM uint32_t  PAD_GRP0_DS3;                             /*!< Offset: 0x68 RW PAD GRP0 DS3 Register */
  __IOM uint32_t  PAD_GRP0_SLEW;                            /*!< Offset: 0x6c RW PAD GRP0 SLEW Register */
  __IOM uint32_t  PAD_GRP0_ST;                              /*!< Offset: 0x70 RW PAD GRP0 ST Register */
  __IOM uint32_t  PAD_GRP0_SW;                              /*!< Offset: 0x74 RW PAD GRP0 SW Register */
  __IM  uint32_t  RESERVECD30[3];                           /*!< Offset: 0x78 RO RESERVECD30[3] Register */
  __IOM uint32_t  PAD_GRP0_EVENT_RISE;                      /*!< Offset: 0x84 RW PAD GRP0 EVENT RISE Register */
  __IOM uint32_t  PAD_GRP0_EVENT_FALL;                      /*!< Offset: 0x88 RW PAD GRP0 EVENT FALL Register */
  __IOM uint32_t  PAD_GRP0_DIGITAL_NOISE_FILTER;            /*!< Offset: 0x8c RW PAD GRP0 DIGITAL NOISE FILTER Register */
  __IOM uint32_t  GPIO_GRP0;                                /*!< Offset: 0x90 RW GPIO GRP0 Register */
  __IOM uint32_t  IP_VERSION;                               /*!< Offset: 0x94 RW IP VERSION Register */
  __IOM uint32_t  PAD_GRP0_EVENT_HIGH;                      /*!< Offset: 0x98 RW PAD GRP0 EVENT HIGH Register */
  __IOM uint32_t  PAD_GRP0_EVENT_LOW;                       /*!< Offset: 0x9c RW PAD GRP0 EVENT LOW Register */
  __IOM uint32_t  DIGITAL_NOISE_FILTER;                     /*!< Offset: 0xa0 RW DIGITAL NOISE FILTER Register */
}LGPIO_TypeDef;

/**
  * \brief  i2c Register Structure definition
  */
typedef struct {
  __IM  uint32_t  RESERVECD[1];                             /*!< Offset: 0x0 RO RESERVECD[1] Register */
  __IOM uint32_t  RX_SIZE;                                  /*!< Offset: 0x4 RW RX SIZE Register */
  __IOM uint32_t  RX_CFG;                                   /*!< Offset: 0x8 RW RX CFG Register */
  __IM  uint32_t  RESERVECD3[1];                            /*!< Offset: 0xc RO RESERVECD3[1] Register */
  __IOM uint32_t  TX_SIZE;                                  /*!< Offset: 0x10 RW TX SIZE Register */
  __IOM uint32_t  TX_CFG;                                   /*!< Offset: 0x14 RW TX CFG Register */
  __IM  uint32_t  RESERVECD6[3];                            /*!< Offset: 0x18 RO RESERVECD6[3] Register */
  __IOM uint32_t  STATUS;                                   /*!< Offset: 0x24 RW STATUS Register */
  __IOM uint32_t  TIMING;                                   /*!< Offset: 0x28 RW TIMING Register */
  __IOM uint32_t  SETUP;                                    /*!< Offset: 0x2c RW SETUP Register */
  __IOM uint32_t  TXDATA;                                   /*!< Offset: 0x30 RW TXDATA Register */
  __IOM uint32_t  RXDATA;                                   /*!< Offset: 0x34 RW RXDATA Register */
  __IOM uint32_t  INT_IE;                                   /*!< Offset: 0x38 RW INT IE Register */
  __IOM uint32_t  SLAVE_ADDRESS1;                           /*!< Offset: 0x3c RW SLAVE ADDRESS1 Register */
  __IM  uint32_t  RESERVECD16[1];                           /*!< Offset: 0x40 RO RESERVECD16[1] Register */
  __IOM uint32_t  SLAVE_ADDRESS2;                           /*!< Offset: 0x44 RW SLAVE ADDRESS2 Register */
  __IOM uint32_t  TIME_OUT;                                 /*!< Offset: 0x48 RW TIME OUT Register */
  __IM  uint32_t  RESERVECD19[6];                           /*!< Offset: 0x4c RO RESERVECD19[6] Register */
  __IOM uint32_t  DMA_RX_DATASIZE;                          /*!< Offset: 0x64 RW DMA RX DATASIZE Register */
  __IOM uint32_t  DMA_TX_DATASIZE;                          /*!< Offset: 0x68 RW DMA TX DATASIZE Register */
  __IM  uint32_t  RESERVECD27[4];                           /*!< Offset: 0x6c RO RESERVECD27[4] Register */
  __IOM uint32_t  TXFIFO_WM;                                /*!< Offset: 0x7c RW TXFIFO WM Register */
  __IOM uint32_t  RXFIFO_WM;                                /*!< Offset: 0x80 RW RXFIFO WM Register */
  __IM  uint32_t  RESERVECD33[5];                           /*!< Offset: 0x84 RO RESERVECD33[5] Register */
  __IOM uint32_t  DEBUG;                                    /*!< Offset: 0x98 RW DEBUG Register */
  __IOM uint32_t  IP_VERSION;                               /*!< Offset: 0x9c RW IP VERSION Register */
  __IOM uint32_t  START_SETUP_TIME;                         /*!< Offset: 0xa0 RW START SETUP TIME Register */
  __IOM uint32_t  START_HOLD_TIME;                          /*!< Offset: 0xa4 RW START HOLD TIME Register */
  __IOM uint32_t  SDA_SETUP_TIME;                           /*!< Offset: 0xa8 RW SDA SETUP TIME Register */
  __IOM uint32_t  SDA_HOLD_TIME;                            /*!< Offset: 0xac RW SDA HOLD TIME Register */
  __IOM uint32_t  SCL_HIGH_PERIOD;                          /*!< Offset: 0xb0 RW SCL HIGH PERIOD Register */
  __IOM uint32_t  SCL_LOW_PERIOD;                           /*!< Offset: 0xb4 RW SCL LOW PERIOD Register */
  __IOM uint32_t  STOP_SETUP_TIME;                          /*!< Offset: 0xb8 RW STOP SETUP TIME Register */
  __IOM uint32_t  BUS_FREE_TIME;                            /*!< Offset: 0xbc RW BUS FREE TIME Register */
  __IM  uint32_t  RESERVECD48[16];                          /*!< Offset: 0xc0 RO RESERVECD48[16] Register */
  __IOM uint32_t  ADVANCED_SETUP;                           /*!< Offset: 0x100 RW ADVANCED SETUP Register */
  __IOM uint32_t  ADVANCED_STATUS;                          /*!< Offset: 0x104 RW ADVANCED STATUS Register */
}I2C_TypeDef;

/**
  * \brief  usart Register Structure definition
  */
typedef struct {
  __IOM uint32_t  TXDATA;                                   /*!< Offset: 0x0 RW TXDATA Register */
  __IOM uint32_t  RXDATA;                                   /*!< Offset: 0x4 RW RXDATA Register */
  __IOM uint32_t  TXCTRL;                                   /*!< Offset: 0x8 RW TXCTRL Register */
  __IOM uint32_t  RXCTRL;                                   /*!< Offset: 0xc RW RXCTRL Register */
  __IOM uint32_t  INT_EN;                                   /*!< Offset: 0x10 RW INT EN Register */
  __IOM uint32_t  STATUS;                                   /*!< Offset: 0x14 RW STATUS Register */
  __IOM uint32_t  DIV;                                      /*!< Offset: 0x18 RW DIV Register */
  __IOM uint32_t  SETUP;                                    /*!< Offset: 0x1c RW SETUP Register */
  __IM  uint32_t  RESERVECD8[1];                            /*!< Offset: 0x20 RO RESERVECD8[1] Register */
  __IOM uint32_t  RX_SIZE;                                  /*!< Offset: 0x24 RW RX SIZE Register */
  __IM  uint32_t  RESERVECD10[2];                           /*!< Offset: 0x28 RO RESERVECD10[2] Register */
  __IOM uint32_t  TX_SIZE;                                  /*!< Offset: 0x30 RW TX SIZE Register */
  __IOM uint32_t  SPI_SLAVE;                                /*!< Offset: 0x34 RW SPI SLAVE Register */
  __IM  uint32_t  RESERVECD14[3];                           /*!< Offset: 0x38 RO RESERVECD14[3] Register */
  __IOM uint32_t  RX_IDLE;                                  /*!< Offset: 0x44 RW RX IDLE Register */
  __IOM uint32_t  RX_WM;                                    /*!< Offset: 0x48 RW RX WM Register */
  __IOM uint32_t  RX_FIFO_LEFT_ENTRY;                       /*!< Offset: 0x4c RW RX FIFO LEFT ENTRY Register */
  __IOM uint32_t  TX_FIFO_LEFT_ENTRY;                       /*!< Offset: 0x50 RW TX FIFO LEFT ENTRY Register */
  __IOM uint32_t  TX_DATASIZE;                              /*!< Offset: 0x54 RW TX DATASIZE Register */
  __IOM uint32_t  RX_DATASIZE;                              /*!< Offset: 0x58 RW RX DATASIZE Register */
  __IM  uint32_t  RESERVECD23[1];                           /*!< Offset: 0x5c RO RESERVECD23[1] Register */
  __IOM uint32_t  SMARTCARD_SETUP;                          /*!< Offset: 0x60 RW SMARTCARD SETUP Register */
  __IOM uint32_t  SMARTCARD_TIMING;                         /*!< Offset: 0x64 RW SMARTCARD TIMING Register */
  __IOM uint32_t  IP_VERSION;                               /*!< Offset: 0x68 RW IP VERSION Register */
  __IM  uint32_t  RESERVECD27[4];                           /*!< Offset: 0x6c RO RESERVECD27[4] Register */
  __IOM uint32_t  ADVANCED_SETUP;                           /*!< Offset: 0x7c RW ADVANCED SETUP Register */
  __IOM uint32_t  ADVANCED_STATUS;                          /*!< Offset: 0x80 RW ADVANCED STATUS Register */
}USART_TypeDef;

/**
  * \brief  qspi Register Structure definition
  */
typedef struct {
  __IOM uint32_t  SCKDIV;                                   /*!< Offset: 0x0 RW SCKDIV Register */
  __IOM uint32_t  SCKMODE;                                  /*!< Offset: 0x4 RW SCKMODE Register */
  __IOM uint32_t  DDR_SCKSAMPLE;                            /*!< Offset: 0x8 RW DDR SCKSAMPLE Register */
  __IOM uint32_t  FORCE;                                    /*!< Offset: 0xc RW FORCE Register */
  __IOM uint32_t  CSID;                                     /*!< Offset: 0x10 RW CSID Register */
  __IOM uint32_t  CSDEF;                                    /*!< Offset: 0x14 RW CSDEF Register */
  __IOM uint32_t  CSMODE;                                   /*!< Offset: 0x18 RW CSMODE Register */
  __IOM uint32_t  VERSION;                                  /*!< Offset: 0x1c RW VERSION Register */
  __IM  uint32_t  RESERVECD8[2];                            /*!< Offset: 0x20 RO RESERVECD8[2] Register */
  __IOM uint32_t  DELAY0;                                   /*!< Offset: 0x28 RW DELAY0 Register */
  __IOM uint32_t  DELAY1;                                   /*!< Offset: 0x2c RW DELAY1 Register */
  __IOM uint32_t  FIFO_NUM;                                 /*!< Offset: 0x30 RW FIFO NUM Register */
  __IOM uint32_t  TSIZE;                                    /*!< Offset: 0x34 RW TSIZE Register */
  __IOM uint32_t  RSIZE;                                    /*!< Offset: 0x38 RW RSIZE Register */
  __IM  uint32_t  RESERVECD15[1];                           /*!< Offset: 0x3c RO RESERVECD15[1] Register */
  __IOM uint32_t  FMT;                                      /*!< Offset: 0x40 RW FMT Register */
  __IM  uint32_t  RESERVECD17[1];                           /*!< Offset: 0x44 RO RESERVECD17[1] Register */
  __IOM uint32_t  TXDATA;                                   /*!< Offset: 0x48 RW TXDATA Register */
  __IOM uint32_t  RXDATA;                                   /*!< Offset: 0x4c RW RXDATA Register */
  __IOM uint32_t  TX_MARK;                                  /*!< Offset: 0x50 RW TX MARK Register */
  __IOM uint32_t  RX_MARK;                                  /*!< Offset: 0x54 RW RX MARK Register */
  __IM  uint32_t  RESERVECD22[6];                           /*!< Offset: 0x58 RO RESERVECD22[6] Register */
  __IOM uint32_t  IE;                                       /*!< Offset: 0x70 RW IE Register */
  __IOM uint32_t  IP;                                       /*!< Offset: 0x74 RW IP Register */
  __IM  uint32_t  RESERVECD30[1];                           /*!< Offset: 0x78 RO RESERVECD30[1] Register */
  __IOM uint32_t  STATUS;                                   /*!< Offset: 0x7c RW STATUS Register */
  __IOM uint32_t  SDR_SCKSAMPLE;                            /*!< Offset: 0x80 RW SDR SCKSAMPLE Register */
  __IOM uint32_t  CR;                                       /*!< Offset: 0x84 RW CR Register */
  __IM  uint32_t  RESERVECD34[18];                          /*!< Offset: 0x88 RO RESERVECD34[18] Register */
  __IOM uint32_t  SYS_DELAY;                                /*!< Offset: 0xd0 RW SYS DELAY Register */
}QSPI_TypeDef;

/**
  * \brief  qspi_xip Register Structure definition
  */
typedef struct {
  __IOM uint32_t  SCKDIV;                                   /*!< Offset: 0x0 RW SCKDIV Register */
  __IOM uint32_t  SCKMODE;                                  /*!< Offset: 0x4 RW SCKMODE Register */
  __IOM uint32_t  DDR_SCKSAMPLE;                            /*!< Offset: 0x8 RW DDR SCKSAMPLE Register */
  __IOM uint32_t  FORCE;                                    /*!< Offset: 0xc RW FORCE Register */
  __IOM uint32_t  CSID;                                     /*!< Offset: 0x10 RW CSID Register */
  __IOM uint32_t  CSDEF;                                    /*!< Offset: 0x14 RW CSDEF Register */
  __IOM uint32_t  CSMODE;                                   /*!< Offset: 0x18 RW CSMODE Register */
  __IOM uint32_t  VERSION;                                  /*!< Offset: 0x1c RW VERSION Register */
  __IOM uint32_t  ADDR_WRAP;                                /*!< Offset: 0x20 RW ADDR WRAP Register */
  __IOM uint32_t  BOUNDARY_CFG;                             /*!< Offset: 0x24 RW BOUNDARY CFG Register */
  __IOM uint32_t  DELAY0;                                   /*!< Offset: 0x28 RW DELAY0 Register */
  __IOM uint32_t  DELAY1;                                   /*!< Offset: 0x2c RW DELAY1 Register */
  __IOM uint32_t  FIFO_NUM;                                 /*!< Offset: 0x30 RW FIFO NUM Register */
  __IOM uint32_t  TSIZE;                                    /*!< Offset: 0x34 RW TSIZE Register */
  __IOM uint32_t  RSIZE;                                    /*!< Offset: 0x38 RW RSIZE Register */
  __IM  uint32_t  RESERVECD15[1];                           /*!< Offset: 0x3c RO RESERVECD15[1] Register */
  __IOM uint32_t  FMT;                                      /*!< Offset: 0x40 RW FMT Register */
  __IM  uint32_t  RESERVECD17[1];                           /*!< Offset: 0x44 RO RESERVECD17[1] Register */
  __IOM uint32_t  TXDATA;                                   /*!< Offset: 0x48 RW TXDATA Register */
  __IOM uint32_t  RXDATA;                                   /*!< Offset: 0x4c RW RXDATA Register */
  __IOM uint32_t  TX_MARK;                                  /*!< Offset: 0x50 RW TX MARK Register */
  __IOM uint32_t  RX_MARK;                                  /*!< Offset: 0x54 RW RX MARK Register */
  __IM  uint32_t  RESERVECD22[2];                           /*!< Offset: 0x58 RO RESERVECD22[2] Register */
  __IOM uint32_t  FCTRL;                                    /*!< Offset: 0x60 RW FCTRL Register */
  __IOM uint32_t  FFMT;                                     /*!< Offset: 0x64 RW FFMT Register */
  __IM  uint32_t  RESERVECD26[2];                           /*!< Offset: 0x68 RO RESERVECD26[2] Register */
  __IOM uint32_t  IE;                                       /*!< Offset: 0x70 RW IE Register */
  __IOM uint32_t  IP;                                       /*!< Offset: 0x74 RW IP Register */
  __IOM uint32_t  FFMT1;                                    /*!< Offset: 0x78 RW FFMT1 Register */
  __IOM uint32_t  STATUS;                                   /*!< Offset: 0x7c RW STATUS Register */
  __IOM uint32_t  SDR_SCKSAMPLE;                            /*!< Offset: 0x80 RW SDR SCKSAMPLE Register */
  __IOM uint32_t  CR;                                       /*!< Offset: 0x84 RW CR Register */
  __IM  uint32_t  RESERVECD34[18];                          /*!< Offset: 0x88 RO RESERVECD34[18] Register */
  __IOM uint32_t  SYS_DELAY;                                /*!< Offset: 0xd0 RW SYS DELAY Register */
}QSPI_XIP_TypeDef;

typedef struct {
    __IOM uint32_t SYSRESET;
    __IOM uint32_t NMI;
    __IOM uint32_t DCLS;
} MISC_CTL_TypeDef;

/**
  * \brief  wwdg Register Structure definition
  */
typedef struct {
  __IOM uint32_t  CR;                                       /*!< Offset: 0x0 RW CR Register */
  __IOM uint32_t  CFR;                                      /*!< Offset: 0x4 RW CFR Register */
  __IOM uint32_t  SR;                                       /*!< Offset: 0x8 RW SR Register */
  __IOM uint32_t  VER;                                      /*!< Offset: 0xc RW VER Register */
}WWDG_TypeDef;

typedef struct {
    __IOM uint32_t CRH;                           /*!< Offset: 0x000 (R/W) RTC Control register high */
    __IOM uint32_t CRL;                           /*!< Offset: 0x004 (R/W) RTC Control register low */
    __IOM uint32_t PRL;                           /*!< Offset: 0x008 (R/W) RTC Prescaler register */
    __IOM uint32_t DIV;                           /*!< Offset: 0x00C (R/W) RTC Prescaler counter register */
    __IOM uint32_t CNTH;                          /*!< Offset: 0x010 (R/W) RTC Counter register high */
    __IOM uint32_t CNTL;                          /*!< Offset: 0x014 (R/W) RTC Counter register low */
    __IOM uint32_t ALRH;                          /*!< Offset: 0x018 (R/W) RTC Alarm register high */
    __IOM uint32_t ALRL;                          /*!< Offset: 0x01C (R/W) RTC Alarm register low */
    __IOM uint32_t CALR;                          /*!< Offset: 0x020 (R/W) RTC Calibration register low */
    __IOM uint32_t VER;                           /*!< Offset: 0x024 (R/W) RTC IP version register */

} RTC_TypeDef;

/**
  * \brief  advanced_timer Register Structure definition
  */
typedef struct {
  __IOM uint32_t  TIM_CR1;                                  /*!< Offset: 0x0 RW TIM CR1 Register */
  __IOM uint32_t  TIM_CR2;                                  /*!< Offset: 0x4 RW TIM CR2 Register */
  __IOM uint32_t  TIM_SMCR;                                 /*!< Offset: 0x8 RW TIM SMCR Register */
  __IOM uint32_t  TIM_DIER;                                 /*!< Offset: 0xc RW TIM DIER Register */
  __IOM uint32_t  TIM_SR;                                   /*!< Offset: 0x10 RW TIM SR Register */
  __IOM uint32_t  TIM_EGR;                                  /*!< Offset: 0x14 RW TIM EGR Register */
  __IOM uint32_t  TIM_CCMR1;                                /*!< Offset: 0x18 RW TIM CCMR1 Register */
  __IOM uint32_t  TIM_CCMR2;                                /*!< Offset: 0x1c RW TIM CCMR2 Register */
  __IOM uint32_t  TIM_CCER;                                 /*!< Offset: 0x20 RW TIM CCER Register */
  __IOM uint32_t  TIM_CNT;                                  /*!< Offset: 0x24 RW TIM CNT Register */
  __IOM uint32_t  TIM_PSC;                                  /*!< Offset: 0x28 RW TIM PSC Register */
  __IOM uint32_t  TIM_ARR;                                  /*!< Offset: 0x2c RW TIM ARR Register */
  __IOM uint32_t  TIM_RCR;                                  /*!< Offset: 0x30 RW TIM RCR Register */
  __IOM uint32_t  TIM_CCR1;                                 /*!< Offset: 0x34 RW TIM CCR1 Register */
  __IOM uint32_t  TIM_CCR2;                                 /*!< Offset: 0x38 RW TIM CCR2 Register */
  __IOM uint32_t  TIM_CCR3;                                 /*!< Offset: 0x3c RW TIM CCR3 Register */
  __IOM uint32_t  TIM_CCR4;                                 /*!< Offset: 0x40 RW TIM CCR4 Register */
  __IOM uint32_t  TIM_BDTR;                                 /*!< Offset: 0x44 RW TIM BDTR Register */
  __IOM uint32_t  TIM_DCR;                                  /*!< Offset: 0x48 RW TIM DCR Register */
  __IOM uint32_t  TIM_DMAR;                                 /*!< Offset: 0x4c RW TIM DMAR Register */
  __IOM uint32_t  TIM_VERSION;                              /*!< Offset: 0x50 RW TIM VERSION Register */
  __IOM uint32_t  TIM_IC_PSC_REP;                           /*!< Offset: 0x54 RW TIM IC PSC REP Register */
  __IOM uint32_t  TIM_AF1;                                  /*!< Offset: 0x58 RW TIM AF1 Register */
  __IOM uint32_t  TIM_AF2;                                  /*!< Offset: 0x5c RW TIM AF2 Register */
  __IOM uint32_t  TIM_TISEL;                                /*!< Offset: 0x60 RW TIM TISEL Register */
  __IOM uint32_t  TIM_OUTPUT_DELAY;                         /*!< Offset: 0x64 RW TIM OUTPUT DELAY Register */
}ADVANCED_TIMER_TypeDef;

/**
  * \brief  basic_timer Register Structure definition
  */
typedef struct {
  __IOM uint32_t  CR1;                                      /*!< Offset: 0x0 RW CR1 Register */
  __IOM uint32_t  CR2;                                      /*!< Offset: 0x4 RW CR2 Register */
  __IM  uint32_t  RESERVECD2[1];                            /*!< Offset: 0x8 RO RESERVECD2[1] Register */
  __IOM uint32_t  DIER;                                     /*!< Offset: 0xc RW DIER Register */
  __IOM uint32_t  SR;                                       /*!< Offset: 0x10 RW SR Register */
  __IOM uint32_t  EGR;                                      /*!< Offset: 0x14 RW EGR Register */
  __IM  uint32_t  RESERVECD6[3];                            /*!< Offset: 0x18 RO RESERVECD6[3] Register */
  __IOM uint32_t  CNT;                                      /*!< Offset: 0x24 RW CNT Register */
  __IOM uint32_t  PSC;                                      /*!< Offset: 0x28 RW PSC Register */
  __IOM uint32_t  ARR;                                      /*!< Offset: 0x2c RW ARR Register */
  __IOM uint32_t  IP_VERSION;                               /*!< Offset: 0x30 RW IP VERSION Register */
}BASIC_TIMER_TypeDef;

/**
  * \brief  xkan Register Structure definition
  */
typedef struct {
  __IOM uint32_t  RELEASE;                                  /*!< Offset: 0x0 RW RELEASE Register */
  __IOM uint32_t  CFG_MCR;                                  /*!< Offset: 0x4 RW CFG MCR Register */
  __IOM uint32_t  CFG_TEST;                                 /*!< Offset: 0x8 RW CFG TEST Register */
  __IOM uint32_t  CFG_PSR;                                  /*!< Offset: 0xc RW CFG PSR Register */
  __IOM uint32_t  CFG_MSR;                                  /*!< Offset: 0x10 RW CFG MSR Register */
  __IOM uint32_t  CFG_GLOBAL_FILTER;                        /*!< Offset: 0x14 RW CFG GLOBAL FILTER Register */
  __IOM uint32_t  CFG_ST_FILTER;                            /*!< Offset: 0x18 RW CFG ST FILTER Register */
  __IOM uint32_t  CFG_EX_FILTER;                            /*!< Offset: 0x1c RW CFG EX FILTER Register */
  __IOM uint32_t  CFG_STIDM;                                /*!< Offset: 0x20 RW CFG STIDM Register */
  __IOM uint32_t  CFG_EXIDM;                                /*!< Offset: 0x24 RW CFG EXIDM Register */
  __IOM uint32_t  CFG_NBTR;                                 /*!< Offset: 0x28 RW CFG NBTR Register */
  __IOM uint32_t  CFG_DBTR;                                 /*!< Offset: 0x2c RW CFG DBTR Register */
  __IOM uint32_t  CFG_ISO_MODE;                             /*!< Offset: 0x30 RW CFG ISO MODE Register */
  __IM  uint32_t  RESERVECD13[3];                           /*!< Offset: 0x34 RO RESERVECD13[3] Register */
  __IOM uint32_t  CFG_RXDATL_RSP;                           /*!< Offset: 0x40 RW CFG RXDATL RSP Register */
  __IOM uint32_t  CFG_RXDATH_RSP;                           /*!< Offset: 0x44 RW CFG RXDATH RSP Register */
  __IOM uint32_t  CFG_RXFIFO0;                              /*!< Offset: 0x48 RW CFG RXFIFO0 Register */
  __IOM uint32_t  CFG_RXFIFO0_STATUS;                       /*!< Offset: 0x4c RW CFG RXFIFO0 STATUS Register */
  __IOM uint32_t  CFG_RXFIFO0_ACK;                          /*!< Offset: 0x50 RW CFG RXFIFO0 ACK Register */
  __IOM uint32_t  CFG_RXBUF;                                /*!< Offset: 0x54 RW CFG RXBUF Register */
  __IOM uint32_t  CFG_RXFIFO1;                              /*!< Offset: 0x58 RW CFG RXFIFO1 Register */
  __IOM uint32_t  CFG_RXFIFO1_STATUS;                       /*!< Offset: 0x5c RW CFG RXFIFO1 STATUS Register */
  __IOM uint32_t  CFG_RXFIFO1_ACK;                          /*!< Offset: 0x60 RW CFG RXFIFO1 ACK Register */
  __IOM uint32_t  CFG_RX_DSIZE;                             /*!< Offset: 0x64 RW CFG RX DSIZE Register */
  __IM  uint32_t  RESERVECD26[2];                           /*!< Offset: 0x68 RO RESERVECD26[2] Register */
  __IOM uint32_t  CFG_TXBUF;                                /*!< Offset: 0x70 RW CFG TXBUF Register */
  __IOM uint32_t  CFG_TXFIFO_STATUS;                        /*!< Offset: 0x74 RW CFG TXFIFO STATUS Register */
  __IOM uint32_t  CFG_TX_DSIZE;                             /*!< Offset: 0x78 RW CFG TX DSIZE Register */
  __IOM uint32_t  CFG_TXBAR;                                /*!< Offset: 0x7c RW CFG TXBAR Register */
  __IOM uint32_t  CFG_TXABORT_REQ;                          /*!< Offset: 0x80 RW CFG TXABORT REQ Register */
  __IOM uint32_t  CFG_TXABORT_FINISH;                       /*!< Offset: 0x84 RW CFG TXABORT FINISH Register */
  __IOM uint32_t  CFG_TXREQ_PENDING;                        /*!< Offset: 0x88 RW CFG TXREQ PENDING Register */
  __IOM uint32_t  CFG_TXB_TRANS;                            /*!< Offset: 0x8c RW CFG TXB TRANS Register */
  __IOM uint32_t  CFG_TIME_STAMP;                           /*!< Offset: 0x90 RW CFG TIME STAMP Register */
  __IOM uint32_t  CFG_TIME_DIV;                             /*!< Offset: 0x94 RW CFG TIME DIV Register */
  __IOM uint32_t  CFG_TRANS_DELAY_COMP;                     /*!< Offset: 0x98 RW CFG TRANS DELAY COMP Register */
  __IOM uint32_t  CFG_ESR;                                  /*!< Offset: 0x9c RW CFG ESR Register */
  __IOM uint32_t  CFG_TRANS_ERR;                            /*!< Offset: 0xa0 RW CFG TRANS ERR Register */
  __IM  uint32_t  RESERVECD41[4];                           /*!< Offset: 0xa4 RO RESERVECD41[4] Register */
  __IOM uint32_t  CFG_INT_EN;                               /*!< Offset: 0xb4 RW CFG INT EN Register */
  __IOM uint32_t  CFG_INT_STATUS;                           /*!< Offset: 0xb8 RW CFG INT STATUS Register */
}XKAN_TypeDef;

/* =========================================  End of section using anonymous unions  ========================================= */
#if defined (__GNUC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/* =========================================================================================================================== */
/* ================                          Device Specific Peripheral Address Map                           ================ */
/* =========================================================================================================================== */

/* ToDo: add here your device peripherals base addresses
         following is an example for timer */
/** @addtogroup Device_Peripheral_peripheralAddr
  * @{
  */
/* Peripheral and SRAM base address */
#define QSPI_FLASH_BASE                           (0x20000000UL)     /*!< (FLASH     ) Base Address */
#define ONCHIP_ILM_BASE                       (0x08000000UL)          /*!< (ILM       ) Base Address */
#define ONCHIP_ILM_SIZE                       (0x10000)
#define ONCHIP_DLM_BASE                       (0x08010000UL)          /*!< (DLM       ) Base Address */
#define ONCHIP_DLM_SIZE                       (0x8000)

/* Peripheral memory map */

#define MISC_CTL_BASE                             (0x12000000UL)

#define CORE0_SLV_LM_BASE                         (0x8000000UL)

#define CORE1_SLV_LM_BASE                         (0x9000000UL)

#define UDMA0_BASE                                (0x10020000UL)

#define USART0_BASE                               (0x10030000UL)

#define USART1_BASE                               (0x10031000UL)

#define USART2_BASE                               (0x10032000UL)

#define USART3_BASE                               (0x10033000UL)

#define USART4_BASE                               (0x10034000UL)

#define I2C0_BASE                                 (0x10040000UL)

#define I2C1_BASE                                 (0x10041000UL)

#define QSPI_XIP0_BASE                            (0x10060000UL)

#define QSPI_XIP0_MEM_BASE                        (0x20000000UL)

#define QSPI_XIP0_XIP_BASE                        (0x20000000UL)

#define QSPI1_BASE                                (0x1c000000UL)

#define QSPI2_BASE                                (0x1c001000UL)

#define QSPI3_BASE                                (0x1c002000UL)

#define ADVANCED_TIMER0_BASE                      (0x10080000UL)

#define ADVANCED_TIMER1_BASE                      (0x10081000UL)

#define ADVANCED_TIMER2_BASE                      (0x10082000UL)

#define ADVANCED_TIMER3_BASE                      (0x10083000UL)

#define BASIC_TIMER0_BASE                         (0x100e0000UL)

#define BASIC_TIMER1_BASE                         (0x100e1000UL)

#define BASIC_TIMER2_BASE                         (0x100e2000UL)

#define BASIC_TIMER3_BASE                         (0x100e3000UL)

#define BASIC_TIMER4_BASE                         (0x100e4000UL)

#define BASIC_TIMER5_BASE                         (0x100e5000UL)

#define BASIC_TIMER6_BASE                         (0x100e6000UL)

#define BASIC_TIMER7_BASE                         (0x100e7000UL)

#define LGPIO0_BASE                               (0x100a0000UL)

#define LGPIO1_BASE                               (0x100a1000UL)

#define LGPIO2_BASE                               (0x100a2000UL)

#define LGPIO3_BASE                               (0x100a3000UL)

#define WWDG0_BASE                                (0x100c0000UL)

#define WWDG1_BASE                                (0x100c1000UL)

#define RTC0_BASE                                 (0x100d0000UL)

#define RTC1_BASE                                 (0x100d1000UL)

#define RTC2_BASE                                 (0x100d2000UL)

#define XKAN0_BASE                                (0x11000000UL)

#define XKAN0_RAM_BASE                            (0x40000000UL)

#define XKAN0_RAM_SIZE                            (0x2000UL)
#define XKAN0_RAM_MASK                            (BITS(31,13))

#define XKAN1_BASE                                (0x11001000UL)

#define XKAN1_RAM_BASE                            (0x40002000UL)

#define XKAN1_RAM_SIZE                            (0x2000UL)
#define XKAN1_RAM_MASK                            (BITS(31,13))

#define SRAM0_MEM_BASE                            (0x30000000UL)

#define SRAM0_MEM_SIZE                            (0x20000UL)
#define SRAM0_MEM_MASK                            (BITS(31,17))

#define SRAM1_MEM_BASE                            (0x30020000UL)

#define SRAM1_MEM_SIZE                            (0x20000UL)
#define SRAM1_MEM_MASK                            (BITS(31,17))

#define SRAM2_MEM_BASE                            (0x30040000UL)

#define SRAM2_MEM_SIZE                            (0x10000UL)
#define SRAM2_MEM_MASK                            (BITS(31,16))

#define SRAM3_MEM_BASE                            (0x30060000UL)

#define SRAM3_MEM_SIZE                            (0x10000UL)
#define SRAM3_MEM_MASK                            (BITS(31,16))

#define AHB_MST0_SYNC_BASE                        (0x14320000UL)

#define AHB_MST1_SYNC_BASE                        (0x14320400UL)

#define AHB_SLV0_RATIO_MEM_BASE                   (0x18000000UL)

#define AHB_SLV1_RATIO_MEM_BASE                   (0x18004000UL)

#define AHB_SLV2_RATIO_MEM_BASE                   (0x18008000UL)

#define AHB_SLV3_RATIO_MEM_BASE                   (0x18010000UL)

#define APB_SLV0_RATIO_MEM_BASE                   (0x13000000UL)

#define APB_SLV1_RATIO_MEM_BASE                   (0x13004000UL)

#define APB_SLV2_RATIO_MEM_BASE                   (0x13008000UL)

#define SOC_GLUE_BASE                             (0x10000000UL)

#define IDU_BASE                                  (0x10010000UL)

#define IOMUX_BASE                                (0x14000000UL)

#define UDMA0_CH0_M2M_CFG_BASE                 (UDMA0_BASE + 0x8)
#define UDMA0_CH0_M2M_IRQ_BASE                 (UDMA0_BASE + 0x800)

#define UDMA0_CH0_PA2M_CFG_BASE                (UDMA0_BASE + 0x400)
#define UDMA0_CH0_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa00)
#define UDMA0_CH1_PA2M_CFG_BASE                (UDMA0_BASE + 0x418)
#define UDMA0_CH1_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa0c)
#define UDMA0_CH2_PA2M_CFG_BASE                (UDMA0_BASE + 0x430)
#define UDMA0_CH2_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa18)
#define UDMA0_CH3_PA2M_CFG_BASE                (UDMA0_BASE + 0x448)
#define UDMA0_CH3_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa24)
#define UDMA0_CH4_PA2M_CFG_BASE                (UDMA0_BASE + 0x460)
#define UDMA0_CH4_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa30)
#define UDMA0_CH5_PA2M_CFG_BASE                (UDMA0_BASE + 0x478)
#define UDMA0_CH5_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa3c)
#define UDMA0_CH6_PA2M_CFG_BASE                (UDMA0_BASE + 0x490)
#define UDMA0_CH6_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa48)
#define UDMA0_CH7_PA2M_CFG_BASE                (UDMA0_BASE + 0x4a8)
#define UDMA0_CH7_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa54)
#define UDMA0_CH8_PA2M_CFG_BASE                (UDMA0_BASE + 0x4c0)
#define UDMA0_CH8_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa60)
#define UDMA0_CH9_PA2M_CFG_BASE                (UDMA0_BASE + 0x4d8)
#define UDMA0_CH9_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa6c)
#define UDMA0_CH10_PA2M_CFG_BASE                (UDMA0_BASE + 0x4f0)
#define UDMA0_CH10_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa78)
#define UDMA0_CH11_PA2M_CFG_BASE                (UDMA0_BASE + 0x508)
#define UDMA0_CH11_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa84)
#define UDMA0_CH12_PA2M_CFG_BASE                (UDMA0_BASE + 0x520)
#define UDMA0_CH12_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa90)
#define UDMA0_CH13_PA2M_CFG_BASE                (UDMA0_BASE + 0x538)
#define UDMA0_CH13_PA2M_IRQ_BASE              (UDMA0_BASE + 0xa9c)
#define UDMA0_CH14_PA2M_CFG_BASE                (UDMA0_BASE + 0x550)
#define UDMA0_CH14_PA2M_IRQ_BASE              (UDMA0_BASE + 0xaa8)
#define UDMA0_CH15_PA2M_CFG_BASE                (UDMA0_BASE + 0x568)
#define UDMA0_CH15_PA2M_IRQ_BASE              (UDMA0_BASE + 0xab4)
#define UDMA0_CH16_PA2M_CFG_BASE                (UDMA0_BASE + 0x580)
#define UDMA0_CH16_PA2M_IRQ_BASE              (UDMA0_BASE + 0xac0)
#define UDMA0_CH17_PA2M_CFG_BASE                (UDMA0_BASE + 0x598)
#define UDMA0_CH17_PA2M_IRQ_BASE              (UDMA0_BASE + 0xacc)
#define UDMA0_CH18_PA2M_CFG_BASE                (UDMA0_BASE + 0x5b0)
#define UDMA0_CH18_PA2M_IRQ_BASE              (UDMA0_BASE + 0xad8)
#define UDMA0_CH19_PA2M_CFG_BASE                (UDMA0_BASE + 0x5c8)
#define UDMA0_CH19_PA2M_IRQ_BASE              (UDMA0_BASE + 0xae4)
#define UDMA0_CH20_PA2M_CFG_BASE                (UDMA0_BASE + 0x5e0)
#define UDMA0_CH20_PA2M_IRQ_BASE              (UDMA0_BASE + 0xaf0)
#define UDMA0_CH21_PA2M_CFG_BASE                (UDMA0_BASE + 0x5f8)
#define UDMA0_CH21_PA2M_IRQ_BASE              (UDMA0_BASE + 0xafc)
#define UDMA0_CH22_PA2M_CFG_BASE                (UDMA0_BASE + 0x610)
#define UDMA0_CH22_PA2M_IRQ_BASE              (UDMA0_BASE + 0xb08)
#define UDMA0_CH23_PA2M_CFG_BASE                (UDMA0_BASE + 0x628)
#define UDMA0_CH23_PA2M_IRQ_BASE              (UDMA0_BASE + 0xb14)
#define UDMA0_CH24_PA2M_CFG_BASE                (UDMA0_BASE + 0x640)
#define UDMA0_CH24_PA2M_IRQ_BASE              (UDMA0_BASE + 0xb20)
#define UDMA0_CH25_PA2M_CFG_BASE                (UDMA0_BASE + 0x658)
#define UDMA0_CH25_PA2M_IRQ_BASE              (UDMA0_BASE + 0xb2c)
#define UDMA0_CH26_PA2M_CFG_BASE                (UDMA0_BASE + 0x670)
#define UDMA0_CH26_PA2M_IRQ_BASE              (UDMA0_BASE + 0xb38)
#define UDMA0_CH27_PA2M_CFG_BASE                (UDMA0_BASE + 0x688)
#define UDMA0_CH27_PA2M_IRQ_BASE              (UDMA0_BASE + 0xb44)

/** @} */ /* End of group Device_Peripheral_peripheralAddr */

/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */

/* ToDo: add here your device peripherals pointer definitions
         following is an example for timer */
/** @addtogroup Device_Peripheral_declaration
  * @{
  */

#define MISC_CTL                                  ((MISC_CTL_TypeDef *) MISC_CTL_BASE)

#define CORE0_SLV_LM                              ((N300_SLV_LM_TypeDef *) CORE0_SLV_LM_BASE)

#define CORE1_SLV_LM                              ((N300_SLV_LM_TypeDef *) CORE1_SLV_LM_BASE)

#define UDMA0                                     ((UDMA_TypeDef *) UDMA0_BASE)

#define USART0                                    ((USART_TypeDef *) USART0_BASE)

#define USART1                                    ((USART_TypeDef *) USART1_BASE)

#define USART2                                    ((USART_TypeDef *) USART2_BASE)

#define USART3                                    ((USART_TypeDef *) USART3_BASE)

#define USART4                                    ((USART_TypeDef *) USART4_BASE)

#define I2C0                                      ((I2C_TypeDef *) I2C0_BASE)

#define I2C1                                      ((I2C_TypeDef *) I2C1_BASE)

#define QSPI_XIP0                                 ((QSPI_XIP_TypeDef *) QSPI_XIP0_BASE)

#define QSPI_XIP0_MEM                             ((QSPI_XIP0_MEM_TypeDef *) QSPI_XIP0_MEM_BASE)

#define QSPI_XIP0_XIP                             ((SPI_TypeDef *) QSPI_XIP0_XIP_BASE)

#define QSPI1                                     ((QSPI_TypeDef *) QSPI1_BASE)

#define QSPI2                                     ((QSPI_TypeDef *) QSPI2_BASE)

#define QSPI3                                     ((QSPI_TypeDef *) QSPI3_BASE)

#define ADVANCED_TIMER0                           ((ADVANCED_TIMER_TypeDef *) ADVANCED_TIMER0_BASE)

#define ADVANCED_TIMER1                           ((ADVANCED_TIMER_TypeDef *) ADVANCED_TIMER1_BASE)

#define ADVANCED_TIMER2                           ((ADVANCED_TIMER_TypeDef *) ADVANCED_TIMER2_BASE)

#define ADVANCED_TIMER3                           ((ADVANCED_TIMER_TypeDef *) ADVANCED_TIMER3_BASE)

#define BASIC_TIMER0                              ((BASIC_TIMER_TypeDef *) BASIC_TIMER0_BASE)

#define BASIC_TIMER1                              ((BASIC_TIMER_TypeDef *) BASIC_TIMER1_BASE)

#define BASIC_TIMER2                              ((BASIC_TIMER_TypeDef *) BASIC_TIMER2_BASE)

#define BASIC_TIMER3                              ((BASIC_TIMER_TypeDef *) BASIC_TIMER3_BASE)

#define BASIC_TIMER4                              ((BASIC_TIMER_TypeDef *) BASIC_TIMER4_BASE)

#define BASIC_TIMER5                              ((BASIC_TIMER_TypeDef *) BASIC_TIMER5_BASE)

#define BASIC_TIMER6                              ((BASIC_TIMER_TypeDef *) BASIC_TIMER6_BASE)

#define BASIC_TIMER7                              ((BASIC_TIMER_TypeDef *) BASIC_TIMER7_BASE)

#define LGPIO0                                    ((LGPIO_TypeDef *) LGPIO0_BASE)

#define LGPIO1                                    ((LGPIO_TypeDef *) LGPIO1_BASE)

#define LGPIO2                                    ((LGPIO_TypeDef *) LGPIO2_BASE)

#define LGPIO3                                    ((LGPIO_TypeDef *) LGPIO3_BASE)

#define WWDG0                                     ((WWDG_TypeDef *) WWDG0_BASE)

#define WWDG1                                     ((WWDG_TypeDef *) WWDG1_BASE)

#define RTC0                                      ((RTC_TypeDef *) RTC0_BASE)

#define RTC1                                      ((RTC_TypeDef *) RTC1_BASE)

#define RTC2                                      ((RTC_TypeDef *) RTC2_BASE)

#define XKAN0                                     ((XKAN_TypeDef *) XKAN0_BASE)

#define XKAN0_RAM                                 ((XKAN_RAM_TypeDef *) XKAN0_RAM_BASE)

#define XKAN1                                     ((XKAN_TypeDef *) XKAN1_BASE)

#define XKAN1_RAM                                 ((XKAN_RAM_TypeDef *) XKAN1_RAM_BASE)

#define SRAM0_MEM                                 ((SRAM_MEM_TypeDef *) SRAM0_MEM_BASE)

#define SRAM1_MEM                                 ((SRAM_MEM_TypeDef *) SRAM1_MEM_BASE)

#define SRAM2_MEM                                 ((SRAM_MEM_TypeDef *) SRAM2_MEM_BASE)

#define SRAM3_MEM                                 ((SRAM_MEM_TypeDef *) SRAM3_MEM_BASE)

#define AHB_MST0_SYNC                             ((DUMMY_MASTER_TypeDef *) AHB_MST0_SYNC_BASE)

#define AHB_MST1_SYNC                             ((DUMMY_MASTER_TypeDef *) AHB_MST1_SYNC_BASE)

#define AHB_SLV0_RATIO_MEM                        ((DUMMY_SLAVE_MEM_TypeDef *) AHB_SLV0_RATIO_MEM_BASE)

#define AHB_SLV1_RATIO_MEM                        ((DUMMY_SLAVE_MEM_TypeDef *) AHB_SLV1_RATIO_MEM_BASE)

#define AHB_SLV2_RATIO_MEM                        ((DUMMY_SLAVE_MEM_TypeDef *) AHB_SLV2_RATIO_MEM_BASE)

#define AHB_SLV3_RATIO_MEM                        ((DUMMY_SLAVE_MEM_TypeDef *) AHB_SLV3_RATIO_MEM_BASE)

#define APB_SLV0_RATIO_MEM                        ((DUMMY_SLAVE_MEM_TypeDef *) APB_SLV0_RATIO_MEM_BASE)

#define APB_SLV1_RATIO_MEM                        ((DUMMY_SLAVE_MEM_TypeDef *) APB_SLV1_RATIO_MEM_BASE)

#define APB_SLV2_RATIO_MEM                        ((DUMMY_SLAVE_MEM_TypeDef *) APB_SLV2_RATIO_MEM_BASE)

#define SOC_GLUE                                  ((SOC_GLUE_TypeDef *) SOC_GLUE_BASE)

#define IDU                                       ((IDU_TypeDef *) IDU_BASE)

/**
  * @}
  */

/** @addtogroup UDMA_Information
  * @{
  */
#define UDMA0_M2M_CH0                       ((UDMA_CHxCfg_TypeDef *) UDMA0_CH0_M2M_CFG_BASE)
#define UDMA0_CH0_M2M_IRQ               ((UDMA_CHx_IRQ_TypeDef *) UDMA0_CH0_M2M_IRQ_BASE)
#define UDMA0_PAM2M_CH0                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH0_PA2M_CFG_BASE)
#define UDMA0_CH0_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH0_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH1                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH1_PA2M_CFG_BASE)
#define UDMA0_CH1_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH1_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH2                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH2_PA2M_CFG_BASE)
#define UDMA0_CH2_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH2_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH3                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH3_PA2M_CFG_BASE)
#define UDMA0_CH3_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH3_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH4                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH4_PA2M_CFG_BASE)
#define UDMA0_CH4_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH4_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH5                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH5_PA2M_CFG_BASE)
#define UDMA0_CH5_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH5_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH6                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH6_PA2M_CFG_BASE)
#define UDMA0_CH6_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH6_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH7                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH7_PA2M_CFG_BASE)
#define UDMA0_CH7_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH7_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH8                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH8_PA2M_CFG_BASE)
#define UDMA0_CH8_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH8_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH9                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH9_PA2M_CFG_BASE)
#define UDMA0_CH9_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH9_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH10                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH10_PA2M_CFG_BASE)
#define UDMA0_CH10_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH10_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH11                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH11_PA2M_CFG_BASE)
#define UDMA0_CH11_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH11_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH12                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH12_PA2M_CFG_BASE)
#define UDMA0_CH12_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH12_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH13                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH13_PA2M_CFG_BASE)
#define UDMA0_CH13_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH13_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH14                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH14_PA2M_CFG_BASE)
#define UDMA0_CH14_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH14_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH15                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH15_PA2M_CFG_BASE)
#define UDMA0_CH15_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH15_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH16                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH16_PA2M_CFG_BASE)
#define UDMA0_CH16_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH16_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH17                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH17_PA2M_CFG_BASE)
#define UDMA0_CH17_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH17_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH18                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH18_PA2M_CFG_BASE)
#define UDMA0_CH18_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH18_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH19                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH19_PA2M_CFG_BASE)
#define UDMA0_CH19_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH19_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH20                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH20_PA2M_CFG_BASE)
#define UDMA0_CH20_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH20_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH21                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH21_PA2M_CFG_BASE)
#define UDMA0_CH21_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH21_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH22                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH22_PA2M_CFG_BASE)
#define UDMA0_CH22_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH22_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH23                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH23_PA2M_CFG_BASE)
#define UDMA0_CH23_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH23_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH24                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH24_PA2M_CFG_BASE)
#define UDMA0_CH24_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH24_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH25                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH25_PA2M_CFG_BASE)
#define UDMA0_CH25_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH25_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH26                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH26_PA2M_CFG_BASE)
#define UDMA0_CH26_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH26_PA2M_IRQ_BASE)
#define UDMA0_PAM2M_CH27                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH27_PA2M_CFG_BASE)
#define UDMA0_CH27_PA2M_IRQ                     ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH27_PA2M_IRQ_BASE)

/******************** udma0 infomation ********************/

#define USART0_USART_DMA_TX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH8_PA2M_CFG_BASE)
#define USART0_USART_DMA_TX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH8_PA2M_IRQ_BASE)
#define UDMA_SEL_USART0_USART_DMA_TX                      PA2M_PER_CHANNEL0

#define USART0_USART_DMA_RX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH9_PA2M_CFG_BASE)
#define USART0_USART_DMA_RX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH9_PA2M_IRQ_BASE)
#define UDMA_SEL_USART0_USART_DMA_RX                      PA2M_PER_CHANNEL0

#define USART1_USART_DMA_TX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH10_PA2M_CFG_BASE)
#define USART1_USART_DMA_TX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH10_PA2M_IRQ_BASE)
#define UDMA_SEL_USART1_USART_DMA_TX                      PA2M_PER_CHANNEL0

#define USART1_USART_DMA_RX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH11_PA2M_CFG_BASE)
#define USART1_USART_DMA_RX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH11_PA2M_IRQ_BASE)
#define UDMA_SEL_USART1_USART_DMA_RX                      PA2M_PER_CHANNEL0

#define USART2_USART_DMA_TX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH12_PA2M_CFG_BASE)
#define USART2_USART_DMA_TX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH12_PA2M_IRQ_BASE)
#define UDMA_SEL_USART2_USART_DMA_TX                      PA2M_PER_CHANNEL0

#define USART2_USART_DMA_RX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH13_PA2M_CFG_BASE)
#define USART2_USART_DMA_RX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH13_PA2M_IRQ_BASE)
#define UDMA_SEL_USART2_USART_DMA_RX                      PA2M_PER_CHANNEL0

#define USART3_USART_DMA_TX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH14_PA2M_CFG_BASE)
#define USART3_USART_DMA_TX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH14_PA2M_IRQ_BASE)
#define UDMA_SEL_USART3_USART_DMA_TX                      PA2M_PER_CHANNEL0

#define USART3_USART_DMA_RX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH15_PA2M_CFG_BASE)
#define USART3_USART_DMA_RX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH15_PA2M_IRQ_BASE)
#define UDMA_SEL_USART3_USART_DMA_RX                      PA2M_PER_CHANNEL0

#define USART4_USART_DMA_TX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH16_PA2M_CFG_BASE)
#define USART4_USART_DMA_TX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH16_PA2M_IRQ_BASE)
#define UDMA_SEL_USART4_USART_DMA_TX                      PA2M_PER_CHANNEL0

#define USART4_USART_DMA_RX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH17_PA2M_CFG_BASE)
#define USART4_USART_DMA_RX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH17_PA2M_IRQ_BASE)
#define UDMA_SEL_USART4_USART_DMA_RX                      PA2M_PER_CHANNEL0

#define I2C0_I2C_DMA_TX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH18_PA2M_CFG_BASE)
#define I2C0_I2C_DMA_TX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH18_PA2M_IRQ_BASE)
#define UDMA_SEL_I2C0_I2C_DMA_TX                      PA2M_PER_CHANNEL0

#define I2C0_I2C_DMA_RX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH19_PA2M_CFG_BASE)
#define I2C0_I2C_DMA_RX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH19_PA2M_IRQ_BASE)
#define UDMA_SEL_I2C0_I2C_DMA_RX                      PA2M_PER_CHANNEL0

#define I2C1_I2C_DMA_TX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH20_PA2M_CFG_BASE)
#define I2C1_I2C_DMA_TX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH20_PA2M_IRQ_BASE)
#define UDMA_SEL_I2C1_I2C_DMA_TX                      PA2M_PER_CHANNEL0

#define I2C1_I2C_DMA_RX_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH21_PA2M_CFG_BASE)
#define I2C1_I2C_DMA_RX_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH21_PA2M_IRQ_BASE)
#define UDMA_SEL_I2C1_I2C_DMA_RX                      PA2M_PER_CHANNEL0

#define QSPI1_TX_DMA_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH22_PA2M_CFG_BASE)
#define QSPI1_TX_DMA_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH22_PA2M_IRQ_BASE)
#define UDMA_SEL_QSPI1_TX_DMA                      PA2M_PER_CHANNEL0

#define QSPI1_RX_DMA_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH23_PA2M_CFG_BASE)
#define QSPI1_RX_DMA_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH23_PA2M_IRQ_BASE)
#define UDMA_SEL_QSPI1_RX_DMA                      PA2M_PER_CHANNEL0

#define QSPI2_TX_DMA_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH24_PA2M_CFG_BASE)
#define QSPI2_TX_DMA_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH24_PA2M_IRQ_BASE)
#define UDMA_SEL_QSPI2_TX_DMA                      PA2M_PER_CHANNEL0

#define QSPI2_RX_DMA_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH25_PA2M_CFG_BASE)
#define QSPI2_RX_DMA_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH25_PA2M_IRQ_BASE)
#define UDMA_SEL_QSPI2_RX_DMA                      PA2M_PER_CHANNEL0

#define QSPI3_TX_DMA_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH26_PA2M_CFG_BASE)
#define QSPI3_TX_DMA_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH26_PA2M_IRQ_BASE)
#define UDMA_SEL_QSPI3_TX_DMA                      PA2M_PER_CHANNEL0

#define QSPI3_RX_DMA_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH27_PA2M_CFG_BASE)
#define QSPI3_RX_DMA_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH27_PA2M_IRQ_BASE)
#define UDMA_SEL_QSPI3_RX_DMA                      PA2M_PER_CHANNEL0

#define ADVANCED_TIMER0_COM_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH0_PA2M_CFG_BASE)
#define ADVANCED_TIMER0_COM_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH0_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER0_COM                      PA2M_PER_CHANNEL0

#define ADVANCED_TIMER0_TRG_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH1_PA2M_CFG_BASE)
#define ADVANCED_TIMER0_TRG_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH1_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER0_TRG                      PA2M_PER_CHANNEL0

#define ADVANCED_TIMER0_UEV_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH2_PA2M_CFG_BASE)
#define ADVANCED_TIMER0_UEV_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH2_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER0_UEV                      PA2M_PER_CHANNEL0

#define ADVANCED_TIMER0_CC1_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH3_PA2M_CFG_BASE)
#define ADVANCED_TIMER0_CC1_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH3_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER0_CC1                      PA2M_PER_CHANNEL0

#define ADVANCED_TIMER0_CC2_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH4_PA2M_CFG_BASE)
#define ADVANCED_TIMER0_CC2_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH4_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER0_CC2                      PA2M_PER_CHANNEL0

#define ADVANCED_TIMER0_CC3_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH5_PA2M_CFG_BASE)
#define ADVANCED_TIMER0_CC3_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH5_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER0_CC3                      PA2M_PER_CHANNEL0

#define ADVANCED_TIMER0_CC4_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH6_PA2M_CFG_BASE)
#define ADVANCED_TIMER0_CC4_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH6_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER0_CC4                      PA2M_PER_CHANNEL0

#define ADVANCED_TIMER1_COM_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH0_PA2M_CFG_BASE)
#define ADVANCED_TIMER1_COM_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH0_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER1_COM                      PA2M_PER_CHANNEL1

#define ADVANCED_TIMER1_TRG_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH1_PA2M_CFG_BASE)
#define ADVANCED_TIMER1_TRG_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH1_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER1_TRG                      PA2M_PER_CHANNEL1

#define ADVANCED_TIMER1_UEV_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH2_PA2M_CFG_BASE)
#define ADVANCED_TIMER1_UEV_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH2_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER1_UEV                      PA2M_PER_CHANNEL1

#define ADVANCED_TIMER1_CC1_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH3_PA2M_CFG_BASE)
#define ADVANCED_TIMER1_CC1_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH3_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER1_CC1                      PA2M_PER_CHANNEL1

#define ADVANCED_TIMER1_CC2_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH4_PA2M_CFG_BASE)
#define ADVANCED_TIMER1_CC2_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH4_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER1_CC2                      PA2M_PER_CHANNEL1

#define ADVANCED_TIMER1_CC3_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH5_PA2M_CFG_BASE)
#define ADVANCED_TIMER1_CC3_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH5_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER1_CC3                      PA2M_PER_CHANNEL1

#define ADVANCED_TIMER1_CC4_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH6_PA2M_CFG_BASE)
#define ADVANCED_TIMER1_CC4_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH6_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER1_CC4                      PA2M_PER_CHANNEL1

#define ADVANCED_TIMER2_COM_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH0_PA2M_CFG_BASE)
#define ADVANCED_TIMER2_COM_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH0_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER2_COM                      PA2M_PER_CHANNEL2

#define ADVANCED_TIMER2_TRG_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH1_PA2M_CFG_BASE)
#define ADVANCED_TIMER2_TRG_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH1_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER2_TRG                      PA2M_PER_CHANNEL2

#define ADVANCED_TIMER2_UEV_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH2_PA2M_CFG_BASE)
#define ADVANCED_TIMER2_UEV_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH2_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER2_UEV                      PA2M_PER_CHANNEL2

#define ADVANCED_TIMER2_CC1_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH3_PA2M_CFG_BASE)
#define ADVANCED_TIMER2_CC1_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH3_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER2_CC1                      PA2M_PER_CHANNEL2

#define ADVANCED_TIMER2_CC2_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH4_PA2M_CFG_BASE)
#define ADVANCED_TIMER2_CC2_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH4_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER2_CC2                      PA2M_PER_CHANNEL2

#define ADVANCED_TIMER2_CC3_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH5_PA2M_CFG_BASE)
#define ADVANCED_TIMER2_CC3_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH5_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER2_CC3                      PA2M_PER_CHANNEL2

#define ADVANCED_TIMER2_CC4_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH6_PA2M_CFG_BASE)
#define ADVANCED_TIMER2_CC4_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH6_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER2_CC4                      PA2M_PER_CHANNEL2

#define ADVANCED_TIMER3_COM_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH0_PA2M_CFG_BASE)
#define ADVANCED_TIMER3_COM_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH0_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER3_COM                      PA2M_PER_CHANNEL3

#define ADVANCED_TIMER3_TRG_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH1_PA2M_CFG_BASE)
#define ADVANCED_TIMER3_TRG_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH1_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER3_TRG                      PA2M_PER_CHANNEL3

#define ADVANCED_TIMER3_UEV_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH2_PA2M_CFG_BASE)
#define ADVANCED_TIMER3_UEV_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH2_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER3_UEV                      PA2M_PER_CHANNEL3

#define ADVANCED_TIMER3_CC1_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH3_PA2M_CFG_BASE)
#define ADVANCED_TIMER3_CC1_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH3_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER3_CC1                      PA2M_PER_CHANNEL3

#define ADVANCED_TIMER3_CC2_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH4_PA2M_CFG_BASE)
#define ADVANCED_TIMER3_CC2_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH4_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER3_CC2                      PA2M_PER_CHANNEL3

#define ADVANCED_TIMER3_CC3_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH5_PA2M_CFG_BASE)
#define ADVANCED_TIMER3_CC3_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH5_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER3_CC3                      PA2M_PER_CHANNEL3

#define ADVANCED_TIMER3_CC4_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH6_PA2M_CFG_BASE)
#define ADVANCED_TIMER3_CC4_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH6_PA2M_IRQ_BASE)
#define UDMA_SEL_ADVANCED_TIMER3_CC4                      PA2M_PER_CHANNEL3

#define BASIC_TIMER0_I_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH7_PA2M_CFG_BASE)
#define BASIC_TIMER0_I_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH7_PA2M_IRQ_BASE)
#define UDMA_SEL_BASIC_TIMER0_I                      PA2M_PER_CHANNEL0

#define BASIC_TIMER1_I_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH7_PA2M_CFG_BASE)
#define BASIC_TIMER1_I_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH7_PA2M_IRQ_BASE)
#define UDMA_SEL_BASIC_TIMER1_I                      PA2M_PER_CHANNEL1

#define BASIC_TIMER2_I_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH7_PA2M_CFG_BASE)
#define BASIC_TIMER2_I_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH7_PA2M_IRQ_BASE)
#define UDMA_SEL_BASIC_TIMER2_I                      PA2M_PER_CHANNEL2

#define BASIC_TIMER3_I_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH7_PA2M_CFG_BASE)
#define BASIC_TIMER3_I_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH7_PA2M_IRQ_BASE)
#define UDMA_SEL_BASIC_TIMER3_I                      PA2M_PER_CHANNEL3

#define BASIC_TIMER4_I_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH7_PA2M_CFG_BASE)
#define BASIC_TIMER4_I_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH7_PA2M_IRQ_BASE)
#define UDMA_SEL_BASIC_TIMER4_I                      PA2M_PER_CHANNEL4

#define BASIC_TIMER5_I_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH7_PA2M_CFG_BASE)
#define BASIC_TIMER5_I_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH7_PA2M_IRQ_BASE)
#define UDMA_SEL_BASIC_TIMER5_I                      PA2M_PER_CHANNEL5

#define BASIC_TIMER6_I_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH7_PA2M_CFG_BASE)
#define BASIC_TIMER6_I_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH7_PA2M_IRQ_BASE)
#define UDMA_SEL_BASIC_TIMER6_I                      PA2M_PER_CHANNEL6

#define BASIC_TIMER7_I_DMA_CH                        ((UDMA_PA_CHxCfg_TypeDef *) UDMA0_CH7_PA2M_CFG_BASE)
#define BASIC_TIMER7_I_DMA_IRQ                       ((UDMA_PA_CHx_IRQ_TypeDef *) UDMA0_CH7_PA2M_IRQ_BASE)
#define UDMA_SEL_BASIC_TIMER7_I                      PA2M_PER_CHANNEL7

/**
  * @}
  */

#define ILM_LOCAL_BASE (0x08000000U)
#define ILM_SIZE_IN_BYTE (0x10000U)
#define DLM_LOCAL_BASE (0x08010000U)
#define DLM_SIZE_IN_BYTE (0x8000U)

#define ILM_SYSTEM_BASE (0x8000000U)
#define DLM_SYSTEM_BASE (0x8000000U+0x10000U)

#define ADDRESS_IN_ILM(address)     ((ILM_LOCAL_BASE) <= (address)) &&     ((ILM_LOCAL_BASE + ILM_SIZE_IN_BYTE) > (address))
#define ADDRESS_IN_DLM(address)     ((DLM_LOCAL_BASE) <= (address)) &&     ((DLM_LOCAL_BASE + DLM_SIZE_IN_BYTE) > (address))
#define ADDRESS_IN_DLM_SYSTEM(address)     ((DLM_SYSTEM_BASE) <= (address)) &&     ((DLM_SYSTEM_BASE + DLM_SIZE_IN_BYTE) > (address))

#define ADDRESS_IN_ILM_SYSTEM(address)     ((ILM_SYSTEM_BASE) <= (address)) &&     ((ILM_SYSTEM_BASE + ILM_SIZE_IN_BYTE) > (address))
#define ADDRESS_IN_DLM_SYSTEM(address)     ((DLM_SYSTEM_BASE) <= (address)) &&     ((DLM_SYSTEM_BASE + DLM_SIZE_IN_BYTE) > (address))

#define SMP_DLM_TO_SYSTEM(core_id,address)     (DLM_SYSTEM_BASE + (address) - (DLM_LOCAL_BASE)+ (core_id*__SMP_DMA_ADDR_OFFSET))
#define SMP_ILM_TO_SYSTEM(core_id,address)     (ILM_SYSTEM_BASE + (address) - (ILM_LOCAL_BASE)+ (core_id*__SMP_DMA_ADDR_OFFSET))

#define DLM_TO_SYSTEM(address)     (DLM_SYSTEM_BASE + (address) - (DLM_LOCAL_BASE))
#define ILM_TO_SYSTEM(address)     (ILM_SYSTEM_BASE + (address) - (ILM_LOCAL_BASE))
#define SYSTEM_TO_DLM(address)     ((address) - DLM_SYSTEM_BASE + (DLM_LOCAL_BASE))
#define SYSTEM_TO_ILM(address)     ((address) - ILM_SYSTEM_BASE + (ILM_LOCAL_BASE))

#define ADDR_CHECK(addr,uplimit,lowlimit)   ((uint32_t)(addr)<=(uint32_t)(uplimit))&&((uint32_t)(addr)>=(uint32_t)(lowlimit))
#define DLM_ADDR_CHECK(addr)                 ADDR_CHECK(addr,ONCHIP_DLM_BASE+ONCHIP_DLM_SIZE,ONCHIP_DLM_BASE)   
#if defined(__SMP_DMA_ADDR_OFFSET) && __SMP_DMA_ADDR_OFFSET>0
#define CAL_ADDR(addr)    ( ADDRESS_IN_ILM(addr) ? SMP_ILM_TO_SYSTEM(hart_id.b.core_id,addr) : ADDRESS_IN_DLM(addr) ? SMP_ILM_TO_SYSTEM(hart_id.b.core_id,addr) : addr )                                                                                   
#define CAL_ADDR_LO(addr)   (ADDR32((CAL_ADDR(ADDR64(addr))&0xFFFFFFFF)))
#define CAL_ADDR_HI(addr)   (ADDR32(((CAL_ADDR(ADDR64(addr)))>>32)&0xFFFFFFFF))
#else  
#define CAL_ADDR(addr)    ( ADDRESS_IN_ILM(addr) ? ILM_TO_SYSTEM(addr) : ADDRESS_IN_DLM(addr) ? DLM_TO_SYSTEM(addr):addr)    
#define CAL_ADDR_LO(addr)   (CAL_ADDR((ADDR32(addr))&0xFFFFFFFF))
#define CAL_ADDR_HI(addr)   (((CAL_ADDR(ADDR32(addr)))>>32)&0xFFFFFFFF)
#endif

#define UNUSED(x)                           ((void)(x))
/* Macros for Bit Operations */
#define _REG8P(p, i)                        ((volatile uint8_t *) ((uintptr_t)((p) + (i))))
#define _REG16P(p, i)                       ((volatile uint16_t *) ((uintptr_t)((p) + (i))))
#define _REG32P(p, i)                       ((volatile uint32_t *) ((uintptr_t)((p) + (i))))
#define _REG64P(p, i)                       ((volatile uint64_t *) ((uintptr_t)((p) + (i))))
#define _REG8(p, i)                         (*(_REG8P(p, i)))
#define _REG16(p, i)                        (*(_REG16P(p, i)))
#define _REG32(p, i)                        (*(_REG32P(p, i)))
#define _REG64(p, i)                        (*(_REG64P(p, i)))
#define REG8(addr)                          _REG8((addr), 0)
#define REG16(addr)                         _REG16((addr), 0)
#define REG32(addr)                         _REG32((addr), 0)
#define REG64(addr)                         _REG64((addr), 0)

/* Macros for Bit Operations */
#if __riscv_xlen == 32
#define BITMASK_MAX                         0xFFFFFFFFUL
#define BITOFS_MAX                          31
#else
#define BITMASK_MAX                         0xFFFFFFFFFFFFFFFFULL
#define BITOFS_MAX                          63
#endif

#define BIT(ofs)                            (0x1UL << (ofs))
#define BITS(start, end)                    ((BITMASK_MAX) << (start) & (BITMASK_MAX) >> (BITOFS_MAX - (end)))

#define SET_VAL(regval, start, end,val)     ((regval) |= (val<<start)&(BITS((start), (end))))
#define GET_BIT(regval, bitofs)             (((regval) >> (bitofs)) & 0x1)
#define SET_BIT(regval, bitofs)             ((regval) |= BIT(bitofs))
#define CLR_BIT(regval, bitofs)             ((regval) &= (~BIT(bitofs)))
#define FLIP_BIT(regval, bitofs)            ((regval) ^= BIT(bitofs))
#define WRITE_BIT(regval, bitofs, val)      CLR_BIT(regval, bitofs); ((regval) |= ((val) << bitofs) & BIT(bitofs))
#define CHECK_BIT(regval, bitofs)           (!!((regval) & (0x1UL<<(bitofs))))
#define GET_BITS(regval, start, end)        (((regval) & BITS((start), (end))) >> (start))
#define SET_BITS(regval, start, end)        ((regval) |= BITS((start), (end)))
#define CLR_BITS(regval, start, end)        ((regval) &= (~BITS((start), (end))))
#define FLIP_BITS(regval, start, end)       ((regval) ^= BITS((start), (end)))
#define CHECK_BITS_ALL(regval, start, end)  (!((~(regval)) & BITS((start), (end))))
#define CHECK_BITS_ANY(regval, start, end)  ((regval) & BITS((start), (end)))
#define WRITE_BITS(regval, start, end, val) CLR_BITS(regval, start, end); ((regval) |= ((val) << start) & BITS((start), (end)))
#define BITMASK_SET(regval, mask)           ((regval) |= (mask))
#define BITMASK_CLR(regval, mask)           ((regval) &= (~(mask)))
#define BITMASK_FLIP(regval, mask)          ((regval) ^= (mask))
#define BITMASK_CHECK_ALL(regval, mask)     (!((~(regval)) & (mask)))
#define BITMASK_CHECK_ANY(regval, mask)     ((regval) & (mask))

#define ADDR8(addr)                         ((uint8_t)(uintptr_t)(addr))
#define ADDR16(addr)                        ((uint16_t)(uintptr_t)(addr))
#define ADDR32(addr)                        ((uint32_t)(uintptr_t)(addr))
#define ADDR64(addr)                        ((uint64_t)(uintptr_t)(addr))
#define ADDR8P(addr)                        ((uint8_t *)(uintptr_t)(addr))
#define ADDR16P(addr)                       ((uint16_t *)(uintptr_t)(addr))
#define ADDR32P(addr)                       ((uint32_t *)(uintptr_t)(addr))
#define ADDR64P(addr)                       ((uint64_t *)(uintptr_t)(addr))

extern uint32_t get_cpu_freq(void);
extern void delay_1ms(uint32_t count);
extern void simulation_pass(void);
extern void simulation_fail(void);
extern void simulation_check(void);
extern void do_check(void);
/** @} */ /* End of group Nuclei */

#ifdef __cplusplus
}
#endif

#endif  /* __NSH__ */

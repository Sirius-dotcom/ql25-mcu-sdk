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
/******************************************************************************
 * \file     startup_demosoc.S
 * \brief    NMSIS Nuclei N/NX Class Core based Core Device Startup File for
 *  ns SoC which using Nuclei RISC-V Core
 * \version  V1.00
 * \date     17. Dec 2019
 *
 ******************************************************************************/

#include "riscv_encoding.h"

/* If BOOT_HARTID is not defined, default value is 0 */
#ifndef BOOT_HARTID
    .equ BOOT_HARTID,    0
#endif

.macro DECLARE_INT_HANDLER  INT_HDL_NAME
#if defined(__riscv_xlen) && (__riscv_xlen == 32)
    .word \INT_HDL_NAME
#else
    .dword \INT_HDL_NAME
#endif
.endm

    .section .text.vtable

    .weak  eclic_msip_handler
    .weak  eclic_mtip_handler
    .weak  eclic_inter_core_int_handler

    .weak  INTER_CORE_IRQHandler
    .weak  UDMA0_IRQHandler
    .weak  USART0_IRQHandler
    .weak  USART1_IRQHandler
    .weak  USART2_IRQHandler
    .weak  USART3_IRQHandler
    .weak  USART4_IRQHandler
    .weak  I2C0_IRQHandler
    .weak  I2C1_IRQHandler
    .weak  QSPI_XIP0_IRQHandler
    .weak  QSPI1_IRQHandler
    .weak  QSPI2_IRQHandler
    .weak  QSPI3_IRQHandler
    .weak  ADVANCED_TIMER0_IRQHandler
    .weak  ADVANCED_TIMER1_IRQHandler
    .weak  ADVANCED_TIMER2_IRQHandler
    .weak  ADVANCED_TIMER3_IRQHandler
    .weak  BASIC_TIMER0_IRQHandler
    .weak  BASIC_TIMER1_IRQHandler
    .weak  BASIC_TIMER2_IRQHandler
    .weak  BASIC_TIMER3_IRQHandler
    .weak  BASIC_TIMER4_IRQHandler
    .weak  BASIC_TIMER5_IRQHandler
    .weak  BASIC_TIMER6_IRQHandler
    .weak  BASIC_TIMER7_IRQHandler
    .weak  LGPIO0_IRQHandler
    .weak  LGPIO1_IRQHandler
    .weak  LGPIO2_IRQHandler
    .weak  LGPIO3_IRQHandler
    .weak  RTC0_IRQHandler
    .weak  RTC1_IRQHandler
    .weak  RTC2_IRQHandler
    .weak  XKAN0_IRQHandler
    .weak  XKAN1_IRQHandler

    .globl vector_base
    .type vector_base, @object
    .option push
    .option norelax
vector_base:
#ifndef VECTOR_TABLE_REMAPPED
    j _start                                                /* 0: Reserved, Jump to _start when reset for vector table not remapped cases.*/
    .align LOG_REGBYTES                                     /*    Need to align 4 byte for RV32, 8 Byte for RV64 */
#else
    DECLARE_INT_HANDLER     default_intexc_handler          /* 0: Reserved, default handler for vector table remapped cases */
#endif
    DECLARE_INT_HANDLER     default_intexc_handler          /* 1: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 2: Reserved */
    DECLARE_INT_HANDLER     eclic_msip_handler              /* 3: Machine software interrupt */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 4: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 5: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 6: Reserved */
    DECLARE_INT_HANDLER     eclic_mtip_handler              /* 7: Machine timer interrupt */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 8: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 9: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 10: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 11: Reserved */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 12: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 13: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 14: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 15: Reserved */

    DECLARE_INT_HANDLER     eclic_inter_core_int_handler    /* 16: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 17: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 18: Reserved */
    
    DECLARE_INT_HANDLER     INTER_CORE_IRQHandler             /* 19: Interrupt 19 */
    DECLARE_INT_HANDLER     UDMA0_IRQHandler             /* 20: Interrupt 20 */
    DECLARE_INT_HANDLER     USART0_IRQHandler             /* 21: Interrupt 21 */
    DECLARE_INT_HANDLER     USART1_IRQHandler             /* 22: Interrupt 22 */
    DECLARE_INT_HANDLER     USART2_IRQHandler             /* 23: Interrupt 23 */
    DECLARE_INT_HANDLER     USART3_IRQHandler             /* 24: Interrupt 24 */
    DECLARE_INT_HANDLER     USART4_IRQHandler             /* 25: Interrupt 25 */
    DECLARE_INT_HANDLER     I2C0_IRQHandler             /* 26: Interrupt 26 */
    DECLARE_INT_HANDLER     I2C1_IRQHandler             /* 27: Interrupt 27 */
    DECLARE_INT_HANDLER     QSPI_XIP0_IRQHandler             /* 28: Interrupt 28 */
    DECLARE_INT_HANDLER     QSPI1_IRQHandler             /* 29: Interrupt 29 */
    DECLARE_INT_HANDLER     QSPI2_IRQHandler             /* 30: Interrupt 30 */
    DECLARE_INT_HANDLER     QSPI3_IRQHandler             /* 31: Interrupt 31 */
    DECLARE_INT_HANDLER     ADVANCED_TIMER0_IRQHandler             /* 32: Interrupt 32 */
    DECLARE_INT_HANDLER     ADVANCED_TIMER1_IRQHandler             /* 33: Interrupt 33 */
    DECLARE_INT_HANDLER     ADVANCED_TIMER2_IRQHandler             /* 34: Interrupt 34 */
    DECLARE_INT_HANDLER     ADVANCED_TIMER3_IRQHandler             /* 35: Interrupt 35 */
    DECLARE_INT_HANDLER     BASIC_TIMER0_IRQHandler             /* 36: Interrupt 36 */
    DECLARE_INT_HANDLER     BASIC_TIMER1_IRQHandler             /* 37: Interrupt 37 */
    DECLARE_INT_HANDLER     BASIC_TIMER2_IRQHandler             /* 38: Interrupt 38 */
    DECLARE_INT_HANDLER     BASIC_TIMER3_IRQHandler             /* 39: Interrupt 39 */
    DECLARE_INT_HANDLER     BASIC_TIMER4_IRQHandler             /* 40: Interrupt 40 */
    DECLARE_INT_HANDLER     BASIC_TIMER5_IRQHandler             /* 41: Interrupt 41 */
    DECLARE_INT_HANDLER     BASIC_TIMER6_IRQHandler             /* 42: Interrupt 42 */
    DECLARE_INT_HANDLER     BASIC_TIMER7_IRQHandler             /* 43: Interrupt 43 */
    DECLARE_INT_HANDLER     LGPIO0_IRQHandler             /* 44: Interrupt 44 */
    DECLARE_INT_HANDLER     LGPIO1_IRQHandler             /* 45: Interrupt 45 */
    DECLARE_INT_HANDLER     LGPIO2_IRQHandler             /* 46: Interrupt 46 */
    DECLARE_INT_HANDLER     LGPIO3_IRQHandler             /* 47: Interrupt 47 */
    DECLARE_INT_HANDLER     RTC0_IRQHandler             /* 48: Interrupt 48 */
    DECLARE_INT_HANDLER     RTC1_IRQHandler             /* 49: Interrupt 49 */
    DECLARE_INT_HANDLER     RTC2_IRQHandler             /* 50: Interrupt 50 */
    DECLARE_INT_HANDLER     XKAN0_IRQHandler             /* 51: Interrupt 51 */
    DECLARE_INT_HANDLER     XKAN1_IRQHandler             /* 52: Interrupt 52 */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 54: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 55: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 56: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 57: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 58: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 59: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 60: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 61: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 62: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 63: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 64: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 65: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 66: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 67: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 68: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 69: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 70: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 71: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 72: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 73: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 74: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 75: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 76: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 77: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 78: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 79: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 80: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 81: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 82: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 83: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 84: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 85: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 86: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 87: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 88: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 89: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 90: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 91: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 92: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 93: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 94: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 95: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 96: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 97: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 98: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 99: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 100: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 101: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 102: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 103: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 104: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 105: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 106: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 107: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 108: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 109: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 110: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 111: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 112: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 113: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 114: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 115: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 116: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 117: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 118: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 119: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 120: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 121: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 122: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 123: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 124: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 125: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 126: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 127: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 128: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 129: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 130: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 131: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 132: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 133: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 134: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 135: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 136: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 137: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 138: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 139: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 140: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 141: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 142: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 143: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 144: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 145: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 146: Reserved */
    .section .init

    .globl _start
    .type _start, @function

/**
 * Reset Handler called on controller reset
 */
_start:
    /* ===== Startup Stage 1 ===== */
    /* Disable Global Interrupt */
    csrc CSR_MSTATUS, MSTATUS_MIE

    /* Initialize GP and TP and jump table base when zcmt enabled */
    .option push
    .option norelax
    la gp, __global_pointer$
    la tp, __tls_base
#if defined(__riscv_zcmt)
    la t0, __jvt_base$
    csrw CSR_JVT, t0
#endif
    .option pop

#if defined(SMP_CPU_CNT) && (SMP_CPU_CNT > 1)
    /* Set correct sp for each cpu
     * each stack size is __STACK_SIZE
     * defined in linker script */
    lui t0, %hi(__STACK_SIZE)
    addi t0, t0, %lo(__STACK_SIZE)
    la sp, _sp
    csrr a0, CSR_MHARTID
    andi a0, a0, 0xFF
    li a1, 0
1:
    beq a0, a1, 2f
    sub sp, sp, t0
    addi a1, a1, 1
    j 1b
2:
#else
    /* Set correct sp for current cpu */
    la sp, _sp
#endif

    /*
     * Set the the NMI base mnvec to share
     * with mtvec by setting CSR_MMISC_CTL
     * bit 9 NMI_CAUSE_FFF to 1
     */
    li t0, MMISC_CTL_NMI_CAUSE_FFF
    csrs CSR_MMISC_CTL, t0

    /*
     * Enable Zc feature when compiled zcmp & zcmt
     */
#if defined(__riscv_zcmp) || defined(__riscv_zcmt)
    li t0, MMISC_CTL_ZC
    csrs CSR_MMISC_CTL, t0
#endif

    /*
     * Intialize ECLIC vector interrupt
     * base address mtvt to vector_base
     */
    la t0, vector_base
    csrw CSR_MTVT, t0

    /*
     * Set ECLIC non-vector entry to be controlled
     * by mtvt2 CSR register.
     * Intialize ECLIC non-vector interrupt
     * base address mtvt2 to irq_entry.
     */
    la t0, irq_entry
    csrw CSR_MTVT2, t0
    csrs CSR_MTVT2, 0x1

    /*
     * Set Exception Entry MTVEC to early_exc_entry
     * Due to settings above, Exception and NMI
     * will share common entry.
     * This early_exc_entry is only used during early
     * boot stage before main
     */
    la t0, early_exc_entry
    csrw CSR_MTVEC, t0

    /* Set the interrupt processing mode to ECLIC mode */
    li t0, 0x3f
    csrc CSR_MTVEC, t0
    csrs CSR_MTVEC, 0x3
    /* ===== Startup Stage 2 ===== */

    /* Enable FPU and Vector Unit if f/d/v exist in march */
#if defined(__riscv_flen) && __riscv_flen > 0
    /* Enable FPU, and set state to initial */
    li t0, MSTATUS_FS
    csrc mstatus, t0
    li t0, MSTATUS_FS_INITIAL
    csrs mstatus, t0
#endif

#if defined(__riscv_vector)
    /* Enable Vector, and set state to initial */
    li t0, MSTATUS_VS
    csrc mstatus, t0
    li t0, MSTATUS_VS_INITIAL
    csrs mstatus, t0
#endif

    /* Enable mcycle and minstret counter */
    csrci CSR_MCOUNTINHIBIT, 0x5

#if defined(SMP_CPU_CNT) && (SMP_CPU_CNT > 1)
    csrr a0, CSR_MHARTID
    li a1, BOOT_HARTID
    bne a0, a1, __skip_init
#endif

__init_common:
    /* ===== Startup Stage 3 ===== */
    /*
     * Load text section from CODE ROM to CODE RAM
     * when text LMA is different with VMA
     */
    la a0, _text_lma
    la a1, _text
    /* If text LMA and VMA are equal
     * then no need to copy text section */
    beq a0, a1, 2f
    la a2, _etext
    bgeu a1, a2, 2f

1:
    /* Load code section if necessary */
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:
    /* Load data section */
    la a0, _data_lma
    la a1, _data
    /* If data vma=lma, no need to copy */
    beq a0, a1, 2f
    la a2, _edata
    bgeu a1, a2, 2f
1:
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:
    /* Clear bss section */
    la a0, __bss_start
    la a1, _end
    bgeu a0, a1, 2f
1:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b
2:
#ifndef CFG_SIMULATION
    la a0, __heap_start
    la a1, __heap_limit
    /* Clear heap section */
1:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b
2:
    /* Clear stack section */
    la a0, __StackLimit
    la a1, __StackTop
1:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b
#endif

.globl _start_premain
.type _start_premain, @function
_start_premain:
    /*
     * Call vendor defined SystemInit to
     * initialize the micro-controller system
     * SystemInit will just be called by boot cpu
     */
    call SystemInit

    /*
     * Call C/C++ constructor start up code,
     * __libc_fini is defined in linker script,
     * so register_fini function will be called
     * and will run atexit (__libc_fini_array)
     * to do previous call atexit function
     */
    call __libc_init_array

__skip_init:
    /* Sync all harts at this function */
    call __sync_harts

    /* do pre-init steps before main */
    /* _premain_init will be called by each cpu
     * please make sure the implementation of __premain_int
     * considered this
     */
    call _premain_init

    /*
     * When all initialization steps done
     * set exception entry to correct exception
     * entry and jump to main.
     * And set the interrupt processing mode to
     * ECLIC mode
     */
    la t0, exc_entry
    csrw CSR_MTVEC, t0
    li t0, 0x3f
    csrc CSR_MTVEC, t0
    csrs CSR_MTVEC, 0x3

    /* BPU cold bringup need time, so enable BPU before enter to main */
    li t0, MMISC_CTL_BPU
    csrs CSR_MMISC_CTL, t0

    /* ===== Call SMP Main Function  ===== */
    /* argc = argv = 0 */
    li a0, 0
    li a1, 0
#if defined(SMP_CPU_CNT) && (SMP_CPU_CNT > 1)
/* The weak implementation of smp_main is in this file */
    call smp_main
#else
#ifdef RTOS_RTTHREAD
    
    call entry
#else
    call main
#endif
#endif
    /* do post-main steps after main
     * this function will be called by each cpu */
    call _postmain_fini

__amp_wait:
1:
    wfi
    j 1b

#if defined(SMP_CPU_CNT) && (SMP_CPU_CNT > 1)
/*
 * You can re-implement smp_main function in your code
 * to do smp boot process and handle multi harts
 */
.weak smp_main
.type smp_main, @function
smp_main:
    addi sp, sp, -2*REGBYTES
    STORE ra, 0*REGBYTES(sp)
    /* only boot hart goto main, other harts do wfi */
    csrr t0, CSR_MHARTID
    li t1, BOOT_HARTID
    beq t0, t1, 2f
1:
    wfi
    j 1b
2:
#ifdef RTOS_RTTHREAD
    
    call entry
#else
    call main
#endif
    LOAD ra, 0*REGBYTES(sp)
    addi sp, sp, 2*REGBYTES
    ret
#endif

/* Early boot exception entry before main */
.align 6
.global early_exc_entry
.type early_exc_entry, @function
early_exc_entry:
    wfi
    j early_exc_entry

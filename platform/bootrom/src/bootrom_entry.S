/*
 * @file bootrom_entry.S
 * @brief QL25 BootROM 汇编入口
 *
 * 固化在主核（Core 1）16KB ROM 中。
 * 上电后 CPU 从 _bootrom_start 开始执行。
 *
 * 参考：SoC/ns_core1/Common/Source/GCC/startup_ns_core1.S
 *
 * 与 startup.S 的区别：
 *   - 不做 .bss/.data 搬运（那是应用 startup 的事）
 *   - 栈放片上 SRAM（Core 1 无 DLM）
 *   - 跳转到 bootrom_main() 而非 main()
 *   - 向量表极简（BootROM 阶段不开中断）
 */

#include "riscv_encoding.h"

/* BootROM 临时栈：SRAM3 顶部 */
#define BOOTROM_SP      0x30070000

/* ========================================================================
 * 异常向量表（BootROM 阶段极简，全部指向死循环）
 * ======================================================================== */

    .section .text.bootrom_vtable, "ax"
    .globl _bootrom_vector_base
    .type  _bootrom_vector_base, @object
    .option push
    .option norelax

_bootrom_vector_base:
    j _bootrom_start                /* 0: Reset → 入口 */
    .align 2
    .word _bootrom_exception        /* 1: Reserved */
    .word _bootrom_exception        /* 2: Reserved */
    .word _bootrom_exception        /* 3: Machine software interrupt */
    .word _bootrom_exception        /* 4: Reserved */
    .word _bootrom_exception        /* 5: Reserved */
    .word _bootrom_exception        /* 6: Reserved */
    .word _bootrom_exception        /* 7: Machine timer interrupt */
    /* 8~18: Reserved */
    .word _bootrom_exception
    .word _bootrom_exception
    .word _bootrom_exception
    .word _bootrom_exception
    .word _bootrom_exception
    .word _bootrom_exception
    .word _bootrom_exception
    .word _bootrom_exception
    .word _bootrom_exception
    .word _bootrom_exception
    .word _bootrom_exception

    .option pop

/* ========================================================================
 * BootROM 入口：最小 CPU 初始化
 * 参考 startup_ns_core1.S Stage 1 + Stage 2
 * ======================================================================== */

    .section .text.bootrom_entry, "ax"
    .globl _bootrom_start
    .type  _bootrom_start, @function

_bootrom_start:
    /* ===== Stage 1: 最小 CPU 初始化 ===== */

    /* 关闭全局中断 */
    csrc CSR_MSTATUS, MSTATUS_MIE

    /* 设置栈指针：SRAM3 顶部（片上 SRAM，上电可用） */
    li   sp, BOOTROM_SP

    /*
     * NMI 基址与 mtvec 共用
     * （参考 startup.S: MMISC_CTL_NMI_CAUSE_FFF）
     */
    li   t0, MMISC_CTL_NMI_CAUSE_FFF
    csrs CSR_MMISC_CTL, t0

    /*
     * 配置 ECLIC 向量中断基址
     * BootROM 阶段不真正使用中断，但需要设置以防异常
     */
    la   t0, _bootrom_vector_base
    csrw CSR_MTVT, t0

    /*
     * 异常入口指向 BootROM 异常处理（死循环）
     * 设置中断模式为 ECLIC 模式
     */
    la   t0, _bootrom_exception
    csrw CSR_MTVEC, t0
    li   t0, 0x3f
    csrc CSR_MTVEC, t0
    li   t0, 0x3
    csrs CSR_MTVEC, t0

    /* ===== Stage 2: CPU 特性使能 ===== */

    /* 使能 FPU（rv32imafc 有单精度 FPU） */
#if defined(__riscv_flen) && __riscv_flen > 0
    li   t0, MSTATUS_FS
    csrc mstatus, t0
    li   t0, MSTATUS_FS_INITIAL
    csrs mstatus, t0
#endif

    /* 使能性能计数器（mcycle, minstret） */
    csrci CSR_MCOUNTINHIBIT, 0x5

    /* 使能分支预测器 */
    li   t0, MMISC_CTL_BPU
    csrs CSR_MMISC_CTL, t0

    /* ===== Stage 3: 跳转到 C 代码 ===== */

    /*
     * 不需要搬运 .data 或清 .bss —— BootROM 在 ROM 中，
     * 全局变量要么是 const（在 ROM），要么放在栈上。
     * BootROM C 代码不应使用可写全局变量。
     */
    call bootrom_main

    /* bootrom_main 不应该返回，如果返回则进入死循环 */
    j    _bootrom_exception

/* ========================================================================
 * BootROM 异常处理：死循环（BootROM 阶段不恢复异常）
 * ======================================================================== */

    .align 6
    .globl _bootrom_exception
    .type  _bootrom_exception, @function
_bootrom_exception:
    /* 可以在此处通过 USART0 输出错误信息用于调试 */
    wfi
    j _bootrom_exception

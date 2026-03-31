#include "nuclei_sdk_soc.h"

__STATIC_FORCEINLINE uint64_t get_timer_freq(void)
{
    return (uint64_t)SOC_TIMER_FREQ;
}

uint32_t measure_cpu_freq(uint32_t n)
{
    uint32_t start_mcycle, delta_mcycle;
    uint32_t start_mtime, delta_mtime;
    uint64_t mtime_freq = get_timer_freq();

    uint32_t tmp = (uint32_t)SysTimer_GetLoadValue();
    do {
        start_mtime = (uint32_t)SysTimer_GetLoadValue();
        start_mcycle = __RV_CSR_READ(CSR_MCYCLE);
    } while (start_mtime == tmp);

    do {
        delta_mtime = (uint32_t)SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < n);

    delta_mcycle = __RV_CSR_READ(CSR_MCYCLE) - start_mcycle;

    return (delta_mcycle / delta_mtime) * mtime_freq
           + ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
}

uint32_t get_cpu_freq(void)
{
    uint32_t cpu_freq;

    measure_cpu_freq(1);
    
    cpu_freq = measure_cpu_freq(10000);

    return cpu_freq;
}

/**
 * \brief      delay a time in milliseconds
 * \details
 *             provide API for delay
 * \param[in]  count: count in milliseconds
 * \remarks
 */
void delay_1ms(uint32_t count)
{
    uint64_t start_mtime, delta_mtime;
    uint64_t delay_ticks = (SOC_TIMER_FREQ * (uint64_t)count) / 1000;

    start_mtime = SysTimer_GetLoadValue();

    do {
        delta_mtime = SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < delay_ticks);
}

void simulation_pass(void)
{

#ifdef SOC_MISC_CORE1_RST_N
        soc_misc_core1_rst_n(DISABLE);
        soc_misc_core1_rst_n(ENABLE);
#endif
#ifdef SOC_RCC_CPU1_RST_N
        soc_rcc_cpu1_rst_n(DISABLE);
        soc_rcc_cpu1_rst_n(ENABLE);
#endif
#ifdef SOC_RCC_CORE1_RST_N
        soc_rcc_core1_rst_n(DISABLE);
        soc_rcc_core1_rst_n(ENABLE);
#endif
#ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
        soc_clk_cluster1_core0_stop_on_reset(DISABLE);
#endif
#ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
        soc_clk_cluster1_core0_stop_on_reset(DISABLE);
#endif
#ifdef CLUSTER1_CORE0_STOP_ON_RESET
        cluster1_core0_stop_on_reset(DISABLE);
#endif
#ifdef CORE1_STOP_ON_RESET
        core1_stop_on_reset(DISABLE);
#endif
#ifdef SOC_CLK_CORE1_STOP_ON_RESET
        soc_clk_core1_stop_on_reset(DISABLE);
#endif
    __disable_irq();
#if CFG_SIMULATION
    __asm__ __volatile__(
    "csrrwi gp,mscratch,15"
    :
    :
    :"gp"
    );
#else 
    printf("test pass!\r\n");
    printf("dump test!\r\n");
#endif
}

void simulation_fail(void)
{
#ifdef SOC_MISC_CORE1_RST_N
        soc_misc_core1_rst_n(DISABLE);
        soc_misc_core1_rst_n(ENABLE);
#endif
#ifdef SOC_RCC_CPU1_RST_N
        soc_rcc_cpu1_rst_n(DISABLE);
        soc_rcc_cpu1_rst_n(ENABLE);
#endif
#ifdef SOC_RCC_CORE1_RST_N
        soc_rcc_core1_rst_n(DISABLE);
        soc_rcc_core1_rst_n(ENABLE);
#endif
#ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
        soc_clk_cluster1_core0_stop_on_reset(DISABLE);
#endif
#ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
        soc_clk_cluster1_core0_stop_on_reset(DISABLE);
#endif
#ifdef CLUSTER1_CORE0_STOP_ON_RESET
        cluster1_core0_stop_on_reset(DISABLE);
#endif
#ifdef CORE1_STOP_ON_RESET
        core1_stop_on_reset(DISABLE);
#endif
#ifdef SOC_CLK_CORE1_STOP_ON_RESET
        soc_clk_core1_stop_on_reset(DISABLE);
#endif
    __disable_irq();
#if CFG_SIMULATION
    __asm__ __volatile__(
    "csrrwi gp,mscratch,14"
    :
    :
    :"gp"
    );
#else
    printf("test fail!\r\n");
    printf("dump test!\r\n");
#endif
}

void simulation_check(void)
{
#ifdef SOC_MISC_CORE1_RST_N
        soc_misc_core1_rst_n(DISABLE);
        soc_misc_core1_rst_n(ENABLE);
#endif
#ifdef SOC_RCC_CPU1_RST_N
        soc_rcc_cpu1_rst_n(DISABLE);
        soc_rcc_cpu1_rst_n(ENABLE);
#endif
#ifdef SOC_RCC_CORE1_RST_N
        soc_rcc_core1_rst_n(DISABLE);
        soc_rcc_core1_rst_n(ENABLE);
#endif
#ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
        soc_clk_cluster1_core0_stop_on_reset(DISABLE);
#endif
#ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
        soc_clk_cluster1_core0_stop_on_reset(DISABLE);
#endif
#ifdef CLUSTER1_CORE0_STOP_ON_RESET
        cluster1_core0_stop_on_reset(DISABLE);
#endif
#ifdef CORE1_STOP_ON_RESET
        core1_stop_on_reset(DISABLE);
#endif
#ifdef SOC_CLK_CORE1_STOP_ON_RESET
        soc_clk_core1_stop_on_reset(DISABLE);
#endif
    __disable_irq();
#if CFG_SIMULATION
    __asm__ __volatile__(
    "csrrwi gp,mscratch,13"
    :
    :
    :"gp"
    );
#else
    printf("todo check!\r\n");
    printf("dump test!\r\n");
#endif
}

void do_check(void)
{ 
#ifdef SOC_MISC_CORE1_RST_N
        soc_misc_core1_rst_n(DISABLE);
        soc_misc_core1_rst_n(ENABLE);
#endif
#ifdef SOC_RCC_CPU1_RST_N
        soc_rcc_cpu1_rst_n(DISABLE);
        soc_rcc_cpu1_rst_n(ENABLE);
#endif
#ifdef SOC_RCC_CORE1_RST_N
        soc_rcc_core1_rst_n(DISABLE);
        soc_rcc_core1_rst_n(ENABLE);
#endif
#ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
        soc_clk_cluster1_core0_stop_on_reset(DISABLE);
#endif
#ifdef SOC_CLK_CLUSTER1_CORE0_STOP_ON_RESET
        soc_clk_cluster1_core0_stop_on_reset(DISABLE);
#endif
#ifdef CLUSTER1_CORE0_STOP_ON_RESET
        cluster1_core0_stop_on_reset(DISABLE);
#endif
#ifdef CORE1_STOP_ON_RESET
        core1_stop_on_reset(DISABLE);
#endif
#ifdef SOC_CLK_CORE1_STOP_ON_RESET
        soc_clk_core1_stop_on_reset(DISABLE);
#endif
#if CFG_SIMULATION
    __asm__ __volatile__(
    "csrrwi gp,mscratch,8"
    :
    :
    :"gp"
    );
#else
    printf("do_check!\r\n");
    printf("dump test!\r\n");
#endif
}

/* map core local memory(DLM/ILM) to system address */

#ifndef CFG_SIMULATION
#define dbg_printf(argc,argv...) ({printf(argc,##argv);} )
#else
#define dbg_printf(argc,argv...)
#endif 

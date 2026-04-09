/**
 * @file FreeRTOSConfig.h
 * @brief 从核（Core 0 数据面）FreeRTOS 配置
 *
 * 针对 48KB ILM 硬约束做激进裁剪：
 *   - 仅静态分配，禁用堆
 *   - 禁用软件定时器（省一个 TCB + 栈）
 *   - 最小化 TCB 附加字段
 *   - 精简 INCLUDE 可选函数
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "nuclei_sdk_soc.h"

/* ======================================================================
 * 基础调度
 * ====================================================================== */

#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE                 0
#define configCPU_CLOCK_HZ                      SystemCoreClock
#define configRTC_CLOCK_HZ                      32768
#define configTICK_RATE_HZ                      1000
#define configMAX_PRIORITIES                    5
#define configMINIMAL_STACK_SIZE                128     /* words，idle 栈 = 512B */
#define configMAX_TASK_NAME_LEN                 12
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_TIME_SLICING                  0       /* 同优先级不时间片切换，省 tick 开销 */
#define configUSE_NEWLIB_REENTRANT              0
#define configENABLE_BACKWARD_COMPATIBILITY     0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 0

/* ======================================================================
 * 内存分配 — 仅静态，零堆
 * ====================================================================== */

#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        0
#define configTOTAL_HEAP_SIZE                   0
#define configAPPLICATION_ALLOCATED_HEAP        0

/* ======================================================================
 * 同步原语 — 最小化
 * ====================================================================== */

#define configUSE_TASK_NOTIFICATIONS            1
#define configUSE_MUTEXES                       0
#define configUSE_RECURSIVE_MUTEXES             0
#define configUSE_COUNTING_SEMAPHORES           0
#define configQUEUE_REGISTRY_SIZE               0
#define configUSE_QUEUE_SETS                    0

/* ======================================================================
 * Hook 函数
 * ====================================================================== */

#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          2       /* bringup 阶段保留栈检测 */
#define configUSE_MALLOC_FAILED_HOOK            0       /* 无堆，不需要 */
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* ======================================================================
 * 运行时统计 — 关闭
 * ====================================================================== */

#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                0
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* ======================================================================
 * 协程 — 不使用
 * ====================================================================== */

#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* ======================================================================
 * 软件定时器 — 关闭（省一个 timer task TCB + 栈）
 * ====================================================================== */

#define configUSE_TIMERS                        0

/* ======================================================================
 * 中断优先级
 * ====================================================================== */

#define configKERNEL_INTERRUPT_PRIORITY          0
#define configMAX_SYSCALL_INTERRUPT_PRIORITY     7

/* ======================================================================
 * 断言
 * ====================================================================== */

#define configASSERT(x) \
    if ((x) == 0) { taskDISABLE_INTERRUPTS(); for (;;); }

/* ======================================================================
 * 可选函数 — 仅保留必需
 * ====================================================================== */

#define INCLUDE_vTaskPrioritySet                0
#define INCLUDE_uxTaskPriorityGet               0
#define INCLUDE_vTaskDelete                     0
#define INCLUDE_vTaskSuspend                    0
#define INCLUDE_xResumeFromISR                  0
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     1       /* bringup 栈审计 */
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        0
#define INCLUDE_xTimerPendFunctionCall          0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              0

#endif /* FREERTOS_CONFIG_H */

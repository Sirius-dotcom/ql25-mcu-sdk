/*
 * FreeRTOSConfig.h -- QL25 AMP Slave (Core0)
 *
 * 从核（数据面）：
 *   - 整个 image 由主核搬运到从核 ILM 执行
 *   - bin 必须 <= 48KB
 *   - 优先使用静态分配，避免 heap 管理代码占空间
 *   - 不启用 software timer / printf / 多余 API
 *
 * 说明：
 *   1. configMAX_SYSCALL_INTERRUPT_PRIORITY 当前按 7 保守配置，
 *      后续需结合 ECLIC priority bit width 再确认。
 *   2. configUSE_NEWLIB_REENTRANT = 0，避免引入额外重入开销。
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "nuclei_sdk_soc.h"

/* -----------------------------------------------------------
 * 基本调度配置
 * ----------------------------------------------------------- */
#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE                 0
#define configCPU_CLOCK_HZ                      ( SystemCoreClock )
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES                    6
#define configMINIMAL_STACK_SIZE                128
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) 0 )
#define configMAX_TASK_NAME_LEN                 8
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1

/* -----------------------------------------------------------
 * 同步/功能开关
 * ----------------------------------------------------------- */
#define configUSE_MUTEXES                       0
#define configUSE_RECURSIVE_MUTEXES             0
#define configUSE_COUNTING_SEMAPHORES           0
#define configUSE_QUEUE_SETS                    0
#define configUSE_TASK_NOTIFICATIONS            1
#define configUSE_TIME_SLICING                  1

/* 从核：全静态分配，禁用动态分配 */
#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        0

/* Hook */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configUSE_MALLOC_FAILED_HOOK            0

/* 关闭 software timer，节省代码 */
#define configUSE_TIMERS                        0

/* -----------------------------------------------------------
 * 中断优先级（需后续按 ECLIC 最终确认）
 * ----------------------------------------------------------- */
#define configKERNEL_INTERRUPT_PRIORITY         0
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    7

/* -----------------------------------------------------------
 * API inclusion：只保留必要接口
 * ----------------------------------------------------------- */
#define INCLUDE_vTaskPrioritySet                0
#define INCLUDE_uxTaskPriorityGet               0
#define INCLUDE_vTaskDelete                     0
#define INCLUDE_vTaskSuspend                    0
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          0
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     0

/* Assert */
#define configASSERT(x)                         if ((x) == 0) { taskDISABLE_INTERRUPTS(); for (;;) {} }

#endif /* FREERTOS_CONFIG_H */

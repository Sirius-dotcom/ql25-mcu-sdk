/*
 * FreeRTOSConfig.h -- QL25 AMP Master (Core1)
 *
 * 主核（管理面）：
 *   - 运行于主核本地 SRAM / XIP（最终模式）
 *   - 允许动态分配（创建少量任务）
 *   - 任务数少，侧重控制/管理/心跳/IPC 服务
 *
 * 说明：
 *   1. configMAX_SYSCALL_INTERRUPT_PRIORITY 当前按 7 保守配置，
 *      后续需结合 ECLIC priority bit width 再确认。
 *   2. 堆大小按 bring-up 先给 8KB，可根据 map/高水位再收敛。
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
#define configMAX_PRIORITIES                    8
#define configMINIMAL_STACK_SIZE                256
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 8 * 1024 ) )
#define configMAX_TASK_NAME_LEN                 16
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1

/* -----------------------------------------------------------
 * 同步/功能开关
 * ----------------------------------------------------------- */
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             0
#define configUSE_COUNTING_SEMAPHORES           1
#define configUSE_QUEUE_SETS                    0
#define configUSE_TASK_NOTIFICATIONS            1
#define configUSE_TIME_SLICING                  1

/* 主核：允许动态分配 */
#define configSUPPORT_STATIC_ALLOCATION         0
#define configSUPPORT_DYNAMIC_ALLOCATION        1

/* Hook */
#define configUSE_IDLE_HOOK                     1
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configUSE_MALLOC_FAILED_HOOK            1

/* Software timer */
#define configUSE_TIMERS                        0
#define configTIMER_TASK_PRIORITY               2
#define configTIMER_QUEUE_LENGTH                4
#define configTIMER_TASK_STACK_DEPTH            256

/* -----------------------------------------------------------
 * 中断优先级（需后续按 ECLIC 最终确认）
 * ----------------------------------------------------------- */
#define configKERNEL_INTERRUPT_PRIORITY         0
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    7

/* -----------------------------------------------------------
 * API inclusion
 * ----------------------------------------------------------- */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     1

/* Assert */
#define configASSERT(x)                         if ((x) == 0) { taskDISABLE_INTERRUPTS(); for (;;) {} }

#endif /* FREERTOS_CONFIG_H */

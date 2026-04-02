/**
 * @file osal.h
 * @brief OS Abstraction Layer — 统一 RTOS 接口
 *
 * 业务层和平台层只调用本文件定义的接口，不直接调用 FreeRTOS API。
 * ISR 上下文中不使用 OSAL，直接调用 FreeRTOS FromISR 系列 API。
 *
 * 当前实现：FreeRTOS V10.3.1 + Nuclei RISC-V 移植层
 */

#ifndef OSAL_H
#define OSAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================
 * Return codes
 * ======================================================================== */

#define OSAL_OK             0
#define OSAL_ERR            (-1)
#define OSAL_ERR_TIMEOUT    (-2)
#define OSAL_ERR_NOMEM      (-3)
#define OSAL_ERR_PARAM      (-4)
#define OSAL_ERR_ISR        (-5)    /* 在 ISR 中调用了非 ISR-safe 接口 */

/* 无限等待 */
#define OSAL_WAIT_FOREVER   UINT32_MAX

/* ========================================================================
 * Opaque handles — 不暴露 FreeRTOS 类型到业务层
 * ======================================================================== */

typedef void *osal_task_t;
typedef void *osal_queue_t;
typedef void *osal_mutex_t;
typedef void *osal_sem_t;
typedef void *osal_timer_t;

/* 任务入口函数原型 */
typedef void (*osal_task_fn_t)(void *arg);

/* 定时器回调函数原型 */
typedef void (*osal_timer_fn_t)(osal_timer_t timer);

/* ========================================================================
 * Task
 * ======================================================================== */

/**
 * @brief 创建任务
 *
 * @param name          任务名称（调试用，最长 configMAX_TASK_NAME_LEN）
 * @param fn            任务入口函数
 * @param arg           传给入口函数的参数
 * @param stack_words   栈大小，单位 word（非 byte）
 * @param priority      任务优先级（0 = 最低，值越大优先级越高）
 * @return              任务句柄，失败返回 NULL
 */
osal_task_t osal_task_create(const char *name,
                             osal_task_fn_t fn,
                             void *arg,
                             uint32_t stack_words,
                             uint32_t priority);

/**
 * @brief 删除任务
 * @param task  任务句柄，NULL 表示删除自身
 */
void osal_task_delete(osal_task_t task);

/**
 * @brief 延时（毫秒）
 * @param ms  延时时间，0 表示主动让出 CPU
 */
void osal_delay_ms(uint32_t ms);

/**
 * @brief 获取当前任务栈高水位（剩余最小 word 数）
 * @param task  任务句柄，NULL 表示当前任务
 * @return      栈剩余最小 word 数
 */
uint32_t osal_task_stack_watermark(osal_task_t task);

/* ========================================================================
 * Queue
 * ======================================================================== */

/**
 * @brief 创建消息队列
 *
 * @param depth      队列深度（能容纳的消息条数）
 * @param item_size  每条消息的字节大小
 * @return           队列句柄，失败返回 NULL
 */
osal_queue_t osal_queue_create(uint32_t depth, uint32_t item_size);

/**
 * @brief 删除消息队列
 */
void osal_queue_delete(osal_queue_t queue);

/**
 * @brief 发送消息到队列尾部
 *
 * @param queue       队列句柄
 * @param item        指向待发送数据的指针
 * @param timeout_ms  超时（ms），OSAL_WAIT_FOREVER 表示无限等待，0 表示不等待
 * @return            OSAL_OK / OSAL_ERR_TIMEOUT / OSAL_ERR_PARAM
 */
int osal_queue_send(osal_queue_t queue, const void *item, uint32_t timeout_ms);

/**
 * @brief 从队列头部接收消息
 *
 * @param queue       队列句柄
 * @param item        接收缓冲区指针
 * @param timeout_ms  超时（ms），OSAL_WAIT_FOREVER 表示无限等待，0 表示不等待
 * @return            OSAL_OK / OSAL_ERR_TIMEOUT / OSAL_ERR_PARAM
 */
int osal_queue_recv(osal_queue_t queue, void *item, uint32_t timeout_ms);

/**
 * @brief 查询队列中当前消息数
 */
uint32_t osal_queue_waiting(osal_queue_t queue);

/* ========================================================================
 * Mutex（互斥锁）
 * ======================================================================== */

/**
 * @brief 创建互斥锁
 * @return  互斥锁句柄，失败返回 NULL
 */
osal_mutex_t osal_mutex_create(void);

/**
 * @brief 删除互斥锁
 */
void osal_mutex_delete(osal_mutex_t mutex);

/**
 * @brief 获取互斥锁
 *
 * @param mutex       互斥锁句柄
 * @param timeout_ms  超时（ms），OSAL_WAIT_FOREVER 表示无限等待
 * @return            OSAL_OK / OSAL_ERR_TIMEOUT / OSAL_ERR_PARAM
 */
int osal_mutex_lock(osal_mutex_t mutex, uint32_t timeout_ms);

/**
 * @brief 释放互斥锁
 */
int osal_mutex_unlock(osal_mutex_t mutex);

/* ========================================================================
 * Semaphore（计数信号量）
 * ======================================================================== */

/**
 * @brief 创建计数信号量
 *
 * @param max_count    最大计数值
 * @param init_count   初始计数值
 * @return             信号量句柄，失败返回 NULL
 */
osal_sem_t osal_sem_create(uint32_t max_count, uint32_t init_count);

/**
 * @brief 删除信号量
 */
void osal_sem_delete(osal_sem_t sem);

/**
 * @brief 等待（获取）信号量
 *
 * @param sem         信号量句柄
 * @param timeout_ms  超时（ms），OSAL_WAIT_FOREVER 表示无限等待
 * @return            OSAL_OK / OSAL_ERR_TIMEOUT / OSAL_ERR_PARAM
 */
int osal_sem_wait(osal_sem_t sem, uint32_t timeout_ms);

/**
 * @brief 释放（发送）信号量
 * @return  OSAL_OK / OSAL_ERR
 */
int osal_sem_post(osal_sem_t sem);

/* ========================================================================
 * Software Timer
 * ======================================================================== */

/**
 * @brief 创建软件定时器
 *
 * @param name        定时器名称（调试用）
 * @param period_ms   定时周期（ms）
 * @param auto_reload true = 周期重复，false = 单次触发
 * @param callback    回调函数
 * @return            定时器句柄，失败返回 NULL
 */
osal_timer_t osal_timer_create(const char *name,
                               uint32_t period_ms,
                               bool auto_reload,
                               osal_timer_fn_t callback);

/**
 * @brief 删除定时器
 */
void osal_timer_delete(osal_timer_t timer);

/**
 * @brief 启动定时器
 * @return  OSAL_OK / OSAL_ERR
 */
int osal_timer_start(osal_timer_t timer);

/**
 * @brief 停止定时器
 * @return  OSAL_OK / OSAL_ERR
 */
int osal_timer_stop(osal_timer_t timer);

/**
 * @brief 重置定时器（重新开始计时）
 * @return  OSAL_OK / OSAL_ERR
 */
int osal_timer_reset(osal_timer_t timer);

/* ========================================================================
 * Time
 * ======================================================================== */

/**
 * @brief 获取系统运行时间（毫秒）
 *
 * 基于 FreeRTOS tick 计数，精度取决于 configTICK_RATE_HZ。
 * 溢出周期极长（TickType_t 为 64 位），实际不会溢出。
 */
uint32_t osal_time_ms(void);

/**
 * @brief 获取系统 tick 计数（原始精度）
 */
uint64_t osal_tick_get(void);

/* ========================================================================
 * Critical Section（临界区，仅用于极短代码段）
 * ======================================================================== */

/**
 * @brief 进入临界区（屏蔽可管理中断）
 *
 * 不可嵌套调用 osal_delay_ms、osal_queue_recv 等阻塞接口。
 * 临界区内应尽可能短，避免影响中断延迟。
 */
void osal_critical_enter(void);

/**
 * @brief 退出临界区
 */
void osal_critical_exit(void);

/* ========================================================================
 * Scheduler
 * ======================================================================== */

/**
 * @brief 启动调度器（不会返回）
 */
void osal_scheduler_start(void);

/**
 * @brief 查询调度器是否已启动
 * @return  true = 已启动
 */
bool osal_scheduler_is_running(void);

#ifdef __cplusplus
}
#endif

#endif /* OSAL_H */

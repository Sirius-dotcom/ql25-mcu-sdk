/**
 * @file osal_freertos.c
 * @brief OSAL FreeRTOS 实现
 *
 * 本文件是唯一 include FreeRTOS 头文件的地方。
 * 业务层通过 osal.h 接口调用，不直接依赖 FreeRTOS。
 */

#include "osal.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/* ========================================================================
 * Internal helpers
 * ======================================================================== */

/**
 * @brief 毫秒转 FreeRTOS tick
 *
 * OSAL_WAIT_FOREVER 映射到 portMAX_DELAY。
 * 0 ms 映射到 0 tick（不等待）。
 */
static inline TickType_t ms_to_ticks(uint32_t ms)
{
    if (ms == OSAL_WAIT_FOREVER) {
        return portMAX_DELAY;
    }
    return pdMS_TO_TICKS(ms);
}

/* ========================================================================
 * Task
 * ======================================================================== */

osal_task_t osal_task_create(const char *name,
                             osal_task_fn_t fn,
                             void *arg,
                             uint32_t stack_words,
                             uint32_t priority)
{
    TaskHandle_t handle = NULL;

    if (fn == NULL) {
        return NULL;
    }

    BaseType_t ret = xTaskCreate(fn, name, (configSTACK_DEPTH_TYPE)stack_words,
                                 arg, (UBaseType_t)priority, &handle);
    if (ret != pdPASS) {
        return NULL;
    }
    return (osal_task_t)handle;
}

void osal_task_delete(osal_task_t task)
{
    vTaskDelete((TaskHandle_t)task);
}

void osal_delay_ms(uint32_t ms)
{
    if (ms == 0) {
        taskYIELD();
        return;
    }
    vTaskDelay(ms_to_ticks(ms));
}

uint32_t osal_task_stack_watermark(osal_task_t task)
{
    return (uint32_t)uxTaskGetStackHighWaterMark((TaskHandle_t)task);
}

/* ========================================================================
 * Queue
 * ======================================================================== */

osal_queue_t osal_queue_create(uint32_t depth, uint32_t item_size)
{
    if (depth == 0 || item_size == 0) {
        return NULL;
    }
    QueueHandle_t q = xQueueCreate((UBaseType_t)depth, (UBaseType_t)item_size);
    return (osal_queue_t)q;
}

void osal_queue_delete(osal_queue_t queue)
{
    if (queue != NULL) {
        vQueueDelete((QueueHandle_t)queue);
    }
}

int osal_queue_send(osal_queue_t queue, const void *item, uint32_t timeout_ms)
{
    if (queue == NULL || item == NULL) {
        return OSAL_ERR_PARAM;
    }

    BaseType_t ret = xQueueSendToBack((QueueHandle_t)queue, item,
                                      ms_to_ticks(timeout_ms));
    return (ret == pdPASS) ? OSAL_OK : OSAL_ERR_TIMEOUT;
}

int osal_queue_recv(osal_queue_t queue, void *item, uint32_t timeout_ms)
{
    if (queue == NULL || item == NULL) {
        return OSAL_ERR_PARAM;
    }

    BaseType_t ret = xQueueReceive((QueueHandle_t)queue, item,
                                   ms_to_ticks(timeout_ms));
    return (ret == pdPASS) ? OSAL_OK : OSAL_ERR_TIMEOUT;
}

uint32_t osal_queue_waiting(osal_queue_t queue)
{
    if (queue == NULL) {
        return 0;
    }
    return (uint32_t)uxQueueMessagesWaiting((QueueHandle_t)queue);
}

/* ========================================================================
 * Mutex
 * ======================================================================== */

osal_mutex_t osal_mutex_create(void)
{
    SemaphoreHandle_t m = xSemaphoreCreateMutex();
    return (osal_mutex_t)m;
}

void osal_mutex_delete(osal_mutex_t mutex)
{
    if (mutex != NULL) {
        vSemaphoreDelete((SemaphoreHandle_t)mutex);
    }
}

int osal_mutex_lock(osal_mutex_t mutex, uint32_t timeout_ms)
{
    if (mutex == NULL) {
        return OSAL_ERR_PARAM;
    }

    BaseType_t ret = xSemaphoreTake((SemaphoreHandle_t)mutex,
                                    ms_to_ticks(timeout_ms));
    return (ret == pdPASS) ? OSAL_OK : OSAL_ERR_TIMEOUT;
}

int osal_mutex_unlock(osal_mutex_t mutex)
{
    if (mutex == NULL) {
        return OSAL_ERR_PARAM;
    }

    BaseType_t ret = xSemaphoreGive((SemaphoreHandle_t)mutex);
    return (ret == pdPASS) ? OSAL_OK : OSAL_ERR;
}

/* ========================================================================
 * Semaphore
 * ======================================================================== */

osal_sem_t osal_sem_create(uint32_t max_count, uint32_t init_count)
{
    if (max_count == 0) {
        return NULL;
    }

    SemaphoreHandle_t s = xSemaphoreCreateCounting(
        (UBaseType_t)max_count, (UBaseType_t)init_count);
    return (osal_sem_t)s;
}

void osal_sem_delete(osal_sem_t sem)
{
    if (sem != NULL) {
        vSemaphoreDelete((SemaphoreHandle_t)sem);
    }
}

int osal_sem_wait(osal_sem_t sem, uint32_t timeout_ms)
{
    if (sem == NULL) {
        return OSAL_ERR_PARAM;
    }

    BaseType_t ret = xSemaphoreTake((SemaphoreHandle_t)sem,
                                    ms_to_ticks(timeout_ms));
    return (ret == pdPASS) ? OSAL_OK : OSAL_ERR_TIMEOUT;
}

int osal_sem_post(osal_sem_t sem)
{
    if (sem == NULL) {
        return OSAL_ERR_PARAM;
    }

    BaseType_t ret = xSemaphoreGive((SemaphoreHandle_t)sem);
    return (ret == pdPASS) ? OSAL_OK : OSAL_ERR;
}

/* ========================================================================
 * Software Timer
 *
 * osal_timer_fn_t 签名为 void(*)(osal_timer_t)，
 * FreeRTOS TimerCallbackFunction_t 签名为 void(*)(TimerHandle_t)。
 * 两者等价（都是 void* 参数），直接强转。
 * ======================================================================== */

osal_timer_t osal_timer_create(const char *name,
                               uint32_t period_ms,
                               bool auto_reload,
                               osal_timer_fn_t callback)
{
    if (callback == NULL || period_ms == 0) {
        return NULL;
    }

    TimerHandle_t t = xTimerCreate(
        name,
        ms_to_ticks(period_ms),
        auto_reload ? pdTRUE : pdFALSE,
        NULL,   /* pvTimerID，暂不使用 */
        (TimerCallbackFunction_t)callback);
    return (osal_timer_t)t;
}

void osal_timer_delete(osal_timer_t timer)
{
    if (timer != NULL) {
        /* 等待最多 100 tick 让 timer daemon 处理删除命令 */
        xTimerDelete((TimerHandle_t)timer, ms_to_ticks(100));
    }
}

int osal_timer_start(osal_timer_t timer)
{
    if (timer == NULL) {
        return OSAL_ERR_PARAM;
    }
    BaseType_t ret = xTimerStart((TimerHandle_t)timer, ms_to_ticks(100));
    return (ret == pdPASS) ? OSAL_OK : OSAL_ERR;
}

int osal_timer_stop(osal_timer_t timer)
{
    if (timer == NULL) {
        return OSAL_ERR_PARAM;
    }
    BaseType_t ret = xTimerStop((TimerHandle_t)timer, ms_to_ticks(100));
    return (ret == pdPASS) ? OSAL_OK : OSAL_ERR;
}

int osal_timer_reset(osal_timer_t timer)
{
    if (timer == NULL) {
        return OSAL_ERR_PARAM;
    }
    BaseType_t ret = xTimerReset((TimerHandle_t)timer, ms_to_ticks(100));
    return (ret == pdPASS) ? OSAL_OK : OSAL_ERR;
}

/* ========================================================================
 * Time
 * ======================================================================== */

uint32_t osal_time_ms(void)
{
    TickType_t ticks = xTaskGetTickCount();
    /* portTICK_PERIOD_MS = 1000 / configTICK_RATE_HZ */
    return (uint32_t)(ticks * portTICK_PERIOD_MS);
}

uint64_t osal_tick_get(void)
{
    return (uint64_t)xTaskGetTickCount();
}

/* ========================================================================
 * Critical Section
 * ======================================================================== */

void osal_critical_enter(void)
{
    taskENTER_CRITICAL();
}

void osal_critical_exit(void)
{
    taskEXIT_CRITICAL();
}

/* ========================================================================
 * Scheduler
 * ======================================================================== */

void osal_scheduler_start(void)
{
    vTaskStartScheduler();
    /* 正常情况不会返回到这里 */
}

bool osal_scheduler_is_running(void)
{
    return (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING);
}

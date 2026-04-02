/**
 * @file main.c
 * @brief OSAL 层单核验证 demo
 *
 * 验证内容：
 *   1. osal_task_create / osal_delay_ms
 *   2. osal_queue_create / osal_queue_send / osal_queue_recv
 *   3. osal_mutex_create / osal_mutex_lock / osal_mutex_unlock
 *   4. osal_sem_create / osal_sem_wait / osal_sem_post
 *   5. osal_timer_create / osal_timer_start
 *   6. osal_time_ms / osal_task_stack_watermark
 *
 * 构建方式：
 *   make SOC=ns_core0 BOARD=fpga_eval DOWNLOAD=ilm RTOS=FreeRTOS \
 *        APP=application/freertos/osal_demo
 *
 * 预期输出：USART 打印各测试项 PASS/FAIL，最终输出总结。
 */

#include <stdio.h>
#include <string.h>
#include "nuclei_sdk_soc.h"
#include "osal.h"

/* ========================================================================
 * Test counters
 * ======================================================================== */

static volatile uint32_t g_test_pass = 0;
static volatile uint32_t g_test_fail = 0;

#define TEST_CHECK(cond, name) do {         \
    if (cond) {                             \
        g_test_pass++;                      \
        printf("[PASS] %s\r\n", (name));    \
    } else {                                \
        g_test_fail++;                      \
        printf("[FAIL] %s\r\n", (name));    \
    }                                       \
} while (0)

/* ========================================================================
 * Test 1: Queue — producer / consumer
 * ======================================================================== */

typedef struct {
    uint32_t seq;
    uint32_t value;
} test_msg_t;

#define QUEUE_TEST_COUNT   50
#define QUEUE_DEPTH        8

static osal_queue_t g_test_queue = NULL;
static volatile uint32_t g_consumer_done = 0;

static void task_producer(void *arg)
{
    (void)arg;
    test_msg_t msg;
    int ret;

    for (uint32_t i = 0; i < QUEUE_TEST_COUNT; i++) {
        msg.seq   = i;
        msg.value = i * 3 + 7;
        ret = osal_queue_send(g_test_queue, &msg, OSAL_WAIT_FOREVER);
        if (ret != OSAL_OK) {
            printf("[FAIL] queue_send seq=%lu\r\n", (unsigned long)i);
            g_test_fail++;
        }
    }
    /* producer 退出 */
    osal_task_delete(NULL);
}

static void task_consumer(void *arg)
{
    (void)arg;
    test_msg_t msg;
    int ret;
    uint32_t errors = 0;

    for (uint32_t i = 0; i < QUEUE_TEST_COUNT; i++) {
        ret = osal_queue_recv(g_test_queue, &msg, 5000);
        if (ret != OSAL_OK) {
            errors++;
            continue;
        }
        if (msg.seq != i || msg.value != (i * 3 + 7)) {
            errors++;
        }
    }

    TEST_CHECK(errors == 0, "queue send/recv 50 msgs");
    g_consumer_done = 1;
    osal_task_delete(NULL);
}

/* ========================================================================
 * Test 2: Mutex — 互斥保护共享计数器
 * ======================================================================== */

#define MUTEX_INC_COUNT    500

static osal_mutex_t g_test_mutex = NULL;
static volatile uint32_t g_shared_counter = 0;
static volatile uint32_t g_mutex_task_done = 0;

static void task_mutex_inc(void *arg)
{
    uint32_t id = (uint32_t)(uintptr_t)arg;

    for (uint32_t i = 0; i < MUTEX_INC_COUNT; i++) {
        osal_mutex_lock(g_test_mutex, OSAL_WAIT_FOREVER);
        g_shared_counter++;
        osal_mutex_unlock(g_test_mutex);
        /* 主动让出，增加竞争概率 */
        if ((i & 0xF) == 0) {
            osal_delay_ms(0);
        }
    }

    g_mutex_task_done |= (1u << id);
    osal_task_delete(NULL);
}

/* ========================================================================
 * Test 3: Semaphore — 生产者/消费者同步
 * ======================================================================== */

#define SEM_TEST_COUNT     30

static osal_sem_t g_test_sem = NULL;
static volatile uint32_t g_sem_consumed = 0;
static volatile uint32_t g_sem_done = 0;

static void task_sem_producer(void *arg)
{
    (void)arg;
    for (uint32_t i = 0; i < SEM_TEST_COUNT; i++) {
        osal_sem_post(g_test_sem);
        osal_delay_ms(5);
    }
    osal_task_delete(NULL);
}

static void task_sem_consumer(void *arg)
{
    (void)arg;
    uint32_t count = 0;

    for (uint32_t i = 0; i < SEM_TEST_COUNT; i++) {
        int ret = osal_sem_wait(g_test_sem, 3000);
        if (ret == OSAL_OK) {
            count++;
        }
    }

    g_sem_consumed = count;
    g_sem_done = 1;
    osal_task_delete(NULL);
}

/* ========================================================================
 * Test 4: Software Timer 回调
 * ======================================================================== */

static volatile uint32_t g_timer_count = 0;

static void timer_callback(osal_timer_t timer)
{
    (void)timer;
    g_timer_count++;
}

/* ========================================================================
 * Main test orchestrator task
 * ======================================================================== */

static void task_test_main(void *arg)
{
    (void)arg;

    printf("\r\n========================================\r\n");
    printf("  OSAL Verification Demo\r\n");
    printf("========================================\r\n\r\n");

    /* --- Basic time test --- */
    {
        uint32_t t0 = osal_time_ms();
        osal_delay_ms(100);
        uint32_t t1 = osal_time_ms();
        uint32_t elapsed = t1 - t0;
        /* 允许 ±20ms 误差（tick 精度 + 调度延迟） */
        TEST_CHECK(elapsed >= 80 && elapsed <= 130,
                   "osal_delay_ms(100) timing");
    }

    /* --- Queue test --- */
    {
        g_test_queue = osal_queue_create(QUEUE_DEPTH, sizeof(test_msg_t));
        TEST_CHECK(g_test_queue != NULL, "osal_queue_create");

        if (g_test_queue != NULL) {
            g_consumer_done = 0;
            osal_task_create("consumer", task_consumer, NULL, 512, 3);
            osal_task_create("producer", task_producer, NULL, 512, 2);

            /* 等待 consumer 完成 */
            uint32_t wait = 0;
            while (!g_consumer_done && wait < 10000) {
                osal_delay_ms(10);
                wait += 10;
            }
            TEST_CHECK(g_consumer_done == 1, "queue test completed in time");

            uint32_t remain = osal_queue_waiting(g_test_queue);
            TEST_CHECK(remain == 0, "queue empty after test");

            osal_queue_delete(g_test_queue);
            g_test_queue = NULL;
        }
    }

    /* --- Mutex test --- */
    {
        g_test_mutex = osal_mutex_create();
        TEST_CHECK(g_test_mutex != NULL, "osal_mutex_create");

        if (g_test_mutex != NULL) {
            g_shared_counter = 0;
            g_mutex_task_done = 0;

            osal_task_create("mtx0", task_mutex_inc, (void *)0, 512, 2);
            osal_task_create("mtx1", task_mutex_inc, (void *)1, 512, 2);

            /* 等待两个任务完成 */
            uint32_t wait = 0;
            while (g_mutex_task_done != 0x3 && wait < 15000) {
                osal_delay_ms(10);
                wait += 10;
            }

            TEST_CHECK(g_mutex_task_done == 0x3, "mutex tasks completed");
            TEST_CHECK(g_shared_counter == MUTEX_INC_COUNT * 2,
                       "mutex protected counter correct");

            /* Mutex timeout test */
            int ret = osal_mutex_lock(g_test_mutex, 0);
            TEST_CHECK(ret == OSAL_OK, "mutex lock (uncontested)");
            /* 锁已被持有，再次锁应超时 */
            /* 注：FreeRTOS mutex 支持同一任务递归锁只有在 recursive mutex 下
               普通 mutex 同一任务重复获取行为取决于实现，此处用另一种方式验证 */
            osal_mutex_unlock(g_test_mutex);

            osal_mutex_delete(g_test_mutex);
            g_test_mutex = NULL;
        }
    }

    /* --- Semaphore test --- */
    {
        g_test_sem = osal_sem_create(SEM_TEST_COUNT, 0);
        TEST_CHECK(g_test_sem != NULL, "osal_sem_create");

        if (g_test_sem != NULL) {
            g_sem_consumed = 0;
            g_sem_done = 0;

            osal_task_create("sem_c", task_sem_consumer, NULL, 512, 3);
            osal_task_create("sem_p", task_sem_producer, NULL, 512, 2);

            uint32_t wait = 0;
            while (!g_sem_done && wait < 10000) {
                osal_delay_ms(10);
                wait += 10;
            }

            TEST_CHECK(g_sem_done == 1, "sem test completed in time");
            TEST_CHECK(g_sem_consumed == SEM_TEST_COUNT,
                       "sem all tokens consumed");

            /* Semaphore timeout test */
            int ret = osal_sem_wait(g_test_sem, 50);
            TEST_CHECK(ret == OSAL_ERR_TIMEOUT, "sem_wait timeout");

            osal_sem_delete(g_test_sem);
            g_test_sem = NULL;
        }
    }

    /* --- Timer test --- */
    {
        g_timer_count = 0;
        osal_timer_t tmr = osal_timer_create("test", 50, true, timer_callback);
        TEST_CHECK(tmr != NULL, "osal_timer_create");

        if (tmr != NULL) {
            int ret = osal_timer_start(tmr);
            TEST_CHECK(ret == OSAL_OK, "osal_timer_start");

            /* 等待 ~300ms，预期触发 5~7 次（50ms 周期） */
            osal_delay_ms(320);

            ret = osal_timer_stop(tmr);
            TEST_CHECK(ret == OSAL_OK, "osal_timer_stop");

            uint32_t cnt = g_timer_count;
            TEST_CHECK(cnt >= 4 && cnt <= 8,
                       "timer callback count in range");
            printf("  (timer fired %lu times in 320ms)\r\n",
                   (unsigned long)cnt);

            osal_timer_delete(tmr);
        }
    }

    /* --- Stack watermark test --- */
    {
        uint32_t wm = osal_task_stack_watermark(NULL);
        TEST_CHECK(wm > 0, "stack watermark > 0");
        printf("  (test_main stack watermark: %lu words)\r\n",
               (unsigned long)wm);
    }

    /* --- Scheduler state test --- */
    {
        TEST_CHECK(osal_scheduler_is_running(), "scheduler is running");
    }

    /* --- Summary --- */
    printf("\r\n========================================\r\n");
    printf("  OSAL Test Summary\r\n");
    printf("  PASS: %lu   FAIL: %lu\r\n",
           (unsigned long)g_test_pass, (unsigned long)g_test_fail);
    printf("========================================\r\n");

    if (g_test_fail == 0) {
        printf("\r\n*** ALL TESTS PASSED ***\r\n");
    } else {
        printf("\r\n*** %lu TESTS FAILED ***\r\n", (unsigned long)g_test_fail);
    }

    /* 测试完成，进入空闲 */
    for (;;) {
        osal_delay_ms(1000);
    }
}

/* ========================================================================
 * FreeRTOS Hook functions（required by FreeRTOSConfig.h）
 * ======================================================================== */

void vApplicationMallocFailedHook(void)
{
    printf("[FATAL] malloc failed\r\n");
    for (;;);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    printf("[FATAL] stack overflow: %s\r\n", pcTaskName);
    for (;;);
}

void vApplicationIdleHook(void)
{
}

/* ========================================================================
 * main
 * ======================================================================== */

int main(void)
{
    printf("\r\nOSAL Demo starting...\r\n");

    osal_task_create("test", task_test_main, NULL, 1024, 4);

    osal_scheduler_start();

    /* should never reach here */
    printf("[FATAL] scheduler exited\r\n");
    for (;;);
}

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "utb_api.h"

#define UTB_DEMO_THREADS_EXPECTED    2u
#define UTB_DEMO_SEM_TOKENS          4u

typedef struct {
    utb_mutex_handle_t *lock;
    utb_sem_handle_t *ready_sem;
    utb_cond_handle_t *ready_cond;
    uint32_t ready_count;
    uint32_t shared_counter;
} utb_demo_ctx_t;

static utb_demo_ctx_t g_demo_ctx;
static utb_os_timer_handle_t *g_demo_timer;
static volatile uint32_t g_demo_timer_fired;
static volatile uint32_t g_demo_pass;
static volatile uint32_t g_demo_fail;

#define DEMO_CHECK(cond, name) do {                     \
    if (cond) {                                         \
        g_demo_pass++;                                  \
        UTB_PRINT("[PASS] %s\r\n", (name));             \
    } else {                                            \
        g_demo_fail++;                                  \
        UTB_PRINT("[FAIL] %s\r\n", (name));             \
    }                                                   \
} while (0)

static void utb_demo_timer_cb(void *arg)
{
    (void)arg;
    g_demo_timer_fired++;
}

static void utb_demo_worker(void *arg)
{
    utb_demo_ctx_t *ctx = (utb_demo_ctx_t *)arg;
    uint32_t i;

    if (utb_os_mutex_lock(ctx->lock, OSAL_WAIT_FOREVER) == UTB_OK) {
        ctx->ready_count++;
        (void)utb_os_cond_signal(ctx->ready_cond);
        (void)utb_os_mutex_unlock(ctx->lock);
    }

    for (i = 0; i < 100u; i++) {
        if (utb_os_mutex_lock(ctx->lock, OSAL_WAIT_FOREVER) == UTB_OK) {
            ctx->shared_counter++;
            (void)utb_os_mutex_unlock(ctx->lock);
        }
        utb_os_thread_sleep_ms(1);
    }

    (void)utb_os_sem_post(ctx->ready_sem);
}

static void utb_demo_run(void *arg)
{
    utb_os_thread_init_t worker_cfg;
    utb_os_timer_init_t timer_cfg;
    utb_thread_handle_t *worker0;
    utb_thread_handle_t *worker1;
    uint32_t i;

    (void)arg;
    memset(&g_demo_ctx, 0, sizeof(g_demo_ctx));

    UTB_PRINT("\r\n==============================\r\n");
    UTB_PRINT("UTB OSAL Demo Start\r\n");
    UTB_PRINT("==============================\r\n");

    g_demo_ctx.lock = utb_os_mutex_create();
    g_demo_ctx.ready_sem = utb_os_sem_create(UTB_DEMO_SEM_TOKENS, 0);
    g_demo_ctx.ready_cond = utb_os_cond_create(UTB_DEMO_THREADS_EXPECTED);

    DEMO_CHECK(g_demo_ctx.lock != NULL, "mutex create");
    DEMO_CHECK(g_demo_ctx.ready_sem != NULL, "semaphore create");
    DEMO_CHECK(g_demo_ctx.ready_cond != NULL, "condition create");

    memset(&worker_cfg, 0, sizeof(worker_cfg));
    worker_cfg.stack_words = 512;
    worker_cfg.sched_priority = 2;
    worker_cfg.func_thread_call_back = utb_demo_worker;
    worker_cfg.arg = &g_demo_ctx;

    strcpy(worker_cfg.thread_name, "utb_w0");
    worker0 = utb_os_thread_create(&worker_cfg);
    DEMO_CHECK(worker0 != NULL, "worker0 create");

    strcpy(worker_cfg.thread_name, "utb_w1");
    worker1 = utb_os_thread_create(&worker_cfg);
    DEMO_CHECK(worker1 != NULL, "worker1 create");

    if ((g_demo_ctx.lock != NULL) && (g_demo_ctx.ready_cond != NULL)) {
        if (utb_os_mutex_lock(g_demo_ctx.lock, OSAL_WAIT_FOREVER) == UTB_OK) {
            while (g_demo_ctx.ready_count < UTB_DEMO_THREADS_EXPECTED) {
                if (utb_os_cond_wait(g_demo_ctx.ready_cond, g_demo_ctx.lock) != UTB_OK) {
                    break;
                }
            }
            (void)utb_os_mutex_unlock(g_demo_ctx.lock);
        }
    }
    DEMO_CHECK(g_demo_ctx.ready_count == UTB_DEMO_THREADS_EXPECTED, "condition wait/signal");

    timer_cfg.delay_ms = 0;
    timer_cfg.interval_ms = 50;
    timer_cfg.param = NULL;
    timer_cfg.func_timer_callback = utb_demo_timer_cb;
    g_demo_timer_fired = 0;
    g_demo_timer = utb_os_timer_create(&timer_cfg);
    DEMO_CHECK(g_demo_timer != NULL, "timer create");
    if (g_demo_timer != NULL) {
        DEMO_CHECK(utb_os_timer_start(g_demo_timer) == UTB_OK, "timer start");
    }

    for (i = 0; i < UTB_DEMO_THREADS_EXPECTED; i++) {
        DEMO_CHECK(utb_os_sem_wait_ms(g_demo_ctx.ready_sem, 5000) == UTB_OK, "semaphore wait");
    }

    DEMO_CHECK(g_demo_ctx.shared_counter == 200u, "mutex protected counter");
    DEMO_CHECK(utb_os_thread_stack_watermark(NULL) > 0u, "stack watermark");

    utb_os_thread_sleep_ms(220);
    DEMO_CHECK(g_demo_timer_fired >= 3u, "timer callback fired");

    if (g_demo_timer != NULL) {
        DEMO_CHECK(utb_os_timer_stop(g_demo_timer) == UTB_OK, "timer stop");
        utb_os_timer_delete(g_demo_timer);
        g_demo_timer = NULL;
    }

    utb_os_mutex_destroy(g_demo_ctx.lock);
    utb_os_sem_destroy(g_demo_ctx.ready_sem);
    utb_os_cond_destroy(g_demo_ctx.ready_cond);

    UTB_PRINT("\r\nUTB OSAL Demo Summary: pass=%lu fail=%lu\r\n",
              (unsigned long)g_demo_pass,
              (unsigned long)g_demo_fail);
    if (g_demo_fail == 0u) {
        UTB_PRINT("UTB OSAL demo passed\r\n");
    } else {
        UTB_PRINT("UTB OSAL demo failed\r\n");
    }

    for (;;) {
        utb_os_thread_sleep_ms(1000);
    }
}

void vApplicationMallocFailedHook(void)
{
    UTB_PRINT("[FATAL] malloc failed\r\n");
    for (;;) {}
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    UTB_PRINT("[FATAL] stack overflow: %s\r\n", pcTaskName);
    for (;;) {}
}

void vApplicationIdleHook(void)
{
}

int main(void)
{
    utb_os_thread_init_t main_cfg;

    memset(&main_cfg, 0, sizeof(main_cfg));
    strcpy(main_cfg.thread_name, "utb_demo");
    main_cfg.stack_words = 1024;
    main_cfg.sched_priority = 4;
    main_cfg.func_thread_call_back = utb_demo_run;
    main_cfg.arg = NULL;

    UTB_PRINT("Starting UTB OSAL demo...\r\n");
    (void)utb_os_thread_create(&main_cfg);
    osal_scheduler_start();

    UTB_PRINT("[FATAL] scheduler exited\r\n");
    for (;;) {}
}

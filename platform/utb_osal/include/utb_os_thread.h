#ifndef UTB_OS_THREAD_H
#define UTB_OS_THREAD_H

#include <stdint.h>

#include "osal.h"
#include "utb_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*utb_thread_entry_t)(void *arg);

typedef struct {
    char thread_name[32];
    uint32_t stack_words;
    uint32_t sched_priority;
    utb_thread_entry_t func_thread_call_back;
    void *arg;
} utb_os_thread_init_t;

typedef struct {
    void *private;
} utb_thread_handle_t;

typedef struct {
    void *private;
} utb_mutex_handle_t;

typedef struct {
    void *private;
} utb_sem_handle_t;

typedef struct {
    void *private;
} utb_cond_handle_t;

utb_thread_handle_t *utb_os_thread_create(const utb_os_thread_init_t *init_cfg);
void utb_os_thread_destroy(utb_thread_handle_t *thread_hdle);
void utb_os_thread_sleep_ms(uint32_t ms);
uint32_t utb_os_thread_stack_watermark(utb_thread_handle_t *thread_hdle);

utb_mutex_handle_t *utb_os_mutex_create(void);
void utb_os_mutex_destroy(utb_mutex_handle_t *mutex_hdle);
int utb_os_mutex_lock(utb_mutex_handle_t *mutex_hdle, uint32_t timeout_ms);
int utb_os_mutex_unlock(utb_mutex_handle_t *mutex_hdle);

utb_sem_handle_t *utb_os_sem_create(uint32_t max_count, uint32_t init_count);
void utb_os_sem_destroy(utb_sem_handle_t *sem_hdle);
int utb_os_sem_wait_ms(utb_sem_handle_t *sem_hdle, uint32_t timeout_ms);
int utb_os_sem_post(utb_sem_handle_t *sem_hdle);

utb_cond_handle_t *utb_os_cond_create(uint32_t max_waiters);
void utb_os_cond_destroy(utb_cond_handle_t *cond_hdle);
int utb_os_cond_wait(utb_cond_handle_t *cond_hdle, utb_mutex_handle_t *mutex_hdle);
int utb_os_cond_signal(utb_cond_handle_t *cond_hdle);
int utb_os_cond_broadcast(utb_cond_handle_t *cond_hdle);

/* Compatibility aliases for legacy names. */
#define utb_os_thread_cond_init()                utb_os_cond_create(8)
#define utb_os_thread_cond_destroy(h)            utb_os_cond_destroy(h)
#define utb_os_thread_cond_wait(c, m)            utb_os_cond_wait(c, m)
#define utb_os_thread_cond_signal(c)             utb_os_cond_signal(c)
#define utb_os_thread_cond_broadcast(c)          utb_os_cond_broadcast(c)
#define utb_os_thread_mutex_init()               utb_os_mutex_create()
#define utb_os_thread_mutex_destroy(h)           utb_os_mutex_destroy(h)
#define utb_os_thread_mutex_lock(h)              utb_os_mutex_lock(h, OSAL_WAIT_FOREVER)
#define utb_os_thread_mutex_unlock(h)            utb_os_mutex_unlock(h)
#define utb_os_thread_wait_ms(h, ms)             utb_os_sem_wait_ms(h, ms)

#ifdef __cplusplus
}
#endif

#endif /* UTB_OS_THREAD_H */

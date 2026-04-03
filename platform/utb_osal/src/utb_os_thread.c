#include "utb_os_thread.h"

#include "osal.h"

typedef struct {
    utb_thread_entry_t entry;
    void *arg;
} utb_thread_priv_t;

typedef struct {
    osal_mutex_t mutex;
} utb_mutex_priv_t;

typedef struct {
    osal_sem_t sem;
} utb_sem_priv_t;

typedef struct {
    osal_queue_t wait_queue;
    uint32_t max_waiters;
} utb_cond_priv_t;

static void utb_os_thread_trampoline(void *arg)
{
    utb_thread_priv_t *priv = (utb_thread_priv_t *)arg;

    if ((priv != NULL) && (priv->entry != NULL)) {
        priv->entry(priv->arg);
    }

    UTB_FREE(priv);
    osal_task_delete(NULL);
}

utb_thread_handle_t *utb_os_thread_create(const utb_os_thread_init_t *init_cfg)
{
    utb_thread_handle_t *handle;
    utb_thread_priv_t *priv;
    osal_task_t task;

    if ((init_cfg == NULL) || (init_cfg->func_thread_call_back == NULL)) {
        return NULL;
    }

    handle = (utb_thread_handle_t *)UTB_MALLOC(sizeof(*handle));
    priv = (utb_thread_priv_t *)UTB_MALLOC(sizeof(*priv));
    if ((handle == NULL) || (priv == NULL)) {
        UTB_FREE(handle);
        UTB_FREE(priv);
        return NULL;
    }

    priv->entry = init_cfg->func_thread_call_back;
    priv->arg = init_cfg->arg;

    task = osal_task_create(init_cfg->thread_name,
                            utb_os_thread_trampoline,
                            priv,
                            (init_cfg->stack_words == 0) ? 512u : init_cfg->stack_words,
                            init_cfg->sched_priority);
    if (task == NULL) {
        UTB_FREE(priv);
        UTB_FREE(handle);
        return NULL;
    }

    handle->private = task;
    return handle;
}

void utb_os_thread_destroy(utb_thread_handle_t *thread_hdle)
{
    if (thread_hdle == NULL) {
        osal_task_delete(NULL);
        return;
    }

    if (thread_hdle->private != NULL) {
        osal_task_delete((osal_task_t)thread_hdle->private);
    }
    UTB_FREE(thread_hdle);
}

void utb_os_thread_sleep_ms(uint32_t ms)
{
    osal_delay_ms(ms);
}

uint32_t utb_os_thread_stack_watermark(utb_thread_handle_t *thread_hdle)
{
    return osal_task_stack_watermark((thread_hdle != NULL) ? (osal_task_t)thread_hdle->private : NULL);
}

utb_mutex_handle_t *utb_os_mutex_create(void)
{
    utb_mutex_handle_t *handle;
    utb_mutex_priv_t *priv;

    handle = (utb_mutex_handle_t *)UTB_MALLOC(sizeof(*handle));
    priv = (utb_mutex_priv_t *)UTB_MALLOC(sizeof(*priv));
    if ((handle == NULL) || (priv == NULL)) {
        UTB_FREE(handle);
        UTB_FREE(priv);
        return NULL;
    }

    priv->mutex = osal_mutex_create();
    if (priv->mutex == NULL) {
        UTB_FREE(handle);
        UTB_FREE(priv);
        return NULL;
    }

    handle->private = priv;
    return handle;
}

void utb_os_mutex_destroy(utb_mutex_handle_t *mutex_hdle)
{
    utb_mutex_priv_t *priv;

    if ((mutex_hdle == NULL) || (mutex_hdle->private == NULL)) {
        return;
    }

    priv = (utb_mutex_priv_t *)mutex_hdle->private;
    osal_mutex_delete(priv->mutex);
    UTB_FREE(priv);
    UTB_FREE(mutex_hdle);
}

int utb_os_mutex_lock(utb_mutex_handle_t *mutex_hdle, uint32_t timeout_ms)
{
    utb_mutex_priv_t *priv;

    if ((mutex_hdle == NULL) || (mutex_hdle->private == NULL)) {
        return UTB_ERR_PARAM;
    }

    priv = (utb_mutex_priv_t *)mutex_hdle->private;
    switch (osal_mutex_lock(priv->mutex, timeout_ms)) {
    case OSAL_OK:
        return UTB_OK;
    case OSAL_ERR_TIMEOUT:
        return UTB_ERR_TIMEOUT;
    default:
        return UTB_ERR;
    }
}

int utb_os_mutex_unlock(utb_mutex_handle_t *mutex_hdle)
{
    utb_mutex_priv_t *priv;

    if ((mutex_hdle == NULL) || (mutex_hdle->private == NULL)) {
        return UTB_ERR_PARAM;
    }

    priv = (utb_mutex_priv_t *)mutex_hdle->private;
    return (osal_mutex_unlock(priv->mutex) == OSAL_OK) ? UTB_OK : UTB_ERR;
}

utb_sem_handle_t *utb_os_sem_create(uint32_t max_count, uint32_t init_count)
{
    utb_sem_handle_t *handle;
    utb_sem_priv_t *priv;

    handle = (utb_sem_handle_t *)UTB_MALLOC(sizeof(*handle));
    priv = (utb_sem_priv_t *)UTB_MALLOC(sizeof(*priv));
    if ((handle == NULL) || (priv == NULL)) {
        UTB_FREE(handle);
        UTB_FREE(priv);
        return NULL;
    }

    priv->sem = osal_sem_create(max_count, init_count);
    if (priv->sem == NULL) {
        UTB_FREE(handle);
        UTB_FREE(priv);
        return NULL;
    }

    handle->private = priv;
    return handle;
}

void utb_os_sem_destroy(utb_sem_handle_t *sem_hdle)
{
    utb_sem_priv_t *priv;

    if ((sem_hdle == NULL) || (sem_hdle->private == NULL)) {
        return;
    }

    priv = (utb_sem_priv_t *)sem_hdle->private;
    osal_sem_delete(priv->sem);
    UTB_FREE(priv);
    UTB_FREE(sem_hdle);
}

int utb_os_sem_wait_ms(utb_sem_handle_t *sem_hdle, uint32_t timeout_ms)
{
    utb_sem_priv_t *priv;

    if ((sem_hdle == NULL) || (sem_hdle->private == NULL)) {
        return UTB_ERR_PARAM;
    }

    priv = (utb_sem_priv_t *)sem_hdle->private;
    switch (osal_sem_wait(priv->sem, timeout_ms)) {
    case OSAL_OK:
        return UTB_OK;
    case OSAL_ERR_TIMEOUT:
        return UTB_ERR_TIMEOUT;
    default:
        return UTB_ERR;
    }
}

int utb_os_sem_post(utb_sem_handle_t *sem_hdle)
{
    utb_sem_priv_t *priv;

    if ((sem_hdle == NULL) || (sem_hdle->private == NULL)) {
        return UTB_ERR_PARAM;
    }

    priv = (utb_sem_priv_t *)sem_hdle->private;
    return (osal_sem_post(priv->sem) == OSAL_OK) ? UTB_OK : UTB_ERR;
}

utb_cond_handle_t *utb_os_cond_create(uint32_t max_waiters)
{
    utb_cond_handle_t *handle;
    utb_cond_priv_t *priv;

    if (max_waiters == 0) {
        max_waiters = 8;
    }

    handle = (utb_cond_handle_t *)UTB_MALLOC(sizeof(*handle));
    priv = (utb_cond_priv_t *)UTB_MALLOC(sizeof(*priv));
    if ((handle == NULL) || (priv == NULL)) {
        UTB_FREE(handle);
        UTB_FREE(priv);
        return NULL;
    }

    priv->wait_queue = osal_queue_create(max_waiters, sizeof(uint32_t));
    if (priv->wait_queue == NULL) {
        UTB_FREE(handle);
        UTB_FREE(priv);
        return NULL;
    }

    priv->max_waiters = max_waiters;
    handle->private = priv;
    return handle;
}

void utb_os_cond_destroy(utb_cond_handle_t *cond_hdle)
{
    utb_cond_priv_t *priv;

    if ((cond_hdle == NULL) || (cond_hdle->private == NULL)) {
        return;
    }

    priv = (utb_cond_priv_t *)cond_hdle->private;
    osal_queue_delete(priv->wait_queue);
    UTB_FREE(priv);
    UTB_FREE(cond_hdle);
}

int utb_os_cond_wait(utb_cond_handle_t *cond_hdle, utb_mutex_handle_t *mutex_hdle)
{
    utb_cond_priv_t *cond_priv;
    uint32_t token = 0;
    int ret;

    if ((cond_hdle == NULL) || (cond_hdle->private == NULL) ||
        (mutex_hdle == NULL) || (mutex_hdle->private == NULL)) {
        return UTB_ERR_PARAM;
    }

    cond_priv = (utb_cond_priv_t *)cond_hdle->private;

    ret = utb_os_mutex_unlock(mutex_hdle);
    if (ret != UTB_OK) {
        return ret;
    }

    ret = osal_queue_recv(cond_priv->wait_queue, &token, OSAL_WAIT_FOREVER);
    if (utb_os_mutex_lock(mutex_hdle, OSAL_WAIT_FOREVER) != UTB_OK) {
        return UTB_ERR;
    }

    return (ret == OSAL_OK) ? UTB_OK : UTB_ERR;
}

int utb_os_cond_signal(utb_cond_handle_t *cond_hdle)
{
    utb_cond_priv_t *cond_priv;
    uint32_t token = 1;
    int ret;

    if ((cond_hdle == NULL) || (cond_hdle->private == NULL)) {
        return UTB_ERR_PARAM;
    }

    cond_priv = (utb_cond_priv_t *)cond_hdle->private;
    ret = osal_queue_send(cond_priv->wait_queue, &token, 0);
    return (ret == OSAL_OK) ? UTB_OK : ((ret == OSAL_ERR_TIMEOUT) ? UTB_ERR_BUSY : UTB_ERR);
}

int utb_os_cond_broadcast(utb_cond_handle_t *cond_hdle)
{
    utb_cond_priv_t *cond_priv;
    uint32_t token = 1;
    uint32_t i;

    if ((cond_hdle == NULL) || (cond_hdle->private == NULL)) {
        return UTB_ERR_PARAM;
    }

    cond_priv = (utb_cond_priv_t *)cond_hdle->private;
    for (i = 0; i < cond_priv->max_waiters; i++) {
        if (osal_queue_send(cond_priv->wait_queue, &token, 0) != OSAL_OK) {
            break;
        }
    }

    return UTB_OK;
}

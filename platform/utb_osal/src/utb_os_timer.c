#include "utb_os_timer.h"

#include "osal.h"

typedef struct {
    osal_timer_t timer;
    void *arg;
    utb_timer_callback_t callback;
} utb_timer_priv_t;

static utb_timer_priv_t *g_utb_timer_slots[16];

static int utb_os_timer_slot_alloc(utb_timer_priv_t *priv)
{
    uint32_t i;

    for (i = 0; i < UTB_ARRAY_SIZE(g_utb_timer_slots); i++) {
        if (g_utb_timer_slots[i] == NULL) {
            g_utb_timer_slots[i] = priv;
            return UTB_OK;
        }
    }

    return UTB_ERR_NOMEM;
}

static void utb_os_timer_slot_free(utb_timer_priv_t *priv)
{
    uint32_t i;

    for (i = 0; i < UTB_ARRAY_SIZE(g_utb_timer_slots); i++) {
        if (g_utb_timer_slots[i] == priv) {
            g_utb_timer_slots[i] = NULL;
            return;
        }
    }
}

static void utb_os_timer_trampoline(osal_timer_t timer)
{
    utb_timer_priv_t *priv = NULL;
    uint32_t i;

    for (i = 0; i < UTB_ARRAY_SIZE(g_utb_timer_slots); i++) {
        if ((g_utb_timer_slots[i] != NULL) && (g_utb_timer_slots[i]->timer == timer)) {
            priv = g_utb_timer_slots[i];
            break;
        }
    }

    if ((priv != NULL) && (priv->callback != NULL)) {
        priv->callback(priv->arg);
    }
}

utb_os_timer_handle_t *utb_os_timer_create(const utb_os_timer_init_t *init_cfg)
{
    utb_os_timer_handle_t *handle;
    utb_timer_priv_t *priv;
    uint32_t period_ms;

    if ((init_cfg == NULL) || (init_cfg->func_timer_callback == NULL)) {
        return NULL;
    }

    period_ms = (init_cfg->interval_ms != 0) ? init_cfg->interval_ms : init_cfg->delay_ms;
    if (period_ms == 0) {
        return NULL;
    }

    handle = (utb_os_timer_handle_t *)UTB_MALLOC(sizeof(*handle));
    priv = (utb_timer_priv_t *)UTB_MALLOC(sizeof(*priv));
    if ((handle == NULL) || (priv == NULL)) {
        UTB_FREE(handle);
        UTB_FREE(priv);
        return NULL;
    }

    priv->arg = init_cfg->param;
    priv->callback = init_cfg->func_timer_callback;
    if (utb_os_timer_slot_alloc(priv) != UTB_OK) {
        UTB_FREE(handle);
        UTB_FREE(priv);
        return NULL;
    }

    priv->timer = osal_timer_create("utb_tmr",
                                    period_ms,
                                    (init_cfg->interval_ms != 0),
                                    utb_os_timer_trampoline);
    if (priv->timer == NULL) {
        utb_os_timer_slot_free(priv);
        UTB_FREE(handle);
        UTB_FREE(priv);
        return NULL;
    }

    handle->private = priv;
    return handle;
}

void utb_os_timer_delete(utb_os_timer_handle_t *handle)
{
    utb_timer_priv_t *priv;

    if ((handle == NULL) || (handle->private == NULL)) {
        return;
    }

    priv = (utb_timer_priv_t *)handle->private;
    osal_timer_delete(priv->timer);
    utb_os_timer_slot_free(priv);
    UTB_FREE(priv);
    UTB_FREE(handle);
}

int utb_os_timer_start(utb_os_timer_handle_t *handle)
{
    utb_timer_priv_t *priv;

    if ((handle == NULL) || (handle->private == NULL)) {
        return UTB_ERR_PARAM;
    }

    priv = (utb_timer_priv_t *)handle->private;
    return (osal_timer_start(priv->timer) == OSAL_OK) ? UTB_OK : UTB_ERR;
}

int utb_os_timer_stop(utb_os_timer_handle_t *handle)
{
    utb_timer_priv_t *priv;

    if ((handle == NULL) || (handle->private == NULL)) {
        return UTB_ERR_PARAM;
    }

    priv = (utb_timer_priv_t *)handle->private;
    return (osal_timer_stop(priv->timer) == OSAL_OK) ? UTB_OK : UTB_ERR;
}

int utb_os_timer_reset(utb_os_timer_handle_t *handle)
{
    utb_timer_priv_t *priv;

    if ((handle == NULL) || (handle->private == NULL)) {
        return UTB_ERR_PARAM;
    }

    priv = (utb_timer_priv_t *)handle->private;
    return (osal_timer_reset(priv->timer) == OSAL_OK) ? UTB_OK : UTB_ERR;
}

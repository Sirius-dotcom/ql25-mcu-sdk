#ifndef UTB_OS_TIMER_H
#define UTB_OS_TIMER_H

#include <stdint.h>

#include "utb_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*utb_timer_callback_t)(void *arg);

typedef struct {
    uint32_t delay_ms;
    uint32_t interval_ms;
    void *param;
    utb_timer_callback_t func_timer_callback;
} utb_os_timer_init_t;

typedef struct {
    void *private;
} utb_os_timer_handle_t;

utb_os_timer_handle_t *utb_os_timer_create(const utb_os_timer_init_t *init_cfg);
void utb_os_timer_delete(utb_os_timer_handle_t *handle);
int utb_os_timer_start(utb_os_timer_handle_t *handle);
int utb_os_timer_stop(utb_os_timer_handle_t *handle);
int utb_os_timer_reset(utb_os_timer_handle_t *handle);

#ifdef __cplusplus
}
#endif

#endif /* UTB_OS_TIMER_H */

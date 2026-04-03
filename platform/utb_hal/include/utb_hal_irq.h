#ifndef UTB_HAL_IRQ_H
#define UTB_HAL_IRQ_H

#include <stdint.h>

#include "nuclei_sdk_soc.h"
#include "utb_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*utb_hal_irq_handler_t)(void);

typedef struct {
    IRQn_Type irqn;
    uint8_t shv;
    ECLIC_TRIGGER_Type trig_mode;
    uint8_t level;
    uint8_t priority;
    utb_hal_irq_handler_t handler;
} utb_hal_irq_cfg_t;

int utb_hal_irq_register(const utb_hal_irq_cfg_t *cfg);
void utb_hal_irq_enable(IRQn_Type irqn);
void utb_hal_irq_disable(IRQn_Type irqn);

#ifdef __cplusplus
}
#endif

#endif /* UTB_HAL_IRQ_H */

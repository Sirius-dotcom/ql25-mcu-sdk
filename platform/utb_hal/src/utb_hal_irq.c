#include "utb_hal_irq.h"

int utb_hal_irq_register(const utb_hal_irq_cfg_t *cfg)
{
    if ((cfg == NULL) || (cfg->handler == NULL)) {
        return UTB_ERR_PARAM;
    }

    return (ECLIC_Register_IRQ(cfg->irqn,
                               cfg->shv,
                               cfg->trig_mode,
                               cfg->level,
                               cfg->priority,
                               (void *)cfg->handler) == 0) ? UTB_OK : UTB_ERR;
}

void utb_hal_irq_enable(IRQn_Type irqn)
{
    ECLIC_EnableIRQ(irqn);
}

void utb_hal_irq_disable(IRQn_Type irqn)
{
    ECLIC_DisableIRQ(irqn);
}

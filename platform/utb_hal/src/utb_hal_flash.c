#include "utb_hal_flash.h"

/*
 * Keep the API stable first. The SDK still exposes flash access mainly via
 * dedicated SPI/XIP helpers rather than a small generic HAL entry, so return
 * a clear unsupported status until flash ownership and backend are frozen.
 */

int utb_hal_flash_read(uint32_t addr, void *buffer, uint32_t size)
{
    (void)addr;
    (void)buffer;
    (void)size;
    return UTB_ERR_UNSUPPORTED;
}

int utb_hal_flash_prog(uint32_t addr, const void *buffer, uint32_t size)
{
    (void)addr;
    (void)buffer;
    (void)size;
    return UTB_ERR_UNSUPPORTED;
}

int utb_hal_flash_erase(uint32_t addr)
{
    (void)addr;
    return UTB_ERR_UNSUPPORTED;
}

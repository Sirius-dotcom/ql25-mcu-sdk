#ifndef UTB_HAL_FLASH_H
#define UTB_HAL_FLASH_H

#include <stdint.h>

#include "utb_port.h"

#ifdef __cplusplus
extern "C" {
#endif

int utb_hal_flash_read(uint32_t addr, void *buffer, uint32_t size);
int utb_hal_flash_prog(uint32_t addr, const void *buffer, uint32_t size);
int utb_hal_flash_erase(uint32_t addr);

#ifdef __cplusplus
}
#endif

#endif /* UTB_HAL_FLASH_H */

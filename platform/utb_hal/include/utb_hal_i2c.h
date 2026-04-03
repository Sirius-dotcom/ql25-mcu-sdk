#ifndef UTB_HAL_I2C_H
#define UTB_HAL_I2C_H

#include <stdint.h>

#include "utb_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UTB_HAL_I2C_BUS_0 = 0,
    UTB_HAL_I2C_BUS_1 = 1,
} utb_hal_i2c_bus_t;

typedef struct {
    utb_hal_i2c_bus_t bus;
    uint32_t speed_hz;
} utb_hal_i2c_cfg_t;

typedef struct {
    void *private;
} utb_hal_i2c_handle_t;

utb_hal_i2c_handle_t *utb_hal_i2c_open(const utb_hal_i2c_cfg_t *cfg);
void utb_hal_i2c_close(utb_hal_i2c_handle_t *handle);
int utb_hal_i2c_mem_write(utb_hal_i2c_handle_t *handle,
                          uint8_t slave_addr,
                          uint32_t reg_addr,
                          uint8_t reg_addr_len,
                          const uint8_t *buf,
                          uint32_t len);
int utb_hal_i2c_mem_read(utb_hal_i2c_handle_t *handle,
                         uint8_t slave_addr,
                         uint32_t reg_addr,
                         uint8_t reg_addr_len,
                         uint8_t *buf,
                         uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* UTB_HAL_I2C_H */

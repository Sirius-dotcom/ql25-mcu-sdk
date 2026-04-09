#ifndef UTB_HAL_SPI_H
#define UTB_HAL_SPI_H

#include <stdbool.h>
#include <stdint.h>

#include "utb_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UTB_HAL_SPI_BUS_QSPI1 = 0,
    UTB_HAL_SPI_BUS_QSPI2 = 1,
    UTB_HAL_SPI_BUS_QSPI3 = 2,
} utb_hal_spi_bus_t;

typedef struct {
    utb_hal_spi_bus_t bus;
    bool master;
    uint8_t mode;
    uint8_t bits_per_word;
    bool lsb_first;
    uint32_t clock_div;
} utb_hal_spi_cfg_t;

typedef struct {
    void *private;
} utb_hal_spi_handle_t;

typedef struct {
    void *inst;
} utb_hal_spi_static_storage_t;

utb_hal_spi_handle_t *utb_hal_spi_open(const utb_hal_spi_cfg_t *cfg);
int utb_hal_spi_open_static(utb_hal_spi_handle_t *handle,
                            utb_hal_spi_static_storage_t *storage,
                            const utb_hal_spi_cfg_t *cfg);
void utb_hal_spi_close(utb_hal_spi_handle_t *handle);
int utb_hal_spi_set_clock_div(utb_hal_spi_handle_t *handle, uint32_t clock_div);
int utb_hal_spi_transfer(utb_hal_spi_handle_t *handle,
                         const uint8_t *tx_buf,
                         uint8_t *rx_buf,
                         uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* UTB_HAL_SPI_H */

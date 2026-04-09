#ifndef DP_SPI_H
#define DP_SPI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void dp_spi_iomux_config(void);
int dp_spi_init(void);
int dp_spi_transfer(const uint8_t *tx_buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* DP_SPI_H */

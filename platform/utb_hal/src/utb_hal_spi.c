#include "utb_hal_spi.h"

#include "nuclei_sdk_soc.h"

typedef struct {
    QSPI_TypeDef *inst;
} utb_hal_spi_priv_t;

static QSPI_TypeDef *utb_hal_spi_bus_to_inst(utb_hal_spi_bus_t bus)
{
    switch (bus) {
    case UTB_HAL_SPI_BUS_QSPI1:
        return QSPI1;
    case UTB_HAL_SPI_BUS_QSPI2:
        return QSPI2;
    case UTB_HAL_SPI_BUS_QSPI3:
        return QSPI3;
    default:
        return NULL;
    }
}

static void utb_hal_spi_enable_bus(utb_hal_spi_bus_t bus)
{
    switch (bus) {
    case UTB_HAL_SPI_BUS_QSPI1:
        qspi1_clk_en(ENABLE);
        qspi1_set_rst(DISABLE);
        qspi1_set_rst(ENABLE);
        break;
    case UTB_HAL_SPI_BUS_QSPI2:
        qspi2_clk_en(ENABLE);
        qspi2_set_rst(DISABLE);
        qspi2_set_rst(ENABLE);
        break;
    case UTB_HAL_SPI_BUS_QSPI3:
        qspi3_clk_en(ENABLE);
        qspi3_set_rst(DISABLE);
        qspi3_set_rst(ENABLE);
        break;
    default:
        break;
    }
}

static uint32_t utb_hal_spi_clock_div_to_reg(uint32_t div)
{
    switch (div) {
    case 2:  return QSPI_SCKDIV_PRESCALER_2;
    case 4:  return QSPI_SCKDIV_PRESCALER_4;
    case 6:  return QSPI_SCKDIV_PRESCALER_6;
    case 8:  return QSPI_SCKDIV_PRESCALER_8;
    case 10: return QSPI_SCKDIV_PRESCALER_10;
    case 12: return QSPI_SCKDIV_PRESCALER_12;
    case 14: return QSPI_SCKDIV_PRESCALER_14;
    case 16: return QSPI_SCKDIV_PRESCALER_16;
    case 32: return QSPI_SCKDIV_PRESCALER_32;
    case 64: return QSPI_SCKDIV_PRESCALER_64;
    default: return QSPI_SCKDIV_PRESCALER_8;
    }
}

static int utb_hal_spi_configure(utb_hal_spi_handle_t *handle,
                                 utb_hal_spi_priv_t *priv,
                                 const utb_hal_spi_cfg_t *cfg)
{
    QSPI_InitTypeDef init_cfg = {0};

    if ((handle == NULL) || (priv == NULL) || (cfg == NULL)) {
        return UTB_ERR_PARAM;
    }

    priv->inst = utb_hal_spi_bus_to_inst(cfg->bus);
    if (priv->inst == NULL) {
        return UTB_ERR_PARAM;
    }

    utb_hal_spi_enable_bus(cfg->bus);

    QSPI_StructInit(&init_cfg);
    init_cfg.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    init_cfg.DataSize = (cfg->bits_per_word > 8u) ? QSPI_FMT_LEN_16B : QSPI_FMT_LEN_8B;
    init_cfg.CPOL = (cfg->mode & 0x2u) ? QSPI_SCKMODE_CPOL_HIGH : QSPI_SCKMODE_CPOL_LOW;
    init_cfg.CPHA = (cfg->mode & 0x1u) ? QSPI_SCKMODE_CPHA_EDGE2 : QSPI_SCKMODE_CPHA_EDGE1;
    init_cfg.SCKDIV = utb_hal_spi_clock_div_to_reg(cfg->clock_div);
    init_cfg.Endian = cfg->lsb_first ? QSPI_FMT_ENDIAN_LSB : QSPI_FMT_ENDIAN_MSB;
    init_cfg.SSM = QSPI_CR_SSM_HARD;
    init_cfg.DevMode = cfg->master ? QSPI_CR_MODE_MASTER : QSPI_CR_MODE_SLAVE;

    QSPI_Init(priv->inst, &init_cfg);
    QSPI_ClearFifo(priv->inst, QSPI_TX_FIFO);
    QSPI_ClearFifo(priv->inst, QSPI_RX_FIFO);
    QSPI_RxFifoEnable(priv->inst, ENABLE);

    handle->private = priv;
    return UTB_OK;
}

utb_hal_spi_handle_t *utb_hal_spi_open(const utb_hal_spi_cfg_t *cfg)
{
    utb_hal_spi_handle_t *handle;
    utb_hal_spi_priv_t *priv;
    int ret;

    if (cfg == NULL) {
        return NULL;
    }

    priv = (utb_hal_spi_priv_t *)UTB_MALLOC(sizeof(*priv));
    handle = (utb_hal_spi_handle_t *)UTB_MALLOC(sizeof(*handle));
    if ((priv == NULL) || (handle == NULL)) {
        UTB_FREE(priv);
        UTB_FREE(handle);
        return NULL;
    }

    ret = utb_hal_spi_configure(handle, priv, cfg);
    if (ret != UTB_OK) {
        UTB_FREE(priv);
        UTB_FREE(handle);
        return NULL;
    }

    return handle;
}

int utb_hal_spi_open_static(utb_hal_spi_handle_t *handle,
                            utb_hal_spi_static_storage_t *storage,
                            const utb_hal_spi_cfg_t *cfg)
{
    if ((handle == NULL) || (storage == NULL)) {
        return UTB_ERR_PARAM;
    }

    return utb_hal_spi_configure(handle, (utb_hal_spi_priv_t *)storage, cfg);
}

void utb_hal_spi_close(utb_hal_spi_handle_t *handle)
{
    if (handle == NULL) {
        return;
    }

    UTB_FREE(handle->private);
    UTB_FREE(handle);
}

int utb_hal_spi_set_clock_div(utb_hal_spi_handle_t *handle, uint32_t clock_div)
{
    utb_hal_spi_priv_t *priv;

    if ((handle == NULL) || (handle->private == NULL)) {
        return UTB_ERR_PARAM;
    }

    priv = (utb_hal_spi_priv_t *)handle->private;
    priv->inst->SCKDIV = utb_hal_spi_clock_div_to_reg(clock_div);
    return UTB_OK;
}

int utb_hal_spi_transfer(utb_hal_spi_handle_t *handle,
                         const uint8_t *tx_buf,
                         uint8_t *rx_buf,
                         uint32_t len)
{
    utb_hal_spi_priv_t *priv;
    uint32_t i;

    if ((handle == NULL) || (handle->private == NULL) || (len == 0u)) {
        return UTB_ERR_PARAM;
    }

    priv = (utb_hal_spi_priv_t *)handle->private;

    for (i = 0; i < len; i++) {
        uint8_t txd = (tx_buf != NULL) ? tx_buf[i] : 0xFFu;

        while (QSPI_GetFlag(priv->inst, QSPI_STATUS_TX_FULL) == SET) {
        }
        QSPI_SendData(priv->inst, txd);

        while (QSPI_GetFlag(priv->inst, QSPI_STATUS_RX_EMPTY) == SET) {
        }
        if (rx_buf != NULL) {
            rx_buf[i] = (uint8_t)QSPI_ReceiveData(priv->inst);
        } else {
            (void)QSPI_ReceiveData(priv->inst);
        }
    }

    return UTB_OK;
}

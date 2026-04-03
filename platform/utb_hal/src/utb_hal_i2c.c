#include "utb_hal_i2c.h"

#include "nuclei_sdk_soc.h"

typedef struct {
    I2C_TypeDef *inst;
} utb_hal_i2c_priv_t;

static I2C_TypeDef *utb_hal_i2c_bus_to_inst(utb_hal_i2c_bus_t bus)
{
    switch (bus) {
    case UTB_HAL_I2C_BUS_0:
        return I2C0;
    case UTB_HAL_I2C_BUS_1:
        return I2C1;
    default:
        return NULL;
    }
}

static void utb_hal_i2c_enable_bus(utb_hal_i2c_bus_t bus)
{
    switch (bus) {
    case UTB_HAL_I2C_BUS_0:
        i2c0_clk_en(ENABLE);
        i2c0_set_rst(DISABLE);
        i2c0_set_rst(ENABLE);
        break;
    case UTB_HAL_I2C_BUS_1:
        i2c1_clk_en(ENABLE);
        i2c1_set_rst(DISABLE);
        i2c1_set_rst(ENABLE);
        break;
    default:
        break;
    }
}

static uint8_t utb_hal_i2c_speed_to_sdk(uint32_t speed_hz)
{
    if (speed_hz <= 100000u) {
        return I2C_SPEED_100K;
    }
    if (speed_hz <= 400000u) {
        return I2C_SPEED_400K;
    }
    if (speed_hz <= 1000000u) {
        return I2C_SPEED_1M;
    }
    return I2C_SPEED_4M;
}

static int utb_hal_i2c_write_reg_addr(I2C_TypeDef *inst, uint32_t reg_addr, uint8_t reg_addr_len)
{
    uint8_t i;

    if ((reg_addr_len == 0u) || (reg_addr_len > 4u)) {
        return UTB_ERR_PARAM;
    }

    for (i = 0; i < reg_addr_len; i++) {
        uint8_t shift = (uint8_t)((reg_addr_len - 1u - i) * 8u);
        if (I2C_Write_Done(inst, (uint8_t)(reg_addr >> shift), I2C_TIMEOUT_VAL) != I2C_OK) {
            return UTB_ERR;
        }
    }

    return UTB_OK;
}

utb_hal_i2c_handle_t *utb_hal_i2c_open(const utb_hal_i2c_cfg_t *cfg)
{
    utb_hal_i2c_handle_t *handle;
    utb_hal_i2c_priv_t *priv;
    I2C_InitTypeDef init_cfg = {0};

    if (cfg == NULL) {
        return NULL;
    }

    handle = (utb_hal_i2c_handle_t *)UTB_MALLOC(sizeof(*handle));
    priv = (utb_hal_i2c_priv_t *)UTB_MALLOC(sizeof(*priv));
    if ((handle == NULL) || (priv == NULL)) {
        UTB_FREE(handle);
        UTB_FREE(priv);
        return NULL;
    }

    priv->inst = utb_hal_i2c_bus_to_inst(cfg->bus);
    if (priv->inst == NULL) {
        UTB_FREE(handle);
        UTB_FREE(priv);
        return NULL;
    }

    utb_hal_i2c_enable_bus(cfg->bus);

    init_cfg.speed = utb_hal_i2c_speed_to_sdk(cfg->speed_hz);
    init_cfg.mode = I2C_MASTER_MODE;
    init_cfg.time_cfg = ENABLE;
    init_cfg.auto_end = DISABLE;
    init_cfg.scl_pull = ENABLE;
    init_cfg.sda_pull = ENABLE;
    init_cfg.dma_mode = DISABLE;
    init_cfg.addr_10bit = DISABLE;
    init_cfg.dma_txdata_size = I2C_DMA_DATA_SIZE_BYTE;
    init_cfg.dma_rxdata_size = I2C_DMA_DATA_SIZE_BYTE;

    I2C_Init(priv->inst, &init_cfg);
    I2C_ACK(priv->inst, DISABLE);

    handle->private = priv;
    return handle;
}

void utb_hal_i2c_close(utb_hal_i2c_handle_t *handle)
{
    if (handle == NULL) {
        return;
    }

    UTB_FREE(handle->private);
    UTB_FREE(handle);
}

int utb_hal_i2c_mem_write(utb_hal_i2c_handle_t *handle,
                          uint8_t slave_addr,
                          uint32_t reg_addr,
                          uint8_t reg_addr_len,
                          const uint8_t *buf,
                          uint32_t len)
{
    utb_hal_i2c_priv_t *priv;
    uint32_t i;

    if ((handle == NULL) || (handle->private == NULL) || (buf == NULL) || (len == 0u)) {
        return UTB_ERR_PARAM;
    }

    priv = (utb_hal_i2c_priv_t *)handle->private;
    I2C_ACK(priv->inst, DISABLE);

    if (I2C_Send_SlaveAddr(priv->inst, slave_addr, I2C_DIR_WRITE, I2C_TIMEOUT_VAL) != I2C_OK) {
        I2C_Stop(priv->inst);
        return UTB_ERR;
    }

    if (utb_hal_i2c_write_reg_addr(priv->inst, reg_addr, reg_addr_len) != UTB_OK) {
        I2C_Stop(priv->inst);
        return UTB_ERR;
    }

    for (i = 0; i < len; i++) {
        if (I2C_Write_Done(priv->inst, buf[i], I2C_TIMEOUT_VAL) != I2C_OK) {
            I2C_Stop(priv->inst);
            return UTB_ERR;
        }
    }

    I2C_Stop(priv->inst);
    return UTB_OK;
}

int utb_hal_i2c_mem_read(utb_hal_i2c_handle_t *handle,
                         uint8_t slave_addr,
                         uint32_t reg_addr,
                         uint8_t reg_addr_len,
                         uint8_t *buf,
                         uint32_t len)
{
    utb_hal_i2c_priv_t *priv;
    uint32_t i;

    if ((handle == NULL) || (handle->private == NULL) || (buf == NULL) || (len == 0u)) {
        return UTB_ERR_PARAM;
    }

    priv = (utb_hal_i2c_priv_t *)handle->private;
    I2C_ACK(priv->inst, DISABLE);

    if (I2C_Send_SlaveAddr(priv->inst, slave_addr, I2C_DIR_WRITE, I2C_TIMEOUT_VAL) != I2C_OK) {
        I2C_Stop(priv->inst);
        return UTB_ERR;
    }

    if (utb_hal_i2c_write_reg_addr(priv->inst, reg_addr, reg_addr_len) != UTB_OK) {
        I2C_Stop(priv->inst);
        return UTB_ERR;
    }

    if (I2C_Send_SlaveAddr(priv->inst, slave_addr, I2C_DIR_READ, I2C_TIMEOUT_VAL) != I2C_OK) {
        I2C_Stop(priv->inst);
        return UTB_ERR;
    }

    if (len > 1u) {
        I2C_ACK(priv->inst, ENABLE);
    }

    for (i = 0; i < len; i++) {
        if (i == (len - 1u)) {
            I2C_ACK(priv->inst, DISABLE);
            I2C_Stop(priv->inst);
        }
        if (I2C_Read(priv->inst, &buf[i], I2C_TIMEOUT_VAL) != I2C_OK) {
            I2C_Stop(priv->inst);
            return UTB_ERR;
        }
    }

    return UTB_OK;
}

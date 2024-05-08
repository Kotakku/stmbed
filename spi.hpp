#pragma once

#include "main.h"

#ifdef HAL_SPI_MODULE_ENABLED

namespace stmbed {
class SPI {
public:
    SPI(SPI_HandleTypeDef *handle) : handle_(handle) {}

    uint8_t write(uint8_t val) {
        uint8_t rx;
        if (HAL_SPI_TransmitReceive(handle_, (uint8_t *)&val, (uint8_t *)&rx, 1, 1000) != HAL_OK) {
            return 0;
        }
        return rx;
    }

    bool write(uint8_t *tx_buf, uint8_t *rx_buf, size_t length) {
        if (HAL_SPI_TransmitReceive(handle_, (uint8_t *)tx_buf, (uint8_t *)rx_buf, length, 1000) != HAL_OK) {
            return false;
        }
        return true;
    }

    uint8_t write_dma(uint8_t val) {
        uint8_t rx;
        if (HAL_SPI_TransmitReceive_DMA(handle_, (uint8_t *)&val, (uint8_t *)&rx, 1) != HAL_OK) {
            return 0;
        }
        while (__HAL_DMA_GET_COUNTER(handle_->hdmarx) || __HAL_DMA_GET_COUNTER(handle_->hdmatx))
            ;
        return rx;
    }

    bool write_dma(uint8_t *tx_buf, uint8_t *rx_buf, size_t length) {
        if (HAL_SPI_TransmitReceive_DMA(handle_, (uint8_t *)tx_buf, (uint8_t *)&rx_buf, length) != HAL_OK) {
            return false;
        }
        while (__HAL_DMA_GET_COUNTER(handle_->hdmarx) || __HAL_DMA_GET_COUNTER(handle_->hdmatx))
            ;
        return true;
    }

private:
    SPI_HandleTypeDef *handle_;
};

} // namespace stmbed

#endif // HAL_SPI_MODULE_ENABLED

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
        HAL_StatusTypeDef st = HAL_SPI_TransmitReceive(handle_, tx_buf, rx_buf, length, 1000);
        if (st != HAL_OK) {
            // Reset SPI from error/busy state (polling mode, no DMA involved)
            HAL_SPI_Abort(handle_);
            handle_->State = HAL_SPI_STATE_READY;
            handle_->ErrorCode = HAL_SPI_ERROR_NONE;
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
        if (HAL_SPI_TransmitReceive_DMA(handle_, tx_buf, rx_buf, static_cast<uint16_t>(length)) != HAL_OK) {
            return false;
        }
        while (__HAL_DMA_GET_COUNTER(handle_->hdmarx) || __HAL_DMA_GET_COUNTER(handle_->hdmatx))
            ;
        return true;
    }

    // Start non-blocking DMA transfer (returns immediately)
    bool write_dma_start(uint8_t *tx_buf, uint8_t *rx_buf, size_t length) {
        return HAL_SPI_TransmitReceive_DMA(handle_, tx_buf, rx_buf, static_cast<uint16_t>(length)) == HAL_OK;
    }

    // Check if SPI DMA transfer is still in progress
    bool is_busy() const {
        return handle_->State != HAL_SPI_STATE_READY;
    }

    // Abort DMA transfer and fully reset SPI + DMA to clean state.
    //
    // Cannot use HAL_DMA_Abort: after SPI is disabled, GPDMA channels sit idle
    // waiting for hardware requests that will never come. HAL_DMA_Abort sets SUSP
    // and waits for SUSPF, but SUSPF never asserts because no transfer is active.
    // Instead, directly write RESET bit to force-reset the GPDMA channel.
    void abort_dma() {
        // 1. Disable SPI DMA requests and SPI peripheral
        CLEAR_BIT(handle_->Instance->CFG1, SPI_CFG1_RXDMAEN | SPI_CFG1_TXDMAEN);
        CLEAR_BIT(handle_->Instance->CR1, SPI_CR1_SPE);

        // 2. Force-reset GPDMA channels (bypass HAL_DMA_Abort entirely)
        auto force_reset_dma = [](DMA_HandleTypeDef *hdma) {
            if (!hdma || !hdma->Instance) return;
            // Direct RESET - no SUSP/SUSPF needed
            hdma->Instance->CCR |= DMA_CCR_RESET;
            for (volatile int i = 0; i < 1000; i++) {
                if ((hdma->Instance->CCR & DMA_CCR_EN) == 0) break;
            }
            hdma->State = HAL_DMA_STATE_RESET;
            HAL_DMA_Init(hdma);
        };
        force_reset_dma(handle_->hdmatx);
        force_reset_dma(handle_->hdmarx);

        // 3. Reset HAL SPI state
        handle_->State = HAL_SPI_STATE_READY;
        handle_->ErrorCode = HAL_SPI_ERROR_NONE;
    }

    SPI_HandleTypeDef *handle() { return handle_; }

private:
    SPI_HandleTypeDef *handle_;
};

} // namespace stmbed

#endif // HAL_SPI_MODULE_ENABLED

#pragma once
#ifdef HAL_UART_MODULE_ENABLED

#include "main.h"

namespace stmbed {

class Uart {
public:
    Uart(UART_HandleTypeDef *handle) : _handle(handle) {}

    HAL_StatusTypeDef write(const void *buffer, size_t size, uint32_t time_out = 10) {
        return HAL_UART_Transmit(_handle, (uint8_t *)buffer, size, time_out);
    }

    HAL_StatusTypeDef read(void *buffer, size_t size, uint32_t time_out = 10) {
        return HAL_UART_Receive(_handle, (uint8_t *)buffer, size, time_out);
    }

    void start_dma_read(uint8_t *data_p, int data_size) { HAL_UART_Receive_DMA(_handle, data_p, data_size); }

private:
    UART_HandleTypeDef *_handle;
};

} // namespace stmbed

#endif // HAL_UART_MODULE_ENABLED

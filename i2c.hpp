#pragma once

#include "main.h"

#ifdef HAL_I2C_MODULE_ENABLED

#include <vector>

namespace stmbed {
class I2C {
private:
    I2C_HandleTypeDef *handle_;

public:
    I2C(I2C_HandleTypeDef *handle) : handle_(handle) {}

    bool read(uint8_t addr, char *data, size_t length) {
        if (HAL_I2C_Master_Receive(handle_, (uint16_t)addr << 1, (uint8_t *)data, length, 100) != HAL_OK)
            return false; // error
        return true;
    }

    bool read_reg(uint8_t addr, uint8_t reg, uint8_t *buffer, size_t length) {
        HAL_I2C_Master_Transmit(handle_, addr << 1, &reg, 1, 100);
        HAL_I2C_Master_Receive(handle_, addr << 1, buffer, length, 100);

        return true;
    }

    bool write(uint8_t addr, char *data, size_t length) {
        if (HAL_I2C_Master_Transmit(handle_, (uint16_t)addr << 1, (uint8_t *)data, length, 100) != HAL_OK)
            return false; // error
        return true;
    }

    bool is_device_ready(uint8_t addr) {
        if (HAL_I2C_IsDeviceReady(handle_, (uint16_t)addr << 1, 5, 1000) != HAL_OK)
            return false; // error
        HAL_Delay(250);
        return true;
    };

    std::vector<uint8_t> get_is_device_ready(void) {
        std::vector<uint8_t> addr;
        for (uint8_t i = 1; i < 128; i++) {
            if (is_device_ready(i)) { // HAL_OK
                addr.push_back(i);    // Received an ACK at that address
            }
        }
        return addr;
    };
};

} // namespace stmbed

#endif // HAL_I2C_MODULE_ENABLED

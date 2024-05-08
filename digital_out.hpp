#pragma once

#include "main.h"

#ifdef HAL_GPIO_MODULE_ENABLED

#include "./pin_names.hpp"
namespace stmbed {
class DigitalOut {
private:
    GPIO_TypeDef *port_;
    uint16_t pin_;

public:
    DigitalOut(GPIO_TypeDef *port, uint16_t pin) : port_(port), pin_(pin) {}

    DigitalOut(PinName pin) : DigitalOut(get_port_type(pin), 1U << STM_PIN(pin)) {}

    void write(int value) { HAL_GPIO_WritePin(port_, pin_, static_cast<GPIO_PinState>(value)); }
    DigitalOut &operator=(int value) {
        write(value);
        return *this;
    }

    int read() { return HAL_GPIO_ReadPin(port_, pin_); }
    operator int() { return read(); }
};
} // namespace stmbed
#endif // HAL_GPIO_MODULE_ENABLED

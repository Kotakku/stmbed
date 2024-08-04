#pragma once

#include "main.h"

#ifdef HAL_GPIO_MODULE_ENABLED

#include "./pin_names.hpp"
#include "callbacks/callback.hpp"
#include <forward_list>
#include <functional>
#include <unordered_map>

namespace stmbed {
class DigitalIn {
private:
    GPIO_TypeDef *port_;
    uint16_t pin_;

public:
    using CallbackFnType = void();

    DigitalIn(GPIO_TypeDef *port, uint16_t pin) : port_(port), pin_(pin) {}

    DigitalIn(PinName pin) : DigitalIn(get_port_type(pin), 1U << STM_PIN(pin)) {}

    int read() { return HAL_GPIO_ReadPin(port_, pin_); }
    operator int() { return read(); }

    void attach(std::function<CallbackFnType> &&fn, uint8_t priority = 100) {
        callback::attach(pin_, std::move(fn), priority);
    }

    GPIO_TypeDef *get_port() const { return port_; }
    uint16_t get_pin() const { return pin_; }
};
} // namespace stmbed
#endif // HAL_GPIO_MODULE_ENABLED

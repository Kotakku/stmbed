#pragma once

#include "main.h"

#ifdef HAL_TIM_MODULE_ENABLED

namespace stmbed {
class PWMOut {
public:
	PWMOut(TIM_HandleTypeDef *handle, uint32_t ch) : handle_(handle), ch_(ch) { HAL_TIM_PWM_Start(handle_, ch_) }

    void write(float value) {
        if (value < 0)
            value = 0;
        if (value > 1)
            value = 1;
        uint32_t pulse = (uint32_t)((float)handle_->Init.Period * value + 0.5);
        __HAL_TIM_SET_COMPARE(handle_, ch_, pulse);
    }

    float read() {
        float value = 0;
        if (handle_->Init.Period > 0) {
            value = (float)(__HAL_TIM_GET_COMPARE(handle_, ch_)) / (float)(handle_->Init.Period);
        }
        return ((value > (float)1.0) ? (float)(1.0) : (value));
    }

    PwmOut &operator=(float value) {
        write(value);
        return *this;
    }

    PwmOut &operator=(PwmOut &rhs) {
        write(rhs.read());
        return *this;
    }

    operator float() { return read(); }

private:
    TIM_HandleTypeDef *handle_;
    uint32_t ch_;
};

} // namespace stmbed

#endif // HAL_TIM_MODULE_ENABLED

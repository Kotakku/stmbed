#pragma once

#include "main.h"
#include "misc/timer_apb.hpp"

#ifdef HAL_TIM_MODULE_ENABLED

namespace stmbed {
class PWMOut {
public:
    PWMOut(TIM_HandleTypeDef *handle, uint32_t ch) : handle_(handle), ch_(ch) {
        HAL_TIM_PWM_Start(handle_, ch_);
        apb_ = Get_TIM_APB(handle_);
        apb_timer_freq_ = Get_APB_TIM_Freq(handle_);
    }

    TIM_APB get_apb() const { return apb_; }
    uint32_t get_apb_timer_frequency() const { return apb_timer_freq_; }
    uint32_t get_prescaler() const { return handle_->Instance->PSC; }
    uint32_t get_counter_period() const { return handle_->Instance->ARR; }

    float get_timer_period() const {
        return (get_prescaler() + 1.0f) * (get_counter_period() + 1.0f) / static_cast<float>(get_apb_timer_frequency());
    }

    float get_timer_frequency() const {
        return get_apb_timer_frequency() / (get_prescaler() + 1.0f) / (get_counter_period() + 1.0f);
    }

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

    PWMOut &operator=(float value) {
        write(value);
        return *this;
    }

    PWMOut &operator=(PWMOut &rhs) {
        write(rhs.read());
        return *this;
    }

    operator float() { return read(); }

private:
    TIM_HandleTypeDef *handle_;
    uint32_t ch_;
    TIM_APB apb_;
    uint32_t apb_timer_freq_;
};

} // namespace stmbed

#endif // HAL_TIM_MODULE_ENABLED

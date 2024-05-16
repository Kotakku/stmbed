#pragma once

#include "main.h"

#ifdef HAL_TIM_MODULE_ENABLED

#include <cassert>

#include "callbacks/callback.hpp"
#include "misc/timer_apb.hpp"

namespace stmbed {

class Ticker {
public:
    using CallbackFnType = void(void);

    Ticker(TIM_HandleTypeDef *handle, uint8_t priority = 100, bool autostart = true) : handle_(handle) {
#ifdef STMBED_TIM_APB_DEFINED
        apb_ = Get_TIM_APB(handle_);
        apb_timer_freq_ = Get_APB_TIM_Freq(handle_);
#endif 
        std::function<CallbackFnType> cb = std::bind(&Ticker::callback, this);
        callback::attach(reinterpret_cast<intptr_t>(handle_), std::move(cb), priority);

        if(autostart)
            start();
    }

    ~Ticker() {
        stop();
    }

    void start() { HAL_TIM_Base_Start_IT(handle_); }
    void stop() { HAL_TIM_Base_Stop_IT(handle_); }

#ifdef STMBED_TIM_APB_DEFINED
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
#endif

    void attach(std::function<CallbackFnType> func, size_t division = 1) {
        if (division == 0)
            division = 1;
        cb_items_.push_back({func, division, 0});
    }

protected:
    void callback(void) {
        for (auto &item : cb_items_) {
            item.cnt++;
            if (item.cnt == item.div) {
                item.func();
                item.cnt = 0;
            }
        }
    }

private:
    TIM_HandleTypeDef *handle_;

#ifdef STMBED_TIM_APB_DEFINED
    TIM_APB apb_;
    uint32_t apb_timer_freq_;
#endif

    struct cb_item_t {
        std::function<CallbackFnType> func;
        size_t div;
        size_t cnt;
    };

    std::vector<cb_item_t> cb_items_;
};

} // namespace stmbed

#endif // HAL_TIM_MODULE_ENABLED

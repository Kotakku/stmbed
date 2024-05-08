#pragma once

#include "main.h"

#ifdef HAL_TIM_MODULE_ENABLED

#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <functional>
#include <map>

namespace stmbed {

#ifndef MAX_TICKER_ID
#define MAX_TICKER_ID (16 - 1)
#endif

class Ticker {
public:
    Ticker(TIM_HandleTypeDef *handle) : handle_(handle), ms_(0), start_us_(0), pretime_() {
        HAL_TIM_Base_Start_IT(handle_);
        tim_its_.insert(std::make_pair(handle_, [this] { callback(); }));
    }

    void attach(std::function<void(void)> func, size_t division = 1) {
        if (division == 0)
            division = 1;
        cb_items_.push_back({func, division, 0});
    }

    void timer_reset(float second = 0) { start_us_ += (read_us() - static_cast<uint32_t>(std::round(second * 1e6))); }

    uint32_t get_counter(void) { return __HAL_TIM_GET_COUNTER(handle_); }

    uint32_t read_us() { return (ms_ * 1000 + get_counter() - start_us_); }

    uint32_t read_ms() { return read_us() / static_cast<uint32_t>(1000); }

    float lap_time(uint8_t ticker_id, bool first_lap_time_is_zero = false) {
        assert(0 <= ticker_id);
        assert(ticker_id <= MAX_TICKER_ID);

        if (first_lap_time_is_zero && pretime_.at(ticker_id) == 0) {
            pretime_.at(ticker_id) = read_us();
        }
        uint32_t us = read_us() - pretime_.at(ticker_id);
        pretime_.at(ticker_id) = read_us();
        return static_cast<float>(us / 1e6);
    }

    float read_time() { return read_us() / 1000000.0f; };

    bool wait(uint32_t ms, uint8_t ticker_id) {
        assert(0 <= ticker_id);
        assert(ticker_id <= MAX_TICKER_ID);

        if (pretime_.at(ticker_id) == 0U) {
            pretime_.at(ticker_id) = read_ms();
            return false;
        } else {
            if (pretime_.at(ticker_id) + ms <= read_ms()) {
                pretime_.at(ticker_id) = 0;
                return true;
            }
        }
        return false;
    }
    bool await(uint32_t ms, uint8_t ticker_id) {
        assert(0 <= ticker_id);
        assert(ticker_id <= MAX_TICKER_ID);

        if (pretime_.at(ticker_id) == 0U) {
            pretime_.at(ticker_id) = read_ms();
            return false;
        } else {
            if (pretime_.at(ticker_id) + ms <= read_ms()) {
                pretime_.at(ticker_id) += ms;
                return true;
            }
        }
        return false;
    }

    // 受信割り込み用
    // 手動で呼び出ししないこと
    static void tim_it(TIM_HandleTypeDef *htim) {
        if (tim_its_.find(htim) != tim_its_.end()) {
            tim_its_.at(htim)();
        }
    }

protected:
    TIM_HandleTypeDef *handle_;

    void startup() { HAL_TIM_Base_Start_IT(handle_); }

    void callback() {
        ms_ = ms_ + 1;
        for (auto &item : cb_items_) {
            item.cnt++;
            if (item.cnt == item.div) {
                item.func();
                item.cnt = 0;
            }
        }
    }

private:
    struct cb_item_t {
        std::function<void(void)> func;
        size_t div;
        size_t cnt;
    };

    std::vector<cb_item_t> cb_items_;
    inline static std::map<TIM_HandleTypeDef *, std::function<void(void)>> tim_its_;

    volatile uint32_t ms_;
    uint32_t start_us_;
    std::array<uint32_t, MAX_TICKER_ID + 1> pretime_;
};

} // namespace stmbed

#endif // HAL_TIM_MODULE_ENABLED

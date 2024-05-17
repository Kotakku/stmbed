#include "../ticker.hpp"

#ifdef HAL_TIM_MODULE_ENABLED
#ifndef STMBED_MANUAL_TIM_CB

#include <stdio.h>

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    using namespace stmbed;
    callback::callback<Ticker::CallbackFnType>(reinterpret_cast<intptr_t>(htim));
}

#endif // STMBED_MANUAL_TIM_CB
#endif // HAL_TIM_MODULE_ENABLED

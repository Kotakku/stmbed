#pragma once

#include "main.h"
#include <algorithm>
#include <set>

#ifdef HAL_TIM_MODULE_ENABLED

namespace stmbed {

#ifdef TIM1
#define IFDEF_TIM1() TIM1,
#else
#define IFDEF_TIM1()
#endif

#ifdef TIM2
#define IFDEF_TIM2() TIM2,
#else
#define IFDEF_TIM2()
#endif

#ifdef TIM3
#define IFDEF_TIM3() TIM3,
#else
#define IFDEF_TIM3()
#endif

#ifdef TIM4
#define IFDEF_TIM4() TIM4,
#else
#define IFDEF_TIM4()
#endif

#ifdef TIM5
#define IFDEF_TIM5() TIM5,
#else
#define IFDEF_TIM5()
#endif

#ifdef TIM6
#define IFDEF_TIM6() TIM6,
#else
#define IFDEF_TIM6()
#endif

#ifdef TIM7
#define IFDEF_TIM7() TIM7,
#else
#define IFDEF_TIM7()
#endif

#ifdef TIM8
#define IFDEF_TIM8() TIM8,
#else
#define IFDEF_TIM8()
#endif

#ifdef TIM9
#define IFDEF_TIM9() TIM9,
#else
#define IFDEF_TIM9()
#endif

#ifdef TIM10
#define IFDEF_TIM10() TIM10,
#else
#define IFDEF_TIM10()
#endif

#ifdef TIM11
#define IFDEF_TIM11() TIM11,
#else
#define IFDEF_TIM11()
#endif

#ifdef TIM12
#define IFDEF_TIM12() TIM12,
#else
#define IFDEF_TIM12()
#endif

#ifdef TIM13
#define IFDEF_TIM13() TIM13,
#else
#define IFDEF_TIM13()
#endif

#ifdef TIM14
#define IFDEF_TIM14() TIM14,
#else
#define IFDEF_TIM14()
#endif

#ifdef TIM15
#define IFDEF_TIM15() TIM15,
#else
#define IFDEF_TIM15()
#endif

#ifdef TIM16
#define IFDEF_TIM16() TIM16,
#else
#define IFDEF_TIM16()
#endif

#ifdef TIM17
#define IFDEF_TIM17() TIM17,
#else
#define IFDEF_TIM17()
#endif

#ifdef TIM18
#define IFDEF_TIM18() TIM18,
#else
#define IFDEF_TIM18()
#endif

#ifdef TIM19
#define IFDEF_TIM19() TIM19,
#else
#define IFDEF_TIM19()
#endif

#ifdef TIM20
#define IFDEF_TIM20() TIM20,
#else
#define IFDEF_TIM20()
#endif

// clang-format off
static std::set<TIM_TypeDef *> apb1_timers = {
    IFDEF_TIM2()
    IFDEF_TIM3()
    IFDEF_TIM4()
    IFDEF_TIM5()
    IFDEF_TIM6()
    IFDEF_TIM7()
    IFDEF_TIM12()
    IFDEF_TIM13()
    IFDEF_TIM14()
};

static std::set<TIM_TypeDef *> apb2_timers = {
    IFDEF_TIM1()
    IFDEF_TIM8()
    IFDEF_TIM9()
    IFDEF_TIM10()
    IFDEF_TIM11()
    IFDEF_TIM15()
    IFDEF_TIM16()
    IFDEF_TIM17()
};
// clang-format on

enum TIM_APB {
    TIM_APB1,
    TIM_APB2,
};

static TIM_APB Get_TIM_APB(TIM_HandleTypeDef *htim) {
    if (std::find(apb1_timers.begin(), apb1_timers.end(), htim->Instance) != apb1_timers.end())
        return TIM_APB1;
    else if (std::find(apb2_timers.begin(), apb2_timers.end(), htim->Instance) != apb2_timers.end())
        return TIM_APB2;
    Error_Handler();
    return TIM_APB1;
}

static uint32_t Get_APB_TIM_Freq(TIM_HandleTypeDef *htim) {
    RCC_ClkInitTypeDef clkconfig;
    uint32_t pFLatency;
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    if (Get_TIM_APB(htim) == TIM_APB1) {
        uint32_t prescaler1 = (clkconfig.APB1CLKDivider == RCC_HCLK_DIV1) ? 1 : 2;
        return HAL_RCC_GetPCLK1Freq() * prescaler1;
    } else if (Get_TIM_APB(htim) == TIM_APB2) {
        uint32_t prescaler2 = (clkconfig.APB2CLKDivider == RCC_HCLK_DIV1) ? 1 : 2;
        return HAL_RCC_GetPCLK2Freq() * prescaler2;
    }
    Error_Handler();
    return 0;
}

} // namespace stmbed

#endif

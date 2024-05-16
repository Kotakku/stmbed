#pragma once

#include "main.h"
#include <set>
#include <algorithm>

#ifdef HAL_TIM_MODULE_ENABLED

namespace stmbed {

// STM32マイコンファミリごとにタイマーがAPB1とAPB2のどちらに接続されているかを定義する

#ifdef STM32F0
// static std::set<TIM_TypeDef*> apb1_timers = {};
// static std::set<TIM_TypeDef*> apb2_timers = {};
// #define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32F1
// static std::set<TIM_TypeDef*> apb1_timers = {};
// static std::set<TIM_TypeDef*> apb2_timers = {};
// #define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32F2
// static std::set<TIM_TypeDef*> apb1_timers = {};
// static std::set<TIM_TypeDef*> apb2_timers = {};
// #define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32F3
static std::set<TIM_TypeDef*> apb1_timers = {TIM2, TIM3, TIM4, TIM6, TIM7};
static std::set<TIM_TypeDef*> apb2_timers = {TIM1, TIM8, TIM15, TIM16, TIM17};
#define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32F4
static std::set<TIM_TypeDef*> apb1_timers = {TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14};
static std::set<TIM_TypeDef*> apb2_timers = {TIM1, TIM8, TIM9, TIM10, TIM11};
#define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32F7
static std::set<TIM_TypeDef*> apb1_timers = {TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14};
static std::set<TIM_TypeDef*> apb2_timers = {TIM1, TIM8, TIM9, TIM10, TIM11};
#define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32L0
// static std::set<TIM_TypeDef*> apb1_timers = {};
// static std::set<TIM_TypeDef*> apb2_timers = {};
// #define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32L1
// static std::set<TIM_TypeDef*> apb1_timers = {};
// static std::set<TIM_TypeDef*> apb2_timers = {};
// #define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32L4
// static std::set<TIM_TypeDef*> apb1_timers = {};
// static std::set<TIM_TypeDef*> apb2_timers = {};
// #define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32L5
// static std::set<TIM_TypeDef*> apb1_timers = {};
// static std::set<TIM_TypeDef*> apb2_timers = {};
// #define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32G0
// static std::set<TIM_TypeDef*> apb1_timers = {};
// static std::set<TIM_TypeDef*> apb2_timers = {};
// #define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32G4
// static std::set<TIM_TypeDef*> apb1_timers = {};
// static std::set<TIM_TypeDef*> apb2_timers = {};
// #define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32H7
// static std::set<TIM_TypeDef*> apb1_timers = {};
// static std::set<TIM_TypeDef*> apb2_timers = {};
// #define STMBED_TIM_APB_DEFINED
#endif

#ifdef STM32H5
// static std::set<TIM_TypeDef*> apb1_timers = {};
// static std::set<TIM_TypeDef*> apb2_timers = {};
// #define STMBED_TIM_APB_DEFINED
#endif

#include <stdio.h>

enum TIM_APB {
    TIM_APB1,
    TIM_APB2,
};

static TIM_APB Get_TIM_APB(TIM_HandleTypeDef *htim) {
    if(std::find(apb1_timers.begin(), apb1_timers.end(), htim->Instance) != apb1_timers.end())
        return TIM_APB1;
    else if(std::find(apb2_timers.begin(), apb2_timers.end(), htim->Instance) != apb2_timers.end())
        return TIM_APB2;
    Error_Handler();
    return TIM_APB1;
}

static uint32_t Get_APB_TIM_Freq(TIM_HandleTypeDef *htim) {
    RCC_ClkInitTypeDef clkconfig;
    uint32_t pFLatency;
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    if(Get_TIM_APB(htim)==TIM_APB1){
        uint32_t prescaler1 = (clkconfig.APB1CLKDivider == RCC_HCLK_DIV1) ? 1 : 2;
        return HAL_RCC_GetPCLK1Freq() * prescaler1;
    }
    else if(Get_TIM_APB(htim)==TIM_APB2){
        uint32_t prescaler2 = (clkconfig.APB2CLKDivider == RCC_HCLK_DIV1) ? 1 : 2;
        return HAL_RCC_GetPCLK2Freq() * prescaler2;
    }
    Error_Handler();
    return 0;
}

}

#endif

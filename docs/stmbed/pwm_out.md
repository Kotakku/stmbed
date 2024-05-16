# PWMOut

## 概要

## Cube側での設定

## リファレンス

| 返り値 | 関数名(引数) | 説明 |
| :-- | :-- | :-- |
|  |  | |

## サンプル
```C++
#include "main.h"
#include "stmbed/stmbed.hpp"

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;

extern "C" void cppmain(void) {
    using namespace stmbed;

    enable_std_printf(&huart2);
    PWMOut pwm(&htim2, TIM_CHANNEL_1);

    printf("PWM frequency: %.3f Hz\n", pwm.get_timer_frequency());
    
    double time = 0;
    while (1) {
        pwm = 0.5f + 0.5f * sin(2*time);
        time += 0.01;
        HAL_Delay(10);
    }
}
```
# Ticker

## 概要

## Cube側での設定

## リファレンス

| 返り値 | 関数名(引数) | 説明 |
| :-- | :-- | :-- |
|  |  | |

## サンプル

周期コールバックを登録する
```C++
#include "main.h"
#include "stmbed/stmbed.hpp"
#include "stmbed_modules/motor/cybergear.hpp"

extern UART_HandleTypeDef huart2;
extern CAN_HandleTypeDef hcan1;
extern TIM_HandleTypeDef htim10; // ticker

extern "C" void cppmain(void) {
    using namespace stmbed;

    enable_std_printf(&huart2);

    Ticker ticker(&htim10);
    DigitalOut led1(PA5);

    printf("Ticker frequency: %.1f Hz\n", ticker.get_timer_frequency()); // for example 1000Hz

    ticker.attach([&]() {
        led1 = !led1;
    }, 500); // 500ms period

    while (1) {
    }
}
```

タイマーの割り込み周期を確認する
```C++
#include "main.h"
#include "stmbed/stmbed.hpp"

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim10;

extern "C" void cppmain(void) {
    using namespace stmbed;

    enable_std_printf(&huart2);

    Ticker ticker(&htim10);    
    printf("Timer information\n");
    printf("APB: %d\n", (ticker.get_apb()==TIM_APB1) ? 1 : 2);
    printf("APB timer frequency: %lu [Hz]\n", ticker.get_apb_frequency());
    printf("prescaler: %lu\n", ticker.get_prescaler());
    printf("counter period: %lu\n", ticker.get_counter_period());
    printf("period: %.3f [s]\n", ticker.get_timer_period());
    printf("frequency: %.3f [Hz]\n", ticker.get_timer_frequency());

    while (1) {
    }
}

```
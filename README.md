# stmbed

Mbed-like STM32 HAL C++ Wrapper

## 使い方
CubeIDEで設定できる項目はすべて設定しておく  
ペリフェラルの初期化関数```MX_XXX_Init()```が呼び出されてからコンストラクタが呼び出されるようにすること

## example
```C++
#include "main.h"
#include "stmbed/stmbed.hpp"

extern UART_HandleTypeDef huart2;

extern "C" void cppmain(void)
{
    using namespace stmbed;

    enable_std_printf(&huart2);
    DigitalOut led(PA5);

    uint8_t cnt = 0;
    while (1)
    {
        printf("Hello World %d\n", cnt++);
        led = !led;
        HAL_Delay(500);
    }
}
```
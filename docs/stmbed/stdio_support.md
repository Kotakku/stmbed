# stdio_support

## 概要
ターミナルへの標準入出力

## Cube側での設定
UART/USARTの有効化  
浮動小数点を表示したい場合```Settings/Tool Settings/MCU Settings```から```Use float with printf from newlib-nano(-u_printf_float)```にチェックを入れるか、リンカーフラグに```-u_printf_float```を追加  
入力も同様に```-u_scanf_float```を設定する

## リファレンス
| 返り値 | 関数名(引数) | 説明 |
| :-- | :-- | :-- |
| void | enable_std_printf(UART_HandleTypeDef *handle) | 引数に取るUARTハンドラを使いprintfを使用できるようにする |
| void | enable_std_scanf(UART_HandleTypeDef *handle) | 引数に取るUARTハンドラを使いscanfを使用できるようにする |
| void | enable_stdio(UART_HandleTypeDef *handle) | 引数に取るUARTハンドラを使いprintfとscanfを使用できるようにする |

## マクロ
| マクロ | 説明 |
| :-- | :-- |
| STMBED_DISABLE_STD_PRINTF | 標準出力を無効化する |
| STMBED_DISABLE_STD_SCANF | 標準入力を無効化する |
| STMBED_DISABLE_STDIO | 標準入出力を無効化する |

## サンプル

printf
```cpp
#define STMBED_STDIO_ENABLE
#include <stmbed/stmbed.hpp>
#include <stdio.h>

extern UART_HandleTypeDef huart3;

extern "C" void cppmain()
{
    stmbed::enable_stdio(&huart3);
    uint32_t cnt = 0;
    const float pi = 3.14159265358979;

    while(1)
    {
        printf("Hello World!, cnt = %ld, pi = %f\n", cnt++, pi);

        HAL_Delay(500);
    }
}

```

scanf
```cpp
#define STMBED_STDIO_ENABLE
#include <stmbed/stmbed.hpp>
#include <stdio.h>

extern UART_HandleTypeDef huart3;

extern "C" void cppmain()
{
    stmbed::enable_stdio(&huart3);
    while(1)
    {
        char str[128];
        scanf("%s", str);
        printf("get message !: %s\n", str);
    }
}
```
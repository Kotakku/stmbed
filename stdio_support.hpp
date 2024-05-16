#pragma once

#include "main.h"

#ifdef HAL_UART_MODULE_ENABLED

#ifdef STMBED_DISABLE_STDIO
#define STMBED_DISABLE_STD_PRINTF
#define STMBED_DISABLE_STD_SCANF
#endif // STMBED_DISABLE_STDIO

#include <stdio.h>

namespace stmbed {
namespace internal {
static UART_HandleTypeDef *hstduart_;
}

#ifndef STMBED_DISABLE_STD_PRINTF
// 浮動小数点の表示を使う場合はリンカーフラグに-u_printf_floatを追加
void enable_std_printf(UART_HandleTypeDef *handle) {
    internal::hstduart_ = handle;
    setbuf(stdout, NULL);
}
#endif // STMBED_DISABLE_STD_PRINTF

#ifndef STMBED_DISABLE_STD_SCANF
// 浮動小数点の入力を使う場合はリンカーフラグに-u_scanf_floatを追加
void enable_std_scanf(UART_HandleTypeDef *handle) {
    internal::hstduart_ = handle;
    setbuf(stdin, NULL);
}
#endif // STMBED_DISABLE_STD_SCANF

#if !defined(STMBED_DISABLE_STD_PRINTF) && !defined(STMBED_DISABLE_STD_SCANF)
void enable_stdio(UART_HandleTypeDef *handle) {
    enable_std_printf(handle);
    enable_std_scanf(handle);
}
#endif

} // namespace stmbed

#ifndef STMBED_DISABLE_STD_PRINTF
extern "C" int __io_putchar(int ch) {
    if (stmbed::internal::hstduart_ != NULL)
        HAL_UART_Transmit(stmbed::internal::hstduart_, (uint8_t *)&ch, 1, 10);
    return 0;
}
#endif // STMBED_DISABLE_STD_PRINTF

#ifndef STMBED_DISABLE_STD_SCANF
extern "C" int __io_getchar(void) {
    if (stmbed::internal::hstduart_ != NULL) {
        HAL_StatusTypeDef status = HAL_BUSY;
        uint8_t data;

        while (status != HAL_OK)
            status = HAL_UART_Receive(stmbed::internal::hstduart_, &data, 1, 10);

        return data;
    }
    return 0;
}

#endif // STMBED_DISABLE_STD_SCANF

#endif // HAL_UART_MODULE_ENABLED

# DigitalOut

## 概要
IOピン1個の出力を行う

## Cube側での設定
ピンを出力設定にする

## リファレンス

| 返り値 | 関数名(引数) | 説明 |
| :-- | :-- | :-- |
|  | DigitalOut(GPIO_TypeDef* port, uint16_t pin) | ポートとピン番号を指定してインスタンス化 |
|  | DigitalOut(PinName pin) | stmbed内で定義しているピン番号を指定してインスタンス化 |
| void | write(int value) | ピン出力 0: Low出力, 1: High出力 |
| int | read() | ピンの状態を読み取る 0: Low出力, 1: High出力 |
| DigitalOut & | operator= (int value) | shorthand for write() |
|  | operator int() | shorthand for read() |

## サンプル

```cpp
#include "stmbed/stmbed.hpp"

extern "C" void cppmain()
{
    stmbed::DigitalOut led(stmbed::PB13);

    while(1)
    {
        led.write(1); // High出力
        HAL_Delay(500);
        led.write(0); // Low出力
        HAL_Delay(500);

        led.write(!led.read()); // 現在の状態を反転 -> High出力
        HAL_Delay(500);
        led.write(!led.read()); // 同上 -> Low出力
        HAL_Delay(500);
    }
}
```

operatorを使用して次のように書くこともできる。
下のコードは上のコードと同じ挙動になる
```cpp
#include "stmbed/stmbed.hpp"

extern "C" void cppmain()
{
    stmbed::DigitalOut led(stmbed::PB13);

    while(1)
    {
        led = 1;
        HAL_Delay(500);
        led = 0;
        HAL_Delay(500);

        led = !led;
        HAL_Delay(500);
        led = !led;
        HAL_Delay(500);
    }
}
```
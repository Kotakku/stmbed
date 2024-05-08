#pragma once

#include "main.h"

#define STM_PORT(X) (((uint32_t)(X) >> 4) & 0xF)
#define STM_PIN(X) ((uint32_t)(X) & 0xF)

namespace stmbed {

typedef enum {
    PA0 = 0x00,
    PA1 = 0x01,
    PA2 = 0x02,
    PA3 = 0x03,
    PA4 = 0x04,
    PA5 = 0x05,
    PA6 = 0x06,
    PA7 = 0x07,
    PA8 = 0x08,
    PA9 = 0x09,
    PA10 = 0x0A,
    PA11 = 0x0B,
    PA12 = 0x0C,
    PA13 = 0x0D,
    PA14 = 0x0E,
    PA15 = 0x0F,

    PB0 = 0x10,
    PB1 = 0x11,
    PB2 = 0x12,
    PB3 = 0x13,
    PB4 = 0x14,
    PB5 = 0x15,
    PB6 = 0x16,
    PB7 = 0x17,
    PB8 = 0x18,
    PB9 = 0x19,
    PB10 = 0x1A,
    PB11 = 0x1B,
    PB12 = 0x1C,
    PB13 = 0x1D,
    PB14 = 0x1E,
    PB15 = 0x1F,

#ifdef GPIOC_BASE
    PC0 = 0x20,
    PC1 = 0x21,
    PC2 = 0x22,
    PC3 = 0x23,
    PC4 = 0x24,
    PC5 = 0x25,
    PC6 = 0x26,
    PC7 = 0x27,
    PC8 = 0x28,
    PC9 = 0x29,
    PC10 = 0x2A,
    PC11 = 0x2B,
    PC12 = 0x2C,
    PC13 = 0x2D,
    PC14 = 0x2E,
    PC15 = 0x2F,
#endif

#ifdef GPIOD_BASE
    PD0 = 0x30,
    PD1 = 0x31,
    PD2 = 0x32,
    PD3 = 0x33,
    PD4 = 0x34,
    PD5 = 0x35,
    PD6 = 0x36,
    PD7 = 0x37,
    PD8 = 0x38,
    PD9 = 0x39,
    PD10 = 0x3A,
    PD11 = 0x3B,
    PD12 = 0x3C,
    PD13 = 0x3D,
    PD14 = 0x3E,
    PD15 = 0x3F,
#endif

#ifdef GPIOE_BASE
    PE0 = 0x40,
    PE1 = 0x41,
    PE2 = 0x42,
    PE3 = 0x43,
    PE4 = 0x44,
    PE5 = 0x45,
    PE6 = 0x46,
    PE7 = 0x47,
    PE8 = 0x48,
    PE9 = 0x49,
    PE10 = 0x4A,
    PE11 = 0x4B,
    PE12 = 0x4C,
    PE13 = 0x4D,
    PE14 = 0x4E,
    PE15 = 0x4F,
#endif

#ifdef GPIOF_BASE
    PF0 = 0x50,
    PF1 = 0x51,
    PF2 = 0x52,
    PF3 = 0x53,
    PF4 = 0x54,
    PF5 = 0x55,
    PF6 = 0x56,
    PF7 = 0x57,
    PF8 = 0x58,
    PF9 = 0x59,
    PF10 = 0x5A,
    PF11 = 0x5B,
    PF12 = 0x5C,
    PF13 = 0x5D,
    PF14 = 0x5E,
    PF15 = 0x5F,
#endif

#ifdef GPIOG_BASE
    PG0 = 0x60,
    PG1 = 0x61,
    PG2 = 0x62,
    PG3 = 0x63,
    PG4 = 0x64,
    PG5 = 0x65,
    PG6 = 0x66,
    PG7 = 0x67,
    PG8 = 0x68,
    PG9 = 0x69,
    PG10 = 0x6A,
    PG11 = 0x6B,
    PG12 = 0x6C,
    PG13 = 0x6D,
    PG14 = 0x6E,
    PG15 = 0x6F,
#endif

#ifdef GPIOH_BASE
    PH0 = 0x70,
    PH1 = 0x71,
    PH2 = 0x72,
    PH3 = 0x73,
    PH4 = 0x74,
    PH5 = 0x75,
    PH6 = 0x76,
    PH7 = 0x77,
    PH8 = 0x78,
    PH9 = 0x79,
    PH10 = 0x7A,
    PH11 = 0x7B,
    PH12 = 0x7C,
    PH13 = 0x7D,
    PH14 = 0x7E,
    PH15 = 0x7F,
#endif

#ifdef GPIOI_BASE
    PI0 = 0x80,
    PI1 = 0x81,
    PI2 = 0x82,
    PI3 = 0x83,
    PI4 = 0x84,
    PI5 = 0x85,
    PI6 = 0x86,
    PI7 = 0x87,
    PI8 = 0x88,
    PI9 = 0x89,
    PI10 = 0x8A,
    PI11 = 0x8B,
    PI12 = 0x8C,
    PI13 = 0x8D,
    PI14 = 0x8E,
    PI15 = 0x8F,
#endif

#ifdef GPIOJ_BASE
    PJ0 = 0x90,
    PJ1 = 0x91,
    PJ2 = 0x92,
    PJ3 = 0x93,
    PJ4 = 0x94,
    PJ5 = 0x95,
    PJ6 = 0x96,
    PJ7 = 0x97,
    PJ8 = 0x98,
    PJ9 = 0x99,
    PJ10 = 0x9A,
    PJ11 = 0x9B,
    PJ12 = 0x9C,
    PJ13 = 0x9D,
    PJ14 = 0x9E,
    PJ15 = 0x9F,
#endif

#ifdef GPIOK_BASE
    PK0 = 0xA0,
    PK1 = 0xA1,
    PK2 = 0xA2,
    PK3 = 0xA3,
    PK4 = 0xA4,
    PK5 = 0xA5,
    PK6 = 0xA6,
    PK7 = 0xA7,
    PK8 = 0xA8,
    PK9 = 0xA9,
    PK10 = 0xAA,
    PK11 = 0xAB,
    PK12 = 0xAC,
    PK13 = 0xAD,
    PK14 = 0xAE,
    PK15 = 0xAF,
#endif

} PinName;

static GPIO_TypeDef *get_port_type(PinName pin) {
    uint16_t port_idx = STM_PORT(pin);

    switch (port_idx) {
    case 0:
        return GPIOA;
    case 1:
        return GPIOB;

#ifdef GPIOC
    case 2:
        return GPIOC;
#endif
#ifdef GPIOD
    case 3:
        return GPIOD;
#endif
#ifdef GPIOE
    case 4:
        return GPIOE;
#endif
#ifdef GPIOF
    case 5:
        return GPIOF;
#endif
#ifdef GPIOG
    case 6:
        return GPIOG;
#endif
#ifdef GPIOH
    case 7:
        return GPIOH;
#endif
#ifdef GPIOI
    case 8:
        return GPIOI;
#endif
#ifdef GPIOJ
    case 9:
        return GPIOJ;
#endif
#ifdef GPIOK
    case 10:
        return GPIOK;
#endif
    default:
        Error_Handler();
    }
    return (GPIO_TypeDef *)(0); // error
}

} // namespace stmbed
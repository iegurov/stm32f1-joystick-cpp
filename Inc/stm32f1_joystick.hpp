#pragma once

//TODO:
//1. Реализация функции isPressed
//2. Работа с ADC2
//3. Выбор диапазона возможных значений осей джойстика

#ifdef __cplusplus
extern "C" {
#endif

#include "common_macros.h"
#include "stm32f1_gpio.h"
#include "stm32f1_adc.h"
#include <stdint.h>

class Joystick
{
    private:
        Pin pinX, pinY, pinButton;
        int16_t zero;
        uint8_t deadzone;
        uint8_t pressed;

        //Pin getPinX();
        //Pin getPinY();
        int16_t getValue(Pin p);

    public:
        Joystick(){};

        void init(Pin _pinX, Pin _pinY, Pin _pinButton);
        void setZero(int16_t z);
        void setDeadzone(uint8_t dz);

        uint8_t isPressed();
        int16_t getX();
        int16_t getY();
};

#ifdef __cplusplus
} //extern "C"
#endif


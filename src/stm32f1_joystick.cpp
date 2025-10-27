#include "stm32f1_joystick.hpp"

void Joystick::init(Pin _pinX, Pin _pinY, Pin _pinButton)
{
    pinX = _pinX;
    pinY = _pinY;
    pinButton = _pinButton;
    deadzone = 0;
    zero = 0;

    ADC_Setup(ADC1);
}

int16_t Joystick::getValue(Pin p)
{
    int16_t v = ADC_Read(p.num);
    
    v = ADC_Normalize(v, -255, 255);
    if(abs(v)>250) //Мера против дрожания значения на концах диапазона
    {
        v = 255*sign(v);
    }

    if(abs(v)<deadzone) v = 0;

    return v;
}

// Pin Joystick::getPinX(){return pinX;}
// Pin Joystick::getPinY(){return pinY;}

int16_t Joystick::getX() {return Joystick::getValue(pinX);}
int16_t Joystick::getY() {return Joystick::getValue(pinY);}

void Joystick::setZero(int16_t z)
{
    zero = z;
}

void Joystick::setDeadzone(uint8_t dz)
{
    deadzone = dz;
}

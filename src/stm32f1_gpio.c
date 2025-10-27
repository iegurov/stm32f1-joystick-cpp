#include "stm32f1_gpio.h"

void PinMode(Pin p, uint8_t mode)
{
	volatile uint32_t* reg = (p.num<8) ? &p.gpio->CRL : &p.gpio->CRH;
	uint8_t shift = (p.num % 8) * 4;

	*reg &= ~(0xF << shift);
	*reg |= (mode << shift);
}

void PinSet(Pin p) //Важно: пин должен быть сконфигурирован как output GP_PP
{
	p.gpio->ODR |= (1 << p.num);
}

void PinReset(Pin p)
{
	p.gpio->ODR &= ~(1 << p.num);
}

bool DigitalRead(Pin p)
{
    return (p.gpio->IDR & (1 << p.num)) ? true : false;
}
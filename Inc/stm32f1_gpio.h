#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx.h"
#include <stdint.h>
#include <stdbool.h> 

#define ANALOG 		 0x0
#define OUT_PP_50MHz 0x3//0b0011
#define AF_PP_50MHz	 0xB//0b1011
#define IN_PP_50MHz  0x8//0b1000
#define IN_FL 		 0x4//0b0100

typedef struct 
{
    GPIO_TypeDef* gpio;
    uint8_t num;
} Pin;

void PinMode(Pin p, uint8_t mode);
void PinSet(Pin p);
void PinReset(Pin p);
bool DigitalRead(Pin p);

#ifdef __cplusplus
} //extern "C"
#endif
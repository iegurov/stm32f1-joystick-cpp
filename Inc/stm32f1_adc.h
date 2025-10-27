#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx.h"
#include <stdint.h>

void ADC_Setup(ADC_TypeDef* adc);
uint16_t ADC_Read(uint8_t channel);
//float ADC_toVoltage(uint16_t raw);
int16_t ADC_Normalize(int16_t raw, int16_t min, int16_t max);

#ifdef __cplusplus
} //extern "C"
#endif
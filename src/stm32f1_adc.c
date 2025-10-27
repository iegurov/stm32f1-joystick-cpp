#include "stm32f1_adc.h"
#include "stm32f1_gpio.h" //Вопрос в целесообразности зависимости ADC от GPIO

void ADC_Setup(ADC_TypeDef* adc)
{
    if(adc==ADC1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    }
    else
    {
        RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
    }

    adc->CR2 |= ADC_CR2_ADON;
    for (volatile int i = 0; i < 1000; i++) __NOP(); //Задержка перед началом калибровки

    adc->CR2 |= ADC_CR2_CAL;
    while(adc->CR2 & ADC_CR2_CAL) {}; //Начинаем калибровку и ждем ее окончания
}

uint16_t ADC_Read(uint8_t channel)
{
    ADC1->SQR1 = 0;
    ADC1->SQR3 = channel; //Настройка последовательности обработки каналов

    ADC1->CR2 |= ADC_CR2_ADON;

    while (!(ADC1->SR & ADC_SR_EOC)){};

    return ADC1->DR;
}

//Конвертируем полученное с АЦП значение в значение в необходимом диапазоне
//Имеется возможность получать отрицательные числа
inline int16_t ADC_Normalize(int16_t raw, int16_t min, int16_t max)
{
    return (int16_t)(min+(raw*1000/4095)*(max-min)/1000);
}
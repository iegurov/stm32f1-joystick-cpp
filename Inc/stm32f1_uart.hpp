/*  
    TODO:
    1. Перегрузки функций print, println
    2. Работа с DMA
*/

#pragma once

#include "stm32f1xx.h"
#include "stm32f1_gpio.h"

#define TERM_CLEAR "\033[2J\033[H"

//Описание физического USART-модуля (На STM32F103 таких 3)
typedef struct
{
    volatile USART_TypeDef* td; //указатель на регистры модуля
    Pin Tx, Rx;
} USART_MODULE;

//Опишем все USART модули в одном массиве для удобства (60 байт)
const USART_MODULE usart_modules[] = 
{
    {USART1, {GPIOA, 9}, {GPIOA, 10}},
    {USART2, {GPIOA, 2}, {GPIOA, 3}},
    {USART3, {GPIOB, 10},{GPIOB, 11}} 
};

class USART
{
    private:
        const USART_MODULE* phys_module;
        uint32_t baudrate;
        uint8_t disabled;
        uint8_t rx_buf[64]; //omg
        uint8_t get_presc(volatile USART_TypeDef* td);
        void send_char(uint8_t ch);
        void compute_brr(); //baud rate register

    public:
        USART() = default;
        void init(USART_TypeDef* _module, uint32_t _baudrate);
        void enable();
        void disable();
        uint8_t read();
        uint8_t* readln(); //получить указатель на строку, отправленную через терминал
        void print(char ch);
        void print(const uint8_t* data);
        void println(const uint8_t* data);
        void print(const char* data);
        void println(const char* data);
        void print(uint32_t data);
        void print(int32_t data);
        void println(uint32_t data);
        void println(int32_t data);
        void print(int8_t value);
        void println(int8_t value);
        void printhex(uint8_t hex);
        void printlnhex(uint8_t hex);
};




#include "common_macros.h"
#include "stm32f1_joystick.hpp"
#include "stm32f1_uart.hpp"

#define A0  {GPIOA, 0} //X-axis
#define A1  {GPIOA, 1} //Y-axis
#define A2  {GPIOA, 2} //Button

USART uart3;
Joystick joy;

int main()
{
    //Пока что необходимо включить порты для пинов джойстика вручную(
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    //Инициализация джойстика
    joy.init(A0, A1, A2);

    //Установка мертвой зоны по обоим осям
    joy.setDeadzone(15);

    //Подготовка UART
    uart3.init(USART3, 9600);
    uart3.print(TERM_CLEAR);

    volatile int8_t value;

    while(1)
    {
        //Получаем отклонение джойстика по X в диапазоне [-255, 255]
        value = joy.getX();
        uart3.println(value);

        //Получаем отклонение джойстика по Y в диапазоне [-255, 255]
        value = joy.getY();
        uart3.println(value);

        for(int i=0; i<1000; i++) __NOP();
        uart3.print(TERM_CLEAR);
    };
    
    return 0;

}

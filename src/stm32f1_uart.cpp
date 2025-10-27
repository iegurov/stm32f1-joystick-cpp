#include "stm32f1_uart.hpp"

//Функция для получения значения прескейлера шины, на которой находится usart-модуль
uint8_t USART::get_presc(volatile USART_TypeDef* td)
{
    uint8_t raw_APB_presc;

    if (td == USART1) 
    {
        raw_APB_presc = (RCC->CFGR >> 11) & 0x7; //raw presc APB2
    }
    else
    {
        raw_APB_presc = (RCC->CFGR >> 8) & 0x7; //raw presc APB1
    }

    switch (raw_APB_presc)
    {
        case 0x4: return 2; //0b100
        case 0x5: return 4; //0b101
        case 0x6: return 8; //0b110
        case 0x7: return 16;//0b111
        default:  return 1;
    }
}

void USART::init(USART_TypeDef* _module, uint32_t _baudrate)
{
    if(_module == USART1) phys_module = &usart_modules[0];
    else if(_module == USART2) phys_module = &usart_modules[1];
    else if(_module == USART3) phys_module = &usart_modules[2];

    baudrate = _baudrate;
    enable();
}

void USART::compute_brr() 
{
    //SystemCoreClockUpdate();

    uint16_t usart_div;
    uint32_t bus_clock;

    bus_clock = SystemCoreClock/get_presc(phys_module->td);
    usart_div = (uint16_t)(bus_clock/baudrate);
    phys_module->td->BRR = usart_div;
}

void USART::enable()
{

    if(phys_module->td == USART1)
    { 
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    } 
    else if (phys_module->td == USART2)
    { 
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    } 
    else if (phys_module->td == USART3) 
    { 

        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;


    }

    PinMode(phys_module->Tx, AF_PP_50MHz);
    PinMode(phys_module->Rx, IN_FL);

    phys_module->td->CR1 |= USART_CR1_TE;
	phys_module->td->CR1 |= USART_CR1_RE;
    compute_brr(); //Настраиваем BRR
    phys_module->td->CR1 |= USART_CR1_UE;

    disabled = 0;
}

void USART::disable()
{
    phys_module->td->CR1 &= ~(USART_CR1_UE);

    if      (phys_module->td == USART1) { RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN);} 
    else if (phys_module->td == USART2) { RCC->APB1ENR &= ~(RCC_APB1ENR_USART2EN);} 
    else if (phys_module->td == USART3) { RCC->APB1ENR &= ~(RCC_APB1ENR_USART3EN);}

    disabled = 1;
}

/* ---------- ЧТЕНИЕ СИМВОЛОВ И СТРОК ---------- */

uint8_t USART::read()
{
    if(disabled) return 0;
    while((phys_module->td->SR & USART_SR_RXNE) == 0) {};
	return phys_module->td->DR;
}

//Чтение до 64 символов (без echo введенного символа)
uint8_t* USART::readln()
{
    if(disabled) return 0;
    for(uint8_t i = 0; i<sizeof(rx_buf); i++)
    {
        rx_buf[i] = read();
        if(rx_buf[i] == '\r' || rx_buf[i] == '\n')
        {
            rx_buf[i] = '\0';
            break;
        }
    }

    return rx_buf;
}

/* ---------- ОТПРАВКА СИМВОЛОВ И СТРОК ---------- */

void USART::send_char(uint8_t ch)
{
    if(disabled) return;
    while((phys_module->td->SR & USART_SR_TXE) == 0) {};
	phys_module->td->DR = ch;
}

void USART::print(const char* data)
{
    if(disabled) return;
    while(*data)
	{
		send_char(*data);
		data++;
	}
}

void USART::print(char ch)
{
    if(disabled) return;
    send_char(ch);
}

void USART::println(const char* data)
{
    if(disabled) return;
	print(data);
	print("\r\n");
}


void USART::print(const uint8_t* data){USART::print((const char*)data);}
void USART::println(const uint8_t* data){USART::print((const char*)data);}

/* ---------- ОТПРАВКА ЧИСЕЛ В ДЕСЯТИЧНОМ ФОРМАТЕ ---------- */

void USART::print(uint32_t value)
{
    uint8_t buf[10];
    uint8_t amount;

    for (uint8_t i = 0; i < 10; i++) 
	{
        buf[i] = value % 10; //В результате получаем инвертированную последовательность цифр
        value /= 10;

        if (value == 0 || i == 9) 
		{
            amount = i;
			buf[9] = '\0';
            break;
        }
    }
    for (int8_t i = amount; i >= 0; i--) 
	{
        print((char)(buf[i] + '0')); //Отправляем байты в правильной последовательности
    }
}

void USART::print(int32_t value)
{
    if(value<0)
	{
		value = -value;
		print('-');
	}
    print((uint32_t)value);
}

void USART::println(uint32_t value)
{
    if(disabled) return;
    print(value);
	print("\r\n");
}

void USART::println(int32_t value)
{
    if(disabled) return;
    print(value);
	print("\r\n");
}

void USART::print(int8_t value) {print((int32_t)value);}
void USART::println(int8_t value) {println((int32_t)value);}

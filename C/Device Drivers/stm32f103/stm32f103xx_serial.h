#ifndef INC_STM32F103XX_SERIAL_H
#define INC_STM32F103XX_SERIAL_H
#include"stm32f103xx_gpio.h"
#include"stm32f103xx_usart.h"
#include"stm32f103xx.h"
#include"stm32f103xx_utilities.h"

#include<stdarg.h>

#define BUFF_SIZE 1024

void _print_buffer(char* buffer, uint32_t* buff_ind);
void _print_int(uint32_t value, char* buffer, uint32_t* buff_ind);
void _reset_buffer(uint32_t* buff_ind);
void _serial_init();
void Serial_init();
uint8_t Serialprint(const char *format, ...);


#endif
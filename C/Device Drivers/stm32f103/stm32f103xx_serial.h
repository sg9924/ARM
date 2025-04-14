#ifndef INC_STM32F103XX_SERIAL_H
#define INC_STM32F103XX_SERIAL_H
#include"stm32f103xx_gpio.h"
#include"stm32f103xx_usart.h"
#include"stm32f103xx.h"
#include"stm32f103xx_utilities.h"

#include<stdarg.h>

#define BUFF_SIZE 1024

// Serial Input Data Type Formats
#define SERIAL_RX_STRING          0
#define SERIAL_RX_INT             1

void _print_buffer(char* buffer, uint32_t* buff_ind);
void _print_int(uint32_t value, char* buffer, uint32_t* buff_ind);
void _reset_buffer(uint32_t* buff_ind);
void _serial_init();
void Serial_init();
uint8_t Serialprint(const char *format, ...);
uint8_t Serialprintln(char *format, ...);
void SerialInput(char* msg, uint8_t datatype, char* d);


#endif
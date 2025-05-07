#ifndef INC_STM32F103XX_UTILITIES_H
#define INC_STM32F103XX_UTILITIES_H

#include<stdint.h>

void wait_ms(uint16_t d);
void wait(uint8_t d);
uint32_t get_size(char* data);
void string_concat(char* s1, char* s2);
int32_t string_to_int(char* str);

#endif /*INC_STM32F103XX_UTILITIES_H*/
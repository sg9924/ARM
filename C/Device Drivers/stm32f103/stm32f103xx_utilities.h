#ifndef INC_STM32F103XX_UTILITIES_H
#define INC_STM32F103XX_UTILITIES_H

#include<stdint.h>

void wait_ms(uint16_t d);
void wait(uint8_t d);
uint32_t get_size(char* data);

#endif /*INC_STM32F103XX_UTILITIES_H*/
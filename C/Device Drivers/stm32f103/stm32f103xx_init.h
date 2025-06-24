#include "stm32f103xx.h"
#include "stm32f103xx_timer.h"
#include "stm32f103xx_rcc.h"
#include "stm32f103xx_serial.h"


void board_init();
void tim_delay_ms(uint16_t delay);
void tim_delay_us(uint16_t delay);
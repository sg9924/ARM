#ifndef SEVENSEG_H
#define SEVENSEG_H

#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"
#include"seven_seg_5161AS.h"
#include"gpio_7_seg_map.h"

//Functions
void seven_seg_init(GPIO_Handle* pGPIOh);
void seven_seg_display(GPIO_Handle* pGPIOh, uint8_t character);


#endif /*SEVENSEG_H*/
#ifndef  KPD_4X4_H
#define  KPD_4X4_H

#include"stm32F407xx_gpio_driver.h"



//function declarations
void keypad_initialize(void);
char row1_scan(void);
char row2_scan(void);
char row3_scan(void);
char row4_scan(void);


#endif /*KDP_4X4_H*/
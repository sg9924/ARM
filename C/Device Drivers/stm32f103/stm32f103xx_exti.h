#ifndef INC_stm32F103xx_EXTI_H
#define INC_stm32F103xx_EXTI_H


#include"stm32F103xx.h"
#include "stm32F103xx_gpio.h"
/************************************************** EXTI Definitions Start **************************************************/

// EXTI Peripheral
#define EXTI               ((EXTI_RegDef*)EXTI_BASE_ADDR)

/*************************************************** EXTI Definitions End ***************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************ EXTI API's Declarations Start *********************************************/

void exti_intrpt_trig_config(uint8_t, uint8_t);
void exti_enable_intrpt(uint8_t);
void exti_enable_event(uint8_t);

/************************************************** EXTI API's Declarations End *********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
#endif /* INC_stm32F103xx_EXTI_H */
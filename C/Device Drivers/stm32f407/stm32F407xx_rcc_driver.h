#ifndef INC_STM32F407XX_RCC_DRIVER_H_
#define INC_STM32F407XX_RCC_DRIVER_H_

#include "stm32f407xx.h"
/*--------------------------------------------------------------------------------------------------------------------------*/
/********************************************* RCC Function Declarations Start **********************************************/
//This returns the APB1 clock value
uint32_t RCC_GetPCLK1Value(void);

//This returns the APB2 clock value
uint32_t RCC_GetPCLK2Value(void);

#endif /* INC_STM32F407XX_RCC_DRIVER_H_ */
/********************************************** RCC Function Declarations End ***********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
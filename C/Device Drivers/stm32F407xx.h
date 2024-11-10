#ifndef INC_STM3F407XX_H
#define INC_STM3F407XX_H
/*--------------------------------------------------------------------------------------------------------------------------*/
// standard header files
#include<stddef.h>
#include<stdint.h>
/*--------------------------------------------------------------------------------------------------------------------------*/
/********************************************* Generic Macros Definitions Start *********************************************/

#define ENABLE 				1
#define DISABLE 			0

#define SET 				ENABLE
#define RESET 				DISABLE
#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET
#define FLAG_RESET          RESET
#define FLAG_SET 			SET

/********************************************** Generic Macros Definitions End **********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************** NVIC Definitions Start **************************************************/

// NVIC ISERx Register Addresses
#define NVIC_ISER0          ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1          ((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2          ((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3          ((volatile uint32_t*)0xE000E10c)

// NVIC ICERx Register Addresses
#define NVIC_ICER0 			((volatile uint32_t*)0XE000E180)
#define NVIC_ICER1			((volatile uint32_t*)0XE000E184)
#define NVIC_ICER2  		((volatile uint32_t*)0XE000E188)
#define NVIC_ICER3			((volatile uint32_t*)0XE000E18C)

// NVIC Prority Register Address
#define NVIC_PR_BASE_ADDR 	((volatile uint32_t*)0xE000E400)

// No. of Priority Bits Implemented
#define NO_PR_BITS_IMPLEMENTED  4
/*************************************************** NVIC Definitions End ***************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/

#endif /* INC_STM3F407XX_H */
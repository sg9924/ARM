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
/************************************************** IRQ Definitions Start ***************************************************/

// IRQ Numbers
#define IRQ_NO_EXTI0 		    6
#define IRQ_NO_EXTI1 		    7
#define IRQ_NO_EXTI2 		    8
#define IRQ_NO_EXTI3 		    9
#define IRQ_NO_EXTI4 		    10
#define IRQ_NO_EXTI9_5 		    23
#define IRQ_NO_EXTI15_10 	    40
#define IRQ_NO_SPI1			    35
#define IRQ_NO_SPI2             36
#define IRQ_NO_SPI3             51
#define IRQ_NO_SPI4
#define IRQ_NO_I2C1_EV          31
#define IRQ_NO_I2C1_ER          32
#define IRQ_NO_USART1	        37
#define IRQ_NO_USART2	        38
#define IRQ_NO_USART3	        39
#define IRQ_NO_UART4	        52
#define IRQ_NO_UART5	        53
#define IRQ_NO_USART6	        71


// IRQ Priority Levels
#define NVIC_IRQ_PRI0           0
#define NVIC_IRQ_PRI15          15
/**************************************************** IRQ Definitions End ***************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/









#endif /* INC_STM3F407XX_H */
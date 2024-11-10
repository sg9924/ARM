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
/********************************************* Memory Address Definitions Start *********************************************/
// Memory Base Addresses
#define FLASH_BASE_ADDR     0x08000000U
#define SRAM1_BASE_ADDR     0x20000000U
#define SRAM2_BASE_ADDR     0x2001C000U
#define SRAM_BASE_ADDR      SRAM1_BASE_ADDR


// Peripheral Base Address
#define PERIP_BASE_ADDR     0x40000000U

// Bus Domain Bases Addresses
#define APB1_BASE_ADDR      PERIP_BASE_ADDR
#define APB2_BASE_ADDR      0x40010000U
#define AHB1_BASE_ADDR      0x40020000U
#define AHB2_BASE_ADDR      0x50000000U


// AHB1 Peripherals Base Addresses
// GPIO Base Addresses
#define GPIOA_BASE_ADDR     (AHB1_BASE_ADDR) 
#define GPIOB_BASE_ADDR     (AHB1_BASE_ADDR + 0x0400)
#define GPIOC_BASE_ADDR     (AHB1_BASE_ADDR + 0x0800)
#define GPIOD_BASE_ADDR     (AHB1_BASE_ADDR + 0x0C00)
#define GPIOE_BASE_ADDR     (AHB1_BASE_ADDR + 0x1000)
#define GPIOF_BASE_ADDR     (AHB1_BASE_ADDR + 0x1400)
#define GPIOG_BASE_ADDR     (AHB1_BASE_ADDR + 0x1800)
#define GPIOH_BASE_ADDR     (AHB1_BASE_ADDR + 0x1C00)
#define GPIOI_BASE_ADDR     (AHB1_BASE_ADDR + 0x2000)
#define GPIOJ_BASE_ADDR     (AHB1_BASE_ADDR + 0x2400)
#define GPIOK_BASE_ADDR     (AHB1_BASE_ADDR + 0x2800)

// CRC
#define CRC_BASE_ADDR       (AHB1_BASE_ADDR + 0x3000)

// RCC - Reset & Clock Control Base Address
#define RCC_BASE_ADDR       (AHB1_BASE_ADDR + 0x3800)


// APB1 Peripherals Base Addresses
// SPI
#define SPI2_BASE_ADDR      (APB1_BASE_ADDR + 0x3800)
#define SPI3_BASE_ADDR      (APB1_BASE_ADDR + 0x3FFF)

// USART
#define USART2_BASE_ADDR    (APB1_BASE_ADDR + 0x4400)
#define USART3_BASE_ADDR    (APB1_BASE_ADDR + 0x4800)

// UART
#define UART4_BASE_ADDR     (APB1_BASE_ADDR + 0x4C00)
#define UART5_BASE_ADDR     (APB1_BASE_ADDR + 0x5000)
#define UART7_BASE_ADDR     (APB1_BASE_ADDR + 0x7800)
#define UART8_BASE_ADDR     (APB1_BASE_ADDR + 0x7C00)

//I2C
#define I2C1_BASE_ADDR      (APB1_BASE_ADDR + 0x5400)
#define I2C2_BASE_ADDR      (APB1_BASE_ADDR + 0x5800)
#define I2C3_BASE_ADDR      (APB1_BASE_ADDR + 0x5C00)


// APB2 Peripherals Base Addresses
// USART
#define USART1_BASE_ADDR    (APB2_BASE_ADDR + 0x1000)
#define USART6_BASE_ADDR    (APB2_BASE_ADDR + 0x1400)

// SPI
#define SPI1_BASE_ADDR      (APB2_BASE_ADDR + 0x3000)
#define SPI4_BASE_ADDR      (APB2_BASE_ADDR + 0x3400)
#define SPI5_BASE_ADDR      (APB2_BASE_ADDR + 0x5000)
#define SPI6_BASE_ADDR      (APB2_BASE_ADDR + 0x5400)

//SYSCFG
#define SYSCFG_BASE_ADDR      (APB2_BASE_ADDR + 0x3800)

//EXTI
#define EXTI_BASE_ADDR      (APB2_BASE_ADDR + 0x3C00)

/********************************************** Memory Address Definitions End **********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/








#endif /* INC_STM3F407XX_H */
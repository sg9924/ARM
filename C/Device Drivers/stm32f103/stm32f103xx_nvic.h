#ifndef INC_stm32F103xx_NVIC_H
#define INC_stm32F103xx_NVIC_H

#include<stdint.h>
#include"stm32F103xx.h"
#include"stm32F103xx_gpio.h"


/************************************************** NVIC Definitions Start **************************************************/
// NVIC ISERx Register Addresses
#define NVIC_ISER0            ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1            ((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2            ((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3            ((volatile uint32_t*)0xE000E10C)
#define NVIC_ISER4            ((volatile uint32_t*)0xE000E110)
#define NVIC_ISER5            ((volatile uint32_t*)0xE000E114)
#define NVIC_ISER6            ((volatile uint32_t*)0xE000E118)
#define NVIC_ISER7            ((volatile uint32_t*)0xE000E11C)

// NVIC ICERx Register Addresses
#define NVIC_ICER0            ((volatile uint32_t*)0XE000E180)
#define NVIC_ICER1            ((volatile uint32_t*)0XE000E184)
#define NVIC_ICER2            ((volatile uint32_t*)0XE000E188)
#define NVIC_ICER3            ((volatile uint32_t*)0XE000E18C)
#define NVIC_ICER4            ((volatile uint32_t*)0XE000E190)
#define NVIC_ICER5            ((volatile uint32_t*)0XE000E194)
#define NVIC_ICER6            ((volatile uint32_t*)0XE000E198)
#define NVIC_ICER7            ((volatile uint32_t*)0XE000E19C)

// NVIC Priority Register Address
#define NVIC_PR_BASE_ADDR   ((volatile uint32_t*)0xE000E400)
/*************************************************** NVIC Definitions End ***************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************ NVIC API's Declarations Start *********************************************/

uint8_t nvic_irq_gpio_port_map(GPIO_Handle*);
void nvic_intrpt_enable(uint8_t);
void nvic_intrpt_disable(uint8_t);

/************************************************** NVIC API's Declarations End *********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/

#endif /*INC_stm32F103xx_NVIC_H*/
#ifndef INC_STM32F103XX_H
#define INC_STM32F103XX_H
/*--------------------------------------------------------------------------------------------------------------------------*/
// standard header files
#include<stdint.h>

//custom header files
#include"stm32f103xx_memory_map.h"
/*--------------------------------------------------------------------------------------------------------------------------*/
/********************************************* Generic Macros Definitions Start *********************************************/

#define ENABLE                 1
#define DISABLE             0

#define SET                 ENABLE
#define RESET                 DISABLE
#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET
#define FLAG_RESET          RESET
#define FLAG_SET             SET

// Bit Manipulations !!Currently not used!!
#define READ_BIT(reg,bit_field)        (reg>>bit_field)
#define SET_BIT(reg,bit_field)         (reg |= 1<<bit_field)
#define RESET_BIT(reg,bit_field)       (reg &= ~(1<<bit_field))

/********************************************** Generic Macros Definitions End **********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************* Peripheral Registers Definition Structures Start *************************************/

// GPIO Register Definition Structure
typedef struct
{
    volatile uint32_t CR[2];        /*<Address Offset: 0x00> <Configuration: Mode & Config Type>*/
    volatile uint32_t IDR;          /*<Address Offset: 0x08> <Input Data>*/
    volatile uint32_t ODR;          /*<Address Offset: 0x0C> <Output Data>*/
    volatile uint32_t BSRR;         /*<Address Offset: 0x10> <Bit Set Reset (atomic operation)>*/
    volatile uint32_t BRR;          /*<Address Offset: 0x14> <Bit Reset>*/
    volatile uint32_t LCKR;         /*<Address Offset: 0x18> <Lock>*/
}GPIO_RegDef;

// AFIO Register Definition Structure
typedef struct
{
    volatile uint32_t EVCR;         /*<Address Offset: 0x00> <Event Control*/
    volatile uint32_t MAPR;         /*<Address Offset: 0x04> <Remap>*/
    volatile uint32_t EXTICR[4];    /*<Address Offset: 0x08> <Output Data>*/
    volatile uint32_t MAPR2;        /*<Address Offset: 0x18> <Remap>*/
}AFIO_RegDef;

// RCC Register Definition Structure
typedef struct
{
    volatile uint32_t CR;           /*<Address offset: 0x00> <Control>*/
    volatile uint32_t CFGR;         /*<Address offset: 0x04> <Configuration>*/
    volatile uint32_t CIR;          /*<Address offset: 0x08> <Clock Interrupt>*/
    volatile uint32_t APB2RSTR;     /*<Address offset: 0x0C> <APB2 Reset>*/
    volatile uint32_t APB1RSTR;     /*<Address offset: 0x10> <APB1 Reset>*/
    volatile uint32_t AHBENR;       /*<Address offset: 0x14> <AHB Enable>*/
    volatile uint32_t APB2ENR;      /*<Address offset: 0x18> <APB2 Enable>*/
    volatile uint32_t APB1ENR;      /*<Address offset: 0x1C> <APB1 Enable>*/
    volatile uint32_t BDCR;         /*<Address offset: 0x20> <Back Domain Clock>*/
    volatile uint32_t CSR;          /*<Address offset: 0x24> <Control/Status>*/
}RCC_RegDef;

// EXTI Register Definition Structure
typedef struct
{
    volatile uint32_t IMR;          /*<Address Offset: 0x00> <Interrupt Mask>*/
    volatile uint32_t EMR;          /*<Address Offset: 0x04> <Event Mask>*/
    volatile uint32_t RTSR;         /*<Address Offset: 0x08> <Rising Trigger Selection>*/
    volatile uint32_t FTSR;         /*<Address Offset: 0x0C> <Falling Trigger Selection>*/
    volatile uint32_t SWIER;        /*<Address Offset: 0x0C> <Software Interrupt Event>*/
    volatile uint32_t PR;           /*<Address Offset: 0x0C> <Pending>*/
}EXTI_RegDef;

/*************************************** Peripheral Registers Definition Structures End ***************************************/
/*----------------------------------------------------------------------------------------------------------------------------*/

#endif /* INC_STM32F103XX_H */
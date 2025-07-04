#ifndef INC_STM32F103XX_H
#define INC_STM32F103XX_H
/*--------------------------------------------------------------------------------------------------------------------------*/
// standard header files
#include<stdint.h>

//custom header files
#include"stm32f103xx_memory_map.h"
/*--------------------------------------------------------------------------------------------------------------------------*/
/********************************************* Generic Macros Definitions Start *********************************************/
#define SYSCORE_CLK          8000000

#define ENABLE              1
#define DISABLE             0

#define SET                 ENABLE
#define RESET               DISABLE
#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET
#define FLAG_RESET          RESET
#define FLAG_SET            SET

// Bit Manipulations !!Currently not used!!
#define READ_BIT(reg,bit_field)              (reg>>bit_field)
#define SET_BIT(reg,bit_field)               (reg |= 1<<bit_field)
#define RESET_BIT(reg,bit_field)             (reg &= ~(1<<bit_field))

/********************************************** Generic Macros Definitions End **********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************* Peripheral Registers Definition Structures Start *************************************/

//CRC Register Definition Structure
typedef struct 
{
    uint32_t DR;
    uint32_t IDR;
    uint32_t CR;
}CRC_RegDef;

//Systick Register Definition Structure
typedef struct
{
    uint32_t CTRL;
    uint32_t LOAD;
    uint32_t VAL;
    uint32_t CALIB;
}SysTick_RegDef;

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
    volatile uint32_t BDCR;         /*<Address offset: 0x20> <Backup Domain Clock>*/
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

// USART Register Definition Structure
typedef struct
{
    volatile uint32_t SR;          /*<Address Offset: 0x00> <>*/
    volatile uint32_t DR;          /*<Address Offset: 0x04> <>*/
    volatile uint32_t BRR;         /*<Address Offset: 0x08> <>*/
    volatile uint32_t CR1;         /*<Address Offset: 0x0C> <>*/
    volatile uint32_t CR2;         /*<Address Offset: 0x10> <>*/
    volatile uint32_t CR3;         /*<Address Offset: 0x14> <>*/
    volatile uint32_t GTPR;        /*<Address Offset: 0x18> <>*/
}USART_RegDef;


//SPI
typedef struct
{
    uint32_t CR1;
    uint32_t CR2;
    uint32_t SR;
    uint32_t DR;
    uint32_t CRCPR;
    uint32_t RXCRCR;
    uint32_t TXCRCR;
    uint32_t I2SCFGR;
    uint32_t I2SPR;
}SPI_RegDef;


//ADC
typedef struct
{
    uint32_t SR;
    uint32_t CR1;
    uint32_t CR2;
    uint32_t SMPR1;
    uint32_t SMPR2;
    uint32_t JOFR[4];
    uint32_t HTR;
    uint32_t LTR;
    uint32_t SQR1;
    uint32_t SQR2;
    uint32_t SQR3;
    uint32_t JSQR;
    uint32_t JDR[4];
    uint32_t DR;
}ADC_RegDef;



//TIMER
typedef struct
{
    uint32_t CR1;
    uint32_t CR2;
    uint32_t SMCR;
    uint32_t DIER;
    uint32_t SR;
    uint32_t EGR;
    uint32_t CCMR[2];
    uint32_t CCER;
    uint32_t CNT;
    uint32_t PSC;
    uint32_t ARR;
    uint32_t CCR[4];
    uint32_t DCR;
    uint32_t DMAR;

}TIM_RegDef;

//I2C
typedef struct
{
    uint32_t CR1;
    uint32_t CR2;
    uint32_t OAR1;
    uint32_t OAR2;
    uint32_t DR;
    uint32_t SR1;
    uint32_t SR2;
    uint32_t CCR;
    uint32_t TRISE;
}I2C_RegDef;

//DMA
typedef struct
{
    uint32_t CCR;
    uint32_t CNDTR;
    uint32_t CPAR;
    uint32_t CMAR;
}DMA_Channel_RegDef;

typedef struct
{
    uint32_t ISR;
    uint32_t IFCR;
    DMA_Channel_RegDef Channel[7];
}DMA_RegDef;

/*************************************** Peripheral Registers Definition Structures End ***************************************/
/*----------------------------------------------------------------------------------------------------------------------------*/

#endif
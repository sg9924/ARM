#ifndef INC_stm32F103xx_ADC_H
#define INC_stm32F103xx_ADC_H

#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"
#include"stm32f103xx_nvic.h"
#include"stm32f103xx_rcc.h"



//ADC Config Structure
typedef struct
{
    uint8_t   mode;
    uint8_t   external_event;
    uint8_t   dma_mode;
    uint8_t   channel_no;
    uint8_t   resolution;
    uint16_t* pbuffer;
}ADC_Config;



//ADC Handle Structure
typedef struct
{
    ADC_RegDef*  pADCx;
    GPIO_Handle* pGPIOHandle;
    ADC_Config   ADCx_Config;
}ADC_Handle;




//ADC Definitions
#define ADC1                   ((ADC_RegDef*)ADC1_BASE_ADDR)
#define ADC2                   ((ADC_RegDef*)ADC2_BASE_ADDR)
#define ADC3                   ((ADC_RegDef*)ADC3_BASE_ADDR)


//ADC_SR Bit Fields Definitions
#define ADC_SR_AWD                  0
#define ADC_SR_EOC                  1
#define ADC_SR_JEOC                 2
#define ADC_SR_JSTRT                3
#define ADC_SR_STRT                 4


//ADC_CR1 Bit Fields Definition
#define ADC_CR1_AWDCH0              0
#define ADC_CR1_AWDCH1              1
#define ADC_CR1_AWDCH2              2
#define ADC_CR1_AWDCH3              3
#define ADC_CR1_AWDCH4              4
#define ADC_CR1_EOCIE               5
#define ADC_CR1_AWDIE               6
#define ADC_CR1_JEOCIE              7
#define ADC_CR1_SCAN                8
#define ADC_CR1_AWDSGL              9
#define ADC_CR1_JAUTO               10
#define ADC_CR1_DISCEN              11
#define ADC_CR1_JDISCEN             12
#define ADC_CR1_DISCNUM0            13
#define ADC_CR1_DISCNUM1            14
#define ADC_CR1_DISCNUM2            15
#define ADC_CR1_DUALMOD0            16
#define ADC_CR1_DUALMOD1            17
#define ADC_CR1_DUALMOD2            18
#define ADC_CR1_DUALMOD3            19
#define ADC_CR1_JAWDEN              22
#define ADC_CR1_AWDEN               23


//ADC_CR2 Bit Fields Definition
#define ADC_CR2_ADON                0
#define ADC_CR2_CONT                1
#define ADC_CR2_CAL                 2
#define ADC_CR2_RSTCAL              3
#define ADC_CR2_DMA                 8
#define ADC_CR2_ALIGN               11
#define ADC_CR2_JEXTSEL0            12
#define ADC_CR2_JEXTSEL1            13
#define ADC_CR2_JEXTSEL2            14
#define ADC_CR2_JEXTTRIG            15
#define ADC_CR2_EXTSEL0             17
#define ADC_CR2_EXTSEL1             18
#define ADC_CR2_EXTSEL2             19
#define ADC_CR2_EXTTRIG             20
#define ADC_CR2_JSWSTART            21
#define ADC_CR2_SWSTART             22
#define ADC_CR2_TSVREFE             23


//ADC_SMPR1 Bit Field Definitions
#define ADC_SMPR1_SMP10             0
#define ADC_SMPR1_SMP11             3
#define ADC_SMPR1_SMP12             6
#define ADC_SMPR1_SMP13             9
#define ADC_SMPR1_SMP14             12
#define ADC_SMPR1_SMP15             15
#define ADC_SMPR1_SMP16             18
#define ADC_SMPR1_SMP17             21 


//ADC_SMPR2 Bit Field Definitions
#define ADC_SMPR2_SMP0              0
#define ADC_SMPR2_SMP1              3
#define ADC_SMPR2_SMP2              6
#define ADC_SMPR2_SMP3              9
#define ADC_SMPR2_SMP4              12
#define ADC_SMPR2_SMP5              15
#define ADC_SMPR2_SMP6              18
#define ADC_SMPR2_SMP7              21
#define ADC_SMPR2_SMP8              24
#define ADC_SMPR2_SMP9              27


//ADC_DR
#define ADC_DR_DATA                 0
#define ADC_DR_ADC2DATA             16

//ADC DMA
#define ADC_DMA_ENABLE              1
#define ADC_DMA_DISABLE             0


//ADC Conversion Modes
#define ADC_MODE_SINGLE             0
#define ADC_MODE_CONT               1
#define ADC_MODE_SCAN               2


//ADC Trigger
#define ADC_EVENT_SWSTART           7


//ADC Peripheral Clock Enable
#define ADC1_PCLK_EN()              RCC->APB2ENR |= 1<< RCC_APB2ENR_ADC1EN
#define ADC2_PCLK_EN()              RCC->APB2ENR |= 1<< RCC_APB2ENR_ADC2EN
#define ADC3_PCLK_EN()              RCC->APB2ENR |= 1<< RCC_APB2ENR_ADC3EN


//ADC Peripheral Clock Disable
#define ADC1_PCLK_DISABLE()         RCC->APB2ENR &= ~(1<< RCC_APB2ENR_ADC1EN)
#define ADC2_PCLK_DISABLE()         RCC->APB2ENR &= ~(1<< RCC_APB2ENR_ADC2EN)
#define ADC3_PCLK_DISABLE()         RCC->APB2ENR &= ~(1<< RCC_APB2ENR_ADC3EN)


//ADC Peripheral Enable
#define ADC1_ENABLE()               (ADC1->CR2 |= 1 << ADC_CR2_ADON)
#define ADC2_ENABLE()               (ADC2->CR2 |= 1 << ADC_CR2_ADON)
#define ADC3_ENABLE()               (ADC3->CR2 |= 1 << ADC_CR2_ADON)


//ADC Peripheral Disable
#define ADC1_DISABLE()              (ADC1->CR2 &= ~(1 << ADC_CR2_ADON))
#define ADC2_DISABLE()              (ADC2->CR2 &= ~(1 << ADC_CR2_ADON))
#define ADC3_DISABLE()              (ADC3->CR2 &= ~(1 << ADC_CR2_ADON))








//Function Declarations
void ADC_PClk_init(ADC_Handle* pADCHandle, uint8_t mode);
void ADC_Configure(ADC_Handle* pADCHandle, ADC_RegDef* ADCx, uint8_t mode, uint8_t event, uint8_t dma_mode);
void ADC_init(ADC_Handle* pADCHandle, GPIO_Handle* pGPIOHandle, GPIO_RegDef* GPIOx);
uint16_t ADC_Start(ADC_Handle* pADCHandle);
void ADC_DMA_Start();



#endif
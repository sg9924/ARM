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
/*********************************** Peripheral Registers Bit Positions Definitions Start ***********************************/

// SPI Bit Positions
// SPI_CR1
#define SPI_CR1_CPHA     				 0
#define SPI_CR1_CPOL      				 1
#define SPI_CR1_MSTR     				 2
#define SPI_CR1_BR   					 3
#define SPI_CR1_SPE     				 6
#define SPI_CR1_LSBFIRST   			 	 7
#define SPI_CR1_SSI     				 8
#define SPI_CR1_SSM      				 9
#define SPI_CR1_RXONLY      		 	10
#define SPI_CR1_DFF     			 	11
#define SPI_CR1_CRCNEXT   			 	12
#define SPI_CR1_CRCEN   			 	13
#define SPI_CR1_BIDIOE     			 	14
#define SPI_CR1_BIDIMODE      			15

// SPI_CR2
#define SPI_CR2_RXDMAEN		 			0
#define SPI_CR2_TXDMAEN				 	1
#define SPI_CR2_SSOE				 	2
#define SPI_CR2_FRF						4
#define SPI_CR2_ERRIE					5
#define SPI_CR2_RXNEIE				 	6
#define SPI_CR2_TXEIE					7

// SPI_SR
#define SPI_SR_RXNE						0
#define SPI_SR_TXE				 		1
#define SPI_SR_CHSIDE				 	2
#define SPI_SR_UDR					 	3
#define SPI_SR_CRCERR				 	4
#define SPI_SR_MODF					 	5
#define SPI_SR_OVR					 	6
#define SPI_SR_BSY					 	7
#define SPI_SR_FRE					 	8


// I2C Bit Positions
// I2C_CR1
#define I2C_CR1_PE						0
#define I2C_CR1_NOSTRETCH  				7
#define I2C_CR1_START 					8
#define I2C_CR1_STOP  				 	9
#define I2C_CR1_ACK 				 	10
#define I2C_CR1_SWRST  				 	15

// I2C_CR2
#define I2C_CR2_FREQ				 	0
#define I2C_CR2_ITERREN				 	8
#define I2C_CR2_ITEVTEN				 	9
#define I2C_CR2_ITBUFEN 			    10

// I2C_OAR1
#define I2C_OAR1_ADD0    				 0
#define I2C_OAR1_ADD71 				 	 1
#define I2C_OAR1_ADD98  			 	 8
#define I2C_OAR1_ADDMODE   			 	15

// I2C_SR1
#define I2C_SR1_SB 					 	0
#define I2C_SR1_ADDR 				 	1
#define I2C_SR1_BTF 					2
#define I2C_SR1_ADD10 					3
#define I2C_SR1_STOPF 					4
#define I2C_SR1_RXNE 					6
#define I2C_SR1_TXE 					7
#define I2C_SR1_BERR 					8
#define I2C_SR1_ARLO 					9
#define I2C_SR1_AF 					 	10
#define I2C_SR1_OVR 					11
#define I2C_SR1_TIMEOUT 				14

// I2C_SR2
#define I2C_SR2_MSL						0
#define I2C_SR2_BUSY 					1
#define I2C_SR2_TRA 					2
#define I2C_SR2_GENCALL 				4
#define I2C_SR2_DUALF 					7

// I2C_CCR
#define I2C_CCR_CCR 					 0
#define I2C_CCR_DUTY 					14
#define I2C_CCR_FS  				 	15


// USART Bit Positions
// USART_CR1
#define USART_CR1_SBK					0
#define USART_CR1_RWU 					1
#define USART_CR1_RE  					2
#define USART_CR1_TE 					3
#define USART_CR1_IDLEIE 				4
#define USART_CR1_RXNEIE  				5
#define USART_CR1_TCIE					6
#define USART_CR1_TXEIE					7
#define USART_CR1_PEIE 					8
#define USART_CR1_PS 					9
#define USART_CR1_PCE 					10
#define USART_CR1_WAKE  				11
#define USART_CR1_M 					12
#define USART_CR1_UE 					13
#define USART_CR1_OVER8  				15

// USART_CR2
#define USART_CR2_ADD   				0
#define USART_CR2_LBDL   				5
#define USART_CR2_LBDIE  				6
#define USART_CR2_LBCL   				8
#define USART_CR2_CPHA   				9
#define USART_CR2_CPOL   				10
#define USART_CR2_STOP   				12
#define USART_CR2_LINEN   				14

// USART_CR3
#define USART_CR3_EIE   				0
#define USART_CR3_IREN   				1
#define USART_CR3_IRLP  				2
#define USART_CR3_HDSEL   				3
#define USART_CR3_NACK   				4
#define USART_CR3_SCEN   				5
#define USART_CR3_DMAR  				6
#define USART_CR3_DMAT   				7
#define USART_CR3_RTSE   				8
#define USART_CR3_CTSE   				9
#define USART_CR3_CTSIE   				10
#define USART_CR3_ONEBIT   				11

// USART_SR
#define USART_SR_PE        				0
#define USART_SR_FE        				1
#define USART_SR_NE        				2
#define USART_SR_ORE       				3
#define USART_SR_IDLE       			4
#define USART_SR_RXNE        			5
#define USART_SR_TC        				6
#define USART_SR_TXE        			7
#define USART_SR_LBD        			8
#define USART_SR_CTS        			9
/************************************ Peripheral Registers Bit Positions Definitions End ************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************* Peripheral Registers Definition Structures Start *************************************/

// GPIO Register Definition Structure
typedef struct
{
    volatile uint32_t MODER;        /*<><Address Offset: 0x00>*/
    volatile uint32_t OTYPER;       /*<><Address Offset: 0x00>*/
    volatile uint32_t OSPEEDR;      /*<><Address Offset: 0x00>*/
    volatile uint32_t PUPDR;        /*<><Address Offset: 0x00>*/
    volatile uint32_t IDR;          /*<><Address Offset: 0x00>*/
    volatile uint32_t ODR;          /*<><Address Offset: 0x00>*/
    volatile uint32_t BSRR;         /*<><Address Offset: 0x00>*/
    volatile uint32_t LCKR;         /*<><Address Offset: 0x00>*/
    volatile uint32_t AFR[2];       /*<Alternate Function -> Low Register: AFRL , High Register: AFRH><Address Offset: 0x00>*/
}GPIO_RegDef;


// RCC Register Definition Structure
typedef struct
{
    volatile uint32_t CR;           /*<><Address Offset: 0x00>*/
    volatile uint32_t PLLCFGR;      /*<><Address Offset: 0x00>*/
    volatile uint32_t CFGR;         /*<><Address Offset: 0x00>*/
    volatile uint32_t CIR;          /*<><Address Offset: 0x00>*/
    volatile uint32_t AHB1RSTR;     /*<><Address Offset: 0x00>*/
    volatile uint32_t AHB2RSTR;     /*<><Address Offset: 0x00>*/
    volatile uint32_t AHB3RSTR;     /*<><Address Offset: 0x00>*/
    uint32_t RESERVED0;             /*<!Reserved Memory - Cannot be used!><Address Offset: 0x00>*/
    volatile uint32_t APB1RSTR;     /*<><Address Offset: 0x00>*/
    volatile uint32_t APB2RSTR;     /*<><Address Offset: 0x00>*/
    uint32_t RESERVED1[2];          /*<!Reserved Memory - Cannot be used!><Address Offset: 0x00>*/
    volatile uint32_t AHB1ENR;      /*<><Address Offset: 0x00>*/
    volatile uint32_t AHB2ENR;      /*<><Address Offset: 0x00>*/
    volatile uint32_t AHB3ENR;      /*<><Address Offset: 0x00>*/
    uint32_t RESERVED2;             /*<!Reserved Memory - Cannot be used!><Address Offset: 0x00>*/
    volatile uint32_t APB1ENR;      /*<><Address Offset: 0x00>*/
    volatile uint32_t APB2ENR;      /*<><Address Offset: 0x00>*/
    uint32_t RESERVED3[2];          /*<!Reserved Memory - Cannot be used!><Address Offset: 0x00>*/
    volatile uint32_t AHB1LPENR;    /*<><Address Offset: 0x00>*/
    volatile uint32_t AHB2LPENR;    /*<><Address Offset: 0x00>*/
    volatile uint32_t AHB3LPENR;    /*<><Address Offset: 0x00>*/
    uint32_t RESERVED4;             /*<!Reserved Memory - Cannot be used!><Address Offset: 0x00>*/
    volatile uint32_t APB1LPENR;    /*<><Address Offset: 0x00>*/
    volatile uint32_t APB2LPENR;    /*<><Address Offset: 0x00>*/
    uint32_t RESERVED5[2];          /*<!Reserved Memory - Cannot be used!><Address Offset: 0x00>*/
    volatile uint32_t BDCR;         /*<><Address Offset: 0x00>*/
    volatile uint32_t CSR;          /*<><Address Offset: 0x00>*/
    uint32_t RESERVED6[2];          /*<!Reserved Memory - Cannot be used!><Address Offset: 0x00>*/
    volatile uint32_t SSCGR;        /*<><Address Offset: 0x00>*/
    volatile uint32_t PLLI2SCFGR;   /*<><Address Offset: 0x00>*/
}RCC_RegDef;


// EXTI Register Definition Structure
typedef struct
{
    volatile uint32_t IMR;          /*<><Address offset: 0x00>*/
    volatile uint32_t EMR;          /*<><Address offset: 0x00>*/
    volatile uint32_t RTSR;         /*<><Address offset: 0x00>*/
    volatile uint32_t FTSR;         /*<><Address offset: 0x00>*/
    volatile uint32_t SWIER;        /*<><Address offset: 0x00>*/
    volatile uint32_t PR;           /*<><Address offset: 0x00>*/
}EXTI_RegDef;


// SPI Register Definition Structure
typedef struct
{
    volatile uint32_t CR1;          /*<><Address Offset: 0x00>*/
    volatile uint32_t CR2;          /*<><Address Offset: 0x00>*/
    volatile uint32_t SR;           /*<><Address Offset: 0x00>*/
    volatile uint32_t DR;           /*<><Address Offset: 0x00>*/
    volatile uint32_t CRCPR;        /*<><Address Offset: 0x00>*/
    volatile uint32_t RXCRCR;       /*<><Address Offset: 0x00>*/
    volatile uint32_t TXCRCR;       /*<><Address Offset: 0x00>*/
    volatile uint32_t I2SCFGR;      /*<><Address Offset: 0x00>*/
    volatile uint32_t I2SPR;        /*<><Address Offset: 0x00>*/
}SPI_RegDef;


// SYSCFG Register Definition Structure
typedef struct
{
    volatile uint32_t MEMRMP;           /*<><Address offset: 0x00>*/
    volatile uint32_t PMC;              /*<><Address offset: 0x00>*/
    volatile uint32_t EXTICR[4];        /*<><Address offset: 0x00>*/
    volatile uint32_t RESERVED1[2];     /*<!Reserved Memory - Cannot be used!><Address offset: 0x00>*/
    volatile uint32_t CMPCR;            /*<><Address offset: 0x00>*/
    volatile uint32_t RESERVED2[2];     /*<!Reserved Memory - Cannot be used!><Address offset: 0x00>*/
    volatile uint32_t CFGR;             /*<><Address offset: 0x00>*/

}SYSCFG_RegDef;


// I2C Register Definition Structure
typedef struct 
{
    volatile uint32_t CR1;              /*<><Address Offset: 0x00>*/
    volatile uint32_t CR2;              /*<><Address Offset: 0x00>*/
    volatile uint32_t OAR1;             /*<><Address Offset: 0x00>*/
    volatile uint32_t OAR2;             /*<><Address Offset: 0x00>*/
    volatile uint32_t DR;               /*<><Address Offset: 0x00>*/
    volatile uint32_t SR1;              /*<><Address Offset: 0x00>*/
    volatile uint32_t SR2;              /*<><Address Offset: 0x00>*/
    volatile uint32_t CCR;              /*<><Address Offset: 0x00>*/
    volatile uint32_t TRISE;            /*<><Address Offset: 0x00>*/
    volatile uint32_t FLTR;             /*<><Address Offset: 0x00>*/

}I2C_RegDef;


// USART Register Definition Structure
typedef struct
{
    volatile uint32_t SR;               /*<><Address Offset: 0x00>*/
    volatile uint32_t DR;               /*<><Address Offset: 0x00>*/
    volatile uint32_t BRR;              /*<><Address Offset: 0x00>*/
    volatile uint32_t CR1;              /*<><Address Offset: 0x00>*/
    volatile uint32_t CR2;              /*<><Address Offset: 0x00>*/
    volatile uint32_t CR3;              /*<><Address Offset: 0x00>*/
    volatile uint32_t GTPR;             /*<><Address Offset: 0x00>*/
}USART_RegDef;

/*************************************** Peripheral Registers Definition Structures End ***************************************/
/*----------------------------------------------------------------------------------------------------------------------------*/
/******************************************** Peripheral Address Definitions Start ********************************************/

// GPIO Port Definitions
#define GPIOA               ((GPIO_RegDef*)GPIOA_BASE_ADDR)
#define GPIOB               ((GPIO_RegDef*)GPIOB_BASE_ADDR)
#define GPIOC               ((GPIO_RegDef*)GPIOC_BASE_ADDR)
#define GPIOD               ((GPIO_RegDef*)GPIOD_BASE_ADDR)
#define GPIOE               ((GPIO_RegDef*)GPIOE_BASE_ADDR)
#define GPIOF               ((GPIO_RegDef*)GPIOF_BASE_ADDR)
#define GPIOG               ((GPIO_RegDef*)GPIOG_BASE_ADDR)
#define GPIOH               ((GPIO_RegDef*)GPIOH_BASE_ADDR)
#define GPIOI               ((GPIO_RegDef*)GPIOI_BASE_ADDR)
#define GPIOJ               ((GPIO_RegDef*)GPIOJ_BASE_ADDR)
#define GPIOK               ((GPIO_RegDef*)GPIOK_BASE_ADDR)

// RCC  Definition
#define RCC                 ((RCC_RegDef*)RCC_BASE_ADDR)

// EXTI Definition
#define EXTI				((EXTI_RegDef*)EXTI_BASE_ADDR)

// SYSCFG Definition
#define SYSCFG				((SYSCFG_RegDef*)SYSCFG_BASE_ADDR)

// SPI Definition
#define SPI1  				((SPI_RegDef*)SPI1_BASE_ADDR)
#define SPI2  				((SPI_RegDef*)SPI2_BASE_ADDR)
#define SPI3  				((SPI_RegDef*)SPI3_BASE_ADDR)

// I2C Definition
#define I2C1  				((I2C_RegDef*)I2C1_BASE_ADDR)
#define I2C2  				((I2C_RegDef*)I2C2_BASE_ADDR)
#define I2C3  				((I2C_RegDef*)I2C3_BASE_ADDR)

// USART Definition
#define USART1  			((USART_RegDef*)USART1_BASE_ADDR)
#define USART2  			((USART_RegDef*)USART2_BASE_ADDR)
#define USART3  			((USART_RegDef*)USART3_BASE_ADDR)
#define USART6  			((USART_RegDef*)USART6_BASE_ADDR)

// UART Definition
#define UART4  				((USART_RegDef*)UART4_BASE_ADDR)
#define UART5  				((USART_RegDef*)UART5_BASE_ADDR)

/********************************************* Peripheral Address Definitions End *********************************************/
/*----------------------------------------------------------------------------------------------------------------------------*/




#endif /* INC_STM3F407XX_H */
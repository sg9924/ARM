#ifndef INC_stm32F103xx_USART_H
#define INC_stm32F103xx_USART_H

#include"stm32f103xx.h"
/************************************************** USART Definitions Start *************************************************/

//USART Peripherals
#define USART1                   ((USART_RegDef*)USART1_BASE_ADDR)
#define USART2                   ((USART_RegDef*)USART2_BASE_ADDR)
#define USART3                   ((USART_RegDef*)USART3_BASE_ADDR)
#define UART4                    ((USART_RegDef*)UART4_BASE_ADDR)
#define UART5                    ((USART_RegDef*)UART5_BASE_ADDR)


//USART_SR bit fields
#define USART_SR_PE               0    /*<Parity Error Flag>*/
#define USART_SR_FE               1    /*<Framing Error Flag>*/
#define USART_SR_NE               2    /*<Noise Error Flag>*/
#define USART_SR_ORE              3    /*<Overrun Error Flag>*/
#define USART_SR_IDLE             4    /*<IDLE Line Flag>*/
#define USART_SR_RXNE             5    /*<RX Data Register not Empty Flag>*/
#define USART_SR_TC               6    /*<TX Complete Flag>*/
#define USART_SR_TXE              7    /*<TX Data Register Empty Flag>*/
#define USART_SR_LBD              8    /*<LIN Break Detection Flag>*/
#define USART_SR_CTS              9    /*<CTS Flag>*/

//USART_DR bit fields
#define USART_DR                  0 

//USART_BRR bit fields
#define USART_BRR_DIV_FRACTION    0
#define USART_BRR_DIV_MANTISSA    4
 

//USART_CR1 bit fields
#define USART_CR1_SBK             0
#define USART_CR1_RWU             1
#define USART_CR1_RE              2
#define USART_CR1_TE              3
#define USART_CR1_IDLEIE          4
#define USART_CR1_RXNEIE          5
#define USART_CR1_TCIE            6
#define USART_CR1_TXEIE           7
#define USART_CR1_PEIE            8
#define USART_CR1_PS              9
#define USART_CR1_PCE             10
#define USART_CR1_WAKE            11
#define USART_CR1_M               12
#define USART_CR1_UE              13

//USART_CR2 bit fields
#define USART_CR2_ADD             0
#define USART_CR2_LBDL            5
#define USART_CR2_LBDIE           6
#define USART_CR2_LBCL            8
#define USART_CR2_CPHA            9
#define USART_CR2_CPOL            10
#define USART_CR2_CLKEN           11
#define USART_CR2_STOP            12
#define USART_CR2_LINEN           14

//USART_CR3 bit fields
#define USART_CR3_EIE              0
#define USART_CR3_IREN             1
#define USART_CR3_IRLP             2
#define USART_CR3_HDSEL            3
#define USART_CR3_NACK             4
#define USART_CR3_SCEN             5
#define USART_CR3_DMAR             6
#define USART_CR3_DMAT             7
#define USART_CR3_RTSE             8
#define USART_CR3_CTSE             9
#define USART_CR3_CTSIE            10


//USART Baudrate
#define USART_BAUDRATE_2400        2400
#define USART_BAUDRATE_4800        4800
#define USART_BAUDRATE_9600        9600
#define USART_BAUDRATE_19200       19200
#define USART_BAUDRATE_57600       57600
#define USART_BAUDRATE_115200      115200

//USART Parity Control
#define USART_PARITY_DISABLE       0
#define USART_PARITY_ENABLE        1

//USART Parity Type
#define USART_PARITY_EVEN          0
#define USART_PARITY_ODD           1

//USART Stop Bits
#define USART_STOPBIT_0_5          0
#define USART_STOPBIT_1            1
#define USART_STOPBIT_1_5          2
#define USART_STOPBIT_2            3

//USART Clock Polarity
#define USART_CPOL_LOW             0
#define USART_CPOL_HIGH            1

//USART Clock Phase
#define USART_CPHA_LOW             0
#define USART_CPHA_HIGH            1

/*************************************************** USART Definitions End **************************************************
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************ USART Macros Definitions Start ********************************************/

// USART/UART Clock Enable
#define USART1_PCLK_EN()         (RCC->APB2ENR |= 1<<14)
#define USART2_PCLK_EN()         (RCC->APB1ENR |= 1<<17)
#define USART3_PCLK_EN()         (RCC->APB1ENR |= 1<<18)
#define UART4_PCLK_EN()          (RCC->APB1ENR |= 1<<19)
#define UART5_PCLK_EN()          (RCC->APB1ENR |= 1<<20)

//USART/UART Enable
#define USART2_ENABLE(pusart_handle)            (pusart_handle->pUSARTx->CR1 |= 1<<USART_CR1_UE)

/************************************************ USART Macros Definitions End **********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/********************************************* USART Structure Definitions Start ********************************************/

typedef struct
{
    uint32_t baudrate;
    uint8_t  parity_control;
    uint8_t  parity_type;
    uint8_t  stop_bits;
    uint8_t  clock_polarity;
    uint8_t  clock_phase;
    char*    rx_data;
}USART_Config;

// USART Handler Structure
typedef struct
{
    USART_RegDef     *pUSARTx;              /*<USART Register Definition>*/
    USART_Config     USARTx_Config;        /*<USART Config Settings>*/
    GPIO_Handle      *pGPIOHandle;
}USART_Handle;

/********************************************* USART Structure Definitions Start ********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
#endif /* INC_stm32F103xx_GPIO_H */
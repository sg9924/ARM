#ifndef INC_STM32F407XX_GPIO_DRIVER_H
#define INC_STM32F407XX_GPIO_DRIVER_H


// Custom Header File
#include"stm32F407xx.h"


/************************************************** GPIO Definitions Start **************************************************/

// GPIO Pin Numbers
#define GPIO_PIN0                       0
#define GPIO_PIN1                       1
#define GPIO_PIN2                       2
#define GPIO_PIN3                       3
#define GPIO_PIN4                       4
#define GPIO_PIN5                       5
#define GPIO_PIN6                       6
#define GPIO_PIN7                       7
#define GPIO_PIN8                       8
#define GPIO_PIN9                       9
#define GPIO_PIN10                      10
#define GPIO_PIN11                      11
#define GPIO_PIN12                      12
#define GPIO_PIN13                      13
#define GPIO_PIN14                      14
#define GPIO_PIN15                      15


// GPIO Possible Pin Modes
#define GPIO_MODE_INP                   0           /*<Input Mode>*/
#define GPIO_MODE_OP                    1           /*<Output Mode>*/
#define GPIO_MODE_ALTFUN                2           /*<Alternate Funcationality>*/
#define GPIO_MODE_ANALOG                3           /*<Analog Mode>*/
#define GPIO_MODE_INTRPT_FE_TRIG        4           /*<Interrupt on Falling Edge Trigger Mode>*/
#define GPIO_MODE_INTRPT_RE_TRIG        5           /*<Interrupt on Rising Edge Trigger Mode>*/
#define GPIO_MODE_INTRPT_FERE_TRIG      6           /*<Interrupt on Falling & Rising Edge Trigger Mode>*/


// GPIO Possible Output Types
#define GPIO_OP_TYPE_PP                 0           /*<Push Pull>*/
#define GPIO_OP_TYPE_OD                 1           /*<Open Drain>*/


// GPIO Possible Ouput Speeds
#define GPIO_OP_SPEED_LOW               0           /*<>*/
#define GPIO_OP_SPEED_MEDIUM            1           /*<>*/
#define GPIO_OP_SPEED_FAST              2           /*<>*/
#define GPIO_OP_SPEED_HIGH              3           /*<>*/

// GPIO Pull Up Pull Down Values
#define GPIO_NO_PUPD   		            0           /*<No Pull Up Pull Down>*/
#define GPIO_PIN_PU			            1           /*<Pull Up>*/
#define GPIO_PIN_PD			            2           /*<Pull Down>*/

/*************************************************** GPIO Definitions End ***************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/


/********************************************* GPIO Structure Definitions Start *********************************************/

//GPIO Pin Configuration Structure
typedef struct
{
    uint8_t PinNo;                          /*<>*/
    uint8_t PinMode;                        /*<>*/
    uint8_t PinSpeed;                       /*<>*/
    uint8_t PinPUPDCtrl;                    /*<>*/
    uint8_t PinOPType;                      /*<>*/
    uint8_t PinAltFuncMode;                 /*<>*/

}GPIO_PinConfig;


// GPIO Handler Structure
typedef struct
{
    GPIO_RegDef    *pGPIOx;                 /*<>*/
    GPIO_PinConfig  GPIOx_PinConfig;        /*<>*/
}GPIO_Handle;

/********************************************** GPIO Structure Definitions End **********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/

#endif /* INC_STM32F407XX_GPIO_DRIVER_H */
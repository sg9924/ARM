#ifndef SEGDISPLAY_H
#define SEGDISPLAY_H

#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"
#include"stm32f103xx_utilities.h"
#include"seg_3461BS.h" //any one config file should be provided here


typedef struct
{
    uint8_t seg_A;
    uint8_t seg_B;
    uint8_t seg_C;
    uint8_t seg_D;
    uint8_t seg_E;
    uint8_t seg_F;
    uint8_t seg_G;
    uint8_t seg_H;

}segments_t;

typedef struct
{
    uint8_t DIGIT_1;
    uint8_t DIGIT_2;
    uint8_t DIGIT_3;
    uint8_t DIGIT_4;
    
}digits_t;

typedef struct
{
    GPIO_RegDef* Seg_GPIO_Port;
    GPIO_RegDef* Digit_GPIO_Port;
    GPIO_Handle* pGPIOHandle;
    segments_t*  segs;
    digits_t*    digits;
    uint8_t      conn_type;
    uint8_t      display_type;
    uint8_t      selected_digit;

}SevSeg_Handle;


//Muxing Time Delay - 5ms is appropriate
#define MUX_DELAY_INTERVAL                5

//Functions
//Configure
void seg_pin_configure(SevSeg_Handle* pSEGh, segments_t* segment_pins, GPIO_RegDef* GPIO_Port);
void seg_digit_pin_configure(SevSeg_Handle* pSEGh, digits_t* digit_pins, GPIO_RegDef* GPIO_Port);
void seg_configure(SevSeg_Handle* pSEGh, segments_t* segment_pins, GPIO_RegDef* Seg_GPIO, digits_t* digit_pins, GPIO_RegDef* Digit_GPIO);

//Initialize
void seg_init(SevSeg_Handle* pSEGh, GPIO_Handle* pGPIOh);

//Display
void seg_display(GPIO_RegDef* pGPIOx, segments_t* segs, uint8_t character);
void seg_display_digit(SevSeg_Handle* pSEGh, uint8_t character, uint8_t digit_pin);

//Reset
void seg_pins_reset(GPIO_RegDef* pGPIOx, segments_t* segs);
void seg_digit_reset(GPIO_RegDef* pGPIOx, digits_t* digits);

//Digit Select
void seg_digit_select(SevSeg_Handle* pSEGh, uint8_t digit_pin);


#endif /*SEGDISPLAY_H*/
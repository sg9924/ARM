#ifndef SEVENSEG_H
#define SEVENSEG_H

#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"
#include"gpio_7_seg_map.h"
#include"seven_seg_5161AS.h"

/*to be used later
typedef struct
{
    GPIO_RegDef* Seg_Handle;
    GPIO_RegDef* Digit_Handle;
    uint8_t display_type;
    uint8_t conn_type;
    uint8_t selected_digit;

}SevSeg_Handle;
*/

//Seven Segment Connection Type
#define SEVEN_SEG_COMMON_CATHODE          0
#define SEVEN_SEG_COMMON_ANODE            1


//Functions
void seven_seg_init(GPIO_Handle* pGPIOh, GPIO_RegDef* GPIO_Port, uint8_t conn_type);
void seven_seg_pins_reset(GPIO_Handle* pGPIOh, uint8_t conn_type);
void seven_seg_display(GPIO_Handle* pGPIOh, uint8_t character, uint8_t conn_type);


#endif /*SEVENSEG_H*/
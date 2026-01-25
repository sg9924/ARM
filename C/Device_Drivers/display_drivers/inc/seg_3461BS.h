#ifndef SEG3461BS_H
#define SEG3461BS_H

#include"seg_display_properties.h"

/*--Segment Pins
SEG_A        SEG_PIN8
SEG_B        SEG_PIN12
SEG_C        SEG_PIN4
SEG_D        SEG_PIN2
SEG_E        SEG_PIN1
SEG_F        SEG_PIN9
SEG_G        SEG_PIN5
SEG_H        SEG_PIN3

DIGIT1       SEG_PIN7
DIGIT2       SEG_PIN10
DIGIT3       SEG_PIN11
DIGIT4       SEG_PIN6
*/

/*--Recommended GPIO Mapping
segments_t seg_pins = {GPIO_PIN7, GPIO_PIN9, GPIO_PIN0, GPIO_PIN5, GPIO_PIN10, GPIO_PIN8, GPIO_PIN4, GPIO_PIN1};
digits_t digit_pins = {GPIO_PIN8, GPIO_PIN5, GPIO_PIN6, GPIO_PIN7};
*/


#define SEG_CONNECTION_TYPE     SEG_COMMON_ANODE
#define SEG_DISPLAY_TYPE        SEG_MULTI_DIGIT
#define NO_OF_SEGMENTS          7
#define NO_OF_DIGITS            4

#endif /*SEG3461BS2_H*/
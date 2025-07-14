#ifndef SEG5161AS_H
#define SEG5161AS_H

#include"seg_display_properties.h"

/*--Segment Pins
SEG_A        SEG_PIN7
SEG_B        SEG_PIN6
SEG_C        SEG_PIN4
SEG_D        SEG_PIN2
SEG_E        SEG_PIN1
SEG_F        SEG_PIN9
SEG_G        SEG_PIN10
SEG_H        SEG_PIN5
*/

/*--Recommended GPIO Mapping
segments_t seg_pins = {GPIO_PIN7, GPIO_PIN9, GPIO_PIN0, GPIO_PIN5, GPIO_PIN10, GPIO_PIN8, GPIO_PIN4, GPIO_PIN1};
*/


#define SEG_CONNECTION_TYPE     SEG_COMMON_CATHODE
#define SEG_DISPLAY_TYPE        SEG_MULTI_DIGIT
#define NO_OF_SEGMENTS          7
#define NO_OF_DIGITS            1

#endif /*SEG3461BS2_H*/
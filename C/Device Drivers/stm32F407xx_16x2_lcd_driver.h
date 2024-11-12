#ifndef LCD_H
#define LCD_H

#include "stm32F407xx.h"

// Macros
#define LCD_GPIO_PORT                   GPIOD
#define LCD_GPIO_RS                     GPIO_PIN0
#define LCD_GPIO_RW                     GPIO_PIN1
#define LCD_GPIO_EN                     GPIO_PIN2
#define LCD_GPIO_D0                     GPIO_PIN3
#define LCD_GPIO_D1                     GPIO_PIN4
#define LCD_GPIO_D2                     GPIO_PIN5
#define LCD_GPIO_D3                     GPIO_PIN6
#define LCD_GPIO_D4                     GPIO_PIN7
#define LCD_GPIO_D5                     GPIO_PIN8
#define LCD_GPIO_D6                     GPIO_PIN9
#define LCD_GPIO_D7                     GPIO_PIN10


// LCD Commands Macros
#define LCD_CMD_4DL_2N_5X8F             0x28
#define LCD_CMD_DON_CURON               0x0E
#define LCD_CMD_INCADD                  0x06
#define LCD_CMD_DIS_CLEAR               0x01
#define LCD_CMD_DIS_RETURN_HOME         0x02      


// LCD Functions Declarations
void display_return_home(void);
void display_clear(void);
void lcd_set_cursor(uint8_t, uint8_t);
void lcd_send_command(uint8_t);
void lcd_send_char(uint8_t);
void lcd_send_string(char*);
void lcd_init(void);
void lcd_print(char*, uint8_t);
void lcd_print_blink(char*, uint8_t);
void lcd_print_slide(char*, uint8_t);


#endif /*LCD_H*/
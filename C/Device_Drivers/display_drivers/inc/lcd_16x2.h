#ifndef INC_stm32F103xx_LCD_16x2_H
#define INC_stm32F103xx_LCD_16x2_H

#include"stm32f103xx.h"
#include"stm32f103xx_memory_map.h"
#include"stm32f103xx_gpio.h"
#include"stm32f103xx_init.h"
#include"stm32f103xx_utilities.h"

/*to be used later
//LCD Handle
typedef struct 
{
    GPIO_Handle* LCD_GPIO_Handle;

    //Display On/Off Control
    uint8_t display_status;
    uint8_t cursor_status;
    uint8_t blink_status;

    //Cursor/Display Shift
    uint8_t display_shift;
    uint8_t cursor_move;
    uint8_t direction;

    //Function Set
    uint8_t data_lines;
    uint8_t display_lines;
    uint8_t font;

    //Entry Mode Set
    uint8_t display_shift_status;
    uint8_t cursor_direction;
}LCD_Handle;
*/

//LCD Pins
#define LCD_GPIO_PORT                   GPIOA
#define LCD_GPIO_RS                     GPIO_PIN0
#define LCD_GPIO_RW                     GPIO_PIN1
#define LCD_GPIO_EN                     GPIO_PIN8
#define LCD_GPIO_D0                     GPIO_PIN4
#define LCD_GPIO_D1                     GPIO_PIN5
#define LCD_GPIO_D2                     GPIO_PIN6
#define LCD_GPIO_D3                     GPIO_PIN7
#define LCD_GPIO_D4                     GPIO_PIN9
#define LCD_GPIO_D5                     GPIO_PIN10
#define LCD_GPIO_D6                     GPIO_PIN11
#define LCD_GPIO_D7                     GPIO_PIN12

//LCD DDRAM Addresses
#define LCD_DDRAM_LINE1_START_ADDR      0x00
#define LCD_DDRAM_LINE2_START_ADDR      0x40

//LCD Commands
//Display ON/OFF Control
#define LCD_CMD1_DISPLAY_ON             0xC
#define LCD_CMD1_CURSOR_ON              0xA
#define LCD_CMD1_DISPLAY_OFF            0x8
#define LCD_CMD1_CURSOR_OFF             0x8
#define LCD_CMD1_CURSOR_BLINK_ON        0x9
#define LCD_CMD1_CURSOR_BLINK_OFF       0x8

//Cursor / Display Shift
#define LCD_CMD2_DISPLAY_SHIFT          0x18
#define LCD_CMD2_CURSOR_MOVE            0x10
#define LCD_CMD2_LEFT                   0x10
#define LCD_CMD2_RIGHT                  0x14
#define LCD_CMD2_MOVE_CURSOR_RIGHT      0x14
#define LCD_CMD2_MOVE_CURSOR_LEFT       0x10
#define LCD_CMD2_SHIFT_DISPLAY_RIGHT    0x1C
#define LCD_CMD2_SHIFT_DISPLAY_LEFT     0x18

//Function Set
#define LCD_CMD3_DATA_LINES_8           0x30
#define LCD_CMD3_DATA_LINES_4           0x20
#define LCD_CMD3_DISPLAY_LINES_2        0x28
#define LCD_CMD3_DISPLAY_LINES_1        0x20
#define LCD_CMD3_FONT_5x11              0x24
#define LCD_CMD3_FONT_5x8               0x20

//Entry Mode Set
#define LCD_CMD4_CURSOR_INCREMENT       0x6
#define LCD_CMD4_CURSOR_DECREMENT       0x4
#define LCD_CDM4_DISPLAY_SHIFT_ON       0x5
#define LCD_CDM4_DISPLAY_SHIFT_OFF      0x4

//Set DDRAM Address
#define LCD_CMD_SET_DDRAM_ADDR          0x80

//Set CGRAM Address
#define LCD_CMD_SET_CGRAM_ADDR          0x40

//Clear Display
#define LCD_CMD_DISPLAY_CLEAR           0x1

//Display Return Home
#define LCD_CMD_DISPLAY_RETURN_HOME     0x2

//LCD Mode
#define LCD_MODE_DATA_WRITE             0
#define LCD_MODE_DATA_READ              1
#define LCD_MODE_COMMAND                2


//LCD Configuration
#define LCD_INTERFACE_8_BIT             LCD_CMD3_DATA_LINES_8
#define LCD_INTERFACE_4_BIT             LCD_CMD3_DATA_LINES_4

#define LCD_FONT_5x8                    LCD_CMD3_FONT_5x8
#define LCD_FONT_5x11                   LCD_CMD3_FONT_5x11

#define LCD_DISPLAY_LINES_1             LCD_CMD3_DISPLAY_LINES_1
#define LCD_DISPLAY_LINES_2             LCD_CMD3_DISPLAY_LINES_2

#define LCD_DISPLAY_ON                  LCD_CMD1_DISPLAY_ON
#define LCD_DISPLAY_OFF                 LCD_CMD1_DISPLAY_OFF

#define LCD_CURSOR_ON                   LCD_CMD1_CURSOR_ON
#define LCD_CURSOR_OFF                  LCD_CMD1_CURSOR_OFF

#define LCD_BLINK_ON                    LCD_CMD1_CURSOR_BLINK_ON
#define LCD_BLINK_OFF                   LCD_CMD1_CURSOR_BLINK_OFF

#define LCD_CURSOR_INC                  LCD_CMD4_CURSOR_INCREMENT
#define LCD_CURSOR_DEC                  LCD_CMD4_CURSOR_DECREMENT

#define LCD_DSIP_SHIFT_ON               LCD_CDM4_DISPLAY_SHIFT_ON
#define LCD_DISP_SHIFT_OFF              LCD_CDM4_DISPLAY_SHIFT_OFF

#define LCD_LINE_1                      1
#define LCD_LINE_2                      2




void lcd_init(uint8_t interface_type, uint8_t display_lines, uint8_t font, uint8_t display, uint8_t cursor, uint8_t blink,
              uint8_t cursor_direction, uint8_t display_shift);
void lcd_send_command(uint8_t cmd);
void lcd_send_char(uint8_t data);
void lcd_send_string(char* msg);
void lcd_display_clear();
void lcd_display_return_home();
void lcd_set_cursor(uint8_t row, uint8_t column);


void lcd_print(char* msg, uint8_t line_no);
void lcd_print_blink(char* msg, uint16_t delay_ms);
void lcd_print_slide(char* msg, uint16_t delay_ms);
void lcd_print_slide_fwd(char* msg, uint16_t delay_ms);
void lcd_print_slide_back(char* msg, uint16_t delay_ms);





#endif
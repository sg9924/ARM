#include "16x2_lcd.h"
#include "stm32F407xx_gpio_driver.h"

/*--------------------------------------------------------------------------------------------------------------------------*/
/*************************************** 16x2 LCD Helper Functions Definitions Start ****************************************/

//larger delay
static void mdelay(uint32_t count)
{
    for(int i=0; i<(count*1000); i++);
}

//smaller delay
static void udelay(uint32_t count)
{
    for(int i=0; i<(count); i++);
}

//lcd enable
static void lcd_enable()
{
    //send HIGH to LOW tranisition on EN line in order to latch the data on the data lines
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_SET);
    //provide delay
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
}

//write four bits
static void write_bits_4(uint8_t value)
{
    //Note: D7-D4 are used for 4 bit data
    // Format: 4 bits -> MSB . . LSB -> D7 D6 D5 D4
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D7, ((value>>3) & 0x1)); //MSB
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D6, ((value>>2) & 0x1));
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D5, ((value>>1) & 0x1));
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D4, ((value>>0) & 0x1)); //LSB

    lcd_enable();
}

/**************************************** 16x2 LCD Helper Functions Definitions End *****************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
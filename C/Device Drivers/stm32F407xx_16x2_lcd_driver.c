#include "stm32F407xx_16x2_lcd_driver.h"
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
/****************************************** 16x2 LCD Functions Definitions Start ********************************************/

//send command to LCD
void lcd_send_command(uint8_t cmd)
{
    // set RS to 0
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
    //set RW to 0
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

    //first send higher nibble of the command
    write_bits_4(cmd >> 4);
    //then send lower nibble of the command
    write_bits_4(cmd & 0x0F);

    lcd_enable();
}

//send character to LCD
void lcd_send_char(uint8_t data)
{
    // set RS to 1
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_SET);
    //set RW to 0
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

    //first send higher nibble of the command
    write_bits_4(data >> 4);
    //then send lower nibble of the command
    write_bits_4(data & 0x0F);

    lcd_enable();
}

//send string to LCD
void lcd_send_string(char* msg)
{
    do
    {
        lcd_send_char((uint8_t)*msg++);
    }while(*msg != '\0');
}



void lcd_init()
{
    //1. Configure GPIO pins used for LCD Connections
    GPIO_Handle lcd_signal;

    lcd_signal.pGPIOx = LCD_GPIO_PORT;
    lcd_signal.GPIOx_PinConfig.PinMode = GPIO_MODE_OP;
    lcd_signal.GPIOx_PinConfig.PinNo = LCD_GPIO_RS;
    lcd_signal.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
    lcd_signal.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
    lcd_signal.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
    GPIO_Init(&lcd_signal);

    //2. Intialize GPIO Pins for the respectiveLCD Pins
    lcd_signal.GPIOx_PinConfig.PinNo = LCD_GPIO_RW;
    GPIO_Init(&lcd_signal);

    lcd_signal.GPIOx_PinConfig.PinNo = LCD_GPIO_EN;
    GPIO_Init(&lcd_signal);

    lcd_signal.GPIOx_PinConfig.PinNo = LCD_GPIO_D4;
    GPIO_Init(&lcd_signal);

    lcd_signal.GPIOx_PinConfig.PinNo = LCD_GPIO_D5;
    GPIO_Init(&lcd_signal);

    lcd_signal.GPIOx_PinConfig.PinNo = LCD_GPIO_D6;
    GPIO_Init(&lcd_signal);

    lcd_signal.GPIOx_PinConfig.PinNo = LCD_GPIO_D7;
    GPIO_Init(&lcd_signal);


    //3. Set initial value as 0 for all the pins configured for the LCD
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D4, GPIO_PIN_RESET);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D5, GPIO_PIN_RESET);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D6, GPIO_PIN_RESET);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D7, GPIO_PIN_RESET);


    //4. Initialize the LCD
    //provide 15ms delay
    mdelay(15);

    //Function set
    write_bits_4(0x3);

    //a. set 0 to RS & RW
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET); 
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

    //provide 4.1 ms (5 ms) delay
    mdelay(5);
    
    //Function set
    write_bits_4(0x3);

    //provide 100 us delay
    udelay(100);

    //Function set
    write_bits_4(0x3);

    //Function set
    write_bits_4(0x3);
    //Function set
    write_bits_4(0x2);

    //function set command - 4 data lines, 2 display lines, small font
    lcd_send_command(LCD_CMD_4DL_2N_5X8F);

    //display on, cursor on
    lcd_send_command(LCD_CMD_DON_CURON);

    //display clear
    lcd_send_command(LCD_CMD_DIS_CLEAR);
    mdelay(2);//provide delay - 2ms

    //entry mode set
    lcd_send_command(LCD_CMD_INCADD);

}


/*--------------------------------------------------------------------------------------------------------------------------*/
/******************************************* 16x2 LCD Functions Definitions End *********************************************/
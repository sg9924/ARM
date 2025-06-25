#include"lcd_16x2.h"

/*--------------------------------------------------------------------------------------------------------------------------*/
/*************************************** 16x2 LCD Helper Functions Definitions Start ****************************************/

static void lcd_enable()
{
    //HIGH to LOW transition on EN pin to latch the Data
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_SET);
    tim_delay_us(10);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
}


static void lcd_write_4_bits(uint8_t data)
{
    //D7-D4 are used for 4 bit interface
    //D7-D4: MSB-LSB
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D7, (data>>3 & 0x1));
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D6, (data>>2 & 0x1));
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D5, (data>>1 & 0x1));
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D4, (data & 0x1));

    lcd_enable();
}


static void lcd_mode(uint8_t mode)
{
    if(mode == LCD_MODE_DATA_WRITE)
    {
        //set RS to HIGH
        GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_SET);
        //set RW to LOW
        GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
    }
    else if(mode == LCD_MODE_COMMAND)
    {
        //set RS to LOW
        GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
        //set RW to LOW
        GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
    }
}


static void lcd_gpio_init()
{
    //Configure GPIO pins used for LCD Connections
    GPIO_Handle lcd;

    lcd.pGPIOx = LCD_GPIO_PORT;
    lcd.GPIOx_PinConfig.PinMode        = GPIO_MODE_OP;
    lcd.GPIOx_PinConfig.PinConfigType  = GPIO_CONFIG_GP_OP_PP;
    lcd.GPIOx_PinConfig.PinOutputSpeed = GPIO_OP_SPEED_2;

    lcd.GPIOx_PinConfig.PinNo = LCD_GPIO_RS;
    GPIO_Init(&lcd);

    lcd.GPIOx_PinConfig.PinNo = LCD_GPIO_RW;
    GPIO_Init(&lcd);

    lcd.GPIOx_PinConfig.PinNo = LCD_GPIO_EN;
    GPIO_Init(&lcd);

    lcd.GPIOx_PinConfig.PinNo = LCD_GPIO_D4;
    GPIO_Init(&lcd);

    lcd.GPIOx_PinConfig.PinNo = LCD_GPIO_D5;
    GPIO_Init(&lcd);

    lcd.GPIOx_PinConfig.PinNo = LCD_GPIO_D6;
    GPIO_Init(&lcd);

    lcd.GPIOx_PinConfig.PinNo = LCD_GPIO_D7;
    GPIO_Init(&lcd);
}


static void lcd_pins_reset(uint8_t interface_type)
{
    //Set initial value as 0 for all the pins configured for the LCD
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D4, GPIO_PIN_RESET);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D5, GPIO_PIN_RESET);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D6, GPIO_PIN_RESET);
    GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D7, GPIO_PIN_RESET);

    if(interface_type == LCD_INTERFACE_8_BIT)
    {
        GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D0, GPIO_PIN_RESET);
        GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D1, GPIO_PIN_RESET);
        GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D2, GPIO_PIN_RESET);
        GPIO_WriteOpPin(LCD_GPIO_PORT, LCD_GPIO_D3, GPIO_PIN_RESET);
    }
}

/**************************************** 16x2 LCD Helper Functions Definitions End *****************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/****************************************** 16x2 LCD Functions Definitions Start ********************************************/

//send command to LCD
void lcd_send_command(uint8_t cmd)
{
    lcd_mode(LCD_MODE_COMMAND);

    //send higher nibble of the command
    lcd_write_4_bits(cmd >> 4);
    //send lower nibble of the command
    lcd_write_4_bits(cmd & 0x0F);
}


void lcd_display_return_home()
{
    lcd_send_command(LCD_CMD_DISPLAY_RETURN_HOME);
    tim_delay_ms(2);
}


void lcd_display_clear()
{
    lcd_send_command(LCD_CMD_DISPLAY_CLEAR);
    //wait for more than 1.53ms
    tim_delay_ms(2);
}


//send character to LCD
void lcd_send_char(uint8_t data)
{
    lcd_mode(LCD_MODE_DATA_WRITE);

    //end higher nibble of the data
    lcd_write_4_bits(data >> 4);
    //send lower nibble of the data
    lcd_write_4_bits(data & 0x0F);
}


//send string to LCD
void lcd_send_string(char* msg)
{
    do
    {
        lcd_send_char((uint8_t)*(msg++));
    }while(*msg != '\0');
}


void lcd_init(uint8_t interface_type, uint8_t display_lines, uint8_t font, uint8_t display, uint8_t cursor, uint8_t blink, uint8_t cursor_direction, uint8_t display_shift)
{
    //Initialize LCD GPIO Pins
    lcd_gpio_init();

    lcd_pins_reset(interface_type);

    //wait for more than 40ms
    tim_delay_ms(50);

    //Set RS & RW to LOW - Command Mode
    lcd_mode(LCD_MODE_COMMAND);

    //Function set
    lcd_write_4_bits(0x3);
    //wait for more than 4ms
    tim_delay_ms(5);
    
    //Function set
    lcd_write_4_bits(0x3);
    //wait for more than 100us
    tim_delay_us(150);

    lcd_write_4_bits(0x3);
    lcd_write_4_bits(0x2);

    //Function Set
    //interface_type | display_lines | font
    //LCD_CMD3_DATA_LINES_4 | LCD_CMD3_DISPLAY_LINES_2 | LCD_CMD3_FONT_5x8
    lcd_send_command(interface_type | display_lines | font);
    //wait for more than 39us
    tim_delay_us(50);

    //Display OF/OFF Control
    //display | cursor | blink
    //LCD_CMD1_DISPLAY_ON
    lcd_send_command(display | cursor | blink);
    //wait for more than 37us
    tim_delay_us(50);

    //display clear
    lcd_display_clear();

    //entry mode set
    //LCD_CMD4_CURSOR_INCREMENT
    //cursor_direction | display_shift
    lcd_send_command(cursor_direction | display_shift);
    //wait for more than 37us
    tim_delay_us(50);
}

/*need to test
static void lcd_check_busy_flag()
{
    //Set R/W pin to Input (for read operation)
    lcd.pGPIOx = LCD_GPIO_PORT;
    lcd.GPIOx_PinConfig.PinMode        = GPIO_MODE_IP;
    lcd.GPIOx_PinConfig.PinConfigType  = GPIO_CONFIG_PD;
    lcd.GPIOx_PinConfig.PinOutputSpeed = GPIO_OP_SPEED_0;
    lcd.GPIOx_PinConfig.PinNo = LCD_GPIO_RW;
    GPIO_Init(&lcd);

    //wait till busy flag becomes 0
    while(GPIO_ReadIpPin(LCD_GPIO_PORT, LCD_GPIO_RW));
}
*/


//set cursor position in LCD
void lcd_set_cursor(uint8_t row, uint8_t column)
{
    column--;
    switch(row)
    {
        case 1: 
            lcd_send_command(LCD_CMD_SET_DDRAM_ADDR | LCD_DDRAM_LINE1_START_ADDR | column);
            break;
        case 2:
            lcd_send_command(LCD_CMD_SET_DDRAM_ADDR | LCD_DDRAM_LINE2_START_ADDR | column);
            break;
        default:
            break;
    }
}


void lcd_print(char* msg, uint8_t line_no)
{
    lcd_set_cursor(line_no,1);
    lcd_send_string(msg);
}


void lcd_print_blink(char* msg, uint16_t delay_ms)
{
    lcd_send_string(msg);
    lcd_display_return_home();
    tim_delay_ms(delay_ms);
    lcd_display_clear();
    tim_delay_ms(delay_ms);
}


void lcd_print_slide(char* msg, uint16_t delay_ms)
{
    char* temp = msg;
    do
    {
        lcd_print_blink(temp, delay_ms);
        temp++;

        if(*temp=='\0')
            temp=msg;

    }while(1);
}


void lcd_print_slide_fwd(char* msg, uint16_t delay_ms)
{
    uint8_t i=1;
    do
    {
        lcd_set_cursor(1, i++);
        lcd_print_blink(msg, delay_ms);
        i=(i>16)?(i%16):i;

    }while(1);
}


void lcd_print_slide_back(char* msg, uint16_t delay_ms)
{
    uint8_t i=16;
    uint8_t cursor_flag=0;
    char* temp = msg;
    do
    {
        lcd_print_blink(temp, delay_ms);

        if(cursor_flag==1 && i>=2)
            lcd_set_cursor(1, --i);
        else if(cursor_flag==0)
            temp++;

        if(i==1)
        {
            cursor_flag=0;
            i=16;
        }

        if(*temp=='\0')
        {   cursor_flag=1;
            temp=msg;
            lcd_set_cursor(1, i);
        }
    }while(1);
}
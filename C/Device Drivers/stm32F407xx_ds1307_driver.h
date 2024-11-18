#ifndef DS1307_H
#define DS1307_H

#include<stdint.h>
#include"stm32F407xx.h"
#include"stm32F407xx_gpio_driver.h"
#include"stm32F407xx_i2c_driver.h"



//Macros
#define DS1307_I2C                  I2C1
#define DS1307_I2C_GPIO_PORT        GPIOB
#define DS1307_I2C_SDA_PIN          GPIO_PIN7
#define DS1307_I2C_SCL_PIN          GPIO_PIN6
#define DS1307_I2C_SPEED            I2C_SCL_SPEED_SM
#define DS1307_I2C_PUPD             GPIO_NO_PUPD


#define DS1307_ADD_SEC              0x00
#define DS1307_ADD_MIN              0x01
#define DS1307_ADD_HRS              0x02
#define DS1307_ADD_DAY              0x03
#define DS1307_ADD_DATE             0x04
#define DS1307_ADD_MONTH            0x05
#define DS1307_ADD_YEAR             0x06

#define TIME_FORMAT_12HRS_AM        0
#define TIME_FORMAT_12HRS_PM        1
#define TIME_FORMAT_24HRS           2


#define DS1307_I2C_ADDRESS          0x068

#define SUNDAY                      1
#define MONDAY                      2
#define TUESDAY                     3
#define WEDNESDAY                   4
#define THURSDAY                    5
#define FRIDAY                      6
#define SATURDAY                    7


typedef struct
{
    uint8_t date;
    uint8_t month;
    uint8_t year;
    uint8_t day;
}RTC_date;


typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t time_format;
}RTC_time;


//Function Delarations

uint8_t ds1307_init(void);

void ds1307_set_current_time();
void ds1307_get_current_time();

void ds1307_set_current_date();
void ds1307_get_current_date();



#endif /*DS1307_H*/


#include<stdio.h>
#include "stm32F407xx_ds1307_driver.h"
#include "stm32F407xx_16x2_lcd_driver.h"

#define SYSTICK_TIM_CLK   16000000UL

//Enable Macro to see time on LCD
//#define PRINT_LCD


static void delay(uint32_t cnt)
{
	for(uint32_t i=0 ; i < (cnt * 1000); i++);
}



//systick timer 
void init_systick_timer(uint32_t tick_hz)
{
	//register addresses
	uint32_t *pSRVR = (uint32_t*)0xE000E014;
	uint32_t *pSCSR = (uint32_t*)0xE000E010;

    //reload value (count) calculation
    uint32_t count_value = (SYSTICK_TIM_CLK/tick_hz)-1;

    //Clear SVR
    *pSRVR &= ~(0x00FFFFFFFF);

    //Load count value into SVR
    *pSRVR |= count_value;

    //configurations
    *pSCSR |= (1 << 1);  //Enable SysTick exception request:
    *pSCSR |= (1 << 2);  //Indicates the clock source, processor clock source

    //Enable SysTick
    *pSCSR |= (1 << 0); //enable the counter

}


//get day of the week
char* get_day_of_week(uint8_t i)
{
	char* days[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

	return days[i-1];
}


//convert number to string
void number_to_string(uint8_t num , char* buf)
{

	if(num < 10)
	{
		buf[0] = '0';    //tenths place
		buf[1] = num+48; //adding 48 to the number (0 in ASCII starts from 48)
	}
	else if(num >= 10 && num < 99)
	{
		buf[0] = (num/10) + 48;     //tenths place
		buf[1] = (num % 10) + 48;   //one's place
	}
}



//convert time to string (HH:MM:SS format)
char* time_to_string(RTC_time *rtc_time)
{
	static char buf[9];

	buf[2]= ':';
	buf[5]= ':';

    //convert hours, minutes & seconds to string
	number_to_string(rtc_time->hours,buf);       
	number_to_string(rtc_time->minutes,&buf[3]);
	number_to_string(rtc_time->seconds,&buf[6]);

	buf[8] = '\0';  //terminate the string

	return buf;     //return string address
}



//convert date to string (DD/MM/YY Format)
char* date_to_string(RTC_date *rtc_date)
{
	static char buf[9];

	buf[2]= '/';
	buf[5]= '/';

    //convert date, month & year to string
	number_to_string(rtc_date->date,buf);
	number_to_string(rtc_date->month,&buf[3]);
	number_to_string(rtc_date->year,&buf[6]);

	buf[8]= '\0';   //terminate the string

	return buf;     //return string address
}






int main(void)
{

	RTC_time current_time;
	RTC_date current_date;

    #ifndef PRINT_LCD
	    printf("RTC DS1307 Testing\n");
    #else
	    lcd_init();
	    lcd_send_string("RTC DS1307 Test");
	    delay(2000);

	    display_clear();
	    display_return_home();
    #endif

	if(ds1307_init())
    {
		printf("DS1307 initialization has failed\n");
		while(1);
	}

	init_systick_timer(1);

    //date values
	current_date.day = FRIDAY;
	current_date.date = 15;
	current_date.month = 1;
	current_date.year = 21;

    //time values
	current_time.hours = 11;
	current_time.minutes = 59;
	current_time.seconds = 30;
	current_time.time_format = TIME_FORMAT_12HRS_PM;

    //set date and time
	ds1307_set_current_date(&current_date);
	ds1307_set_current_time(&current_time);

    //get date and time
    ds1307_get_current_date(&current_date);
	ds1307_get_current_time(&current_time);

	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24HRS)
    {
		am_pm = (current_time.time_format)?"PM":"AM";
    #ifndef PRINT_LCD
		printf("Current time = %s %s\n",time_to_string(&current_time),am_pm); //
    #else
		lcd_send_string(time_to_string(&current_time));
		lcd_send_string(am_pm);
    #endif
	}
    else
    {
    #ifndef PRINT_LCD
		printf("Current time = %s\n",time_to_string(&current_time)); //
    #else
		lcd_send_string(time_to_string(&current_time));
    #endif
	}

    #ifndef PRINT_LCD
	printf("Current date = %s <%s>\n",date_to_string(&current_date), get_day_of_week(current_date.day));
    #else
	lcd_set_cursor(2, 1);
	lcd_send_string(date_to_string(&current_date));
    #endif

	while(1);
	return 0;
}


void SysTick_Handler(void)
{
	RTC_time current_time;
	RTC_date current_date;

	ds1307_get_current_time(&current_time);

	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24HRS)
    {
		am_pm = (current_time.time_format) ? "PM" : "AM";
    #ifndef PRINT_LCD
		printf("Current time = %s %s\n",time_to_string(&current_time),am_pm); //
    #else
		lcd_set_cursor(1, 1);
		lcd_send_string(time_to_string(&current_time));
		lcd_send_string(am_pm);
    #endif

	}
    else
    {
    #ifndef PRINT_LCD
		printf("Current time = %s\n",time_to_string(&current_time)); //
    #else
		lcd_set_cursor(1, 1);
		lcd_send_string(time_to_string(&current_time));
    #endif
	}

	ds1307_get_current_date(&current_date);

    #ifndef PRINT_LCD
	printf("Current date = %s <%s>\n",date_to_string(&current_date), get_day_of_week(current_date.day));
    #else
	lcd_set_cursor(2, 1);
	lcd_send_string(date_to_string(&current_date));
	lcd_send_char('<');
	lcd_send_string(get_day_of_week(current_date.day));
	lcd_send_char('>');
    #endif
}
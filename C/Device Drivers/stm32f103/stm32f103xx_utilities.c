#include"stm32f103xx_utilities.h"
#include"stm32f103xx_gpio.h"
#include"stm32f103xx_usart.h"
#include"stm32f103xx_memory_map.h"

static USART_Handle U2;
static GPIO_Handle GA;

void delay(int d)
{
    for(int i=0; i<65536;i++)
    {
        for(int j=0; j<d; j++);
    }
}

void wait_ms(uint16_t d)
{
	for(int i=0; i<(d*350); i++);
}

void wait(uint8_t d)
{
	wait_ms(d*1000);
}

uint32_t get_size(char* data)
{
    uint32_t c=0;
    while(*data++ != '\0')
        c++;
    return c;
}

void string_concat(char* s1, char* s2)
{
    //traverse to end of s1
    while(*s1++ !='\0');
    while(*s2 != '\0')
        *s1++ = *s2++;
    *s2='\0';
}


void Serialprint_init()
{
    //initializing USART2 with default configuration
    USART_init(&U2, &GA, USART2);
}

void Serialprint(char* data)
{
    USART_TX(&U2, data, get_size(data));
    delay(10);
}
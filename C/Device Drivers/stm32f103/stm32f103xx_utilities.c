#include"stm32f103xx_utilities.h"

void wait_ms(uint16_t d)
{
	for(int i=0; i<(d*350); i++);
}

void wait(uint8_t d)
{
	wait_ms(d*1000);
}

uint32_t string_size(char* data)
{
    uint32_t c=0;
    while(*data++ != '\0')
        c++;
    return c;
}

void string_concat(char* s1, char* s2)
{
    //traverse to end of s1
    while(*(s1++) !='\0');
    s1--;
    
    while(*s2 != '\0')
    {
        *(s1++) = *(s2++);
    }
    *s1='\0';
}


int32_t string_to_int(char* str)
{
    int8_t sign = 1;
    int32_t result = 0;

    if(*str == '-')
    {
        sign = -1;
        str++;
    }

    for(;*str != '\0'; str++)
    {
        if(*str >= '0' && *str <= '9')
            result = (result*10) + (*str - '0');
        else
            return 1;
    }

    return (sign * result);
}


uint32_t power(uint8_t num, uint8_t power)
{
    uint32_t result=1;
    while(power!=0)
    {
        result = result*num;
        power--;
    }

    return result;
}
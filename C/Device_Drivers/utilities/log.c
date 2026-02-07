#include"log.h"
#include"stm32f103xx_serial.h"
#include"stm32f103xx_utilities.h"

#include<stdarg.h>

void logINFO(char* format, ...)
{
    va_list args;
    va_start(args, format);
    vSerialprintln(format, INFO, args); 
    va_end(args);
}



void logWARN(char* format, ...)
{
    va_list args;
    va_start(args, format);
    vSerialprintln(format, WARN, args); 
    va_end(args);
}



void logDEBUG(char* format, ...)
{
    va_list args;
    va_start(args, format);
    vSerialprintln(format, DEBUG, args); 
    va_end(args);
}

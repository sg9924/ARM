#include "string_util.h"
#include <stdint.h>


//String Length
size_t xstr_len(const char* str)
{
    if(str == NULL) return 0;

    size_t len = 0;
    while(*str != EOS)
    {
        len++;
        str++;
    }
    return len;
}
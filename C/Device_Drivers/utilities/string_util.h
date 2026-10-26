#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <stddef.h>
#include <stdint.h>

//#define NULL                       (void*)0
#define EOS                        '\0'
#define SPACE                      ' '
#define TAB                        '\t'
#define NEWLINE                    '\n'
#define CARRIAGE_RETURN            '\r'
#define FORMFEED                   '\f'
#define VERTICAL_TAB               '\v'

#define TRUNCATE_LEFT               0
#define TRUNCATE_RIGHT              1

size_t xstr_len(const char* str);

#endif
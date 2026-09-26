#ifndef INC_STM32F103XX_SERIAL_H
#define INC_STM32F103XX_SERIAL_H

#include "stm32f103xx_gpio.h"
#include "stm32f103xx_usart.h"
#include "stm32f103xx.h"
#include "../../utilities/stm32f103xx_utilities.h"
#include "../../utilities/serial_ansi.h"

#include <stdarg.h>

#define FLOAT_PRECISION_MAX            6

#define BUFF_SIZE_BYTES                1024
#define SERIAL_DELAY                   0

#define SERIAL_NL()                    (Serialprintln("", NONE))

//Serial Input Data Type Formats
#define SERIAL_RX_STRING               0
#define SERIAL_RX_INT                  1
#define SERIAL_RX_CHAR                 2

//Serial Message Type
#define NONE                           0
#define INFO                           1
#define DEBUG                          2
#define WARN                           3
#define ERROR                          4
#define FATAL                          5
#define ASSERT                         6

#define DEBUG_COLOR                    ""
#define INFO_COLOR                     COLOR_GREEN
#define WARN_COLOR                     COLOR_YELLOW
#define ERROR_COLOR                    COLOR_RED
#define FATAL_COLOR                    COLOR_RED
#define ASSERT_COLOR                   ""

#define SERIAL_DEBUG_STRING            (DEBUG_COLOR  "[DEBUG]: "  COLOR_RESET)
#define SERIAL_INFO_STRING             (INFO_COLOR   "[INFO]: "   COLOR_RESET)
#define SERIAL_WARN_STRING             (WARN_COLOR   "[WARN]: "   COLOR_RESET)
#define SERIAL_ERROR_STRING            (ERROR_COLOR  "[ERROR]: "  COLOR_RESET)
#define SERIAL_FATAL_STRING            (FATAL_COLOR  "[FATAL]: "  COLOR_RESET)
#define SERIAL_ASSERT_STRING           (ASSERT_COLOR "[ASSERT]: " COLOR_RESET)

#define SERIAL_NL_STRING               ("\r\n")

#define GET_LOG_LEVEL(msg_type)        msg_type_levels[msg_type]
#define SET_LOG_LEVEL(level)           {                               \
                                        if (level <= LOG_LEVEL_ASSERT) \
                                        {current_log_level = level;}   \
                                        }
#define GET_CURRENT_LOG_LEVEL()        (current_log_level)



typedef enum {
    LOG_LEVEL_NONE   = NONE,
    LOG_LEVEL_DEBUG  = DEBUG,
    LOG_LEVEL_INFO   = INFO,
    LOG_LEVEL_WARN   = WARN,
    LOG_LEVEL_ERROR  = ERROR,
    LOG_LEVEL_FATAL  = FATAL,
    LOG_LEVEL_ASSERT = ASSERT
} LogLevel_t;

static uint8_t current_log_level = LOG_LEVEL_NONE;


void _print_buffer(char* buffer, uint32_t* buff_ind);
void _reset_buffer(uint32_t* buff_ind);
void _print_int(int32_t value, char* buffer, uint32_t* buff_ind);
void _print_uint(uint32_t value, char* buffer, uint32_t* buff_ind);
void _print_hex(int32_t value, char* buffer, uint32_t* buff_ind);
void _print_float(double value, char* buffer, uint32_t* buff_ind, uint8_t precision);

void Serial_UART_init(uint32_t baud_rate);
void Serial_init();

uint32_t Serialprint(const char *format, uint8_t msg_type, ...);
uint32_t Serialprintln(const char *format, uint8_t msg_type, ...);
void SerialInput(char* msg, uint8_t datatype, void* d);
//void SerialInput(char* msg, char* format, ...);
uint32_t vSerialprintln(char *format, uint8_t msg_type, va_list args);


#endif
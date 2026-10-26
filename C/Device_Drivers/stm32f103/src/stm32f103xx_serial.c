#include"../inc/stm32f103xx_serial.h"
#include"../../utilities/stm32f103xx_utilities.h"
#include"../../utilities/string_util.h"
#include"../inc/stm32f103xx_config.h"

#include <stdlib.h>

#define ABS(x)        (x>=0 ? x : -x)

#define _UPDATE_CHAR_COUNT(buff_idx)               (char_count += buff_idx)

USART_Handle U2;
uint8_t      char_count;     //Initialize a counter for the number of characters printed

//Buffer
static GPIO_Handle GA;
static uint32_t    buff_ind;
static char        buffer[BUFF_SIZE_BYTES];
static bool        serial_flag = true;

//Message Types
static char*       msg_type_list[10] =
{
    [NONE]   = "",
    [INFO]   = SERIAL_INFO_STRING,
    [DEBUG]  = SERIAL_DEBUG_STRING,
    [WARN]   = SERIAL_WARN_STRING,
    [FATAL]  = SERIAL_FATAL_STRING,
    [ERROR]  = SERIAL_ERROR_STRING,
    [ASSERT] = SERIAL_ASSERT_STRING
};

//Log Levels
static const uint8_t msg_type_levels[] =
{
    [NONE]   = LOG_LEVEL_NONE,
    [DEBUG]  = LOG_LEVEL_DEBUG,
    [INFO]   = LOG_LEVEL_INFO,
    [WARN]   = LOG_LEVEL_WARN,
    [ERROR]  = LOG_LEVEL_ERROR,
    [FATAL]  = LOG_LEVEL_FATAL,
    [ASSERT] = LOG_LEVEL_ASSERT
};


//Serial Output Function Pointer
typedef void (*serial_output_fn)(const char* data, uint32_t length);


typedef struct
{
    serial_output_fn handler;
    uint8_t          enabled;
} SerialOutput_t;

SerialOutput_t outputs[MAX_SERIAL_OUTPUTS] =
{
    {.handler = serial_output_uart, .enabled = 1}
    /* more to follow: LCD, OLED, SD Card*/
};




#if SERIAL_USART_TYPE == USART_TYPE_IT
    void USART2_IRQHandler(void)
    {
        USART_IRQ_Handler(&U2);
    }
#endif




void serial_output_uart(const char* data, uint32_t length)
{
    #if SERIAL_USART_TYPE == USART_TYPE_IT
    USART_TX_IT(&U2, (uint8_t*)buffer, length);
    //while(U2.TXState != USART_READY);
    #elif SERIAL_USART_TYPE == USART_TYPE_DEFAULT
    USART_TX(&U2, (uint8_t*)buffer, length);
    #endif
}



void _print_buffer(char* buffer, uint32_t* buff_ind)
{
    if(*buff_ind > 0)
    {
        //Route to enabled Serial outputs
        for (uint8_t i = 0; i < MAX_SERIAL_OUTPUTS; i++)
        {
            if (outputs[i].enabled == 1 && outputs[i].handler != NULL)
                outputs[i].handler((char*)buffer, (*buff_ind));
        }
    }
}


void _reset_buffer(uint32_t* buff_ind)
{
    *buff_ind = 0;
}


static void _enable_serial_output()
{
    serial_flag = true;
}


static void _disable_serial_output()
{
    serial_flag = false;
}


static uint32_t _get_remaining_bytes()
{
    return (BUFF_SIZE_BYTES - buff_ind);
}


static void _flush_buffer()
{
    if(serial_flag)
    {
        _print_buffer(buffer, &buff_ind);
        _UPDATE_CHAR_COUNT(buff_ind);
        _reset_buffer(&buff_ind);
    }
    else
        _UPDATE_CHAR_COUNT(buff_ind);
}


static void _flush_buffer_preemptive(uint32_t required_bytes)
{
    if (_get_remaining_bytes() < required_bytes)
        _flush_buffer();
}


static void _buffer_write_char(char ch)
{
    buffer[buff_ind++] = ch;

    //If Buffer is full
    //Flush it
    if (buff_ind == BUFF_SIZE_BYTES)
        _flush_buffer();
}


static void _buffer_write_str(char* ch)
{
    while (*ch)  //Iterate over each character in the string
    {
        buffer[buff_ind++] = *ch;
        ch++;
        if (buff_ind == BUFF_SIZE_BYTES)
            _flush_buffer();
    }
}



static uint32_t _uintTostr(uint32_t value, char* buffer)
{
    uint8_t  i   = 0;
    uint32_t len = 0;
    char     int_buff[12];

    do
    {
        int_buff[i++] = '0' + (value % 10);
        value /= 10;
    } while(value > 0);

    while(i > 0)
        buffer[len++] = int_buff[--i];

    buffer[len] = EOS;
    return len;
}



static uint32_t _intTostr(int32_t value, char* buffer)
{
    int8_t   i = 0;   //!!!don't change datatype to uint!!!
    uint32_t len = 0;
    char     int_buff[12];
    uint32_t mag;
    
    mag = (value < 0) ? ((uint32_t)0 - (uint32_t)value) : (uint32_t)value;

    if(value < 0)
    {
        buffer[len++] = '-';
        buffer++;
    }
    
    len += _uintTostr(mag, buffer);

    return len;
}



static uint32_t _floatTostr(double value, char* buffer, int8_t precision)
{
    uint32_t len = 0;
    //Handle sign
    if(value < 0)
    {
        buffer[len++] = '-';
        value = -value;
    }
 
    //Extract parts
    int32_t integral = (int32_t)value;
    float frac       = value - integral;
        
    uint32_t scale = 1;
    for(uint8_t j = 0; j < precision; j++)
        scale *= 10;

    //Round fractional part to 'precision' digits
    uint32_t frac_part = (uint32_t)(frac * scale + 0.5);

    //Carry into integer part (e.g. 0.999 -> 1.00)
    if(frac_part >= scale)
    {
        frac_part = 0;
        integral++;
    }
 
    //integer part
    len += _uintTostr(integral, buffer);
 
    //decimal part
    //write right to left so leading zeros are kept
    if (precision > 0)
    {
        buffer[len++] = '.';
        for(uint8_t p = precision; p > 0; p--)
        {
            buffer[len + p - 1] = '0' + (frac_part % 10);
            frac_part /= 10;
        }
        len += precision;
    }

    buffer[len] = EOS;
    return len;
}




static uint32_t _floatToExponentStr(double value, char* buffer, int8_t precision)
{
    uint32_t len = 0;

    //Handle sign
    if(value < 0)
    {
        buffer[len++] = '-';
        value = -value;
    }

    int16_t exp = 0;

    if(value >= 1)
    {
        while(value >= 10)
        {
            value /= 10;
            exp++;
        }
    }
    else if(value < 1 && value > 0)
    {
        while(value < 1)
        {
            value *= 10;
            exp--;
        }
    }

    //Mantissa Rounding
    char mant[32];
    _floatTostr(value, mant, precision);
    if(mant[1] != '.' && mant[0] == '1' && mant[1] == '0')
    {
        //mantissa "10.000" becomes "1.000" with exp + 1
        value /= 10;
        exp++;
    }

    len += _floatTostr(value, buffer+len, precision);

    buffer[len++] = 'e';
    buffer[len++] = (exp >= 0) ? '+' : '-';

    int16_t absexp = ABS(exp);
    
    if(absexp < 10)
        buffer[len++] = '0';
    
    len += _uintTostr(absexp, buffer + len);

    buffer[len++] = EOS;
    return len;
    
}




static uint32_t _intTohexstr(uint32_t value, char* buffer)
{
    uint8_t  i   = 0;
    uint32_t len = 0;
    char     hex_buff[8];

    //Extract hex digits in reverse
    do
    {
        hex_buff[i++] = "0123456789ABCDEF"[value & 0xF];
        value >>= 4;
    } while(value > 0);

    //Prefix
    buffer[len++] = '0';
    buffer[len++] = 'x';

    //Digits in correct order
    while(i > 0)
        buffer[len++] = hex_buff[--i];

    buffer[len] = EOS;
    return len;
}



static FormatSpec parse_format_spec(const char** format)
{
    FormatSpec spec = 
    {
        .left_align      = 0,
        .zero_pad        = 0,
        .length_modifier = 0,
        .width           = -1,
        .precision       = -1
    };
 
    if(format == NULL || *format == NULL)
        return spec;
 
    //1. Parse flags
    //'-' for left alignment
    if(**format == '-')
    {
        spec.left_align = 1;
        (*format)++;
    }
    //'0' for zero padding
    else if (**format == '0')
    {
        spec.zero_pad = 1;
        (*format)++;
    }
 
    //2. Parse width (sequence of digits)
    if(**format >= '0' && **format <= '9')
    {
        spec.width = 0;
        while (**format >= '0' && **format <= '9')
        {
            spec.width = spec.width * 10 + (**format - '0');
            (*format)++;
        }
    }
 
    //3. Parse precision (.digits)
    if(**format == '.')
    {
        (*format)++;  //skip the '.'
        spec.precision = 0;
        
        //Parse precision digits
        while (**format >= '0' && **format <= '9')
        {
            spec.precision = spec.precision * 10 + (**format - '0');
            (*format)++;
        }
    }

    //4. length modifier
    //ignore for STM32 bluepill
    if (**format == 'l')
    (*format)++;
 
    return spec;
}



static void _write_formatted(const char *str, uint32_t str_len, uint32_t prefix_len, FormatSpec spec)
{
    int32_t pad = spec.width - (int32_t)str_len;

    //Right padding (left-aligned output)
    if(spec.left_align == 1)
    {
        for(int32_t i = 0; i < str_len; i++)
            _buffer_write_char(str[i]);
        for(int32_t p = 0; p < pad; p++)
            _buffer_write_char(' ');
        
        return;
    }

    //zero padding
    if(spec.zero_pad == 1)
    {
        //Prefix (sign or 0x) first
        for(int32_t i = 0; i < prefix_len; i++)
            _buffer_write_char(str[i]);
        //zero padding
        for(int32_t p = 0; p < pad; p++)
            _buffer_write_char('0');
        //data string
        for(int32_t i = prefix_len; i < str_len; i++)
            _buffer_write_char(str[i]);

        return;
    }

    //Left padding (right-aligned output)
    //zero padding is ignored
    //print spaces
    if(spec.left_align == 0)
    {
        //left space padding
        for(int32_t i = 0; i < pad; i++)
            _buffer_write_char(' ');
        //data string
        for(uint32_t i = 0; i < str_len; i++)
            _buffer_write_char(str[i]);
    }
}


uint32_t _string_format(const char *format, uint8_t msg_type, va_list args, uint8_t add_newline)
{
    char_count = 0;

    if(add_newline)
    {
        _buffer_write_str("\r\n");
    }
    
    //Log Type
    _buffer_write_str(msg_type_list[msg_type]);


    while (*format != '\0')                              //Iterate over each character in the format string
    {
        if (*format == '%')                              //Check for the start of a conversion specifier
        {
            format++;                                    //Move to the next character after '%'

            //Parse the format specifications: %[flags][width][.precision][specifier]
            FormatSpec spec = parse_format_spec(&format);

            switch (*format)
            {
                case '%':  //'%%' prints a single '%'
                {
                    _buffer_write_char('%');
                    break;
                }

                case 'c':  //'%c' prints a character
                {
                    char ch = (char)va_arg(args, int);
                    spec.zero_pad = 0;
                    _write_formatted(&ch, 1, 0, spec);
                    break;
                }

                case 's':  //'%s' prints a string (with optional width/precision)
                {
                    char    *str     = va_arg(args, char *);

                    if(str == NULL)
                        str = "(null)";

                    uint32_t str_len = xstr_len(str);

                    //Truncation: if precision is set, limit string length
                    if(spec.precision >= 0 && spec.precision < str_len)
                        str_len = spec.precision;

                    spec.zero_pad = 0;
                    _write_formatted(str, str_len, 0, spec);
                    break;
                }

                case 'i':  //'%i' (alias for %d)
                case 'd':  //'%d' : signed int
                {
                    int  value = va_arg(args, int);
                    char num_str[33];
                    uint32_t len = _intTostr(value, num_str);
                    
                    _write_formatted(num_str, len, (value < 0), spec);
                    break;
                }
 
                case 'u':  //'%u' prints an unsigned integer
                {
                    unsigned int value = va_arg(args, unsigned int);
                    char     num_str[33];
                    uint32_t len = _uintTostr(value, num_str);

                    _write_formatted(num_str, len, 0, spec);
                    break;
                }
 
                case 'X':
                case 'x':  //'%x' prints hexadecimal
                {
                    uint32_t value = va_arg(args, unsigned int);
                    char     num_str[12];
                    uint32_t len = _intTohexstr(value, num_str);

                    _write_formatted(num_str, len, 2, spec);
                    break;
                }
 
                case 'f':  //'%f': float
                {
                    double value = va_arg(args, double);
                    char   num_str[32];
                    int    float_precision = (spec.precision >= 0) ? spec.precision : FLOAT_PRECISION_MAX;
                    uint32_t len = _floatTostr(value, num_str, float_precision);

                    _write_formatted(num_str, xstr_len(num_str), (value < 0), spec);
                    break;
                }

                case 'e':
                {
                    double value = va_arg(args, double);
                    char   num_str[32];
                    int    float_precision = (spec.precision >= 0) ? spec.precision : FLOAT_PRECISION_MAX;
                    uint32_t len = _floatToExponentStr(value, num_str, float_precision);

                    _write_formatted(num_str, xstr_len(num_str), (value < 0), spec);
                    break;

                }
 
                default:  //Unknown specifier - just skip it
                {
                    break;
                }
            }
            
        }
        else //Regular character, not a conversion specifier
            _buffer_write_char(*format);

        format++; //Move to the next character in the format string
    }

    _flush_buffer();

    va_end(args); //Clean up the argument list
    return char_count; //Return the number of characters printed
}




void Serial_UART_init(uint8_t usart_type, uint32_t baud_rate)
{
    USART_Configure(&U2, USART_MODE_TXRX, baud_rate, USART_CPHA_DEFAULT, USART_CPOL_DEFAULT, USART_WORD_8BIT, USART_PARITY_DISABLE, USART_PARITY_NONE, USART_STOPBIT_DEFAULT);
    USART_init(&U2, &GA, USART2);

    if(usart_type == USART_TYPE_IT)
        USART_IT_Config(&U2, ENABLE);
}



void Serial_init()
{
    buff_ind = 0;
    Serial_UART_init(SERIAL_USART_TYPE, SERIAL_BAUD_RATE);
    SERIAL_NL();
    Serialprint("Serial Communication has been initialized.", INFO);
    SERIAL_NL();
}




uint32_t Serialprint(const char *format, uint8_t msg_type, ...)
{
    va_list args;                 //initialize the list pointer
    va_start(args, msg_type);     //Initialize the argument list

    if (GET_LOG_LEVEL(msg_type) < current_log_level)
    {
        va_end(args);
        return 0;
    }

    uint32_t result = _string_format(format, msg_type, args, 0); //add new line is false
    
    va_end(args);
    return result;
}



uint32_t Serialprintln(const char *format, uint8_t msg_type, ...)
{
    va_list args;                 //initialize the list pointer
    va_start(args, msg_type);     //Initialize the argument list

    if (GET_LOG_LEVEL(msg_type) < current_log_level)
    {
        va_end(args);
        return 0;
    }

    uint32_t result = _string_format(format, msg_type, args, 1);  //add new line is true
    
    va_end(args);
    return result;
}



char* Stringformat(const char *format, uint8_t msg_type, bool new_line, ...)
{
    _disable_serial_output();      //disable UART
    va_list args;                 //initialize the list pointer
    va_start(args, new_line);     //Initialize the argument list

    buff_ind = 0;                 //Reset buffer index
    uint32_t result = _string_format(format, msg_type, args, new_line);
    _buffer_write_char('\0');     //mark end of the string
    
    va_end(args);
    _enable_serial_output();       //enable UART

    return buffer;
}



void SerialInput(char* msg, uint8_t datatype, void* var)
{
    Serialprint(msg, NONE);

    if(datatype == SERIAL_RX_STRING)
    {
        char* a = (char*)var;
        uint8_t b_ind = 0;
        while(1)
        {
            USART_RX(&U2, (uint8_t*)a, 1);
            USART_TX(&U2, (uint8_t*)a, 1);
            b_ind++;
            if(*a == '\r')
                break;
            a++;
        }
        *a = '\0';
    }
    else if(datatype == SERIAL_RX_CHAR)
    {
        char* a = (char*)var;
        while(1)
        {
            USART_RX(&U2, (uint8_t*)a, 1);
            USART_TX(&U2, (uint8_t*)a, 1);
            break;
        }
    }
    else if(datatype == SERIAL_RX_INT)
    {
        char rx_buffer[33];
        char* a = rx_buffer;
        uint8_t b_ind = 0;
        while(1)
        {
            USART_RX(&U2, (uint8_t*)a, 1);
            USART_TX(&U2, (uint8_t*)a, 1);
            b_ind++;
            if(*a == '\r')
                break;
            a++;
        }
        *a = '\0';

        *((int32_t*)var) = string_to_int(rx_buffer);
    }
}



uint32_t vSerialprintln(char *format, uint8_t msg_type, va_list args)
{
    uint32_t result = _string_format(format, msg_type, args, 1);  //add new line is true
    return result;
}



/*
void SerialInput(char* msg, char* format, ...)
{
    Serialprint(msg);

    va_list args;                 //initializing list pointer 
    va_start(args, format);       //Initialize the argument list

    while (*format != '\0')       //Iterate over each character in the format string
    {
        if (*format == '%')       //Check for the start of a conversion specifier
        {
            format++;             //Move to the next character after '%'
            if (*format == 'c')   //Input is a character
            {
                char* ch = va_arg(args, char*);
                USART_RX(&U2, (uint8_t*)ch, 1);
                USART_TX(&U2, (uint8_t*)ch, 1);
            }
            else if (*format == 's')                //Input is a string
            {
                char* str = va_arg(args, char*);    //Fetch the next argument as char*
                while(1)
                {
                    USART_RX(&U2, (uint8_t*)str, 1);
                    USART_TX(&U2, (uint8_t*)str, 1);
                    if(*str == '\r')
                        break;
                    str++;
                }
                *(--str) = '\0';
            }
        format++; //Move to the next character in the format string
        }
    }
}
*/
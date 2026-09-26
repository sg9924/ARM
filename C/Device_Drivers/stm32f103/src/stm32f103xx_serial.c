#include"../inc/stm32f103xx_serial.h"
#include"../../utilities/stm32f103xx_utilities.h"
#include"../inc/stm32f103xx_config.h"

#define _UPDATE_CHAR_COUNT(buff_idx)               (char_count += buff_idx)

USART_Handle U2;
uint8_t      char_count;     //Initialize a counter for the number of characters printed

//Buffer
static GPIO_Handle GA;
static uint32_t    buff_ind;
static char        buffer[BUFF_SIZE_BYTES];
static bool        serial_flag = true;
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


void _print_buffer(char* buffer, uint32_t* buff_ind)
{
    if(*buff_ind > 0)
        USART_TX(&U2, (uint8_t*)buffer, (*buff_ind));
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




void _print_int(int32_t value, char* buffer, uint32_t* buff_ind)
{
    int8_t i=0;   //!!!don't change datatype to uint!!!
    char int_buff[32];

    //negative integer
    if(value < 0)
    {
        _buffer_write_char('-');
        value = -value; //making the integer value positive
    }
    else if(value == 0)
    {
        _buffer_write_char('0');
        return;
    }

    // extract digits of integer in reverse
    while(value>0)
    {
        int_buff[i++] = '0' + (value%10);
        value/=10;
    }
    i--;

    // store the digits in correct order
    while(i>=0)
        _buffer_write_char(int_buff[i--]);
}


void _print_uint(uint32_t value, char* buffer, uint32_t* buff_ind)
{
    int8_t i=0;   //!!!don't change datatype to uint!!!
    char uint_buff[32];

    if(value == 0)
    {
        _buffer_write_char('0');
        return;
    }

    // extract digits of integer in reverse
    while(value>0)
    {
        uint_buff[i++] = '0' + (value%10);
        value/=10;
    }
    i--;

    //store the digits in correct order
    while(i>=0)
        _buffer_write_char(uint_buff[i--]);
}



void _print_float(double value, char* buffer, uint32_t* buff_ind, uint8_t precision)
{
    int32_t integral;
    int8_t i=0;       //!!!don't change datatype to uint!!!
    char buff[32];

    if(value>0)
        integral = value;
    else if(value<0)
        integral=0;
    
    float fractional = (float)value - integral;

    _print_int(integral, buffer, buff_ind);

    if(precision!=0)
    {
        buffer[(*buff_ind)++] = '.';

        while(precision>0)
        {
            fractional = fractional * 10;
            if(((uint32_t)fractional)%10 == 0)
                buffer[(*buff_ind)++] = '0';
            precision--;
        }

        
        uint32_t f = (uint32_t)fractional;

        // extract digits of integer in reverse
        while(f>0)
        {
            buff[i++] = '0' + (f%10);
            f/=10;
        }
        i--;

        // store the digits in correct order
        while(i>=0)
        {
            buffer[(*buff_ind)++] = buff[i--];
            if (*buff_ind == BUFF_SIZE_BYTES)
            {
                _print_buffer(buffer, buff_ind);
                char_count += (*buff_ind);
                _reset_buffer(buff_ind);
            }
        }
        char_count += (*buff_ind);
        
    }
}


void _print_hex(int32_t value, char* buffer, uint32_t* buff_ind)
{
    int i=0;
    char hex_buff[32];

    //negative integer
    
    if(value < 0)
    {
        _buffer_write_char('-');
        value = -value; //making the integer value positive
    }

    _buffer_write_str("0x");

    if(value == 0)
    {
        _buffer_write_char('0');
        return;
    }
    else
    {
        //extract digits of integer in reverse in hexadecimal
        while(value > 0)
        {
            hex_buff[i++] = "0123456789ABCDEF"[value % 16];
            value = value/16;
        }
        i--;

        //store the digits in correct order
        while(i >= 0)
            _buffer_write_char(hex_buff[i--]);
    }
}



void Serial_UART_init(uint32_t baud_rate)
{
    USART_Configure(&U2, USART_MODE_TXRX, baud_rate, USART_CPHA_DEFAULT, USART_CPOL_DEFAULT, USART_WORD_8BIT, USART_PARITY_DISABLE, USART_PARITY_NONE, USART_STOPBIT_DEFAULT);
    USART_init(&U2, &GA, USART2);
}


void Serial_init()
{
    buff_ind = 0;
    Serial_UART_init(SERIAL_BAUD_RATE);
    SERIAL_NL();
    Serialprint("Serial Communication has been initialized.", INFO);
    SERIAL_NL();
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


    while (*format != '\0')                              // Iterate over each character in the format string
    {
        if (*format == '%')                              // Check for the start of a conversion specifier
        {
            format++;                                    // Move to the next character after '%'
            if (*format == '%')                          // Case: '%%' prints a single '%'
            {
                _buffer_write_char('%');
            }
            else if (*format == 'c')                     // Case: '%c' prints a character
            {
                int ch = va_arg(args, int);              // Fetch the next argument as int
                _buffer_write_char(ch);
            }
            else if (*format == 's')                     // Case: '%s' prints a string
            {
                char *str = va_arg(args, char *);        // Fetch the next argument as char*
                _buffer_write_str(str);
            }
            else if (*format == 'd' || *format == 'i')   // Case: '%d' or '%i' prints an integer
            {
                int value = va_arg(args, int);
                _flush_buffer_preemptive(13);            //32 bit int -> max 11 chars
                _print_int(value, buffer, &buff_ind);
            }
            else if (*format == 'u')                     // Case: '%u' prints an unsigned integer
            {
                int value = va_arg(args, unsigned int);
                _flush_buffer_preemptive(13);            //32 bit uint -> max 12 chars
                _print_uint(value, buffer, &buff_ind);
            }
            else if(*format == 'x')
            {
                unsigned int value = va_arg(args, unsigned int);
                _flush_buffer_preemptive(14);            //32 bit uint -> max 12 chars
                _print_hex(value, buffer, &buff_ind);
            }
            //NOT WORKING
            //TOCHECK
            else if(*format == '.' && *(format+2) == 'f')
            {
                double value = va_arg(args, double);
                _print_float(value, buffer, &buff_ind, *(++format) - '0');
                ++format;
            }
            //NOT WORKING
            //TOCHECK
            else if(*format == 'f')
            {
                double value = va_arg(args, double);
                _flush_buffer_preemptive(50);            //float: 39 integer + 6-9 decimal 
                _print_float(value, buffer, &buff_ind, FLOAT_PRECISION_MAX);
            }
        }
        else // Case: Regular character, not a conversion specifier
            _buffer_write_char(*format);

        format++; // Move to the next character in the format string
    }

    _flush_buffer();

    va_end(args); // Clean up the argument list
    return char_count; // Return the number of characters printed
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
    va_list args;                 // initialize the list pointer
    va_start(args, msg_type);     // Initialize the argument list

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

    va_list args;                 // initializing list pointer 
    va_start(args, format);       // Initialize the argument list

    while (*format != '\0')       // Iterate over each character in the format string
    {
        if (*format == '%')       // Check for the start of a conversion specifier
        {
            format++;             // Move to the next character after '%'
            if (*format == 'c')   // Case: Input is a character
            {
                char* ch = va_arg(args, char*);
                USART_RX(&U2, (uint8_t*)ch, 1);
                USART_TX(&U2, (uint8_t*)ch, 1);
            }
            else if (*format == 's')                // Case: Input is a string
            {
                char* str = va_arg(args, char*);    // Fetch the next argument as char*
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
        format++; // Move to the next character in the format string
        }
    }
}
*/
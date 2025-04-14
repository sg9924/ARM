#include"stm32f103xx_serial.h"


static USART_Handle U2;
static GPIO_Handle GA;

uint8_t char_count;     // Initialize a counter for the number of characters printed

void _print_buffer(char* buffer, uint32_t* buff_ind)
{
    if(*buff_ind > 0)
    {
        USART_TX(&U2, (uint8_t*)buffer, (*buff_ind));
    }
}

void _reset_buffer(uint32_t* buff_ind)
{
    *buff_ind = 0;
}

void _print_int(uint32_t value, char* buffer, uint32_t* buff_ind)
{
    int i= 0;
    char buff[32];

    // extract digits of integer in reverse
    while(value>0)
    {
        buff[i++] = '0' + (value%10);
        value/=10;
    }
    i--;

    // print the digits in correct order
    while(i>=0)
    {
        buffer[(*buff_ind)++] = buff[i--];
        if (*buff_ind == BUFF_SIZE)
        {
            _print_buffer(buffer, buff_ind);
            char_count += (*buff_ind);
        }
    }
    char_count += (*buff_ind);
}


void _serial_init()
{
    //USART_Config_Default(&U2);
    USART_Configure(&U2, USART_MODE_TXRX, USART_BAUDRATE_9600, USART_CPHA_DEFAULT, USART_CPOL_DEFAULT, USART_WORD_8BIT, USART_PARITY_DISABLE, USART_PARITY_NONE, USART_STOPBIT_DEFAULT);
    USART_init(&U2, &GA, USART2);
}

void Serial_init()
{
    _serial_init();
    Serialprint("\n\rSerial Communication has been initialized.\r\n");
}

uint8_t Serialprint(const char *format, ...)
{
    uint32_t buff_ind;            // Buffer index
    char buffer[BUFF_SIZE];
    char_count = 0;
    buff_ind = 0;

	va_list args;                 // initializing list pointer 
    va_start(args, format);       // Initialize the argument list

    while (*format != '\0')       // Iterate over each character in the format string
    {
        if (*format == '%')       // Check for the start of a conversion specifier
        {
            format++;             // Move to the next character after '%'
            if (*format == '%')   // Case: '%%' prints a single '%'
            {
                buffer[buff_ind++] = '%';
                if (buff_ind == BUFF_SIZE)
                {
                    _print_buffer(buffer, &buff_ind);
                    char_count += buff_ind;
                    _reset_buffer(&buff_ind);
                }
            }
            else if (*format == 'c')          // Case: '%c' prints a character
            {
                int ch = va_arg(args, int);   // Fetch the next argument as int
                buffer[buff_ind++] = ch;
                if (buff_ind == BUFF_SIZE)
                {
                    _print_buffer(buffer, &buff_ind);
                    char_count += buff_ind;
                    _reset_buffer(&buff_ind);
                }
            }
            else if (*format == 's')                 // Case: '%s' prints a string
            {
                char *str = va_arg(args, char *);    // Fetch the next argument as char*
                while (*str)                         // Iterate over each character in the string
                {
                    buffer[buff_ind++] = *str;
                    str++;
                    if (buff_ind == BUFF_SIZE)
                    {
                        _print_buffer(buffer, &buff_ind);
                        char_count += buff_ind;
                        _reset_buffer(&buff_ind);
                    }
                }
            }
            else if (*format == 'd' || *format == 'i') // Case: '%d' or '%i' prints an integer
            {
                int value = va_arg(args, int);
                _print_int(value, buffer, &buff_ind);
            }
        }
        else // Case: Regular character, not a conversion specifier
        {
            buffer[buff_ind++] = *format;
            if (buff_ind == BUFF_SIZE)
            {
                _print_buffer(buffer, &buff_ind);
                char_count += buff_ind;
                _reset_buffer(&buff_ind);
            }
        }

        format++; // Move to the next character in the format string
    }

    _print_buffer(buffer, &buff_ind);
    char_count += buff_ind;
    _reset_buffer(&buff_ind);

    va_end(args); // Clean up the argument list

    wait(2);
    return char_count; // Return the number of characters printed
}


//not working, need to fix
uint8_t Serialprintln(char *format, ...)
{
    //add line break at end of the formatted string
    string_concat(format, "\r\n");

    uint32_t buff_ind;            // Buffer index
    char buffer[BUFF_SIZE];
    char_count = 0;
    buff_ind = 0;

	va_list args;                 // initializing list pointer 
    va_start(args, format);       // Initialize the argument list

    while (*format != '\0')       // Iterate over each character in the format string
    {
        if (*format == '%')       // Check for the start of a conversion specifier
        {
            format++;             // Move to the next character after '%'
            if (*format == '%')   // Case: '%%' prints a single '%'
            {
                buffer[buff_ind++] = '%';
                if (buff_ind == BUFF_SIZE)
                {
                    _print_buffer(buffer, &buff_ind);
                    char_count += buff_ind;
                    _reset_buffer(&buff_ind);
                }
            }
            else if (*format == 'c')          // Case: '%c' prints a character
            {
                int ch = va_arg(args, int);   // Fetch the next argument as int
                buffer[buff_ind++] = ch;
                if (buff_ind == BUFF_SIZE)
                {
                    _print_buffer(buffer, &buff_ind);
                    char_count += buff_ind;
                    _reset_buffer(&buff_ind);
                }
            }
            else if (*format == 's')                 // Case: '%s' prints a string
            {
                char *str = va_arg(args, char *);    // Fetch the next argument as char*
                while (*str)                         // Iterate over each character in the string
                {
                    buffer[buff_ind++] = *str;
                    str++;
                    if (buff_ind == BUFF_SIZE)
                    {
                        _print_buffer(buffer, &buff_ind);
                        char_count += buff_ind;
                        _reset_buffer(&buff_ind);
                    }
                }
            }
            else if (*format == 'd' || *format == 'i') // Case: '%d' or '%i' prints an integer
            {
                int value = va_arg(args, int);
                _print_int(value, buffer, &buff_ind);
            }
        }
        else // Case: Regular character, not a conversion specifier
        {
            buffer[buff_ind++] = *format;
            if (buff_ind == BUFF_SIZE)
            {
                _print_buffer(buffer, &buff_ind);
                char_count += buff_ind;
                _reset_buffer(&buff_ind);
            }
        }

        format++; // Move to the next character in the format string
    }

    _print_buffer(buffer, &buff_ind);
    char_count += buff_ind;
    _reset_buffer(&buff_ind);

    va_end(args); // Clean up the argument list

    wait(2);
    return char_count; // Return the number of characters printed
}



void SerialInput(char* msg, uint8_t datatype, char* rx_buffer)
{
    char* a = rx_buffer;
    uint8_t b_ind = 0;

    Serialprint(msg);

    if(datatype == SERIAL_RX_STRING)
    {
        while(1)
        {
            USART_RX(&U2, (uint8_t*)a, 1);
            USART_TX(&U2, (uint8_t*)a, 1);
            b_ind++;
            if(*a == '\r')
                break;
            a++;
        }
        rx_buffer[--b_ind] = '\0';
    }
}
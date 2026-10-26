#ifndef INC_SERIAL_ANSI_H
#define INC_SERIAL_ANSI_H

#define ESC_HEX                 "\x1b"
#define ESC_UNICODE             "\u001b"

//Colors
//Basic 8 Colors
#define COLOR_BLACK             ESC_HEX"[30m"
#define COLOR_RED               ESC_HEX"[31m"
#define COLOR_GREEN             ESC_HEX"[32m"
#define COLOR_YELLOW            ESC_HEX"[33m"
#define COLOR_BLUE              ESC_HEX"[34m"
#define COLOR_MAGENTA           ESC_HEX"[35m"
#define COLOR_CYAN              ESC_HEX"[36m"
#define COLOR_WHITE             ESC_HEX"[37m"
#define COLOR_RESET             ESC_HEX"[0m"

//not working with teraterm
//Bright/Bold Colors
#define COLOR_BRIGHT_BLACK      COLOR_BLACK";1m"
#define COLOR_BRIGHT_RED        COLOR_RED";1m"
#define COLOR_BRIGHT_GREEN      COLOR_GREEN";1m"
#define COLOR_BRIGHT_YELLOW     COLOR_YELLOW";1m"
#define COLOR_BRIGHT_BLUE       COLOR_BLUE";1m"
#define COLOR_BRIGHT_MAGENTA    COLOR_MAGENTA";1m"
#define COLOR_BRIGHT_CYAN       COLOR_CYAN";1m"
#define COLOR_BRIGHT_WHITE      COLOR_WHITE";1m"

//not working with teraterm
//256 bit color
#define SET_COLOR_256(x)        ESC_HEX"[38;5;$"(x)"m"


//Background Colors
#define COLOR_BG_BLACK          ESC_HEX"[40m"
#define COLOR_BG_RED            ESC_HEX"[41m"
#define COLOR_BG_GREEN          ESC_HEX"[42m"
#define COLOR_BG_YELLOW         ESC_HEX"[43m"
#define COLOR_BG_BLUE           ESC_HEX"[44m"
#define COLOR_BG_MAGENTA        ESC_HEX"[45m"
#define COLOR_BG_CYAN           ESC_HEX"[46m"
#define COLOR_BG_WHITE          ESC_HEX"[47m"


//not working in teraterm
//Background Bright Colors
#define COLOR_BGB_BLACK         COLOR_BG_BLACK";1m"
#define COLOR_BGB_RED           COLOR_BG_RED";1m"
#define COLOR_BGB_GREEN         COLOR_BG_GREEN";1m"
#define COLOR_BGB_YELLOW        COLOR_BG_YELLOW";1m"
#define COLOR_BGB_BLUE          COLOR_BG_BLUE";1m"
#define COLOR_BGB_MAGENTA       COLOR_BG_MAGENTA";1m"
#define COLOR_BGB_CYAN          COLOR_BG_CYAN";1m"
#define COLOR_BGB_WHITE         COLOR_BG_WHITE";1m"

//not working in teraterm
//256 color backgrounds
#define SET_COLOR_BG_256(x)     ESC_HEX"[48;5;$"(x)"m"


//Decorations
#define BOLD                    ESC_HEX"[1m" //has no effect in teraterm, possibly BOLD is rendered by default
#define UNDERLINE               ESC_HEX"[4m"
#define REVERSED                ESC_HEX"[7m"


//Cursor Navigation
#define MOVE_CURSOR_UP(x)       ESC_HEX"["(x)"A"
#define MOVE_CURSOR_DOWN(x)     ESC_HEX"["(x)"B"
#define MOVE_CURSOR_RIGHT(x)    ESC_HEX"["(x)"C"
#define MOVE_CURSOR_LEFT(x)     ESC_HEX"["(x)"D"


#endif
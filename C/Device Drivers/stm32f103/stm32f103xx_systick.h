#ifndef INC_STM32F103XX_SYSTICK_H
#define INC_STM32F103XX_SYSTICK_H

#include "stm32f103xx.h"
#include "stm32f103xx_memory_map.h"

//Systick Peripheral Definition
#define SYSTICK       ((SysTick_RegDef*)SYSTICK_BASE_ADDR)

//STK_CTRL Bit Field Definitions
#define STK_CTRL_ENABLE                  0
#define STK_CTRL_TICKINT                 1
#define STK_CTRL_CLKSOURCE               2
#define STK_CTRL_COUNTFLAG               16



#define SYSTICK_EXCEPTION_DISABLE        0
#define SYSTICK_EXCEPTION_ENABLE         1

#define SYSTICK_CLK_SRC_AHB_DIV_8        0
#define SYSTICK_CLK_SRC_AHB              1



//APi Declarations
void Systick_Configure(uint32_t load_value, uint8_t clk_src, uint8_t exception);
void SysTick_Load(uint32_t load_value);
void SysTick_Clear();
uint8_t SysTick_Check_Flag();

#endif
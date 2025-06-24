#include "stm32f103xx_systick.h"


void SysTick_Load(uint32_t load_value)
{
    SYSTICK->LOAD = load_value;
}


void SysTick_Clear()
{
    SYSTICK->VAL = 0;
}


void Systick_Configure(uint32_t load_value, uint8_t clk_src, uint8_t exception)
{
    //set clock source
    if(clk_src == SYSTICK_CLK_SRC_AHB_DIV_8)
        SYSTICK->CTRL &= ~(1<<STK_CTRL_CLKSOURCE);
    else if(clk_src == SYSTICK_CLK_SRC_AHB)
        SYSTICK->CTRL |= 1<<STK_CTRL_CLKSOURCE;

    //enable exception
    if(exception == SYSTICK_EXCEPTION_DISABLE)
        SYSTICK->CTRL &= ~(1<<STK_CTRL_TICKINT);
    else if(exception == SYSTICK_EXCEPTION_ENABLE)
        SYSTICK->CTRL |= 1<<STK_CTRL_TICKINT;

    //Load Value
    SysTick_Load(load_value);

    //Clear VAL Register
    SysTick_Clear();
}


uint8_t SysTick_Check_Flag()
{
    return (SYSTICK->CTRL && (1<<STK_CTRL_COUNTFLAG));
}


uint32_t SysTick_Get()
{
    return SYSTICK->VAL;
}
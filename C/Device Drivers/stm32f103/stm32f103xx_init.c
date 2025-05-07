#include"stm32f103xx_init.h"

//USART2 for Serial Debugging

RCC_Handle R;

void board_init()
{
    Serial_init();              // Initialize USART2 for Serial Print (Debugging)
    RCC_init(&R, RCC_CLK_HSI);  // Initialize RCC with HSI Clock

    if(RCC_Get_Clock_Source(&R) == RCC_CLK_HSE)
        Serialprint("[INFO]: Clock Source is HSE\r\n");
    else if(RCC_Get_Clock_Source(&R)  == RCC_CLK_HSI)
        Serialprint("[INFO]: Clock Source is HSI\r\n");
    else if(RCC_Get_Clock_Source(&R)  == RCC_CLK_PLL)
        Serialprint("[INFO]: Clock Source is PLL\r\n");

    Serialprint("[INFO]: HCLK is %d MHz\r\n", R.RCC_State.High_Clock/1000000);
    Serialprint("[INFO]: PCLK1 is %d MHz\r\n", R.RCC_State.P_Clock_1/1000000);
    Serialprint("[INFO]: PCLK2 is %d MHz\r\n", R.RCC_State.P_Clock_2/1000000);
}
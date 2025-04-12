#include"stm32f103xx.h"
#include"stm32f103xx_rcc.h"
#include"stm32f103xx_serial.h"


/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************ RCC API's Definitions Start ***********************************************/

void RCC_Clock_Source(RCC_Handle* pRCCHandle)
{
    if(pRCCHandle->pRCC->CR & 1<< RCC_CR_HSEON)
        Serialprint("HSE Clock is selected\r\n");
    else if(pRCCHandle->pRCC->CR & 1<< RCC_CR_HSION)
        Serialprint("HSI Clock is selected\r\n");
    else if(pRCCHandle->pRCC->CR & 1<< RCC_CR_PLLON)
        Serialprint("PLL Clock is selected\r\n");
}

/************************************************ RCC API's Definitions End *************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
#include "stm32F407xx_gpio_driver.h"
#include<stdio.h>


/*--------------------------------------------------------------------------------------------------------------------------*/
/****************************************** GPIO Helper Function Definitions Start ******************************************/

// View GPIO Details
void GPIO_Details(GPIO_Handle* pGPIOHandle)
{
	// GPIO Registers Details
    printf("\nAFR:     %lu", (unsigned long int)pGPIOHandle->pGPIOx->AFR);
    printf("\nBSRR:    %lu", (unsigned long int)pGPIOHandle->pGPIOx->BSRR);
    printf("\nIDR:     %lu", (unsigned long int)pGPIOHandle->pGPIOx->IDR);
    printf("\nLCKR:    %lu", (unsigned long int)pGPIOHandle->pGPIOx->LCKR);
    printf("\nMODER:   %lu", (unsigned long int)pGPIOHandle->pGPIOx->MODER);
    printf("\nODR:     %lu", (unsigned long int)pGPIOHandle->pGPIOx->ODR);
    printf("\nOSPEEDR: %lu", (unsigned long int)pGPIOHandle->pGPIOx->OSPEEDR);
    printf("\nOTYPER:  %lu", (unsigned long int)pGPIOHandle->pGPIOx->OTYPER);
    printf("\nPUPDR:   %lu", (unsigned long int)pGPIOHandle->pGPIOx->PUPDR);

	// GPIO Pin Config Details
    printf("\nPin No:                        %d", pGPIOHandle->GPIOx_PinConfig.PinNo);
    printf("\nPin Mode:                      %d", pGPIOHandle->GPIOx_PinConfig.PinMode);
    printf("\nPin Output Type:               %d", pGPIOHandle->GPIOx_PinConfig.PinOPType);
    printf("\nPin Speed:                     %d", pGPIOHandle->GPIOx_PinConfig.PinSpeed);
    printf("\nPin Pull Up Pull Down Config:  %d", pGPIOHandle->GPIOx_PinConfig.PinPUPDCtrl);
    printf("\nPin Alternate Function Mode:   %d", pGPIOHandle->GPIOx_PinConfig.PinAltFuncMode);
}


// Reset the Pin Config Details
void GPIO_Pin_Reset(GPIO_PinConfig* p)
{
    p->PinNo = 0;
    p->PinMode = 0;
    p->PinOPType = 0;
    p->PinSpeed = 0;
    p->PinPUPDCtrl = 0;
    p->PinAltFuncMode = 0;
}
/******************************************* GPIO Helper Function Definitions End *******************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
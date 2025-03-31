#include "stm32f103xx.h"
#include "stm32f103xx_gpio.h"
#include "stm32f103xx_usart.h"
#include "stm32f103xx_afio.h"
#include "stm32f103xx_exti.h"
#include "stm32f103xx_nvic.h"


/*USART Pins
-USART2
--TX   PA2
--RX   PA3
--CK   PA4
--CTS  PA0
--RTS  PA1
-USART3
--TX   PB10
--RX   PB11
--CK   PB12
--CTS  PB13
--RTS  PB14
*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** USART API's Definitions Start **********************************************/

// USART - Peripheral Clock Enable
void USART_PClk_init(USART_RegDef *pUSARTx, uint8_t mode)
{
	if(mode == ENABLE)
	{
		if(pUSARTx == USART1)
			USART1_PCLK_EN();
        else if (pUSARTx == USART2)
			USART2_PCLK_EN();
        else if (pUSARTx == USART3)
			USART3_PCLK_EN();
		else if (pUSARTx == UART4)
			UART4_PCLK_EN();
        else if (pUSARTx == UART5)
			UART5_PCLK_EN();
	}
	else
	{
		//To Do
	}

}

//Set Default USART Configuration
void USART_Config_Default(USART_Handle* pUSARTHandle)
{
    pUSARTHandle->USARTx_Config.baudrate       = USART_BAUDRATE_9600;
    pUSARTHandle->USARTx_Config.clock_phase    = USART_CPHA_LOW;
    pUSARTHandle->USARTx_Config.clock_polarity = USART_CPOL_LOW;
    pUSARTHandle->USARTx_Config.parity_control = USART_PARITY_DISABLE;
    pUSARTHandle->USARTx_Config.stop_bits      = USART_STOPBIT_1;
    pUSARTHandle->USARTx_Config.prx_buffer     = 0;
}

//USART Set Baudrate
void USART_SetBaudRate(USART_Handle* pUSARTHandle)
{
    uint16_t uartdiv = SYSCORE_CLK/(pUSARTHandle->USARTx_Config.baudrate);
    pUSARTHandle->pUSARTx->BRR |= ((uartdiv/16) << USART_BRR_DIV_MANTISSA) | ((uartdiv%16) << USART_BRR_DIV_FRACTION);
}                                                                                               

//USART Init
void USART_init(USART_Handle* pUSARTHandle, GPIO_Handle* pGPIOHandle,  USART_RegDef* pUSARTx)
{
    USART_Config_Default(pUSARTHandle);
    pUSARTHandle->pUSARTx = pUSARTx;
    pUSARTHandle->pGPIOHandle = pGPIOHandle;

    //USART2
    if(pUSARTHandle->pUSARTx == USART2)
    {
        pUSARTHandle->pGPIOHandle->pGPIOx = GPIOA;

        //1. Enable GPIOA Peripheral Clock
        GPIO_PClk_init(pUSARTHandle->pGPIOHandle->pGPIOx, ENABLE);

        //2. Enable USART2 Peripheral Clock
        USART_PClk_init(pUSARTHandle->pUSARTx, ENABLE);

        //3. Initialize GPIOA pins
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinOutputSpeed = GPIO_OP_SPEED_10;
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinMode = GPIO_MODE_AF;
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinConfigType = GPIO_CONFIG_AF_OP_PP;

        //PA2
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinNo = 2;
        GPIO_Init(pUSARTHandle->pGPIOHandle);

        //PA3
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinNo = 3;
        GPIO_Init(pUSARTHandle->pGPIOHandle);

        //4. Configure baudrate
        USART_SetBaudRate(pUSARTHandle);

        //5. Enable USART for TX
        pUSARTHandle->pUSARTx->CR1 |= 1<< USART_CR1_TE;

        //6. Enable USART2
        pUSARTHandle->pUSARTx->CR1 |= 1<< USART_CR1_UE;
    }
}


void USART_TX(USART_Handle* pUSARTHandle, char* data, uint32_t size)
{
    while(size)
    {
        //wait till USART_SR->TXE becomes 1 to indicate that USART_DR is empty
        while(!(pUSARTHandle->pUSARTx->SR & 1<<USART_SR_TXE));
        pUSARTHandle->pUSARTx->DR = *data++;
        size--;
    }

    //wait till TC flag is set
    while(!(pUSARTHandle->pUSARTx->SR & 1<<USART_SR_TC));
}

void USART_RX(USART_Handle* pUSARTHandle, char* data, uint32_t size)
{
    while(size)
    {
        //wait till USART_SR->TXE becomes 1 to indicate that USART_DR is empty
        while(!(pUSARTHandle->pUSARTx->SR & 1<<USART_SR_TXE));
        *(data)++ = pUSARTHandle->pUSARTx->DR;
        size--;
    }
}
/*********************************************** USART API's Definitions Start **********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
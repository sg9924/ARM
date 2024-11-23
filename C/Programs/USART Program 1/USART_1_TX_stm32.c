#include<stdio.h>
#include<string.h>
#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_rcc_driver.h"

char msg[1024] = "USART TX Testing from STM32 to Arduino Uno.......\n\r";

USART_Handle usart2_handle;



//initialize USART2
void USART2_Init(void)
{
	usart2_handle.pUSARTx = USART2;
	usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
	usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart2_handle.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
	usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART_Init(&usart2_handle);
}


//initialize GPIO for USART2
void USART2_GPIOInit(void)
{
	GPIO_Handle usart_gpios;

	usart_gpios.pGPIOx = GPIOA;
	usart_gpios.GPIOx_PinConfig.PinMode = GPIO_MODE_ALTFUN;
	usart_gpios.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIOx_PinConfig.PinPUPDCtrl = GPIO_PIN_PU;
	usart_gpios.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
	usart_gpios.GPIOx_PinConfig.PinAltFuncMode =7;

	//USART2 TX
	usart_gpios.GPIOx_PinConfig.PinNo  = GPIO_PIN2;
	GPIO_Init(&usart_gpios);

	//USART2 RX
	usart_gpios.GPIOx_PinConfig.PinNo = GPIO_PIN3;
	GPIO_Init(&usart_gpios);
}


//initialize GPIO for button and LED
void GPIO_ButtonInit(void)
{
	GPIO_Handle GPIOBtn,GpioLed;

	//button
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIOx_PinConfig.PinNo = GPIO_PIN0;
	GPIOBtn.GPIOx_PinConfig.PinMode = GPIO_MODE_INP;
	GPIOBtn.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
	GPIOBtn.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
	GPIO_Init(&GPIOBtn);

	//LED
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIOx_PinConfig.PinNo = GPIO_PIN12;
	GpioLed.GPIOx_PinConfig.PinMode = GPIO_MODE_OP;
	GpioLed.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
	GpioLed.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_OD;
	GpioLed.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
	GPIO_Init(&GpioLed);
}

void delay(void)
{
	for(uint32_t i=0; i<200000; i++);
}


int main(void)
{

	GPIO_ButtonInit();

	USART2_GPIOInit();

    USART2_Init();

    USART_PeripheralControl(USART2,ENABLE);

    while(1)
    {
		//wait till button is pressed
		while(!GPIO_ReadIpPin(GPIOA,GPIO_PIN0));

		delay();

		USART_SendData(&usart2_handle,(uint8_t*)msg,strlen(msg));
    }

	return 0;
}
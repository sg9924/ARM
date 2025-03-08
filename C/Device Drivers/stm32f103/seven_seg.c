#include"seven_seg.h"
#include<stdint.h>
#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"


void seven_seg_init(GPIO_Handle* pGPIOh)
{
	pGPIOh->pGPIOx                         = GPIOA;
	pGPIOh->GPIOx_PinConfig.PinMode        = GPIO_MODE_OP;
	pGPIOh->GPIOx_PinConfig.PinOutputSpeed = GPIO_OP_SPEED_10;
	pGPIOh->GPIOx_PinConfig.PinConfigType  = GPIO_GP_OP_PP;
	
	
	pGPIOh->GPIOx_PinConfig.PinNo = SEG_A;
	GPIO_Init(pGPIOh);
	
	pGPIOh->GPIOx_PinConfig.PinNo = SEG_B;
	GPIO_Init(pGPIOh);
	
	pGPIOh->GPIOx_PinConfig.PinNo = SEG_C;
	GPIO_Init(pGPIOh);
	
	pGPIOh->GPIOx_PinConfig.PinNo = SEG_D;
	GPIO_Init(pGPIOh);
	
	pGPIOh->GPIOx_PinConfig.PinNo = SEG_E;
	GPIO_Init(pGPIOh);
	
	pGPIOh->GPIOx_PinConfig.PinNo = SEG_F;
	GPIO_Init(pGPIOh);
	
	pGPIOh->GPIOx_PinConfig.PinNo = SEG_G;
	GPIO_Init(pGPIOh);
	
	pGPIOh->GPIOx_PinConfig.PinNo = SEG_H;
	GPIO_Init(pGPIOh);
}


void seven_seg_display(GPIO_Handle* pGPIOh, uint8_t character)
{
	if(character == '0')
	{
		uint16_t port_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_C | 1<<SEG_D | 1<<SEG_E | 1<<SEG_F);
		GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
	}

	if(character == '1')
	{
		uint16_t port_value = (1<<SEG_B | 1<<SEG_C);
		GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
	}

	if(character == '2')
	{
		uint16_t port_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_G | 1<<SEG_D | 1<<SEG_E);
		GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
	}

	if(character == '3')
	{
		uint16_t port_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_C | 1<<SEG_D | 1<<SEG_G);
		GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
	}

	if(character == '4')
	{
		uint16_t port_value = (1<<SEG_B | 1<<SEG_C | 1<<SEG_F | 1<<SEG_G);
		GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
	}

	if(character == '5')
	{
		uint16_t port_value = (1<<SEG_A | 1<<SEG_C | 1<<SEG_D | 1<<SEG_F | 1<<SEG_G);
		GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
	}

	if(character == '6')
	{
		uint16_t port_value = (1<<SEG_A | 1<<SEG_C | 1<<SEG_D | 1<<SEG_E | 1<<SEG_F | 1<<SEG_G);
		GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
	}

	if(character == '7')
	{
		uint16_t port_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_C);
		GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
	}

	if(character == '8')
	{
		uint16_t port_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_C | 1<<SEG_D | 1<<SEG_E | 1<<SEG_F | 1<<SEG_G);
		GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
	}

	if(character == '9')
	{
		uint16_t port_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_C | 1<<SEG_D | 1<<SEG_F | 1<<SEG_G);
		
		GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
	}
}
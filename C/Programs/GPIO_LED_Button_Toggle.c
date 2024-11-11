#include "stm32F407xx_gpio_driver.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


void delay(uint32_t d)
{
	for(uint32_t i=0;i<d;i++);
}
int main(void)
{
    GPIO_Handle gpio_led, gpio_button;

    // led configuration
    gpio_led.pGPIOx = GPIOD;
    gpio_led.GPIOx_PinConfig.PinNo = GPIO_PIN12;
	gpio_led.GPIOx_PinConfig.PinMode = GPIO_MODE_OP;
	gpio_led.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	gpio_led.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
	gpio_led.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;

	GPIO_Init(&gpio_led);

	// Inbuilt button configuration
	gpio_button.pGPIOx = GPIOA;
	gpio_button.GPIOx_PinConfig.PinNo = GPIO_PIN0;
	gpio_button.GPIOx_PinConfig.PinMode = GPIO_MODE_INP;
	gpio_button.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
	gpio_button.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;

	GPIO_Init(&gpio_button);

	while(1)
	{
		if(GPIO_ReadIpPin(GPIOA, GPIO_PIN0))
		{
			delay(500000); // delay to prevent de-bouncing
			GPIO_OpToggle(GPIOD, GPIO_PIN12);
			//delay(500000);
			//GPIO_OpToggle(GPIOD, GPIO_PIN12);
		}
	}

	return 0;
}
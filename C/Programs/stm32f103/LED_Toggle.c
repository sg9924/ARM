#include"../../Device_Drivers/stm32f103/inc/stm32f103xx_init.h"
#include"../../Device_Drivers/stm32f103/inc/stm32f103xx_gpio.h"
#include"../../Device_Drivers/stm32f103/inc/stm32f103xx_serial.h"


int main()
{
    board_init();

    GPIO_Handle led;
    GPIO_Config(&led, GPIOC, GPIO_MODE_OP, GPIO_CONFIG_GP_OP_PP, GPIO_PIN13, GPIO_OP_SPEED_2);
    GPIO_Init(&led);

    while(1)
	{
		//Toggle PC13 Led Pin
		GPIO_OpToggle(led.pGPIOx, led.GPIOx_PinConfig.PinNo);
		tim_delay_ms(1000);
        Serialprintln("LED at GPIOC Pin 13 is being toggled!", INFO);
	}
}
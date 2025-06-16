#include"stm32f103xx_init.h"

GPIO_Handle B;

#define BUTTON_PRESSED GPIO_PIN_RESET

int main()
{
    board_init();

    //Open Drain - Pull Up Resistor
    GPIO_Config(&B, GPIOB, GPIO_MODE_IP, GPIO_CONFIG_PU, GPIO_PIN5, GPIO_OP_SPEED_0);
    GPIO_Init(&B);

    while(1)
    {
        //wait till button is pressed
        while(GPIO_ReadIpPin(GPIOB, GPIO_PIN5) != BUTTON_PRESSED);
        wait_ms(250);
        Serialprint("Button has been pressed!\r\n");
    }
}
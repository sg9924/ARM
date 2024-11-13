#include"stm32F407xx_gpio_driver.h"
#include<stdio.h>

static void delay(uint32_t count)
{
    for(int i=0; i<(count*1000); i++);
}


int main(void)
{
    GPIO_Handle row[4], col[4];

    //1. Initialization
    //rows - outputs
    row[0].pGPIOx = GPIOD;
    row[0].GPIOx_PinConfig.PinNo = GPIO_PIN0;
    row[0].GPIOx_PinConfig.PinMode = GPIO_MODE_OP;
    row[0].GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
    row[0].GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
    row[0].GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
    GPIO_Init(&row[0]);

    row[1].pGPIOx = GPIOD;
    row[1].GPIOx_PinConfig.PinNo = GPIO_PIN1;
    row[1].GPIOx_PinConfig.PinMode = GPIO_MODE_OP;
    row[1].GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
    row[1].GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
    row[1].GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
    GPIO_Init(&row[1]);

    row[2].pGPIOx = GPIOD;
    row[2].GPIOx_PinConfig.PinNo = GPIO_PIN2;
    row[2].GPIOx_PinConfig.PinMode = GPIO_MODE_OP;
    row[2].GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
    row[2].GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
    row[2].GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
    GPIO_Init(&row[2]);

    row[3].pGPIOx = GPIOD;
    row[3].GPIOx_PinConfig.PinNo = GPIO_PIN3;
    row[3].GPIOx_PinConfig.PinMode = GPIO_MODE_OP;
    row[3].GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
    row[3].GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
    row[3].GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
    GPIO_Init(&row[3]);




    //columns - inputs
    col[0].pGPIOx = GPIOD;
    col[0].GPIOx_PinConfig.PinNo = GPIO_PIN8;
    col[0].GPIOx_PinConfig.PinMode = GPIO_MODE_INP;
    col[0].GPIOx_PinConfig.PinPUPDCtrl = GPIO_PIN_PU;
    GPIO_Init(&col[0]);

    col[1].pGPIOx = GPIOD;
    col[1].GPIOx_PinConfig.PinNo = GPIO_PIN9;
    col[1].GPIOx_PinConfig.PinMode = GPIO_MODE_INP;
    col[1].GPIOx_PinConfig.PinPUPDCtrl = GPIO_PIN_PU;
    GPIO_Init(&col[1]);

    col[2].pGPIOx = GPIOD;
    col[2].GPIOx_PinConfig.PinNo = GPIO_PIN10;
    col[2].GPIOx_PinConfig.PinMode = GPIO_MODE_INP;
    col[2].GPIOx_PinConfig.PinPUPDCtrl = GPIO_PIN_PU;
    GPIO_Init(&col[2]);

    col[3].pGPIOx = GPIOD;
    col[3].GPIOx_PinConfig.PinNo = GPIO_PIN11;
    col[3].GPIOx_PinConfig.PinMode = GPIO_MODE_INP;
    col[3].GPIOx_PinConfig.PinPUPDCtrl = GPIO_PIN_PU;
    GPIO_Init(&col[3]);


    while(1) // loop to scan all columns in the keypad for all rows
    {

        // make all rows HIGH
        GPIO_WriteOpPin(row[0].pGPIOx, GPIO_PIN0, 1);
        GPIO_WriteOpPin(row[1].pGPIOx, GPIO_PIN1, 1);
        GPIO_WriteOpPin(row[2].pGPIOx, GPIO_PIN2, 1);
        GPIO_WriteOpPin(row[3].pGPIOx, GPIO_PIN3, 1);

        //3. Make Row 1 pin LOW
        GPIO_WriteOpPin(row[0].pGPIOx, GPIO_PIN0, 0);

        //Read column 1
        if(!(GPIO_ReadIpPin(col[0].pGPIOx, GPIO_PIN8))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key 1 has been pressed\n");
        }

        if(!(GPIO_ReadIpPin(col[1].pGPIOx, GPIO_PIN9))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key 2 has been pressed\n");
        }

        if(!(GPIO_ReadIpPin(col[2].pGPIOx, GPIO_PIN10))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key 3 has been pressed\n");
        }

        if(!(GPIO_ReadIpPin(col[3].pGPIOx, GPIO_PIN11))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key A has been pressed\n");
        }


        //make all rows HIGH
        GPIO_WriteOpPin(row[0].pGPIOx, GPIO_PIN0, 1);
        GPIO_WriteOpPin(row[1].pGPIOx, GPIO_PIN1, 1);
        GPIO_WriteOpPin(row[2].pGPIOx, GPIO_PIN2, 1);
        GPIO_WriteOpPin(row[3].pGPIOx, GPIO_PIN3, 1);

        //4. Make Row 2 pin LOW
        GPIO_WriteOpPin(row[0].pGPIOx, GPIO_PIN1, 0);

        // Read column 1
        if(!(GPIO_ReadIpPin(col[0].pGPIOx, GPIO_PIN8))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key 4 has been pressed\n");
        }
        // Read column 2
        if(!(GPIO_ReadIpPin(col[1].pGPIOx, GPIO_PIN9))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key 5 has been pressed\n");
        }
        // Read column 3
        if(!(GPIO_ReadIpPin(col[2].pGPIOx, GPIO_PIN10))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key 6 has been pressed\n");
        }
        // Read column 4
        if(!(GPIO_ReadIpPin(col[3].pGPIOx, GPIO_PIN11))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key B has been pressed\n");
        }


        // make all rows HIGH
        GPIO_WriteOpPin(row[0].pGPIOx, GPIO_PIN0, 1);
        GPIO_WriteOpPin(row[1].pGPIOx, GPIO_PIN1, 1);
        GPIO_WriteOpPin(row[2].pGPIOx, GPIO_PIN2, 1);
        GPIO_WriteOpPin(row[3].pGPIOx, GPIO_PIN3, 1);

        //5. Make Row 3 pin LOW
        GPIO_WriteOpPin(row[0].pGPIOx, GPIO_PIN2, 0);

        // Read column 1
        if(!(GPIO_ReadIpPin(col[0].pGPIOx, GPIO_PIN8))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key 7 has been pressed\n");
        }
        // Read column 2
        if(!(GPIO_ReadIpPin(col[1].pGPIOx, GPIO_PIN9))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key 8 has been pressed\n");
        }
        // Read column 3
        if(!(GPIO_ReadIpPin(col[2].pGPIOx, GPIO_PIN10))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key 9 has been pressed\n");
        }
        // Read column 4
        if(!(GPIO_ReadIpPin(col[3].pGPIOx, GPIO_PIN11))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key C has been pressed\n");
        }


        // make all rows HIGH
        GPIO_WriteOpPin(row[0].pGPIOx, GPIO_PIN0, 1);
        GPIO_WriteOpPin(row[1].pGPIOx, GPIO_PIN1, 1);
        GPIO_WriteOpPin(row[2].pGPIOx, GPIO_PIN2, 1);
        GPIO_WriteOpPin(row[3].pGPIOx, GPIO_PIN3, 1);

        //6. Make Row 4 pin LOW
        GPIO_WriteOpPin(row[0].pGPIOx, GPIO_PIN3, 0);

        // Read column 1
        if(!(GPIO_ReadIpPin(col[0].pGPIOx, GPIO_PIN8))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key * has been pressed\n");
        }
        // Read column 2
        if(!(GPIO_ReadIpPin(col[1].pGPIOx, GPIO_PIN9))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key 0 has been pressed\n");
        }
        // Read column 3
        if(!(GPIO_ReadIpPin(col[2].pGPIOx, GPIO_PIN10))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key # has been pressed\n");
        }
        // Read column 4
        if(!(GPIO_ReadIpPin(col[3].pGPIOx, GPIO_PIN11))) // if key is pressed , goes low
        {
            delay(300);
            printf("Key D has been pressed\n");
        }
    }


    return 0;
}

#include "stm32F407xx_4x4_keypad_driver.h"

GPIO_Handle row[4], col[4];

static void delay(uint32_t count)
{
    for(int i=0; i<(count*1000); i++);
}



static void initialize_rows()
{
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
}



static void initialize_columns()
{
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
}


//rows enable
static void rows_enable()
{
    GPIO_WriteOpPin(row[0].pGPIOx, GPIO_PIN0, 1);
    GPIO_WriteOpPin(row[1].pGPIOx, GPIO_PIN1, 1);
    GPIO_WriteOpPin(row[2].pGPIOx, GPIO_PIN2, 1);
    GPIO_WriteOpPin(row[3].pGPIOx, GPIO_PIN3, 1);
}


static void row1_enable()
{
    GPIO_WriteOpPin(row[0].pGPIOx, GPIO_PIN0, 1);
}


static void row2_enable()
{
    GPIO_WriteOpPin(row[1].pGPIOx, GPIO_PIN1, 1);
}


static void row3_enable()
{
    GPIO_WriteOpPin(row[2].pGPIOx, GPIO_PIN2, 1);
}


static void row4_enable()
{
    GPIO_WriteOpPin(row[3].pGPIOx, GPIO_PIN3, 1);
}


//rows disable
static void row1_disable()
{
    GPIO_WriteOpPin(row[0].pGPIOx, GPIO_PIN0, 0);
}


static void row2_disable()
{
    GPIO_WriteOpPin(row[1].pGPIOx, GPIO_PIN1, 0);
}


static void row3_disable()
{
    GPIO_WriteOpPin(row[2].pGPIOx, GPIO_PIN2, 0);
}


static void row4_disable()
{
    GPIO_WriteOpPin(row[3].pGPIOx, GPIO_PIN3, 0);
}





void keypad_initialize()
{
    initialize_rows();
    initialize_columns();
}

//rows scan
char row1_scan()
{
    rows_enable();
    row1_disable();

    //Read column 1
    if(!(GPIO_ReadIpPin(col[0].pGPIOx, GPIO_PIN8))) // if key is pressed , goes low
    {
        delay(300);
        return '1';
    }

    if(!(GPIO_ReadIpPin(col[1].pGPIOx, GPIO_PIN9))) // if key is pressed , goes low
    {
        delay(300);
        return '2';
    }

    if(!(GPIO_ReadIpPin(col[2].pGPIOx, GPIO_PIN10))) // if key is pressed , goes low
    {
        delay(300);
        return '3';
    }

    if(!(GPIO_ReadIpPin(col[3].pGPIOx, GPIO_PIN11))) // if key is pressed , goes low
    {
        delay(300);
        return 'A';
    }
}


//rows scan
char row2_scan()
{
    rows_enable();
    row2_disable();

    //Read column 1
    if(!(GPIO_ReadIpPin(col[0].pGPIOx, GPIO_PIN8))) // if key is pressed , goes low
    {
        delay(300);
        return '4';
    }

    if(!(GPIO_ReadIpPin(col[1].pGPIOx, GPIO_PIN9))) // if key is pressed , goes low
    {
        delay(300);
        return '5';
    }

    if(!(GPIO_ReadIpPin(col[2].pGPIOx, GPIO_PIN10))) // if key is pressed , goes low
    {
        delay(300);
        return '6';
    }

    if(!(GPIO_ReadIpPin(col[3].pGPIOx, GPIO_PIN11))) // if key is pressed , goes low
    {
        delay(300);
        return 'B';
    }
}




//rows scan
char row3_scan()
{
    rows_enable();
    row3_disable();

    //Read column 1
    if(!(GPIO_ReadIpPin(col[0].pGPIOx, GPIO_PIN8))) // if key is pressed , goes low
    {
        delay(300);
        return '7';
    }

    if(!(GPIO_ReadIpPin(col[1].pGPIOx, GPIO_PIN9))) // if key is pressed , goes low
    {
        delay(300);
        return '8';
    }

    if(!(GPIO_ReadIpPin(col[2].pGPIOx, GPIO_PIN10))) // if key is pressed , goes low
    {
        delay(300);
        return '9';
    }

    if(!(GPIO_ReadIpPin(col[3].pGPIOx, GPIO_PIN11))) // if key is pressed , goes low
    {
        delay(300);
        return 'C';
    }
}



//rows scan
char row4_scan()
{
    rows_enable();
    row4_disable();

    //Read column 1
    if(!(GPIO_ReadIpPin(col[0].pGPIOx, GPIO_PIN8))) // if key is pressed , goes low
    {
        delay(300);
        return '*';
    }

    if(!(GPIO_ReadIpPin(col[1].pGPIOx, GPIO_PIN9))) // if key is pressed , goes low
    {
        delay(300);
        return '0';
    }

    if(!(GPIO_ReadIpPin(col[2].pGPIOx, GPIO_PIN10))) // if key is pressed , goes low
    {
        delay(300);
        return '#';
    }

    if(!(GPIO_ReadIpPin(col[3].pGPIOx, GPIO_PIN11))) // if key is pressed , goes low
    {
        delay(300);
        return 'D';
    }
}
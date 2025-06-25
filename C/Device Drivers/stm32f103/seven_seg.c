#include"seven_seg.h"
#include<stdint.h>
#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"


void seven_seg_init(GPIO_Handle* pGPIOh, GPIO_RegDef* GPIO_Port, uint8_t conn_type)
{
    pGPIOh->pGPIOx                         = GPIO_Port;
    pGPIOh->GPIOx_PinConfig.PinMode        = GPIO_MODE_OP;
    pGPIOh->GPIOx_PinConfig.PinOutputSpeed = GPIO_OP_SPEED_10;
    pGPIOh->GPIOx_PinConfig.PinConfigType  = GPIO_CONFIG_GP_OP_PP;
    
    
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

    seven_seg_pins_reset(pGPIOh, conn_type);
}


void seven_seg_pins_reset(GPIO_Handle* pGPIOh, uint8_t conn_type)
{
    if(conn_type == SEVEN_SEG_COMMON_CATHODE)
    {
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_A, GPIO_PIN_RESET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_B, GPIO_PIN_RESET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_C, GPIO_PIN_RESET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_D, GPIO_PIN_RESET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_E, GPIO_PIN_RESET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_F, GPIO_PIN_RESET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_G, GPIO_PIN_RESET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_H, GPIO_PIN_RESET);
    }
    else if(conn_type == SEVEN_SEG_COMMON_ANODE)
    {
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_A, GPIO_PIN_SET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_B, GPIO_PIN_SET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_C, GPIO_PIN_SET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_D, GPIO_PIN_SET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_E, GPIO_PIN_SET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_F, GPIO_PIN_SET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_G, GPIO_PIN_SET);
        GPIO_WriteOpPin(pGPIOh->pGPIOx, SEG_H, GPIO_PIN_SET);
    }
}


void seven_seg_display(GPIO_Handle* pGPIOh, uint8_t character, uint8_t conn_type)
{
    uint16_t port_reset_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_C | 1<<SEG_D | 1<<SEG_E | 1<<SEG_F | 1<<SEG_G | 1<<SEG_H);
    uint16_t port_value;

    if(character == '0')
    {
        if(conn_type == SEVEN_SEG_COMMON_CATHODE)
            port_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_C | 1<<SEG_D | 1<<SEG_E | 1<<SEG_F);
        GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
    }

    if(character == '1')
    {
        if(conn_type == SEVEN_SEG_COMMON_CATHODE)
            port_value = (1<<SEG_B | 1<<SEG_C);
        GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
    }

    if(character == '2')
    {
        if(conn_type == SEVEN_SEG_COMMON_CATHODE)
            port_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_G | 1<<SEG_D | 1<<SEG_E);
        GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
    }

    if(character == '3')
    {
        if(conn_type == SEVEN_SEG_COMMON_CATHODE)
            port_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_C | 1<<SEG_D | 1<<SEG_G);
        GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
    }

    if(character == '4')
    {
        if(conn_type == SEVEN_SEG_COMMON_CATHODE)
            port_value = (1<<SEG_B | 1<<SEG_C | 1<<SEG_F | 1<<SEG_G);
        GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
    }

    if(character == '5')
    {
        if(conn_type == SEVEN_SEG_COMMON_CATHODE)
            port_value = (1<<SEG_A | 1<<SEG_C | 1<<SEG_D | 1<<SEG_F | 1<<SEG_G);
        GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
    }

    if(character == '6')
    {
        if(conn_type == SEVEN_SEG_COMMON_CATHODE)
            port_value = (1<<SEG_A | 1<<SEG_C | 1<<SEG_D | 1<<SEG_E | 1<<SEG_F | 1<<SEG_G);
        GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
    }

    if(character == '7')
    {
        if(conn_type == SEVEN_SEG_COMMON_CATHODE)
            port_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_C);
        GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
    }

    if(character == '8')
    {
        if(conn_type == SEVEN_SEG_COMMON_CATHODE)
            port_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_C | 1<<SEG_D | 1<<SEG_E | 1<<SEG_F | 1<<SEG_G);
        GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
    }

    if(character == '9')
    {
        if(conn_type == SEVEN_SEG_COMMON_CATHODE)
            port_value = (1<<SEG_A | 1<<SEG_B | 1<<SEG_C | 1<<SEG_D | 1<<SEG_F | 1<<SEG_G);
        GPIO_WriteOpPort(pGPIOh->pGPIOx, port_value);
    }
}
#include<stdio.h>
#include<string.h>
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_i2c_driver.h"
#include "stm32f407xx_rcc_driver.h"

#define MY_ADDR       0x70
#define SLAVE_ADDR    0x80


//PB6:  SCL
//PB7:  SDA


void delay(void)
{
	for(uint32_t i=0 ; i<100000; i++);
}


I2C_Handle I2C1Handle;
uint8_t rcv_buf[32];      //data to be received


//initialize GPIO for I2C
void I2C1_GPIOInits(void)
{
	GPIO_Handle I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIOx_PinConfig.PinMode = GPIO_MODE_ALTFUN;
	I2CPins.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIOx_PinConfig.PinPUPDCtrl = GPIO_PIN_PU;
	I2CPins.GPIOx_PinConfig.PinAltFuncMode = 4;
	I2CPins.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;

	//SCL
	I2CPins.GPIOx_PinConfig.PinNo = GPIO_PIN6;
	GPIO_Init(&I2CPins);

	//SDA
	I2CPins.GPIOx_PinConfig.PinNo = GPIO_PIN7;
	GPIO_Init(&I2CPins);
}


//initialize I2C
void I2C1_Inits(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);

}


//initialize GPIO for button
void GPIO_ButtonInit(void)
{
	GPIO_Handle GPIOBtn;

	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIOx_PinConfig.PinNo = GPIO_PIN0;
	GPIOBtn.GPIOx_PinConfig.PinMode = GPIO_MODE_INP;
	GPIOBtn.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
	GPIOBtn.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);
}



int main(void)
{
	uint8_t commandcode;
	uint8_t len;

	printf("Application is running\n");

	GPIO_ButtonInit();

	//initailize GPIO for I2C
	I2C1_GPIOInits();

	//initialize I2C
	I2C1_Inits();

	//enable I2C peripheral
	I2C_PeripheralControl(I2C1,ENABLE);

	//enable ACK
	I2C_ManageAcking(I2C1,I2C_ACK_ENABLE);

	while(1)
	{
		//wait till button is pressed
		while(!GPIO_ReadIpPin(GPIOA,GPIO_PIN0));

		delay();

        //send command code to get length of data to be recieved
		commandcode = 0x51;
		I2C_MasterSendData(&I2C1Handle,&commandcode,1,SLAVE_ADDR,I2C_ENABLE_SR);
		I2C_MasterReceiveData(&I2C1Handle,&len,1,SLAVE_ADDR,I2C_ENABLE_SR);

        //send command code to get the required data
		commandcode = 0x52;
		I2C_MasterSendData(&I2C1Handle,&commandcode,1,SLAVE_ADDR,I2C_ENABLE_SR);
		I2C_MasterReceiveData(&I2C1Handle,rcv_buf,len,SLAVE_ADDR,I2C_DISABLE_SR);

		rcv_buf[len+1] = '\0'; //terminate the received string
		printf("Received Data: %s",rcv_buf);
	}
}
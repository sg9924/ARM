#include<stdio.h>
#include<string.h>
#include"stm32f407xx_gpio_driver.h"
#include"stm32f407xx_i2c_driver.h"

#define MASTER_ADDR 0x61;
#define SLAVE_ADDR  0x68

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

I2C_Handle I2C1_Handle;
uint8_t some_data[] = "We are testing I2C master Tx\n";

//PB6: SCL
//PB7: SDA


//initialize GPIO for I2C
void I2C1_GPIOInits(void)
{
	GPIO_Handle I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIOx_PinConfig.PinMode = GPIO_MODE_ALTFUN;
	I2CPins.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIOx_PinConfig.PinPUPDCtrl = GPIO_PIN_PU;           //internal Pullup Resistors are used - if not then 3.3k external resistors should be used
	I2CPins.GPIOx_PinConfig.PinAltFuncMode = 4;
	I2CPins. GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;

	//SCL
	I2CPins.GPIOx_PinConfig.PinNo = GPIO_PIN6;
	GPIO_Init(&I2CPins);

	//SDA
	I2CPins.GPIOx_PinConfig.PinNo = GPIO_PIN7;

	GPIO_Init(&I2CPins);
}


//initialize I2C settings
void I2C1_Inits(void)
{
	I2C1_Handle.pI2Cx = I2C1;
	I2C1_Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	I2C1_Handle.I2C_Config.I2C_DeviceAddress = MASTER_ADDR;
	I2C1_Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1_Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1_Handle);
}


//initialize GPIO for button which used to start I2C communication
void GPIO_ButtonInit(void)
{
	GPIO_Handle GPIOBtn;

	//this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIOx_PinConfig.PinNo = GPIO_PIN0;
	GPIOBtn.GPIOx_PinConfig.PinMode = GPIO_MODE_INP;
	GPIOBtn.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
	GPIOBtn.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);
}



int main()
{
    //initialize GPIO for button
	GPIO_ButtonInit();

	//initialize GPIO for I2C1
	I2C1_GPIOInits();

	//initialize I2C1
	I2C1_Inits();

	//enable I2C1 peripheral
	I2C_PeripheralControl(I2C1,ENABLE);

	while(1)
	{
		//wait till button is pressed
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN0));

		//provide delay to avoid debouncing
		delay();

		//send to data to slave (arduino)
		I2C_MasterSendData(&I2C1_Handle,some_data,strlen((char*)some_data),SLAVE_ADDR,0);
	}

}
#include<string.h>
#include"stm32F407xx_gpio_driver.h"
#include"stm32F407xx_spi_driver.h"

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000 ; i ++);
}

//STM32 (Master) Pins
//PB15: SPI2_MOSI
//PB13: SPI2_SCLK
//PB12: SPI2_NSS
//Alt function mode: 5


//Arduino (Slave) Pins
//SCK:   13  
//MISO:  12
//MOSI:  11
//SS:    10


void SPI2_GPIOInits() // initialize GPIO for SPI
{
	GPIO_Handle SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIOx_PinConfig.PinMode = GPIO_MODE_ALTFUN;
	SPIPins.GPIOx_PinConfig.PinAltFuncMode = 5;
	SPIPins.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
	SPIPins.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;

	//SCLK
	SPIPins.GPIOx_PinConfig.PinNo = GPIO_PIN13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIOx_PinConfig.PinNo = GPIO_PIN15;
	GPIO_Init(&SPIPins);

	//NSS - Connecting to Arduino, no software slave management
	SPIPins.GPIOx_PinConfig.PinNo = GPIO_PIN12;
	GPIO_Init(&SPIPins);
}

void SPI2_Inits() //initialize SPI settings
{

	SPI_Handle SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;            //full duplex
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;      //master
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;         //clock  
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;                      //data frame format - 8 bits
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;                      //CPOL=0 - CLock low when idle
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;                      //CPHA=0 - low to high transition
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;                         //software slave management disable

	SPI_Init(&SPI2handle);
}


void GPIO_ButtonInit() //initialize GPIO for button used to start SPI communication
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
	char user_data[] = "This is a message sent from STM32F407 Board to Arduino Uno to test SPI communications";

    //initialize button
	GPIO_ButtonInit();

	//intialize SPI pins
	SPI2_GPIOInits();

	//intialize SPI
	SPI2_Inits();

    //enable SSOE - NSS will be managed automatically by hardware
	SPI_SSOEConfig(SPI2,ENABLE);

	while(1)
	{
		//wait till the button is pressed
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN0));

		//provide delay to avoid button debouncing
		delay();

		//enable SPI2 peripheral
		SPI_PeripheralControl(SPI2,ENABLE);

		//Transaction 1: send length of data
		uint8_t dataLen = strlen(user_data);
		SPI_SendData(SPI2,&dataLen,1);

		//Transaction 2: send user data
		SPI_SendData(SPI2,(uint8_t*)user_data,strlen(user_data));

		//wait till SPI is not busy
		while(SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG));

		//disable SPI2 peripheral as communication is over
		SPI_PeripheralControl(SPI2,DISABLE);
	}

	return 0;
}
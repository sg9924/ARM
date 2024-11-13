#include<string.h>
#include"stm32F407xx_gpio_driver.h"
#include"stm32F407xx_spi_driver.h"


//PB15 --> SPI2_MOSI
//PB13 -> SPI2_SCLK
//ALT function mode: 5

void SPI2_GPIOInits(void)
{
	GPIO_Handle SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIOx_PinConfig.PinMode = GPIO_MODE_ALTFUN;
	SPIPins.GPIOx_PinConfig.PinAltFuncMode = 5;
	SPIPins.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
	SPIPins.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;

	//SCLK Intialize
	SPIPins.GPIOx_PinConfig.PinNo = GPIO_PIN13;
	GPIO_Init(&SPIPins);

	//MOSI Intialize
	SPIPins.GPIOx_PinConfig.PinNo = GPIO_PIN15;
	GPIO_Init(&SPIPins);

//This test uses Full Duplex for only sending data
//No slave is present
//Hence MISO and NSS are not configured
//However internal NSS is used (Software Slave management)
}

void SPI2_Inits(void)
{

	SPI_Handle SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;             //Full Duplex
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;       //Master
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;           //8 MHz clock for SCLK
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;                       //Data Frame Format of 8 bits
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_HIGH;                      //CPOL=1 - Clock is High during idle state
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;                       //CPHA=0 - High to Low transition of Clock
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;                          //Software Slave Management

	SPI_Init(&SPI2handle);
}

int main(void)
{
	char user_data[] = "Hello world";

	//Intialize GPIO Pins for SPI communication in SPI2
	SPI2_GPIOInits();

	//Intialize the configured settings for SPI2
	SPI2_Inits();

	//SSM is enabled, Internal NSS should be 1 for Master
	SPI_SSIConfig(SPI2,ENABLE);

	//SPI2 Peripheral Enable
	SPI_PeripheralControl(SPI2,ENABLE);

	//Send sample data
	SPI_SendData(SPI2,(uint8_t*)user_data,strlen(user_data));

	//wait till SPI is not busy - communication is done
	while(SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG));

	//Disable SPI2 Peripheral since communication is over
	SPI_PeripheralControl(SPI2,DISABLE);

	while(1); //hang the program

	return 0;

}
#include<stdio.h>
#include<string.h>
#include"stm32f407xx_gpio_driver.h"
#include"stm32f407xx_spi_driver.h"


//commands
#define COMMAND_LED_CTRL      0x50
#define COMMAND_LED_READ      0x51
#define COMMAND_PRINT         0x52
#define COMMAND_ID_READ       0x53

#define LED_ON     1
#define LED_OFF    0

//arduino analog pins
#define ANALOG_PIN0 	0
#define ANALOG_PIN1 	1
#define ANALOG_PIN2 	2
#define ANALOG_PIN3 	3
#define ANALOG_PIN4 	4

//Arduino led pin
#define LED_PIN  9

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

//STM32 SPI Pins
//PB14 --> SPI2_MISO
//PB15 --> SPI2_MOSI
//PB13 -> SPI2_SCLK
//PB12 --> SPI2_NSS
//Alt function mode : 5


//initialize GPIO for SPI
void SPI2_GPIOInits(void)
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

	//MISO
	SPIPins.GPIOx_PinConfig.PinNo = GPIO_PIN14;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIOx_PinConfig.PinNo = GPIO_PIN12;
	GPIO_Init(&SPIPins);


}


//initialize SPI2 for SPI communication
void SPI2_Inits(void)
{
	SPI_Handle SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;            //Full Duplex
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;      //Master
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;         //clk
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;                      //CPOL=0
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;                      //CPHA=0
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;                         //disable software slave management

	SPI_Init(&SPI2handle);
}


//initialize GPIO for button & led
void GPIO_ButtonInit(void)
{
	GPIO_Handle GPIOBtn,GpioLed;

	//GPIO configuration for Button
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIOx_PinConfig.PinNo = GPIO_PIN0;
	GPIOBtn.GPIOx_PinConfig.PinMode = GPIO_MODE_INP;
	GPIOBtn.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
	GPIOBtn.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);

	//GPIO configuration for LED
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIOx_PinConfig.PinNo = GPIO_PIN12;
	GpioLed.GPIOx_PinConfig.PinMode = GPIO_MODE_OP;
	GpioLed.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
	GpioLed.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_OD;
	GpioLed.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;

	GPIO_Init(&GpioLed);
}


//verify received message from Arduino
uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if(ackbyte == (uint8_t)0xF5)
	{
		//using value of 0xF5 as acknowledgement
		return 1;
	}
	return 0;
}



int main()
{
	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;

	printf("Application is running\n");

    //initialize button & led
	GPIO_ButtonInit();

	//initialize GPIO for SPI2 peripheral
	SPI2_GPIOInits();

	//initialize SPI2
	SPI2_Inits();

	printf("SPI Iinitialized\n");

    //enabling SSOE, using internal NSS, managed automatically by the hardware
	SPI_SSOEConfig(SPI2,ENABLE);

	while(1)
	{
		//wait till button is pressed
		while(!GPIO_ReadFromInputPin(GPIOA,GPIO_PIN0));

		//delay for avoiding debouncing
		delay();

		//enable SPI2 peripheral
		SPI_PeripheralControl(SPI2,ENABLE);

//-----------------> 1. Command for LED
		uint8_t commandcode = COMMAND_LED_CTRL;
		uint8_t ackbyte;
		uint8_t args[2];

		//send command
		SPI_SendData(SPI2,&commandcode,1);

		//do a dummy read to clear RXNE
		SPI_ReceiveData(SPI2,&dummy_read,1);

		//send dummy byte to fetch response from slave (Arduino)
		SPI_SendData(SPI2,&dummy_write,1);

		//read the received byte for acknowledgement
		SPI_ReceiveData(SPI2,&ackbyte,1);

        //verify if the received byte is acknowledgement
		if( SPI_VerifyResponse(ackbyte))
		{
            //configure arguments
			args[0] = LED_PIN;
			args[1] = LED_ON;

			//send the arguments
			SPI_SendData(SPI2,args,2);

			//do a dummy read
			SPI_ReceiveData(SPI2,args,2);
			printf("COMMAND_LED_CTRL Executed\n");
		}
	

//-----------------> 2. Command for LED Read

		//wait till button is pressed
		while(!GPIO_ReadFromInputPin(GPIOA,GPIO_PIN0));

		//delay to avoid debouncing
		delay();

		commandcode = COMMAND_LED_READ;

		//send command
		SPI_SendData(SPI2,&commandcode,1);

		//do a dummy read to clear RXNE
		SPI_ReceiveData(SPI2,&dummy_read,1);

		//send a dummy byte to fetch response from Arduino slave
		SPI_SendData(SPI2,&dummy_write,1);

		//read the byte received for acknowledgement
		SPI_ReceiveData(SPI2,&ackbyte,1);

		if( SPI_VerifyResponse(ackbyte))
		{
			args[0] = LED_PIN;

			//send arguments
			SPI_SendData(SPI2,args,1);

			//do a dummy read to clear RXNE
			SPI_ReceiveData(SPI2,&dummy_read,1);

			delay();

			//send dummy byte to fetch response from slave
			SPI_SendData(SPI2,&dummy_write,1);

			uint8_t led_status;

            //receive data (status of led)
			SPI_ReceiveData(SPI2,&led_status,1);
			printf("COMMAND_READ_LED: %d\n",led_status);
		}
	}
	return 0;
}
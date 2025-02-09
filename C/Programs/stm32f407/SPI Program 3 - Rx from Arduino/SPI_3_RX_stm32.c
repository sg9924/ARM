#include<stdio.h>
#include<string.h>
#include "stm32F407xx_gpio_driver.h"
#include "stm32F407xx_spi_driver.h"

//here stm32 is master and Arduino is slave
SPI_Handle SPI2handle;

char receive_buffer[500];       //data buffer

volatile uint8_t ReadByte;
volatile uint8_t receive_stop = 0;

/*This flag will be set in the interrupt handler of the Arduino interrupt GPIO */
volatile uint8_t dataAvailable = 0;

void delay(void)
{
	for(uint32_t i=0; i<500000; i++);
}


//PB14:   SPI2_MISO
//PB15:   SPI2_MOSI
//PB13:   SPI2_SCLK
//PB12:   SPI2_NSS
//Alt function mode: 5
//PD6:    for GPIO interrupt connects to pin 8 of Arduino


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

void SPI2_Inits(void)
{
	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;           //Full Duplex
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;                        //software slave management disabled

	SPI_Init(&SPI2handle);
}


//initialize GPIO for interrupt
void GPIO_InterruptPinInit(void)
{
	GPIO_Handle SPI_Int_Pin;
	memset(&SPI_Int_Pin,0,sizeof(SPI_Int_Pin));

	SPI_Int_Pin.pGPIOx = GPIOD;
	SPI_Int_Pin.GPIOx_PinConfig.PinNo = GPIO_PIN6;
	SPI_Int_Pin.GPIOx_PinConfig.PinMode = GPIO_MODE_INTRPT_FE_TRIG; //interrupt will be triggered on falling edge of signal
	SPI_Int_Pin.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_LOW;
	SPI_Int_Pin.GPIOx_PinConfig.PinPUPDCtrl = GPIO_PIN_PU;

	GPIO_Init(&SPI_Int_Pin);

    //configure interrupt and its priority
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5,NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,ENABLE);
}


int main(void)
{
	uint8_t dummy = 0xff; //dummy value

    //initialize GPIO for interrupt
	GPIO_InterruptPinInit();

	//initialize GPIO for SPI2
	SPI2_GPIOInits();

	//initialize SPI2
	SPI2_Inits();

	//set SSOE to use internal NNS which is managed by hardware automatically
	SPI_SSOEConfig(SPI2,ENABLE);

    //enable interrupt for SPI2
	SPI_IRQInterruptConfig(IRQ_NO_SPI2,ENABLE);

	while(1)
    {
        //flag for receive
		receive_stop = 0;

        //wait till data is received
		while(!dataAvailable);

        //disable interrupt in GPIO
		GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,DISABLE);

		//enable SPI2 peripheral
		SPI_Enable(SPI2,ENABLE);

		while(!receive_stop)
		{
			//wait till SPI is not busy in TX
			while(SPI_SendDataIT(&SPI2handle,&dummy,1) == SPI_BUSY_IN_TX);//send dummy byte

            //wait till SPI is not busy in RX
			while(SPI_ReceiveDataIT(&SPI2handle,&ReadByte,1) == SPI_BUSY_IN_RX);
		}

		//wait till SPI is not busy
		while(SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG));

		//Disable SPI2 peripheral
		SPI_Enable(SPI2,DISABLE);

		printf("Received Data = %s\n",receive_buffer);

		dataAvailable = 0;

		GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,ENABLE);
	}
	return 0;
}

/* Runs when a data byte is received from the peripheral over SPI*/
void SPI2_IRQHandler(void)
{
	SPI_IRQHandling(&SPI2handle);
}


void SPI_ApplicationEventCallback(SPI_Handle *pSPIHandle,uint8_t AppEv)
{
	static uint32_t i = 0;

	if(AppEv == SPI_EVENT_RX_CMPLT)
	{
				receive_buffer[i++] = ReadByte;
				if(ReadByte == '\0' || ( i == 500)){
					receive_stop = 1;
					receive_buffer[i-1] = '\0';
					i = 0;
				}
	}
}

/* Slave data available interrupt handler */
void EXTI9_5_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN6);
	dataAvailable = 1;
}
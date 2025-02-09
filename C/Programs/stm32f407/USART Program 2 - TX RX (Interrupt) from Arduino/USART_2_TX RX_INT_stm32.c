#include<stdio.h>
#include<string.h>
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_usart_driver.h"

USART_Handle usart2_handle;

//data to send to Arduino
char* msg[3] = {"Hello", "This is STM32" , "USART RX Testign using Interrupts"};

//data buffer for reception from Arduino
char rx_buf[1024] ;

//reception complete flag
uint8_t rxCmplt = RESET;
uint8_t g_data = 0;


//initialize USART2
void USART2_Init(void)
{
	usart2_handle.pUSARTx = USART2;
	usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
	usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart2_handle.USART_Config.USART_Mode = USART_MODE_TXRX;
	usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART_Init(&usart2_handle);
}

//initialize GPIO for USART2
void USART2_GPIOInit(void)
{
	GPIO_Handle usart_gpios;

	usart_gpios.pGPIOx = GPIOA;
	usart_gpios.GPIOx_PinConfig.PinMode = GPIO_MODE_ALTFUN;
	usart_gpios.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIOx_PinConfig.PinPUPDCtrl = GPIO_PIN_PU;
	usart_gpios.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
	usart_gpios.GPIOx_PinConfig.PinAltFuncMode = 7;

	usart_gpios.GPIOx_PinConfig.PinNo  = GPIO_PIN2;
	GPIO_Init(&usart_gpios);

	usart_gpios.GPIOx_PinConfig.PinNo = GPIO_PIN3;
	GPIO_Init(&usart_gpios);
}


//initialize GPIO for button and LED
void GPIO_ButtonInit(void)
{
	GPIO_Handle GPIOBtn,GpioLed;

	//button
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIOx_PinConfig.PinNo = GPIO_PIN0;
	GPIOBtn.GPIOx_PinConfig.PinMode = GPIO_MODE_INP;
	GPIOBtn.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
	GPIOBtn.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
	GPIO_Init(&GPIOBtn);

	//LED
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIOx_PinConfig.PinNo = GPIO_PIN12;
	GpioLed.GPIOx_PinConfig.PinMode = GPIO_MODE_OP;
	GpioLed.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
	GpioLed.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_OD;
	GpioLed.GPIOx_PinConfig.PinPUPDCtrl = GPIO_NO_PUPD;
	GPIO_Init(&GpioLed);
}

void delay(void)
{
	for(uint32_t i=0; i <200000; i++);
}



int main(void)
{
	uint32_t count = 0; //count for tracking tx message array index

	USART2_GPIOInit();

    USART2_Init();

    USART_IRQInterruptConfig(IRQ_NO_USART2,ENABLE);

    USART_PeripheralControl(USART2,ENABLE);

    printf("Application is running\n");

    while(1)
    {
		//wait till button is pressed
		while(!GPIO_ReadFromInputPin(GPIOA,GPIO_PIN0));

		delay();

		count = count % 3;

        //enable data reception in interrupt mode
		while(USART_ReceiveDataIT(&usart2_handle,rx_buf,strlen(msg[count])) != USART_READY);

		//send data in msg array to Arduino in normal mode
    	USART_SendData(&usart2_handle,(uint8_t*)msg[count],strlen(msg[count]));

    	printf("Data Sent: %s\n",msg[count]);


    	//rxCmplt will be set via application callback
        //wait till reception from Arduino is complete
    	while(rxCmplt != SET);

    	//terminate the string received from Arduino
    	rx_buf[strlen(msg[count])+ 1] = '\0';

    	//Print what we received from the arduino
    	printf("Data Received: %s\n",rx_buf);

    	//reset the reception flag
    	rxCmplt = RESET;

    	//proceed to the next data in msg
    	count ++;
    }

	return 0;
}


//USART2 IRQ Handler
void USART2_IRQHandler(void)
{
	USART_IRQHandling(&usart2_handle);
}




//Application Event callback implementation
void USART_ApplicationEventCallback(USART_Handle *pUSARTHandle,uint8_t ApEv)
{
   if(ApEv == USART_EVENT_RX_CMPLT)
   {
	rxCmplt = SET;
   }
   else if (ApEv == USART_EVENT_TX_CMPLT)
   {
	   ;
   }
}
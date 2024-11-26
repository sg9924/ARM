#include<stdio.h>
#include<string.h>
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_i2c_driver.h"

#define MY_ADDR        0x70
#define SLAVE_ADDR     0x80

//STM32: Master ; Arduino Uno: Slave ; Receive data from Slave via Interrupts
//PB6:  SCL
//PB7:  SDA

uint8_t rxComplt = RESET;  //flag
uint8_t rcv_buf[32];       //data to be received
I2C_Handle I2C1Handle;




void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}



//initialize GPIO for I2C
void I2C1_GPIOInits(void)
{
	GPIO_Handle I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIOx_PinConfig.PinMode = GPIO_MODE_ALTFUN;
	I2CPins.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIOx_PinConfig.PinPUPDCtrl = GPIO_PIN_PU;
	I2CPins.GPIOx_PinConfig.PinAltFuncMode = 4;
	I2CPins. GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;

	//SCL
	I2CPins.GPIOx_PinConfig.PinNo = GPIO_PIN6;
	GPIO_Init(&I2CPins);

	//SDA
	I2CPins.GPIOx_PinConfig.PinNo = GPIO_PIN7;
	GPIO_Init(&I2CPins);
}



//initialize I2C1
void I2C1_Inits(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
	I2C_Init(&I2C1Handle);
}



//initialize GPIO for Button & LED
void GPIO_ButtonInit(void)
{
	GPIO_Handle GPIOBtn,GpioLed;

	//Button
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



int main(void)
{
	uint8_t commandcode;   //command code
	uint8_t len;           //length of data to be recived

	printf("Application is running\n");

	GPIO_ButtonInit();

	//initialize GPIO for I2C1
	I2C1_GPIOInits();

	//initialize I2C1
	I2C1_Inits();

	//I2C1 IRQ Configuration for Event and Error
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV,ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER,ENABLE);

	//Enable I2C1 peripheral
	I2C_PeripheralControl(I2C1,ENABLE);

	//Enable ACK
	I2C_ManageAcking(I2C1,I2C_ACK_ENABLE);

	while(1)
	{
		//wait till button is pressed
		while(!GPIO_ReadIpPin(GPIOA,GPIO_PIN0));

		delay();

        //get length of string to be received
		commandcode = 0x51;
		while(I2C_MasterSendDataIT(&I2C1Handle,&commandcode,1,SLAVE_ADDR,I2C_ENABLE_SR)!=I2C_READY);   //send command via interrupts
		while(I2C_MasterReceiveDataIT(&I2C1Handle,&len,1,SLAVE_ADDR,I2C_ENABLE_SR)!=I2C_READY);         //receive data via interrupts

        //get the string data
		commandcode = 0x52;
		while(I2C_MasterSendDataIT(&I2C1Handle,&commandcode,1,SLAVE_ADDR,I2C_ENABLE_SR)!=I2C_READY);   //send command via interrupts

		rxComplt = RESET;//reset the receive complete flag
		while(I2C_MasterReceiveDataIT(&I2C1Handle,rcv_buf,len,SLAVE_ADDR,I2C_DISABLE_SR)!=I2C_READY);   //receive data via interrupts


		//wait till reception is completed
        while(rxComplt != SET)
        {
        	;
        }

		rcv_buf[len+1] = '\0';   //terminate the received string

		printf("Received Data : %s",rcv_buf);
		rxComplt = RESET;  //reset the flag
	}
}



//I2C1 Event Interrupt Default Handler
void I2C1_EV_IRQHandler (void)
{
	I2C_EV_IRQHandling(&I2C1Handle); //custom handler
}


//I2C1 Error Interrupt Default Handler
void I2C1_ER_IRQHandler (void)
{
	I2C_ER_IRQHandling(&I2C1Handle); //custom handler
}



//Application event Callback
void I2C_ApplicationEventCallback(I2C_Handle *pI2CHandle,uint8_t AppEv)
{
     if(AppEv == I2C_EV_TX_CMPLT)         //Event: Transmission is complete
     {
    	 printf("TX Complete\n");
     }
     else if (AppEv == I2C_EV_RX_CMPLT)   //Event: Reception is completed
     {
    	 printf("RX Complete\n");
    	 rxComplt = SET;
     }
     else if (AppEv == I2C_ERROR_AF)      //Error: ACK Failure
     {
    	printf("Error! ACK Failure\n");

        //disable interrupts and reset I2C1 for future transactions
    	I2C_CloseSendData(pI2CHandle);

    	//stop data transmission
    	I2C_GenerateStopCondition(I2C1);

    	while(1);
     }
}
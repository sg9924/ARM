#include<stdio.h>
#include<string.h>
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_i2c_driver.h"
#include "stm32f407xx_rcc_driver.h"


#define SLAVE_ADDR   0x20
#define MY_ADDR      SLAVE_ADDR


//STM32: Slave; Arduino: Master; Scenario: Slave send data to master via Interrupts
//PB6:  SCL
//PB7:  SDA


I2C_Handle I2C1Handle;
uint8_t Tx_buf[32]  = "STM32 - Slave, Arduino - Master, Slave Transmit Testing through Interrupts"; //string to be sent
uint8_t cmd;
uint32_t data_len;


void delay(void)
{
	for(uint32_t i=0; i<500000; i++);
}



//initialize GPIO for I2C1
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
    //initialize button
	GPIO_ButtonInit();

	//initialize GPIO for I2C1
	I2C1_GPIOInits();

	//initialize I2C1
	I2C1_Inits();

	//I2C Interrupt Configurations in NVIC
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV,ENABLE); //Event enable in NVIC
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER,ENABLE); //Error enable in NVIC

    //enable I2C interrupts for Event, Buffer & Error in I2C1
	I2C_SlaveEnableDisableCallbackEvents(I2C1,ENABLE);

	//Enable I2C1 peripheral
	I2C_PeripheralControl(I2C1,ENABLE);

	//enable ACK
	I2C_ManageAcking(I2C1,I2C_ACK_ENABLE);

	while(1);
}


void I2C1_EV_IRQHandler (void)
{
	I2C_EV_IRQHandling(&I2C1Handle);
}


void I2C1_ER_IRQHandler (void)
{
	I2C_ER_IRQHandling(&I2C1Handle);
}



void I2C_ApplicationEventCallback(I2C_Handle *pI2CHandle, uint8_t AppEv)
{
	static uint32_t cnt = 0;
	static uint32_t w_ptr = 0;

	if(AppEv == I2C_ERROR_AF)
	{
		//Master needs no more data, NACk will be sent and Slave should stop transmitting anymore data
		//if the current active code is 0x52 then dont invalidate
		if(!(cmd == 0x52))
			cmd = 0XFF;

		//reset count
		cnt = 0;

		//when w_ptr reaches data_len, then all data has been sent to master (Arduino)
		if(w_ptr >= (data_len))
		{
			w_ptr=0;       //reset w_ptr
			cmd = 0xFF;    //reset command code
		}

	}
	else if (AppEv == I2C_EV_STOP)
	{
		//This happens during end of SLave reception
		cnt = 0;
	}
	else if (AppEv == I2C_EV_DATA_REQ)
	{
		//Master requests for data, slave should start sending data.
		if(cmd == 0x51)
		{
			//Here we are sending 4 bytes of length information
			I2C_SlaveSendData(I2C1,((data_len >> ((cnt%4) * 8)) & 0xFF));
		    cnt++;
		}
		else if (cmd == 0x52)
		{
			//sending Tx_buf contents indexed by w_ptr variable
			I2C_SlaveSendData(I2C1,Tx_buf[w_ptr++]);
		}
	}else if (AppEv == I2C_EV_DATA_RCV)
	{
		//Slave receives data (command code) from Master
		 cmd = I2C_SlaveReceiveData(I2C1);
	}
}
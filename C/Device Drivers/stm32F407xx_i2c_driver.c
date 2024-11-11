#include "stm32f407xx_i2c_driver.h"


/*--------------------------------------------------------------------------------------------------------------------------*/
/***************************************** I2C Helper Functions Definitions Start *******************************************/

static void I2C_GenerateStartCondition(I2C_RegDef *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef *pI2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle *pI2CHandle);

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle *pI2CHandle);


// I2C - Generate Start Condition
static void I2C_GenerateStartCondition(I2C_RegDef *pI2Cx)
{
	pI2Cx->CR1 |= (1<<I2C_CR1_START);
}


// I2C - Execute Address Phase for Write Operation
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr<<1;
	SlaveAddr &= ~(1); //SlaveAddr is Slave address + r/nw bit=0
	pI2Cx->DR = SlaveAddr;
}


// I2C - Execute Address Phase for Read Operation
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr<<1;
	SlaveAddr |= 1; //SlaveAddr is Slave address + r/nw bit=1
	pI2Cx->DR = SlaveAddr;
}


// I2C - Clear Address Flag
static void I2C_ClearADDRFlag(I2C_Handle *pI2CHandle)
{
	uint32_t dummy_read;

	//check Device Mode
	if(pI2CHandle->pI2Cx->SR2 & (1<<I2C_SR2_MSL)) //For Master Mode
	{
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX) // if I2C is busy in reception
		{
			if(pI2CHandle->RxSize == 1) // if The reception data length has reduced to 1 (bytes)
			{
				//Disable ACK - set to 0
				I2C_ManageAcking(pI2CHandle->pI2Cx,DISABLE);

				//Clear ADDR -> Read SR1 & then Read SR2
				dummy_read = pI2CHandle->pI2Cx->SR1;
				dummy_read = pI2CHandle->pI2Cx->SR2;
				(void)dummy_read;
			}

		}
		else // If I2C is not busy in Reception
		{
			//clear the ADDR flag -> Read SR1 & then Read SR2
			dummy_read = pI2CHandle->pI2Cx->SR1;
			dummy_read = pI2CHandle->pI2Cx->SR2;
			(void)dummy_read;

		}

	}
	else //For Slave Mode
	{
		//Clear ADDR -> Read SR1 & Read SR2
		dummy_read = pI2CHandle->pI2Cx->SR1;
		dummy_read = pI2CHandle->pI2Cx->SR2;
		(void)dummy_read;
	}
}


// I2C - Generate Stop Condition
void I2C_GenerateStopCondition(I2C_RegDef *pI2Cx)
{
	pI2Cx->CR1 |= (1<<I2C_CR1_STOP);
}


// I2C - CallBack Event for I2C Interrupt Enable & Disable
void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef *pI2Cx,uint8_t mode)
 {
	 if(mode == ENABLE)
	 {
			pI2Cx->CR2 |= (1<<I2C_CR2_ITEVTEN);
			pI2Cx->CR2 |= (1<<I2C_CR2_ITBUFEN);
			pI2Cx->CR2 |= (1<<I2C_CR2_ITERREN);
	 }else
	 {
			pI2Cx->CR2 &= ~(1<<I2C_CR2_ITEVTEN);
			pI2Cx->CR2 &= ~(1<<I2C_CR2_ITBUFEN);
			pI2Cx->CR2 &= ~(1<<I2C_CR2_ITERREN);
	 }
 }

/****************************************** I2C Helper Functions Definitions End ********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** I2C API's Definitions Start ************************************************/

// I2C - Enable I2C Peripheral
void I2C_PeripheralControl(I2C_RegDef *pI2Cx, uint8_t mode)
{
	if(mode == ENABLE)
	{
		pI2Cx->CR1 |= (1<<I2C_CR1_PE);
		//pI2cBaseAddress->CR1 |= I2C_CR1_PE_Bit_Mask;
	}else
	{
		pI2Cx->CR1 &= ~(1<<0);
	}

}


// I2C - Enable Clock for I2C Peripheral
void I2C_ClkEnable(I2C_RegDef *pI2Cx, uint8_t mode)
{
	if(mode == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}else if (pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}else if (pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}
	}
	else
	{
		//TODO
	}

}


// I2C - Initalization
void I2C_Init(I2C_Handle *pI2CHandle)
{
	uint32_t tempreg = 0;

	//Enable Clock for I2C
	I2C_ClkEnable(pI2CHandle->pI2Cx,ENABLE);

	//Set ACK control Bit (Enable Acknowledging)
	tempreg |= pI2CHandle->I2C_Config.I2C_AckControl<<10;
	pI2CHandle->pI2Cx->CR1 = tempreg;

	//Configure FREQ
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value() /1000000U; // Get Configuration value for the Clock Frequency
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3F); // load the configurated value into I2C -> CR2

   //Configure the Host Device Address
	tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress<<1;
	tempreg |= (1<<14);
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	//CCR calculations
	uint16_t ccr_value = 0;
	tempreg = 0;

	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)  //For Standard Mode
	{
		ccr_value = (RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		tempreg |= (ccr_value & 0xFFF);
	}
    else //For Fast Mode
	{
		tempreg |= (1<<15);
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle<<14);
		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = (RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}else
		{
			ccr_value = (RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		tempreg |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->CCR = tempreg;

	//TRISE Configuration
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM) //For Standard Mode
	{

		tempreg = (RCC_GetPCLK1Value() /1000000U) + 1;
	}
    else //For Fast Mode
	{
		tempreg = ((RCC_GetPCLK1Value() * 300) / 1000000000U) + 1;
	}

	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);
}


// I2C - Deintialization
void I2C_DeInit(I2C_RegDef *pI2Cx)
{
    //TODO
}

// I2C - Get Flag Status
uint8_t I2C_GetFlagStatus(I2C_RegDef *pI2Cx , uint32_t FlagName)
{
	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}


// I2C - Master Send Data
void I2C_MasterSendData(I2C_Handle *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm that start generation is completed by checking the SB flag in the SR1
	//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB)));

	//3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits)
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,SlaveAddr);

	//4. Confirm that address phase is completed by checking the ADDR flag in SR1
	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR)));

	//5. clear the ADDR flag according to its software sequence
	//   Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle);

	//6. send the data until len becomes 0

	while(Len > 0)
	{
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE)); //Wait till TXE is set
		pI2CHandle->pI2Cx->DR = *pTxbuffer;
		pTxbuffer++;
		Len--;
	}

	//7. when Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
	//   Note: TXE=1 , BTF=1 , means that both SR and DR are empty and next transmission should begin
	//   when BTF=1 SCL will be stretched (pulled to LOW)

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE));

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_BTF));


	//8. Generate STOP condition and master need not to wait for the completion of stop condition.
	//   Note: generating STOP, automatically clears the BTF
	if(Sr == I2C_DISABLE_SR)
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

}


void I2C_MasterReceiveData(I2C_Handle *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr)
{

	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completed by checking the SB flag in the SR1
	//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB));

	//3. Send the address of the slave with r/nw bit set to R(1) (total 8 bits)
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,SlaveAddr);

	//4. wait until address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR));


	//procedure to read only 1 byte from slave
	if(Len == 1)
	{
		//Disable Acking
		I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//wait until  RXNE becomes 1
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE));

		//generate STOP condition
		if(Sr == I2C_DISABLE_SR)
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//read data in to buffer
		*pRxBuffer = pI2CHandle->pI2Cx->DR;

	}


    //procedure to read data from slave when Len > 1
	if(Len > 1)
	{
		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//read the data until Len becomes zero
		for (uint32_t i = Len; i > 0; i--)
		{
			//wait until RXNE becomes 1
			while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE));

			if(i == 2) //if last 2 bytes are remaining
			{
				//Disable Acking
				I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

				//generate STOP condition
				if(Sr == I2C_DISABLE_SR)
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

			}

			//read the data from data register in to buffer
			*pRxBuffer = pI2CHandle->pI2Cx->DR;

			//increment the buffer address
			pRxBuffer++;

		}

	}

	//re-enable ACKing
	if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);
	}

}

// I2C - IRQ Interrupt Config
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t mode)
{

	if(mode == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			*NVIC_ISER0 |= (1<<IRQNumber);

		}else if(IRQNumber > 31 && IRQNumber < 64) //32 to 63
		{
			//program ISER1 register
			*NVIC_ISER1 |= (1<<(IRQNumber % 32));
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			//program ISER2 register //64 to 95
			*NVIC_ISER3 |= (1<<(IRQNumber % 64));
		}
	}else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= (1<<IRQNumber);
		}else if(IRQNumber > 31 && IRQNumber < 64)
		{
			//program ICER1 register
			*NVIC_ICER1 |= (1<<(IRQNumber % 32));
		}
		else if(IRQNumber >= 6 && IRQNumber < 96)
		{
			//program ICER2 register
			*NVIC_ICER3 |= (1<<(IRQNumber % 64));
		}
	}

}


// I2C - IRQ Priority Config
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	// Locate IPR Register
	uint8_t iprx = IRQNumber / 4; // locate the block
	uint8_t iprx_section = IRQNumber %4; // locate the register section

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED); // configure the value

	*(NVIC_PR_BASE_ADDR + iprx) |=  (IRQPriority<<shift_amount); // load the value into the register setion

}
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
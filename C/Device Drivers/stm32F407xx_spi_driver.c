#include "stm32f407xx_spi_driver.h"


// SPI Clock Control
void SPI_Clk_Enable(SPI_RegDef *pSPIx, uint8_t mode)
{

	if(mode == ENABLE)
	{
        //Enable clock for the specified SPIx Peripheral
		if(pSPIx == SPI1)
			SPI1_PCLK_EN();
		else if (pSPIx == SPI2)
			SPI2_PCLK_EN();
		else if (pSPIx == SPI3)
			SPI3_PCLK_EN();
	}
	else
	{
		//TODO
	}
}

// SPI Enable
void SPI_Enable(SPI_RegDef *pSPIx, uint8_t mode)
{
	if(mode == ENABLE) // Enable SPI
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}else // Disable SPI
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}



// SPI SSI Configuration (Internal Slave Select)
void  SPI_SSIConfig(SPI_RegDef *pSPIx, uint8_t mode)
{
	if(mode == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI); //set bit
	}else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI); //clear bit
	}
}


// SPI SSOE Configuration (Slave Select Output Enable)
void  SPI_SSOEConfig(SPI_RegDef *pSPIx, uint8_t mode)
{
	if(mode == ENABLE)
	{
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE); //set bit
	}else
	{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE); //clear bit
	}
}

// SPI Peripheral Intialization
void SPI_Init(SPI_Handle *pSPIHandle)
{

	// Enable clock for SPI
	SPI_Clk_Enable(pSPIHandle->pSPIx, ENABLE);

	uint32_t tempreg = 0; // Temp variable
	//1. configure the SPI Mode (Master/Slave)
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	//2. configure the bus mode
    //Full Duplex
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//BIDI mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);

	}
    //Half Duplex
    else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//BIDI mode should be set
		tempreg |= (1 << SPI_CR1_BIDIMODE);
	}
    // Simplex Receive Only
    else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//BIDI mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
		//RXONLY bit must be set
		tempreg |= (1 << SPI_CR1_RXONLY);
	}

	// 3.Configure the SPI serial clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	//4. Configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	//5. Configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	//6. Configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;
	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;
	pSPIHandle->pSPIx->CR1 = tempreg;
}


// SPI Deinitialization
void SPI_DeInit(SPI_RegDef *pSPIx)
{
 //todo
}


// SPI Get Flag Status
uint8_t SPI_GetFlagStatus(SPI_RegDef *pSPIx , uint32_t Flag)
{
	if(pSPIx->SR & Flag)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}


// SPI Send Data
void SPI_SendData(SPI_RegDef *pSPIx,uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. check for TXE Flag whether data can be sent (Wait until TXE Flag becomes 1)
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG)  == FLAG_RESET);

		//2. check the DFF bit in CR1
		if((pSPIx->CR1 & (1 << SPI_CR1_DFF)))
		{
			//16 bit DFF
			//load the data into the Data Register
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;
		}else
		{
			//8 bit DFF
            //load the data into the Data Register
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}
}


// SPI Receive Data
void SPI_ReceiveData(SPI_RegDef *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
		{
			//1. wait until RXNE is set (RXNE should be 0)
			while(SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG)  == (uint8_t)FLAG_RESET);

			//2. check the DFF bit in CR1
			if((pSPIx->CR1 & (1 << SPI_CR1_DFF)))
			{
				//16 bit DFF
				//load the data from DR to Rxbuffer address
				 *((uint16_t*)pRxBuffer) = pSPIx->DR ;
				Len--;
				Len--;
				(uint16_t*)pRxBuffer++;
			}else
			{
				//8 bit DFF
                //load the data from DR to Rxbuffer address
				*(pRxBuffer) = pSPIx->DR ;
				Len--;
				pRxBuffer++;
			}
		}
}

// SPI Interrupt Configuration
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t mode)
{
	// Enable the specified Interrupt Number
	if(mode == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program NVIC ISER0 register
			*NVIC_ISER0 |= (1 << IRQNumber);

		}else if(IRQNumber > 31 && IRQNumber < 64) //32 to 63
		{
			//program NVIC ISER1 register
			*NVIC_ISER1 |= (1 << (IRQNumber % 32));
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			//program NVIC ISER2 register //64 to 95
			*NVIC_ISER3 |= (1 << (IRQNumber % 64));
		}
	}else // Disable the specified Interrupt Number
	{
		if(IRQNumber <= 31)
		{
			//program NVIC ICER0 register
			*NVIC_ICER0 |= (1 << IRQNumber);
		}else if(IRQNumber > 31 && IRQNumber < 64)
		{
			//program NVIC ICER1 register
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		}
		else if(IRQNumber >= 6 && IRQNumber < 96)
		{
			//program NVIC ICER2 register
			*NVIC_ICER3 |= (1 << (IRQNumber % 64));
		}
	}

}



// SPI Interrupt Priority Configuration
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	// get IPR value
	uint8_t iprx = IRQNumber/4; // get IPR Register no.
	uint8_t iprx_section = IRQNumber %4; // get IPR register section no.

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED); // calculate the configuration value for IPR (priority)

	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount); // load the configurated value into the specified IPR section which sets the priority

}
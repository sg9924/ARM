#include "stm32f407xx_spi_driver.h"

// Application Event Callback Function
__attribute__((weak)) SPI_ApplicationEventCallback(SPI_Handle *pSPIHandle,uint8_t AppEv) //
{

	//This is a weak implementation. the user application may override this function.
}

//Helper Functions

// SPI Close Transmission - Helper
void SPI_CloseTransmisson(SPI_Handle *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}


// SPI Close Reception - Helper
void SPI_CloseReception(SPI_Handle *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}


// SPI TX Interrupt Handle - Helper
static void spi_txe_interrupt_handle(SPI_Handle *pSPIHandle)
{
	// check the DFF bit in CR1
	if((pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)))
	{
		//16 bit DFF
		//1. load the data in to the DR
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
		(uint16_t*)pSPIHandle->pTxBuffer++;
	}else
	{
		//8 bit DFF
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}

	if(!(pSPIHandle->TxLen)) // if length of transmission data is zero
	{
		//Close SPI Transmission
		SPI_CloseTransmisson(pSPIHandle);

		//This prevents interrupts from setting the TXE flag
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);
	}

}


// SPI Rx Interrupt Handle - Helper
static void spi_rxne_interrupt_handle(SPI_Handle *pSPIHandle)
{
	// Check DFF bit in CR
	if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
	{
		//16 bit
		*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen -= 2;
		pSPIHandle->pRxBuffer++;
		pSPIHandle->pRxBuffer++;

	}else
	{
		//8 bit
		*(pSPIHandle->pRxBuffer) = (uint8_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}

	if(!(pSPIHandle->RxLen)) // if length of data to be recieved is zero
	{
		// Close SPI Reception
		SPI_CloseReception(pSPIHandle);

		//This prevents interrupts from setting the TXE flag
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
	}
}


// SPI Overwrite Error Handle - Helper
static void spi_ovr_err_interrupt_handle(SPI_Handle *pSPIHandle)
{
	uint8_t temp;

	// Clear the OVR flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}

	(void) temp; //

	// Inform the Application
	SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_OVR_ERR);
}



// SPI Clear OVR Flag - Helper
void SPI_ClearOVRFlag(SPI_RegDef *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp; //
}



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

// SPI Send Data for Interrupt
uint8_t SPI_SendDataIT(SPI_Handle *pSPIHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	// Get the state of the Transmitter
	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX) // if Tx state is not busy
	{
		//1. Get the TX Buffer address and length of the data to be sent 
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;

		//2. Mark SPI state as busy to prevent other codes from taking over the SPI until the transmission is over
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//3. Enable the TXEIE to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
	}
	
	return state;
}


// SPI Receive Data for Interrupt
uint8_t SPI_ReceiveDataIT(SPI_Handle *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	// Get state of the Receiver
	uint8_t state = pSPIHandle->RxState;

	if(state != SPI_BUSY_IN_RX) // if receiver is not busy
	{
		//1. Save the Rx buffer address and Len information in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;

		//2. Mark the SPI state as busy in reception so that no other code can take over same SPI peripheral until reception is over
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		//3. Enable the RXNEIE control bit to get interrupt whenever RXNEIE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

	}
	return state;
}


// SPI Interrupt Handler
void SPI_IRQHandling(SPI_Handle *pHandle)
{

	uint8_t temp1 , temp2;
	
	// Check for TXE
	// check TXE Flag whether the Buffer is empty
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	// check if Interrupt is enabled for Tx
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 && temp2) // if both TXE and TXE are valid
	{
		// handle TXE (Helper Function)
		spi_txe_interrupt_handle(pHandle);
	}

	// Check for RXNE
	// check RXNE Flag to receive data
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	// check if Interrupt is enabled for Rx
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 && temp2) // if both RXNE and RXNEIE are valid
	{
		//  handle RXNE (Helper Function)
		spi_rxne_interrupt_handle(pHandle);
	}

	// check for OVR flag
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if(temp1 && temp2)
	{
		// handle OVR error (Helper Function)
		spi_ovr_err_interrupt_handle(pHandle);
	}
}
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


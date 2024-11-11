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
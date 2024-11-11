#ifndef INC_STM32F407XX_SPI_DRIVER_H
#define INC_STM32F407XX_SPI_DRIVER_H

#include "stm32f407xx.h"

/*--------------------------------------------------------------------------------------------------------------------------*/
/********************************************* SPI Structure Definitions Start **********************************************/

// SPI Configuration Structure
typedef struct
{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
}SPI_Config;


// SPI Handle Structure
typedef struct
{
	SPI_RegDef	    *pSPIx;         /*!<This holds the base address of SPIx(x:0,1,2) peripheral>*/
	SPI_Config 	    SPIConfig;      /*!<>*/
	uint8_t 		*pTxBuffer;     /*!<To store the app. Tx buffer address>*/
	uint8_t 		*pRxBuffer;	    /*!<To store the app. Rx buffer address>*/
	uint32_t 		TxLen;		    /*!<To store Tx len>*/
	uint32_t 		RxLen;		    /*!<To store Tx len>*/
	uint8_t 		TxState;	    /*!<To store Tx state>*/
	uint8_t 		RxState;	    /*!<To store Rx state>*/
}SPI_Handle;
/*********************************************** SPI Structure Definitions End **********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/


#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
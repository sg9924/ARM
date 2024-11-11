#ifndef INC_STM32F407XX_I2C_DRIVER_H
#define INC_STM32F407XX_I2C_DRIVER_H

#include "stm32f407xx.h"



/*--------------------------------------------------------------------------------------------------------------------------*/
/********************************************* I2C Structure Definitions Start **********************************************/

// I2C Configuration
typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint8_t  I2C_DeviceAddress;
	uint8_t  I2C_AckControl;
	uint8_t  I2C_FMDutyCycle;

}I2C_Config;


// I2C Handler
typedef struct
{
	I2C_RegDef 	    *pI2Cx;             /*!<>*/
	I2C_Config 	    I2C_Config;         /*!<>*/
	uint8_t 		*pTxBuffer;         /*!<To store the app. Tx buffer address>*/
	uint8_t 		*pRxBuffer;	        /*!<To store the app. Rx buffer address>*/
	uint32_t 		TxLen;		        /*!<To store Tx len>*/
	uint32_t 		RxLen;		        /*!<To store Tx len>*/
	uint8_t 		TxRxState;	        /*!<To store Communication state>*/
	uint8_t 		DevAddr;	        /*!<To store slave/device address>*/
    uint32_t        RxSize;		        /*!<To store Rx size >*/
    uint8_t         Sr;			        /*!<To store repeated start value >*/
}I2C_Handle;

/********************************************** I2C Structure Definitions End ***********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/


#endif /*INC_STM32F407XX_I2C_DRIVER_H*/
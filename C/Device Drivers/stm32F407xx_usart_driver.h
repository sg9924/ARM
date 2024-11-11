#ifndef STM32F446X_UART_DRIVER_H_
#define STM32F446X_UART_DRIVER_H_

#include "stm32f407xx.h"

/*--------------------------------------------------------------------------------------------------------------------------*/
/******************************************** USART Structure Definitions Start *********************************************/

//Configuration
typedef struct
{
	uint8_t USART_Mode;
	uint32_t USART_Baud;
	uint8_t USART_NoOfStopBits;
	uint8_t USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFlowControl;
}USART_Config_t;


//Handle
typedef struct
{
	USART_RegDef_t *pUSARTx;
	USART_Config_t   USART_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
}USART_Handle_t;

/********************************************** USART Structure Definitions End *********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/


#endif /* STM32F446X_UART_DRIVER_H_ */
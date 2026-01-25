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
}USART_Config;


//Handle
typedef struct
{
	USART_RegDef *pUSARTx;
	USART_Config   USART_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
}USART_Handle;

/********************************************** USART Structure Definitions End *********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************* USART Definitions Start **************************************************/

//USART Modes
#define USART_MODE_ONLY_TX 	                0
#define USART_MODE_ONLY_RX 	                1
#define USART_MODE_TXRX  	                2


//USART Baud Rate
#define USART_STD_BAUD_1200					1200
#define USART_STD_BAUD_2400					400
#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 				115200
#define USART_STD_BAUD_230400 				230400
#define USART_STD_BAUD_460800 				460800
#define USART_STD_BAUD_921600 				921600
#define USART_STD_BAUD_2M 					2000000
#define SUART_STD_BAUD_3M 					3000000


//USART Parity Control
#define USART_PARITY_EN_ODD                 2
#define USART_PARITY_EN_EVEN                1
#define USART_PARITY_DISABLE                0

//USART Word Length
#define USART_WORDLEN_8BITS                 0
#define USART_WORDLEN_9BITS                 1

//USART Stop Bits
#define USART_STOPBITS_1                    0
#define USART_STOPBITS_0_5                  1
#define USART_STOPBITS_2                    2
#define USART_STOPBITS_1_5                  3


//USART HW Flow COntrol
#define USART_HW_FLOW_CTRL_NONE    	        0
#define USART_HW_FLOW_CTRL_CTS    	        1
#define USART_HW_FLOW_CTRL_RTS    	        2
#define USART_HW_FLOW_CTRL_CTS_RTS	        3


//USART Flags
#define USART_FLAG_TXE 			            (1 << USART_SR_TXE)
#define USART_FLAG_RXNE 		            (1 << USART_SR_RXNE)
#define USART_FLAG_TC 			            (1 << USART_SR_TC)
#define USART_FLAG_ORE 			            (1 << USART_SR_ORE)

//USART States
#define USART_BUSY_IN_RX                    1
#define USART_BUSY_IN_TX                    2
#define USART_READY                         0


#define 	USART_EVENT_TX_CMPLT            0
#define		USART_EVENT_RX_CMPLT            1
#define		USART_EVENT_IDLE                2
#define		USART_EVENT_CTS                 3
#define		USART_EVENT_PE                  4
#define		USART_ERR_FE     	            5
#define		USART_ERR_NE    	            6
#define		USART_ERR_ORE    	            7

/*************************************************** USART Definitions End **************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/******************************************** USART Functions Declarations Start *********************************************/

// USART Peripheral CLock Enable
void USART_CLkEnable(USART_RegDef *pUSARTx, uint8_t mode);

// USART Intialization & De-Intialization
void USART_Init(USART_Handle *pUSARTHandle);
void USART_DeInit(USART_Handle *pUSARTHandle);


// USART - Send & Receive Data
void USART_SendData(USART_Handle *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
void  USART_ReceiveData(USART_Handle *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len);


// USART - Send & receive Data for Interrupt
uint8_t USART_SendDataIT(USART_Handle *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len);


// USART - Interrupt Config
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t mode);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle *pUSARTHandle);


uint8_t USART_GetFlagStatus(USART_RegDef *pUSARTx, uint8_t StatusFlagName);
void USART_ClearFlag(USART_RegDef *pUSARTx, uint16_t StatusFlagName);
void USART_PeripheralControl(USART_RegDef *pUSARTx, uint8_t mode);
void USART_SetBaudRate(USART_RegDef *pUSARTx, uint32_t BaudRate);


//Application Callback
void USART_ApplicationEventCallback(USART_Handle *pUSARTHandle,uint8_t ApEv);

/********************************************** USART Function Declarations End **********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/


#endif /* STM32F446X_UART_DRIVER_H_ */
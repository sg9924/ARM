#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_rcc_driver.h"

/*--------------------------------------------------------------------------------------------------------------------------*/
/********************************************** USART API's Definitions Start ***********************************************/


// SART - Get Flag status
uint8_t USART_GetFlagStatus(USART_RegDef *pUSARTx, uint8_t StatusFlagName)
{
    if(pUSARTx->SR & StatusFlagName)
    {
    	return SET;
    }

   return RESET;
}



// USART - Clock Enable
void USART_CLkEnable(USART_RegDef *pUSARTx, uint8_t mode)
{
	if(mode == ENABLE)
	{
		if(pUSARTx == USART1)
		{
			USART1_PCCK_EN();
		}
        else if (pUSARTx == USART2)
		{
			USART2_PCCK_EN();
		}
        else if (pUSARTx == USART3)
		{
			USART3_PCCK_EN();
		}
		else if (pUSARTx == UART4)
		{
			UART4_PCCK_EN();
		}
	}
	else
	{
		//
	}

}


// USART - Set Baudrate
void USART_SetBaudRate(USART_RegDef *pUSARTx, uint32_t BaudRate)
{

	uint32_t PCLKx; //holds APB clock
	uint32_t usartdiv;

	//Mantissa and Fraction values
	uint32_t M_part,F_part;

    //temp register
    uint32_t tempreg=0;

  //Get value of APB bus clock into PCLKx
  if(pUSARTx == USART1 || pUSARTx == USART6)
  {
	   //USART1 and USART6 connect to APB2 bus
	   PCLKx = RCC_GetPCLK2Value();
  }
  else
  {
	   PCLKx = RCC_GetPCLK1Value();
  }

  //if OVER8 is 1
  if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
  {
	   //over sampling by 8
	   usartdiv = ((25*PCLKx) / (2*BaudRate));
  }
  else //OVER8 is not 1
  {
	   //over sampling by 16
	   usartdiv = ((25*PCLKx) / (4*BaudRate));
  }

  //Calculate Mantissa
  M_part = usartdiv/100;

  //Place Mantissa part in respective bit position (USART_BRR)
  tempreg |= M_part << 4;

  //Extract fractional part
  F_part = (usartdiv - (M_part * 100));

  //Calculate final fractional part
  //if OVER8 is 1
  if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
   {
	  //over sampling by 8
	  F_part = (((F_part * 8)+ 50) / 100)& ((uint8_t)0x07);

   }
   else //if OVER8 is not 1
   {
	   //over sampling by 16
	   F_part = (((F_part * 16) + 50) / 100) & ((uint8_t)0x0F);

   }

  //Place fractional part in respective bit position (USART_BRR)
  tempreg |= F_part;

  //Place the configurated value in temp register variable into BRR
  pUSARTx->BRR = tempreg;
}


// USART - Intialization
void USART_Init(USART_Handle *pUSARTHandle)
{

	//temp variable
	uint32_t tempreg=0;

    //CR1 configuration
	//Enable Clock for USART
	USART_CLkEnable(pUSARTHandle->pUSARTx,ENABLE);

	//Enable USART Tx and Rx engines according to the USART_Mode
    // Receiver Mode
	if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
	{
		tempreg|= (1 << USART_CR1_RE);
	}
    else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX) //Transmitter mode
	{
		tempreg |= (1 << USART_CR1_TE);
	}
    else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX) //Transmitter & Receiver
	{
		tempreg |= ((1 << USART_CR1_RE) | (1 << USART_CR1_TE));
	}

    //COnfigure Word Length
	tempreg |= pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M ;

    //Configuration Parity
	if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN) //Even Parity
	{
		//Enable Parity
		tempreg |= (1 << USART_CR1_PCE);

		//By Default EVEN parity will be selected when Parity is Enabled

	}else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD) //Odd Parity
	{
		//Enable Parity
	    tempreg |= (1 << USART_CR1_PCE);

	    //Configure for Odd Parity
	    tempreg |= (1 << USART_CR1_PS);

	}

    //Load the configurated value in the temp variable into CR1
	pUSARTHandle->pUSARTx->CR1 = tempreg;



    //CR2 Configuration
    //temp value
	tempreg=0;

	//Configure No. of Stop Bits
	tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP;

	//load the configurated value into CR2
	pUSARTHandle->pUSARTx->CR2 = tempreg;


    //CR3 Configuration
    //temp value
	tempreg=0;

	//Hardware Flow Control Configuration
	if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		//Enable CTS Flow
		tempreg |= (1 << USART_CR3_CTSE);
	}
    else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		//Enable RTS Flow
		tempreg |= (1 << USART_CR3_RTSE);
	}
    else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		//Enable CTS and RTS FLows
		tempreg |= (1 << USART_CR3_CTSE);
		tempreg |= (1 << USART_CR3_RTSE);
	}


    //load the configurated value into CR3
	pUSARTHandle->pUSARTx->CR3 = tempreg;

    //Set USART Baudrate
	USART_SetBaudRate(pUSARTHandle->pUSARTx,pUSARTHandle->USART_Config.USART_Baud);

}


void USART_PeripheralControl(USART_RegDef *pUSARTx, uint8_t cmd)
{
	if(cmd == ENABLE)
	{
		pUSARTx->CR1 |= (1 << 13);
	}
    else
	{
		pUSARTx->CR1 &= ~(1 << 13);
	}

}


// USART - Send Data
void USART_SendData(USART_Handle *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    //data buffer
	uint16_t *pdata;

   //Loop for sending data bytes of length Len
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Wait until TXE flag is set in the SR
		while(!USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TXE));

		//Check for 9 bit or 8 bit word length in a frame
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS) // 9 bit word length
		{
			//load DR with 2 bytes masking the bits other than first 9 bits
			pdata = (uint16_t*) pTxBuffer;
			pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

			//check for Parity in USART
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) // No Parity
			{
				//increment pTxBuffer twice
				pTxBuffer++;
				pTxBuffer++;
			}
			else // parity is used
			{
				//8bits of user data will be sent
				// 9th bit will be parity bit
				pTxBuffer++;
			}
		}
		else // 8 bit word length
		{

			pUSARTHandle->pUSARTx->DR = (*pTxBuffer  & (uint8_t)0xFF);

			//increment buffer address
			pTxBuffer++;
		}
	}

	//wait till TC flag is set in the SR
	while(!USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TC));
}


// USART - Receive Data
void USART_ReceiveData(USART_Handle *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
   //Loop for receiving data bytes of length Len
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Wait until RXNE flag is set in SR
		while(!USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_RXNE));

		//Check for 9 bit or 8 bit word length in a frame
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{

			//Check for Parity
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) // no Parity
			{

				//read only first 9 bits so mask the DR with 0x01FF
				*((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

				//increment buffer address
				pRxBuffer++;
				pRxBuffer++;
			}
			else // parity is used
			{
				//8 bits of user data and 1 parity bit
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
				pRxBuffer++;
			}
		}
		else // 8 bit word length
		{

			// check for parity
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) // parity is not used
			{
				//read 8 bits from DR
				 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
			}
			else // parity is used
			{
                //7 bits will be of user data and 1 bit is parity
				//read 7 bits - mask DR with 0X7F
				 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0x7F);

			}

			//increment pRxBuffer
			pRxBuffer++;
		}
	}
}


// USART - Send Data for Interrupt
uint8_t USART_SendDataIT(USART_Handle *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len)
{
    //Get TX state of USART
	uint8_t txstate = pUSARTHandle->TxBusyState;

	if(txstate != USART_BUSY_IN_TX) // not busy in transmission
	{
		pUSARTHandle->TxLen = Len; //length of data
		pUSARTHandle->pTxBuffer = pTxBuffer; //data buffer address
		pUSARTHandle->TxBusyState = USART_BUSY_IN_TX; //set state as busy in TX

		//Enable interrupt for TXE
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);

		//Enable interrupt for TC
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
	}

	return txstate;
}


// USART - Receive Data for Interrupt
uint8_t USART_ReceiveDataIT(USART_Handle *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len)
{
    //Get RX state of USART
	uint8_t rxstate = pUSARTHandle->RxBusyState;

	if(rxstate != USART_BUSY_IN_RX) // not busy in RX
	{
		pUSARTHandle->RxLen = Len; //length of data
		pUSARTHandle->pRxBuffer = pRxBuffer; //data buffer address
		pUSARTHandle->RxBusyState = USART_BUSY_IN_RX; //set state as busy in RX

		(void)pUSARTHandle->pUSARTx->DR;

		//Enable interrupt for RXNE
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);
	}

	return rxstate;
}


// USART - Clear Flag
void USART_ClearFlag(USART_RegDef *pUSARTx, uint16_t StatusFlagName)
{
	pUSARTx->SR &= ~(StatusFlagName);
}


// USART Interrupt Configuration
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t mode)
{

	if(mode == ENABLE) //interrupt enable
	{
		if(IRQNumber <= 31) //for IRQ number equal to or below 31
		{
			//program ISER0 register
			*NVIC_ISER0 |= (1 << IRQNumber);

		}else if(IRQNumber > 31 && IRQNumber < 64) //32 to 63
		{
			//program ISER1 register
			*NVIC_ISER1 |= (1 << (IRQNumber % 32));
		}
		else if(IRQNumber >= 64 && IRQNumber < 96) //64-95
		{
			//program ISER2 register //64 to 95
			*NVIC_ISER3 |= (1 << (IRQNumber % 64));
		}
	}
    else //interrupt disable
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= (1 << IRQNumber);
		}
        else if(IRQNumber > 31 && IRQNumber < 64)
		{
			//program ICER1 register
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		}
		else if(IRQNumber >= 6 && IRQNumber < 96)
		{
			//program ICER2 register
			*NVIC_ICER3 |= (1 << (IRQNumber % 64));
		}
	}
}


// USART - Interrupt Priority Configuration
void USART_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//Locate IPR Register
	uint8_t iprx = IRQNumber/4; //Locate the block
	uint8_t iprx_section = IRQNumber %4; //Locate the register section

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED) ;

	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);

}



void USART_IRQHandling(USART_Handle *pUSARTHandle)
{
    //Temp values
	uint32_t temp1,temp2,temp3;

    //data buffer
	uint16_t *pdata;

//------------------------------> Check for TC Flag
    //check state of TC in SR
	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_TC);

	 //check state of TCEIE bit
	temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_TCIE);

    // if TC and TCIE are set
	if(temp1 && temp2)
	{
		//close transmission and call application callback if TxLen is zero
		if (pUSARTHandle->TxBusyState == USART_BUSY_IN_TX) //if USART is busy in TX
		{
			//if TxLen is 0
			if(!pUSARTHandle->TxLen)
			{
				//clear the TC flag
				pUSARTHandle->pUSARTx->SR &= ~(1 << USART_SR_TC);

				//clear the TCIE control bit
                pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);

				//Reset the application state
				pUSARTHandle->TxBusyState = USART_READY;

				//Reset Data Buffer address to NULL
				pUSARTHandle->pTxBuffer = NULL;

				//Reset the data length to zero
				pUSARTHandle->TxLen = 0;

				//Application callback for TX complete Event
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_TX_CMPLT);
			}
		}
	}

//------------------------------> Check for TXE Flag
	//check state of TXE bit in SR
	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_TXE);

	//check state of TXEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_TXEIE);

    //if TXE & TXIE are set
	if(temp1 && temp2)
	{
		if(pUSARTHandle->TxBusyState == USART_BUSY_IN_TX) //if USART busy in TX
		{
			//send until Txlen reaches zero
			if(pUSARTHandle->TxLen > 0)
			{
				//check word length for 8 bit or 9 bit
				if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS) //if 9 bit
				{
					//load the DR with 2 bytes masking the bits other than first 9 bits
					pdata = (uint16_t*) pUSARTHandle->pTxBuffer;
					pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

					//check for Parity
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) //if parity is disabled
					{
						//9 bits of user data will be sent

                        // increment buffer address twice
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->pTxBuffer++;

                        //decrement length twice
						pUSARTHandle->TxLen-=2;
					}
					else //parity is enabled
					{
						//8 bits of user data will be sent
						//9th bit will be replaced by parity bit

                        //increment buffer once and decrement length once
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->TxLen-=1;
					}
				}
				else //if 8 bit word length
				{
                    //mask last 8 bits of data in buffer
					pUSARTHandle->pUSARTx->DR = (*pUSARTHandle->pTxBuffer & (uint8_t)0xFF);

					//increment buffer address once and decrement length once
					pUSARTHandle->pTxBuffer++;
					pUSARTHandle->TxLen-=1;
				}
			}

            //if TX length is 0
			if (pUSARTHandle->TxLen == 0)
			{
				//clear the TXEIE bit (disable interrupt for TXE flag)
				pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_TXEIE);
			}
		}
	}

//------------------------------> Check for RXNE Flag
    //check state of RXNE in SR
	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_RXNE);

    //check state of RXNEIE in CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_RXNEIE);

    //if RXNE and RXNEIE are set (interrupt due to RXNE)
	if(temp1 && temp2)
	{
		if(pUSARTHandle->RxBusyState == USART_BUSY_IN_RX) //USARt busy in RX
		{
            //length is not 0 (receive data until length is 0)
			if(pUSARTHandle->RxLen > 0)
			{
				//check word length for 8 or 9 bits
				if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
				{
					//check for parity
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) //parity is disabled
					{
						//all 9 bits will be user data
						//read only first 9 bits (mask of 0x1FF)
						*((uint16_t*) pUSARTHandle->pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

						//increment buffer address twice
						pUSARTHandle->pRxBuffer++;
						pUSARTHandle->pRxBuffer++;

                        //decrement length twice
						pUSARTHandle->RxLen-=2;
					}
					else //parity is enabled
					{
						//8 bits will be user data and 1 bit is parity
						*pUSARTHandle->pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);

                        //increment buffer address
						pUSARTHandle->pRxBuffer++;

                        //decrement length
						pUSARTHandle->RxLen-=1;
					}
				}
				else //if 8 bit word length
				{

					//check for parity
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) //parity is disabled
					{
						//all 8 bits will be user data
						//read 8 bits from DR
						*pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
					}
					else //parity is enabled
					{
						//7 bits will be user data and 1 bit is parity
						//read only 7 bits (mask 0X7F)
						*pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0x7F);
					}

					//increment buffer address
					pUSARTHandle->pRxBuffer++;

                    //decrement length
					pUSARTHandle->RxLen-=1;
				}


			}

            //if length is 0
			if(!pUSARTHandle->RxLen)
			{
				//disable RXNE Interrupt
				pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_RXNEIE);

                //reset USART state to Ready
				pUSARTHandle->RxBusyState = USART_READY;

                //application callback for RX complete event
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_RX_CMPLT);
			}
		}
	}

//------------------------------> Check for CTS Flag
//Note: CTS feature is not applicable for UART4 and UART5

	//check status of CTS bit in SR
	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_CTS);

	//check status of CTSE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR3 & (1 << USART_CR3_CTSE);

	//check status of CTSIE bit in CR3 (This is not available for UART4 & UART5)
	temp3 = pUSARTHandle->pUSARTx->CR3 & (1 << USART_CR3_CTSIE);

    //if CTS and CTSE is set
	if(temp1 && temp2)
	{
		//clear CTS flag in SR
		pUSARTHandle->pUSARTx->SR &=  ~(1 << USART_SR_CTS);

		//application callback for CTS event
		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_CTS);
	}

//------------------------------> Check for IDLE Flag
	//check status of IDLE flag in SR
	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_IDLE);

	//check state of IDLEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_IDLEIE);

    //if IDLE & IDLEIE are set
	if(temp1 && temp2)
	{
		//clear IDLE flag
		pUSARTHandle->pUSARTx->SR &= ~(1 << USART_SR_IDLE);

		//application callback for IDLE Event
		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_IDLE);
	}

//------------------------------> Check for ORE Flag
	//check status of ORE flag in SR
	temp1 = pUSARTHandle->pUSARTx->SR & USART_SR_ORE;

	//check status of RXNEIE in CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & USART_CR1_RXNEIE;

    //if ORE & RXNEIE are set
	if(temp1 && temp2)
	{
		//Clear ORE
        pUSARTHandle->pUSARTx->SR &= ~(1 << USART_SR_ORE);
        //USART_ClearFlag(pUSARTHandle->pUSARTx, USART_FLAG_ORE);

		//application callback for ORE Error
		USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_ORE);
	}

}
/*********************************************** USART API's Definitions End ************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/

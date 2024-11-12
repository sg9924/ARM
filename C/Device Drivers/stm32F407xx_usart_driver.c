#include "stm32f407xx_usart_driver.h"

/*--------------------------------------------------------------------------------------------------------------------------*/
/********************************************** USART API's Definitions Start ***********************************************/

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



/*********************************************** USART API's Definitions End ************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
#include"../inc/stm32f103xx.h"
#include"../inc/stm32f103xx_gpio.h"
#include"../inc/stm32f103xx_usart.h"
#include"../inc/stm32f103xx_afio.h"
#include"../inc/stm32f103xx_exti.h"
#include"../inc/stm32f103xx_nvic.h"

/*USART Pins
-USART2
--TX   PA2
--RX   PA3
--CK   PA4
--CTS  PA0
--RTS  PA1
-USART3
--TX   PB10
--RX   PB11
--CK   PB12
--CTS  PB13
--RTS  PB14
*/


extern uint32_t sysclock;
/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** USART API's Definitions Start **********************************************/

// USART - Peripheral Clock Enable
bool USART_PClk_init(USART_RegDef *pUSARTx, uint8_t mode)
{
	if(mode == ENABLE)
	{
		if(pUSARTx == USART1)
			USART1_PCLK_EN();
        else if (pUSARTx == USART2)
			USART2_PCLK_EN();
        else if (pUSARTx == USART3)
			USART3_PCLK_EN();
		else if (pUSARTx == UART4)
			UART4_PCLK_EN();
        else if (pUSARTx == UART5)
			UART5_PCLK_EN();
        return true;
	}
	else
        return false;

}

//Set Default USART Configuration
void USART_Config_Default(USART_Handle* pUSARTHandle)
{
    if(pUSARTHandle == NULL) return;

    pUSARTHandle->USARTx_Config.mode           = USART_MODE_TX;
    pUSARTHandle->USARTx_Config.baudrate       = USART_BAUDRATE_9600;
    pUSARTHandle->USARTx_Config.clock_phase    = USART_CPHA_LOW;
    pUSARTHandle->USARTx_Config.clock_polarity = USART_CPOL_LOW;
    pUSARTHandle->USARTx_Config.word_length    = USART_WORD_8BIT;
    pUSARTHandle->USARTx_Config.parity_control = USART_PARITY_DISABLE;
    pUSARTHandle->USARTx_Config.stop_bits      = USART_STOPBIT_1;

    pUSARTHandle->pRXBuffer = 0;
    pUSARTHandle->pTXBuffer = 0;
    pUSARTHandle->TXState   = USART_READY;
    pUSARTHandle->RXState   = USART_READY;

    USART_Clear_Errors(pUSARTHandle);
}

//Set USART Configuration
void USART_Configure(USART_Handle* pUSARTHandle, uint8_t mode, uint32_t baudrate, uint8_t clk_phase,
                     uint8_t clk_polarity, uint8_t word_len, uint8_t parity_ctrl, uint8_t parity_type, uint8_t stop_bits)
{
    if(pUSARTHandle == NULL) return;

    pUSARTHandle->USARTx_Config.mode           = mode;
    pUSARTHandle->USARTx_Config.baudrate       = baudrate;
    pUSARTHandle->USARTx_Config.clock_phase    = clk_phase;
    pUSARTHandle->USARTx_Config.clock_polarity = clk_polarity;
    pUSARTHandle->USARTx_Config.word_length    = word_len;
    pUSARTHandle->USARTx_Config.parity_control = parity_ctrl;
    pUSARTHandle->USARTx_Config.parity_type    = parity_type;
    pUSARTHandle->USARTx_Config.stop_bits      = stop_bits;
    pUSARTHandle->USARTx_Config.type           = USART_TYPE_DEFAULT;

    pUSARTHandle->pRXBuffer = 0;
    pUSARTHandle->pTXBuffer = 0;
    pUSARTHandle->TXState   = USART_READY;
    pUSARTHandle->RXState   = USART_READY;

    USART_Clear_Errors(pUSARTHandle);
}



//USART Set Baudrate
void USART_SetBaudRate(USART_Handle* pUSARTHandle)
{
    if(pUSARTHandle == NULL) return;

    uint16_t uartdiv            = RCC_Get_PCLK1()/(pUSARTHandle->USARTx_Config.baudrate);
    pUSARTHandle->pUSARTx->BRR |= ((uartdiv/16) << USART_BRR_DIV_MANTISSA) | ((uartdiv%16) << USART_BRR_DIV_FRACTION);
}                                                                                               




//USART Init
bool USART_init(USART_Handle* pUSARTHandle, GPIO_Handle* pGPIOHandle,  USART_RegDef* pUSARTx)
{
    if(pUSARTHandle == NULL || pGPIOHandle == NULL) return false;

    pUSARTHandle->pUSARTx     = pUSARTx;
    pUSARTHandle->pGPIOHandle = pGPIOHandle;

    USART_Clear_Errors(pUSARTHandle);

    //USART2
    if(pUSARTHandle->pUSARTx == USART2)
    {
        pUSARTHandle->pGPIOHandle->pGPIOx = GPIOA;

        //Initialize GPIOA pins
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinOutputSpeed = GPIO_OP_SPEED_10;
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinMode        = GPIO_MODE_AF;
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinConfigType  = GPIO_CONFIG_AF_OP_PP;

        //PA2
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinNo = 2;
        GPIO_Init(pUSARTHandle->pGPIOHandle);

        //PA3
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinNo = 3;
        GPIO_Init(pUSARTHandle->pGPIOHandle);
    }
    else
        return false;

    //1. Enable GPIOA Peripheral Clock
    GPIO_PClk_init(pUSARTHandle->pGPIOHandle->pGPIOx, ENABLE);

    //2. Enable USART2 Peripheral Clock
    USART_PClk_init(pUSARTHandle->pUSARTx, ENABLE);

    //3. Configure baudrate
    USART_SetBaudRate(pUSARTHandle);

    //CR1 Configuration
    uint32_t temp = 0;
    //4. Enable USART for the configured mode
    if(pUSARTHandle->USARTx_Config.mode == USART_MODE_TX)
        temp |= 1<< USART_CR1_TE;
    else if(pUSARTHandle->USARTx_Config.mode == USART_MODE_RX)
        temp |= 1<< USART_CR1_RE;
    else if(pUSARTHandle->USARTx_Config.mode == USART_MODE_TXRX)
        temp |= (1<< USART_CR1_TE) | (1<<USART_CR1_RE);

    //5. Configure Word Length
    temp |= pUSARTHandle->USARTx_Config.word_length<<USART_CR1_M;

    //6. Configure Parity
    if(pUSARTHandle->USARTx_Config.parity_control == USART_PARITY_ENABLE)
    {
        //Enable Parity
        temp |= 1<<USART_CR1_PCE;
        if(pUSARTHandle->USARTx_Config.parity_type == USART_PARITY_ODD)
            temp |= 1<<USART_CR1_PS;
        //by default even parity will be selected when parity is enabled
    }

    //7. Enable USART2
    temp |= 1<< USART_CR1_UE;

    //load the configurated values into CR1
    pUSARTHandle->pUSARTx->CR1 = temp;


    //CR2 Configuration
    //8. Configure Stop Bits
    temp  = 0;
    temp |= pUSARTHandle->USARTx_Config.stop_bits<<USART_CR2_STOP;

    //load the configurated values into CR1
    pUSARTHandle->pUSARTx->CR1 |= temp;

    return true;
}



void USART_Clear_Errors(USART_Handle* pUH)
{
    if(pUH == NULL) return;

    pUH->error_flags = 0;
    pUH->ore_count   = 0;
    pUH->ne_count    = 0;
    pUH->fe_count    = 0;
    pUH->pe_count    = 0;
}



uint8_t USART_Get_Errors(USART_Handle* pUH)
{
    if(pUH == NULL) return -1;

    return pUH->error_flags;
}



uint16_t USART_Get_Error_Count(USART_Handle* pUH, uint8_t error_type)
{
    if(pUH == NULL) return -1;

    switch(error_type)
    {
        case USART_ERROR_ORE: return pUH->ore_count;
        case USART_ERROR_NE:  return pUH->ne_count;
        case USART_ERROR_FE:  return pUH->fe_count;
        case USART_ERROR_PE:  return pUH->pe_count;
        default: return 0;
    }
}



void USART_TXCallback_Add(USART_Handle* pUH, USART_TX_Callback callback)
{
    pUH->TX_Complete_Callback = callback;
}

void USART_RXCallback_Add(USART_Handle* pUH, USART_RX_Callback callback)
{
    pUH->RX_Complete_Callback = callback;
}

void USART_ErrorCallback_Add(USART_Handle* pUH, USART_Error_Callback callback)
{
    pUH->Error_Callback = callback;
}


void USART_TXCallback_Remove(USART_Handle* pUH)
{
    pUH->TX_Complete_Callback = NULL;
}

void USART_RXCallback_Remove(USART_Handle* pUH)
{
    pUH->RX_Complete_Callback = NULL;
}

void USART_ErrorCallback_Remove(USART_Handle* pUH)
{
    pUH->Error_Callback = NULL;
}







//USART Send Data (Blocking)
void USART_TX(USART_Handle* pUSARTHandle, uint8_t* pbuffer, uint32_t size)
{
    if(pUSARTHandle == NULL) return;

    pUSARTHandle->pTXBuffer = pbuffer;
    pUSARTHandle->TXLen     = size;
    pUSARTHandle->TXState   = USART_TX_BUSY;

    while(size--)
    {
        //wait till USART_SR->TXE becomes 1 to indicate that USART_DR is empty
        while(!(pUSARTHandle->pUSARTx->SR & 1<<USART_SR_TXE));

        //9 bit word length
        if(pUSARTHandle->USARTx_Config.word_length == USART_WORD_9BIT)
        {
            uint16_t *pdata           = (uint16_t*)pbuffer;
            pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF); //extract 9 bits from LSB

            if(pUSARTHandle->USARTx_Config.parity_type == USART_PARITY_DISABLE) //no parity
                pbuffer += 2; //increment buffer address twice
            else //with parity
                pbuffer++;    //increment buffer address once
        }
        else //8 bit word length
        {
            pUSARTHandle->pUSARTx->DR = (uint8_t)*pbuffer;
            pbuffer++;
        }
    }

    //wait till TC flag is set
    while(!(pUSARTHandle->pUSARTx->SR & 1<<USART_SR_TC));

    //clear TC flag
    pUSARTHandle->pUSARTx->SR &= ~(1<<USART_SR_TC);

    //reset state
    pUSARTHandle->TXState = USART_READY;
}

//USART Receive Data (Blocking)
void USART_RX(USART_Handle* pUSARTHandle, uint8_t* pbuffer, uint32_t size)
{
    if(pUSARTHandle == NULL) return;

    pUSARTHandle->pRXBuffer = pbuffer;
    pUSARTHandle->RXLen     = size;
    pUSARTHandle->RXState   = USART_RX_BUSY;

    while(size--)
    {
        //wait till USART_SR->TXE becomes 1 to indicate that USART_DR is not empty
        while(!(pUSARTHandle->pUSARTx->SR & 1<<USART_SR_RXNE));

        //9 bit word length
        if(pUSARTHandle->USARTx_Config.word_length == USART_WORD_9BIT)
        {
            if(pUSARTHandle->USARTx_Config.parity_type == USART_PARITY_DISABLE) //no parity
            {
                *((uint16_t*)pbuffer) = (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF); //extract 9 bits from LSB
                pbuffer += 2;
            }
            else //with parity
            {
                *(pbuffer) = pUSARTHandle->pUSARTx->DR;
                pbuffer++;
            }
        }
        else //8 bit word length
        {
            *(pbuffer) = (uint8_t)pUSARTHandle->pUSARTx->DR;
            pbuffer++;
        }
    }

    pUSARTHandle->RXState = USART_READY;
}

//USART Interrupt Configuration
void USART_IT_Config(USART_Handle* pUSARTHandle, uint8_t mode)
{
    if(pUSARTHandle == NULL) return;

    pUSARTHandle->USARTx_Config.type = USART_TYPE_IT;

    if(mode == ENABLE)
    {
        if(pUSARTHandle->pUSARTx == USART1)
            nvic_intrpt_enable(USART1_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == USART2)
            nvic_intrpt_enable(USART2_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == USART3)
            nvic_intrpt_enable(USART3_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == UART4)
            nvic_intrpt_enable(UART4_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == UART5)
            nvic_intrpt_enable(UART5_IRQ_NO);
    }
    else if(mode == DISABLE)
    {
        if(pUSARTHandle->pUSARTx == USART1)
            nvic_intrpt_disable(USART1_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == USART2)
            nvic_intrpt_disable(USART2_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == USART3)
            nvic_intrpt_disable(USART3_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == UART4)
            nvic_intrpt_disable(UART4_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == UART5)
            nvic_intrpt_disable(UART5_IRQ_NO);
    }
}

//USART Send Data through Interrupts (Non-Blocking)
uint8_t USART_TX_IT(USART_Handle* pUSARTHandle, uint8_t* pbuffer, uint32_t size)
{
    //Checks
    if(pUSARTHandle == NULL || pbuffer == NULL || size == 0)
        return USART_ERROR;

    //1. Get Current state of USART
    uint8_t TX_State = pUSARTHandle->TXState;

    //USART not busy in TX
    if(TX_State != USART_TX_BUSY)
    {
        //2. Get Data size and buffer address
        pUSARTHandle->TXLen     = size;
        pUSARTHandle->pTXBuffer = pbuffer;

        //3. Set state as busy in TX
        pUSARTHandle->TXState = USART_TX_BUSY;

        //4. Enable TX Interrupt
        pUSARTHandle->pUSARTx->CR1 |= 1<<USART_CR1_TXEIE;

        //5. Enable TC Interrupt
        pUSARTHandle->pUSARTx->CR1 |= 1<<USART_CR1_TCIE;
    }

    return pUSARTHandle->TXState;
}

//USART Receive Data through Interrupts (Non-Blocking)
uint8_t USART_RX_IT(USART_Handle* pUSARTHandle, uint8_t* pbuffer, uint32_t size)
{
    //Checks
    if(pUSARTHandle == NULL || pbuffer == NULL || size == 0)
        return USART_ERROR;
    
    //1. Get Current state of USART
    uint8_t RX_State = pUSARTHandle->RXState;

    //USART not busy in RX
    if(RX_State != USART_RX_BUSY)
    {
        //2. Get Data size and buffer address
        pUSARTHandle->RXLen     = size;
        pUSARTHandle->pRXBuffer = pbuffer;

        //3. Set state as busy in TX
        pUSARTHandle->RXState = USART_RX_BUSY;

        //4. Enable TX Interrupt
        pUSARTHandle->pUSARTx->CR1 |= 1<<USART_CR1_RXNEIE;
    }

    return pUSARTHandle->RXState;
}



void handle_ORE(USART_Handle* pUSARTHandle)
{
    if(pUSARTHandle == NULL) return;

    //overrun error detected
    if(pUSARTHandle->pUSARTx->SR & (1<<USART_SR_ORE))
    {
        //RXNE is set
        //last valid data is stored in the RDR register which can be read
        if(pUSARTHandle->pUSARTx->SR & (1<<USART_SR_RXNE))
        {
            //Discard current received byte and wait for next one
            //Read DR to clear ORE flag
            uint8_t dummy = pUSARTHandle->pUSARTx->DR;
        }
        //RXNE is 0
        //last valid data has already been read
        //case 1: last valid data is read at same time when new (lost) data is received
        //case 2: new data is received during the reading sequence between SR read access and DR read access
        else
            ;//TODO
        
        //Set error flag
        pUSARTHandle->error_flags |= USART_ERROR_ORE;
        
        //Increment counter
        pUSARTHandle->ore_count++;

        //Callback
        if(pUSARTHandle->Error_Callback != NULL)
            pUSARTHandle->Error_Callback(pUSARTHandle, USART_ERROR_ORE);
    }
}



void handle_NE(USART_Handle* pUSARTHandle)
{
    if(pUSARTHandle == NULL) return;

    //Read SR
    if(pUSARTHandle->pUSARTx->SR & (1<<USART_SR_NE))
    {
        //Read DR to clear NE flag
        //the current received byte contains noise and is invalid
        //we discard it
        uint8_t dummy = pUSARTHandle->pUSARTx->DR;
    }

    //Set error flag
    pUSARTHandle->error_flags |= USART_ERROR_NE;
    
    //Increment counter
    pUSARTHandle->ne_count++;

    //Callback
    if(pUSARTHandle->Error_Callback != NULL)
        pUSARTHandle->Error_Callback(pUSARTHandle, USART_ERROR_NE);
}



void handle_FE(USART_Handle* pUSARTHandle)
{
    if(pUSARTHandle == NULL) return;

    //Read SR
    if(pUSARTHandle->pUSARTx->SR & (1<<USART_SR_FE))
    {
        //Read DR to clear FE flag
        uint8_t dummy = pUSARTHandle->pUSARTx->DR;
        
        //Abort current RX transaction
        pUSARTHandle->RXLen   = 0;
        pUSARTHandle->RXState = USART_READY;
    }

    //Set error flag
    pUSARTHandle->error_flags |= USART_ERROR_FE;
    
    //Increment counter
    pUSARTHandle->fe_count++;

    //Callback
    if(pUSARTHandle->Error_Callback != NULL)
        pUSARTHandle->Error_Callback(pUSARTHandle, USART_ERROR_FE);
}



void handle_PE(USART_Handle* pUSARTHandle)
{
    if(pUSARTHandle == NULL) return;
    
    if(pUSARTHandle->pUSARTx->SR & (1<<USART_SR_PE))
    {
        //Read DR to clear PE flag
        uint8_t dummy = pUSARTHandle->pUSARTx->DR;
        
        //Abort current RX transaction (config mismatch)
        pUSARTHandle->RXLen   = 0;
        pUSARTHandle->RXState = USART_READY;
    }

    //Set error flag
    pUSARTHandle->error_flags |= USART_ERROR_PE;
    
    //Increment counter
    pUSARTHandle->pe_count++;

    //Callback
    if(pUSARTHandle->Error_Callback != NULL)
        pUSARTHandle->Error_Callback(pUSARTHandle, USART_ERROR_PE);
}








void USART_IRQ_Handler(USART_Handle* pUSARTHandle)
{
    //Checks
    if(pUSARTHandle == NULL)
        return;


    /*********ORE Flag *********/
    if(pUSARTHandle->pUSARTx->SR & (1<<USART_SR_ORE))
    {
        handle_ORE(pUSARTHandle);

        //TODO
        //Set Error flag in USART Handle to check later
        //count of no of ORE
    }


    /*********NE Flag *********/
    if(pUSARTHandle->pUSARTx->SR & (1<<USART_SR_NE))
    {
        handle_NE(pUSARTHandle);

        //TODO
        //Set Error flag in USART Handle to check later
        //count of no of NE
    }


    /*********FE Flag *********/
    if(pUSARTHandle->pUSARTx->SR & (1<<USART_SR_FE))
    {
        handle_FE(pUSARTHandle);

        //TODO
        //Add error message in handle
    }



    /*********PE Flag *********/
    if(pUSARTHandle->pUSARTx->SR & (1<<USART_SR_PE))
    {
        handle_PE(pUSARTHandle);

        //TODO
        //Add error message in handle
    }




    /********* TC Flag *********/
    uint32_t temp1 = pUSARTHandle->pUSARTx->SR & (1<<USART_SR_TC);
    uint32_t temp2 = pUSARTHandle->pUSARTx->CR1 & (1<<USART_CR1_TCIE);

    //If TC and TCIE are set
    if(temp1 && temp2)
    {
        //1. Check if USART is busy in TX
        if(pUSARTHandle->TXState == USART_TX_BUSY)
        {
            //2. if data length is zero
            if(pUSARTHandle->TXLen == 0)
            {
                //clear TC flag
                pUSARTHandle->pUSARTx->SR &= ~(1<<USART_SR_TC);

                //disable TC Interrupt
                pUSARTHandle->pUSARTx->CR1 &= ~(1<<USART_CR1_TCIE);
                
                //Reset USART State
                pUSARTHandle->TXState = USART_READY;

                //Reset USART Buffer Address
                pUSARTHandle->pTXBuffer = 0;

                //Reset Data Length
                pUSARTHandle->TXLen = 0;

                //Callback
                if(pUSARTHandle->TX_Complete_Callback != NULL)
                    pUSARTHandle->TX_Complete_Callback(pUSARTHandle);
            }
        }
    }

    /********* TXE Flag *********/
    temp1 = pUSARTHandle->pUSARTx->SR & (1<<USART_SR_TXE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1<<USART_CR1_TXEIE);

    if(temp1 && temp2)
    {
        //1.Check state of USART
        if(pUSARTHandle->TXState == USART_TX_BUSY)
        {
            //2. Send Data until its length becomes 0
            if(pUSARTHandle->TXLen > 0)
            {
                if(pUSARTHandle->USARTx_Config.word_length == USART_WORD_9BIT)
                {
                    uint16_t *pdata           = (uint16_t*)pUSARTHandle->pTXBuffer;
                    pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);  //extract 9 bits from LSB

                    if(pUSARTHandle->USARTx_Config.parity_type == USART_PARITY_DISABLE) //no parity
                    {
                        pUSARTHandle->pTXBuffer += 2; //increment buffer address twice: 8 bits first and then 1 bit
                        pUSARTHandle->TXLen     -= 2;
                    }
                    else //with parity
                    {
                        pUSARTHandle->pTXBuffer++;    //increment buffer address once
                        pUSARTHandle->TXLen--;
                    }
                }
                else //8 bit word length
                {
                    pUSARTHandle->pUSARTx->DR = *pUSARTHandle->pTXBuffer;
                    pUSARTHandle->pTXBuffer++;
                    pUSARTHandle->TXLen--;
                }
            }
        }

        //Disable interrupt for TXE when TX length becomes 0
        if(pUSARTHandle->TXLen == 0)
        {
            pUSARTHandle->pUSARTx->CR1 &= ~(1<<USART_CR1_TXEIE);
        }
    }


    /********* RXNE Flag *********/
    temp1 = pUSARTHandle->pUSARTx->SR & (1<<USART_SR_RXNE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1<<USART_CR1_RXNEIE);
 
    if(temp1 && temp2)
    {
        //1. Check state of USART
        if(pUSARTHandle->RXState == USART_RX_BUSY)
        {
            //2. Receive Data until its length becomes 0
            if(pUSARTHandle->RXLen > 0)
            {
                if(pUSARTHandle->USARTx_Config.word_length == USART_WORD_9BIT)
                {
                    if(pUSARTHandle->USARTx_Config.parity_type == USART_PARITY_DISABLE)
                    {
                        *((uint16_t*)pUSARTHandle->pRXBuffer) = (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF);
                        pUSARTHandle->pRXBuffer += 2;
                        pUSARTHandle->RXLen     -= 2;
                    }
                    else
                    {
                        *(pUSARTHandle->pRXBuffer) = pUSARTHandle->pUSARTx->DR;
                        pUSARTHandle->pRXBuffer++;
                        pUSARTHandle->RXLen--;
                    }
                }
                else //8 bit word length
                {
                    *(pUSARTHandle->pRXBuffer) = (uint8_t)pUSARTHandle->pUSARTx->DR;
                    pUSARTHandle->pRXBuffer++;
                    pUSARTHandle->RXLen--;
                }
            }
 
            //Disable interrupt for RXNE when RX length becomes 0
            if(pUSARTHandle->RXLen == 0)
            {
                pUSARTHandle->pUSARTx->CR1 &= ~(1<<USART_CR1_RXNEIE);
                pUSARTHandle->RXState       = USART_READY;
                pUSARTHandle->pRXBuffer     = NULL;

                //Callback
                if(pUSARTHandle->RX_Complete_Callback != NULL)
                    pUSARTHandle->RX_Complete_Callback(pUSARTHandle);
            }
        }
    }
}

/*********************************************** USART API's Definitions End ************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
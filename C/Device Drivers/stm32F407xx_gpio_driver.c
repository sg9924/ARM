#include "stm32F407xx_gpio_driver.h"
#include<stdio.h>


/*--------------------------------------------------------------------------------------------------------------------------*/
/****************************************** GPIO Helper Function Definitions Start ******************************************/

// View GPIO Details
void GPIO_Details(GPIO_Handle* pGPIOHandle)
{
	// GPIO Registers Details
    printf("\nAFR:     %lu", (unsigned long int)pGPIOHandle->pGPIOx->AFR);
    printf("\nBSRR:    %lu", (unsigned long int)pGPIOHandle->pGPIOx->BSRR);
    printf("\nIDR:     %lu", (unsigned long int)pGPIOHandle->pGPIOx->IDR);
    printf("\nLCKR:    %lu", (unsigned long int)pGPIOHandle->pGPIOx->LCKR);
    printf("\nMODER:   %lu", (unsigned long int)pGPIOHandle->pGPIOx->MODER);
    printf("\nODR:     %lu", (unsigned long int)pGPIOHandle->pGPIOx->ODR);
    printf("\nOSPEEDR: %lu", (unsigned long int)pGPIOHandle->pGPIOx->OSPEEDR);
    printf("\nOTYPER:  %lu", (unsigned long int)pGPIOHandle->pGPIOx->OTYPER);
    printf("\nPUPDR:   %lu", (unsigned long int)pGPIOHandle->pGPIOx->PUPDR);

	// GPIO Pin Config Details
    printf("\nPin No:                        %d", pGPIOHandle->GPIOx_PinConfig.PinNo);
    printf("\nPin Mode:                      %d", pGPIOHandle->GPIOx_PinConfig.PinMode);
    printf("\nPin Output Type:               %d", pGPIOHandle->GPIOx_PinConfig.PinOPType);
    printf("\nPin Speed:                     %d", pGPIOHandle->GPIOx_PinConfig.PinSpeed);
    printf("\nPin Pull Up Pull Down Config:  %d", pGPIOHandle->GPIOx_PinConfig.PinPUPDCtrl);
    printf("\nPin Alternate Function Mode:   %d", pGPIOHandle->GPIOx_PinConfig.PinAltFuncMode);
}


// Reset the Pin Config Details
void GPIO_Pin_Reset(GPIO_PinConfig* p)
{
    p->PinNo = 0;
    p->PinMode = 0;
    p->PinOPType = 0;
    p->PinSpeed = 0;
    p->PinPUPDCtrl = 0;
    p->PinAltFuncMode = 0;
}
/******************************************* GPIO Helper Function Definitions End *******************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** GPIO API's Definitions Start ***********************************************/

// GPIO Peripheral Clock Setup
void GPIO_Clk_Enable(GPIO_RegDef* pGPIOx, uint8_t setup_mode)
{
	if(setup_mode == ENABLE)
	{
		if(pGPIOx == GPIOA)
			GPIOA_PCLK_EN(); 				// Enable Clock for the specified GPIO Port using Macros
        else if (pGPIOx == GPIOB)
			GPIOB_PCLK_EN();
        else if (pGPIOx == GPIOC)
			GPIOC_PCLK_EN();
        else if (pGPIOx == GPIOD)
			GPIOD_PCLK_EN();
        else if (pGPIOx == GPIOE)
			GPIOE_PCLK_EN();
        else if (pGPIOx == GPIOF)
			GPIOF_PCLK_EN();
        else if (pGPIOx == GPIOG)
			GPIOG_PCLK_EN();
        else if (pGPIOx == GPIOH)
			GPIOH_PCLK_EN();
        else if (pGPIOx == GPIOI)
			GPIOI_PCLK_EN();
	}
	else
	{
		//TODO
	}

}

// GPIO Initialisation & De-Initialisation
void GPIO_Init(GPIO_Handle* pGPIOHandle)
{
	uint32_t temp=0; //temp register value

	//1. enable the peripheral clock for GPIO
	GPIO_Clk_Enable(pGPIOHandle->pGPIOx, ENABLE);

	//2. configure the mode of GPIO pin
	if(pGPIOHandle->GPIOx_PinConfig.PinMode <= GPIO_MODE_ANALOG)
	{
		//Non-Interrupt Mode
		temp = (pGPIOHandle->GPIOx_PinConfig.PinMode << (2 * pGPIOHandle->GPIOx_PinConfig.PinNo)); //storing the configuration value in temp
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIOx_PinConfig.PinNo)); 	//clearing the MODER register for the specified Pin no.
		pGPIOHandle->pGPIOx->MODER |= temp; //setting

	}else
	{
		//Interrupt Mode
		//a. Configure Falling or Rising edge or both modes in EXTI
		//b. Enable clock for SYSCFG
		//c. Configure EXTICR in SYSCFG for GPIO Port Selection
		//d. Enable EXTI Interrupt Delivery
		//Note: EXTI doesn't need clock enable operation

		if(pGPIOHandle->GPIOx_PinConfig.PinMode == GPIO_MODE_INTRPT_FE_TRIG) //Falling Edge Trigger Interrupt Mode Config
		{
			//1. Configure FTSR bit
			EXTI->FTSR |= (1 << pGPIOHandle->GPIOx_PinConfig.PinNo);
			//2. Clear RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIOx_PinConfig.PinNo);

		}else if (pGPIOHandle->GPIOx_PinConfig.PinMode == GPIO_MODE_INTRPT_RE_TRIG) //Rising Edge Trigger Interrupt Mode Config
		{
			//1. Configure RTSR bit
			EXTI->RTSR |= (1 << pGPIOHandle->GPIOx_PinConfig.PinNo);
			//2. Clear FTSR bit
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIOx_PinConfig.PinNo);

		}else if (pGPIOHandle->GPIOx_PinConfig.PinMode == GPIO_MODE_INTRPT_FERE_TRIG) //Falling & Rising Edge Trigger Interrupt Mode Config
		{
			//1. Configure RTSR bit
			EXTI->RTSR |= (1 << pGPIOHandle->GPIOx_PinConfig.PinNo);
			//2. Configure FTSR bit
			EXTI->FTSR |= (1 << pGPIOHandle->GPIOx_PinConfig.PinNo);
		}

		//configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIOx_PinConfig.PinNo/4; //get EXTICR Register number
		uint8_t temp2 = pGPIOHandle->GPIOx_PinConfig.PinNo%4; //get bit no. of the EXTICR register
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx); //get the corresponding code of the specified GPIO port
		SYSCFG_PCLK_EN(); // Enable Clock for SYSCFG Peripheral
		if(temp2<=4)
		SYSCFG->EXTICR[temp1] |= portcode << (temp2 * 4); //set the configuration value in EXITCRx register
		else
		printf("\nIncorrect Pin Number!");

		//enable EXTI Interrupt Delivery
		EXTI->IMR |= 1 << pGPIOHandle->GPIOx_PinConfig.PinNo;
	}

	//3. configure the speed
	temp = (pGPIOHandle->GPIOx_PinConfig.PinSpeed << (2 * pGPIOHandle->GPIOx_PinConfig.PinNo));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIOx_PinConfig.PinNo)); //clearing the OSPEEDR register for the specified Pin no.
	pGPIOHandle->pGPIOx->OSPEEDR |= temp; //setting

	//4. configure the Pull up Pull Down settings
	temp = (pGPIOHandle->GPIOx_PinConfig.PinPUPDCtrl << (2 * pGPIOHandle->GPIOx_PinConfig.PinNo));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIOx_PinConfig.PinNo)); //clearing the PUPDR register for the specified Pin no.
	pGPIOHandle->pGPIOx->PUPDR |= temp; //setting


	//5. configure the Output Type (Push Pull / Open Drain)
	temp = (pGPIOHandle->GPIOx_PinConfig.PinOPType << pGPIOHandle->GPIOx_PinConfig.PinNo);
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIOx_PinConfig.PinNo); //clearing the OTYPER register for the specified Pin no.
	pGPIOHandle->pGPIOx->OTYPER |= temp; //setting

	//6. configure Alternate Functionality
	if(pGPIOHandle->GPIOx_PinConfig.PinMode == GPIO_MODE_ALTFUN)
	{
		//configure the alt function registers
		uint8_t temp1, temp2;

		temp1 = pGPIOHandle->GPIOx_PinConfig.PinNo/8; //get the alternate function register no (2 in total)
		temp2 = pGPIOHandle->GPIOx_PinConfig.PinNo%8; //get the section no of the alt func register
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2)); //clearing the AFR[1:0] register for the specified Pin no.
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIOx_PinConfig.PinAltFuncMode<<(4*temp2)); //setting
	}

}

void GPIO_DeInit(GPIO_RegDef* pGPIOx)
{
	if(pGPIOx == GPIOA)
		GPIOA_REG_RESET();
	else if (pGPIOx == GPIOB)
		GPIOB_REG_RESET();
	else if (pGPIOx == GPIOC)
		GPIOC_REG_RESET();
	else if (pGPIOx == GPIOD)
		GPIOD_REG_RESET();
	else if (pGPIOx == GPIOE)
		GPIOE_REG_RESET();
	else if (pGPIOx == GPIOF)
		GPIOF_REG_RESET();
	else if (pGPIOx == GPIOG)
		GPIOG_REG_RESET();
	else if (pGPIOx == GPIOH)
		GPIOH_REG_RESET();
	else if (pGPIOx == GPIOI)
		GPIOI_REG_RESET();
}

// GPIO Read & Write
// GPIO Read & Write from & to Pin
uint8_t GPIO_ReadIpPin(GPIO_RegDef* pGPIOx, uint8_t pin_no)
{
   uint8_t value;
   value = (uint8_t)((pGPIOx->IDR >> pin_no) & 0x00000001); // the IDR data can be accessed in word mode only
   return value;
}

void GPIO_WriteOpPin(GPIO_RegDef* pGPIOx, uint8_t pin_no, uint8_t value)
{
	if(value == GPIO_PIN_SET)
		pGPIOx->ODR |= (1<<pin_no); // setting output bit to 1
	else
		pGPIOx->ODR &= ~(1<<pin_no); // clearing output bit to 0
}

// GPIO Read & Write from & to Port
uint16_t GPIO_ReadIpPort(GPIO_RegDef* pGPIOx)
{
	uint16_t value;
	value = (uint16_t)pGPIOx->IDR;// read entire port value (16 bits)
	return value;
}

void GPIO_WriteOpPort(GPIO_RegDef* pGPIOx, uint16_t value)
{
	pGPIOx->ODR  = value; // write value (16 bits) to entire port
}

// GPIO Toggle Output
void GPIO_OpToggle(GPIO_RegDef* pGPIOx, uint8_t pin_no)
{
	pGPIOx->ODR  ^= (1<<pin_no); // toggle the bit value for the specified GPIO pin
}


// GPIO Interrupt Number Configuration
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t mode)
{

	if(mode == ENABLE)
	{
		// Interrupt Set Enable
		if(IRQNumber <= 31)
		{
			//configure ISER0 register for 1 to 31
			*NVIC_ISER0 |= (1 << IRQNumber);

		}else if(IRQNumber>31 && IRQNumber<64)
		{
			//configure ISER1 for 32 to 63
			*NVIC_ISER1 |= (1 << (IRQNumber % 32));
		}
		else if(IRQNumber>=64 && IRQNumber<96)
		{
			//configure ISER2 for 64 to 95
			*NVIC_ISER2 |= (1 << (IRQNumber % 64));
		}
	}else
	{
		// Interrupt Clear Enable
		if(IRQNumber <= 31)
		{
			//configure ICER0 for 1 to 31
			*NVIC_ICER0 |= (1 << IRQNumber);
		}else if(IRQNumber>31 && IRQNumber<64)
		{
			//configure ICER1 for 32 to 63
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		}
		else if(IRQNumber>=64 && IRQNumber<96)
		{
			//configure ICER2 for 64 to 95
			*NVIC_ICER2 |= (1 << (IRQNumber % 64));
		}
	}
}


// GPIO Interrupt Priority Config (Optional in case of single interrupts)
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//configure IPR
	uint8_t iprx = IRQNumber/4; // get IPR register no.
	uint8_t iprx_section = IRQNumber%4 ; // get the section no. of the IPRx register

	uint8_t shift_amount = (8*iprx_section) + (8-NO_PR_BITS_IMPLEMENTED); // frame the config value

	*(NVIC_PR_BASE_ADDR+iprx) |= (IRQPriority<<shift_amount); // load the config value into the required section of the IPRx register

}

// GPIO Interrupt Handling
void GPIO_IRQHandling(uint8_t PinNumber)
{
	
	if(EXTI->PR & (1 << PinNumber)) // If the PR register is set for the Pin Number,
	{
		//clear the EXTI PR register for the specified Pin Number
		EXTI->PR |= (1 << PinNumber);
	}

}
/*********************************************** GPIO API's Definitions Start ***********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
#include "stm32f103xx.h"
#include "stm32f103xx_gpio.h"
#include "stm32f103xx_afio.h"
#include "stm32f103xx_exti.h"
#include "stm32f103xx_nvic.h"


/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** GPIO API's Definitions Start ***********************************************/

// GPIO Peripheral Clock Setup
void GPIO_PClk_init(GPIO_RegDef* pGPIOx, uint8_t setup_mode)
{
    if(setup_mode == ENABLE)
    {
        if(pGPIOx == GPIOA)
            GPIOA_PCLK_EN();
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
    }
    else
    {
        //TODO
    }

}


// GPIO Initialisation & De-Initialisation
void GPIO_Init(GPIO_Handle* pGPIOHandle)
{
    //uint32_t temp=0; //temp register value

    //1. enable the peripheral clock for GPIO
    GPIO_PClk_init(pGPIOHandle->pGPIOx, ENABLE);
    
    uint8_t reg_no = pGPIOHandle->GPIOx_PinConfig.PinNo/8;              //get register number : 0->GPIOx_CRL, 1->GPIOx_CRH
    uint8_t section_no = pGPIOHandle->GPIOx_PinConfig.PinNo%8;          //get section position
    uint8_t bit_no = section_no*4;                                      //MODE bit position no
    uint32_t mode, config_type;

    //2. configure the mode of GPIO pin
    if(pGPIOHandle->GPIOx_PinConfig.PinMode <= GPIO_MODE_AF)
    {
        //Non-Interrupt Mode    
        //output mode + speed is configured + general purpose output config type
        if(pGPIOHandle->GPIOx_PinConfig.PinMode == GPIO_MODE_OP && pGPIOHandle->GPIOx_PinConfig.PinOutputSpeed != 0 && pGPIOHandle->GPIOx_PinConfig.PinConfigType <= GPIO_GP_OP_OD)
        {
            mode = pGPIOHandle->GPIOx_PinConfig.PinOutputSpeed << (bit_no);
            config_type = pGPIOHandle->GPIOx_PinConfig.PinConfigType << (bit_no+2);
            
        }//output mode + speed is configured + alternate function output config type
        else if(pGPIOHandle->GPIOx_PinConfig.PinMode == GPIO_MODE_OP && pGPIOHandle->GPIOx_PinConfig.PinOutputSpeed != 0 && pGPIOHandle->GPIOx_PinConfig.PinConfigType > GPIO_AF_OP_OD
                && pGPIOHandle->GPIOx_PinConfig.PinConfigType <= GPIO_AF_OP_OD)
        {
            //to do
        }//input mode + floating/pull up/pull down/analog config type
        else if(pGPIOHandle->GPIOx_PinConfig.PinMode == GPIO_MODE_IP && pGPIOHandle->GPIOx_PinConfig.PinConfigType <= GPIO_PD)
        {
            mode = ~(3 << (bit_no)); //input mode
            
            if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_ANALOG) //Analog
                config_type = ~(~pGPIOHandle->GPIOx_PinConfig.PinConfigType << (bit_no+2)); //CNF bit position = MODE bit position + 2
            else if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_PU) //Pull Up
            {
                config_type = 2 << (bit_no+2);     //CNF bit position = MODE bit position + 2
                pGPIOHandle->pGPIOx->ODR &= ~(1 << (pGPIOHandle->GPIOx_PinConfig.PinNo));
                pGPIOHandle->pGPIOx->ODR |= 1 << (pGPIOHandle->GPIOx_PinConfig.PinNo);
            }
            else if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_PD) //Pull Down
            {
                config_type = 2 << (bit_no+2);     //CNF bit position = MODE bit position + 2
                pGPIOHandle->pGPIOx->ODR &= ~(1 << (pGPIOHandle->GPIOx_PinConfig.PinNo));
            }
            else if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_FLOAT) //No PUPD (ie) Floating
                config_type = 1 << (bit_no+2);     //CNF bit position = MODE bit position + 2
        }
        
        if(pGPIOHandle->GPIOx_PinConfig.PinMode == GPIO_MODE_IP)
        {
            //configuring GPIO mode
            pGPIOHandle->pGPIOx->CR[reg_no] &= mode;
        }
        else
        {
            //configuring GPIO mode
            pGPIOHandle->pGPIOx->CR[reg_no] |= mode;
        }
        
        //clear config bits
        pGPIOHandle->pGPIOx->CR[reg_no] &= ~(3 << (bit_no+2));
        //configuring GPIO config type
        pGPIOHandle->pGPIOx->CR[reg_no] |= config_type;
    }
    else
    {
        //Interrupt mode
        //1. configure for input
        mode = ~(3 << (bit_no)); //input mode
            
        if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_ANALOG) //Analog
            config_type = ~(~pGPIOHandle->GPIOx_PinConfig.PinConfigType << (bit_no+2)); //CNF bit position = MODE bit position + 2
        else if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_PU) //Pull Up
        {
            config_type = 2 << (bit_no+2);     //CNF bit position = MODE bit position + 2
            pGPIOHandle->pGPIOx->ODR &= ~(1 << (pGPIOHandle->GPIOx_PinConfig.PinNo));
            pGPIOHandle->pGPIOx->ODR |= 1 << (pGPIOHandle->GPIOx_PinConfig.PinNo);
        }
        else if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_PD) //Pull Down
        {
            config_type = 2 << (bit_no+2);     //CNF bit position = MODE bit position + 2
            pGPIOHandle->pGPIOx->ODR &= ~(1 << (pGPIOHandle->GPIOx_PinConfig.PinNo));
        }
        else if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_FLOAT) //No PUPD (ie) Floating
            config_type = 1 << (bit_no+2);     //CNF bit position = MODE bit position + 2
        
        //configuring GPIO mode
        pGPIOHandle->pGPIOx->CR[reg_no] &= mode;
        
        //clear config bits
        pGPIOHandle->pGPIOx->CR[reg_no] &= ~(3 << (bit_no+2));
        //configuring GPIO config type
        pGPIOHandle->pGPIOx->CR[reg_no] |= config_type;
            
        
        //2. configure edge triggering of interrupts in EXTI (peripheral side)
        exti_intrpt_trig_config(pGPIOHandle->GPIOx_PinConfig.PinNo, pGPIOHandle->GPIOx_PinConfig.PinMode);
        
        
        //3. config exticr for port selection in AFIO (peripheral side)
        afio_exti_config(pGPIOHandle->pGPIOx, pGPIOHandle->GPIOx_PinConfig.PinNo);
        
        
        //4. enable exti interrupts (peripheral side)
        exti_enable_intrpt(pGPIOHandle->GPIOx_PinConfig.PinNo);
        
        
        //5. configure enable and mask bits in NVIC side
        uint8_t irq_no = nvic_irq_gpio_port_map(pGPIOHandle);
        nvic_intrpt_enable(irq_no);
    
    }
}


// GPIo De-Initialization
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
}



// GPIO Toggle Output
void GPIO_OpToggle(GPIO_RegDef* pGPIOx, uint8_t pin_no)
{
    pGPIOx->ODR ^= (1<<pin_no); // toggle the bit value for the specified GPIO pin
}



// GPIO Read & Write
// GPIO Read from Pin
uint8_t GPIO_ReadIpPin(GPIO_RegDef* pGPIOx, uint8_t pin_no)
{
   uint8_t value;
   value = (uint8_t)((pGPIOx->IDR >> pin_no) & 0x00000001); // the IDR data can be accessed in word mode only
   return value;
}

// GPIO Write to Pin
void GPIO_WriteOpPin(GPIO_RegDef* pGPIOx, uint8_t pin_no, uint8_t value)
{
    if(value == GPIO_PIN_SET)
        pGPIOx->ODR |= (1<<pin_no); // setting output bit to 1
    else
        pGPIOx->ODR &= ~(1<<pin_no); // clearing output bit to 0
}



// GPIO Interrupt Handling
void GPIO_IRQHandling(uint8_t pin_no)
{
    if(EXTI->PR & (1 << pin_no)) // If the PR register is set for the Pin Number,
    {
        //clear the EXTI PR register for the specified Pin Number
        EXTI->PR |= (1 << pin_no);
    }
}
/*********************************************** GPIO API's Definitions Start ***********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
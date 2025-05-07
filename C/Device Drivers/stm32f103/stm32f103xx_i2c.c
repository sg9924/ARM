#include"stm32f103xx_i2c.h"


//I2C Peripheral Clock Init
void I2C_PClk_init(I2C_Handle* pI2CHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pI2CHandle->pI2Cx == I2C1)
            I2C1_PCLK_ENABLE();
        else if(pI2CHandle->pI2Cx == I2C2)
            I2C2_PCLK_ENABLE();
    }
    else if(mode == DISABLE)
    {
        if(pI2CHandle->pI2Cx == I2C1)
            I2C1_PCLK_DISABLE();
        else if(pI2CHandle->pI2Cx == I2C2)
            I2C2_PCLK_DISABLE();
    }
}

//I2C Peripheral Enable / Disable
void I2C_P_init(I2C_Handle* pI2CHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pI2CHandle->pI2Cx == I2C1)
            I2C1_ENABLE();
        else if(pI2CHandle->pI2Cx == I2C2)
            I2C2_ENABLE();
    }
    else if(mode == DISABLE)
    {
        if(pI2CHandle->pI2Cx == I2C1)
            I2C1_DISABLE();
        else if(pI2CHandle->pI2Cx == I2C2)
            I2C2_DISABLE();
    }
}


//I2C Configuration
void I2C_Configure(I2C_Handle* pI2CHandle, uint8_t ack_control, uint8_t mode, uint8_t mode_type, uint32_t speed, uint8_t fm_duty_cycle, uint8_t address_type, uint8_t device_address)
{
    pI2CHandle->I2CConfig.ack_control      = ack_control;
    pI2CHandle->I2CConfig.address_type     = address_type;
    pI2CHandle->I2CConfig.device_address   = device_address;
    pI2CHandle->I2CConfig.fm_duty_cycle    = fm_duty_cycle;
    pI2CHandle->I2CConfig.mode             = mode;
    pI2CHandle->I2CConfig.mode_type        = mode_type;
    pI2CHandle->I2CConfig.speed            = speed;

    pI2CHandle->prxbuffer = 0;
    pI2CHandle->ptxbuffer = 0;
    pI2CHandle->rx_len    = 0;
    pI2CHandle->tx_len    = 0;
    pI2CHandle->rx_state  = I2C_STATE_READY;
    pI2CHandle->tx_state  = I2C_STATE_READY;
}


//I2C GPIO Pins Init
void I2C_GPIO_init(I2C_Handle* pI2CHandle)
{
    //Open Drain Configurations, External Pullups of 4.7K to +3.3V should be used
    pI2CHandle->pGPIOHandle->pGPIOx = GPIOB;

    if(pI2CHandle->pI2Cx == I2C1)
    {
        //PB6 - SCL, PB7 - SDA
        //SCL
        GPIO_Config(pI2CHandle->pGPIOHandle, GPIOB, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_OD, GPIO_PIN6, GPIO_OP_SPEED_2);
        GPIO_Init(pI2CHandle->pGPIOHandle);

        //SDA
        GPIO_Config(pI2CHandle->pGPIOHandle, GPIOB, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_OD, GPIO_PIN7, GPIO_OP_SPEED_2);
        GPIO_Init(pI2CHandle->pGPIOHandle);
    }
    else if(pI2CHandle->pI2Cx == I2C2)
    {
        //PB10 - SCL, PB11 - SDA
        //SCL
        GPIO_Config(pI2CHandle->pGPIOHandle, GPIOB, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_OD, GPIO_PIN10, GPIO_OP_SPEED_2);
        GPIO_Init(pI2CHandle->pGPIOHandle);

        //SDA
        GPIO_Config(pI2CHandle->pGPIOHandle, GPIOB, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_OD, GPIO_PIN11, GPIO_OP_SPEED_2);
        GPIO_Init(pI2CHandle->pGPIOHandle);
    }
}


//I2C Peripheral Init
void I2C_init(I2C_Handle* pI2CHandle, GPIO_Handle* pGPIOHandle, I2C_RegDef* pI2Cx)
{
    pI2CHandle->pGPIOHandle = pGPIOHandle;
    pI2CHandle->pI2Cx       = pI2Cx;
    
    //Enable Peripheral Clock
    I2C_PClk_init(pI2CHandle, ENABLE);

    //Configure GPIO pins for I2C
    I2C_GPIO_init(pI2CHandle);
    

    uint32_t temp = 0;

    //Enable ACK (doesn't work when PE is 0)
    //pI2CHandle->pI2Cx->CR1 |= (pI2CHandle->I2CConfig.ack_control)<<I2C_CR1_ACK;

    //Configure Frequency
    temp = 8000000/1000000;
    pI2CHandle->pI2Cx->CR2 |= (temp & 0x3F)<<I2C_CR2_FREQ;

    //5. Configure Host Device Address for Slave Mode
    if(pI2CHandle->I2CConfig.mode == I2C_MODE_SLAVE)
    {
        if(pI2CHandle->I2CConfig.address_type == I2C_ADDR_7BIT)
        {
            temp=0;
            temp = pI2CHandle->I2CConfig.device_address<<I2C_OAR1_ADD1;
            pI2CHandle->pI2Cx->OAR1 |= temp;
        }
        else if(pI2CHandle->I2CConfig.address_type == I2C_ADDR_10BIT)
        {
            //to do
        }
    }

    //Configure CCR
    temp=0;
    temp |= pI2CHandle->I2CConfig.mode_type<<I2C_CCR_FS;         // Standard or Fast Mode Config

    if(pI2CHandle->I2CConfig.mode_type == I2C_MODE_TYPE_STD)
    {
        temp |= 8000000/(2 * pI2CHandle->I2CConfig.speed);
    }
    else
    {
        temp |= pI2CHandle->I2CConfig.fm_duty_cycle << I2C_CCR_DUTY;

        if(pI2CHandle->I2CConfig.fm_duty_cycle == I2C_FM_DUTY_2)
            temp |= 8000000/(3 * pI2CHandle->I2CConfig.speed);
        else if(pI2CHandle->I2CConfig.fm_duty_cycle == I2C_FM_DUTY_16_9)
            temp |= 8000000/(25 * pI2CHandle->I2CConfig.speed);
    }
    pI2CHandle->pI2Cx->CCR |= temp;

    //Configure TRISE
    if(pI2CHandle->I2CConfig.mode_type == I2C_MODE_TYPE_STD)
        pI2CHandle->pI2Cx->TRISE |= ((8000000/1000000) + 1) << I2C_TRISE0;
    else if(pI2CHandle->I2CConfig.mode_type == I2C_MODE_TYPE_FAST)
        pI2CHandle->pI2Cx->TRISE |= ((8000000/1000000)*300 + 1) << I2C_TRISE0;

    //8. Enable I2C Peripheral
    I2C_P_init(pI2CHandle, ENABLE);
}
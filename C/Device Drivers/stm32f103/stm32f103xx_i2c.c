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
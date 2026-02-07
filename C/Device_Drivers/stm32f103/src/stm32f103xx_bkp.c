#include"stm32f103xx.h"
#include"stm32f103xx_bkp.h"


uint8_t BKP_RegisterWrite(uint8_t reg_no, uint16_t* pdata, uint8_t mode)
{
    if(reg_no<=0) return 0;

    if(reg_no<=10)
    {
        if(mode == BKP_REG_OVERWRITE)
        {
            BKP->DR[reg_no-1] &= ~(1<<16); //clear lower 16 bits
            BKP->DR[reg_no-1] = (uint16_t)*pdata;
        }
        else if(mode == BKP_REG_APPEND) BKP->DR[reg_no-1] |= (uint16_t)*pdata;
    }
    else if(reg_no>10 && reg_no<=42)
    {
        if(mode == BKP_REG_OVERWRITE)
        {
            BKP->DR_[reg_no-1] &= ~(1<<16); //clear lower 16 bits
            BKP->DR_[reg_no-1] = (uint16_t)*pdata;
        }
        else if(mode == BKP_REG_APPEND) BKP->DR_[reg_no-1] |= (uint16_t)*pdata;
    }
    return 1;
}


uint16_t BKP_RegisterRead(uint8_t reg_no)
{
    uint16_t result;

    if(reg_no<=0) return 0;

    if(reg_no<=10) result = BKP->DR[reg_no-1];
    else if(reg_no>10 && reg_no<=42) result = BKP->DR_[reg_no-1];

    return result;
}


uint8_t BKP_RegisterClear(uint8_t reg_no)
{
    if(reg_no<=0) return 0;

    if(reg_no<=10) BKP->DR[reg_no-1] |= (uint16_t)0x0;
    else if(reg_no>10 && reg_no<=42) BKP->DR_[reg_no-1] |= (uint16_t)0x0;
    return 1;
}
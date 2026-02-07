#ifndef INC_STM32F103XX_BKP_H
#define INC_STM32F103XX_BKP_H

#include"stm32f103xx_rcc.h"

#define BKP                                      ((BKP_RegDef*)BKP_BASE_ADDR)

//BKP_RTCCR Bit Fields
#define BKP_RTCCR_CAL0                           0
#define BKP_RTCCR_CCO                            7
#define BKP_RTCCR_ASOE                           8
#define BKP_RTCCR_ASOS                           9

//BKPCR Bit Fields
#define BKP_CR_TPE                               0
#define BKP_CR_TPAL                              1

//BKP_CSR Bit Fields
#define BKP_CSR_CTE                              0
#define BKP_CSR_CTI                              1
#define BKP_CSR_TPIE                             2
#define BKP_CSR_TEF                              8
#define BKP_CSR_TIF                              9


#define BKP_PLCK_ENABLE()                        (RCC->APB1ENR |= 1<<RCC_APB1ENR_BKPEN)
#define BKP_PLCK_DISABLE()                       (RCC->APB1ENR &= ~(1<<RCC_APB1ENR_BKPEN))



uint8_t BKP_RegisterWrite(uint8_t reg_no, uint16_t* pdata, uint8_t mode);
uint16_t BKP_RegisterRead(uint8_t reg_no);
uint8_t BKP_RegisterClear(uint8_t reg_no);



#define BKP_REG_OVERWRITE                        0
#define BKP_REG_APPEND                           1



#endif
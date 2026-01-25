#ifndef INC_STM32F103XX_RTC_H
#define INC_STM32F103XX_RTC_H

#include"stm32f103xx.h"
#include"stm32f103xx_memory_map.h"
#include"stm32f103xx_core.h"
#include"stm32f103xx_rcc.h"
#include"../../utilities/stm32f103xx_utilities.h"


#define PWR                                      ((PWR_RegDef*)PWR_BASE_ADDR)

//PWR_CR Bit fields
#define PWR_CR_LPDS                              0
#define PWR_CR_PDDS                              1
#define PWR_CR_CWUF                              2
#define PWR_CR_CSBF                              3
#define PWR_CR_PVDE                              4
#define PWR_CR_PLS0                              5
#define PWR_CR_PLS1                              6
#define PWR_CR_PLS2                              7
#define PWR_CR_DBP                               8

//PWR_CSR Bit Filds
#define PWR_CSR_WUF                              0
#define PWR_CSR_SBF                              1
#define PWR_CSR_PVDO                             2
#define PWR_CSR_EWUP                             8




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


#define RTC                                      ((RTC_RegDef*)RTC_BASE_ADDR)


//RTC_CRH Bit Fields
#define RTC_CRH_SECIE                            0
#define RTC_CRH_ALRIE                            1
#define RTC_CRH_OWIE                             2

//RTC_CRL Bit Fields
#define RTC_CRL_SECF                             0
#define RTC_CRL_ALRF                             1
#define RTC_CRL_OWF                              2
#define RTC_CRL_RSF                              3
#define RTC_CRL_CNF                              4
#define RTC_CRL_RTOFF                            5











#define RTC_CLK_NONE                             0
#define RTC_CLK_LSE                              1
#define RTC_CLK_LSI                              2
#define RTC_CLK_HSE                              3


//Macros
#define BKP_WRITE_PROTECT_DISABLE()              (PWR->CR |= 1<<8) //DBP field - disable write protect

#define RTC_PCLK_ENABLE()                         (RCC->BDCR |= 1<<RCC_BDCR_RTCEN)
#define RTC_PCLK_DISABLE()                        (RCC->BDCR &= ~(1<<RCC_BDCR_RTCEN))

#define RTC_ENTER_CONFIG()                       (RTC->CRL |= 1<<RTC_CRL_CNF)
#define RTC_EXIT_CONFIG()                        (RTC->CRL &= ~(1<<RTC_CRL_CNF))

#define RTC_ALARM_INTRPT_ENABLE()                (RTC->CRH |= 1<<RTC_CRH_ALRIE)
#define RTC_ALARM_INTRPT_DISABLE()               (RTC->CRH &= ~(1<<RTC_CRH_ALRIE))


typedef struct
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    char*   formatted_time;

    uint8_t clock;
}RTCHandle;


void RTC_Clk_LSE(uint8_t mode);
void RTC_Clk_select(uint8_t rtc_clk);
void RTC_Init(RTCHandle* pRTCHandle, uint8_t clock);

uint32_t RTC_Counter_Get(void);
void RTC_Counter_Set(uint32_t value);

void RTC_Time_Get(uint8_t *hour, uint8_t *min, uint8_t *sec);
void RTC_Time_Set(uint8_t hour, uint8_t min, uint8_t sec);
void RTC_Time_Print(RTCHandle* pRTCHandle);

void RTC_Alarm_SetAbsolute(uint32_t value);
void RTC_Alarm_SetSeconds(uint32_t secs);
void RTC_Alarm_SetTime(uint8_t hour, uint8_t min, uint8_t sec);
uint32_t RTC_Alarm_SecondsUntil(void);

void RTC_ALARM_InterruptEnable(void (*user_callback)(void));
void RTC_Alarm_InterruptDisable(void);

#endif
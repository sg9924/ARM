#include"../inc/stm32f103xx_rtc.h"
#include"../inc/stm32f103xx_serial.h"
#include"../inc/stm32f103xx_exti.h"
#include"../inc/stm32f103xx_nvic.h"


static inline uint32_t clocktime_to_seconds(uint8_t hour, uint8_t min, uint8_t sec)
{
    return (uint32_t)hour * 3600u + (uint32_t)min * 60u + (uint32_t)sec;
}

static void (*RTC_Alarm_UserCallback) (void) = NULL;

/* Convert seconds (0..86399) to h,m,s */
static inline void seconds_to_clocktime(uint32_t seconds, uint8_t *hour, uint8_t *min, uint8_t *sec)
{
    seconds %= 86400u;

    *hour = seconds / 3600u;
    seconds %= 3600u;

    *min = seconds / 60u;
    *sec = seconds % 60u;
}




void RTC_Clk_select(uint8_t rtc_clk)
{
    if(rtc_clk == RTC_CLK_LSE || rtc_clk == RTC_CLK_LSI || rtc_clk == RTC_CLK_HSE)
        RCC->BDCR |= rtc_clk<<RCC_BDCR_RTCSEL0;
    else if(rtc_clk == RTC_CLK_NONE)
        RCC->BDCR &= ~(3<<RCC_BDCR_RTCSEL0);
    else
    {    //invalid
    }
}

void RTC_Clk_LSE(uint8_t mode)
{
    if(mode == ENABLE)
    {
        RCC->BDCR |= 1<<RCC_BDCR_LSEON;
        //wait till 1
        while(!(RCC->BDCR & 1<<RCC_BDCR_LSERDY));
    }
    else if(mode == DISABLE)
    {
        RCC->BDCR &= ~(1<<RCC_BDCR_LSEON);
        //wait till 0 (usually 6 oscillator clock cycles)
        while((RCC->BDCR & 1<<RCC_BDCR_LSERDY));
    }
}



void RTC_Init(RTCHandle* pRTCHandle, uint8_t clock)
{
    pRTCHandle->clock = clock;

    //enable power interface
    PWR_PLCK_ENABLE();

    //enable backup interface
    BKP_PLCK_ENABLE();

    //set DBP in PWR_CR to disable write protection
    PWR->CR |= 1<<PWR_CR_DBP;

    if(clock == RTC_CLK_LSE)
    {
        //Enable LSE
        RTC_Clk_LSE(ENABLE);
    }

    //select RTC clock
    RTC_Clk_select(clock);

    //enable RTC clock
    RTC_PCLK_ENABLE();

    //wait for RTC registers sync
    while(!(RTC->CRL & 1<<RTC_CRL_RSF));

    //wait till registers are free for write
    while(!(RTC->CRL & 1<<RTC_CRL_RTOFF));

    //enter config mode
    RTC_ENTER_CONFIG();

    if(clock == RTC_CLK_LSE)
    {
        //set prescaler
        RTC->PRLL = 32767;
    }

    //clear counters
    RTC->CNTH = 0;
    RTC->CNTL = 0;

    //exit config mode
    RTC_EXIT_CONFIG();

    //wait for write operation to complete
    while(!(RTC->CRL & 1<<RTC_CRL_RTOFF));
}



uint32_t RTC_Counter_Get(void)
{
    uint16_t high1, low, high2;

    do 
    {
        high1 = RTC->CNTH;
        low   = RTC->CNTL;
        high2 = RTC->CNTH;
    } while (high1 != high2);
    return ((uint32_t)high1 << 16) | low;
}


void RTC_Counter_Set(uint32_t value)
{
    while(!(RTC->CRL & 1<<RTC_CRL_RTOFF));
    RTC_ENTER_CONFIG();

    RTC->CNTH = value >> 16;
    RTC->CNTL = value & 0xFFFF;

    RTC_EXIT_CONFIG();
    while(!(RTC->CRL & 1<<RTC_CRL_RTOFF));
}


void RTC_Time_Get(uint8_t *hour, uint8_t *min, uint8_t *sec)
{
    uint32_t counter_value = RTC_Counter_Get();
    seconds_to_clocktime(counter_value, hour, min, sec);
}


void RTC_Time_Set(uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t counter_value = clocktime_to_seconds(hour, min, sec);
    RTC_Counter_Set(counter_value);
}

//to do
void RTC_Time_Format(RTCHandle* pRTCHandle)
{

}


void RTC_Time_Print(RTCHandle* pRTCHandle)
{
    RTC_Time_Get(&pRTCHandle->hour, &pRTCHandle->min, &pRTCHandle->sec);
    Serialprintln("Time from RTC: %d:%d:%d", NONE, pRTCHandle->hour, pRTCHandle->min, pRTCHandle->sec);
}


//debug alarm API's, Alarm set not working

void RTC_Alarm_SetAbsolute(uint32_t value)
{
    RTC_ENTER_CONFIG();

    RTC->ALRH = (uint16_t)(value>>16);
    RTC->ALRL = (uint16_t)(value && 0xFFFF);

    RTC_EXIT_CONFIG();
}


void RTC_Alarm_SetSeconds(uint32_t secs)
{
    RTC_Alarm_SetAbsolute(RTC_Counter_Get() + secs);
}


void RTC_Alarm_SetTime(uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t target_value = clocktime_to_seconds(hour, min, sec);
    uint32_t now_value    = RTC_Counter_Get() % (86400U);

    //if target time is past today, set the alarm for next day
    if(target_value <= now_value)
        target_value += 86400U;

    //get the alarm value
    uint32_t alarm_value = (RTC_Counter_Get() - now_value) + target_value;

    //set alarm
    RTC_Alarm_SetAbsolute(alarm_value);
}


uint32_t RTC_Alarm_SecondsUntil(void)
{
    uint32_t alarm_value = ((uint32_t)RTC->ALRH << 16) | RTC->ALRL;
    uint32_t counter_value = RTC_Counter_Get();

    if(alarm_value > counter_value)
        return alarm_value - counter_value;
    else
        return 0;
}


void RTC_ALARM_InterruptEnable(void (*user_callback)(void))
{
    //callback function if any
    RTC_Alarm_UserCallback = user_callback;

    RTC_ALARM_INTRPT_ENABLE();

    //enable exti line 17
    EXTI_Intrpt_Mask(17, ENABLE);

    //set rising edge trigger
    EXTI_Intrpt_Config(17, GPIO_MODE_INTRPT_RE_TRIG, ENABLE);

    //clear pending bit
    EXTI_Pend_Clear(17);

    //enable NVIC
    nvic_intrpt_enable(RTCAlarm_IRQ_NO);
}



void RTC_Alarm_InterruptDisable(void)
{
    RTC_ALARM_INTRPT_DISABLE();
    EXTI_Intrpt_Mask(17, DISABLE);
    EXTI_Pend_Clear(17);
    nvic_intrpt_disable(RTCAlarm_IRQ_NO);

    RTC_Alarm_UserCallback = NULL;
}


void RTC_Alarm_IRQHandler(void)
{
    if(RTC->CRL && RTC_CRL_ALRF) //alarm detected
        RTC->CRL &= ~(1<<RTC_CRL_ALRF);

    //clear exti pend
    EXTI_Pend_Clear(17);

    if(RTC_Alarm_UserCallback) //call user callback if provided
        RTC_Alarm_UserCallback();
}
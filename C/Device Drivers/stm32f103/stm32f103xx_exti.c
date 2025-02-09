#include<stdint.h>
#include"stm32f103xx_exti.h"


/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** EXTI API's Definitions Start ***********************************************/
//!!EXTI doesn't need clock enable!!

//Interrupt trigger configuration function
void exti_intrpt_trig_config(uint8_t pin_no, uint8_t intrpt_mode)
{        
        //Falling Edge trigger
        if(intrpt_mode == GPIO_MODE_INTRPT_FE_TRIG)
        {
            EXTI->FTSR |= (1 << pin_no);
            EXTI->RTSR &= ~(1 << pin_no); //precaution
        }//Rising Edge Trigger
        else if(intrpt_mode == GPIO_MODE_INTRPT_RE_TRIG)
        {
            EXTI->RTSR |= (1 << pin_no);
            EXTI->FTSR &= ~(1 << pin_no); //precaution
        }//Falling & Rising Edge Trigger
        else if(intrpt_mode == GPIO_MODE_INTRPT_FERE_TRIG)
        {
            EXTI->FTSR |= (1 << pin_no);
            EXTI->RTSR |= (1 << pin_no);
        }
}


void exti_enable_intrpt(uint8_t pin_no)
{
    EXTI->IMR = (1 << pin_no);
}

void exti_enable_event(uint8_t pin_no)
{
    EXTI->EMR = (1 << pin_no);
}

/*********************************************** EXTI API's Definitions Start ***********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
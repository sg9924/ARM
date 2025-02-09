#include<stdint.h>
#include "stm32F103xx_afio.h"

/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** AFIO API's Definitions Start ***********************************************/

void afio_exti_config(GPIO_RegDef* port_code, uint8_t pin_no)
{
    uint8_t reg_no = pin_no/4; //get EXTICR Register number
    uint8_t bit_no = (pin_no%4)*4;   //get bit no. of the EXTICR register
    uint8_t port_no = GPIO_BASEADDR_TO_CODE(port_code); //get the corresponding code of the specified GPIO port
    //uint8_t port_no = 2;
    
    AFIO->EXTICR[reg_no] |= (port_no << bit_no);
}

/*********************************************** AFIO API's Definitions Start ***********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
#include"../inc/stm32f103xx_init.h"
#include"../inc/stm32f103xx.h"
#include"../inc/stm32f103xx_core.h"
#include"../inc/stm32f103xx_timer.h"
#include"../inc/stm32f103xx_rcc.h"
#include"../inc/stm32f103xx_serial.h"

//USART2 for Serial Debugging
//TIM2 for delays

//RCC_Handle R;
TIM_Handle Delay;
GPIO_Handle LED;
extern uint32_t sysclock;

void board_init()
{
    clock_init();  //SYSCLK: PLL with HSE at 72MHz
    Serial_init(); //Initialize USART2 for Serial Print (Debugging)
    led_init();    //Initialize built in LED at PC13
    led_reset();   //switch off led
    display_cpu_info();
    display_clk_info();
    
}


void led_init()
{
    GPIO_Config(&LED, GPIOC, GPIO_MODE_OP, GPIO_CONFIG_GP_OP_PP, GPIO_PIN13, GPIO_OP_SPEED_2);
    GPIO_Init(&LED);
}

void led_reset()
{
    //GPIO_Bit_Reset(&LED, GPIO_PIN13);
    GPIO_Bit_Set(&LED, GPIO_PIN13);
    Serialprintln("User LED has been initialized and reset", INFO);
}

void led_toggle()
{
    GPIO_OpToggle(GPIOC, GPIO_PIN13);
}


void clock_init()
{
    RCC_Config_PLL(PLL_CLK_SRC_HSE, PLL_MUL_9, PLL_HSE_DIV_NONE);
    RCC_init(RCC_CLK_HSE, SYSCLK_PLL, AHB_PRESCALAR_1, APB1_PRESCALAR_2, APB2_PRESCALAR_1);
}


void display_cpu_info()
{
    SERIAL_NL();
    Serialprintln("Processor Info:", NONE);
    Serialprintln("Implementer: %x", INFO, GET_IMPLEMENTER());
    Serialprintln("Variant: %x", INFO, GET_VARIANT());
    Serialprintln("Part No: %x", INFO, GET_PARTNO());
    Serialprintln("Revision: %x", INFO, GET_REVISION());
}


void display_clk_info()
{
    SERIAL_NL();
    Serialprintln("Clock Info:", NONE);
    if(RCC_Get_Clock_Source() == RCC_CLK_HSE)       Serialprintln("Clock Source is HSE", INFO);
    else if(RCC_Get_Clock_Source()  == RCC_CLK_HSI) Serialprintln("Clock Source is HSI", INFO);

    Serialprintln("HCLK is %d MHz", INFO, RCC_Get_HCLK()/1000000);
    Serialprintln("PCLK1 is %d MHz", INFO, RCC_Get_PCLK1()/1000000);
    Serialprintln("PCLK2 is %d MHz", INFO, RCC_Get_PCLK2()/1000000);
}


void tim_delay_ms(uint16_t delay)
{
    //Configure Timer 2
    TIM_Base_Configure(&Delay, TIM2, TIM_COUNT_DIR_UP, ((sysclock/10000)-1), (delay*10)-1, TIM_AR_PRELOAD_ENABLE);
    TIM_Count_Reset(&Delay); //reset count
    TIM_Base_init(&Delay); //initialize
    TIM_Base_Start(&Delay); //enable timer
    TIM_Update_Event_Check(&Delay); //wait for update event flag
    TIM_Base_Stop(&Delay); //disable timer
}


void tim_delay_us(uint16_t delay)
{
    //Configure Timer 2
    TIM_Base_Configure(&Delay, TIM2, TIM_COUNT_DIR_UP, ((sysclock/1000000)-1), delay-1, TIM_AR_PRELOAD_ENABLE);
    TIM_Base_init(&Delay); //initialize
    TIM_Base_Start(&Delay); //enable timer
    TIM_Update_Event_Check(&Delay); //wait for update event flag
    TIM_Base_Stop(&Delay); //disable timer
}
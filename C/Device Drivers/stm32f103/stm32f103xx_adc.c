#include"stm32f103xx_adc.h"




static void ADC_GPIO_Config(ADC_Handle* pADCHandle)
{
    if(pADCHandle->pGPIOHandle->pGPIOx == GPIOA)
    {
        // Configure for Analog Mode - Pin 6
        GPIO_Config(pADCHandle->pGPIOHandle, pADCHandle->pGPIOHandle->pGPIOx, GPIO_MODE_IP, GPIO_CONFIG_ANALOG, GPIO_PIN6, GPIO_OP_SPEED_0);
        GPIO_Init(pADCHandle->pGPIOHandle);
    }
}



void ADC_PClk_init(ADC_Handle* pADCHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pADCHandle->pADCx == ADC1)
            ADC1_PCLK_EN();
        else if(pADCHandle->pADCx == ADC2)
            ADC3_PCLK_EN();
        else if(pADCHandle->pADCx == ADC3)
            ADC3_PCLK_EN();
    }
    else if(mode == DISABLE)
    {
        if(pADCHandle->pADCx == ADC1)
            ADC1_PCLK_DISABLE();
        else if(pADCHandle->pADCx == ADC2)
            ADC1_PCLK_DISABLE();
        else if(pADCHandle->pADCx == ADC3)
            ADC1_PCLK_DISABLE();
    }
}



void ADC_Configure(ADC_Handle* pADCHandle, ADC_RegDef* ADCx, uint8_t mode, uint8_t event, uint8_t dma_mode)
{
    pADCHandle->pADCx                        = ADCx;
    pADCHandle->ADCx_Config.mode             = mode;
    pADCHandle->ADCx_Config.external_event   = event;
    pADCHandle->ADCx_Config.dma_mode         = dma_mode;
}




void ADC_init(ADC_Handle* pADCHandle, GPIO_Handle* pGPIOHandle, GPIO_RegDef* GPIOx)
{
    pADCHandle->pGPIOHandle             = pGPIOHandle;
    pADCHandle->pGPIOHandle->pGPIOx     = GPIOx;

    //Configure GPIO for ADC
    ADC_GPIO_Config(pADCHandle);

    //Enable ADC Peripheral Clock
    ADC_PClk_init(pADCHandle, ENABLE);

    //ADC Modes
    if(pADCHandle->ADCx_Config.mode == ADC_MODE_SINGLE)
        pADCHandle->pADCx->CR2 &= ~(1<<ADC_CR2_CONT);
    else if(pADCHandle->ADCx_Config.mode == ADC_MODE_CONT)
        pADCHandle->pADCx->CR2 |= 1<<ADC_CR2_CONT;
    else if(pADCHandle->ADCx_Config.mode == ADC_MODE_SCAN)
        pADCHandle->pADCx->CR1 |= 1<<ADC_CR1_SCAN;

    //ADC DMA
    if(pADCHandle->ADCx_Config.dma_mode == ADC_DMA_ENABLE)
        pADCHandle->pADCx->CR2 |= 1<<ADC_CR2_DMA;
    else if(pADCHandle->ADCx_Config.dma_mode == ADC_DMA_DISABLE)
        pADCHandle->pADCx->CR2 &= ~(1<<ADC_CR2_DMA);

    //Configure the Event type
    if(pADCHandle->ADCx_Config.external_event == ADC_EVENT_SWSTART)
        pADCHandle->pADCx->CR2 |= ADC_EVENT_SWSTART << ADC_CR2_EXTSEL0;
}


static void _ADC_Start()
{
    //Enable ADC to wake from sleep
    ADC1_ENABLE();
    //!!delay should be added here!!
    //Enable ADC after Wake
    ADC1_ENABLE();
}

uint16_t ADC_Start(ADC_Handle* pADCHandle)
{
    uint16_t adc_data = 0;

    _ADC_Start();

    //Wait till ADC has completed conversion (EOC becomes 1)
    while(!(pADCHandle->pADCx->SR & 1<<ADC_SR_EOC));

    //Extract ADC Data (lower 16 bits only)
    adc_data = pADCHandle->pADCx->DR & 0xFFFF;
    
    return adc_data;
}



void ADC_DMA_Start()
{
    _ADC_Start();
}
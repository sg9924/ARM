#include"stm32f103xx_dma.h"


void DMA_PClk_init(DMA_Handle* pDMAHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pDMAHandle->pDMAx == DMA1)
            DMA1_PCLK_ENABLE();
        else if(pDMAHandle->pDMAx == DMA2)
            DMA2_PCLK_ENABLE();
    }
    else if(mode == DISABLE)
    {
        if(pDMAHandle->pDMAx == DMA1)
            DMA1_PCLK_DISABLE();
        else if(pDMAHandle->pDMAx == DMA2)
            DMA2_PCLK_DISABLE();
    }
}

//DMA Config
void DMA_Configure(DMA_Handle* pDMAHandle, DMA_RegDef* pDMAx, uint8_t channel, uint8_t channel_priority, uint8_t direction, uint16_t no_of_data, uint8_t peri_size, uint8_t mem_size,
                   uint8_t peri_inc, uint8_t mem_inc, uint8_t mem_to_mem_mode)
{
    pDMAHandle->pDMAx   = pDMAx;
    pDMAHandle->channel = channel;
    
    pDMAHandle->DMAx_Config.channel_priority = channel_priority;
    pDMAHandle->DMAx_Config.direction        = direction;
    pDMAHandle->DMAx_Config.no_of_data       = no_of_data;
    pDMAHandle->DMAx_Config.peripheral_size  = peri_size;
    pDMAHandle->DMAx_Config.memory_size      = mem_size;
    pDMAHandle->DMAx_Config.peripheral_inc   = peri_inc;
    pDMAHandle->DMAx_Config.memory_inc       = mem_inc;
    pDMAHandle->DMAx_Config.mem_to_mem_mode  = mem_to_mem_mode;
}


//DMA Address Config
void DMA_Address_init(DMA_Handle* pDMAHandle, uint32_t peri_addr, uint32_t mem_addr)
{
    pDMAHandle->memory_addr_1 = mem_addr;

    if(pDMAHandle->DMAx_Config.mem_to_mem_mode == DMA_MEM_TO_MEM_ENABLE)
        pDMAHandle->memory_addr_2 = mem_addr;
    else if(pDMAHandle->DMAx_Config.mem_to_mem_mode == DMA_MEM_TO_MEM_DISABLE)
        pDMAHandle->peripheral_addr = peri_addr;
}



//DMA Interrupt Configure
void DMA_Interrupt_init(DMA_Handle* pDMAHandle, uint8_t interrupt, uint8_t mode)
{
    uint8_t chnl = pDMAHandle->channel;

    if(mode == ENABLE)
    {
        if(interrupt == DMA_INTRPT_TC_ENABLE)
            pDMAHandle->pDMAx->Channel[chnl].CCR |= DMA_INTRPT_TC_ENABLE;
        else if(interrupt == DMA_INTRPT_HT_ENABLE)
            pDMAHandle->pDMAx->Channel[chnl].CCR |= DMA_INTRPT_HT_ENABLE;
        else if(interrupt == DMA_INTRPT_TE_ENABLE)
            pDMAHandle->pDMAx->Channel[chnl].CCR |= DMA_INTRPT_TE_ENABLE;
    }
    else if(mode == DISABLE)
    {
        if(interrupt == DMA_INTRPT_TC_DISABLE)
            pDMAHandle->pDMAx->Channel[chnl].CCR &= DMA_INTRPT_TC_DISABLE;
        else if(interrupt == DMA_INTRPT_HT_DISABLE)
            pDMAHandle->pDMAx->Channel[chnl].CCR &= DMA_INTRPT_HT_DISABLE;
        else if(interrupt == DMA_INTRPT_TE_DISABLE)
            pDMAHandle->pDMAx->Channel[chnl].CCR &= DMA_INTRPT_TE_DISABLE;
    }
}


//DMA Initialization
void DMA_init(DMA_Handle* pDMAHandle)
{
    //get channel
    uint8_t chnl = pDMAHandle->channel;

    //set direction
    if(pDMAHandle->DMAx_Config.direction == DMA_DIR_FROM_MEMORY)
        pDMAHandle->pDMAx->Channel[chnl].CCR |= 1<<DMA_CCRx_DIR;
    else if(pDMAHandle->DMAx_Config.direction == DMA_DIR_FROM_PERIPHERAL)
        pDMAHandle->pDMAx->Channel[chnl].CCR &= ~(1<<DMA_CCRx_DIR);

    //Mem to Mem Mode
    if(pDMAHandle->DMAx_Config.mem_to_mem_mode == DMA_MEM_INC_ENABLE)
        pDMAHandle->pDMAx->Channel[chnl].CCR |= 1<<DMA_CCRx_MEM2MEM;

    //Circular Mode
    if(pDMAHandle->DMAx_Config.circular_mode == DMA_CIRC_MODE_ENABLE)
        pDMAHandle->pDMAx->Channel[chnl].CCR |= 1<<DMA_CCRx_CIRC;
    
    //Peripheral Size
    pDMAHandle->pDMAx->Channel[chnl].CCR |= (pDMAHandle->DMAx_Config.peripheral_size)<<DMA_CCRx_PSIZE0;

    //Memory Size
    pDMAHandle->pDMAx->Channel[chnl].CCR |= (pDMAHandle->DMAx_Config.memory_size)<<DMA_CCRx_MSIZE0;

    //No of Data Items
    pDMAHandle->pDMAx->Channel[chnl].CNDTR = pDMAHandle->DMAx_Config.no_of_data;

    if(pDMAHandle->DMAx_Config.mem_to_mem_mode == DMA_MEM_TO_MEM_ENABLE)
    {
        pDMAHandle->pDMAx->Channel[chnl].CPAR = pDMAHandle->memory_addr_1;
        pDMAHandle->pDMAx->Channel[chnl].CMAR = pDMAHandle->memory_addr_2;
    }
    else if(pDMAHandle->DMAx_Config.mem_to_mem_mode == DMA_MEM_TO_MEM_DISABLE)
    {
        pDMAHandle->pDMAx->Channel[chnl].CPAR = pDMAHandle->peripheral_addr;
        pDMAHandle->pDMAx->Channel[chnl].CMAR = pDMAHandle->memory_addr_1;
    }
}
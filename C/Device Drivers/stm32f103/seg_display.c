#include<stdint.h>
#include"seg_display.h"

segments_t Segment_Pin;
digits_t Segment_Digit;
uint8_t digit_no;

void seg_pin_configure(SevSeg_Handle* pSEGh, segments_t* segment_pins, GPIO_RegDef* GPIO_Port)
{
	pSEGh->conn_type     = SEG_CONNECTION_TYPE;
	pSEGh->display_type  = SEG_DISPLAY_TYPE;
	pSEGh->segs          = segment_pins;
	pSEGh->Seg_GPIO_Port = GPIO_Port;
}


void seg_digit_pin_configure(SevSeg_Handle* pSEGh, digits_t* digit_pins, GPIO_RegDef* GPIO_Port)
{
	pSEGh->digits            = digit_pins;
	pSEGh->Digit_GPIO_Port   = GPIO_Port;
}


void seg_configure(SevSeg_Handle* pSEGh, segments_t* segment_pins, GPIO_RegDef* Seg_GPIO, digits_t* digit_pins, GPIO_RegDef* Digit_GPIO)
{
	seg_pin_configure(pSEGh, segment_pins, Seg_GPIO);
	
	if(SEG_DISPLAY_TYPE == SEG_MULTI_DIGIT)
		seg_digit_pin_configure(pSEGh, digit_pins, Digit_GPIO);
}


void seg_init(SevSeg_Handle* pSEGh, GPIO_Handle* pGPIOh)
{
	//pGPIOh->pGPIOx                         = GPIO_Port;
	//pGPIOh->GPIOx_PinConfig.PinMode        = GPIO_MODE_OP;
	//pGPIOh->GPIOx_PinConfig.PinOutputSpeed = GPIO_OP_SPEED_10;
	//pGPIOh->GPIOx_PinConfig.PinConfigType  = GPIO_CONFIG_GP_OP_PP;

	pSEGh->pGPIOHandle = pGPIOh;

	GPIO_Config(pSEGh->pGPIOHandle, pSEGh->Seg_GPIO_Port, GPIO_MODE_OP, GPIO_CONFIG_GP_OP_PP, pSEGh->segs->seg_A, GPIO_OP_SPEED_10);
	GPIO_Init(pSEGh->pGPIOHandle);
	
	pSEGh->pGPIOHandle->GPIOx_PinConfig.PinNo = pSEGh->segs->seg_B;
	GPIO_Init(pSEGh->pGPIOHandle);
	
	pSEGh->pGPIOHandle->GPIOx_PinConfig.PinNo = pSEGh->segs->seg_C;
	GPIO_Init(pSEGh->pGPIOHandle);
	
	pSEGh->pGPIOHandle->GPIOx_PinConfig.PinNo = pSEGh->segs->seg_D;
	GPIO_Init(pSEGh->pGPIOHandle);
	
	pSEGh->pGPIOHandle->GPIOx_PinConfig.PinNo = pSEGh->segs->seg_E;
	GPIO_Init(pSEGh->pGPIOHandle);
	
	pSEGh->pGPIOHandle->GPIOx_PinConfig.PinNo = pSEGh->segs->seg_F;
	GPIO_Init(pSEGh->pGPIOHandle);
	
	pSEGh->pGPIOHandle->GPIOx_PinConfig.PinNo = pSEGh->segs->seg_G;
	GPIO_Init(pSEGh->pGPIOHandle);
	
	pSEGh->pGPIOHandle->GPIOx_PinConfig.PinNo = pSEGh->segs->seg_H;
	GPIO_Init(pSEGh->pGPIOHandle);

	seg_pins_reset(pSEGh->Seg_GPIO_Port, pSEGh->segs);

	
	if(pSEGh->display_type == SEG_MULTI_DIGIT)
	{
		GPIO_Config(pSEGh->pGPIOHandle, pSEGh->Digit_GPIO_Port, GPIO_MODE_OP, GPIO_CONFIG_GP_OP_PP, pSEGh->digits->DIGIT_1, GPIO_OP_SPEED_10);
		GPIO_Init(pSEGh->pGPIOHandle);

		pSEGh->pGPIOHandle->GPIOx_PinConfig.PinNo = pSEGh->digits->DIGIT_2;
		GPIO_Init(pSEGh->pGPIOHandle);

		pSEGh->pGPIOHandle->GPIOx_PinConfig.PinNo = pSEGh->digits->DIGIT_3;
		GPIO_Init(pSEGh->pGPIOHandle);

		pSEGh->pGPIOHandle->GPIOx_PinConfig.PinNo = pSEGh->digits->DIGIT_4;
		GPIO_Init(pSEGh->pGPIOHandle);

		seg_digit_reset(pSEGh->Digit_GPIO_Port, pSEGh->digits);
	}
	
}


void seg_pins_reset(GPIO_RegDef* pGPIOx, segments_t* segs)
{
	if(SEG_CONNECTION_TYPE == SEG_COMMON_CATHODE)
	{
		GPIO_WriteOpPin(pGPIOx, segs->seg_A, GPIO_PIN_RESET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_B, GPIO_PIN_RESET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_C, GPIO_PIN_RESET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_D, GPIO_PIN_RESET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_E, GPIO_PIN_RESET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_F, GPIO_PIN_RESET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_G, GPIO_PIN_RESET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_H, GPIO_PIN_RESET);
	}
	else if(SEG_CONNECTION_TYPE == SEG_COMMON_ANODE)
	{
		GPIO_WriteOpPin(pGPIOx, segs->seg_A, GPIO_PIN_SET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_B, GPIO_PIN_SET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_C, GPIO_PIN_SET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_D, GPIO_PIN_SET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_E, GPIO_PIN_SET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_F, GPIO_PIN_SET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_G, GPIO_PIN_SET);
		GPIO_WriteOpPin(pGPIOx, segs->seg_H, GPIO_PIN_SET);
	}
}


void seg_digit_reset(GPIO_RegDef* pGPIOx, digits_t* digits)
{
	GPIO_WriteOpPin(pGPIOx, digits->DIGIT_1, GPIO_PIN_RESET);
	GPIO_WriteOpPin(pGPIOx, digits->DIGIT_2, GPIO_PIN_RESET);
	GPIO_WriteOpPin(pGPIOx, digits->DIGIT_3, GPIO_PIN_RESET);
	GPIO_WriteOpPin(pGPIOx, digits->DIGIT_4, GPIO_PIN_RESET);
}



void seg_display(GPIO_RegDef* pGPIOx, segments_t* segs, uint8_t character)
{
	uint16_t port_reset_value = (1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_E | 1<<segs->seg_F | 1<<segs->seg_G | 1<<segs->seg_H);
	uint16_t port_value;

	if(character == '0')
	{
		if(SEG_CONNECTION_TYPE == SEG_COMMON_CATHODE)
			port_value = (1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_E | 1<<segs->seg_F);
		else if(SEG_CONNECTION_TYPE == SEG_COMMON_ANODE)
			port_value = ((port_reset_value) & ~(1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_E | 1<<segs->seg_F));
		GPIO_WriteOpPort(pGPIOx, port_value);
	}

	if(character == '1')
	{
		if(SEG_CONNECTION_TYPE == SEG_COMMON_CATHODE)
			port_value = (1<<segs->seg_B | 1<<segs->seg_C);
		else if(SEG_CONNECTION_TYPE == SEG_COMMON_ANODE)
			port_value = ((port_reset_value) & ~(1<<segs->seg_B | 1<<segs->seg_C));
		GPIO_WriteOpPort(pGPIOx, port_value);
	}

	if(character == '2')
	{
		if(SEG_CONNECTION_TYPE == SEG_COMMON_CATHODE)
			port_value = (1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_G | 1<<segs->seg_D | 1<<segs->seg_E);
		else if(SEG_CONNECTION_TYPE == SEG_COMMON_ANODE)
			port_value = ((port_reset_value) & ~(1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_G | 1<<segs->seg_D | 1<<segs->seg_E));
		GPIO_WriteOpPort(pGPIOx, port_value);
	}

	if(character == '3')
	{
		if(SEG_CONNECTION_TYPE == SEG_COMMON_CATHODE)
			port_value = (1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_G);
		else if(SEG_CONNECTION_TYPE == SEG_COMMON_ANODE)
			port_value = ((port_reset_value) & ~(1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_G));
		GPIO_WriteOpPort(pGPIOx, port_value);
	}

	if(character == '4')
	{
		if(SEG_CONNECTION_TYPE == SEG_COMMON_CATHODE)
			port_value = (1<<segs->seg_B | 1<<segs->seg_C | 1<<segs->seg_F | 1<<segs->seg_G);
		else if(SEG_CONNECTION_TYPE == SEG_COMMON_ANODE)
			port_value = ((port_reset_value) & ~(1<<segs->seg_B | 1<<segs->seg_C | 1<<segs->seg_F | 1<<segs->seg_G));
		GPIO_WriteOpPort(pGPIOx, port_value);
	}

	if(character == '5')
	{
		if(SEG_CONNECTION_TYPE == SEG_COMMON_CATHODE)
			port_value = (1<<segs->seg_A | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_F | 1<<segs->seg_G);
		else if(SEG_CONNECTION_TYPE == SEG_COMMON_ANODE)
			port_value = (1<<digit_no) | ((port_reset_value) & ~(1<<segs->seg_A | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_F | 1<<segs->seg_G));
		GPIO_WriteOpPort(pGPIOx, port_value);
	}

	if(character == '6')
	{
		if(SEG_CONNECTION_TYPE == SEG_COMMON_CATHODE)
			port_value = (1<<segs->seg_A | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_E | 1<<segs->seg_F | 1<<segs->seg_G);
		else if(SEG_CONNECTION_TYPE == SEG_COMMON_ANODE)
			port_value = ((port_reset_value) & ~(1<<segs->seg_A | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_E | 1<<segs->seg_F | 1<<segs->seg_G));
		GPIO_WriteOpPort(pGPIOx, port_value);
	}

	if(character == '7')
	{
		if(SEG_CONNECTION_TYPE == SEG_COMMON_CATHODE)
			port_value = (1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_C);
		else if(SEG_CONNECTION_TYPE == SEG_COMMON_ANODE)
			port_value = ((port_reset_value) & ~(1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_C));
		GPIO_WriteOpPort(pGPIOx, port_value);
	}

	if(character == '8')
	{
		if(SEG_CONNECTION_TYPE == SEG_COMMON_CATHODE)
			port_value = (1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_E | 1<<segs->seg_F | 1<<segs->seg_G);
		else if(SEG_CONNECTION_TYPE == SEG_COMMON_ANODE)
			port_value = ((port_reset_value) & ~(1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_E | 1<<segs->seg_F | 1<<segs->seg_G));
		GPIO_WriteOpPort(pGPIOx, port_value);
	}

	if(character == '9')
	{
		if(SEG_CONNECTION_TYPE == SEG_COMMON_CATHODE)
			port_value = (1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_F | 1<<segs->seg_G);
		else if(SEG_CONNECTION_TYPE == SEG_COMMON_ANODE)
			port_value = ((port_reset_value) & ~(1<<segs->seg_A | 1<<segs->seg_B | 1<<segs->seg_C | 1<<segs->seg_D | 1<<segs->seg_F | 1<<segs->seg_G));
		GPIO_WriteOpPort(pGPIOx, port_value);
	}
}


void seg_digit_select(SevSeg_Handle* pSEGh, uint8_t digit_pin)
{
	seg_digit_reset(pSEGh->Digit_GPIO_Port, pSEGh->digits);
	GPIO_WriteOpPin(pSEGh->Digit_GPIO_Port, digit_pin, GPIO_PIN_SET);
}


void seg_display_digit(SevSeg_Handle* pSEGh, uint8_t character, uint8_t digit_pin)
{	
	GPIO_WriteOpPin(pSEGh->pGPIOHandle->pGPIOx, digit_pin, GPIO_PIN_SET);
	seg_display(pSEGh->pGPIOHandle->pGPIOx, pSEGh->segs, character);
}

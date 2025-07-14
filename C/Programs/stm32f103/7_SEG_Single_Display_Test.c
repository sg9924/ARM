#include<stdint.h>
#include"stm32f103xx_init.h"
#include"seg_display.h" //5161AS config header should be included inside this

SevSeg_Handle S;
GPIO_Handle G;

segments_t seg_pins = {GPIO_PIN7, GPIO_PIN9, GPIO_PIN0, GPIO_PIN5, GPIO_PIN10, GPIO_PIN8, GPIO_PIN4, GPIO_PIN1};

int main()
{
	board_init();

	seg_pin_configure(&S,&seg_pins, GPIOA);
    seg_init(&S, &G);


	char a[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	int i=0;

	//display all numbers from 0-9
	while(1)
	{	
		seg_display(S.Seg_GPIO_Port, &seg_pins, a[i]);	
		tim_delay_ms(1000);

		i=(i+1)%10;
	}
	
	return 0;
}
#include<stdint.h>
#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"
#include"seven_seg.h"


void wait_ms(uint16_t d)
{
	for(uint32_t i=0; i<(d*500); i++);
}

void wait(uint8_t d)
{
	wait_ms(d*1000);
}


int main()
{

	//GPIO Handle
	GPIO_Handle d1;
	seven_seg_init(&d1);

	char a[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	
	int i=0;
	while(1)
	{		
		seven_seg_display(&d1, a[i]);
		wait(1);
		//wait_ms(500);

		i=(i+1)%10;
	}
	
	return 0;
}
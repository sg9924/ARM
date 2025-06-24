#include<stdint.h>
#include"stm32f103xx_init.h"
#include"seven_seg.h"


int main()
{
	board_init();

	//GPIO Handle
	GPIO_Handle d1;
	seven_seg_init(&d1);

	char a[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	
	int i=0;
	while(1)
	{		
		seven_seg_display(&d1, a[i]);
		tim_delay_ms(1000);

		i=(i+1)%10;
	}
	
	return 0;
}
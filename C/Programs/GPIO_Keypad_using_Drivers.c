#include "stm32F407xx_4x4_keypad_driver.h"


int main()
{
    keypad_initialize();

    while(1)
    {
        if(row1_scan() == '1')
            printf("\nKey 1 has been pressed");
        
        if(row1_scan() == '2')
            printf("\nKey 2 has been pressed");

        if(row1_scan() == '3')
            printf("\nKey 3 has been pressed");

        if(row1_scan() == 'A')
            printf("\nKey A has been pressed");

        
        if(row2_scan() == '4')
            printf("\nKey 4 has been pressed");

        if(row2_scan() == '5')
            printf("\nKey 5 has been pressed");

        if(row2_scan() == '6')
            printf("\nKey 6 has been pressed");

        if(row2_scan() == 'B')
            printf("\nKey B has been pressed");

        
        if(row3_scan() == '7')
            printf("\nKey 7 has been pressed");
        
        if(row3_scan() == '8')
            printf("\nKey 8 has been pressed");

        if(row3_scan() == '9')
            printf("\nKey 9 has been pressed");

        if(row3_scan() == 'C')
            printf("\nKey C has been pressed");
        

        if(row3_scan() == '*')
            printf("\nKey * has been pressed");
        
        if(row3_scan() == '0')
            printf("\nKey 0 has been pressed");
        
        if(row3_scan() == '#')
            printf("\nKey # has been pressed");
        
        if(row3_scan() == 'D')
            printf("\nKey D has been pressed");
    }

    return 0;
}
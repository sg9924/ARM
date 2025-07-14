#include"stm32f103xx_init.h"
#include"seg_display.h"

SevSeg_Handle S;
GPIO_Handle G;

segments_t seg_pins = {GPIO_PIN7, GPIO_PIN9, GPIO_PIN0, GPIO_PIN5, GPIO_PIN10, GPIO_PIN8, GPIO_PIN4, GPIO_PIN1};
digits_t digit_pins = {GPIO_PIN8, GPIO_PIN5, GPIO_PIN6, GPIO_PIN7};

int main()
{
    board_init();

    seg_pin_configure(&S,&seg_pins, GPIOA);
    seg_digit_pin_configure(&S, &digit_pins, GPIOB);
    seg_init(&S, &G);

    while(1)
    {
        seg_digit_select(&S, digit_pins.DIGIT_1);
        seg_display(S.Seg_GPIO_Port, &seg_pins, '0');
        //seg_pins_reset(S.Seg_GPIO_Port &seg_pins,);
        tim_delay_ms(5);

        seg_digit_select(&S, digit_pins.DIGIT_2);
        seg_display(S.Seg_GPIO_Port, &seg_pins, '1');
        //seg_pins_reset(S.Seg_GPIO_Port &seg_pins,);
        tim_delay_ms(5);

        seg_digit_select(&S, digit_pins.DIGIT_3);
        seg_display(S.Seg_GPIO_Port, &seg_pins, '2');
        //seg_pins_reset(S.Seg_GPIO_Port &seg_pins,);
        tim_delay_ms(5);

        seg_digit_select(&S, digit_pins.DIGIT_4);
        seg_display(S.Seg_GPIO_Port, &seg_pins, '3');
        //seg_pins_reset(S.Seg_GPIO_Port &seg_pins,);
        tim_delay_ms(5);
        
    }
    return 0;
}

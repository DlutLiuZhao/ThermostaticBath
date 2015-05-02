#include"led.h"
#include"delay.h"
#include "lcd12864.h"
#include "uart.h"
#include "intrins.h"

void main()
{
	lcd_init();
	lcd_pos(0, 0);
	lcd_write_string("Œ“Ω–¡ıÓ»£∫1234°Ê");
	UART_Init();
	while (1) 
	{
		LED_ON();
		delay_ms(100);
		LED_OFF();
		delay_ms(100);
		SendData('b');
		SendData('\n');
	}
}
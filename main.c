#include"led.h"
#include"delay.h"
#include "lcd12864.h"
#include "uart.h"

void main()
{
//	lcd_init();
//	lcd_pos(0, 0);
//	lcd_write_string("ΑυξΘ");
//	lcd_pos(1, 0);
//	lcd_write_string("ΑυξΘ");
//	lcd_pos(2, 0);
//	lcd_write_string("ΑυξΘ");
//	lcd_pos(3, 0);
//	lcd_write_string("ΑυξΘ");
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
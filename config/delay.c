#include "delay.h"

void delay_ms(unsigned int xms)
{
 	unsigned int x = 0;
	unsigned int y = 0;
	for (x = 0; x < xms; ++x)
		for(y = 0; y < 100; ++y);
}

void delay(unsigned int i)
{
	while(i--);
}
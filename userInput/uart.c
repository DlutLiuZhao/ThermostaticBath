#include "uart.h"
#include <reg52.h>

#define   FOSC      8000000L
#define   BAUD      115200	//19200	57600	14400	9600

//初始化串口
void UART_Init(void)
{
	//AUXR|=T1x12;    //1T模式
	SCON |= 0x5a;                    //8 bit data ,no parity bit
    TMOD |= 0x20;                    //T1 as 8-bit auto reload
    TH1 = TL1 = -(FOSC/32/BAUD);     //Set Uart baudrate
    TR1 = 1;   	
}

void SendData(unsigned char dat)
{
      while (!TI);                        //Wait for the previous data is sent
      TI = 0;                             //Clear TI flag
      SBUF = dat;                         //Send current data
}
unsigned char ReceiveData(void)
{
	while (!RI);                        //Wait for the previous data is sent
    RI = 0;                             //Clear TI flag
   	return SBUF;                        //Send current data	
}


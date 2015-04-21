#ifndef __UART_H_
#define __UART_H_

void UART_Init(void);
void SendData(unsigned char dat);
unsigned char ReceiveData(void);

#endif

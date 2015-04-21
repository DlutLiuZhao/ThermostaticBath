#ifndef __DEFINE_H_
#define __DEFINE_H_

#include<reg52.h>
//LED
sbit LED0 = P1^0;

/* LCD 12864 Pin */
#define LCD12864_DATA P0   //并行数据口
sbit	LCD12864_RS = P3^5;  //并行的指令/数据选择信号；串行的片选信号
sbit 	LCD12864_WR = P3^6;  //并行的读写选择信号；串行的数据口
sbit 	LCD12864_EN = P3^4;  //并行的使能信号；串行的同步时钟
sbit 	LCD12864_RD = P3^7;  //并/串行接口选择； H-并行 L-串行

#endif
#ifndef __DEFINE_H_
#define __DEFINE_H_

#include<reg52.h>
//LED
sbit LED0 = P1^0;

/* LCD 12864 Pin */
#define LCD12864_DATA P0   //�������ݿ�
sbit	LCD12864_RS = P3^5;  //���е�ָ��/����ѡ���źţ����е�Ƭѡ�ź�
sbit 	LCD12864_WR = P3^6;  //���еĶ�дѡ���źţ����е����ݿ�
sbit 	LCD12864_EN = P3^4;  //���е�ʹ���źţ����е�ͬ��ʱ��
sbit 	LCD12864_RD = P3^7;  //��/���нӿ�ѡ�� H-���� L-����

#endif
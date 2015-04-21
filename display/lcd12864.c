#include "lcd12864.h"
#include "define.h"
#include "delay.h"

const char CMD_CLEAR_DISPLAY = 0X01; //����
const char CMD_BASIC_INSTRUCTION = 0X30; //����ָ�
const char CMD_EXTEND_INSTRUCTION = 0X34; //����ָ�
const char CMD_DISPLAY_ALL_ON = 0X0C; //������ʾON
const char CMD_DISPLAY_CURSOR_ON = 0X0A; //�α�ON
const char CMD_CURSOR_POSITION = 0X09; //�α�λ��ON



/*******************************************************************/
/*                                                                 */
/*���LCDæ״̬                                                    */
/*lcd_busyΪ1ʱ��æ���ȴ���lcd-busyΪ0ʱ,�У���дָ�������ݡ�      */
/*                                                                 */
/*******************************************************************/
bit lcd_busy()
 {                          
    bit result;
    LCD12864_RS = 0;
    LCD12864_WR = 1;
    LCD12864_EN = 1;
	delay_ms(2);
    result = (bit)(P0&0x80);
    LCD12864_EN = 0;
    return(result); 
 }
/*******************************************************************/
/*                                                                 */
/*������                                                    */
/*                                                                 */
/*******************************************************************/
unsigned char read_Byte(void)
{
	unsigned char read=0;
	while(lcd_busy());
    LCD12864_RS = 1;
    LCD12864_WR = 1;
	LCD12864_DATA=0xff;
    LCD12864_EN = 1;
	delay_ms(2);
	read=LCD12864_DATA;
	LCD12864_EN = 0;
	return read;
}


/*******************************************************************/
/*                                                                 */
/*дָ�����ݵ�LCD                                                  */
/*RS=L��RW=L��E=�����壬D0-D7=ָ���롣                             */
/*                                                                 */
/*******************************************************************/
void lcd_write_cmd(unsigned char cmd)
{                          
   while(lcd_busy());
    LCD12864_RS = 0;
    LCD12864_WR = 0;
    LCD12864_EN = 0;
    LCD12864_DATA = cmd;
    delay_ms(2);
    LCD12864_EN = 1;
    delay_ms(2);
    LCD12864_EN = 0;  
}
/*******************************************************************/
/*                                                                 */
/*д��ʾ���ݵ�LCD                                                  */
/*RS=H��RW=L��E=�����壬D0-D7=���ݡ�                               */
/*                                                                 */
/*******************************************************************/
void lcd_write_data(unsigned char dat)
{                          
   while(lcd_busy());
    LCD12864_RS = 1;
    LCD12864_WR = 0;
    LCD12864_EN = 0;
    LCD12864_DATA = dat;
	delay_ms(2);
    LCD12864_EN = 1;
	delay_ms(2);
    LCD12864_EN = 0; 
}
/*******************************************************************/
/*                                                                 */
/*  LCD��ʼ���趨                                                  */
/*                                                                 */
/*******************************************************************/
void lcd_init()
{ 

    LCD12864_RD = 1;         //���ڷ�ʽ 
    //lcd_write_cmd(0x34);      //����ָ�����
    //delay_ms(5);
    lcd_write_cmd(0x30);      //����ָ�����
    delay_ms(1);
    lcd_write_cmd(0x0c);      //��ʾ�����ع��
    delay_ms(5);
    lcd_write_cmd(0x01);      //���LCD����ʾ����
    delay_ms(5);
}

void clear_lcd()
{	
	unsigned char i,j;
	lcd_write_cmd(0x34);
	for(i=0;i<32;i++)			  //��ΪLCD��������32������д��ʮ����
	{
		lcd_write_cmd(0x80+i);		  //��д��������Y��ֵ
		lcd_write_cmd(0x80);		  //��д�������X��ֵ
		for(j=0;j<32;j++)		  //��������16λ��ÿλд�������ֽڵĵ����ݣ�Ҳ��д��32��
		{						  //��Ϊ��д�������ֽ�֮���������Զ���1�����ԾͲ����ٴ�д���ַ�ˡ�
			lcd_write_data(0x00);	  
		}
	}
	lcd_write_cmd(0x36);
	lcd_write_cmd(0x30);
}

/*********************************************************/
/*                                                       */
/* �趨��ʾλ��                                          */
/*                                                       */
/*********************************************************/
void lcd_pos(unsigned char X,unsigned char Y)
{                          
   unsigned char  pos;
   if (X==0)
     {X=0x80;}
   else if (X==1)
     {X=0x90;}
   else if (X==2)
     {X=0x88;}
   else if (X==3)
     {X=0x98;}
   pos = X+Y ;  
   lcd_write_cmd(pos);     //��ʾ��ַ
}
void lcd_write_string(unsigned char *s)
{
	while(*s!='\0')
	{
		lcd_write_data(*s);
		s++;	
	}
}

void display_BMP(unsigned char *address)
{
	unsigned char i=0,j=0;
	for(i=0;i<32;i++)
	{
		lcd_write_cmd(0x80+i);
		lcd_write_cmd(0x80);
		for(j=0;j<16;j++)
		{
			lcd_write_data(*address);
			address++;	
		}	
	}
	for(i=0;i<32;i++)
	{
		lcd_write_cmd(0x80+i);
		lcd_write_cmd(0x88);
		for(j=0;j<16;j++)
		{
			lcd_write_data(*address);
			address++;	
		}	
	}	
}
void Init_BMP(void)
{
	lcd_write_cmd(0x36);
	delay(1200);
	lcd_write_cmd(0x36);
	delay(210);
	lcd_write_cmd(0x3e);
	delay(1200);
	lcd_write_cmd(0x01);
	delay(1200);
}
/***********����***********/
void draw_dots(unsigned char x,unsigned char y,unsigned char color)
{
	unsigned char ROW,xlable,xlable_bit;
	unsigned char Read_H,Read_L;
	lcd_write_cmd(0x34);
	lcd_write_cmd(0x36);
	xlable=x>>4;	
	xlable_bit=x&0x0f;	//����õ���16λ�����еĵڼ�λ
	if(y<32)
	{	
		ROW=y;
	}
	else
	{
		ROW=y-32;
		xlable=xlable+8;
	}
	lcd_write_cmd(ROW+0x80);
	lcd_write_cmd(xlable+0x80);
	read_Byte();
	Read_H=read_Byte();		 	//���߰�λ 
	Read_L=read_Byte();			//���Ͱ�λ 
	lcd_write_cmd(ROW+0x80);
	lcd_write_cmd(xlable+0x80);

	if(xlable_bit<8)
	{					  
		switch(color)
		{
			case 0:Read_H&=(~(0x01<<(7-xlable_bit)));break;	//��� 
			case 1:Read_H|=(0x01<<(7-xlable_bit));   break;	//���
			case 2:Read_H^=(0x01<<(7-xlable_bit));   break;	//��ɫ
			default:break;
		}
		lcd_write_data(Read_H);
		lcd_write_data(Read_L);
	}
	else
	{
		switch(color)
		{
			case 0:Read_L&=(~(0x01<<(15-xlable_bit)));break;	//��� 
			case 1:Read_L|=(0x01<<(15-xlable_bit));   break;	//���
			case 2:Read_L^=(0x01<<(15-xlable_bit));   break;	//��ɫ
			default:break;
		}
		lcd_write_data(Read_H);
		lcd_write_data(Read_L);	
	}
	lcd_write_cmd(0x30);				
}

void draw_Straight_Line(unsigned char X_Start,unsigned char Y_Start,unsigned char X_End,unsigned char Y_End)
{
	short int X_Len,Y_Len,Row,Col;
	short int Increase_X,Increase_Y,XErr=0,YErr=0;
	short int distance;
	unsigned char i=0;
	Row=X_Start;
	Col=Y_Start;
	X_Len=X_End-X_Start;	//X��������
	Y_Len=Y_End-Y_Start;	//Y��������
	if(X_Len>0)		Increase_X=1;			
	else if(X_Len==0)	Increase_X=0;	   	//��ֱ��
	else{	Increase_X=-1;	X_Len=-X_Len;}
	if(Y_Len>0)		Increase_Y=1;			
	else if(Y_Len==0)	Increase_Y=0; 		//ˮƽ��
	else{	Increase_Y=-1;	Y_Len=-Y_Len;}
	if(X_Len>Y_Len)
		distance=X_Len;
	else
		distance=Y_Len;
		
	for(i=0;i<=distance+1;i++)
	{
		draw_dots(Row,Col,1);
		XErr+=X_Len;
		YErr+=Y_Len;
		if(XErr>distance)
		{
			XErr-=distance;
			Row+=Increase_X;
		}
		if(YErr>distance)
		{
			YErr-=distance;
			Col+=Increase_Y;
		}		
	}	
}
/*----------------------------------------------------------------------------
ʹ��Bresenham�㷨��Բ
* �������ƣ�Draw_One_Circle	
* �������ܣ���һ��Բ	
* ��ڲ�����X��Y��Բ������ R���뾶 
* ���ڲ�������
-----------------------------------------------------------------------------*/
void draw_One_Circle(unsigned char X,unsigned char Y,unsigned char R)
{
	unsigned char a,b;
	 int  d=0;
	a=0,b=R;
	d=3-(R<<1);		//������߲����ĳ�ʼֵ
	while(a<=b)
	{
		draw_dots(X+a,Y-b,1);
		draw_dots(X+a,Y+b,1);
		draw_dots(X-a,Y-b,1);
		draw_dots(X-a,Y+b,1);
		draw_dots(X+b,Y-a,1);
		draw_dots(X+b,Y+a,1);
		draw_dots(X-b,Y-a,1);
		draw_dots(X-b,Y+a,1);
		if(d<0)
			d+=4*a+6;
		else
		{
			d+=(a-b)*4+10;
			b--;
		}
		a++;			
	}
}
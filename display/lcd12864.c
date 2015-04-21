#include "lcd12864.h"
#include "define.h"
#include "delay.h"

const char CMD_CLEAR_DISPLAY = 0X01; //清屏
const char CMD_BASIC_INSTRUCTION = 0X30; //基本指令集
const char CMD_EXTEND_INSTRUCTION = 0X34; //扩充指令集
const char CMD_DISPLAY_ALL_ON = 0X0C; //整体显示ON
const char CMD_DISPLAY_CURSOR_ON = 0X0A; //游标ON
const char CMD_CURSOR_POSITION = 0X09; //游标位置ON



/*******************************************************************/
/*                                                                 */
/*检查LCD忙状态                                                    */
/*lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据。      */
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
/*读数据                                                    */
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
/*写指令数据到LCD                                                  */
/*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                             */
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
/*写显示数据到LCD                                                  */
/*RS=H，RW=L，E=高脉冲，D0-D7=数据。                               */
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
/*  LCD初始化设定                                                  */
/*                                                                 */
/*******************************************************************/
void lcd_init()
{ 

    LCD12864_RD = 1;         //并口方式 
    //lcd_write_cmd(0x34);      //扩充指令操作
    //delay_ms(5);
    lcd_write_cmd(0x30);      //基本指令操作
    delay_ms(1);
    lcd_write_cmd(0x0c);      //显示开，关光标
    delay_ms(5);
    lcd_write_cmd(0x01);      //清除LCD的显示内容
    delay_ms(5);
}

void clear_lcd()
{	
	unsigned char i,j;
	lcd_write_cmd(0x34);
	for(i=0;i<32;i++)			  //因为LCD有纵坐标32格所以写三十二次
	{
		lcd_write_cmd(0x80+i);		  //先写入纵坐标Y的值
		lcd_write_cmd(0x80);		  //再写入横坐标X的值
		for(j=0;j<32;j++)		  //横坐标有16位，每位写入两个字节的的数据，也就写入32次
		{						  //因为当写入两个字节之后横坐标会自动加1，所以就不用再次写入地址了。
			lcd_write_data(0x00);	  
		}
	}
	lcd_write_cmd(0x36);
	lcd_write_cmd(0x30);
}

/*********************************************************/
/*                                                       */
/* 设定显示位置                                          */
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
   lcd_write_cmd(pos);     //显示地址
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
/***********画点***********/
void draw_dots(unsigned char x,unsigned char y,unsigned char color)
{
	unsigned char ROW,xlable,xlable_bit;
	unsigned char Read_H,Read_L;
	lcd_write_cmd(0x34);
	lcd_write_cmd(0x36);
	xlable=x>>4;	
	xlable_bit=x&0x0f;	//计算该点在16位数据中的第几位
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
	Read_H=read_Byte();		 	//读高八位 
	Read_L=read_Byte();			//读低八位 
	lcd_write_cmd(ROW+0x80);
	lcd_write_cmd(xlable+0x80);

	if(xlable_bit<8)
	{					  
		switch(color)
		{
			case 0:Read_H&=(~(0x01<<(7-xlable_bit)));break;	//变白 
			case 1:Read_H|=(0x01<<(7-xlable_bit));   break;	//变黑
			case 2:Read_H^=(0x01<<(7-xlable_bit));   break;	//反色
			default:break;
		}
		lcd_write_data(Read_H);
		lcd_write_data(Read_L);
	}
	else
	{
		switch(color)
		{
			case 0:Read_L&=(~(0x01<<(15-xlable_bit)));break;	//变白 
			case 1:Read_L|=(0x01<<(15-xlable_bit));   break;	//变黑
			case 2:Read_L^=(0x01<<(15-xlable_bit));   break;	//反色
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
	X_Len=X_End-X_Start;	//X增量长度
	Y_Len=Y_End-Y_Start;	//Y增量长度
	if(X_Len>0)		Increase_X=1;			
	else if(X_Len==0)	Increase_X=0;	   	//垂直线
	else{	Increase_X=-1;	X_Len=-X_Len;}
	if(Y_Len>0)		Increase_Y=1;			
	else if(Y_Len==0)	Increase_Y=0; 		//水平线
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
使用Bresenham算法画圆
* 函数名称：Draw_One_Circle	
* 函数功能：画一个圆	
* 入口参数：X、Y：圆心坐标 R：半径 
* 出口参数：无
-----------------------------------------------------------------------------*/
void draw_One_Circle(unsigned char X,unsigned char Y,unsigned char R)
{
	unsigned char a,b;
	 int  d=0;
	a=0,b=R;
	d=3-(R<<1);		//计算决策参数的初始值
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
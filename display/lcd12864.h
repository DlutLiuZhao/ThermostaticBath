#ifndef __LCD12864_H_
#define __LCD12864_H_

void lcd_pos(unsigned char X,unsigned char Y);
void lcd_init();
void lcd_write_data(unsigned char dat);
void lcd_write_cmd(unsigned char cmd);
bit lcd_busy();	
unsigned char read_Byte(void);					   	
void lcd_write_string(unsigned char *s);
void delay(unsigned int i);
void Init_BMP(void);
void display_BMP(unsigned char *address);
unsigned char read_Byte(void);
void draw_dots(unsigned char x,unsigned char y,unsigned char color);
void draw_Straight_Line(unsigned char X_Start,unsigned char Y_Start,unsigned char X_End,unsigned char Y_End);
void clear_lcd();
void draw_One_Circle(unsigned char X,unsigned char Y,unsigned char R);


#endif
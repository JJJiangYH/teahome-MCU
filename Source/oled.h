#ifndef OLED_H
#define OLED_H

#include "delay.h"
#include "oledfont.h"

#define OLED_WR_CMD     0
#define OLED_WR_DAT     1
#define SIZE 16

/**********************
引脚别名定义
***********************/
sbit RES=P0^4;   
sbit DC=P0^5;         
sbit CS=P2^4;

#define  LCD_DC     DC     //OLED命令/数据选择 H=命令 L=数据
#define  LCD_CS     CS     //OLED片选

#define XLevelL        0x00
#define XLevelH        0x10
#define XLevel         ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column     128
#define Max_Row        64
#define Brightness     0xCF 
#define X_WIDTH        128
#define Y_WIDTH        64


#define SPIF 0x80 //SPSTAT.7
#define WCOL 0x40 //SPSTAT.6
#define SSIG 0x80 //SPCTL.7
#define SPEN 0x40 //SPCTL.6
#define DORD 0x20 //SPCTL.5
#define MSTR 0x10 //SPCTL.4
#define CPOL 0x08 //SPCTL.3
#define CPHA 0x04 //SPCTL.2
#define SPDHH 0x00 //CPU_CLK/4
#define SPDH 0x01 //CPU_CLK/8
#define SPDL 0x02 //CPU_CLK/16
#define SPDLL 0x03 //CPU_CLK/32


extern  void OLED_Init(void);
extern  void delay_ms(uint16 x); 
extern  void OLED_Fill(uint8 y, uint8 dat);
extern  void OLED_ShowChar(uint8 x,uint8 y,uint8 chr);
extern  void OLED_Set_Pos(uint8 x, uint8 y);
extern  void LCD_P8x16Str(uint8 x, uint8 y,uint8 ch[]);
extern	void OLED_Fill_Row(uint8 y, uint8 dat);

#endif

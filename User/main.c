/****************************************Copyright (c)****************************************************
**                                        
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			
** Last modified Date:          
** Last Version:		   
** Descriptions:							
**--------------------------------------------------------------------------------------------------------
**--------------------------------------------------------------------------------------------------------
** Modified by:			JiangYuHang
** Modified date:		2021-3-28
** Version:				  1.0
** Descriptions:	  
** Rechecked by:		
**********************************************************************************************************/

#include	"uart.h"	
#include	"ds18b20.h" 
#include 	"oled.h"
#include	"15W4KxxS4.h"
#include	"oledfont.h"
#include  "delay.h"
#include  "timeControl.h"	
#include "Math.h"
#include	"led.h"

uint8 state_all=0;

/***************************************************************************
 * 描  述 : 主函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************/
int main() {
	uint8 i = 0;
	uint8 lastTime = 0;
	uint8 lastTem[6];
	uint8 isEquals = 1;
	int conTemp = 0;
	int boiled = 0;
	uint16 ht = 0;
	uint8 ht_char[7];

	memset(lastTem, 0, 6);
	memset(ht_char, 0, 7);

	//时钟模块初始化
	P3M1 &= 0xFC;	P3M0 &= 0xFC;	                  //设置P3.0~P3.1为准双向口

	Uart1_Init();                                 //串口1初始化 	
	Uart4_Init();         												//串口4初始化
	EA = 1;                                       //使能总中断
	delay_ms(10);                                 //初始化后延时

	PCF8563_Init();                               //PCF8563时钟初始化 

	//温度初始化
	P3M1 &= 0xBF;	P3M0 &= 0xBF;	                  //设置P3.6为准双向口

	DATA_PIN = 1;                                   //总线拉高 
	delay_ms(10);                                 //初始化后延时

	//OLED初始化
	P0M1 &= 0xCF;	P0M0 &= 0xCF;	     //设置P0.4,P0.5为准双向口
	P2M1 &= 0xE5;	P2M0 &= 0xE5;	       //设置P2.1,P2.3，P2.4为准双向口 
	RES = 0;                            //对OLED屏复位引脚的控制，低电平有效
	delay_ms(500);                    //此延时不可少，是用于复位延时用
	RES = 1;                            //对OLED屏复位引脚的控制
	OLED_Init();                      //对OLED屏初始化  
	OLED_Fill(0, 0x00);                  //清屏 

	P0M1 &= 0x3F;	P0M0 &= 0x3F;	 //设置P0.6、P0.7为准双向口
	P5M1 &= 0xF3;	P5M0 &= 0xF3;
	
	//加热次数
	sprintf(ht_char, "HT:%02d", ht);
	LCD_P8x16Str(44, 6, ht_char);
	
	while (1) {
		DS18B20_TEST();       //测试温度并串口打印
		isEquals = 1;
		for (i = 0; i < 7; i++) {
			if (lastTem[i] != temp[i]) isEquals = 0;
		}
		LCD_P8x16Str(4, 2, "TempNow");
		if (isEquals == 0) {
			OLED_Fill_Row(4, 0x00);
			OLED_Fill_Row(5, 0x00);
			LCD_P8x16Str(12, 4, temp);
			for (i = 0; i < 6; i++) {
				lastTem[i] = temp[i];
			}
			isEquals = 1;
		}

		//设置恒温
		sprintf(constantTemp, "%03d", ctemp);

		LCD_P8x16Str(4 + 64, 2, "ConTemp");
		LCD_P8x16Str(64 + 20, 4, constantTemp);

		//显示状态
		OLED_Fill_Row(0, 0x00);
		OLED_Fill_Row(1, 0x00);
		if (state_all == 1) {
			LCD_P8x16Str(32, 0, "State:ON");
		}
		else {
			LCD_P8x16Str(27, 0, "State:OFF");
		}
		
		delay_ms(1000);

		HC08_Tx_Puts();   //单片机接收到HC08控制命令后，驱动指示灯动作，并串口返回给HC08模块
	}
}
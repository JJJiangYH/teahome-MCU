#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "delay.h"	
#include <string.h>           // 加入此头文件后,可使用strstr库函数
#include <stdio.h>
#include <intrins.H>

/**********************
引脚别名定义
***********************/	
sbit DATA_PIN=P3^6;       //DS18B20数据引脚，该引脚必须接上拉电阻

extern uint8 temp[6];
extern uint8 constantTemp[3];
extern float fTemp;

extern float floatReadDs18B20(void);    //温度传感器读取    
extern void DS18B20_TEST(void);         //串口实时显示温度

#endif 
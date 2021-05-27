#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "delay.h"	
#include <string.h>           // �����ͷ�ļ���,��ʹ��strstr�⺯��
#include <stdio.h>
#include <intrins.H>

/**********************
���ű�������
***********************/	
sbit DATA_PIN=P3^6;       //DS18B20�������ţ������ű������������

extern uint8 temp[6];
extern uint8 constantTemp[3];
extern float fTemp;

extern float floatReadDs18B20(void);    //�¶ȴ�������ȡ    
extern void DS18B20_TEST(void);         //����ʵʱ��ʾ�¶�

#endif 